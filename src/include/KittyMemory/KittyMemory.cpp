//
//  KittyMemory.cpp
//
//  Created by MJ (Ruit) on 1/1/19.
//

#include <unistd.h>
#include <android/log.h>

#include "KittyMemory.hpp"

using KittyMemory::Memory_Status;
using KittyMemory::ProcMap;

struct mapsCache {
    std::string identifier;
    ProcMap map;
};

static std::vector<mapsCache> __mapsCache;
static ProcMap findMapInCache(std::string id){
    ProcMap ret;
    for(int i = 0; i < __mapsCache.size(); i++){
        if(__mapsCache[i].identifier.compare(id) == 0){
            ret = __mapsCache[i].map;
            break;
        }
    }
    return ret;
}

int KittyMemory::GetMemoryPermission(void* address) {
    unsigned long addr = (unsigned long)address;
    char line[PATH_MAX] = {0};

    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp == nullptr)
        return 0;

    char perms[5];
    int bol = 1;
    while (fgets(line, PATH_MAX, fp) != nullptr) {
        unsigned long start, end;
        int eol = (strchr(line, '\n') != nullptr);
        if (bol) {
            if (!eol) {
                bol = 0;
            }
        } else {
            if (eol) {
                bol = 1;
            }

            continue;
        }

        if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) != 3)
            continue;

        if (start <= addr && addr < end) {
            int prot = 0;
            if (perms[0] == 'r') {
                prot |= PROT_READ;
            } else if (perms[0] != '-') {
                goto unknown_perms;
            }

            if (perms[1] == 'w') {
                prot |= PROT_WRITE;
            } else if (perms[1] != '-') {
                goto unknown_perms;
            }

            if (perms[2] == 'x') {
                prot |= PROT_EXEC;
            } else if (perms[2] != '-') {
                goto unknown_perms;
            }

            if (perms[3] != 'p') {
                goto unknown_perms;
            }

            if (perms[4] != '\0') {
                perms[4] = '\0';
                goto unknown_perms;
            }

            fclose(fp);
            return prot;
        }
    }

unknown_perms:
    fclose(fp);
    return 0;
}

bool KittyMemory::ProtectAddr(void *addr, size_t length, int protection) {
    uintptr_t pageStart = _PAGE_START_OF_(addr);
    uintptr_t pageLen   = _PAGE_LEN_OF_(addr, length);
    return (mprotect(reinterpret_cast<void *>(pageStart), pageLen, protection) != -1);
}

Memory_Status KittyMemory::memWrite(void *addr, const void *buffer, size_t len) {
    if (addr == nullptr)
        return INV_ADDR;

    if (buffer == nullptr)
        return INV_BUF;

    if (len < 1 || len > INT_MAX)
        return INV_LEN;

    int prot = GetMemoryPermission(addr);
    if (prot == 0) {
        return INV_PROT;
    }

    if (!(prot & PROT_WRITE)) {
        if (!ProtectAddr(addr, len, _PROT_RWX_))
            return INV_PROT;
    }

    if (memcpy(addr, buffer, len) != nullptr) {
        if (!(prot & PROT_WRITE)) {
            if (!ProtectAddr(addr, len, prot))
                return INV_PROT;
        }

        return SUCCESS;
    }

    return FAILED;
}

Memory_Status KittyMemory::memRead(void *buffer, const void *addr, size_t len) {
    if (addr == nullptr)
        return INV_ADDR;

    if (buffer == nullptr)
        return INV_BUF;

    if (len < 1 || len > INT_MAX)
        return INV_LEN;

    if (memcpy(buffer, addr, len) != nullptr)
        return SUCCESS;

    return FAILED;
}

std::string KittyMemory::read2HexStr(const void *addr, size_t len) {
    char temp[len];
    memset(temp, 0, len);
	
    const size_t bufferLen = len * 2 + 1;
    char buffer[bufferLen];
    memset(buffer, 0, bufferLen);

    std::string ret;

    if (memRead(temp, addr, len) != SUCCESS)
        return ret;

    for (int i = 0; i < len; i++) {
        sprintf(&buffer[i * 2], "%02X", (unsigned char) temp[i]);
    }

    ret += buffer;
    return ret;
}

Memory_Status KittyMemory::makeNOP(void *ptr, size_t len) {
    if (ptr == nullptr)
        return INV_ADDR;

    if (len < 1 || len > INT_MAX)
        return INV_LEN;

    if (!ProtectAddr(ptr, len, _PROT_RWX_))
        return INV_PROT;

    uintptr_t finalPtr = reinterpret_cast<uintptr_t>(ptr);
    for (uintptr_t ptr = finalPtr; ptr != (finalPtr + (len * 2)); ptr += 2) {
#if defined(__aarch64__)
        /* ~ */
#elif defined(__arm__)
        *(char*)ptr = 0x00;
        *(char*)(ptr+1) = 0x46;
#endif
    }

    if (!ProtectAddr(ptr, len, _PROT_RX_))
        return INV_PROT;

    return SUCCESS;
}

ProcMap KittyMemory::getLibraryMap(const char *libraryName) {
    ProcMap retMap;
    char line[PATH_MAX] = {0};

    FILE *fp = fopen("/proc/self/maps", "rt");
    if (fp != nullptr) {
        while (fgets(line, PATH_MAX, fp)) {
            if (strstr(line, libraryName)) {
                char tmpPerms[5] = {0}, tmpDev[12] = {0}, tmpPathname[444] = {0};
                // parse a line in maps file
                // (format) startAddress-endAddress perms offset dev inode pathname
                sscanf(line, "%llx-%llx %s %ld %s %d %s",
                       (long long unsigned *) &retMap.startAddr,
                       (long long unsigned *) &retMap.endAddr,
                       tmpPerms, &retMap.offset, tmpDev, &retMap.inode, tmpPathname);

                retMap.length = (uintptr_t) retMap.endAddr - (uintptr_t) retMap.startAddr;
                retMap.perms = tmpPerms;
                retMap.dev = tmpDev;
                retMap.pathname = tmpPathname;

                break;
            }
        }
        fclose(fp);
    }
    return retMap;
}

uintptr_t KittyMemory::getAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr, bool useCache) {
    ProcMap libMap;

    if (useCache){
        libMap = findMapInCache(libraryName);
        if (libMap.isValid())
            return (reinterpret_cast<uintptr_t>(libMap.startAddr) + relativeAddr);
    }
       
    libMap = getLibraryMap(libraryName);
    if (!libMap.isValid())
        return 0;

    if (useCache){
        mapsCache cachedMap;
        cachedMap.identifier = libraryName;
        cachedMap.map        = libMap;
        __mapsCache.push_back(cachedMap);
    }

    return (reinterpret_cast<uintptr_t>(libMap.startAddr) + relativeAddr);
}

uintptr_t KittyMemory::getAbsoluteAddress(ProcMap libMap, uintptr_t relativeAddr) {
    if (!libMap.isValid())
        return 0;

    return (reinterpret_cast<uintptr_t>(libMap.startAddr) + relativeAddr);
}
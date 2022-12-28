//
//  KittyMemory.cpp
//
//  Created by MJ (Ruit) on 1/1/19.
//

#include <climits>
#include <cstring>

#if USE_DOBBY_CODE_PATCH
#include <dobby.h>
#endif

#include "KittyMemory.h"

namespace KittyMemory {
    int setAddressProtection(void* address, size_t length, int protection, int* oldProtect)
    {
        uintptr_t pageStart = _PAGE_START_OF_(address, _SYS_PAGE_SIZE_);
        uintptr_t pageLen = _PAGE_LEN_OF_(address, length, _SYS_PAGE_SIZE_);

        if (oldProtect) {
            ProcMap addressMap = getAddressMap(reinterpret_cast<void*>(pageStart));
            *oldProtect = addressMap.protection;
        }

        int ret = mprotect(reinterpret_cast<void*>(pageStart), pageLen, protection);
        KITTY_LOGI("mprotect(%p, %zu, %d) = %d", address, length, protection, ret);
        return ret;
    }

    bool memWrite(void* address, const void* buffer, size_t len)
    {
        KITTY_LOGI("memWrite(%p, %p, %zu)", address, buffer, len);

        if (!address) {
            KITTY_LOGE("memWrite err address (%p) is null", address);
            return false;
        }

        if (!buffer) {
            KITTY_LOGE("memWrite err buffer (%p) is null", buffer);
            return false;
        }

        if (len < 1 || len > INT_MAX) {
            KITTY_LOGE("memWrite err invalid len (%zu) < 1 || > INT_MAX", len);
            return false;
        }

#ifdef USE_DOBBY_CODE_PATCH
        if (DobbyCodePatch(addr, (uint8_t*) buffer, len) != kMemoryOperationSuccess) {
            return false;
        }

        return true;
#else
        ProcMap addressMap = getAddressMap(address);
        if (!addressMap.isValid()) {
            KITTY_LOGE("memWrite err couldn't find address (%p) in any map", address);
            return false;
        }

        if (addressMap.protection & PROT_WRITE) {
            uintptr_t pageStart = _PAGE_START_OF_(address, _SYS_PAGE_SIZE_);
            uintptr_t pageOffset = _PAGE_OFFSET_OF_(address, _SYS_PAGE_SIZE_);

            memcpy((void*) (pageStart + pageOffset), buffer, len);

            uintptr_t clearStart = pageStart + pageOffset;
#if defined(__clang__)
            __clear_cache((void*) clearStart, (void*) (clearStart + len));
#elif defined(__GNUC__) || defined(__GNUG__)
            __builtin___clear_cache((void*) clearStart, (void*) (clearStart + len));
#endif
            return true;
        }

        if (setAddressProtection(address, len, addressMap.protection | PROT_WRITE) != 0) {
            KITTY_LOGE("memWrite err couldn't add write perm to address (%p, len: %zu, prot: %d)",
                       address, len, addressMap.protection);
            return false;
        }

        uintptr_t pageStart = _PAGE_START_OF_(address, _SYS_PAGE_SIZE_);
        uintptr_t pageOffset = _PAGE_OFFSET_OF_(address, _SYS_PAGE_SIZE_);

        memcpy((void*) (pageStart + pageOffset), buffer, len);

        uintptr_t clearStart = pageStart + pageOffset;
#if defined(__clang__)
        __clear_cache((void*) clearStart, (void*) (clearStart + len));
#elif defined(__GNUC__) || defined(__GNUG__)
        __builtin___clear_cache((void*) clearStart, (void*) (clearStart + len));
#endif

        if (setAddressProtection(address, len, addressMap.protection) != 0) {
            KITTY_LOGE("memWrite err couldn't revert protection of address (%p, len: %zu, prot: %d)",
                       address, len, addressMap.protection);
            return false;
        }

        return true;
#endif
    }

    bool memRead(void* buffer, const void* address, size_t len)
    {
        KITTY_LOGI("memRead(%p, %p, %zu)", buffer, address, len);

        if (!address) {
            KITTY_LOGE("memRead err address (%p) is null", address);
            return false;
        }

        if (!buffer) {
            KITTY_LOGE("memRead err buffer (%p) is null", buffer);
            return false;
        }

        if (len < 1 || len > INT_MAX) {
            KITTY_LOGE("memRead err invalid len (%zu) < 1 || > INT_MAX", len);
            return false;
        }

        uintptr_t pageStart = _PAGE_START_OF_(address, _SYS_PAGE_SIZE_);
        uintptr_t pageOffset = _PAGE_OFFSET_OF_(address, _SYS_PAGE_SIZE_);

        memcpy(buffer, (void*) (pageStart + pageOffset), len);

        uintptr_t clearStart = pageStart + pageOffset;
#if defined(__clang__)
        __clear_cache((void*) clearStart, (void*) (clearStart + len));
#elif defined(__GNUC__) || defined(__GNUG__)
        __builtin___clear_cache((void*) clearStart, (void*) (clearStart + len));
#endif
        return true;
    }

    std::string read2HexStr(const void* address, size_t len)
    {
        std::string temp(len, ' ');
        if (!memRead(&temp[0], address, len)) {
            return "";
        }

        std::string ret(len * 2, ' ');
        for (int i = 0; i < len; i++) {
            sprintf(&ret[i * 2], "%02X", (unsigned char) temp[i]);
        }

        return ret;
    }

    bool makeNOP(void* address, size_t len, bool thumb)
    {
        if (!address) {
            KITTY_LOGE("makeNOP err address (%p) is null", address);
            return false;
        }

        if (len < 1 || len > INT_MAX) {
            KITTY_LOGE("makeNOP err invalid len (%zu) < 1 || > INT_MAX", len);
            return false;
        }

        auto finalAddr = (uintptr_t) address;
        for (uintptr_t currentAddr = finalAddr;
             currentAddr != (finalAddr + (len * (thumb ? 2 : 4))); currentAddr += (thumb ? 2 : 4)) {
#ifdef __arm__
            if (thumb)
                memWrite((void*) currentAddr, "\x00\xBF", 2);
            else
                memWrite((void*) currentAddr, "\x00\xF0\x20\xE3", 4);
#elif __aarch64__
            memWrite((void*) currentAddr, "\x1F\x20\x03\xD5", 4);
#elif defined(__i386__) || defined(__x86_64__)
            memWrite((void*) currentAddr, "\x90", 1);
#endif
        }

        return true;
    }

    bool makeRET(void* address, bool thumb)
    {
        if (!address) {
            KITTY_LOGE("makeRET err address (%p) is null", address);
            return false;
        }

#ifdef __arm__
        if (thumb)
            memWrite(address, "\x70\x47", 2); // BX LR
        else
            memWrite(address, "\x1E\xFF\x2F\xE1", 4); // BX LR
#elif __aarch64__
        memWrite(address, "\xC0\x03\x5F\xD6", 4); // RET
#elif defined(__i386__) || defined(__x86_64__)
        memWrite(address, "\xC3", 1); // RET
#endif

        return true;
    }

    std::vector<ProcMap> getAllMaps() {
        std::vector<ProcMap> retMaps;
        char line[512] = {0};

        FILE *fp = fopen("/proc/self/maps", "r");
        if (fp) {
            while (fgets(line, sizeof(line), fp)) {
                ProcMap map;

                char perms[5] = {0}, dev[11] = {0}, pathname[256] = {0};
                // parse a line in maps file
                // (format) startAddress-endAddress perms offset dev inode pathname
                sscanf(line, "%llx-%llx %s %llx %s %lu %s",
                       &map.startAddress, &map.endAddress,
                       perms, &map.offset, dev, &map.inode, pathname);

                map.length = map.endAddress - map.startAddress;
                map.dev = dev;
                map.pathname = pathname;

                if (perms[0] == 'r')
                {
                    map.protection |= PROT_READ;
                    map.readable = true;
                }
                if (perms[1] == 'w')
                {
                    map.protection |= PROT_WRITE;
                    map.writeable = true;
                }
                if (perms[2] == 'x')
                {
                    map.protection |= PROT_EXEC;
                    map.executable = true;
                }

                map.is_private = (perms[3] == 'p');
                map.is_shared = (perms[3] == 's');

                map.is_rx = (strncmp(perms, "r-x", 3) == 0);
                map.is_rw = (strncmp(perms, "rw-", 3) == 0);
                map.is_ro = (strncmp(perms, "r--", 3) == 0);

                retMaps.push_back(map);
            }
            fclose(fp);
        }

        if (retMaps.empty()) {
            KITTY_LOGE("getAllMaps err couldn't find any map");
        }
        return retMaps;
    }

    std::vector<ProcMap> getMapsByName(const std::string &name) {
        if (name.empty()) return {};

        KITTY_LOGI("getMapsByName(%s)", name.c_str());

        std::vector<ProcMap> retMaps;
        char line[512] = {0};

        FILE *fp = fopen("/proc/self/maps", "r");
        if (fp) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, name.c_str())) {
                    ProcMap map;

                    char perms[5] = {0}, dev[11] = {0}, pathname[256] = {0};
                    // parse a line in maps file
                    // (format) startAddress-endAddress perms offset dev inode pathname
                    sscanf(line, "%llx-%llx %s %llx %s %lu %s",
                           &map.startAddress, &map.endAddress,
                           perms, &map.offset, dev, &map.inode, pathname);

                    map.length = map.endAddress - map.startAddress;
                    map.dev = dev;
                    map.pathname = pathname;

                    if (perms[0] == 'r')
                    {
                        map.protection |= PROT_READ;
                        map.readable = true;
                    }
                    if (perms[1] == 'w')
                    {
                        map.protection |= PROT_WRITE;
                        map.writeable = true;
                    }
                    if (perms[2] == 'x')
                    {
                        map.protection |= PROT_EXEC;
                        map.executable = true;
                    }

                    map.is_private = (perms[3] == 'p');
                    map.is_shared = (perms[3] == 's');

                    map.is_rx = (strncmp(perms, "r-x", 3) == 0);
                    map.is_rw = (strncmp(perms, "rw-", 3) == 0);
                    map.is_ro = (strncmp(perms, "r--", 3) == 0);

                    retMaps.push_back(map);
                    // KITTY_LOGI("getMapsByName [%llx-%llx %s %llx %s %lu %s]",
                    // map.startAddress, map.endAddress, perms, map.offset,
                    // map.dev.empty() ? "null" : map.dev.c_str(),
                    // map.inode, map.pathname.empty() ? "null" : map.pathname.c_str());
                }
            }
            fclose(fp);
        }

        if (retMaps.empty()) {
            KITTY_LOGE("getMapsByName err couldn't find any map with name (%s)", name.c_str());
        }
        return retMaps;
    }

    ProcMap getAddressMap(const void *address) {
        KITTY_LOGI("getAddressMap(%p)", address);

        if (!address) return {};

        ProcMap map;
        char line[512] = {0};
        unsigned long long startAddress = 0, endAddress = 0;

        FILE *fp = fopen("/proc/self/maps", "r");
        if (fp) {
            while (fgets(line, sizeof(line), fp)) {
                sscanf(line, "%llx-%llx", &startAddress, &endAddress);
                if ((uintptr_t)address >= startAddress && (uintptr_t)address <= endAddress) {

                    char perms[5] = {0}, dev[11] = {0}, pathname[256] = {0};
                    // parse a line in maps file
                    // (format) startAddress-endAddress perms offset dev inode pathname
                    sscanf(line, "%*llx-%*llx %s %llx %s %lu %s",
                           perms, &map.offset, dev, &map.inode, pathname);

                    map.startAddress = startAddress;
                    map.endAddress = endAddress;
                    map.length = map.endAddress - map.startAddress;
                    map.dev = dev;
                    map.pathname = pathname;

                    if (perms[0] == 'r')
                    {
                        map.protection |= PROT_READ;
                        map.readable = true;
                    }
                    if (perms[1] == 'w')
                    {
                        map.protection |= PROT_WRITE;
                        map.writeable = true;
                    }
                    if (perms[2] == 'x')
                    {
                        map.protection |= PROT_EXEC;
                        map.executable = true;
                    }

                    map.is_private = (perms[3] == 'p');
                    map.is_shared = (perms[3] == 's');

                    map.is_rx = (strncmp(perms, "r-x", 3) == 0);
                    map.is_rw = (strncmp(perms, "rw-", 3) == 0);
                    map.is_ro = (strncmp(perms, "r--", 3) == 0);

                    // KITTY_LOGI("Address (%p) map = (%llx-%llx %s %llx %s %lu %s)", address,
                    // map.startAddress, map.endAddress, perms[0] == 0 ? "null" : perms,
                    // map.offset, map.dev.empty() ? "null" : map.dev.c_str(),
                    // map.inode, map.pathname.empty() ? "null" : map.pathname.c_str());

                    break;
                }
            }
            fclose(fp);
        }

        if (!map.isValid()) {
            KITTY_LOGE("getAddressMap err couldn't find any map with address (%p)",
                       address);
        }
        return map;
    }

    ProcMap getLibraryBaseMap(const std::vector<ProcMap>& maps) {
        ProcMap retMap{};

        if (maps.empty())
            return retMap;

        for (auto &it : maps)
        {
            if (!it.isValid() || it.writeable || !it.is_private) continue;

            if (memcmp((const void *)it.startAddress, "\177ELF", 4) == 0)
            {
                retMap = it;
                // sometimes both r--p and r-xp could have a valid elf header,
                // don't break here because we need the last map with a valid elf header.
            }
        }
        return retMap;
    }

    ProcMap getLibraryBaseMap(const std::string& name) {
        return getLibraryBaseMap(getMapsByName(name));
    }
}

//
//  KittyMemory.hpp
//
//  Created by MJ (Ruit) on 1/1/19.
//

#pragma once

#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>

#define _SYS_PAGE_SIZE_         (sysconf(_SC_PAGE_SIZE))

#define _PAGE_START_OF_(x)      ((uintptr_t)x & ~(uintptr_t)(_SYS_PAGE_SIZE_ - 1))
#define _PAGE_END_OF_(x, len)   (_PAGE_START_OF_((uintptr_t)x + len - 1))
#define _PAGE_LEN_OF_(x, len)   (_PAGE_END_OF_(x, len) - _PAGE_START_OF_(x) + _SYS_PAGE_SIZE_)
#define _PAGE_OFFSET_OF_(x)     ((uintptr_t)x - _PAGE_START_OF_(x))

#define _PROT_RWX_              (PROT_READ | PROT_WRITE | PROT_EXEC)
#define _PROT_RW_               (PROT_READ | PROT_WRITE)
#define _PROT_WX_               (PROT_WRITE | PROT_EXEC)
#define _PROT_RX_               (PROT_READ | PROT_EXEC)

#define EMPTY_VEC_OFFSET        std::vector<int>()

namespace KittyMemory {
    typedef enum {
        FAILED = 0,
        SUCCESS = 1,
        INV_ADDR = 2,
        INV_LEN = 3,
        INV_BUF = 4,
        INV_PROT = 5
    } Memory_Status;

    struct ProcMap {
        void *startAddr;
        void *endAddr;
        size_t length;
        std::string perms;
        long offset;
        std::string dev;
        int inode;
        std::string pathname;

        bool isValid() { return (startAddr != NULL && endAddr != NULL && !pathname.empty()); }
    };

    /*
     * Gets memory permission of address (PROT_READ, PROT_WRITE, PROT_EXEC)
     */
    int GetMemoryPermission(void *address);

    /*
     * Changes protection of an address with given length
     */
    bool ProtectAddr(void *addr, size_t length, int protection);

    /*
     * Writes buffer content to an address
     */
    Memory_Status memWrite(void *addr, const void *buffer, size_t len);

    /*
     * Reads an address content into a buffer
     */
    Memory_Status memRead(void *buffer, void *addr, size_t len);

    /*
     * Reads an address content and returns hex string
     */
    std::string read2HexStr(void *addr, size_t len);

    /*
     *
     */
    Memory_Status makeNOP(void *ptr, size_t len=2);

    /*
     * Wrapper to dereference & get value of a multi level pointer
     * Make sure to use the correct data type!
     */
    template<typename Type>
    Type readMultiPtr(void *ptr, std::vector<int> offsets) {
        Type defaultVal = {};
        if (ptr == nullptr)
            return defaultVal;

        uintptr_t finalPtr = reinterpret_cast<uintptr_t>(ptr);
        int offsetsSize = offsets.size();
        if (offsetsSize > 0) {
            for (int i = 0; finalPtr != 0 && i < offsetsSize; i++) {
                if (i == (offsetsSize - 1))
                    return *reinterpret_cast<Type *>(finalPtr + offsets[i]);

                finalPtr = *reinterpret_cast<uintptr_t *>(finalPtr + offsets[i]);
            }
        }

        if (finalPtr == 0)
            return defaultVal;

        return *reinterpret_cast<Type *>(finalPtr);
    }

    /*
     * Wrapper to dereference & set value of a multi level pointer
     * Make sure to use the correct data type!, const objects won't work
     */
    template<typename Type>
    bool writeMultiPtr(void *ptr, std::vector<int> offsets, Type val) {
        if (ptr == nullptr)
            return false;

        uintptr_t finalPtr = reinterpret_cast<uintptr_t>(ptr);
        int offsetsSize = offsets.size();
        if (offsetsSize > 0) {
            for (int i = 0; finalPtr != 0 && i < offsetsSize; i++) {
                if (i == (offsetsSize - 1)) {
                    *reinterpret_cast<Type *>(finalPtr + offsets[i]) = val;
                    return true;
                }

                finalPtr = *reinterpret_cast<uintptr_t *>(finalPtr + offsets[i]);
            }
        }

        if (finalPtr == 0)
            return false;

        *reinterpret_cast<Type *>(finalPtr) = val;
        return true;
    }

	/*
     * Wrapper to dereference & get value of a pointer
     * Make sure to use the correct data type!
     */
    template<typename Type>
    Type readPtr(void *ptr) {
        Type defaultVal = {};
        if (ptr == nullptr)
            return defaultVal;

        return *reinterpret_cast<Type *>(ptr);
    }

	/*
     * Wrapper to dereference & set value of a pointer
     * Make sure to use the correct data type!, const objects won't work
     */
    template<typename Type>
    bool writePtr(void *ptr, Type val) {
        if (ptr == nullptr)
            return false;

        *reinterpret_cast<Type *>(ptr) = val;
        return true;
    }

    /*
     *
     */
    template <typename Type, typename...Type2>
    Type callFunction(void *ptr, Type2...args) {
        if (ptr == nullptr)
            return Type();

        return reinterpret_cast<Type(__cdecl *)(Type2...)>(ptr)(args...);
    }
	
    /*
     * Gets info of a mapped library in self process
     */
    ProcMap getLibraryMap(const char *libraryName);

    /*
     * Expects a relative address in a library
     * Returns final absolute address
     */
    uintptr_t getAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr, bool useCache=false);

    /*
     * Same as above but no need libraryName just need ProcMap you loaded from getLibraryMap
     * Returns final absolute address
     */
    uintptr_t getAbsoluteAddress(ProcMap libMap, uintptr_t relativeAddr);

    /*
     * Comparing data with pattern
     * Returns false if data not match the pattern
     */
    /*bool matchPattern(const char* data, const char* pattern) {
        bool found = false;
        for (; *data; ++data, ++pattern) {
            if (*data == *pattern || *pattern == '?') {
                found = true;
            }
            else {
                found = false;
                break;
            }
        }

        return found;
    }*/

    /*
     * Scan all address signatures according to the pattern
     * Returns 0 if none of the signature addresses match the pattern
     */
    template <typename Type = void*>
    Type patternScan(const size_t start, const size_t end, const char* pattern, const intptr_t offset = 0) {
        Type ret = Type(0x0);

        /*int prot = GetMemoryPermission(reinterpret_cast<void*>(start));
        if (!ProtectAddr(reinterpret_cast<void*>(start), end - start, _PROT_RWX_)) {
            return ret;
        }*/

        /*if (start > 0 && end > 0 && pattern) {
            for (size_t i = 0; i <= end - start; i++) {
                if (matchPattern(reinterpret_cast<char* >(start + i), pattern)) {
                    ret = Type(i + offset);
                }
            }
        }*/

        // ProtectAddrReal(reinterpret_cast<void*>(start), end - start, prot);

        return ret;
    }

    /*
     * Scan all address signatures according to the pattern
     * Returns 0 if none of the signature addresses match the pattern
     */
    template <typename Type = void*>
    Type patternScan(const ProcMap map, const char* pattern, const intptr_t offset = 0) {
        size_t start = reinterpret_cast<size_t>(map.startAddr);
        size_t end = reinterpret_cast<size_t>(map.endAddr);

        return patternScan<Type>(start, end, pattern, offset);
    }
}

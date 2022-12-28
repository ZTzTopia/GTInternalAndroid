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

#define _SYS_PAGE_SIZE_ (sysconf(_SC_PAGE_SIZE))

#define _PAGE_START_OF_(x, range) ((uintptr_t)x & ~(uintptr_t)(range - 1))
#define _PAGE_END_OF_(x, len, range) (_PAGE_START_OF_((uintptr_t)x + len - 1, range))
#define _PAGE_LEN_OF_(x, len, range) (_PAGE_END_OF_(x, len, range) - _PAGE_START_OF_(x, range) + range)
#define _PAGE_OFFSET_OF_(x, range) ((uintptr_t)x - _PAGE_START_OF_(x, range))

#define EMPTY_VEC_OFFSET std::vector<int>()

#if defined(__ANDROID__) || defined(__linux__)
#define _PROT_RWX_ (PROT_READ | PROT_WRITE | PROT_EXEC)
#define _PROT_RW_ (PROT_READ | PROT_WRITE)
#define _PROT_RX_ (PROT_READ | PROT_EXEC)
#endif

#ifdef KITTYMEMORY_DEBUG
#include <android/log.h>
#define KITTY_LOGI(...) ((void)__android_log_print(4, "KittyMemory", __VA_ARGS__))
#define KITTY_LOGE(...) ((void)__android_log_print(6, "KittyMemory", __VA_ARGS__))
#else
#define KITTY_LOGI(...)
#define KITTY_LOGE(...)
#endif

namespace KittyMemory {
    struct ProcMap {
        unsigned long long startAddress;
        unsigned long long endAddress;
        size_t length;
        int protection;
        bool readable, writeable, executable, is_private, is_shared, is_ro, is_rw, is_rx;
        unsigned long long offset;
        std::string dev;
        unsigned long inode;
        std::string pathname;

        ProcMap() : startAddress(0), endAddress(0), length(0), protection(0),
                    is_private(false), is_shared(false),
                    readable(false), writeable(false), executable(false),
                    is_ro(false), is_rw(false), is_rx(false),
                    offset(0), inode(0) {}

        inline bool isValid() const { return (length > 0); }
        inline bool isUnknown() const { return pathname.empty(); }
    };

    /*
     * Changes protection of an address with given length
     */
    int setAddressProtection(void* address, size_t length, int protection, int* oldProtect = nullptr);

    /*
     * Writes buffer content to an address
     */
    bool memWrite(void* address, const void* buffer, size_t len);

    /*
     * Reads an address content into a buffer
     */
    bool memRead(void* buffer, const void* address, size_t len);

    /*
     * Reads an address content and returns hex string
     */
    std::string read2HexStr(const void* address, size_t len);

    /*
     * Nop-ing out of an address with given length
     */
    bool makeNOP(void* address, size_t len = 2, bool thumb = false);

    /*
     * Ret-ing out of an function
     */
    bool makeRET(void* address, bool thumb = false);

    /*
     * Wrapper to dereference & get value of a multi level pointer
     * Make sure to use the correct data type!
     */
    template<typename Type>
    Type readMultiPtr(void* ptr, std::vector<int> offsets)
    {
        Type defaultVal = {};
        if (ptr == NULL)
            return defaultVal;

        uintptr_t finalPtr = reinterpret_cast<uintptr_t>(ptr);
        int offsetsSize = offsets.size();
        if (offsetsSize > 0) {
            for (int i = 0; finalPtr != 0 && i < offsetsSize; i++) {
                if (i == (offsetsSize - 1))
                    return *reinterpret_cast<Type*>(finalPtr + offsets[i]);

                finalPtr = *reinterpret_cast<uintptr_t*>(finalPtr + offsets[i]);
            }
        }

        if (finalPtr == 0)
            return defaultVal;

        return *reinterpret_cast<Type*>(finalPtr);
    }

    /*
     * Wrapper to dereference & set value of a multi level pointer
     * Make sure to use the correct data type!, const objects won't work
     */
    template<typename Type>
    bool writeMultiPtr(void* ptr, std::vector<int> offsets, Type val)
    {
        if (ptr == NULL)
            return false;

        uintptr_t finalPtr = reinterpret_cast<uintptr_t>(ptr);
        int offsetsSize = offsets.size();
        if (offsetsSize > 0) {
            for (int i = 0; finalPtr != 0 && i < offsetsSize; i++) {
                if (i == (offsetsSize - 1)) {
                    *reinterpret_cast<Type*>(finalPtr + offsets[i]) = val;
                    return true;
                }

                finalPtr = *reinterpret_cast<uintptr_t*>(finalPtr + offsets[i]);
            }
        }

        if (finalPtr == 0)
            return false;

        *reinterpret_cast<Type*>(finalPtr) = val;
        return true;
    }

    /*
     * Wrapper to dereference & get value of a pointer
     * Make sure to use the correct data type!
     */
    template<typename Type>
    Type readPtr(void* ptr)
    {
        Type defaultVal = {};
        if (ptr == NULL)
            return defaultVal;

        return *reinterpret_cast<Type*>(ptr);
    }

    /*
     * Wrapper to dereference & set value of a pointer
     * Make sure to use the correct data type!, const objects won't work
     */
    template<typename Type>
    bool writePtr(void* ptr, Type val)
    {
        if (ptr == NULL)
            return false;

        *reinterpret_cast<Type*>(ptr) = val;
        return true;
    }

    /*
     *
     */
    template<typename Ret, typename...Args>
    Ret callFunction(void* ptr, Args...args)
    {
        if (ptr == NULL)
            return Ret();

        uintptr_t address_ = reinterpret_cast<uintptr_t>(ptr);
        return reinterpret_cast<Ret(*)(Args...)>(address_)(args...);
    }

    /*
     *
     */
    template<typename Ret, typename C, typename... Args>
    Ret callMethod(void* ptr, C thiz, Args...args)
    {
        if (ptr == NULL || thiz == NULL)
            return Ret();

        uintptr_t address_ = reinterpret_cast<uintptr_t>(ptr);
        return reinterpret_cast<Ret(*)(C, Args...)>(address_)(thiz, args...);
    }

    /*
     * Gets info of all maps in self process
     */
    std::vector<ProcMap> getAllMaps();

    /*
     * Gets info of all maps which contain "name" in self process
     */
    std::vector<ProcMap> getMapsByName(const std::string& name);

    /*
     * Gets map info of an address in self process
     */
    ProcMap getAddressMap(const void *address);

    /*
     * Gets first r-xp map of a library
     */
    ProcMap getLibraryBaseMap(const std::vector<ProcMap>& maps);
    ProcMap getLibraryBaseMap(const std::string& name);
};

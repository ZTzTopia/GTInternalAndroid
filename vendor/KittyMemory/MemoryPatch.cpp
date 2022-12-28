//
//  MemoryPatch.cpp
//
//  Created by MJ (Ruit) on 1/1/19.
//

#include "MemoryPatch.h"

using KittyMemory::ProcMap;

MemoryPatch::MemoryPatch()
{
    _address = 0;
    _size = 0;
    _orig_code.clear();
    _patch_code.clear();
    _is_nop = false;
    _is_ret = false;
}

MemoryPatch::~MemoryPatch()
{
    // clean up
    _orig_code.clear();
    _patch_code.clear();
}

MemoryPatch::MemoryPatch(const ProcMap &map, uintptr_t address,
                         const void* patch_code, size_t patch_size)
{
    _address = 0;
    _size = 0;
    _orig_code.clear();
    _patch_code.clear();
    _is_nop = false;
    _is_ret = false;

    if (!map.isValid() || address == 0 || !patch_code || patch_size < 1) {
        return;
    }

    _address = map.startAddress + address;
    if (_address == 0) {
        return;
    }

    _size = patch_size;

    _orig_code.resize(patch_size);
    _patch_code.resize(patch_size);

    _is_nop = false;
    _is_ret = false;

    // initialize patch & backup current content
    KittyMemory::memRead(&_patch_code[0], patch_code, patch_size);
    KittyMemory::memRead(&_orig_code[0], reinterpret_cast<const void*>(_address), patch_size);
}

MemoryPatch::MemoryPatch(uintptr_t absolute_address,
                         const void* patch_code, size_t patch_size)
{
    _address = 0;
    _size = 0;
    _orig_code.clear();
    _patch_code.clear();
    _is_nop = false;
    _is_ret = false;

    if (absolute_address == 0 || !patch_code || patch_size < 1) {
        return;
    }

    _address = absolute_address;
    _size = patch_size;

    _orig_code.resize(patch_size);
    _patch_code.resize(patch_size);

    _is_nop = false;
    _is_ret = false;

    // initialize patch & backup current content
    KittyMemory::memRead(&_patch_code[0], patch_code, patch_size);
    KittyMemory::memRead(&_orig_code[0], reinterpret_cast<const void*>(_address), patch_size);
}

MemoryPatch MemoryPatch::createWithHex(const ProcMap &map, uintptr_t address, std::string hex)
{
    MemoryPatch patch;

    if (!map.isValid() || address == 0 || !KittyUtils::validateHexString(hex)) {
        return patch;
    }

    patch._address = map.startAddress + address;
    if (patch._address == 0) {
        return patch;
    }

    patch._size = hex.length() / 2;

    patch._orig_code.resize(patch._size);
    patch._patch_code.resize(patch._size);

    patch._is_nop = false;
    patch._is_ret = false;

    // initialize patch
    KittyUtils::fromHex(hex, &patch._patch_code[0]);

    // backup current content
    KittyMemory::memRead(&patch._orig_code[0], reinterpret_cast<const void*>(patch._address),
                         patch._size);
    return patch;
}

MemoryPatch MemoryPatch::createWithHex(uintptr_t absolute_address, std::string hex)
{
    MemoryPatch patch;

    if (absolute_address == 0 || !KittyUtils::validateHexString(hex)) {
        return patch;
    }

    patch._address = absolute_address;
    patch._size = hex.length() / 2;

    patch._orig_code.resize(patch._size);
    patch._patch_code.resize(patch._size);

    patch._is_nop = false;
    patch._is_ret = false;

    // initialize patch
    KittyUtils::fromHex(hex, &patch._patch_code[0]);

    // backup current content
    KittyMemory::memRead(&patch._orig_code[0], reinterpret_cast<const void*>(patch._address),
                         patch._size);
    return patch;
}

MemoryPatch MemoryPatch::nop(const ProcMap &map, uintptr_t address,
                             size_t patch_size, bool thumb)
{
    MemoryPatch patch;

    if (!map.isValid() || address == 0 || patch_size < 1) {
        return patch;
    }

    patch._address = map.startAddress + address;
    if (patch._address == 0) {
        return patch;
    }

#ifdef __arm__
    if (thumb) {
        patch._size = patch_size * 2;
    }
    else {
        patch._size = patch_size * 4;
    }
#elif __aarch64__
    patch._size = patch_size * 4;
#elif defined(__i386__) || defined(__x86_64__)
    patch._size = patch_size;
#endif

    patch._orig_code.resize(patch._size);
    patch._patch_code.resize(patch._size);

    patch._is_nop = true;
    patch._is_nop_thumb = thumb;
    patch._is_ret = false;

    // backup current content
    KittyMemory::memRead(&patch._orig_code[0], reinterpret_cast<void*>(patch._address),
                         patch._size);
    return patch;
}

MemoryPatch MemoryPatch::nop(uintptr_t absolute_address, size_t patch_size, bool thumb)
{
    MemoryPatch patch;

    if (absolute_address == 0) {
        return patch;
    }

    patch._address = absolute_address;
#ifdef __arm__
    if (thumb) {
        patch._size = patch_size * 2;
    }
    else {
        patch._size = patch_size * 4;
    }
#elif __aarch64__
    patch._size = patch_size * 4;
#elif defined(__i386__) || defined(__x86_64__)
    patch._size = patch_size;
#endif

    patch._orig_code.resize(patch._size);
    patch._patch_code.resize(patch._size);

    patch._is_nop = true;
    patch._is_nop_thumb = thumb;
    patch._is_ret = false;

    // backup current content
    KittyMemory::memRead(&patch._orig_code[0], reinterpret_cast<void*>(patch._address),
                         patch._size);
    return patch;
}

MemoryPatch MemoryPatch::ret(const ProcMap &map, uintptr_t address, bool thumb)
{
    MemoryPatch patch;

    if (!map.isValid() || address == 0) {
        return patch;
    }

    patch._address = map.startAddress + address;
    if (patch._address == 0) {
        return patch;
    }

    patch._is_nop = false;
    patch._is_ret = true;
    patch._is_ret_thumb = thumb;

    // backup current content
    KittyMemory::memRead(&patch._orig_code[0], reinterpret_cast<void*>(patch._address),
                         patch._size);
    return patch;
}

MemoryPatch MemoryPatch::ret(uintptr_t absolute_address, bool thumb)
{
    MemoryPatch patch;

    if (absolute_address == 0) {
        return patch;
    }

    patch._address = absolute_address;

    patch._is_nop = false;
    patch._is_ret = true;
    patch._is_ret_thumb = thumb;

    // backup current content
    KittyMemory::memRead(&patch._orig_code[0], reinterpret_cast<void*>(patch._address),
                         patch._size);
    return patch;
}

bool MemoryPatch::isValid() const
{
    if (_address == 0) {
        return false;
    }

    if (_is_ret) {
        return true;
    }

    if (_size <= 0) {
        return false;
    }

    if (_orig_code.size() == _size && _patch_code.size() == _size) {
        return true;
    }

    return false;
}

size_t MemoryPatch::get_PatchSize() const
{
    return _size;
}

uintptr_t MemoryPatch::get_TargetAddress() const
{
    return _address;
}

bool MemoryPatch::Restore()
{
    if (!isValid()) {
        return false;
    }

    return KittyMemory::memWrite(reinterpret_cast<void*>(_address), &_orig_code[0], _size);
}

bool MemoryPatch::Modify()
{
    if (!isValid()) {
        return false;
    }

    if (_is_nop) {
        return KittyMemory::makeNOP(reinterpret_cast<void*>(_address), _size, _is_nop_thumb);
    }
    else if (_is_ret) {
        return KittyMemory::makeRET(reinterpret_cast<void*>(_address), _is_ret_thumb);
    }
    else {
        return KittyMemory::memWrite(reinterpret_cast<void*>(_address), &_patch_code[0], _size);
    }
}

std::string MemoryPatch::get_CurrBytes() const
{
    if (!isValid()) {
        return "";
    }

    return KittyMemory::read2HexStr(reinterpret_cast<const void *>(_address), _size);
}

std::string MemoryPatch::get_OrigBytes() const
{
    if (!isValid()) {
        return "";
    }

    return KittyMemory::read2HexStr(_orig_code.data(), _orig_code.size());
}

std::string MemoryPatch::get_PatchBytes() const
{
    if (!isValid()) {
        return "";
    }

    return KittyMemory::read2HexStr(_patch_code.data(), _patch_code.size());
}

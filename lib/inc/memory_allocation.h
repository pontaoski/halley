/*
    SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef HALLEY_MEMORY_ALLOCATION_H
#define HALLEY_MEMORY_ALLOCATION_H

#ifdef __cplusplus
extern "C" {
#endif

struct HLMemoryAllocation;

typedef void *(*HLMemoryAllocFunc)(struct HLMemoryAllocation *memory,
                                   long size);
typedef void (*HLMemoryFreeFunc)(struct HLMemoryAllocation *memory,
                                 void *block);
typedef void *(*HLMemoryReallocFunc)(struct HLMemoryAllocation *memory,
                                     long currentSize,
                                     long newSize,
                                     void *block);

struct HLMemoryAllocation {
    void *userData;
    HLMemoryAllocFunc alloc;
    HLMemoryFreeFunc free;
    HLMemoryReallocFunc realloc;
};

#ifdef __cplusplus
}
#endif

#endif
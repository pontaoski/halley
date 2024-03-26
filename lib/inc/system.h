/*
    SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef HALLEY_CPU_H
#define HALLEY_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

struct HLSystem;
struct HLMemoryAllocation;

void HLSystemInit(struct HLSystem **system, struct HLMemoryAllocation *alloc);
void HLSystemExec(struct HLSystem *system);

#ifdef __cplusplus
}
#endif

#endif

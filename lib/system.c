// SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>
//
// SPDX-License-Identifier: MIT

#include "system.h"
#include "memory_allocation.h"
#include "system_p.h"

void HLSystemInit(struct HLSystem **system, struct HLMemoryAllocation *alloc)
{
    struct HLSystem *newSystem = alloc->alloc(alloc, sizeof(struct HLSystem));

    // TODO: check allocation
    newSystem->allocator = alloc;

    *system = newSystem;
}

void HLSystemExec(struct HLSystem *system)
{
}

// SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "system.h"
#include "memory_allocation.h"
#include "system_p.h"
#include "assembler.h"

HLMemoryAllocation alloc {
    nullptr,
    [](HLMemoryAllocation* self, long size) -> void* { return calloc(1, size); },
    [](HLMemoryAllocation* self, void* block) { return free(block);  },
    [](HLMemoryAllocation* self, long oldSize, long newSize, void* block) -> void* { return realloc(block, newSize); },
};

TEST(CPUTest, Interrupt255Exits) {
    HLSystem* system;
    HLSystemInit(&system, &alloc);
    system->memory.memory = new uint8_t[] { ASM(ASMOpcode_int | ASMImm_F(255)) };
    system->memory.memoryLimit = 4;

    HLSystemExec(system);
    EXPECT_EQ(system->testCode, 1);

    HLSystemDone(&system);
}

TEST(CPUTest, Interrupt254Exits) {
    HLSystem* system;
    HLSystemInit(&system, &alloc);
    system->memory.memory = new uint8_t[] { ASM(ASMOpcode_int | ASMImm_F(254)) };
    system->memory.memoryLimit = 4;

    HLSystemExec(system);
    EXPECT_EQ(system->testCode, 0);

    HLSystemDone(&system);
}

TEST(CPUTest, InstructionBraForwardOne) {
    HLSystem* system;
    HLSystemInit(&system, &alloc);
    system->memory.memory = new uint8_t[] {
        ASM(ASMOpcode_bra | ASMFunc_bra | ASMImm_B(1)),
        ASM(0xAA),
        ASM(ASMOpcode_int | ASMImm_F(255)),
    };
    system->memory.memoryLimit = 4 * 3;

    HLSystemExec(system);
    EXPECT_EQ(system->testCode, 1);

    HLSystemDone(&system);
}

TEST(CPUTest, InstructionBraForwardAndBackwards) {
    HLSystem* system;
    HLSystemInit(&system, &alloc);
    system->memory.memory = new uint8_t[] {
        ASM(ASMOpcode_bra | ASMFunc_bra | ASMImm_B(4)),
        ASM(0xAA),
        ASM(ASMOpcode_int | ASMImm_F(255)),
        ASM(0xAA),
        ASM(0xAA),
        ASM(ASMOpcode_bra | ASMFunc_bra | ASMImm_B(-4)),
    };
    system->memory.memoryLimit = 4 * 6;

    HLSystemExec(system);
    EXPECT_EQ(system->testCode, 1);

    HLSystemDone(&system);
}

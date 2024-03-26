// SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "memory_management_unit_p.h"

TEST(MemoryTest, OutOfBounds)
{
    HLMemoryManagementUnit mmu;
    mmu.memory = new uint8_t[1];
    mmu.memoryLimit = 1;
    mmu.pageTableBase = 0;

    HLMemoryResult result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt8(&mmu, 2, &result);

    EXPECT_EQ(result, HLMemoryResultBus);

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt8(&mmu, 0, &result);

    delete[] mmu.memory;
}

TEST(MemoryTest, Alignment)
{
    HLMemoryManagementUnit mmu;
    mmu.memory = new uint8_t[16];
    mmu.memoryLimit = 16;
    mmu.pageTableBase = 0;

    HLMemoryResult result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt16(&mmu, 1, &result);

    EXPECT_EQ(result, HLMemoryResultUnaligned);

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt16(&mmu, 2, &result);

    EXPECT_EQ(result, HLMemoryResultOK);

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt32(&mmu, 1, &result);

    EXPECT_EQ(result, HLMemoryResultUnaligned);

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt32(&mmu, 4, &result);

    EXPECT_EQ(result, HLMemoryResultOK);

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt64(&mmu, 1, &result);

    EXPECT_EQ(result, HLMemoryResultUnaligned);

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadPhysicalUInt64(&mmu, 8, &result);

    EXPECT_EQ(result, HLMemoryResultOK);

    delete[] mmu.memory;
}

TEST(MemoryTest, BasicPermissionChecks)
{
    HLMemoryResult result;
    HLMemoryManagementUnit mmu;
    mmu.memory = new uint8_t[8 * 64]{0};
    mmu.memoryLimit = 8 * 64;
    mmu.pageTableBase = 0;

    /*
    reading trips
    */

    // one trip with the UInt8

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt8(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b101;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt8(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    // one trip with the UInt16

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt16(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b101;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt16(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    // one trip with the UInt32

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt32(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b101;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt32(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    // one trip with the UInt64

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt64(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b101;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualUInt64(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    /*
    writing trips
    */

    // one trip with the UInt8

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt8(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b1001;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt8(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    // one trip with the UInt16

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt16(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b1001;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt16(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    // one trip with the UInt32

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt32(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b1001;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt32(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    // one trip with the UInt64

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt64(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b1001;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitWriteVirtualUInt64(&mmu, 0, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    // read instruction

    mmu.memory[0] = 0b1;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualInstruction(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultAccessViolation);

    mmu.memory[0] = 0b10001;

    result = HLMemoryResultOK;
    HLMemoryManagementUnitReadVirtualInstruction(&mmu, 0, &result);
    EXPECT_EQ(result, HLMemoryResultOK);

    delete[] mmu.memory;
}

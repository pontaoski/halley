/*
    SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef HALLEY_MEMORY_MANAGEMENT_UNIT_P_H
#define HALLEY_MEMORY_MANAGEMENT_UNIT_P_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t HLInstruction;

typedef uint8_t HLMemoryPermission;
enum {
    HLMemoryPermissionNone = 0x0,
    HLMemoryPermissionRead = 0x1,
    HLMemoryPermissionWrite = 0x2,
    HLMemoryPermissionExecute = 0x4,
};

typedef uint8_t HLMemoryResult;
enum {
    HLMemoryResultOK,
    HLMemoryResultUnmappedLevel1,
    HLMemoryResultUnmappedLevel2,
    HLMemoryResultUnmappedLevel3,
    HLMemoryResultUnmappedLevel4,
    HLMemoryResultUnmappedLevel5,
    HLMemoryResultAccessViolation,
    HLMemoryResultBus,
    HLMemoryResultUnaligned,
};

struct HLMemoryManagementUnit {
    uint8_t *memory;
    uint64_t memoryLimit;
    uint64_t pageTableBase;
};

HLInstruction HLMemoryManagementUnitReadVirtualInstruction(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);
HLInstruction HLMemoryManagementUnitReadPhysicalInstruction(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);

uint8_t HLMemoryManagementUnitReadVirtualUInt8(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);
uint16_t HLMemoryManagementUnitReadVirtualUInt16(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);
uint32_t HLMemoryManagementUnitReadVirtualUInt32(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);
uint64_t HLMemoryManagementUnitReadVirtualUInt64(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);

void HLMemoryManagementUnitWriteVirtualUInt8(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint8_t value,
    HLMemoryResult *code);
void HLMemoryManagementUnitWriteVirtualUInt16(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint16_t value,
    HLMemoryResult *code);
void HLMemoryManagementUnitWriteVirtualUInt32(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint32_t value,
    HLMemoryResult *code);
void HLMemoryManagementUnitWriteVirtualUInt64(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint64_t value,
    HLMemoryResult *code);

uint8_t HLMemoryManagementUnitReadPhysicalUInt8(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);
uint16_t HLMemoryManagementUnitReadPhysicalUInt16(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);
uint32_t HLMemoryManagementUnitReadPhysicalUInt32(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);
uint64_t HLMemoryManagementUnitReadPhysicalUInt64(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryResult *code);

void HLMemoryManagementUnitWritePhysicalUInt8(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint8_t value,
    HLMemoryResult *code);
void HLMemoryManagementUnitWritePhysicalUInt16(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint16_t value,
    HLMemoryResult *code);
void HLMemoryManagementUnitWritePhysicalUInt32(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint32_t value,
    HLMemoryResult *code);
void HLMemoryManagementUnitWritePhysicalUInt64(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    uint64_t value,
    HLMemoryResult *code);

uint64_t HLMemoryManagementUnitTranslateAddress(
    struct HLMemoryManagementUnit *mmu,
    uint64_t address,
    HLMemoryPermission requiredPermissions,
    HLMemoryResult *code);

#ifdef __cplusplus
}
#endif

#endif

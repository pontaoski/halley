// SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stddef.h>

#include "memory_management_unit_p.h"

#define HLPDEValid(pde) (pde & 0b1)
#define HLPDEOverride(pde) ((pde >> 1) & 0b1)
#define HLPDERead(pde) ((pde >> 2) & 0b1)
#define HLPDEWrite(pde) ((pde >> 3) & 0b1)
#define HLPDEExecute(pde) ((pde >> 4) & 0b1)
#define HLPDENext(pde)                                                         \
    (pde                                                                       \
     & 0b1111111111111111111111111111111111111111111111111100000000000000ull)

#define HLGarbage64 0xAAAAAAAAAAAAAAAAull
#define HLGarbage32 0xAAAAAAAAull
#define HLGarbage16 0xAAAAull
#define HLGarbage8 0xAAull

#define HLPDECheckNoPerms(pde, permissions)                                    \
    ((!HLPDERead(pde) && (permissions & HLMemoryPermissionRead))               \
     || (!HLPDEWrite(pde) && (permissions & HLMemoryPermissionWrite))          \
     || (!HLPDEExecute(pde) && (permissions & HLMemoryPermissionExecute)))

#define HLMemoryManagementUnitTranslateAddressLevel(mmu,                       \
                                                    permissions,               \
                                                    base,                      \
                                                    index,                     \
                                                    authoritative,             \
                                                    this,                      \
                                                    next,                      \
                                                    error)                     \
    uint64_t this =                                                            \
        HLMemoryManagementUnitReadPhysicalUInt64(mmu, base + index * 8, code); \
    if (*code != HLMemoryResultOK) {                                           \
        return HLGarbage64;                                                    \
    }                                                                          \
    if (!HLPDEValid(this)) {                                                   \
        *code = error;                                                         \
        return HLGarbage64;                                                    \
    }                                                                          \
    if (HLPDEOverride(this)) {                                                 \
        authoritative = this;                                                  \
    }                                                                          \
    if (authoritative == 0) {                                                  \
        if (HLPDECheckNoPerms(this, permissions)) {                            \
            *code = HLMemoryResultAccessViolation;                             \
            return HLGarbage64;                                                \
        }                                                                      \
    } else {                                                                   \
        if (HLPDECheckNoPerms(authoritative, permissions)) {                   \
            *code = HLMemoryResultAccessViolation;                             \
            return HLGarbage64;                                                \
        }                                                                      \
    }                                                                          \
    uint64_t next = HLPDENext(this);

uint64_t HLMemoryManagementUnitTranslateAddress(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryPermission requiredPermissions,
    HLMemoryResult *restrict code)
{
    uint64_t levelOneIndex = (address & (0b111111ull << 58)) >> 58;
    uint64_t levelTwoIndex = (address & (0b11111111111ull << 47)) >> 47;
    uint64_t levelThreeIndex = (address & (0b11111111111ull << 36)) >> 36;
    uint64_t levelFourIndex = (address & (0b11111111111ull << 25)) >> 25;
    uint64_t levelFiveIndex = (address & (0b11111111111ull << 14)) >> 14;
    uint64_t physicalIndex = (address & (0b11111111111111ull));

    uint64_t authoritativePde = 0;

    // level one
    HLMemoryManagementUnitTranslateAddressLevel(mmu,
                                                requiredPermissions,
                                                mmu->pageTableBase,
                                                levelOneIndex,
                                                authoritativePde,
                                                pdeOne,
                                                nextOne,
                                                HLMemoryResultUnmappedLevel1)

    HLMemoryManagementUnitTranslateAddressLevel(mmu,
                                                requiredPermissions,
                                                nextOne,
                                                levelTwoIndex,
                                                authoritativePde,
                                                pdeTwo,
                                                nextTwo,
                                                HLMemoryResultUnmappedLevel2)

    HLMemoryManagementUnitTranslateAddressLevel(mmu,
                                                requiredPermissions,
                                                nextTwo,
                                                levelThreeIndex,
                                                authoritativePde,
                                                pdeThree,
                                                nextThree,
                                                HLMemoryResultUnmappedLevel3)

    HLMemoryManagementUnitTranslateAddressLevel(mmu,
                                                requiredPermissions,
                                                nextThree,
                                                levelFourIndex,
                                                authoritativePde,
                                                pdeFour,
                                                nextFour,
                                                HLMemoryResultUnmappedLevel4)

    HLMemoryManagementUnitTranslateAddressLevel(mmu,
                                                requiredPermissions,
                                                nextFour,
                                                levelFiveIndex,
                                                authoritativePde,
                                                pdeFive,
                                                nextFive,
                                                HLMemoryResultUnmappedLevel5)

    return nextFive + physicalIndex;
}

static inline bool HLMemoryManagementUnitDoTranslateAddress(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t *restrict address,
    HLMemoryPermission requiredPermissions,
    HLMemoryResult *restrict code)
{
    *address = HLMemoryManagementUnitTranslateAddress(mmu,
                                                      *address,
                                                      requiredPermissions,
                                                      code);
    if (*code != HLMemoryResultOK) {
        return false;
    }
    return true;
}

HLInstruction HLMemoryManagementUnitReadVirtualInstruction(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionExecute,
                                                  code)) {
        return 0;
    }
    return HLMemoryManagementUnitReadPhysicalUInt32(mmu, address, code);
}

uint8_t HLMemoryManagementUnitReadVirtualUInt8(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionRead,
                                                  code)) {
        return 0;
    }
    return HLMemoryManagementUnitReadPhysicalUInt8(mmu, address, code);
}
uint16_t HLMemoryManagementUnitReadVirtualUInt16(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionRead,
                                                  code)) {
        return 0;
    }
    return HLMemoryManagementUnitReadPhysicalUInt16(mmu, address, code);
}
uint32_t HLMemoryManagementUnitReadVirtualUInt32(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionRead,
                                                  code)) {
        return 0;
    }
    return HLMemoryManagementUnitReadPhysicalUInt32(mmu, address, code);
}
uint64_t HLMemoryManagementUnitReadVirtualUInt64(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionRead,
                                                  code)) {
        return 0;
    }
    return HLMemoryManagementUnitReadPhysicalUInt64(mmu, address, code);
}

void HLMemoryManagementUnitWriteVirtualUInt8(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint8_t value,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionWrite,
                                                  code)) {
        return;
    }
    return HLMemoryManagementUnitWritePhysicalUInt8(mmu, address, value, code);
}
void HLMemoryManagementUnitWriteVirtualUInt16(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint16_t value,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionWrite,
                                                  code)) {
        return;
    }
    return HLMemoryManagementUnitWritePhysicalUInt16(mmu, address, value, code);
}
void HLMemoryManagementUnitWriteVirtualUInt32(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint32_t value,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionWrite,
                                                  code)) {
        return;
    }
    return HLMemoryManagementUnitWritePhysicalUInt32(mmu, address, value, code);
}
void HLMemoryManagementUnitWriteVirtualUInt64(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint64_t value,
    HLMemoryResult *restrict code)
{
    if (!HLMemoryManagementUnitDoTranslateAddress(mmu,
                                                  &address,
                                                  HLMemoryPermissionWrite,
                                                  code)) {
        return;
    }
    return HLMemoryManagementUnitWritePhysicalUInt64(mmu, address, value, code);
}

#define HLMemoryManagementUnitCheckDetail(mmu, address, size, ret)             \
    if (address > mmu->memoryLimit) {                                          \
        *code = HLMemoryResultBus;                                             \
        return ret;                                                            \
    } else if (address % sizeof(uint##size##_t) != 0) {                        \
        *code = HLMemoryResultUnaligned;                                       \
        return ret;                                                            \
    }

#define HLMemoryManagementUnitCheck(mmu, address, size)                        \
    HLMemoryManagementUnitCheckDetail(mmu, address, size, HLGarbage##size)

#define HLMemoryManagementUnitCheckVoid(mmu, address, size)                    \
    HLMemoryManagementUnitCheckDetail(mmu, address, size, )

#define HLMemoryManagementUnitIndex(base, size, address)                       \
    ((uint##size##_t *)(base))[address / sizeof(uint##size##_t)]

void HLMemoryManagementUnitWritePhysicalUInt8(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint8_t value,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheckVoid(mmu, address, 8)
    HLMemoryManagementUnitIndex(mmu->memory, 8, address) = value;
}
void HLMemoryManagementUnitWritePhysicalUInt16(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint16_t value,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheckVoid(mmu, address, 16)
    HLMemoryManagementUnitIndex(mmu->memory, 16, address) = value;
}
void HLMemoryManagementUnitWritePhysicalUInt32(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint32_t value,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheckVoid(mmu, address, 32)
    HLMemoryManagementUnitIndex(mmu->memory, 32, address) = value;
}
void HLMemoryManagementUnitWritePhysicalUInt64(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    uint64_t value,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheckVoid(mmu, address, 64)
    HLMemoryManagementUnitIndex(mmu->memory, 64, address) = value;
}

uint8_t HLMemoryManagementUnitReadPhysicalUInt8(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheck(mmu, address, 8)
    return HLMemoryManagementUnitIndex(mmu->memory, 8, address);
}
uint16_t HLMemoryManagementUnitReadPhysicalUInt16(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheck(mmu, address, 16)
    return HLMemoryManagementUnitIndex(mmu->memory, 16, address);
}
uint32_t HLMemoryManagementUnitReadPhysicalUInt32(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheck(mmu, address, 32)
    return HLMemoryManagementUnitIndex(mmu->memory, 32, address);
}
uint64_t HLMemoryManagementUnitReadPhysicalUInt64(
    struct HLMemoryManagementUnit *restrict mmu,
    uint64_t address,
    HLMemoryResult *restrict code)
{
    HLMemoryManagementUnitCheck(mmu, address, 64)
    return HLMemoryManagementUnitIndex(mmu->memory, 64, address);
}

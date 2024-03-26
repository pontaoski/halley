/*
    SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef HALLEY_SYSTEM_P_H
#define HALLEY_SYSTEM_P_H

#include <stdbool.h>
#include <stdint.h>

#include "memory_allocation.h"
#include "memory_management_unit_p.h"

/**
 * little endian
 */
typedef uint32_t HLInstruction;

#define HLOpcode(instruction) (instruction & 0xFF)

/* Macros for E-encoded instructions */

#define HLImm_E(instruction) ((instruction >> 8) & 0xFF)
#define HLFunc_E(instruction) ((instruction >> 16) & 0xF)
#define HLRs2_E(instruction) ((instruction >> 20) & 0xF)
#define HLRs1_E(instruction) ((instruction >> 24) & 0xF)
#define HLRde_E(instruction) ((instruction >> 28) & 0xF)

/* Macros for R-encoded instructions */

#define HLImm_R(instruction) ((instruction >> 8) & 0xFFF)
#define HLRs2_R(instruction) ((instruction >> 20) & 0xF)
#define HLRs1_R(instruction) ((instruction >> 24) & 0xF)
#define HLRde_R(instruction) ((instruction >> 28) & 0xF)

/* Macros for M-encoded instructions */

#define HLImm_M(instruction) ((instruction >> 8) & 0xFFFF)
#define HLRs1_M(instruction) ((instruction >> 24) & 0xF)
#define HLRde_M(instruction) ((instruction >> 28) & 0xF)

/* Macros for F-encoded instructions */

#define HLImm_F(instruction) ((instruction >> 8) & 0xFFFF)
#define HLFunc_F(instruction) ((instruction >> 24) & 0xF)
#define HLRde_F(instruction) ((instruction >> 28) & 0xF)

/* Macros for B-encoded instructions */

#define HLImm_B(instruction) ((instruction >> 8) & 0xFFFFF)
#define HLFunc_B(instruction) ((instruction >> 28) & 0xF)

typedef uint8_t HLInstructionFormat;

enum {
    HLFormatE,
    HLFormatR,
    HLFormatM,
    HLFormatF,
    HLFormatB,
};

typedef uint8_t HLRegister;

enum {
    /** zero register */
    HLRegRZ,
    /** general purpose registers */
    HLRegRA,
    HLRegRB,
    HLRegRC,
    HLRegRD,
    HLRegRE,
    HLRegRF,
    HLRegRG,
    HLRegRH,
    HLRegRI,
    HLRegRJ,
    HLRegRK,
    /** instruction pointer */
    HLRegIP,
    /** stack and frame pointer */
    HLRegSP,
    HLRegFP,
    /** status register */
    HLRegStatus,
    HLNReg,
};

typedef uint8_t HLStatusFlag;

enum {
    HLFlagSign,
    HLFlagZero,
    HLFlagCarryBorrow,
    HLFlagCarryBorrowUnsigned,
    HLFlagEqual,
    HLFlagLess,
    HLFlagLessUnsigned,
    HLFlagMode,

    HLFlagExtensionF = 31,
};

typedef uint8_t HLInterrupt;

enum {
    /**
     * Triggers when the second argument of a div, mod, or rem instruction is
     * zero.
     */
    HLInterruptDivideByZero,
    /** Reserved for debugger breakpoints. */
    HLInterruptBreakpoint,
    /**
     * Triggers when some kind of restricted or invalid operation occurs.
     * This includes unrecognized opcode, unrecognized secondary function
     * values, or when a restricted instruction is encountered / modification
     * of a restricted register is attempted in user mode.
     */
    HLInterruptInvalidOperation,
    /** Triggers when the stack pointer exceeds the frame pointer, which
     *  indicates a stack underflow has occurred.
     */
    HLInterruptStackUnderflow,
    /** Triggers when memory has been accessed across type width boundaries. */
    HLInterruptUnalignedAccess,
    /** Triggers when memory has been accessed in an invalid way.
     *  In kernel mode, this triggers due to accesses outside physical memory
     *  bounds.
     *  In user mode, this triggers when unmapped/invalid memory is accessed or
     *  when virtual memory permissions do not allow the access.
     */
    HLInterruptAccessViolation,
    /** Triggers when interrupt controller has experienced an interrupt queue
     *  overflow due to too many interrupts in a short period of time.
     */
    HLInterruptOverflow,
};

struct HLCPUCore {
    uint64_t registers[HLNReg];
    uint64_t cycles;
    HLInstruction currentInstruction;
    bool running;
};

struct HLInterruptController {
    uint64_t interruptVectorTableBaseAddress;
    uint64_t returnAddress;
    uint64_t returnStatus;

    HLInterrupt queue[16];
    uint8_t queueSize;
};

struct HLSystem {
    /* host system stuff */
    struct HLMemoryAllocation *allocator;

    /* emulated system stuff */
    struct HLCPUCore cpu;
    struct HLInterruptController interrupts;
    struct HLMemoryManagementUnit memory;
};

#endif

/*
    SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef HALLEY_ASSEMBLER_P_H
#define HALLEY_ASSEMBLER_P_H

#define ASM(instruction) ((instruction) & 0xFF), (((instruction) >> 8) & 0xFF), (((instruction) >> 16) & 0xFF), (((instruction) >> 24) & 0xFF)

/* Macros for E-encoded instructions */

#define ASMImm_E(instruction) ((instruction & 0xFF) << 8)
#define ASMFunc_E(instruction) ((instruction & 0xF) << 16)
#define ASMRs2_E(instruction) ((instruction & 0xF) << 20)
#define ASMRs1_E(instruction) ((instruction & 0xF) << 24)
#define ASMRde_E(instruction) ((instruction & 0xF) << 28)

/* Macros for R-encoded instructions */

#define ASMImm_R(instruction) ((instruction & 0xFFF) << 8)
#define ASMRs2_R(instruction) ((instruction & 0xF) << 20)
#define ASMRs1_R(instruction) ((instruction & 0xF) << 24)
#define ASMRde_R(instruction) ((instruction & 0xF) << 28)

/* Macros for M-encoded instructions */

#define ASMImm_M(instruction) ((instruction & 0xFFFF) << 8)
#define ASMRs1_M(instruction) ((instruction & 0xF) << 24)
#define ASMRde_M(instruction) ((instruction & 0xF) << 28)

/* Macros for F-encoded instructions */

#define ASMImm_F(instruction) ((instruction & 0xFFFF) << 8)
#define ASMFunc_F(instruction) ((instruction & 0xF) << 24)
#define ASMRde_F(instruction) ((instruction & 0xF) << 28)

/* Macros for B-encoded instructions */

#define ASMImm_B(instruction) ((instruction & 0xFFFFF) << 8)
#define ASMFunc_B(instruction) ((instruction & 0xF) << 28)

#define HL_INSTRUCTION(mnemonic, opcode, func, format, comment)\
/** comment */\
enum { ASMOpcode_ ## mnemonic = opcode };\
/** comment */\
enum { ASMFunc_ ## mnemonic = func };
#include "instructions.h"

#endif

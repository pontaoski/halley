/*
    SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include "system.h"
#include "memory_allocation.h"
#include "system_p.h"

void HLSystemInit(struct HLSystem **system, struct HLMemoryAllocation *alloc)
{
    struct HLSystem *newSystem = alloc->alloc(alloc, sizeof(struct HLSystem));

    /* TODO: check allocation */
    newSystem->allocator = alloc;

    *system = newSystem;
}

#define HL_INSTRUCTION(mnemonic, opcode, func, format, comment)\
/** comment */\
enum { HLOpcode_ ## mnemonic = opcode };\
/** comment */\
enum { HLFunc_ ## mnemonic = func };
#include "instructions.h"

void HLSystemDone(struct HLSystem **system)
{
    struct HLSystem *ptrSystem = *system;
    ptrSystem->allocator->free(ptrSystem->allocator, ptrSystem);
    *system = 0;
}

#define U1 (uint64_t)(1)
#define U19 (uint64_t)(19)
#define HLSignExtend64By20(value) ((int64_t)((((value) ^ (U1 << (U19))) - (U1 << (U19)))))

void HLSystemExec(struct HLSystem *system)
{
    HLInstruction instruction;
    HLMemoryResult result = HLMemoryResultOK;
    while (1) {
        if (system->cpu.registers[HLRegStatus] & HLFlagMode) {
            /* if user mode is set then do address translation to read the instruction */
            instruction = HLMemoryManagementUnitReadVirtualInstruction(&system->memory, system->cpu.registers[HLRegIP], &result);
        } else {
            /* otherwise read w/o address translation */
            instruction = HLMemoryManagementUnitReadPhysicalInstruction(&system->memory, system->cpu.registers[HLRegIP], &result);
        }

        if (result != HLMemoryResultOK) {
            /* TODO: queue up an interrupt */
            assert(0 && "TODO: queue interrupt");
            continue;
        }

        /* only increment the instruction pointer after we've confirmed that the read succeeded */
        system->cpu.registers[HLRegIP] += 4;

        switch (HLOpcode(instruction)) {
        case 0x01: /* the system control instructions */
            switch (HLFunc_F(instruction)) {
            case HLFunc_int:
                if (HLImm_F(instruction) == 255) {
                    system->testCode = 1;
                    return;
                }
                if (HLImm_F(instruction) == 254) {
                    system->testCode = 0;
                    return;
                }
                assert(0 && "Unimplemented opcode int");
                break;
            case HLFunc_iret:
                assert(0 && "Unimplemented opcode iret");
                break;
            case HLFunc_ires:
                assert(0 && "Unimplemented opcode ires");
                break;
            case HLFunc_usr:
                assert(0 && "Unimplemented opcode usr");
                break;
            }
            break;
        case 0xA: /* the branching instructions */
            switch (HLFunc_B(instruction)) {
            case HLFunc_bra:
                system->cpu.registers[HLRegIP] += 4 * HLSignExtend64By20(HLImm_B(instruction));
                break;
            case HLFunc_beq:
                assert(0 && "Unimplemented opcode beq");
                break;
            case HLFunc_bez:
                assert(0 && "Unimplemented opcode bez");
                break;
            case HLFunc_blt:
                assert(0 && "Unimplemented opcode blt");
                break;
            case HLFunc_ble:
                assert(0 && "Unimplemented opcode ble");
                break;
            case HLFunc_bltu:
                assert(0 && "Unimplemented opcode ltu");
                break;
            case HLFunc_bleu:
                assert(0 && "Unimplemented opcode leu");
                break;
            case HLFunc_bne:
                assert(0 && "Unimplemented opcode bne");
                break;
            case HLFunc_bnz:
                assert(0 && "Unimplemented opcode bnz");
                break;
            case HLFunc_bge:
                assert(0 && "Unimplemented opcode bge");
                break;
            case HLFunc_bgt:
                assert(0 && "Unimplemented opcode bgt");
                break;
            case HLFunc_bgeu:
                assert(0 && "Unimplemented opcode geu");
                break;
            case HLFunc_bgtu:
                assert(0 && "Unimplemented opcode gtu");
                break;
            }
            break;
        default:
            /* TODO: raise interrupt */
            assert(0 && "Unknown opcode");
            break;
        }
    }
}

int HLSystemTestCode(struct HLSystem *system)
{
    return system->testCode;
}

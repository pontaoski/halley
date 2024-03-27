#ifndef HL_INSTRUCTION
#define HL_INSTRUCTION(mnemonic,opcode,func,format,comment)
#endif

#ifndef HLFuncUndefined
#define HLFuncUndefined 0xAA
#endif

/* 3.1 System control instructions */
HL_INSTRUCTION(int, 0x01, 0x00, HLFormatF, Trigger interrupt imm)
HL_INSTRUCTION(iret, 0x01, 0x01, HLFormatF, Return from interrupt)
HL_INSTRUCTION(ires, 0x01, 0x02, HLFormatF, Resolve interrupt)
HL_INSTRUCTION(usr, 0x01, 0x03, HLFormatF, Enter usermode in the address contained in rde)

/* 3.2 IO instructions */
HL_INSTRUCTION(outr, 0x02, HLFuncUndefined, HLFormatM, Outputs the value of rs1 to the port specified in rde)
HL_INSTRUCTION(outi, 0x03, HLFuncUndefined, HLFormatM, Outputs the value of rs1 to the port specified in imm)
HL_INSTRUCTION(inr, 0x04, HLFuncUndefined, HLFormatM, Reads the data from the port specified in rs1 to rde)
HL_INSTRUCTION(ini, 0x05, HLFuncUndefined, HLFormatM, Reads the data from the port specified in imm to rde)

/* 3.3 Control flow instructions */
HL_INSTRUCTION(jal, 0x06, HLFuncUndefined, HLFormatM, Pushes IP to stack; IP becomes rs1 + 4*(int64_t)imm)
HL_INSTRUCTION(jalr, 0x07, HLFuncUndefined, HLFormatM, Stores IP to rde; IP becomes rs1 + 4*(int64_t)imm)
HL_INSTRUCTION(ret, 0x08, HLFuncUndefined, HLFormatM, Pops IP from stack)
HL_INSTRUCTION(retr, 0x09, HLFuncUndefined, HLFormatM, Sets IP to value in rs)
/* 3.3.1 Branch instructions

   Sets IP to IP + 4*(int64_t)imm
*/
HL_INSTRUCTION(bra, 0xA, 0x0, HLFormatB, Always branches)
HL_INSTRUCTION(beq, 0xA, 0x1, HLFormatB, Branches if EQUAL is set)
HL_INSTRUCTION(bez, 0xA, 0x2, HLFormatB, Branches if ZERO is set)
HL_INSTRUCTION(blt, 0xA, 0x3, HLFormatB, Branches if LESS is set)
HL_INSTRUCTION(ble, 0xA, 0x4, HLFormatB, Branches if LESS or EQUAL is set)
HL_INSTRUCTION(bltu, 0xA, 0x5, HLFormatB, Branches if LESS_UNSIGNED is set)
HL_INSTRUCTION(bleu, 0xA, 0x6, HLFormatB, Branches if LESS_UNSIGNED or EQUAL is set)
HL_INSTRUCTION(bne, 0xA, 0x9, HLFormatB, Branches if EQUAL is not set set)
HL_INSTRUCTION(bnz, 0xA, 0xA, HLFormatB, Branches if ZERO is not set)
HL_INSTRUCTION(bge, 0xA, 0xB, HLFormatB, Branches if LESS is not set)
HL_INSTRUCTION(bgt, 0xA, 0xC, HLFormatB, Branches if LESS and EQUAL is not set)
HL_INSTRUCTION(bgeu, 0xA, 0xD, HLFormatB, Branches if LESS_UNSIGNED is not set)
HL_INSTRUCTION(bgtu, 0xA, 0xE, HLFormatB, Branches if LESS_UNSIGNED and EQUAL is not set)

/* 3.4 Stack Operations */
HL_INSTRUCTION(push, 0x0B, HLFuncUndefined, HLFormatM, Pushes the value in rs onto the stack.)
HL_INSTRUCTION(pop, 0x0C, HLFuncUndefined, HLFormatM, Pops the value in rs from the stack.)
HL_INSTRUCTION(enter, 0x0D, HLFuncUndefined, HLFormatM, Pushes FP to the stack and assigns SP to the FP.)
HL_INSTRUCTION(leave, 0x0E, HLFuncUndefined, HLFormatM, Sets SP to the FP; pops the value in FP from the stack..)

/* 3.5 Data flow */
HL_INSTRUCTION(lli, 0x10, 0x0, HLFormatF, Loads imm into the lower 16 bits of rd)
HL_INSTRUCTION(lui, 0x10, 0x2, HLFormatF, Loads imm into the lower-upper 16 bits of rd)
HL_INSTRUCTION(lti, 0x10, 0x4, HLFormatF, Loads imm into the upper-lower 16 bits of rd)
HL_INSTRUCTION(ltui, 0x10, 0x6, HLFormatF, Loads imm into the upper-upper 16 bits of rd)

HL_INSTRUCTION(llis, 0x10, 0x1, HLFormatF, Loads sign-extended imm into rd)
HL_INSTRUCTION(luis, 0x10, 0x3, HLFormatF, Loads sign-extended imm shifted left by 16 into rd)
HL_INSTRUCTION(ltis, 0x10, 0x5, HLFormatF, Loads sign-extended imm shifted left by 32 into rd)
HL_INSTRUCTION(ltuis, 0x10, 0x7, HLFormatF, Loads sign-extended imm shifted left by 48 into rd)

HL_INSTRUCTION(lw, 0x11, HLFuncUndefined, HLFormatE, Loads the word on the bus at address rs1 + 8*(int64_t)imm + rs2 << func into rde)

HL_INSTRUCTION(lh, 0x12, HLFuncUndefined, HLFormatE, Loads the half-word on the bus at address rs1 + 4*(int64_t)imm + rs2 << func into the lower 32 bits of rde)
HL_INSTRUCTION(lhs, 0x13, HLFuncUndefined, HLFormatE, Loads the sign-extended half-word on the bus at address rs1 + 4*(int64_t)imm + rs2 << func into rde)

HL_INSTRUCTION(lq, 0x14, HLFuncUndefined, HLFormatE, Loads the quarter-word on the bus at address rs1 + 2*(int64_t)imm + rs2 << func into the lower 16 bits of rde)
HL_INSTRUCTION(lqs, 0x15, HLFuncUndefined, HLFormatE, Loads the sign-extended quarter-word on the bus at address rs1 + 2*(int64_t)imm + rs2 << func into rde)

HL_INSTRUCTION(lb, 0x16, HLFuncUndefined, HLFormatE, Loads the byte on the bus at address rs1 + 1*(int64_t)imm + rs2 << func into the lower 16 bits of rde)
HL_INSTRUCTION(lbs, 0x17, HLFuncUndefined, HLFormatE, Loads the sign-extended on the bus at address rs1 + 2*(int64_t)imm + rs2 << func into rde)

HL_INSTRUCTION(sw, 0x18, HLFuncUndefined, HLFormatE, Stores the word in rde to the bus at address rs1 + 2*(int64_t)imm + rs2 << func)
HL_INSTRUCTION(sh, 0x19, HLFuncUndefined, HLFormatE, Stores the half-word in rde to the bus at address rs1 + 2*(int64_t)imm + rs2 << func)
HL_INSTRUCTION(sq, 0x1A, HLFuncUndefined, HLFormatE, Stores the quarter-word in rde to the bus at address rs1 + 2*(int64_t)imm + rs2 << func)
HL_INSTRUCTION(sb, 0x1B, HLFuncUndefined, HLFormatE, Stores the byte in rde to the bus at address rs1 + 2*(int64_t)imm + rs2 << func)

/* 3.6 Comparisons */
HL_INSTRUCTION(cmpr, 0x1E, HLFuncUndefined, HLFormatM, Compares rs1 and rs2 and sets flags)
HL_INSTRUCTION(cmpi, 0x1F, 0x00, HLFormatM, Compares rs1 and imm and sets flags. If func is 1; then imm and rs1 are compared; otherwise func must be 0.)

/* 3.7 Arithmetic operations */
HL_INSTRUCTION(addr, 0x20, HLFuncUndefined, HLFormatR, Sets rde to rs1 plus rs2.)
HL_INSTRUCTION(addi, 0x21, HLFuncUndefined, HLFormatM, Sets rde to rs1 plus imm.)
HL_INSTRUCTION(subr, 0x22, HLFuncUndefined, HLFormatR, Sets rde to rs1 minus rs2.)
HL_INSTRUCTION(subi, 0x23, HLFuncUndefined, HLFormatM, Sets rde to rs1 minus imm.)
HL_INSTRUCTION(imulr, 0x24, HLFuncUndefined, HLFormatR, Sets rde to rs1 times rs2 (signed).)
HL_INSTRUCTION(imuli, 0x25, HLFuncUndefined, HLFormatM, Sets rde to rs1 times imm (signed).)
HL_INSTRUCTION(idivr, 0x26, HLFuncUndefined, HLFormatR, Sets rde to rs1 divided by rs2 (signed).)
HL_INSTRUCTION(idivi, 0x27, HLFuncUndefined, HLFormatM, Sets rde to rs1 divided by imm (signed).)
HL_INSTRUCTION(umulr, 0x28, HLFuncUndefined, HLFormatR, Sets rde to rs1 times rs2 (unsigned).)
HL_INSTRUCTION(umuli, 0x29, HLFuncUndefined, HLFormatM, Sets rde to rs1 times imm (unsigned).)
HL_INSTRUCTION(udivr, 0x2A, HLFuncUndefined, HLFormatR, Sets rde to rs1 divided by rs2 (unsigned).)
HL_INSTRUCTION(udivi, 0x2B, HLFuncUndefined, HLFormatM, Sets rde to rs1 divided by imm (unsigned).)
HL_INSTRUCTION(remr, 0x2C, HLFuncUndefined, HLFormatR, Sets rde to the remainder of rs1 divided by rs2 (signed).)
HL_INSTRUCTION(remi, 0x2D, HLFuncUndefined, HLFormatM, Sets rde to the remainder of rs1 divided by imm (signed).)
HL_INSTRUCTION(modr, 0x2E, HLFuncUndefined, HLFormatR, Sets rde to the remainder of rs1 divided by rs2 (unsigned).)
HL_INSTRUCTION(modi, 0x2F, HLFuncUndefined, HLFormatM, Sets rde to the remainder of rs1 divided by imm (unsigned).)

/* 3.8 Bitwise operations */
HL_INSTRUCTION(andr, 0x30, HLFuncUndefined, HLFormatR, Sets rde to to rs1 bitwise ANDed by rs2)
HL_INSTRUCTION(andi, 0x31, HLFuncUndefined, HLFormatM, Sets rde to to rs1 bitwise ANDed by imm)
HL_INSTRUCTION(orr, 0x32, HLFuncUndefined, HLFormatR, Sets rde to to rs1 bitwise ORed by rs2)
HL_INSTRUCTION(ori, 0x33, HLFuncUndefined, HLFormatM, Sets rde to to rs1 bitwise ORed by imm)
HL_INSTRUCTION(norr, 0x34, HLFuncUndefined, HLFormatR, Sets rde to to rs1 bitwise NORed by rs2)
HL_INSTRUCTION(nori, 0x35, HLFuncUndefined, HLFormatM, Sets rde to to rs1 bitwise NORed by imm)
HL_INSTRUCTION(xorr, 0x36, HLFuncUndefined, HLFormatR, Sets rde to to rs1 bitwise XORed by rs2)
HL_INSTRUCTION(xori, 0x37, HLFuncUndefined, HLFormatM, Sets rde to to rs1 bitwise XORed by imm)
HL_INSTRUCTION(shlr, 0x38, HLFuncUndefined, HLFormatR, Sets rde to to rs1 shifted left by rs2)
HL_INSTRUCTION(shli, 0x39, HLFuncUndefined, HLFormatM, Sets rde to to rs1 shifted left by imm)
HL_INSTRUCTION(asrr, 0x3A, HLFuncUndefined, HLFormatR, Sets rde to to rs1 arithmetic shifted right by rs2)
HL_INSTRUCTION(asri, 0x3B, HLFuncUndefined, HLFormatM, Sets rde to to rs1 arithmetic shifted right by imm)
HL_INSTRUCTION(lsrr, 0x3C, HLFuncUndefined, HLFormatR, Sets rde to to rs1 logically shifted right by rs2)
HL_INSTRUCTION(lsri, 0x3D, HLFuncUndefined, HLFormatM, Sets rde to to rs1 logically shifted right by imm)
HL_INSTRUCTION(bitr, 0x3E, HLFuncUndefined, HLFormatR, Sets rde to to the rs2-th bit of rs1 if 0 <= rs2 <= 63; else 0)
HL_INSTRUCTION(biti, 0x3F, HLFuncUndefined, HLFormatM, Sets rde to to the imm-th bit of rs1 if 0 <= imm <= 63; else 0)

// SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cstdint>
#include <gtest/gtest.h>
#include <type_traits>

#include "assembler.h"
#include "memory_management_unit_p.h"
#include "system_p.h"

#define HLSignExtend64By20(value) ((int64_t)((((value) ^ (1UL << (19))) - (1UL << (19)))))

TEST(SignExtensionTest, SignExtend64By20) {
    HLInstruction instruction = ASMOpcode_bra | ASMFunc_bra | ASMImm_B(-4);
    EXPECT_EQ(HLSignExtend64By20(HLImm_B(instruction)), -4);

    instruction = ASMOpcode_bra | ASMFunc_bra | ASMImm_B(17);
    EXPECT_EQ(HLSignExtend64By20(HLImm_B(instruction)), 17);

    instruction = ASMOpcode_bra | ASMFunc_bra | ASMImm_B(-300);
    EXPECT_EQ(HLSignExtend64By20(HLImm_B(instruction)), -300);

    auto it = HLSignExtend64By20(HLImm_B(instruction));
    static_assert(std::is_same<decltype(it), int64_t>::value, "sign extension returns a int64_t");
}

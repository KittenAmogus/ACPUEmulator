#include <interpreter.h>

cmd_jmp_t jmp_map[48] = {
    {0}, // HALT
    {0}, // NOP
    {0}, // Reserved
         // JMP <imm>
    {.dest = IREG_I, .flag = IFLAG_C, .noflag = 1, .invert = 0, .reg_dest = 0},

    // JMP <reg>
    {.dest = IREG_A, .flag = IFLAG_Z, .noflag = 1, .invert = 0, .reg_dest = 1},
    {.dest = IREG_B, .flag = IFLAG_S, .noflag = 1, .invert = 0, .reg_dest = 1},
    {.dest = IREG_C, .flag = IFLAG_C, .noflag = 1, .invert = 0, .reg_dest = 1},
    {.dest = IREG_D, .flag = IFLAG_O, .noflag = 1, .invert = 0, .reg_dest = 1},

    // J<flag> <imm>
    {.dest = IREG_I, .flag = IFLAG_Z, .noflag = 0, .invert = 0, .reg_dest = 0},
    {.dest = IREG_I, .flag = IFLAG_S, .noflag = 0, .invert = 0, .reg_dest = 0},
    {.dest = IREG_I, .flag = IFLAG_C, .noflag = 0, .invert = 0, .reg_dest = 0},
    {.dest = IREG_I, .flag = IFLAG_O, .noflag = 0, .invert = 0, .reg_dest = 0},

    // JN<flag> <imm>
    {.dest = IREG_I, .flag = IFLAG_Z, .noflag = 0, .invert = 1, .reg_dest = 0},
    {.dest = IREG_I, .flag = IFLAG_S, .noflag = 0, .invert = 1, .reg_dest = 0},
    {.dest = IREG_I, .flag = IFLAG_C, .noflag = 0, .invert = 1, .reg_dest = 0},
    {.dest = IREG_I, .flag = IFLAG_O, .noflag = 0, .invert = 1, .reg_dest = 0},

    // J<flag> A
    {.dest = IREG_A, .flag = IFLAG_Z, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_A, .flag = IFLAG_S, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_A, .flag = IFLAG_C, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_A, .flag = IFLAG_O, .noflag = 0, .invert = 0, .reg_dest = 1},

    // J<flag> B
    {.dest = IREG_B, .flag = IFLAG_Z, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_B, .flag = IFLAG_S, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_B, .flag = IFLAG_C, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_B, .flag = IFLAG_O, .noflag = 0, .invert = 0, .reg_dest = 1},

    // J<flag> C
    {.dest = IREG_C, .flag = IFLAG_Z, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_C, .flag = IFLAG_S, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_C, .flag = IFLAG_C, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_C, .flag = IFLAG_O, .noflag = 0, .invert = 0, .reg_dest = 1},

    // J<flag> D
    {.dest = IREG_D, .flag = IFLAG_Z, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_D, .flag = IFLAG_S, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_D, .flag = IFLAG_C, .noflag = 0, .invert = 0, .reg_dest = 1},
    {.dest = IREG_D, .flag = IFLAG_O, .noflag = 0, .invert = 0, .reg_dest = 1},

    // JN<flag> A
    {.dest = IREG_A, .flag = IFLAG_Z, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_A, .flag = IFLAG_S, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_A, .flag = IFLAG_C, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_A, .flag = IFLAG_O, .noflag = 0, .invert = 1, .reg_dest = 1},

    // JN<flag> B
    {.dest = IREG_B, .flag = IFLAG_Z, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_B, .flag = IFLAG_S, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_B, .flag = IFLAG_C, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_B, .flag = IFLAG_O, .noflag = 0, .invert = 1, .reg_dest = 1},

    // JN<flag> C
    {.dest = IREG_C, .flag = IFLAG_Z, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_C, .flag = IFLAG_S, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_C, .flag = IFLAG_C, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_C, .flag = IFLAG_O, .noflag = 0, .invert = 1, .reg_dest = 1},

    // JN<flag> D
    {.dest = IREG_D, .flag = IFLAG_Z, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_D, .flag = IFLAG_S, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_D, .flag = IFLAG_C, .noflag = 0, .invert = 1, .reg_dest = 1},
    {.dest = IREG_D, .flag = IFLAG_O, .noflag = 0, .invert = 1, .reg_dest = 1},
};

cmd_str_t store_map[16] = {
    // 0x30
    {.src = IREG_A, .dest = IREG_I, .single = 1},
    {.src = IREG_B, .dest = IREG_A, .single = 0},
    {.src = IREG_C, .dest = IREG_A, .single = 0},
    {.src = IREG_D, .dest = IREG_A, .single = 0},
    {.src = IREG_A, .dest = IREG_B, .single = 0},
    // 0x35
    {.src = IREG_B, .dest = IREG_I, .single = 1},
    {.src = IREG_C, .dest = IREG_B, .single = 0},
    {.src = IREG_D, .dest = IREG_B, .single = 0},
    {.src = IREG_A, .dest = IREG_C, .single = 0},
    {.src = IREG_B, .dest = IREG_C, .single = 0},
    // 0x3A
    {.src = IREG_C, .dest = IREG_I, .single = 1},
    {.src = IREG_D, .dest = IREG_C, .single = 0},
    {.src = IREG_A, .dest = IREG_D, .single = 0},
    {.src = IREG_B, .dest = IREG_D, .single = 0},
    {.src = IREG_C, .dest = IREG_D, .single = 0},
    // 0x3F
    {.src = IREG_D, .dest = IREG_I, .single = 1},
};

cmd_ldr_t load_map[32] = {
    // 0x40
    {.dest = IREG_A, .src = IREG_A, .single = 0},
    {.dest = IREG_B, .src = IREG_A, .single = 0},
    {.dest = IREG_C, .src = IREG_A, .single = 0},
    {.dest = IREG_D, .src = IREG_A, .single = 0},
    // 0x44
    {.dest = IREG_A, .src = IREG_B, .single = 0},
    {.dest = IREG_B, .src = IREG_B, .single = 0},
    {.dest = IREG_C, .src = IREG_B, .single = 0},
    {.dest = IREG_D, .src = IREG_B, .single = 0},
    // 0x48
    {.dest = IREG_A, .src = IREG_C, .single = 0},
    {.dest = IREG_B, .src = IREG_C, .single = 0},
    {.dest = IREG_C, .src = IREG_C, .single = 0},
    {.dest = IREG_D, .src = IREG_C, .single = 0},
    // 0x4c
    {.dest = IREG_A, .src = IREG_D, .single = 0},
    {.dest = IREG_B, .src = IREG_D, .single = 0},
    {.dest = IREG_C, .src = IREG_D, .single = 0},
    {.dest = IREG_D, .src = IREG_D, .single = 0},

    // 0x50
    {.dest = IREG_A, .src = IREG_I, .single = 1},
    {.dest = IREG_A, .src = IREG_I, .single = 1},
    {.dest = IREG_A, .src = IREG_I, .single = 1},
    {.dest = IREG_A, .src = IREG_I, .single = 1},

    // 0x54
    {.dest = IREG_A, .src = IREG_I, .single = 1, .imm = 1},
    {.dest = IREG_A, .src = IREG_I, .single = 1, .imm = 1},
    {.dest = IREG_A, .src = IREG_I, .single = 1, .imm = 1},
    {.dest = IREG_A, .src = IREG_I, .single = 1, .imm = 1},

    // 0x58 - 0x5F (Reserved)
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

cmd_alu_t alu_map[144] = {
    // =========================================================================
    // 0x60 - 0x6F: INC (Single, ZSCO) / ADD (Double, ZSCO)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_INC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x60: inc a
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x61: add b, a
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x62: add c, a
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x63: add d, a
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x64: add a, b
    {.a = IREG_B,
     .b = 0,
     .type = IALU_INC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x65: inc b
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x66: add c, b
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x67: add d, b
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x68: add a, c
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x69: add b, c
    {.a = IREG_C,
     .b = 0,
     .type = IALU_INC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x6A: inc c
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x6B: add d, c
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x6C: add a, d
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x6D: add b, d
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_ADD,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x6E: add c, d
    {.a = IREG_D,
     .b = 0,
     .type = IALU_INC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x6F: inc d

    // =========================================================================
    // 0x70 - 0x7F: DEC (Single, ZSCO) / SUB (Double, ZSCO)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_DEC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x70: dec a
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x71: sub b, a
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x72: sub c, a
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x73: sub d, a
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x74: sub a, b
    {.a = IREG_B,
     .b = 0,
     .type = IALU_DEC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x75: dec b
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x76: sub c, b
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x77: sub d, b
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x78: sub a, c
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x79: sub b, c
    {.a = IREG_C,
     .b = 0,
     .type = IALU_DEC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x7A: dec c
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x7B: sub d, c
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x7C: sub a, d
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x7D: sub b, d
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_SUB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x7E: sub c, d
    {.a = IREG_D,
     .b = 0,
     .type = IALU_DEC,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O}, // 0x7F: dec d

    // =========================================================================
    // 0x80 - 0x8F: NOT (Single, ZS) / ADC (Double, ZSCO)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_NOT,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S},
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_B,
     .b = 0,
     .type = IALU_NOT,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S},
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_C,
     .b = 0,
     .type = IALU_NOT,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S},
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_ADC,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_D,
     .b = 0,
     .type = IALU_NOT,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S},

    // =========================================================================
    // 0x90 - 0x9F: NEG (Single, ZSCO) / SBB (Double, ZSCO)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_NEG,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_B,
     .b = 0,
     .type = IALU_NEG,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_C,
     .b = 0,
     .type = IALU_NEG,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_SBB,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},
    {.a = IREG_D,
     .b = 0,
     .type = IALU_NEG,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C | FMASK_O},

    // =========================================================================
    // 0xA0 - 0xAF: CLR (Single, None) / MOV (Double, None)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_CLR,
     .double_args = 0,
     .fmask = 0}, // 0xA0: clr a
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA1: mov b, a
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA2: mov c, a
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA3: mov d, a
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA4: mov a, b
    {.a = IREG_B,
     .b = 0,
     .type = IALU_CLR,
     .double_args = 0,
     .fmask = 0}, // 0xA5: clr b
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA6: mov c, b
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA7: mov d, b
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA8: mov a, c
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xA9: mov b, c
    {.a = IREG_C,
     .b = 0,
     .type = IALU_CLR,
     .double_args = 0,
     .fmask = 0}, // 0xAA: clr c
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xAB: mov d, c
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xAC: mov a, d
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xAD: mov b, d
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_MOV,
     .double_args = 1,
     .fmask = 0}, // 0xAE: mov c, d
    {.a = IREG_D,
     .b = 0,
     .type = IALU_CLR,
     .double_args = 0,
     .fmask = 0}, // 0xAF: clr d

    // =========================================================================
    // 0xB0 - 0xBF: TEST (Single, ZS) / AND (Double, ZS)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_TEST,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xB0: test a
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB1: and b, a
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB2: and c, a
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB3: and d, a
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB4: and a, b
    {.a = IREG_B,
     .b = 0,
     .type = IALU_TEST,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xB5: test b
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB6: and c, b
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB7: and d, b
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB8: and a, c
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xB9: and b, c
    {.a = IREG_C,
     .b = 0,
     .type = IALU_TEST,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xBA: test c
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xBB: and d, c
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xBC: and a, d
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xBD: and b, d
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_AND,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xBE: and c, d
    {.a = IREG_D,
     .b = 0,
     .type = IALU_TEST,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xBF: test d

    // =========================================================================
    // 0xC0 - 0xCF: RCL (Single, ZSC) / OR (Double, ZS)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_RCL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xC0: rcl a
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC1: or b, a
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC2: or c, a
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC3: or d, a
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC4: or a, b
    {.a = IREG_B,
     .b = 0,
     .type = IALU_RCL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xC5: rcl b
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC6: or c, b
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC7: or d, b
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC8: or a, c
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xC9: or b, c
    {.a = IREG_C,
     .b = 0,
     .type = IALU_RCL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xCA: rcl c
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xCB: or d, c
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xCC: or a, d
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xCD: or b, d
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_OR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xCE: or c, d
    {.a = IREG_D,
     .b = 0,
     .type = IALU_RCL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xCF: rcl d

    // =========================================================================
    // 0xD0 - 0xDF: RCR (Single, ZSC) / XOR (Double, ZS)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_RCR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xD0: rcr a
    {.a = IREG_B,
     .b = IREG_A,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD1: xor b, a
    {.a = IREG_C,
     .b = IREG_A,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD2: xor c, a
    {.a = IREG_D,
     .b = IREG_A,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD3: xor d, a
    {.a = IREG_A,
     .b = IREG_B,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD4: xor a, b
    {.a = IREG_B,
     .b = 0,
     .type = IALU_RCR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xD5: rcr b
    {.a = IREG_C,
     .b = IREG_B,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD6: xor c, b
    {.a = IREG_D,
     .b = IREG_B,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD7: xor d, b
    {.a = IREG_A,
     .b = IREG_C,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD8: xor a, c
    {.a = IREG_B,
     .b = IREG_C,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xD9: xor b, c
    {.a = IREG_C,
     .b = 0,
     .type = IALU_RCR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xDA: rcr c
    {.a = IREG_D,
     .b = IREG_C,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xDB: xor d, c
    {.a = IREG_A,
     .b = IREG_D,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xDC: xor a, d
    {.a = IREG_B,
     .b = IREG_D,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xDD: xor b, d
    {.a = IREG_C,
     .b = IREG_D,
     .type = IALU_XOR,
     .double_args = 1,
     .fmask = FMASK_Z | FMASK_S}, // 0xDE: xor c, d
    {.a = IREG_D,
     .b = 0,
     .type = IALU_RCR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xDF: rcr d

    // =========================================================================
    // 0xE0 - 0xEF: Сдвиги (Single, ZSC) и RND (Single, ZS)
    // =========================================================================
    {.a = IREG_A,
     .b = 0,
     .type = IALU_SHL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE0: shl a
    {.a = IREG_B,
     .b = 0,
     .type = IALU_SHL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE1: shl b
    {.a = IREG_C,
     .b = 0,
     .type = IALU_SHL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE2: shl c
    {.a = IREG_D,
     .b = 0,
     .type = IALU_SHL,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE3: shl d
    {.a = IREG_A,
     .b = 0,
     .type = IALU_SHR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE4: shr a
    {.a = IREG_B,
     .b = 0,
     .type = IALU_SHR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE5: shr b
    {.a = IREG_C,
     .b = 0,
     .type = IALU_SHR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE6: shr c
    {.a = IREG_D,
     .b = 0,
     .type = IALU_SHR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE7: shr d
    {.a = IREG_A,
     .b = 0,
     .type = IALU_SAR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE8: sar a
    {.a = IREG_B,
     .b = 0,
     .type = IALU_SAR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xE9: sar b
    {.a = IREG_C,
     .b = 0,
     .type = IALU_SAR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xEA: sar c
    {.a = IREG_D,
     .b = 0,
     .type = IALU_SAR,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S | FMASK_C}, // 0xEB: sar d
    {.a = IREG_A,
     .b = 0,
     .type = IALU_RND,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xEC: rnd a
    {.a = IREG_B,
     .b = 0,
     .type = IALU_RND,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xED: rnd b
    {.a = IREG_C,
     .b = 0,
     .type = IALU_RND,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xEE: rnd c
    {.a = IREG_D,
     .b = 0,
     .type = IALU_RND,
     .double_args = 0,
     .fmask = FMASK_Z | FMASK_S}, // 0xEF: rnd d
};

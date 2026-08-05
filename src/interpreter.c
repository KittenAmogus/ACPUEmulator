#include "interpreter.h"

const cmd_jmp_t table_jmp[] = {
  { .dest=IREG_A, .flag=IFLAG_Z, .imm=1, .noflag=1, .invert=0 },  // jmp
  { .dest=IREG_A, .flag=IFLAG_Z, .imm=0, .noflag=1, .invert=0 },  // jmp a
  { .dest=IREG_B, .flag=IFLAG_Z, .imm=0, .noflag=1, .invert=0 },  // jmp b
  { .dest=IREG_C, .flag=IFLAG_Z, .imm=0, .noflag=1, .invert=0 },  // jmp c
  { .dest=IREG_D, .flag=IFLAG_Z, .imm=0, .noflag=1, .invert=0 },  // jmp d
  { .dest=IREG_A, .flag=IFLAG_Z, .imm=1, .noflag=0, .invert=0 },  // jz
  { .dest=IREG_A, .flag=IFLAG_S, .imm=1, .noflag=0, .invert=0 },  // js
  { .dest=IREG_A, .flag=IFLAG_C, .imm=1, .noflag=0, .invert=0 },  // jc
  { .dest=IREG_A, .flag=IFLAG_O, .imm=1, .noflag=0, .invert=0 },  // jo
  { .dest=IREG_A, .flag=IFLAG_Z, .imm=1, .noflag=0, .invert=1 },  // jnz
  { .dest=IREG_A, .flag=IFLAG_S, .imm=1, .noflag=0, .invert=1 },  // jns
  { .dest=IREG_A, .flag=IFLAG_C, .imm=1, .noflag=0, .invert=1 },  // jnc
  { .dest=IREG_A, .flag=IFLAG_O, .imm=1, .noflag=0, .invert=1 },  // jno
  { .dest=IREG_A, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=0 },  // jz a
  { .dest=IREG_A, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=0 },  // js a
  { .dest=IREG_A, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=0 },  // jc a
  { .dest=IREG_A, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=0 },  // jo a
  { .dest=IREG_B, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=0 },  // jz b
  { .dest=IREG_B, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=0 },  // js b
  { .dest=IREG_B, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=0 },  // jc b
  { .dest=IREG_B, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=0 },  // jo b
  { .dest=IREG_C, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=0 },  // jz c
  { .dest=IREG_C, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=0 },  // js c
  { .dest=IREG_C, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=0 },  // jc c
  { .dest=IREG_C, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=0 },  // jo c
  { .dest=IREG_D, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=0 },  // jz d
  { .dest=IREG_D, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=0 },  // js d
  { .dest=IREG_D, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=0 },  // jc d
  { .dest=IREG_D, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=0 },  // jo d
  { .dest=IREG_A, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=1 },  // jnz a
  { .dest=IREG_A, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=1 },  // jns a
  { .dest=IREG_A, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=1 },  // jnc a
  { .dest=IREG_A, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=1 },  // jno a
  { .dest=IREG_B, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=1 },  // jnz b
  { .dest=IREG_B, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=1 },  // jns b
  { .dest=IREG_B, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=1 },  // jnc b
  { .dest=IREG_B, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=1 },  // jno b
  { .dest=IREG_C, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=1 },  // jnz c
  { .dest=IREG_C, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=1 },  // jns c
  { .dest=IREG_C, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=1 },  // jnc c
  { .dest=IREG_C, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=1 },  // jno c
  { .dest=IREG_D, .flag=IFLAG_Z, .imm=0, .noflag=0, .invert=1 },  // jnz d
  { .dest=IREG_D, .flag=IFLAG_S, .imm=0, .noflag=0, .invert=1 },  // jns d
  { .dest=IREG_D, .flag=IFLAG_C, .imm=0, .noflag=0, .invert=1 },  // jnc d
  { .dest=IREG_D, .flag=IFLAG_O, .imm=0, .noflag=0, .invert=1 },  // jno d
};

const cmd_str_t table_str[] = {
  { .src=IREG_A, .dest=IREG_A, .imm=1 },  // st a
  { .src=IREG_B, .dest=IREG_A, .imm=0 },  // st b, a
  { .src=IREG_C, .dest=IREG_A, .imm=0 },  // st c, a
  { .src=IREG_D, .dest=IREG_A, .imm=0 },  // st d, a
  { .src=IREG_A, .dest=IREG_B, .imm=0 },  // st a, b
  { .src=IREG_B, .dest=IREG_A, .imm=1 },  // st b
  { .src=IREG_C, .dest=IREG_B, .imm=0 },  // st c, b
  { .src=IREG_D, .dest=IREG_B, .imm=0 },  // st d, b
  { .src=IREG_A, .dest=IREG_C, .imm=0 },  // st a, c
  { .src=IREG_B, .dest=IREG_C, .imm=0 },  // st b, c
  { .src=IREG_C, .dest=IREG_A, .imm=1 },  // st c
  { .src=IREG_D, .dest=IREG_C, .imm=0 },  // st d, c
  { .src=IREG_A, .dest=IREG_D, .imm=0 },  // st a, d
  { .src=IREG_B, .dest=IREG_D, .imm=0 },  // st b, d
  { .src=IREG_C, .dest=IREG_D, .imm=0 },  // st c, d
  { .src=IREG_D, .dest=IREG_A, .imm=1 },  // st d
};

const cmd_ldr_t table_ldr[] = {
  { .dest=IREG_A, .src=IREG_A, .imm=0, .direct=0 },  // ld a, a
  { .dest=IREG_B, .src=IREG_A, .imm=0, .direct=0 },  // ld b, a
  { .dest=IREG_C, .src=IREG_A, .imm=0, .direct=0 },  // ld c, a
  { .dest=IREG_D, .src=IREG_A, .imm=0, .direct=0 },  // ld d, a
  { .dest=IREG_A, .src=IREG_B, .imm=0, .direct=0 },  // ld a, b
  { .dest=IREG_B, .src=IREG_B, .imm=0, .direct=0 },  // ld b, b
  { .dest=IREG_C, .src=IREG_B, .imm=0, .direct=0 },  // ld c, b
  { .dest=IREG_D, .src=IREG_B, .imm=0, .direct=0 },  // ld d, b
  { .dest=IREG_A, .src=IREG_C, .imm=0, .direct=0 },  // ld a, c
  { .dest=IREG_B, .src=IREG_C, .imm=0, .direct=0 },  // ld b, c
  { .dest=IREG_C, .src=IREG_C, .imm=0, .direct=0 },  // ld c, c
  { .dest=IREG_D, .src=IREG_C, .imm=0, .direct=0 },  // ld d, c
  { .dest=IREG_A, .src=IREG_D, .imm=0, .direct=0 },  // ld a, d
  { .dest=IREG_B, .src=IREG_D, .imm=0, .direct=0 },  // ld b, d
  { .dest=IREG_C, .src=IREG_D, .imm=0, .direct=0 },  // ld c, d
  { .dest=IREG_D, .src=IREG_D, .imm=0, .direct=0 },  // ld d, d
  { .dest=IREG_A, .src=IREG_A, .imm=1, .direct=0 },  // ld a
  { .dest=IREG_B, .src=IREG_A, .imm=1, .direct=0 },  // ld b
  { .dest=IREG_C, .src=IREG_A, .imm=1, .direct=0 },  // ld c
  { .dest=IREG_D, .src=IREG_A, .imm=1, .direct=0 },  // ld d
  { .dest=IREG_A, .src=IREG_A, .imm=1, .direct=1 },  // ldi a
  { .dest=IREG_B, .src=IREG_A, .imm=1, .direct=1 },  // ldi b
  { .dest=IREG_C, .src=IREG_A, .imm=1, .direct=1 },  // ldi c
  { .dest=IREG_D, .src=IREG_A, .imm=1, .direct=1 },  // ldi d
};

const cmd_alu_t table_alu[] = {
  { .type=IALU_INC, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x03 },  // inc a
  { .type=IALU_ADD, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x0f },  // add b, a
  { .type=IALU_ADD, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x0f },  // add c, a
  { .type=IALU_ADD, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x0f },  // add d, a
  { .type=IALU_ADD, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x0f },  // add a, b
  { .type=IALU_INC, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x03 },  // inc b
  { .type=IALU_ADD, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x0f },  // add c, b
  { .type=IALU_ADD, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x0f },  // add d, b
  { .type=IALU_ADD, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x0f },  // add a, c
  { .type=IALU_ADD, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x0f },  // add b, c
  { .type=IALU_INC, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x03 },  // inc c
  { .type=IALU_ADD, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x0f },  // add d, c
  { .type=IALU_ADD, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x0f },  // add a, d
  { .type=IALU_ADD, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x0f },  // add b, d
  { .type=IALU_ADD, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x0f },  // add c, d
  { .type=IALU_INC, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x03 },  // inc d
  { .type=IALU_DEC, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x03 },  // dec a
  { .type=IALU_SUB, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x0f },  // sub b, a
  { .type=IALU_SUB, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x0f },  // sub c, a
  { .type=IALU_SUB, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x0f },  // sub d, a
  { .type=IALU_SUB, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x0f },  // sub a, b
  { .type=IALU_DEC, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x03 },  // dec b
  { .type=IALU_SUB, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x0f },  // sub c, b
  { .type=IALU_SUB, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x0f },  // sub d, b
  { .type=IALU_SUB, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x0f },  // sub a, c
  { .type=IALU_SUB, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x0f },  // sub b, c
  { .type=IALU_DEC, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x03 },  // dec c
  { .type=IALU_SUB, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x0f },  // sub d, c
  { .type=IALU_SUB, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x0f },  // sub a, d
  { .type=IALU_SUB, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x0f },  // sub b, d
  { .type=IALU_SUB, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x0f },  // sub c, d
  { .type=IALU_DEC, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x03 },  // dec d
  { .type=IALU_NOT, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x03 },  // not a
  { .type=IALU_ADC, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x0f },  // adc b, a
  { .type=IALU_ADC, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x0f },  // adc c, a
  { .type=IALU_ADC, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x0f },  // adc d, a
  { .type=IALU_ADC, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x0f },  // adc a, b
  { .type=IALU_NOT, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x03 },  // not b
  { .type=IALU_ADC, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x0f },  // adc c, b
  { .type=IALU_ADC, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x0f },  // adc d, b
  { .type=IALU_ADC, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x0f },  // adc a, c
  { .type=IALU_ADC, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x0f },  // adc b, c
  { .type=IALU_NOT, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x03 },  // not c
  { .type=IALU_ADC, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x0f },  // adc d, c
  { .type=IALU_ADC, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x0f },  // adc a, d
  { .type=IALU_ADC, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x0f },  // adc b, d
  { .type=IALU_ADC, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x0f },  // adc c, d
  { .type=IALU_NOT, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x03 },  // not d
  { .type=IALU_NEG, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x03 },  // neg a
  { .type=IALU_SBB, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x0f },  // sbb b, a
  { .type=IALU_SBB, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x0f },  // sbb c, a
  { .type=IALU_SBB, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x0f },  // sbb d, a
  { .type=IALU_SBB, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x0f },  // sbb a, b
  { .type=IALU_NEG, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x03 },  // neg b
  { .type=IALU_SBB, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x0f },  // sbb c, b
  { .type=IALU_SBB, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x0f },  // sbb d, b
  { .type=IALU_SBB, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x0f },  // sbb a, c
  { .type=IALU_SBB, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x0f },  // sbb b, c
  { .type=IALU_NEG, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x03 },  // neg c
  { .type=IALU_SBB, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x0f },  // sbb d, c
  { .type=IALU_SBB, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x0f },  // sbb a, d
  { .type=IALU_SBB, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x0f },  // sbb b, d
  { .type=IALU_SBB, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x0f },  // sbb c, d
  { .type=IALU_NEG, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x03 },  // neg d
  { .type=IALU_CLR, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x00 },  // clr a
  { .type=IALU_MOV, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x00 },  // mov b, a
  { .type=IALU_MOV, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x00 },  // mov c, a
  { .type=IALU_MOV, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x00 },  // mov d, a
  { .type=IALU_MOV, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x00 },  // mov a, b
  { .type=IALU_CLR, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x00 },  // clr b
  { .type=IALU_MOV, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x00 },  // mov c, b
  { .type=IALU_MOV, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x00 },  // mov d, b
  { .type=IALU_MOV, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x00 },  // mov a, c
  { .type=IALU_MOV, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x00 },  // mov b, c
  { .type=IALU_CLR, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x00 },  // clr c
  { .type=IALU_MOV, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x00 },  // mov d, c
  { .type=IALU_MOV, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x00 },  // mov a, d
  { .type=IALU_MOV, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x00 },  // mov b, d
  { .type=IALU_MOV, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x00 },  // mov c, d
  { .type=IALU_CLR, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x00 },  // clr d
  { .type=IALU_TEST, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x03 },  // test a
  { .type=IALU_AND, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x03 },  // and b, a
  { .type=IALU_AND, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x03 },  // and c, a
  { .type=IALU_AND, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x03 },  // and d, a
  { .type=IALU_AND, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x03 },  // and a, b
  { .type=IALU_TEST, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x03 },  // test b
  { .type=IALU_AND, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x03 },  // and c, b
  { .type=IALU_AND, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x03 },  // and d, b
  { .type=IALU_AND, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x03 },  // and a, c
  { .type=IALU_AND, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x03 },  // and b, c
  { .type=IALU_TEST, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x03 },  // test c
  { .type=IALU_AND, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x03 },  // and d, c
  { .type=IALU_AND, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x03 },  // and a, d
  { .type=IALU_AND, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x03 },  // and b, d
  { .type=IALU_AND, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x03 },  // and c, d
  { .type=IALU_TEST, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x03 },  // test d
  { .type=IALU_RCL, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcl a
  { .type=IALU_OR, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x03 },  // or b, a
  { .type=IALU_OR, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x03 },  // or c, a
  { .type=IALU_OR, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x03 },  // or d, a
  { .type=IALU_OR, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x03 },  // or a, b
  { .type=IALU_RCL, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcl b
  { .type=IALU_OR, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x03 },  // or c, b
  { .type=IALU_OR, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x03 },  // or d, b
  { .type=IALU_OR, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x03 },  // or a, c
  { .type=IALU_OR, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x03 },  // or b, c
  { .type=IALU_RCL, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcl c
  { .type=IALU_OR, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x03 },  // or d, c
  { .type=IALU_OR, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x03 },  // or a, d
  { .type=IALU_OR, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x03 },  // or b, d
  { .type=IALU_OR, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x03 },  // or c, d
  { .type=IALU_RCL, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcl d
  { .type=IALU_RCR, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcr a
  { .type=IALU_XOR, .a=IREG_B, .b=IREG_A, .dbl=1, .fmask=0x03 },  // xor b, a
  { .type=IALU_XOR, .a=IREG_C, .b=IREG_A, .dbl=1, .fmask=0x03 },  // xor c, a
  { .type=IALU_XOR, .a=IREG_D, .b=IREG_A, .dbl=1, .fmask=0x03 },  // xor d, a
  { .type=IALU_XOR, .a=IREG_A, .b=IREG_B, .dbl=1, .fmask=0x03 },  // xor a, b
  { .type=IALU_RCR, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcr b
  { .type=IALU_XOR, .a=IREG_C, .b=IREG_B, .dbl=1, .fmask=0x03 },  // xor c, b
  { .type=IALU_XOR, .a=IREG_D, .b=IREG_B, .dbl=1, .fmask=0x03 },  // xor d, b
  { .type=IALU_XOR, .a=IREG_A, .b=IREG_C, .dbl=1, .fmask=0x03 },  // xor a, c
  { .type=IALU_XOR, .a=IREG_B, .b=IREG_C, .dbl=1, .fmask=0x03 },  // xor b, c
  { .type=IALU_RCR, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcr c
  { .type=IALU_XOR, .a=IREG_D, .b=IREG_C, .dbl=1, .fmask=0x03 },  // xor d, c
  { .type=IALU_XOR, .a=IREG_A, .b=IREG_D, .dbl=1, .fmask=0x03 },  // xor a, d
  { .type=IALU_XOR, .a=IREG_B, .b=IREG_D, .dbl=1, .fmask=0x03 },  // xor b, d
  { .type=IALU_XOR, .a=IREG_C, .b=IREG_D, .dbl=1, .fmask=0x03 },  // xor c, d
  { .type=IALU_RCR, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x07 },  // rcr d
  { .type=IALU_SHL, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shl a
  { .type=IALU_SHL, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shl b
  { .type=IALU_SHL, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shl c
  { .type=IALU_SHL, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shl d
  { .type=IALU_SHR, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shr a
  { .type=IALU_SHR, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shr b
  { .type=IALU_SHR, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shr c
  { .type=IALU_SHR, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x07 },  // shr d
  { .type=IALU_SAR, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x07 },  // sar a
  { .type=IALU_SAR, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x07 },  // sar b
  { .type=IALU_SAR, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x07 },  // sar c
  { .type=IALU_SAR, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x07 },  // sar d
  { .type=IALU_RND, .a=IREG_A, .b=IREG_A, .dbl=0, .fmask=0x03 },  // rnd a
  { .type=IALU_RND, .a=IREG_B, .b=IREG_A, .dbl=0, .fmask=0x03 },  // rnd b
  { .type=IALU_RND, .a=IREG_C, .b=IREG_A, .dbl=0, .fmask=0x03 },  // rnd c
  { .type=IALU_RND, .a=IREG_D, .b=IREG_A, .dbl=0, .fmask=0x03 },  // rnd d
};


// Counts:
// JMP: 45
// LDR: 16
// STR: 24
// ALU: 144
// Total: 229
// Fixed: 256 (+ Reserved + HLT + NOP)

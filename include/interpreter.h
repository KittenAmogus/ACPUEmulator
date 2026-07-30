#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdint.h>

typedef enum {
  ICMD_NOP = 0x00,
  ICMD_HLT = 0x01,
} interp_cmd_e;

typedef enum {
  IREG_A = 0,
  IREG_B = 1,
  IREG_C = 2,
  IREG_D = 3,
  IREG_I = 4,
} interp_regs_e;

typedef enum {
  IFLAG_Z = 0, // Zero
  IFLAG_S = 1, // Sign
  IFLAG_C = 2, // Carry
  IFLAG_O = 3, // Overflow
} interp_flags_e;

typedef enum {
  FMASK_Z = (1 << 0), // Zero
  FMASK_S = (1 << 1), // Sign
  FMASK_C = (1 << 2), // Carry
  FMASK_O = (1 << 3), // Overflow
} alu_flags_e;

typedef enum {
  IALU_CLR = 0,
  IALU_MOV = 1,
  IALU_AND = 2,
  IALU_OR = 3,
  IALU_XOR = 4,
  IALU_ADD = 5,
  IALU_ADC = 6,
  IALU_SUB = 7,
  IALU_SBB = 8,
  IALU_TEST = 9,
  IALU_INC = 10,
  IALU_DEC = 11,
  IALU_NOT = 12,
  IALU_NEG = 13,
  IALU_RND = 14,
  IALU_SHL = 15,
  IALU_SHR = 16,
  IALU_SAR = 17,
  IALU_RCL = 18,
  IALU_RCR = 19,
} interp_alu_e;

typedef struct {
  uint16_t dest : 8;     // Destination addr
  uint16_t flag : 2;     // Flag to check
  uint16_t noflag : 1;   // Do not check flag
  uint16_t invert : 1;   // Invert flag (jz / jnz)
  uint16_t reg_dest : 1; // Destination is not addr but register with addr
  uint16_t _rsvd : 4;
} __attribute__((packed)) cmd_jmp_t;

typedef struct {
  uint16_t src : 8;  // source register
  uint16_t dest : 8; // Destination register (unused if is_single)

  uint16_t single : 1; // Dest as operand
  uint16_t _rsvd : 15;
} __attribute__((packed)) cmd_str_t;

typedef struct {
  uint32_t dest : 8;   // Destination register
  uint32_t src : 8;    // Source register (unused if is_single)
  uint32_t single : 1; // Src as operand
  uint32_t imm : 1;    // Src is value instead of addr
  uint32_t _rsvd : 14;
} __attribute__((packed)) cmd_ldr_t;

typedef struct {
  uint32_t a : 8;           // First reg (dest)
  uint32_t b : 8;           // Second reg
  uint32_t type : 9;        // CMD type
  uint32_t fmask : 4;       // Affect mask
  uint32_t double_args : 1; // Has second arg
  uint32_t _rsvd : 2;
} __attribute__((packed)) cmd_alu_t;

extern cmd_jmp_t jmp_map[48];
extern cmd_str_t store_map[16];
extern cmd_ldr_t load_map[32];
extern cmd_alu_t alu_map[144];

#endif // INTERPRETER_H

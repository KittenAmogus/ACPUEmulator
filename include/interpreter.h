#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdint.h>

typedef enum {
  IREG_A = 0,
  IREG_B = 1,
  IREG_C = 2,
  IREG_D = 3,
} interpreter_reg_e;

typedef enum {
  IFLAG_Z = 0,
  IFLAG_S = 1,
  IFLAG_C = 2,
  IFLAG_O = 3,
} interpreter_flag_e;

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
} interpreter_alu_e;

typedef struct {
  uint8_t dest : 2;   // Destination
  uint8_t flag : 2;   // Flag to check
  uint8_t imm : 1;    // Is dest in next byte
  uint8_t noflag : 1; // Do not check flag
  uint8_t invert : 1; // Invert flag value

  uint8_t _pad : 1;
} __attribute__((packed)) cmd_jmp_t;

typedef struct {
  uint8_t src : 2;  // Source
  uint8_t dest : 2; // Destination
  uint8_t imm : 1;  // Is destination in next byte

  uint8_t _pad : 3;
} __attribute__((packed)) cmd_str_t;

typedef struct {
  uint8_t dest : 2;   // Destination
  uint8_t src : 2;    // Source
  uint8_t imm : 1;    // Is source in next byte
  uint8_t direct : 1; // Source is value instead of addr

  uint8_t _pad : 2;
} __attribute__((packed)) cmd_ldr_t;

typedef struct {
  uint16_t type : 5;  // ALU type
  uint16_t a : 2;     // First reg (dest)
  uint16_t b : 2;     // Second reg
  uint16_t dbl : 1;   // Second reg needed
  uint16_t fmask : 4; // Update flag mask

  uint16_t _pad : 2;
} __attribute__((packed)) cmd_alu_t;

#endif // INTERPRETER_H

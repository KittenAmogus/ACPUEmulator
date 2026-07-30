#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct CPU {
  struct {
    uint8_t ip; // Instruction pointer
    uint8_t ir; // Instruction register

    union {
      struct {
        uint32_t a : 8;            // GP0
        uint32_t b : 8;            // GP1
        uint32_t c : 8;            // GP2
        uint32_t d : 8;            // GP3
      } __attribute__((packed)) r; // By name
      uint8_t gp[4];               // By index
    } gp;                          // General-purpose regs

    // Flags
    union {
      struct {
        uint32_t z : 1; // Zero
        uint32_t s : 1; // Sign
        uint32_t c : 1; // Carry
        uint32_t o : 1; // Overflow
        uint32_t _pad : 24;
      } __attribute__((packed)) b;
      uint32_t w; // Full register
    } f;
  } regs; // Registers

} cpu_t;

#endif // CPU_H

#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct CPU {
  uint8_t ip; // Instruction pointer
  uint8_t ir; // instruction register

  union {
    struct {
      uint8_t a; // GP0
      uint8_t b; // GP1
      uint8_t c; // GP2
      uint8_t d; // GP3
    } __attribute__((packed));

    uint8_t gp[4]; // General-purpose regs
  };

  union {
    struct {
      uint32_t z : 1;            // Zero
      uint32_t s : 1;            // Sign
      uint32_t c : 1;            // Carry
      uint32_t o : 1;            // Overflow
      uint32_t _pad : 28;        // Padding (reserved)
    } __attribute__((packed)) b; // Access as bits
    uint32_t w;                  // Access as word
  } f;

} cpu_t;

#endif // CPU_H

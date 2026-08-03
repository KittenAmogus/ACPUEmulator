#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <cpu.h>
#include <ram.h>

typedef struct CU {
  struct RAM RAM;
  struct CPU CPU;
} control_unit_t;

control_unit_t *cu_init(void);    // Init CU
int cu_step(control_unit_t *cu);  // Load and execute next instruction
void cu_free(control_unit_t *cu); // Free CU

#endif // CONTROL_UNIT_H

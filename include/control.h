#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>

#include <cpu.h>
#include <ram.h>

typedef struct control_unit {
  struct CPU cpu;
  struct RAM ram;
} control_unit_t;

int cu_update(control_unit_t *cu);

#endif // CONTROL_H

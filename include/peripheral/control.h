#ifndef PERI_CONTROL_H
#define PERI_CONTROL_H

#include <control.h>
#include <peri.h>

#define PORT_BANK 0x3F

typedef struct {
  control_unit_t *cu;
} control_data_t;

extern peripheral_t per_control;

#endif // PERI_CONTROL_H

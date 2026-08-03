#ifndef PERI_DISPLAY_H
#define PERI_DISPLAY_H

#include <control.h>
#include <peri.h>

#define PORT_IO 0x3E
#define BIT_DISPLAY (1 << 4)
#define BIT_COLOR (1 << 5)

#define ADDR_DISPLAY_START 0x40
#define ADDR_DISPLAY_END 0x5F
#define ADDR_DISPLAY_END_COLOR 0x7F
#define DISPLAY_SIZE 16 // 16x16
#define DISPLAY_BLUE_OFFSET 0x20
#define BLUE_OFFSET (DISPLAY_SIZE * DISPLAY_SIZE)

#define DISPLAY_PIXEL_SIZE 40
#define DISPLAY_WINDOW_SIZE (DISPLAY_SIZE * DISPLAY_PIXEL_SIZE)

typedef struct {
  int dirty;
  int active;
  int colormode;
  control_unit_t *cu;
} display_data_t;

extern peripheral_t per_display;

#endif // PERI_DISPLAY_H

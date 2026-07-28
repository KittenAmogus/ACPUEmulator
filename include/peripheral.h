#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <control.h>
#include <gui.h>

typedef int (*peripheral_create_t)(control_unit_t *cu);
typedef int (*peripheral_update_t)(control_unit_t *cu);
typedef int (*peripheral_destroy_t)(control_unit_t *cu);
typedef int (*peripheral_onevent_t)(control_unit_t *cu, SDL_Event evt);

typedef struct {
  int init;
  gui_data_t gui;
  peripheral_create_t create;
  peripheral_update_t update;
  peripheral_destroy_t destroy;
  peripheral_onevent_t onevent;
} peripheral_t;

#endif // PERIPHERAL_H

#include "SDL_events.h"
#include <control.h>
#include <display.h>
#include <gui.h>
#include <peripheral.h>

#include <ram.h>
#include <stdio.h>

static const char *TITLE = "ACPU Display";
static gui_data_t gui;

int display_create(control_unit_t *cu) {
  if (gui.init)
    return 1;

  if (!gui_init(&gui, TITLE, DISPLAY_SIZE, DISPLAY_SIZE))
    return 0;
  per_display.gui = gui;
  per_display.init = 1;

  printf("DISPLAY: Created display %dx%d\r\n", DISPLAY_SIZE, DISPLAY_SIZE);
  return 1;
}

int display_update(control_unit_t *cu) {
  if (!gui.init)
    return 0;

  // Clear
  SDL_SetRenderDrawColor(gui.renderer, 255, 255, 255, 255);
  SDL_RenderClear(gui.renderer);

  // Redraw

  // Flush
  SDL_RenderPresent(gui.renderer);

  return 1;
}

int display_destroy(control_unit_t *cu) {
  if (!gui.init)
    return 1;

  printf("DISPLAY: Destroyed display\r\n");
  per_display.init = 0;
  return gui_quit(&gui);
}

int display_onevent(control_unit_t *cu, SDL_Event evt) {
  if (evt.type == SDL_QUIT)
    return display_destroy(cu);

  return 1;
}

peripheral_t per_display = {
    .init = 0,
    .gui = {0},
    .create = display_create,
    .update = display_update,
    .destroy = display_destroy,
    .onevent = display_onevent,
};

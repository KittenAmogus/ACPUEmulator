#include <control.h>
#include <gui.h>
#include <peripheral/display.h>
#include <peripheral/peripheral.h>

#include <SDL2/SDL.h>

#define LOG_MODULE "DISPLAY"
#include <log.h>

static int display_getsize(int *width, int *height) {
  *width = 600;
  *height = 600;
  return 1;
}

static int display_redraw(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
  SDL_RenderClear(renderer);
  return 1;
}

static int display_onfocus(SDL_Event *evt) {
  // LOG_DEBUG("Focus %d", evt->window.windowID);
  return 1;
}

static int display_onmouse(SDL_Event *evt) {
  // LOG_DEBUG("Mouse %d", evt->motion.windowID);
  return 1;
}

static int display_onkey(SDL_Event *evt) {
  // LOG_DEBUG("Key %d", evt->key.windowID);
  return 1;
}

static int display_create(control_unit_t *cu, gui_data_t *gui) {}

static int display_update(void) {}

static int display_remove(void) {}

static uint8_t display_read_byte(uint8_t port) {}

static void display_write_byte(uint8_t port, uint8_t byte) {}

peripheral_t per_display = {
    .id = 0,
    .init = 0,
    .active = 0,

    .gui = -1,
    .cu = NULL,

    .redraw = display_redraw,
    .onfocus = display_onfocus,
    .onmouse = display_onmouse,
    .onkey = display_onkey,

    .getsize = display_getsize,
    .create = display_create,
    .update = display_update,
    .remove = display_remove,

    .read_byte = display_read_byte,
    .write_byte = display_write_byte,
};

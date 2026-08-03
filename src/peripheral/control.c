#define LOG_MODULE "CONTROL"
#include <logger.h>

#include <control.h>
#include <gui.h>
#include <peri.h>
#include <peripheral/control.h>
#include <ports.h>
#include <ram.h>

static const char *TITLE = "Control menu";
static control_data_t data;

static void on_port_write(uint8_t port, uint8_t value) {
  switch (port) {
  case PORT_BANK:
    data.cu->ram.active_bank_id =
        (value > BANK_START ? value : BANK_START) % BANK_COUNT;
    LOG_DEBUG("Switched bank to %d", data.cu->ram.active_bank_id);
    break;
  }
}

static int redraw(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(renderer);

  SDL_Rect rect = {
      .x = 100,
      .y = 100,
      .w = 256,
      .h = 256,
  };

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
  SDL_RenderDrawRect(renderer, &rect);

  return 1;
}

static int onfocus(SDL_Event *event) {
  if (event->type == SDL_WINDOWEVENT_FOCUS_GAINED) {

  } else if (event->type == SDL_WINDOWEVENT_FOCUS_LOST) {
  }

  return 1;
}

static int onmouse(SDL_Event *event) {
  if (event->type == SDL_MOUSEBUTTONDOWN) {

  } else if (event->type == SDL_MOUSEWHEEL) {
  }

  return 1;
}

static int onkey(SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
  }

  return 1;
}

static int getsize(int *width, int *height) {
  if (width)
    *width = 800;
  if (height)
    *height = 600;
  return 1;
}

static int create(control_unit_t *cu) {
  data.cu = cu;

  port_register(PORT_BANK, PORT_BANK, NULL, on_port_write);
  return 1;
}

static int gettitle(const char **title) {
  if (title)
    *title = TITLE;
  return 1;
}

static int update(void) { return 1; }

static int _remove(void) {
  data.cu = NULL;
  return 1;
}

peripheral_t per_control = {
    .id = 0,
    .init = 0,
    .active = 0,

    .gui = -1,
    .cu = NULL,

    .redraw = redraw,

    .onfocus = onfocus,
    .onmouse = onmouse,
    .onkey = onkey,

    .getsize = getsize,
    .gettitle = gettitle,
    .create = create,
    .update = update,
    .remove = _remove, // Conflicting type if without '_'
};

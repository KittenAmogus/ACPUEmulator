#define LOG_MODULE "DISPLAY"
#include <logger.h>

#include <control.h>
#include <gui.h>
#include <peri.h>
#include <peripheral/display.h>
#include <ports.h>
#include <ram.h>
#include <stdint.h>

static const char *TITLE = "Display";

static uint32_t DISPLAY_COLORS[] = {
    0x00, 0x00, 0x00, // 0b00 - BLACK
    0x00, 0x00, 0xFF, // 0b01 - BLUE
    0xFF, 0x00, 0x00, // 0b10 - RED
    0xFF, 0x00, 0xFF, // 0b11 - BLUE & RED
    0xFF, 0xFF, 0xFF, // Mono - WHITE
};

uint8_t vidmem[DISPLAY_SIZE * DISPLAY_SIZE * 2];
static display_data_t data;

static void on_port_write(uint8_t port, uint8_t value) {
  if (port >= ADDR_DISPLAY_START && port <= ADDR_DISPLAY_END_COLOR) {

    // Do not react if not active
    if (!data.active) {
      LOG_WARNING("Update unitialized display");
      return;
    }

    // Do not react if not in range of current mode
    // if ((!data.colormode) && port > ADDR_DISPLAY_END)
    //  return;

    // Change vidmem

    int pixel = (port - ADDR_DISPLAY_START);
    pixel = (8 * pixel);
    for (int i = 0; i < 8; ++i) {
      // vidmem[(8 * (port - ADDR_DISPLAY_START)) + i] =
      // (value >> (i)) & 1; // 0->7
      vidmem[pixel + i] = (0x80 - (value >> (7 - i))) & 1;
      // vidmem[pixel + i] = (0x80 - (value >> (i))) & 1;
    }

    // Need redraw
    data.dirty = 1;
    return;
  }

  switch (port) {
  case PORT_IO:
    if (value & BIT_DISPLAY) {
      data.active = 1;
    } else {
      data.active = 0;
    }

    if (value & BIT_COLOR) {
      data.colormode = 1;
    } else {
      data.colormode = 0;
    }

    LOG_DEBUG("Display IO changed: ACTIVE=%d, COLOR=%d", data.active,
              data.colormode);
    // memset(vidmem, 0, (DISPLAY_SIZE * DISPLAY_SIZE * 2));
    break;
  }
}

static int redraw(SDL_Renderer *renderer) {
  if (!data.dirty)
    return 1;

  LOG_DEBUG("DRAWING");

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(renderer);

  SDL_Rect rect;
  rect.w = DISPLAY_PIXEL_SIZE;
  rect.h = DISPLAY_PIXEL_SIZE;

  int colID;
  int red, blue;
  int r, g, b;

  for (int i = 0; i < (DISPLAY_SIZE * DISPLAY_SIZE); ++i) {
    rect.x = (i % DISPLAY_SIZE) * DISPLAY_PIXEL_SIZE;
    rect.y = (i / DISPLAY_SIZE) * DISPLAY_PIXEL_SIZE;

    red = vidmem[i];
    blue = vidmem[i + BLUE_OFFSET];
    if (data.colormode) {
      colID = 3 * ((red << 1) | blue);
    } else {
      colID = ((red || blue) ? 3 * 4 : 0);
    }

    // LOG_WARNING("RED =vidmem[%02x + %d]", 0, i);
    // LOG_WARNING("BLUE=vidmem[%02x + %d]", BLUE_OFFSET, i);

    r = DISPLAY_COLORS[colID + 0];
    g = DISPLAY_COLORS[colID + 1];
    b = DISPLAY_COLORS[colID + 2];

    // LOG_WARNING("COL=%d(%d, %d), R=%d, G=%d, B=%d", colID, red, blue, r, g,
    // b);

    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
  }

  /*SDL_Rect rect = {
      .x = 100,
      .y = 100,
      .w = 256,
      .h = 256,
  };

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
  SDL_RenderDrawRect(renderer, &rect);*/

  data.dirty = 0;
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

static int gettitle(const char **title) {
  if (title)
    *title = TITLE;
  return 1;
}

static int getsize(int *width, int *height) {
  if (width)
    *width = DISPLAY_WINDOW_SIZE;
  if (height)
    *height = DISPLAY_WINDOW_SIZE;
  return 1;
}

static int create(control_unit_t *cu) {
  data.cu = cu;
  data.dirty = 1;
  data.active = 0;
  data.colormode = 0;

  memset(vidmem, 0, sizeof(vidmem));

  port_register(PORT_IO, PORT_IO, NULL, on_port_write);
  port_register(ADDR_DISPLAY_START, ADDR_DISPLAY_END_COLOR, NULL,
                on_port_write);
  return 1;
}

static int update(void) { return 1; }

static int _remove(void) {
  data.cu = NULL;
  return 1;
}

peripheral_t per_display = {
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

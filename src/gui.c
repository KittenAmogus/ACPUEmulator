#include <SDL2/SDL.h>
#include <control.h>
#include <gui.h>

#define GUI_WINDOWWIDTH 1600
#define GUI_WINDOWHEIGHT 1000
#define GUI_DISPLAYWIDTH 1000
#define GUI_DISPLAYHEIGHT 1000

int gui_init(gui_data_t *data, const char *title, int w, int h) {
  if (data == NULL)
    return 0;

  data->window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w,
                       h, SDL_WINDOW_SHOWN);
  if (data->window == NULL)
    return 0;

  data->renderer = SDL_CreateRenderer(
      data->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  data->init = 1;
  return 1;
}

int gui_quit(gui_data_t *data) {
  if (data == NULL)
    return 0;

  SDL_DestroyWindow(data->window);
  SDL_DestroyRenderer(data->renderer);
  data->init = 0;
  data->renderer = NULL;
  data->window = NULL;

  return 1;
}

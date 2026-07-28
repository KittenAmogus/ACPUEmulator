#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

typedef struct {
  int init;
  SDL_Event event;
  SDL_Window *window;
  SDL_Renderer *renderer;
} gui_data_t;

int gui_init(gui_data_t *data, const char *title, int w, int h);
int gui_quit(gui_data_t *data);

#endif // GUI_H

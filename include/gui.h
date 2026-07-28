#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} gui_data;

int gui_init(void);
void gui_exit(void);
int gui_update(void);

#endif // GUI_H

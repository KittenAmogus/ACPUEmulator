#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_MAINWINDOW_WIDTH 1280
#define GUI_MAINWINDOW_HEIGHT 720
#define GUI_MAX_TEXTURES 16

#define GUI_MAIN_TITLE "ACPU Control window"

#define GUI_RENDERER_FLAGS                                                     \
  (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

typedef struct {
  int id;
  int init;
  int detached;

  int offx;
  int offy;

  SDL_Window *window;
  SDL_Texture *texture;
  SDL_Renderer *renderer;
} gui_data_t;

int gui_create_texture(int parent, int width, int height, int offx, int offy);
int gui_detach_texture(int texture, const char *window_title);
int gui_move_texture(int texture, int offx, int offy);
int gui_attach_texture(int texture, int parent);
int gui_remove_texture(int texture);
int gui_redraw_texture(int texture);
int gui_handle_evt(SDL_Event *evt);

#endif // GUI_H

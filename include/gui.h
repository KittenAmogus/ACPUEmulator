#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_MAINWINDOW_WIDTH 1280
#define GUI_MAINWINDOW_HEIGHT 720
#define GUI_MAX_TEXTURES 16

#define GUI_MAIN_TITLE "ACPU Control window"

#define GUI_RENDERER_FLAGS                                                     \
  (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

typedef int (*gui_redraw_t)(SDL_Renderer *renderer);
typedef int (*gui_onmouse_t)(SDL_Event *evt);
typedef int (*gui_onfocus_t)(SDL_Event *evt);
typedef int (*gui_onkey_t)(SDL_Event *evt);

typedef struct {
  // Global data
  int id;       // ID in GUI private array
  int init;     // Is initialized
  int detached; // Is detached (owns window)

  // Offset if attached
  int offx; // X Offset from left
  int offy; // Y Offset from top

  // GUI Private pointers
  SDL_Window *window;     // Window (Can be parent's)
  SDL_Texture *texture;   // Texture (Must be different)
  SDL_Renderer *renderer; // Renderer (Can be parent's)

  // Peripheral private functions
  gui_redraw_t _redraw; // On gui_redraw_texture

  gui_onkey_t _onkey;     // On KEYDOWN/KEYUP events
  gui_onfocus_t _onfocus; // On FOCUS_(GAINED/LOST) events,
                          // On WINDOW(MINIMIZED/RESTORED) events
  gui_onmouse_t _onmouse; // On MOUSE(BUTTONDOWN/BUTTONUP/MOTION) events
} gui_data_t;

int gui_create_texture(int parent, int width, int height, int offx,
                       int offy);    // Create texture
int gui_remove_texture(int texture); // Delete texture (and window if detached)

int gui_detach_texture(int texture,
                       const char *window_title); // Detach into separate window
int gui_attach_texture(int texture,
                       int parent); // Attach to parent and destroy window

int gui_move_texture(int texture, int offx, int offy);     // Change offsets
int gui_rename_window(int texture, const char *new_title); // Change title

int gui_redraw_texture(int texture); // Redraw texture (and window if detached)
int gui_handle_evt(SDL_Event *evt);  // Handle event in parent's window

#endif // GUI_H

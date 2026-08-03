#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_MAX_GUIS 16

#define GUI_RENDERER_FLAGS                                                     \
  (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

// If not set, child detaches instead of destroying
#define ONCLOSE_DESTROY // Destroy window if parent destroyed

typedef int (*gui_redraw_t)(SDL_Renderer *renderer);

extern void per_on_gui_moved(int oldID, int newID);

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

  gui_redraw_t redraw; // GUI private function
} gui_data_t;

typedef struct {
  gui_data_t pool[GUI_MAX_GUIS];
  int count;
} gui_mgr_t;

/* Signle GUI funcs */
int gui_check_windows(int gui1, int gui2);
int gui_check_windows_ID(int gui1, int winID2);
int gui_link_redraw(int gui, gui_redraw_t func);

int gui_create(int parent_gui, int width, int height, int offx, int offy,
               const char *title);
int gui_destroy(int gui);
int gui_redraw(int gui);

int gui_get_offset(int gui, int *offxptr, int *offyptr);
int gui_get_title(int gui, const char **titleptr);

int gui_set_offset(int gui, int offx, int offy);
int gui_set_title(int gui, const char *title);

int gui_get_detached(int gui);

int gui_attach(int gui, int parent_gui);
int gui_detach(int gui, int width, int height, const char *title);

/* Global GUI funcs */
int gui_destroyall(void);

#endif // GUI_H

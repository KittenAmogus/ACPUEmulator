#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_LIMIT 16

#define GUI_RENDERER_F (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
#define GUI_WINPOS (SDL_WINDOWPOS_CENTERED)
#define GUI_TEXTURE_FMT (SDL_PIXELFORMAT_RGBA8888)
#define GUI_TEXTURE_ACC (SDL_TEXTUREACCESS_TARGET)

typedef int (*gui_redraw_t)(SDL_Renderer *renderer);

typedef struct {
  int id;        // Index in array
  int hidden;    // Hidden
  int detached;  // Is detached
  int z_index;   // Texture depth for drawing queue
  int parent_id; // Last parent's index (-1 : was always detached)

  int win_width;  // Width of window (stretch target size)
  int win_height; // Height of window (stretch target size)
  int tex_width;  // Width of texture
  int tex_height; // Height of texture

  const char *title; // Title of window (For detaching)

  int offx; // Offset from left
  int offy; // Offset from top

  SDL_Window *window;     // Window instance (Can be parent's)
  SDL_Renderer *renderer; // Renderer instance (Can be parent's)
  SDL_Texture *texture;   // Texture instance (Must be own)

  gui_redraw_t redraw; // Redraw callback
} gui_t;

typedef struct {
  gui_t pool[GUI_LIMIT];     // GUI instances
  int count;                 // Count of active GUI instances
  gui_t *z_queue[GUI_LIMIT]; // Sorted pointers for drawing
  int zcount;                // Z queue length before sorting
} gui_mgr_t;

int gui_call_redraw(int gui); // Call GUI to redraw
int gui_present_all(void);    // Present all windows

int gui_create(int parent, int width, int height, const char *title,
               gui_redraw_t redraw_func); // Create GUI instance (texture size)
int gui_destroy(int gui);                 // Destroy GUI instance
int gui_destroyall(void);                 // Destroy every GUI

int gui_detach(int gui);             // Detach into separate window
int gui_attach(int gui, int parent); // Attach to detached GUI instance

int gui_visible(int gui, int visible); // Set visibility
int gui_layer(int gui, int z_index);   // Set Z index

int gui_get_title(int gui, const char **title); // Get window title
int gui_get_state(int gui, int *hidden, int *detached,
                  int *parent_id); // Get instance state
int gui_get_pos(int gui, int *width, int *height, int *offx,
                int *offy); // Get texture (stretch target size) and offset

int gui_set_title(int gui, const char *title); // Set window title
int gui_resize(int gui, int width,
               int height); // Resize window (stretch target size)
int gui_set_pos(int gui, int offx, int offy); // Set offset

#endif // GUI_H

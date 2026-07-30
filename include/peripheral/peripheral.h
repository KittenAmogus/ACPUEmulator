#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "SDL_events.h"
#include <control.h>
#include <gui.h>
#include <stdint.h>

#define PER_MAX_PERIPHERALS 16

typedef int (*per_create_t)(control_unit_t *cu,
                            gui_data_t *gui); // Init pointers and peripheral
typedef int (*per_update_t)(void);            // Update peripheral
typedef int (*per_remove_t)(void);            // Remove peripheral
typedef uint8_t (*per_read_byte_t)(uint8_t port);             // On port read
typedef void (*per_write_byte_t)(uint8_t port, uint8_t byte); // On port write
typedef int (*per_getsize_t)(int *width,
                             int *height); // Load requested size for texture

typedef int (*per_onkey_t)(SDL_Event *evt);
typedef int (*per_onfocus_t)(SDL_Event *evt);
typedef int (*per_onmouse_t)(SDL_Event *evt);

typedef struct {
  // Global flags
  int id;     // ID in peripheral array
  int init;   // Is initialized
  int active; // Is active (by writing into IO port)
  int gui;    // Texture id for GUI

  // Pointers
  control_unit_t *cu; // ControlUnit data

  // Private GUI functions
  gui_redraw_t redraw;

  // Private event functions
  per_onfocus_t onfocus; // Focused
  per_onmouse_t onmouse; // Mouse events
  per_onkey_t onkey;     // Key events

  // Private functions for peripheral
  per_getsize_t getsize;       // Get req size
  per_create_t create;         // Create/init
  per_update_t update;         // Update
  per_remove_t remove;         // Remove
  per_read_byte_t read_byte;   // Handle port reading
  per_write_byte_t write_byte; // Handle port writing
} peripheral_t;

typedef struct {
  peripheral_t pool[GUI_MAX_TEXTURES];
  int count;
} per_mgr_t;

/* Single peripheral funcs */
int per_create(peripheral_t *source, int parent, control_unit_t *cu, int ox,
               int oy);
int per_update(int per);
int per_destroy(int per);
int per_redraw(int per);

/* Global peripheral funcs */
int per_destroyall(void);
int per_handle_event(SDL_Event *evt);

void per_on_gui_moved(int oldID, int newID); // Update GUI IDs

#endif // PERIPHERAL_H

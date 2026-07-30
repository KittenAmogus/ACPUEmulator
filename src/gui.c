#include <SDL2/SDL.h>
#include <control.h>
#include <gui.h>

#define LOG_MODULE "GUI"
#include <log.h>

static gui_mgr_t mgr;

static inline gui_data_t *getdata(int gui) {
  if (gui < 0 || gui >= mgr.count) {
    LOG_ERROR("Invalid ID: %d", gui);
    return NULL; // Cause segfault
  }
  return &mgr.pool[gui];
}

static int allocdata(void) {
  if (mgr.count >= GUI_MAX_TEXTURES) {
    LOG_EXCEPT("Could not allocate GUI, no memory");
    return -1;
  }

  ++mgr.count;
  LOG_DEBUG("Allocated ID=%d, count=%d", mgr.count - 1, mgr.count);
  return mgr.count - 1;
}

static SDL_Window *create_window(const char *title, int w, int h) {
  return SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                          w, h, SDL_WINDOW_SHOWN);
  LOG_DEBUG("Created window %dx%d", w, h);
}

static SDL_Renderer *create_renderer(SDL_Window *window) {
  return SDL_CreateRenderer(window, -1, GUI_RENDERER_FLAGS);
  LOG_DEBUG("Created renderer");
}

static SDL_Texture *create_texture(SDL_Renderer *renderer, int w, int h) {
  return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                           SDL_TEXTUREACCESS_TARGET, w, h);
  LOG_DEBUG("Created texture %dx%d", w, h);
}

static int gui_resort(void) {
  int new_count = mgr.count;

  for (int i = 0; i < new_count; ++i) {
    gui_data_t *dest = getdata(i);

    if (!dest->init) {
      --new_count;
      gui_data_t *src = getdata(new_count);
      *dest = *src;
      // memcpy(dest, src, sizeof(gui_data_t));
      dest->id = i;

      per_on_gui_moved(new_count, i); // Tell global world about move
      --i;                            // Check again
    }
  }
  mgr.count = new_count;

  LOG_DEBUG("Resorted array");
  return 1;
}

static inline uint32_t getwinID(int gui) {
  return SDL_GetWindowID(getdata(gui)->window);
}

int gui_check_windowID(int gui, int windowID) {
  return getwinID(gui) == (uint32_t)windowID;
}

int gui_link_redraw(int gui, gui_redraw_t func) {
  getdata(gui)->redraw = func;
  LOG_DEBUG("Linked redraw function ID=%d", gui);
  return 1;
}

int gui_create(int parent_gui, int width, int height, int offx, int offy,
               const char *title) {
  int gui = allocdata();
  if (gui < 0)
    return 0;

  gui_data_t *data = getdata(gui);
  data->id = gui;

  if (parent_gui >= 0) {
    // Set parent's window and renderer
    gui_data_t *parent = getdata(parent_gui);
    data->window = parent->window;
    data->renderer = parent->renderer;
    data->detached = 0;
    LOG_INFO("Created attached peripheral, ID=%d", data->id);
  } else {
    // Create own window and renderer
    data->window = create_window(title, width, height);
    data->renderer = create_renderer(data->window);
    data->detached = 1;
    LOG_INFO("Created detached peripheral, ID=%d", data->id);
  }

  // Create rest of data
  data->texture = create_texture(data->renderer, width, height);
  data->offx = offx;
  data->offy = offy;
  data->init = 1;
  data->id = gui;

  return gui;
}

static int gui_free(int gui) {
  gui_data_t *data = getdata(gui);
  if (!data->init)
    return 1;

  if (data->detached) {
    uint32_t winID = SDL_GetWindowID(data->window);

    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windowID(i, winID)) {
        getdata(i)->init = 0;
        LOG_DEBUG("Marked to destroy ID=%d", i);
        // on_parent_destroy(i);
      }
    }

    // SDL_DestroyWindow(data->window);
    // SDL_DestroyRenderer(data->renderer);
  }

  // SDL_DestroyTexture(data->texture);
  // gui_resort(data->id);
  return 1;
}

int gui_redraw(int gui) {
  gui_data_t *data = getdata(gui);
  if (data->redraw)
    return data->redraw(data->renderer);
  else {
    // LOG_EXCEPT("No redraw function");
    return 1;
  }
}

int gui_get_offset(int gui, int *offxptr, int *offyptr) {
  gui_data_t *data = getdata(gui);
  *offxptr = data->offx;
  *offyptr = data->offy;
  return 1;
}

int gui_get_title(int gui, const char **titleptr) {
  *titleptr = SDL_GetWindowTitle(getdata(gui)->window);
  return 1;
}

int gui_set_offset(int gui, int offx, int offy) {
  gui_data_t *data = getdata(gui);
  data->offx = offx;
  data->offy = offy;
  LOG_DEBUG("Set offset ID=%d, (%d, %d)", gui, offx, offy);
  return 1;
}

int gui_set_title(int gui, const char *title) {
  SDL_SetWindowTitle(getdata(gui)->window, title);
  LOG_DEBUG("Set title ID=%d, %s", gui, title);
  return 1;
}

int gui_get_detached(int gui) { return getdata(gui)->detached; }

int gui_attach(int gui, int parent_gui) {
  gui_data_t *data = getdata(gui);
  gui_data_t *parent = getdata(parent_gui);

  if (!parent->detached) {
    LOG_EXCEPT("Attaching to attached texture is forbidden");
    return 0;
  }

  if (!data->detached) {
    LOG_INFO("Attaching attached texture is forbidden");
    return 1;
  }

  SDL_DestroyWindow(data->window);
  SDL_DestroyRenderer(data->renderer);

  data->window = parent->window;
  data->renderer = parent->renderer;
  data->detached = 0;

  LOG_INFO("Attached %d to %d", gui, parent_gui);
  return 1;
}

int gui_detach(int gui, const char *title) {
  gui_data_t *data = getdata(gui);
  if (data->detached) {
    LOG_INFO("Attempt to detach detached texture ID=%d", gui);
    return 1;
  }

  int width, height;
  SDL_QueryTexture(data->texture, NULL, NULL, &width, &height);

  SDL_Window *window = create_window(title, width, height);
  if (!window) {
    LOG_EXCEPT("Cannot create window to detach ID=%d", gui);
    return 0;
  }

  data->window = window;
  data->renderer = create_renderer(data->window);
  data->detached = 1;

  LOG_INFO("Detached %d", gui);
  return 1;
}

int gui_destroy(int gui) {
  gui_free(gui); // Mark to delete
  gui_resort();  // Actually delete
  LOG_INFO("Destroyed %d", gui);
  return 1;
}

/* Global GUI funcs */
int gui_destroyall(void) {
  for (int i = 0; i < mgr.count; ++i) {
    gui_free(i); // Mark to delete
  }

  gui_resort(); // Actually delete
  LOG_INFO("Destroyed all");
  return 1;
}

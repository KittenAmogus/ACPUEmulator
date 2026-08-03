#include <SDL2/SDL.h>
#include <control.h>
#include <gui.h>

#define LOG_MODULE "GUI"
#include <logger.h>

static gui_mgr_t mgr;

static inline gui_data_t *getdata(int gui) {
  if (gui < 0 || gui >= mgr.count) {
    LOG_ERROR("Invalid GUI(id=%d)", gui);
    return NULL; // Cause segfault
  }
  return &mgr.pool[gui];
}

static int allocdata(void) {
  if (mgr.count >= GUI_MAX_GUIS) {
    LOG_EXCEPT("Could not allocate GUI, not enough memory");
    return -1;
  }

  ++mgr.count;
  LOG_DEBUG("Allocated new GUI(id=%d)", mgr.count - 1);
  return mgr.count - 1;
}

static SDL_Window *create_window(const char *title, int w, int h) {
  LOG_ERROR("Created window %dx%d px", w, h);
  return SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                          w, h, SDL_WINDOW_SHOWN);
}

static SDL_Renderer *create_renderer(SDL_Window *window) {
  LOG_DEBUG("Created renderer");
  return SDL_CreateRenderer(window, -1, GUI_RENDERER_FLAGS);
}

static SDL_Texture *create_texture(SDL_Renderer *renderer, int w, int h) {
  LOG_DEBUG("Created texture %dx%d px", w, h);
  return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                           SDL_TEXTUREACCESS_TARGET, w, h);
}

static int gui_resort(void) {
  int new_count = mgr.count;

  for (int i = 0; i < new_count; ++i) {
    gui_data_t *dest = getdata(i);

    if (!dest->init) {

      --new_count;

      SDL_DestroyWindow(dest->window);
      SDL_DestroyRenderer(dest->renderer);

      dest->window = NULL;
      dest->renderer = NULL;

      if (dest->id == new_count) {
        per_on_gui_moved(dest->id, -1);
        continue;
      }

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

int gui_check_windows(int gui1, int gui2) {
  return getwinID(gui1) == getwinID(gui2);
}

int gui_check_windows_ID(int gui, int winID2) {
  return getwinID(gui) == (uint32_t)winID2;
}

int gui_link_redraw(int gui, gui_redraw_t func) {
  getdata(gui)->redraw = func;
  LOG_DEBUG("Linked redraw function for GUI(id=%d)", gui);
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
  } else {
    // Create own window and renderer
    data->window = create_window(title, width, height);
    data->renderer = create_renderer(data->window);
    data->detached = 1;
  }
  LOG_INFO("Created GUI(id=%d, detached=%d)", data->id, data->detached);

  // Create rest of data
  data->texture = create_texture(data->renderer, width, height);

  int cw, ch;
  if (0 != SDL_QueryTexture(data->texture, NULL, NULL, &cw, &ch)) {
    LOG_EXCEPT("Could not create texture: %dx%d", cw, ch);
    LOG_EXCEPT("Error: %s", SDL_GetError());
    return 0;
  }

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
      if (gui_check_windows_ID(i, winID)) {
        getdata(i)->init = 0;
        LOG_DEBUG("Marked to destroyGUI(id=%d)", i);
      }
    }
  }

  return 1;
}

int gui_redraw(int gui) {
  gui_data_t *data = getdata(gui);

  // Set texture target
  SDL_SetRenderTarget(data->renderer, data->texture);

  if (data->redraw)
    data->redraw(data->renderer);

  if (data->detached) {
    SDL_Rect dstrect;

    for (int i = 0; i < mgr.count; ++i) {
      gui_data_t *data1 = getdata(i);
      if (gui_check_windows(data->id, data1->id)) {

        int w, h;
        SDL_QueryTexture(data1->texture, NULL, NULL, &w, &h);

        dstrect = (SDL_Rect){
            .x = data1->offx,
            .y = data1->offy,
            .w = w,
            .h = h,
        };

        SDL_SetRenderTarget(data->renderer, NULL);
        SDL_RenderCopy(data->renderer, data1->texture, NULL, &dstrect);
      }
    }

    SDL_RenderPresent(data->renderer);
  }

  SDL_SetRenderTarget(data->renderer, NULL);
  return 1;
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
  LOG_DEBUG("Changed offset for GUI(id=%d) to (%d, %d)", gui, offx, offy);
  return 1;
}

int gui_set_title(int gui, const char *title) {
  SDL_SetWindowTitle(getdata(gui)->window, title);
  LOG_DEBUG("Changed title for GUI(id=%d) to \"%s\"", gui, title);
  return 1;
}

int gui_get_detached(int gui) { return getdata(gui)->detached; }

int gui_attach(int gui, int parent_gui) {
  gui_data_t *data = getdata(gui);
  gui_data_t *parent = getdata(parent_gui);

  if (!parent->detached) {
    LOG_EXCEPT("Failed to attach GUI(id=%d) to GUI(id=%d): Parent is attached",
               gui, parent_gui);
    return 0;
  }

  if (!data->detached) {
    LOG_DEBUG("Attempt to attach attached GUI(id=%d)", gui);
    return 1;
  }

  SDL_DestroyWindow(data->window);
  SDL_DestroyRenderer(data->renderer);

  data->window = parent->window;
  data->renderer = parent->renderer;
  data->detached = 0;

  LOG_DEBUG("Attached GUI(id=%d) to GUI(id=%d)", gui, parent_gui);
  return 1;
}

int gui_detach(int gui, int width, int height, const char *title) {
  gui_data_t *data = getdata(gui);

  if (!data->init) {
    LOG_EXCEPT("Attempt to detach uninitialized GUI: %d", data->id);
    return 0;
  }

  if (data->detached) {
    LOG_DEBUG("Attempt to detach detached GUI(id=%d)", gui);
    return 1;
  }

  /*
  int width, height;
  if (0 != SDL_QueryTexture(data->texture, NULL, NULL, &width, &height)) {
    LOG_EXCEPT("Could not get size: %s", SDL_GetError());
    return 0;
  }

  if (width < 1 || height < 1) {
    LOG_EXCEPT("NEGATIVE W/H | %s", SDL_GetError());
    return 0;
  }

  if (width > 2560 || height > 1600) {
    LOG_EXCEPT("TOO BIG WINDOW | %s", SDL_GetError());
    return 0;
  }

  LOG_ERROR("Texture size is %dx%d", width, height);
  */

  SDL_Window *window = create_window(title, width, height);
  if (!window) {
    LOG_EXCEPT("Failed to detach GUI(id=%d)", gui);
    return 0;
  }

  data->window = window;
  data->renderer = create_renderer(data->window);
  data->detached = 1;

  SDL_DestroyTexture(data->texture);
  data->texture = create_texture(data->renderer, width, height);

  LOG_DEBUG("Detached GUI(id=%d)", gui);
  return 1;
}

int gui_destroy(int gui) {
  gui_free(gui); // Mark to delete
  gui_resort();  // Actually delete
  LOG_DEBUG("Destroyed GUI(id=%d)", gui);
  return 1;
}

/* Global GUI funcs */
int gui_destroyall(void) {
  for (int i = 0; i < mgr.count; ++i) {
    gui_free(i); // Mark to delete
  }

  gui_resort(); // Actually delete
  LOG_DEBUG("Destroyed all GUIs");
  return 1;
}

#define LOG_MODULE "GUI"
#include <logger.h>

#include <SDL2/SDL.h>
#include <gui.h>

static gui_mgr_t mgr = {0};

/* === Static functions === */

static inline gui_t *getdata(int gui) { return &mgr.pool[gui]; }

static inline int allocdata(void) {
  if (mgr.count >= GUI_LIMIT)
    return -1;
  return mgr.count++;
}

static inline SDL_Window *create_window(int width, int height,
                                        const char *title) {
  return SDL_CreateWindow(title, GUI_WINPOS, GUI_WINPOS, width, height,
                          SDL_WINDOW_SHOWN);
}

static inline SDL_Renderer *create_renderer(SDL_Window *win) {
  return SDL_CreateRenderer(win, -1, GUI_RENDERER_F);
}

static inline SDL_Texture *create_texture(SDL_Renderer *ren, int width,
                                          int height) {
  return SDL_CreateTexture(ren, GUI_TEXTURE_FMT, GUI_TEXTURE_ACC, width,
                           height);
}

static int gui_free(int gui) {
  gui_t *data = getdata(gui);
  data->id = -1;
  LOG_DEBUG("Marked to destroy GUI: %d", gui);

  if (data->detached) {
    for (int i = 0; i < mgr.count; ++i) {
      getdata(i)->id = -1;
      LOG_DEBUG("Marked to destroy attached GUI: %d", i);
    }
  }
  return 1;
}

static int gui_resort(void) {
  for (int i = 0; i < mgr.count; ++i) {
    gui_t *data = getdata(i);
    if (data->id >= 0)
      continue;

    mgr.z_queue[data->id] = NULL;
    SDL_DestroyTexture(data->texture);
    if (data->detached) {
      SDL_DestroyRenderer(data->renderer);
      SDL_DestroyWindow(data->window);
      LOG_DEBUG("Destroyed window");
    }

    --mgr.count;

    if (mgr.count != data->id) {
      gui_t *last = getdata(mgr.count); // Move last to freed
      // on_gui_move(last->id, data->id); // GUI move callback (last->data)
      LOG_DEBUG("Moved GUI %d -> %d", last->id, data->id);
      *data = *last;
      data->id = i;
    } else {
      // on_gui_move(data->id, -1); // GUI destroy callback (data-> -1)
      LOG_DEBUG("Destroyed GUI %d", data->id);
    }

    --i;
  }

  return 1;
}

static int compare_z(const void *a, const void *b) {
  return (*(const gui_t **)a)->z_index - (*(const gui_t **)b)->z_index;
}

static int gui_resort_z(void) {
  qsort(mgr.z_queue, mgr.zcount, sizeof(gui_t *), compare_z);
  LOG_DEBUG("Resorted Zbuff: %d -> %d", mgr.zcount, mgr.count);
  mgr.zcount = mgr.count;

  for (int i = 0; i < mgr.zcount; ++i) {
    LOG_DEBUG("ZBUFF[%d] = %d", i, mgr.z_queue[i]->id);
  }

  return 1;
}

/* === Global functions === */

int gui_create(int parent, int width, int height, const char *title,
               gui_redraw_t redraw_func) {
  int id = allocdata();
  if (id < 0) {
    LOG_EXCEPT("Out of space");
    return -1;
  }

  gui_t *data = getdata(id);
  data->id = id;

  data->hidden = 0;
  data->parent_id = parent;
  data->win_width = width;
  data->win_height = height;
  data->tex_width = width;
  data->tex_height = height;
  data->title = title;
  data->offx = 0;
  data->offy = 0;
  data->redraw = redraw_func;

  // Get last Z index

  if (mgr.zcount > 0)
    data->z_index = (mgr.z_queue[mgr.zcount - 1])->z_index + 1;
  else
    data->z_index = 0;

  if (parent >= 0) {
    gui_t *pdata = getdata(parent);

    data->window = pdata->window;
    data->renderer = pdata->renderer;
    data->detached = 0;

  } else {

    data->window = create_window(width, height, title);

    if (!data->window) {
      LOG_EXCEPT("Could not create window for GUI: %d", data->id);
      --mgr.count; // Free space
      return -1;
    }

    data->renderer = create_renderer(data->window);
    data->detached = 1;
  }

  data->texture = create_texture(data->renderer, width, height);

  LOG_DEBUG("Created new %s GUI: %d",
            (data->detached ? "detached" : "attached"), data->id);

  ++mgr.zcount;
  mgr.z_queue[data->id] = data;
  gui_resort_z();
  return data->id;
}

int gui_destroy(int gui) {
  gui_free(gui);
  gui_resort();
  gui_resort_z();
  return 1;
}

int gui_destroyall(void) {
  for (int i = 0; i < mgr.count; ++i) {
    gui_free(i);
  }

  gui_resort();
  return 1;
}

int gui_detach(int gui) {
  gui_t *data = getdata(gui);

  if (data->detached) {
    LOG_WARNING("GUI is already detached: %d", gui);
    return 1;
  }

  SDL_Window *win =
      create_window(data->win_width, data->win_height, data->title);

  if (!win) {
    LOG_EXCEPT("Could not create window: %d", gui);
    return 0;
  }

  SDL_DestroyTexture(data->texture);

  data->window = win;
  data->renderer = create_renderer(data->window);
  data->texture =
      create_texture(data->renderer, data->tex_width, data->tex_height);

  data->detached = 1;
  LOG_DEBUG("Detached GUI: %d from %d", gui, data->parent_id);
  return 1;
}

int gui_attach(int gui, int parent) {
  if (parent < 0) {
    LOG_EXCEPT("Parent's id is negative");
    return 0;
  }

  gui_t *data = getdata(gui);
  gui_t *pdata = getdata(parent);

  if (!data->detached) {
    if (data->parent_id == parent) {
      LOG_WARNING("GUI is already attached: %d to %d", gui, data->parent_id);
      return 1;
    }
  }

  SDL_DestroyTexture(data->texture);

  if (data->detached) {
    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->window);
  }

  data->window = pdata->window;
  data->renderer = pdata->renderer;
  data->texture =
      create_texture(data->renderer, data->tex_width, data->tex_height);

  data->parent_id = parent;
  data->detached = 0;
  LOG_DEBUG("Attached GUI: %d to %d", gui, data->parent_id);
  return 1;
}

int gui_visible(int gui, int visible) {
  gui_t *data = getdata(gui);

  if (data->hidden == !visible) {
    LOG_WARNING("GUI is already %s", (visible ? "visible" : "hidden"));
    return 1;
  }

  data->hidden = !visible;

  if (data->detached) {
    if (data->hidden) {
      SDL_HideWindow(data->window);
    } else {
      SDL_ShowWindow(data->window);
    }
  }

  LOG_DEBUG("Changed visibility for GUI %d to %d", gui, visible);
  return 1;
}

int gui_layer(int gui, int z_index) {
  gui_t *data = getdata(gui);

  if (data->z_index == z_index) {
    LOG_WARNING("GUI layer is already %d", z_index);
    return 1;
  }

  data->z_index = z_index;
  gui_resort_z();

  LOG_DEBUG("Changed layer for GUI %d to %d", gui, z_index);
  return 1;
}

int gui_get_title(int gui, const char **title) {
  if (title) {
    *title = getdata(gui)->title;
    return 1;
  }
  return 0;
}

int gui_get_state(int gui, int *hidden, int *detached, int *parent_id) {
  gui_t *data = getdata(gui);

  if (hidden)
    *hidden = data->hidden;

  if (detached)
    *detached = data->detached;

  if (parent_id)
    *parent_id = data->parent_id;

  return 1;
}

int gui_get_pos(int gui, int *width, int *height, int *offx, int *offy) {
  gui_t *data = getdata(gui);

  if (width)
    *width = data->win_width;

  if (height)
    *height = data->win_height;

  if (offx)
    *offx = data->offx;

  if (offy)
    *offy = data->offy;

  return 1;
}

int gui_set_title(int gui, const char *title) {
  gui_t *data = getdata(gui);
  data->title = title;

  if (data->detached) {
    SDL_SetWindowTitle(data->window, title);
    LOG_DEBUG("Changed window title to %s", title);
  }

  LOG_DEBUG("Changed GUI title %d to %s", gui, title);
  return 1;
}

int gui_resize(int gui, int width, int height) {
  gui_t *data = getdata(gui);

  if (data->win_width == width && data->win_height == height) {
    LOG_WARNING("GUI size if already %dx%d", width, height);
    return 1;
  }

  data->win_width = width;
  data->win_height = height;

  if (data->detached) {
    SDL_SetWindowSize(data->window, width, height);
    LOG_DEBUG("Resized window to %dx%d", width, height);
  }

  LOG_DEBUG("Resized GUI to %dx%d", width, height);
  return 1;
}

int gui_set_pos(int gui, int offx, int offy) {
  gui_t *data = getdata(gui);

  if (data->offx == offx && data->offy == offy) {
    LOG_WARNING("GUI offset if already %dx%d", offx, offy);
    return 1;
  }

  data->offx = offx;
  data->offy = offy;

  if (data->detached) {
    SDL_SetWindowPosition(data->window, offx, offy);
    LOG_DEBUG("Moved window to %dx%d", offx, offy);
  }

  LOG_DEBUG("Moved GUI to %dx%d", offx, offy);
  return 1;
}

int gui_present_all(void) {
  SDL_Rect rect;

  for (int i = 0; i < mgr.zcount; ++i) {
    gui_t *data = mgr.z_queue[i];
    if (data->hidden)
      continue;

    if (data->detached) {
      rect.x = 0;
      rect.y = 0;
    } else {
      rect.x = data->offx;
      rect.y = data->offy;
    }
    rect.w = data->win_width;
    rect.h = data->win_height;
    SDL_RenderCopy(data->renderer, data->texture, NULL, &rect);
  }

  for (int i = 0; i < mgr.zcount; ++i) {
    gui_t *data = mgr.z_queue[i];
    if (data->detached) {
      SDL_RenderPresent((mgr.z_queue[i])->renderer);
    }
  }

  return 1;
}

int gui_call_redraw(int gui) {
  gui_t *data = getdata(gui);
  if (data->hidden)
    return 0;

  SDL_SetRenderTarget(data->renderer, data->texture);

  if (data->redraw)
    data->redraw(data->renderer);

  SDL_SetRenderTarget(data->renderer, NULL);

  return 1;
}

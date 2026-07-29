#include "SDL_events.h"
#include "SDL_video.h"
#include <SDL2/SDL.h>
#include <control.h>
#include <gui.h>

#include <stdio.h>
#include <string.h>

static gui_data_t textures[GUI_MAX_TEXTURES] = {0};
static int textures_pos = 0;

static inline void logtxt(const char *txt) {
  fprintf(stdout, "GUI: %s\r\n", txt);
}

static inline void logerr(const char *err) {
  fprintf(stderr, "[ERROR] GUI: %s\r\n", err);
}

static inline gui_data_t *getdata(int id) { return &(textures[id]); }

static SDL_Window *create_window(const char *title, int width, int height) {
  return SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                          width, height, SDL_WINDOW_SHOWN);
}

static SDL_Renderer *create_renderer(SDL_Window *window) {
  return SDL_CreateRenderer(window, -1, GUI_RENDERER_FLAGS);
}

int gui_create_texture(int parent, int width, int height, int offx, int offy) {
  if (textures_pos >= GUI_MAX_TEXTURES) {
    logerr("No space for another texture");
    return -1;
  }

  if (width < 1 || height < 1) {
    logerr("Invalid texture size");
    return -1;
  }

  gui_data_t *data = getdata(textures_pos);

  // Create window
  if (parent == -1) {
    data->window = create_window(GUI_MAIN_TITLE, width, height);
    if (!data->window) {
      logerr("Cannot create window");
      return -1;
    }

    data->renderer = create_renderer(data->window);
    data->detached = 1;
  } else {
    gui_data_t *parent_data = getdata(parent);
    data->window = parent_data->window;
    data->renderer = parent_data->renderer;
    data->detached = 0;

    logtxt("Created new attached texture");
  }

  data->texture = SDL_CreateTexture(data->renderer, SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, width, height);

  // Create data
  data->init = 1;
  data->offx = offx;
  data->offy = offy;
  data->id = textures_pos;

  ++textures_pos;
  return data->id;
}

int gui_detach_texture(int texture, const char *window_title) {
  if (texture < 0 || texture > textures_pos) {
    logerr("Attempt to detach nonexisting texture");
    return 0;
  }

  gui_data_t *data = getdata(texture);
  if (!data->init) {
    logerr("Attempt to detach uninitialized texture");
    return 0;
  }

  if (data->detached)
    return 1;

  int width, height;
  SDL_QueryTexture(data->texture, NULL, NULL, &width, &height);

  data->window = create_window(window_title, width, height);

  if (!data->window) {
    logerr("Cannot create window");
    return 0;
  }

  data->renderer = SDL_CreateRenderer(data->window, -1, GUI_RENDERER_FLAGS);
  data->detached = 1;

  logtxt("Created new window");
  return 1;
}

int gui_move_texture(int texture, int offx, int offy) {
  if (texture < 0 || texture >= textures_pos) {
    logerr("Attempt to move nonexisting texture");
    return 0;
  }

  gui_data_t *data = getdata(texture);
  data->offx = offx;
  data->offy = offy;
  return 1;
}

int gui_attach_texture(int texture, int parent) {
  if (texture < 0 || texture >= textures_pos) {
    logerr("Attempt to attach nonexisting texture");
    return 0;
  }

  if (parent < 0 || parent >= textures_pos) {
    logerr("Attempt to attach texture without parent");
    return 0;
  }

  gui_data_t *data = getdata(texture);
  gui_data_t *pdata = getdata(parent);

  if (!data->detached)
    return 1;

  SDL_DestroyRenderer(data->renderer);
  SDL_DestroyWindow(data->window);

  data->renderer = pdata->renderer;
  data->window = pdata->window;
  data->detached = 0;

  logtxt("Attached texture");
  return 1;
}

int gui_remove_texture(int texture) {
  if (texture < 0 || texture >= textures_pos) {
    logerr("Attempt to remove nonexisting texture");
    return 0;
  }

  gui_data_t *data = getdata(texture);

  SDL_DestroyTexture(data->texture);
  data->texture = NULL;

  if (data->detached) {
    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->window);
    data->renderer = NULL;
    data->window = NULL;
    data->init = 0;

    logtxt("Removed window");
  }

  --textures_pos;
  if (data->id != textures_pos) {
    gui_data_t *last = getdata(textures_pos);
    last->id = data->id;
    memcpy(data, last, sizeof(gui_data_t));
  }

  logtxt("Removed texture");
  return 1;
}

int gui_redraw_texture(int texture) {
  if (texture < 0 || texture >= textures_pos) {
    // logerr("Attempt to redraw nonexisting texture");
    return 0;
  }

  gui_data_t *data = getdata(texture);
  SDL_RenderPresent(data->renderer);
  return 1;
}

int gui_handle_evt(SDL_Event *evt) {
  gui_data_t *data;

  if (evt->type == SDL_WINDOWEVENT) {
    switch (evt->window.event) {

    case SDL_WINDOWEVENT_CLOSE:
      for (int i = 0; i < textures_pos; ++i) {
        data = getdata(i);
        if (SDL_GetWindowID(data->window) == evt->window.windowID) {
          gui_remove_texture(i);
        }
      }
      break;
    }
  }

  else if (evt->type == SDL_KEYDOWN) {
    switch (evt->key.keysym.sym) {}
  }

  return 1;
}

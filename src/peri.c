#include "SDL_video.h"
#define LOG_MODULE "PERIPHERAL"
#include <logger.h>

#include <control.h>
#include <gui.h>
#include <peri.h>

per_mgr_t mgr;

extern void main_on_peri_destroyed(int destrID);
extern void main_on_peri_moved(int oldID, int newID);

static inline peripheral_t *getdata(int per) {
  if (per < 0 || per >= mgr.count) {
    LOG_ERROR("Invalid PERI(id=%d)", per);
    return NULL; // Cause segfault
  }
  return &mgr.pool[per];
}

static int allocdata(void) {
  if (mgr.count >= PER_MAX_PERIPHERALS) {
    LOG_EXCEPT("Could not allocate PERI, no memory");
    return -1;
  }

  ++mgr.count;
  LOG_DEBUG("Allocated PERI(id=%d)", mgr.count - 1);
  return mgr.count - 1;
}

static int per_resort(void) {
  int new_count = mgr.count;

  for (int i = 0; i < new_count; ++i) {
    peripheral_t *dest = getdata(i);

    if (!dest->init) {
      --new_count;

      LOG_WARNING("Destroying GUI %d", dest->gui);
      if (dest->gui >= 0)
        gui_destroy(dest->gui);

      main_on_peri_destroyed(dest->id);
      if (dest->id == new_count) {
        continue;
      }

      peripheral_t *src = getdata(new_count);
      *dest = *src;
      // memcpy(dest, src, sizeof(per_data_t));
      dest->id = i;

      main_on_peri_moved(new_count, i);
      --i; // Check again
    }
  }
  mgr.count = new_count;

  LOG_DEBUG("Resorted PERI array, %d", new_count);
  return 1;
}

static int per_free(int per) {
  peripheral_t *data = getdata(per);
  if (!data->init)
    return 1;

  if (gui_get_detached(data->gui)) {

    for (int i = 0; i < mgr.count; ++i) {
      peripheral_t *per2 = getdata(i);
      if (gui_check_windows(per2->gui, data->gui)) {
        per2->init = 0;
        LOG_DEBUG("Marked to destroy PERI(id=%d)", i);
      }
    }
  }
  return 1;
}

/* Single peripheral funcs */
int per_create(peripheral_t *source, int parent, control_unit_t *cu, int ox,
               int oy) {

  int per = allocdata();
  if (per < 0)
    return -1;

  peripheral_t *data = getdata(per);
  *data = *source;

  int parent_gui;

  if (parent >= 0) {
    peripheral_t *pdata = getdata(parent);
    parent_gui = pdata->gui;
    LOG_DEBUG("Got parent: PERI(id=%d, gui=%d)", pdata->id, pdata->gui);
  } else {
    parent_gui = -1;
  }

  int width, height;
  data->getsize(&width, &height);

  const char *title;
  if (!data->gettitle(&title)) {
    LOG_WARNING("Could not get title");
    title = "UNDEFINED";
  }

  data->id = per;
  data->gui =
      gui_create(parent_gui, width, height, ox, oy, title); // TODO: Title
  gui_link_redraw(data->gui, data->redraw);
  data->init = 1;
  data->active = 0;

  LOG_DEBUG("Created PERI(id=%d, gui=%d)", data->id, data->gui);
  return data->id;
}

int per_attach(int per, int parent) {
  return gui_attach(getdata(per)->gui, getdata(parent)->gui);
}

int per_detach(int per) {
  peripheral_t *data = getdata(per);

  const char *title;
  int width, height;

  if (!data->getsize(&width, &height)) {
    LOG_EXCEPT("Could not get size");
    return 0;
  }

  if (!data->gettitle(&title)) {
    LOG_WARNING("Could not get title");
    title = "UNDEFINED";
  }
  return gui_detach(data->gui, width, height, title);
}

int per_init(int per, void *data) {
  getdata(per)->create(data);
  return 1;
}

int per_update(int per) { return getdata(per)->update(); }

int per_destroy(int per) {
  per_free(per);
  per_resort();
  LOG_INFO("Destroyed PERI(id=%d)", per);
  return 1;
}

int per_redraw(int per) {
  if (!getdata(per)->init) {
    LOG_EXCEPT("Failed to redraw PERI(id=%d): uninitialized", per);
    return 0;
  }

  return gui_redraw(getdata(per)->gui);
}

/* Global peripheral funcs */
int per_destroyall(void) {
  for (int i = 0; i < mgr.count; ++i) {
    per_free(i);
  }
  per_resort();
  LOG_INFO("Destroyed all PERIs");
  return 1;
}

int per_handle_event(SDL_Event *evt) {

  if (evt->window.event == SDL_WINDOWEVENT_CLOSE) {
    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windows_ID(getdata(i)->gui, evt->window.windowID)) {
        per_destroy(i);
        // per_free(i);
      }
    }
    // per_resort();
    return 1;
  }

  if (evt->type == SDL_KEYDOWN || evt->type == SDL_KEYUP) {
    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windows_ID(getdata(i)->gui, evt->key.windowID)) {

        if (evt->key.keysym.sym == SDLK_BACKSPACE) {
          const char *title;
          int width, height;
          if (getdata(i)->getsize(&width, &height) &&
              getdata(i)->gettitle(&title) && title)
            gui_detach(getdata(i)->gui, width, height, title);
          else {
            LOG_EXCEPT("Could not detach PERI(id=%d): invalid title or size",
                       i);
          }
        }

        getdata(i)->onkey(evt);
      }
    }
  } else if (evt->type == SDL_WINDOWEVENT_FOCUS_LOST ||
             evt->type == SDL_WINDOWEVENT_FOCUS_GAINED) {
    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windows_ID(getdata(i)->gui, evt->window.windowID))
        getdata(i)->onfocus(evt);
    }
  } else if (evt->type == SDL_MOUSEBUTTONUP || evt->type == SDL_MOUSEBUTTONUP) {
    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windows_ID(getdata(i)->gui, evt->button.windowID))
        getdata(i)->onmouse(evt);
    }
  } else if (evt->type == SDL_MOUSEWHEEL) {
    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windows_ID(getdata(i)->gui, evt->wheel.windowID))
        getdata(i)->onmouse(evt);
    }
  } else if (evt->type == SDL_MOUSEMOTION) {
    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windows_ID(getdata(i)->gui, evt->motion.windowID))
        getdata(i)->onmouse(evt);
    }
  }

  return 1;
}

void per_on_gui_moved(int oldID, int newID) {
  LOG_INFO("Moved GUI(id=%d) to GUI(id=%d)", oldID, newID);

  for (int i = 0; i < mgr.count; ++i) {
    peripheral_t *data = getdata(i);
    if (data->gui == oldID) {
      data->gui = newID;
    }
  }
}

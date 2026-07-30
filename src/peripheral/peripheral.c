#include "SDL_events.h"
#include "SDL_video.h"
#include <control.h>
#include <gui.h>
#include <peripheral/peripheral.h>

#define LOG_MODULE "PERIPHERAL"
#include <log.h>

per_mgr_t mgr;

static inline peripheral_t *getdata(int per) {
  if (per < 0 || per >= mgr.count) {
    LOG_ERROR("Invalid ID: %d", per);
    return NULL; // Cause segfault
  }
  return &mgr.pool[per];
}

static int allocdata(void) {
  if (mgr.count >= PER_MAX_PERIPHERALS) {
    LOG_EXCEPT("Could not allocate per, no memory");
    return -1;
  }

  ++mgr.count;
  LOG_DEBUG("Allocated ID=%d, count=%d", mgr.count - 1, mgr.count);
  return mgr.count - 1;
}

static int per_resort(void) {
  int new_count = mgr.count;

  for (int i = 0; i < new_count; ++i) {
    peripheral_t *dest = getdata(i);

    if (!dest->init) {
      --new_count;
      if (dest->gui > 0)
        gui_destroy(dest->gui);
      peripheral_t *src = getdata(new_count);
      *dest = *src;
      // memcpy(dest, src, sizeof(per_data_t));
      dest->id = i;

      --i; // Check again
    }
  }
  mgr.count = new_count;

  LOG_DEBUG("Resorted array");
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
        LOG_DEBUG("Marked to destroy ID=%d", i);
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
    LOG_DEBUG("PARENT GUI: %d", pdata->gui);
  } else {
    parent_gui = -1;
  }

  int width, height;
  data->getsize(&width, &height);

  data->id = per;
  data->gui =
      gui_create(parent_gui, width, height, ox, oy, "title"); // TODO: Title
  gui_link_redraw(data->gui, data->redraw);
  data->init = 1;
  data->active = 0;

  LOG_DEBUG("per_create(source, parent=%d, cu, ox=%d, oy=%d); -> %d", parent,
            ox, oy, data->id);
  LOG_DEBUG("Parent: %d | Parent GUI: %d | GUI: %d", parent, parent_gui,
            data->gui);

  return data->id;
}

int per_update(int per) { return getdata(per)->update(); }

int per_destroy(int per) {
  per_free(per);
  per_resort();
  LOG_INFO("Destroyed %d", per);
  return 1;
}

int per_redraw(int per) {
  if (!getdata(per)->init) {
    LOG_EXCEPT("Attempt to redraw uninitialized ID=%d", per);
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
  LOG_INFO("Destroyed all");
  return 1;
}

int per_handle_event(SDL_Event *evt) {
  if (evt->type == SDL_KEYDOWN || evt->type == SDL_KEYUP) {
    for (int i = 0; i < mgr.count; ++i) {
      if (gui_check_windows_ID(getdata(i)->gui, evt->key.windowID))
        getdata(i)->onkey(evt);
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
  LOG_INFO("GUI_MOVED: %d => %d", oldID, newID);

  for (int i = 0; i < mgr.count; ++i) {
    peripheral_t *data = getdata(i);
    if (data->gui == oldID)
      data->gui = newID;
  }
}

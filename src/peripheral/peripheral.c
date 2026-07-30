#include <control.h>
#include <gui.h>
#include <peripheral/peripheral.h>

#include <stdio.h>

// void per_on_gui_moved(int oldID, int newID) { return; }

/*
static peripheral_t peripherals[PER_MAX_PERIPHERALS];
static int peripherals_pos = 0;

static inline void logerr(const char *err) {
  fprintf(stderr, "[ERROR] PERIPHERAL: %s\r\n", err);
}

static inline void logtxt(const char *txt) {
  printf("PERIPHERAL: %s\r\n", txt);
}

static inline peripheral_t *getdata(int per) { return &(peripherals[per]); }

int per_create(peripheral_t *source, peripheral_t *parent, control_unit_t *cu,
               int ox, int oy) {
  int parent_gui = -1;
  if (parent != NULL)
    parent_gui = parent->gui;

  int width, height;
  source->getsize(&width, &height);

  source->cu = cu;
  source->gui = gui_create_texture(parent_gui, width, height, ox, oy);

  gui_link_redrawfunc(source->gui, source->redraw);

  logtxt("Created peripheral");
  return 1;
}

int per_update(int per) {
  if (per < 0 || per >= peripherals_pos) {
    logerr("Attempt to redraw nonexisting peripheral");
    return 0;
  }

  peripheral_t *data = getdata(per);
  return data->update();
}

int per_destroy(int per);

int per_redraw(int per) {
  if (per < 0 || per >= peripherals_pos) {
    logerr("Attempt to redraw nonexisting peripheral");
    return 0;
  }
  peripheral_t *data = getdata(per);

  return gui_redraw_texture(data->gui);
}

int per_handle_event(SDL_Event *evt) { return 1; }*/

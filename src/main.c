#include "SDL_events.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <control.h>
#include <interpreter.h>
#include <peripheral.h>

#include <display.h>

uint8_t program[] = {
    // TODO: Assembler
    0xEC,       // rnd a
    0xED,       // rnd b
    0x64,       // add a, b
    0x54, 0x01, // ldi a, 0x01
    0x55, 0xAA, // ldi b, 0xAA
    0x00,       // nop
    0x00,       // nop
    0x01,       // hlt
};

typedef enum {
  PER_DISPLAY = 0,
  PER_KBD = 1,
  PER_TERM = 2,
  PER_DIGINT = 3,
  PER_COUNT = 4,
} peripheral_e;

peripheral_t *peripherals[] = {NULL, NULL, NULL, NULL};

static control_unit_t *init_control_unit(void) {
  control_unit_t *cu = malloc(sizeof(control_unit_t));
  if (cu == NULL)
    return NULL;

  memset(cu, 0, sizeof(control_unit_t));
  memcpy(cu->ram.ram.bank_raw[0], program, sizeof(program)); // TODO: Assembler
  return cu;
}

int main(void) {
  /* === Init simulation === */
  control_unit_t *cu = init_control_unit();
  if (cu == NULL) {
    fprintf(stderr, "MAIN: Failed to create control unit\r\n");
    return -1;
  }

  // Fill peripherals
  peripherals[PER_DISPLAY] = &per_display;

  /* === Init peripherals === */
  peripheral_t *display = peripherals[PER_DISPLAY];
  display->create(cu);

  int running = 1;
  int must_run = 1;
  int need_update = 1;

  SDL_Event evt;
  peripheral_t *per;

  while (running && must_run) {
    while (SDL_PollEvent(&evt)) {
      for (int i = 0; i < PER_COUNT; ++i) {
        per = peripherals[i];
        if (per == NULL || !per->init)
          continue;

        // if (SDL_GetWindowID(per->gui.window) == evt.window.windowID)
        per->onevent(cu, evt);
      }
    }

    must_run = 0;
    if (need_update) {
      must_run = 1;
      if (!update_control_unit(cu)) {
        need_update = 0;

        printf("+-------+-------+------+------+------+------+--------+\r\n");
        printf("+ HALTED --------------------------------------------+\r\n");
        printf("| IP    | IR    | A    | B    | C    | D    | F OCSZ |\r\n");
        printf("+-------+-------+------+------+------+------+--------+\r\n");
      }
    }

    // Update peripherals
    for (int i = 0; i < PER_COUNT; ++i) {
      per = peripherals[i];
      if (per == NULL || !per->init)
        continue;

      must_run = 1;
      per->update(cu);
    }

    usleep(8000);
  }

  for (int i = 0; i < PER_COUNT; ++i) {
    per = peripherals[i];
    if (per == NULL || !per->init)
      continue;

    per->destroy(cu);
  }

  // Clean up
  free(cu);
  return 0;
}

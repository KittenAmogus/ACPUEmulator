#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <control.h>
#include <gui.h>
#include <interpreter.h>
#include <peripheral/peripheral.h>

#define LOG_MODULE "MAIN"
#include <log.h>

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

uint32_t active_window_id = 0;

uint8_t on_port_read(uint8_t port) { return 0x00; }
void on_port_write(uint8_t port, uint8_t value) {}

static control_unit_t *init_control_unit(void) {
  control_unit_t *cu = malloc(sizeof(control_unit_t));
  if (cu == NULL)
    return NULL;

  memset(cu, 0, sizeof(control_unit_t));
  memcpy(cu->ram.ram.bank_raw[0], program, sizeof(program)); // TODO: Assembler

  LOG_INFO("Loaded program %d bytes", sizeof(program));
  return cu;
}

int moved = 0;

void per_on_gui_moved(int oldID, int newID) { moved = 1; }

int main(void) {
  log_setlvl(LOG_LVL_DEBUG);

  /* === Init simulation === */
  control_unit_t *cu = init_control_unit();
  if (cu == NULL) {
    LOG_ERROR("Failed to create control unit");
    return -1;
  }

  /* === Create peripherals === */
  // int display = per_create(&per_display, NULL, cu, 0, 0);
  int display1 = gui_create(-1, 800, 800, 0, 0, "Display");
  int display2 = gui_create(display1, 600, 600, 100, 100, NULL);
  LOG_INFO("Created peripherals");

  gui_link_redraw(display1, NULL);
  gui_link_redraw(display2, NULL);

  SDL_Event event;
  int running = 1;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        LOG_INFO("Received QUIT event");
        running = 0;
        break;
      }

      else if (event.type == SDL_WINDOWEVENT || event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE) {
          LOG_DEBUG("Received BACKSPACE, detaching display2");
          gui_detach(display2, "Display 2");
        }
      }
    }

    if (!moved) {
      gui_redraw(display1);
      gui_redraw(display2);
    }

    /*// UPDATE
    per_update(display);

    // DRAW
    per_redraw(display);*/
    usleep(8000);
  }

  gui_destroyall();
  // per_destroy(display);
  SDL_Quit();
  free(cu);

  LOG_INFO("All done, exit");
  return 0;
}

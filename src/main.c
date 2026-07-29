#include "SDL_keycode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <control.h>
#include <gui.h>
#include <interpreter.h>

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
  return cu;
}

int main(void) {
  /* === Init simulation === */
  control_unit_t *cu = init_control_unit();
  if (cu == NULL) {
    fprintf(stderr, "MAIN: Failed to create control unit\r\n");
    return -1;
  }

  SDL_Event event;
  int running = 1;

  int display = gui_create_texture(-1, 800, 800, 0, 0);
  int display2 = gui_create_texture(-1, 160, 160, 0, 0);
  int display3 = gui_create_texture(display2, 500, 500, 150, 150);

  gui_rename_window(display, "Display");
  gui_rename_window(display2, "Display 2");
  gui_rename_window(display3, "Display 3");

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
        break;
      }

      else if (event.type == SDL_WINDOWEVENT || event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_KP_ENTER) {
          gui_detach_texture(display3, "Display 3");
        }

        gui_handle_evt(&event); // GUI global event handler
      }
    }

    gui_redraw_texture(display);
    gui_redraw_texture(display2);
    gui_redraw_texture(display3);

    usleep(8000);
  }

  gui_redraw_texture(display);
  gui_redraw_texture(display2);
  gui_redraw_texture(display3);

  SDL_Quit();
  free(cu);
}

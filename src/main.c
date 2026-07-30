#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <control.h>
#include <gui.h>
#include <interpreter.h>
#include <peripheral/peripheral.h>
#include <program.h>

#include <peripheral/display.h>

#define LOG_MODULE "MAIN"
#include <log.h>

uint32_t active_window_id = 0;

uint8_t on_port_read(uint8_t port) { return 0x00; }
void on_port_write(uint8_t port, uint8_t value) {}

static control_unit_t *init_control_unit(void) {
  control_unit_t *cu = malloc(sizeof(control_unit_t));
  if (cu == NULL)
    return NULL;

  memset(cu, 0, sizeof(control_unit_t));

  uint32_t size;
  uint8_t *program = load_program(&size);

  memcpy(cu->ram.ram.bank_raw[0], program, size);

  LOG_INFO("Loaded program %d bytes", sizeof(program));
  return cu;
}

int main(void) {
  log_setlvl(LOG_LVL_DEBUG);
  log_set_color(1);

  /* === Init simulation === */
  control_unit_t *cu = init_control_unit();
  if (cu == NULL) {
    LOG_ERROR("Failed to create control unit");
    return -1;
  }

  /* === Create peripherals === */

  // int display = per_create(&per_display, NULL, cu, 0, 0);
  /*int display1 = gui_create(-1, 800, 800, 0, 0, "Display");
  int display2 = gui_create(display1, 600, 600, 100, 100, NULL);*/

  int display1 = per_create(&per_display, -1, cu, 0, 0);
  int display2 = per_create(&per_display, display1, cu, 100, 100);

  LOG_INFO("Created peripherals, D1=%d, D2=%d", display1, display2);

  if (display1 < 0 || display2 < 0)
    return -1;

  SDL_Event event;
  int running = 1;

  fprintf(stdout, "\r\n");
  LOG_DEBUG("DEBUG");
  LOG_INFO("INFO");
  LOG_WARNING("WARNING");
  LOG_EXCEPT("EXCEPTION");
  LOG_ERROR("ERROR");
  fprintf(stdout, "\r\n");

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

      per_handle_event(&event);
    }

    update_control_unit(cu);

    per_update(display1);
    per_update(display2);

    per_redraw(display1);
    per_redraw(display2);

    usleep(8000);
  }

  per_destroyall();
  // per_destroy(display);
  SDL_Quit();
  free(cu);

  LOG_INFO("All done, exit");
  return 0;
}

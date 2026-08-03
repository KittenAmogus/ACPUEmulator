#define LOG_MODULE "MAIN"
#include <logger.h>

#include <stdlib.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <control.h>
#include <peri.h>
#include <program.h>
#include <simulation.h>

#include <peripheral/control.h>
#include <peripheral/display.h>

int peri[16];
int peri_count = 0;

void main_on_peri_destroyed(int destrID) {
  --peri_count;
  peri[destrID] = -1;

  LOG_DEBUG("Destroyed peri %d", destrID);
}

void main_on_peri_moved(int oldID, int newID) {
  peri[oldID] = -1;
  if (oldID > newID)
    --peri_count;

  LOG_DEBUG("Moved peri %d -> %d", oldID, newID);
}

int main(void) {
  // Configure logger
  logger_level_set(LOG_LVL_DEBUG);
  logger_ansii_set(1);

  // Test messages
  LOG_DEBUG("DEBUG");
  LOG_INFO("INFO");
  LOG_WARNING("WARNING");
  LOG_EXCEPT("EXCEPTION");
  LOG_ERROR("ERROR");

  control_unit_t *control_unit = malloc(sizeof(control_unit_t));
  if (control_unit == NULL) {
    LOG_ERROR("Cannot allocate control unit");
    return -1;
  }

  // Create peripherals
  int control = per_create(&per_control, -1, NULL, 0, 0);
  // per_init(control, control_unit);

  int display = per_create(&per_display, -1, NULL, 0, 0);
  // per_init(display, control_unit);

  peri[peri_count++] = control;
  peri[peri_count++] = display;

  for (int i = 0; i < peri_count; ++i)
    per_init(peri[i], control_unit);

  per_attach(display, control);

  // Init and start simulation
  sim_init(control_unit);
  sim_lock();

  // Copy program
  uint32_t size;
  const uint8_t *program = program_load(&size);

  int current_bank = 1;
  uint8_t addr = 0;

  for (size_t i = 0; i < size; ++i) {

    control_unit->ram.active_bank_id = current_bank; // Choose bank
    ram_write(&control_unit->ram, addr, program[i]);
    // LOG_DEBUG("PGM %02x, %d", addr, current_bank);

    for (int k = 0; k < peri_count; ++k) {
      // LOG_DEBUG("UPD i=%d", i);
      per_update(peri[k]);
    }

    // Next bank
    ++addr;
    if (addr == 0) {
      addr = 0x80;    // Reset to start of bank
      ++current_bank; // Next bank
    }
  }
  control_unit->ram.active_bank_id = 1; // Choose bank 1

  sim_continue();
  sim_unlock();

  int running = 1;
  while (running) {
    SDL_Event evt;
    sim_lock();
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        LOG_INFO("QUIT Event");
        running = 0;
      }

      if (evt.type == SDL_KEYDOWN) {
        if (evt.key.keysym.sym == SDLK_SPACE) {
          LOG_INFO("Received SPACE");
          sim_toggle();
        }
      }

      per_handle_event(&evt);
    }

    sim_unlock();

    // Reading data
    sim_lock();

    for (int i = 0; i < peri_count; ++i) {
      // LOG_DEBUG("UPD i=%d", i);
      per_update(peri[i]);
    }

    sim_unlock();

    for (int i = 0; i < peri_count; ++i)
      per_redraw(peri[i]);

    usleep(8000);
  }

  // Destroy peripherals
  sim_lock();

  for (int i = 0; i < peri_count; ++i)
    per_destroy(peri[i]);

  sim_unlock();

  // Close simulation
  sim_close();

  LOG_DEBUG("end");
  return 0;
}

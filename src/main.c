#define LOG_MODULE "MAIN"
#include <logger.h>

#include <stdint.h>

#include <control_unit.h>
#include <ram.h>

const uint8_t program[] = {
    0x01, // hlt
};

int main(void) {

  // Setup logger
  logger_level_set(LOG_LVL_DEBUG);
  logger_ansii_set(1);

  // Create CU
  control_unit_t *cu = cu_init();

  // Load program
  for (uint32_t i = 0; i < sizeof(program); ++i) {
    ram_write(&cu->RAM, (uint8_t)i, program[i]);
  }

  // Mainloop
  int running = 1;
  while (running) {

    if (!cu_step(cu)) {
      running = 0;
      LOG_INFO("CPU Halted");
    }
  }

  cu_free(cu);
  return 0;
}

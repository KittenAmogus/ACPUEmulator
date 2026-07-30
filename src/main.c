#define LOG_MODULE "MAIN"
#include <logger.h>

#include <SDL2/SDL.h>
#include <simulation.h>
#include <unistd.h>

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

  // Init simulation
  sim_init();
  LOG_INFO("Simulation state: %d", sim_state());

  // Start simulation
  sim_continue();
  LOG_INFO("Simulation state: %d", sim_state());

  for (int i = 0; i < 10; ++i) {
    LOG_DEBUG("tick");

    // Redraw
    sim_lock();
    LOG_INFO("Simulation state: %d", sim_state());

    // Draw debug delay
    usleep(1600);

    if (i == 3) {
      LOG_DEBUG("Pausing");

      // Pause test
      sim_pause();
      LOG_INFO("Simulation state: %d", sim_state());
      sleep(3); // Wait 3 sec
      sim_continue();
      LOG_INFO("Simulation state: %d", sim_state());
    }

    sim_unlock();

    // FPS Delay
    usleep(800000);
  }

  // Close simulation
  sim_close();
  LOG_DEBUG("end");
  return 0;
}

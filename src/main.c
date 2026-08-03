#define LOG_MODULE "MAIN"
#include <logger.h>

#include <simulation.h>
#include <unistd.h>

int main(void) {

  // Setup logger
  logger_level_set(LOG_LVL_DEBUG);
  logger_ansii_set(1);

  sim_init(64);
  sim_continue();

  for (int i = 0; i < 10; ++i) {
    usleep(500000);
    LOG_INFO("TPS: %.2f", sim_tps());
  }

  sim_close();
  return 0;
}

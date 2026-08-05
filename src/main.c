#define LOG_MODULE "MAIN"
#include <logger.h>

#include <simulation.h>

#include <stdlib.h>
#include <unistd.h>

static void cleanup(void) { sim_close(); }

int main(int argc, char *argv[]) {
  atexit(cleanup);

  // Get target TPS
  uint64_t target_tps = 8;

  if (argc > 1) {
    char *endptr;
    int64_t parsed_tps = strtol(argv[1], &endptr, 10);

    if (argv[1] == endptr || *endptr != 0) {
      LOG_ERROR("Cannot get target TPS, '%s' is not a number", argv[1]);
      LOG_ERROR("Usage: Emulator [tps]");
      return -1;
    }

    if (parsed_tps <= 0) {
      LOG_ERROR("TPS must be > 0");
      return -1;
    }

    target_tps = (uint64_t)parsed_tps;
  }

  // Setup logger
  logger_level_set(LOG_LVL_INFO);
  logger_ansii_set(1);

  sim_init(target_tps);
  sim_continue();

  do {
    usleep(500000);
    sim_state_e state = sim_state();
    // LOG_DEBUG("STATE %d", (int)state);
    if (state == SIM_STOPPED) {
      LOG_DEBUG("State changed to STOPPED");
      // sim_close();
      break;
    }
    // LOG_DEBUG("TPS: %.2f", sim_tps());
  } while (1);

  LOG_INFO("Exit code 0");
  return 0;
}

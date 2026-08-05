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
  logger_level_set(LOG_LVL_DEBUG);
  logger_ansii_set(1);

  sim_init(target_tps);
  sim_continue();

  for (int i = 0; i < 10; ++i) {
    usleep(500000);
    LOG_INFO("TPS: %.2f", sim_tps());
  }

  return 0;
}

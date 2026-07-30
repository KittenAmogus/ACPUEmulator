#define LOG_MODULE "MAIN"
#include <logger.h>

#include <unistd.h>

#include <SDL2/SDL.h>

SDL_atomic_t sim_running;

int simulation_thread(void *data) {
  SDL_mutex *sim_mutex = (SDL_mutex *)data;
  SDL_UnlockMutex(sim_mutex);

  while (SDL_AtomicGet(&sim_running)) {
    SDL_LockMutex(sim_mutex);

    LOG_DEBUG("tick");

    SDL_UnlockMutex(sim_mutex);
    usleep(1000000 / 10);
  }
  return 0;
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

  // Multithreading, yea!
  SDL_mutex *mutex = SDL_CreateMutex();
  SDL_Thread *sim_thread = SDL_CreateThread(
      (SDL_ThreadFunction)simulation_thread, "sim_thread", (void *)mutex);

  // Run simulation
  SDL_AtomicSet(&sim_running, 1);

  LOG_DEBUG("start");

  for (int j = 0; j < 4; ++j) {
    SDL_LockMutex(mutex);
    for (int i = 0; i < 10; ++i) {
      LOG_DEBUG("tack");
    }
    SDL_UnlockMutex(mutex);
    sleep(1);
  }

  LOG_DEBUG("end");

  SDL_AtomicSet(&sim_running, 0);
  SDL_WaitThread(sim_thread, NULL);
  return 0;
}

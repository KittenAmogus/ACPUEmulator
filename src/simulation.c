#define LOG_MODULE "SIMULATION"
#include <logger.h>

#include <SDL2/SDL.h>
#include <simulation.h>
#include <unistd.h>

static int init = 0;
static SDL_Thread *thread;
static SDL_mutex *mutex = NULL;
static SDL_atomic_t running;   // Is running (0 - paused)
static SDL_atomic_t need_stop; // Closing (1 - break mainloop)

static int sim_thread_function(void *data) {
  LOG_INFO("Simulation started");

  while (!SDL_AtomicGet(&need_stop)) {

    // Paused and not need to stop
    while (!SDL_AtomicGet(&need_stop) && (SDL_AtomicGet(&running) == 0)) {
      LOG_DEBUG("sim sleep");
      usleep(400000);
    }
    LOG_INFO("Unpaused");

    // Running and not need to stop
    while (!SDL_AtomicGet(&need_stop) && (SDL_AtomicGet(&running) != 0)) {
      LOG_DEBUG("sim tick");

      // Some updates
      sim_lock();
      usleep(8000);
      sim_unlock();

      // TPS Delay
      usleep(400000);
    }
    LOG_INFO("Paused");
  }

  return 0;
}

int sim_init(void) {
  if (init)
    return 0;

  // Create mutex
  mutex = SDL_CreateMutex();

  // Set paused state
  SDL_AtomicSet(&running, 0);
  SDL_AtomicSet(&need_stop, 0);
  init = 1;

  thread = SDL_CreateThread(sim_thread_function, "sim_thread", NULL);
  return 1;
}

int sim_close(void) {
  if (!init)
    return 0;

  // Set closed state
  SDL_AtomicSet(&running, 1);   // Need to be 1, otherwise it won't stop
  SDL_AtomicSet(&need_stop, 1); // Need to stop

  SDL_WaitThread(thread, NULL);
  LOG_INFO("Closed simulation");
  return 1;
}

int sim_pause(void) {
  if (!init)
    return 0;

  // Can't pause stopping simulation
  if (!SDL_AtomicGet(&need_stop))
    SDL_AtomicSet(&running, 0);

  LOG_INFO("Paused simulation");
  return 1;
}

int sim_continue(void) {
  if (!init)
    return 0;

  // Can't unpause stopping simulation
  if (!SDL_AtomicGet(&need_stop))
    SDL_AtomicSet(&running, 1);

  LOG_INFO("Continued simulation");
  return 1;
}

int sim_lock(void) {
  SDL_LockMutex(mutex);
  LOG_DEBUG("Locked sim");
  return 1;
}

int sim_unlock(void) {
  SDL_UnlockMutex(mutex);
  LOG_DEBUG("Unlocked sim");
  return 1;
}

sim_state_e sim_state(void) {
  int running_v = SDL_AtomicGet(&running);
  int need_stop_v = SDL_AtomicGet(&need_stop);

  if (init) {
    if (running_v)
      return SIM_RUNNING;
    return SIM_PAUSED;
  }

  if (need_stop_v)
    return SIM_CLOSED;
  return SIM_NOT_INIT;
}

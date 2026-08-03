#include <control.h>
#define LOG_MODULE "SIMULATION"
#include <logger.h>

#include <SDL2/SDL.h>
#include <simulation.h>
#include <unistd.h>

#define MAX_TPS 100;

static int init = 0;
static SDL_Thread *thread;
static SDL_mutex *mutex = NULL;
static SDL_atomic_t running;   // Is running (0 - paused)
static SDL_atomic_t need_stop; // Closing (1 - break mainloop)
__useconds_t sleep_useconds = 1000000 / MAX_TPS;

static int sim_tick(control_unit_t *cu, int *update_cu) {

  if (*update_cu) {
    if (!cu_update(cu)) {
      LOG_INFO("ControlUnit halted");
      *update_cu = 0;

      sim_lock();
      SDL_AtomicSet(&running, 0);
      SDL_AtomicSet(&need_stop, 1);
      sim_unlock();
    }
  }

  sim_lock();
  sim_unlock();

  usleep(sleep_useconds);
  return 1;
}

static int sim_thread_function(void *data) {
  control_unit_t *cu = (control_unit_t *)data;

  LOG_DEBUG("Simulation thread started");

  int update_cu = 1;

  while (!SDL_AtomicGet(&need_stop)) {

    // Paused and not need to stop
    while (!SDL_AtomicGet(&need_stop) && (SDL_AtomicGet(&running) == 0)) {
      usleep(8000);
    }

    // Running and not need to stop
    while (!SDL_AtomicGet(&need_stop) && (SDL_AtomicGet(&running) != 0)) {
      sim_tick(cu, &update_cu);
    }
  }

  return 0;
}

int sim_init(control_unit_t *cu) {
  if (init)
    return 0;

  // Create mutex
  mutex = SDL_CreateMutex();

  // Set paused state
  SDL_AtomicSet(&running, 0);
  SDL_AtomicSet(&need_stop, 0);
  init = 1;

  thread = SDL_CreateThread(sim_thread_function, "sim_thread", (void *)cu);

  LOG_INFO("Simulation started");
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

int sim_toggle(void) {
  if (!init)
    return 0;

  // Can't toggle stopping simulation
  if (!SDL_AtomicGet(&need_stop))
    SDL_AtomicSet(&running, !SDL_AtomicGet(&running));

  LOG_INFO("Toggled simulation, running = %d", SDL_AtomicGet(&running));
  return 1;
}

int sim_lock(void) {
  SDL_LockMutex(mutex);
  // LOG_DEBUG("Locked sim");
  return 1;
}

int sim_unlock(void) {
  SDL_UnlockMutex(mutex);
  // LOG_DEBUG("Unlocked sim");
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

#include "SDL_timer.h"
#include <stdint.h>
#define LOG_MODULE "SIMULATION"
#include <logger.h>

#include <SDL2/SDL.h>
#include <simulation.h>

#include <unistd.h>

static sim_mgr_t mgr = {0};

static SDL_Thread *thread = NULL;
static SDL_mutex *mutex = NULL;
static SDL_atomic_t running;
static SDL_atomic_t need_stop;

static int sim_delay(void) {
  mgr.end = SDL_GetPerformanceCounter();
  uint64_t elapsed = mgr.end - mgr.start;

  if (elapsed < mgr.target_ticks) {
    uint64_t tts = mgr.target_ticks - elapsed;
    double mts = ((double)tts * 1000.0) / (double)mgr.freq;

    if (mts > 1.5) {
      SDL_Delay((uint32_t)(mts - 1.0));
    }

    while (SDL_GetPerformanceCounter() - mgr.start < mgr.target_ticks) {

#if defined(__x86_64__) || defined(_M_X64)
      __builtin_ia32_pause();
#endif
    }
  }

  else {
    LOG_WARNING("Tick took too long");
  }
  mgr.start = SDL_GetPerformanceCounter();

  return 1;
}

static int sim_calctps(void) {
  uint64_t now = SDL_GetPerformanceCounter();
  uint64_t elapsed = now - mgr.tps_start;

  if (elapsed > mgr.freq) {
    mgr.tps = ((double)mgr.ticks * (double)mgr.freq) / (double)elapsed;

    mgr.ticks = 0;
    mgr.tps_start = now;
  }

  return 1;
}

static int sim_tick(void) {
  if (!cu_step(mgr.cu))
    return 0;

  return 1;
}

static int sim_sleep(void) {
  usleep(8000);
  return 1;
}

// TODO: program
const uint8_t program[] = {
    0x00, // nop

    0x56,
    13, // ldi c, counter

    0x48, // ld a, c
    // 0xA8, // mov a, c

    0xB0, // test a

    0x08,
    12, // jz label

    // 0x54,
    // 0x00, // ldi a, 0x00

    0x70, // dec a

    0x30,
    13, // st a, counter

    0x03,
    1, // jmp 0x01

    //          label:
    0x01, // hlt

    0x03, // counter: db 0x00
};

static int sim_thread_func(void *_) {

  // Prepare
  LOG_DEBUG("Thread started");

  mgr.start = SDL_GetPerformanceCounter();
  mgr.tps_start = mgr.start;

  LOG_DEBUG("Initial target TPS: %" PRIu64, mgr.target_tps);

  // TODO: program
  memcpy(mgr.cu->RAM.memory.bank[0], program, sizeof(program));

  while (!SDL_AtomicGet(&need_stop)) {

    LOG_DEBUG("State: Running");
    // Active state
    while (SDL_AtomicGet(&running) && !SDL_AtomicGet(&need_stop)) {

      // Update simulation
      sim_lock();
      if (!sim_tick()) {
        SDL_AtomicSet(&need_stop, 1);
      }
      ++mgr.ticks;

      // Wait for valid TPS
      sim_unlock();
      sim_delay();
      sim_calctps();
    }

    LOG_DEBUG("State: Paused");
    // Paused state
    while (!SDL_AtomicGet(&running) && !SDL_AtomicGet(&need_stop)) {
      sim_sleep();
    }
  }

  SDL_AtomicSet(&running, 0);
  LOG_DEBUG("Thread exit");
  return 1;
}

int sim_init(uint64_t target_tps) {
  mgr.cu = cu_init();

  SDL_AtomicSet(&running, 0);   // Paused
  SDL_AtomicSet(&need_stop, 0); // Not stopping

  mutex = SDL_CreateMutex();
  thread = SDL_CreateThread(sim_thread_func, "simulation", NULL);

  sim_set_target_tps(target_tps);

  LOG_DEBUG("Initialized simulation");
  return 1;
}

int sim_close(void) {
  SDL_AtomicSet(&need_stop, 1);
  SDL_WaitThread(thread, NULL);
  SDL_DestroyMutex(mutex);

  cu_free(mgr.cu);
  SDL_Quit();
  LOG_DEBUG("Closed");
  return 1;
}

int sim_toggle(void) {
  SDL_AtomicSet(&running, !SDL_AtomicGet(&running));
  LOG_DEBUG("Toggled, %d", SDL_AtomicGet(&running));
  mgr.ticks = 0;
  mgr.start = 0;
  mgr.tps_start = SDL_GetPerformanceCounter();
  return 1;
}

int sim_pause(void) {
  SDL_AtomicSet(&running, 0);
  LOG_DEBUG("Paused");
  mgr.ticks = 0;
  mgr.start = 0;
  mgr.tps_start = SDL_GetPerformanceCounter();
  return 1;
}

int sim_continue(void) {
  SDL_AtomicSet(&running, 1);
  LOG_DEBUG("Continued");
  mgr.ticks = 0;
  mgr.start = 0;
  mgr.tps_start = SDL_GetPerformanceCounter();
  return 1;
}

int sim_lock(void) {
  SDL_LockMutex(mutex);
  // LOG_DEBUG("Locked");
  return 1;
}

int sim_unlock(void) {
  SDL_UnlockMutex(mutex);
  // LOG_DEBUG("Unlocked");
  return 1;
}

double sim_tps(void) { return mgr.tps; }

int sim_set_target_tps(uint64_t tps) {
  if (tps <= 0) {
    LOG_EXCEPT("Invalid TPS: %" PRIu64, tps);
    tps = 1;
  }

  mgr.freq = SDL_GetPerformanceFrequency();
  mgr.target_tps = tps;
  mgr.target_ticks = mgr.freq / tps;
  LOG_DEBUG("Target TPS: %" PRIu64 ", Target ticks: %" PRIu64, mgr.target_tps,
            mgr.target_ticks);
  return 1;
}

sim_state_e sim_state(void) {
  return (SDL_AtomicGet(&running) << 1) | SDL_AtomicGet(&need_stop);
}

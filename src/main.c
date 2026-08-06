#define LOG_MODULE "MAIN"
#include <logger.h>

#include <simulation.h>

#include <stdlib.h>
#include <unistd.h>

#include <gui.h>

static void cleanup(void) { sim_close(); }

static int _redraw1(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer);
  return 1;
}

static int _redraw2(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderClear(renderer);
  return 1;
}

static int _redraw3(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
  SDL_RenderClear(renderer);
  return 1;
}

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

  int display = gui_create(-1, 800, 600, "Display", _redraw1);
  int display2 = gui_create(-1, 900, 700, "Display2", _redraw2);
  int d2 = -1; // = gui_create(-1, 400, 300, "D2", NULL);

  int running = 1;
  int closed = 0;

  do {

    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        running = 0;
      }

      if (evt.type == SDL_KEYDOWN) {
        if (evt.key.keysym.sym == SDLK_a && d2 >= 0) {

          int parent, detached;
          if (gui_get_state(d2, NULL, &detached, &parent)) {
            if (detached || 1) {

              LOG_DEBUG("DETACHED, PARENT=%d, DISPLAY=%d, DIS2=%d", parent,
                        display, display2);

              if (parent == display) {
                LOG_DEBUG("D1 -> D2, P=%d", parent);
                gui_attach(d2, display2);
              }

              else if (parent == display2) {
                LOG_DEBUG("D2 -> D1, P=%d", parent);
                gui_attach(d2, display);
              }
            }
          }

        } else if (evt.key.keysym.sym == SDLK_d && d2 >= 0) {
          LOG_DEBUG("Detaching d2");
          gui_detach(d2);

        } else if (evt.key.keysym.sym == SDLK_RETURN) {

          if (d2 < 0) {
            LOG_DEBUG("Creating d2");
            d2 = gui_create(display, 100, 200, "D2", _redraw3);
          } else {
            LOG_DEBUG("Moving d2");
            int x, y;
            if (gui_get_pos(d2, NULL, NULL, &x, &y)) {
              gui_set_pos(d2, x + 2, y + 1);
            }
          }
        }
      }
    }

    // usleep(500000);
    sim_state_e state = sim_state();
    // LOG_DEBUG("STATE %d", (int)state);
    if (!closed && state == SIM_STOPPED) {
      LOG_DEBUG("State changed to STOPPED");
      closed = 1;
      // sim_close();
      // break;
    }

    if (d2 >= 0)
      gui_call_redraw(d2);

    gui_call_redraw(display2);
    gui_call_redraw(display);

    gui_present_all();

    // LOG_DEBUG("TPS: %.2f", sim_tps());
    usleep(8000);
  } while (running);

  if (sim_state() != SIM_STOPPED)
    sim_close();

  gui_destroyall();
  LOG_INFO("Exit code 0");
  return 0;
}

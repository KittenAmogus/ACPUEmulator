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

  // Init and start simulation
  sim_init();
  sim_continue();

  // Create SDL window and renderer
  int a = SDL_WINDOWPOS_CENTERED;
  uint32_t fl = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Window *window =
      SDL_CreateWindow("Title", a, a, 800, 800, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, fl);

  int running = 1;
  while (running) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        running = 0;
      }

      if (evt.type == SDL_KEYDOWN) {
        if (evt.key.keysym.sym == SDLK_SPACE) {
          LOG_INFO("Received SPACE");
          sim_lock();
          sim_toggle();
          sim_unlock();
        }
      }
    }

    // Reading data...
    sim_lock();
    usleep(80000);
    sim_unlock();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);

    SDL_Rect rect = {
        .x = 100,
        .y = 100,
        .w = 256,
        .h = 256,
    };

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
  }

  // CLose SDL
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  // Close simulation
  sim_close();
  LOG_DEBUG("end");
  return 0;
}

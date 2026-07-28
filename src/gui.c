#include <SDL2/SDL.h>
#include <gui.h>
#include <stdio.h>

#define GUI_WINDOWWIDTH 1600
#define GUI_WINDOWHEIGHT 1000
#define GUI_DISPLAYWIDTH 1000
#define GUI_DISPLAYHEIGHT 1000

static const char *TITLE = "ArrowCPU Emulator";

static gui_data data;

int gui_init(void) {
  data.window =
      SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       GUI_WINDOWWIDTH, GUI_WINDOWHEIGHT, SDL_WINDOW_SHOWN);
  if (data.window == NULL) {
    fprintf(stderr, "GUI: %s\r\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }

  data.renderer = SDL_CreateRenderer(
      data.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  return 1;
}

int gui_update(void) {
  SDL_Event evt;
  while (SDL_PollEvent(&evt)) {
    // QUIT
    if (evt.type == SDL_QUIT) {
      printf("GUI: SDL_QUIT event\r\n");
      return 0;
    } else if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) {
      printf("GUI: SDL_QUIT event\r\n");
      return 0;
    }
  }

  SDL_SetRenderDrawColor(data.renderer, 0, 0, 0, 255);
  SDL_RenderClear(data.renderer);

  // Redraw sim

  SDL_RenderPresent(data.renderer);
  return 1;
}

void gui_exit(void) {
  SDL_DestroyRenderer(data.renderer);
  SDL_DestroyWindow(data.window);
  SDL_Quit();
  printf("GUI: Exit\r\n");
}

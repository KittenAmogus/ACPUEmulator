#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include <control.h>
#include <interpreter.h>

#define WIDTH 1280
#define HEIGHT 720

uint8_t program[] = {
    0xEC,       // rnd a
    0xED,       // rnd b
    0x64,       // add a, b
    0x54, 0x01, // ldi a, 0x01
    0x55, 0xAA, // ldi b, 0xAA
    0x00,       // nop
    0x00,       // nop
    0x01,       // hlt
};

control_unit_t *init_control_unit(void) {
  control_unit_t *cu = malloc(sizeof(control_unit_t));
  if (cu == NULL)
    return NULL;

  memset(cu, 0, sizeof(control_unit_t));
  memcpy(cu->ram.ram.bank_raw[0], program, sizeof(program));
  return cu;
}

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL: Init failed: %s\r\n", SDL_GetError());
    return -1;
  }

  SDL_Window *window =
      SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "SDL: Window creating failed: %s\r\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  /* === Init simulation === */

  control_unit_t *cu = init_control_unit();
  if (cu == NULL) {
    fprintf(stderr, "MAIN: Failed to create control unit\r\n");
    return -1;
  }

  int running = 1;
  SDL_Event event;

  printf("+-------+-------+------+------+------+------+--------+\r\n");
  printf("| IP    | IR    | A    | B    | C    | D    | F OCSZ |\r\n");
  printf("+-------+-------+------+------+------+------+--------+\r\n");
  while (running) {
    // TODO: Multithreading
    if (!update_control_unit(cu)) {
      break;
    }

    // Poll events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = 0;
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        running = 0;
    }

    // Clear
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Redraw

    // Flush display
    SDL_RenderPresent(renderer);
  }

  printf("+-------+-------+------+------+------+------+--------+\r\n");
  printf("+ HALTED --------------------------------------------+\r\n");
  printf("| IP    | IR    | A    | B    | C    | D    | F OCSZ |\r\n");
  printf("+-------+-------+------+------+------+------+--------+\r\n");

  free(cu);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <control.h>
#include <gui.h>
#include <interpreter.h>

uint8_t program[] = {
    // TODO: Assembler
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
  memcpy(cu->ram.ram.bank_raw[0], program, sizeof(program)); // TODO: Assembler
  return cu;
}

int main(void) {
  /* === Init simulation === */
  control_unit_t *cu = init_control_unit();
  if (cu == NULL) {
    fprintf(stderr, "MAIN: Failed to create control unit\r\n");
    return -1;
  }

  printf("+-------+-------+------+------+------+------+--------+\r\n");
  printf("| IP    | IR    | A    | B    | C    | D    | F OCSZ |\r\n");
  printf("+-------+-------+------+------+------+------+--------+\r\n");

  /* === Init GUI === */
  if (!gui_init())
    return -1;

  int running = 1;
  while (running) {
    // TODO: Multithreading
    if (!update_control_unit(cu)) {
      break;
    }

    // Update GUI
    if (!gui_update()) {
      running = 0;
    }
  }

  printf("+-------+-------+------+------+------+------+--------+\r\n");
  printf("+ HALTED --------------------------------------------+\r\n");
  printf("| IP    | IR    | A    | B    | C    | D    | F OCSZ |\r\n");
  printf("+-------+-------+------+------+------+------+--------+\r\n");

  // Clean up
  free(cu);
  gui_exit();
  return 0;
}

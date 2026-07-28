#include <ram.h>
#include <stdint.h>

uint8_t ram_read(struct RAM *ram, uint8_t addr) {
  // Read common
  if (addr < BANK_OFFSET) {
    // TODO: Keyboard support
    return ram->ram.bank_raw[ram->active_bank_id][addr];
    return 0;
  }

  // Read bank
  return ram->ram.bank_raw[ram->active_bank_id][addr - BANK_OFFSET];
}

uint8_t ram_write(struct RAM *ram, uint8_t addr, uint8_t value) {
  if (addr < BANK_OFFSET) {
    // TODO: Common support
    return value;
  }

  ram->ram.bank_raw[ram->active_bank_id][addr - BANK_OFFSET] = value;
  return value;
}

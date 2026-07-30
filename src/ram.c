#include <ram.h>
#include <stdint.h>

uint8_t ram_read(struct RAM *ram, uint8_t addr) {
  // TODO: Port reading
  if (addr >= PORT_OFFSET && addr < BANK_OFFSET) {
    return 0;
  }

  // Real data is not offset
  if (addr > BANK_OFFSET)
    addr -= BANK_OFFSET;

  // Read byte
  return ram->ram.bank_raw[ram->active_bank_id][addr];
}

void ram_write(struct RAM *ram, uint8_t addr, uint8_t value) {
  // TODO: Port writing
  if (addr >= PORT_OFFSET && addr < BANK_OFFSET) {
    return;
  }

  // Real data is not offset
  if (addr > BANK_OFFSET)
    addr -= BANK_OFFSET;

  // Write byte
  ram->ram.bank_raw[ram->active_bank_id][addr] = value;
}

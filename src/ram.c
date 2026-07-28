#include <ram.h>
#include <stdint.h>

extern uint8_t on_port_read(uint8_t port);
extern void on_port_write(uint8_t port, uint8_t value);

uint8_t ram_read(struct RAM *ram, uint8_t addr) {
  // Read common
  if (addr >= PORT_OFFSET && addr < BANK_OFFSET) {
    return on_port_read(addr);
  }

  // Read bank
  if (addr > BANK_OFFSET) // Bank offset
    addr -= BANK_OFFSET;
  return ram->ram.bank_raw[ram->active_bank_id][addr];
}

uint8_t ram_write(struct RAM *ram, uint8_t addr, uint8_t value) {
  if (addr >= PORT_OFFSET && addr < BANK_OFFSET) {
    on_port_write(addr, value);
    return value;
  }

  ram->ram.bank_raw[ram->active_bank_id][addr - BANK_OFFSET] = value;
  return value;
}

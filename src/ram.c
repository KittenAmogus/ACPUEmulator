#define LOG_MODULE "RAM"
#include <logger.h>

#include <ram.h>
#include <stdint.h>

#include <ports.h>

uint8_t ram_read(struct RAM *ram, uint8_t addr) {
  uint8_t data;

  if (port_call_read(addr, &data))
    return data;

  if (addr >= BANK_OFFSET) {
    return ram->ram
        .bank_raw[ram->active_bank_id][addr - BANK_OFFSET]; // Read real bank
  } else {
    return ram->ram.common_raw[addr]; // Read from common
  }
}

void ram_write(struct RAM *ram, uint8_t addr, uint8_t value) {

  if (port_call_write(addr, value))
    return;

  // Real data is not offset
  if (addr >= BANK_OFFSET)
    ram->ram.bank_raw[ram->active_bank_id][addr - BANK_OFFSET] = value;
  else {
    // if (addr < PORT_OFFSET)
    //   LOG_WARNING("Writing to COMMON, %02x AC=%d", addr,
    //   ram->active_bank_id);

    // Write byte
    ram->ram.common_raw[addr] = value;
  }
  // LOG_DEBUG("str %02x, [%02x]", value, addr);
}

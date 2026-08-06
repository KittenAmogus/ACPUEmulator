#define LOG_MODULE "RAM"
#include <logger.h>

#include <ram.h>

static ram_mgr_t mgr;

static inline ram_reg_t *getdata(int id) { return &(mgr.pool[id]); }

uint8_t ram_read(struct RAM *ram, uint8_t addr) {

  // LOG_DEBUG("LD [%d, %02x]", ram->active_bank, addr);

  // Read from active bank, with offset
  if (addr >= RAM_BANK_SIZE) {
    return ram->memory.bank[ram->active_bank][addr - RAM_BANK_SIZE];
  }

  // Read from bank 0, no offset
  return ram->memory.bank[0][addr];
}

void ram_write(struct RAM *ram, uint8_t addr, uint8_t value) {

  // Call all callbacks
  ram_reg_t *reg;
  for (int i = 0; i < mgr.count; ++i) {
    reg = getdata(i);
    if (addr >= reg->start && addr <= reg->end && reg->write) {

      // Write callback
      reg->write(addr, value);
    }
  }

  // Write byte to RAM
  if (addr >= RAM_BANK_SIZE) {

    // Write bank with offset
    ram->memory.bank[ram->active_bank][addr - RAM_BANK_SIZE] = value;
  } else {

    // Write bank 0, no offset
    ram->memory.bank[0][addr] = value;
  }

  // LOG_DEBUG("ST %02x, [%02x]", value, addr);
}

int ram_register(uint8_t start, uint8_t end, ram_write_t write) {
  if (mgr.count >= RAM_MAX_CALLBACKS)
    return -1; // No space

  // Create register data
  ram_reg_t *data = getdata(mgr.count);
  data->start = start;
  data->end = end;
  data->write = write;

  // Return ID
  ++mgr.count;
  return mgr.count - 1;
}

int ram_unregister(int id) {
  if (id < 0)
    return 0;

  --mgr.count; // Decrease count

  // Move last to freed
  if (id != mgr.count)
    mgr.pool[id] = mgr.pool[mgr.count];

  return 1;
}

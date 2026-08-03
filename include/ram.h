#ifndef RAM_H
#define RAM_H

#include <stdint.h>

#define RAM_BANK_START 1 // First bank after common space
#define RAM_BANK_COUNT 8 // Physical bank count

#define RAM_COMMON_SIZE 0x40                          // First 64 byte of bank0
#define RAM_DISPLAY_BUFFER_SIZE 0x40                  // Display buffer size
#define RAM_BANK_SIZE 0x80                            // Size of one RAM bank
#define RAM_RAM_SIZE (RAM_BANK_SIZE * RAM_BANK_COUNT) // Size of RAM

#define RAM_MAX_CALLBACKS 16 // Limit of registered callbacks

typedef int (*ram_write_t)(
    uint8_t addr, uint8_t value); // Write function type for registering

typedef struct {
  uint8_t common[RAM_COMMON_SIZE];
  uint8_t display[RAM_DISPLAY_BUFFER_SIZE];
} __attribute__((packed)) bank0_t;

typedef struct {
  uint8_t start;     // Call if addr >= start
  uint8_t end;       // Call if addr <= end
  ram_write_t write; // Call if write
} ram_reg_t;

typedef struct {
  ram_reg_t pool[RAM_MAX_CALLBACKS]; // All registered callbacks
  int count;                         // Count of registered callbacks
} ram_mgr_t;

typedef struct RAM {
  union {
    uint8_t raw[RAM_RAM_SIZE];                   // Raw bytes
    uint8_t bank[RAM_BANK_COUNT][RAM_BANK_SIZE]; // Bytes by bank
  } memory;

  uint32_t active_bank; // Actibe bank (switch via bank port)
} ram_t;

uint8_t ram_read(struct RAM *ram, uint8_t addr);              // Read byte
void ram_write(struct RAM *ram, uint8_t addr, uint8_t value); // Write byte

int ram_register(uint8_t start, uint8_t end,
                 ram_write_t write); // Register callback functions for
                                     // write specific addrs
int ram_unregister(int id);          // Unregister callback

#endif // RAM_H

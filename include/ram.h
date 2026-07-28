#ifndef RAM_H
#define RAM_H

#include <stdint.h>

#define BANK_START 1     // if (bank < start) bank = start
#define BANK_COUNT 8     // Count of physical banks
#define BANK_OFFSET 0x80 // First byte in physical bank
#define BANK_SIZE 0x80   // 128 byte
#define RAM_SIZE (BANK_SIZE * BANK_COUNT)

#define PORT_OFFSET 0x3A // Offset of first port from 0
#define PORT_COUNT 6     // Port count

#define COMM_SIZE 64         // Full common size (including ports)
#define DISPLAY_BUFF_SIZE 64 // Full display buffer size

typedef enum {
  BIT_IO_TERM = 0,          // Enable terminal
  BIT_IO_RSVD = 1,          // TODO I could not find this bit in specs
  BIT_IO_DIGINT = 2,        // Enable digital indicator
  BIT_IO_DIGINT_SIGNED = 3, // Digint signed mode (0 = unsigned)
  BIT_IO_DISPLAY = 4,       // Enable display
  BIT_IO_COLORED = 5,       // Display colored mode (0 = mono)
} bits_io_e;

/*
 * Ports:
 * 0x3A (digital):
 *  WRITE: Each sent byte is sent to digital indicator
 * 0x3B (digital_256):
 *  WRITE: Each sent byte is sent to digital indicator and multiplied by 256
 * 0x3C (term):
 *  WRITE: Output byte to terminal as text
 * 0x3D (term_graphics):
 *  WRITE: Output byte to terminal as pixels
 * 0x3E (io):
 *  WRITE: Select IO device
 *  READ: Read char from keyboard
 * 0x3F (bank):
 *  WRITE: Select RAM bank (0 = 1)
 */

typedef struct {
  uint8_t reserved[COMM_SIZE - PORT_COUNT]; // Common buffer
  uint8_t digital;                          // 0x3A
  uint8_t digital_256;                      // 0x3B
  uint8_t term;                             // 0x3C
  uint8_t term_graphics;                    // 0x3D
  uint8_t io;                               // 0x3E
  uint8_t bank;                             // 0x3F
  uint8_t display[DISPLAY_BUFF_SIZE];       // Display buffer
} __attribute__((packed)) common_t;

/*
 * RAM:
 * full:
 *  Raw bytes from RAM
 * bank_raw:
 *  All banks (including common) in 2D array
 * common_raw:
 *  Raw bytes from bank0 (common)
 * common:
 *  structured bytes from bank0 (common)
 */

typedef struct RAM {
  union {
    uint8_t full[RAM_SIZE];                  // Raw RAM
    uint8_t bank_raw[BANK_COUNT][BANK_SIZE]; // Via banks
    uint8_t common_raw[BANK_SIZE];           // Bank0 = common
    common_t common;
  } ram;

  uint32_t active_bank_id;
} ram_t;

uint8_t ram_read(struct RAM *ram, uint8_t addr);
uint8_t ram_write(struct RAM *ram, uint8_t addr, uint8_t value);

#endif // RAM_H

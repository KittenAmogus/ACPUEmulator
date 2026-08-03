#define LOG_MODULE "CONTROL UNIT"
#include <logger.h>

#include <control_unit.h>
#include <ram.h>
#include <stdlib.h>

static void log_state(control_unit_t *cu) {
  LOG_INFO(
      // Format
      "|| IP %02x | IR %02x || F %04b || A %02x | B %02x | C %02x | D %02x ||",

      // Special registers
      cu->CPU.ip, cu->CPU.ir, cu->CPU.f.w & 15,

      // GP registers
      cu->CPU.a, cu->CPU.b, cu->CPU.c, cu->CPU.d);
}

control_unit_t *cu_init(void) { return malloc(sizeof(control_unit_t)); }

void cu_free(control_unit_t *cu) { free(cu); }

int cu_step(control_unit_t *cu) {

  // Load instruction
  cu->CPU.ir = ram_read(&cu->RAM, cu->CPU.ip);
  log_state(cu); // Log state
  ++cu->CPU.ip;  // Next instruction

  // Execute instruction
  uint8_t ir = cu->CPU.ir;

  // NOP
  if (ir == 0x00) {
    // LOG_DEBUG("NOP instruction");
    return 1;
  }

  // HLT
  if (ir == 0x01) {
    LOG_DEBUG("HLT instruction");
    return 0;
  }

  // Reserved 0x02
  if (ir == 0x02) {
    LOG_WARNING("RESERVED | %02x (IP=%02x)", ir, cu->CPU.ip - 1);
    return 1; // Act as NOP
  }

  // Other instructions

  return 1;
}

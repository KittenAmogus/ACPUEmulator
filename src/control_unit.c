#include <interpreter.h>
#define LOG_MODULE "CONTROL UNIT"
#include <logger.h>

#include <control_unit.h>
#include <executor.h>
#include <interpreter.h>
#include <ram.h>
#include <stdlib.h>

#include <stdio.h>

#define IS_RESERVED(ir)                                                        \
  (ir == 0x02 || (ir >= 0x58 && ir < 0x60) || (ir >= 0xF0))

#define IS_JMP(ir) (ir >= 0x03 && ir < 0x30)
#define IS_STR(ir) (ir >= 0x30 && ir < 0x40)
#define IS_LDR(ir) (ir >= 0x40 && ir < 0x58)
#define IS_ALU(ir) (ir >= 0x60 && ir < 0xF0)

#define JMP(ir) (table_jmp[ir - 0x03])
#define STR(ir) (table_str[ir - 0x30])
#define LDR(ir) (table_ldr[ir - 0x40])
#define ALU(ir) (table_alu[ir - 0x60])

static void log_state(control_unit_t *cu) {
  printf(
      // Format
      "|| IP %02x | IR %02x || %c%c%c%c || A %02x | B %02x | C %02x | D %02x "
      "||\r\n",

      // Special registers
      cu->CPU.ip, cu->CPU.ir,

      // Flags
      (cu->CPU.f.b.z ? 'Z' : '.'), (cu->CPU.f.b.s ? 'S' : '.'),
      (cu->CPU.f.b.c ? 'C' : '.'), (cu->CPU.f.b.o ? 'O' : '.'),

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

  if (IS_RESERVED(ir)) {
    LOG_WARNING("Reserved %02x at %02x", ir, cu->CPU.ip - 1);
    return 1; // Can continue
  }

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

  LOG_DEBUG("Instruction: %02x", ir);

  // Other instructions
  if (IS_JMP(ir))
    execute_jmp(cu, JMP(ir));

  else if (IS_STR(ir))
    execute_str(cu, STR(ir));

  else if (IS_LDR(ir))
    execute_ldr(cu, LDR(ir));

  else if (IS_ALU(ir))
    execute_alu(cu, ALU(ir));

  else {
    LOG_ERROR("Invalid instruction, %02x", ir);
    return 0;
  }

  return 1;
}

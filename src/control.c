#define LOG_MODULE "CONTROL UNIT"
#include <logger.h>

#include <stdint.h>
#include <stdio.h>

#include <control.h>
#include <handler.h>
#include <interpreter.h>
#include <ram.h>

#define o_JMP 0x03
#define o_STR 0x30
#define o_LDR 0x40
#define o_ALU 0x60

// Reserved:     // 0x58 - 0x5F               // 0xF0 - 0xFF
#define IS_RSVD(ir) ((ir < 0x60 && ir > 0x57) || ((ir & 0xF0) == 0xF0))

#define IS_SPEC(ir) (ir < 0x03) // Special (NOP, HLT)
#define IS_JMP(ir) (ir < 0x30)  // Jump (JMP, JF, JNF)
#define IS_ST(ir) (ir < 0x40)   // Store (ST)
#define IS_LD(ir) (ir < 0x58)   // Load (LD, LDI)
// Else:                        // Computational (CLR, MOV, ALU funcs)

// TODO: Add CU logging to GUI
static inline void print_state(control_unit_t *cu) {
  printf(
      "| IP=%02x | IR=%02x | A=%02x | B=%02x | C=%02x | D=%02x | F=%04b |\r\n",
      cu->cpu.regs.ip, cu->cpu.regs.ir, cu->cpu.regs.gp.r.a,
      cu->cpu.regs.gp.r.b, cu->cpu.regs.gp.r.c, cu->cpu.regs.gp.r.d,
      cu->cpu.regs.f.w & 15);
}

int cu_update(control_unit_t *cu) {
  if (cu == (void *)0)
    return 0;

  // Load instruction
  cu->cpu.regs.ir = ram_read(&(cu->ram), cu->cpu.regs.ip);
  uint8_t ir = cu->cpu.regs.ir;

  // Print state
  print_state(cu);

  // Increment instruction pointer
  ++cu->cpu.regs.ip;

  // Reserved commands
  if (IS_RSVD(ir) || ir == 0x02) {
    LOG_WARNING("RESERVED (IP=%02x, IR=%02x) | BANK ADDR: %d", cu->cpu.regs.ip,
                ir, cu->ram.active_bank_id);
    return 1;
  }

  // Special (0-1)
  else if (IS_SPEC(ir)) {
    LOG_DEBUG("SPECIAL");
    switch (ir) {
    case ICMD_NOP:
      return 1; // Just continue
    case ICMD_HLT:
      return 0; // Just not continue
    }
  }

  // Jump
  else if (IS_JMP(ir)) {
    LOG_INFO("JMP %02x %02x", ir, ir - o_JMP);
    cu_jmp(cu, (cmd_jmp_t *)&(jmp_map[ir - o_JMP]));
  }

  // Store
  else if (IS_ST(ir)) {
    LOG_INFO("STR");
    cu_str(cu, (cmd_str_t *)&(store_map[ir - o_STR]));
  }

  // Load
  else if (IS_LD(ir)) {
    LOG_INFO("LDR");
    cu_ldr(cu, (cmd_ldr_t *)&(load_map[ir - o_LDR]));
  }

  // Computational (most alu)
  else {
    LOG_INFO("ALU");
    cu_alu(cu, (cmd_alu_t *)&(alu_map[ir - o_ALU]));
  }

  return 1;
}

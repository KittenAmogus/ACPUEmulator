#include <control.h>
#include <cpu.h>
#include <interpreter.h>
#include <ram.h>

#define IS_POSOP(i)                                                            \
  (i == IALU_ADD || i == IALU_ADC || i == IALU_INC || i == IALU_SHL ||         \
   i == IALU_RCL)
#define IS_NEGOP(i)                                                            \
  (i == IALU_SUB || i == IALU_SBB || i == IALU_DEC || i == IALU_NEG ||         \
   i == IALU_TEST)
#define IS_SROP(i) (i == IALU_SHR || i == IALU_SAR || i == IALU_RCR)

#define READ_RAM(addr) ram_read(&cu->ram, addr)
#define WRITE_RAM(addr, value) ram_write(&cu->ram, addr, value)
#define GET_NEAR() ram_read(&cu->ram, cu->cpu.regs.ip++)
#define REGISTER(id) (cu->cpu.regs.gp.gp[id])
#define GETFLAG(id) ((cu->cpu.regs.f.w >> id) & 1)
#define CLRFLAG(id) (cu->cpu.regs.f.w &= ~(1 << id))
#define SETFLAG(id) (cu->cpu.regs.f.w |= (1 << id))

// NOTE:
// IP is ALREADY incremented
// and points to the NEXT instruction

static uint8_t get_random(void) {
  static uint8_t lfsr = 0xE1;
  uint8_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
  lfsr = (lfsr >> 1) | (bit << 7);
  return lfsr;
}

void cu_alu(control_unit_t *cu, cmd_alu_t *cmd) {
  uint8_t va, vb;

  va = REGISTER(cmd->a);
  if (cmd->double_args) {
    vb = REGISTER(cmd->b);
  } else {
    vb = 0;
  }

  uint32_t res = 0;
  switch (cmd->type) {
  case IALU_CLR:
    res = 0;
    break;

  case IALU_MOV:
    res = vb;
    break;

  case IALU_AND:
    res = va & vb;
    break;

  case IALU_OR:
    res = va | vb;
    break;

  case IALU_XOR:
    res = va ^ vb;
    break;

  case IALU_ADD:
    res = va + vb;
    break;

  case IALU_ADC:
    res = va + vb + cu->cpu.regs.f.b.c;
    break;

  case IALU_SUB:
    res = va - vb;
    break;

  case IALU_SBB:
    res = va - vb - cu->cpu.regs.f.b.c;
    break;

  case IALU_TEST:
    res = va;
    break;

  case IALU_INC:
    res = va + 1;
    break;

  case IALU_DEC:
    res = va - 1;
    break;

  case IALU_NOT:
    res = ~va;
    break;

  case IALU_NEG:
    res = va | (1 << 7);
    break;

  case IALU_RND:
    res = get_random();
    break;

  case IALU_SHL:
    res = (va << 1) & ~1;
    break;

  case IALU_SHR:
    res = (va >> 1) & 0x7F;
    break;

  case IALU_SAR:
    res = (va >> 1);
    break;

  case IALU_RCL:
    res = ((va << 1) & ~1) | cu->cpu.regs.f.b.c;
    break;

  case IALU_RCR:
    res = ((va >> 1) & 0x7F) | (cu->cpu.regs.f.b.c << 7);
    break;
  }

  uint8_t res8 = (uint8_t)(res & 0xFF);
  REGISTER(cmd->a) = res8;

  // Flag Zero
  if (cmd->fmask & FMASK_Z) {
    cu->cpu.regs.f.b.z = (res == 0);
  }

  // Flag Sign
  if (cmd->fmask & FMASK_S) {
    cu->cpu.regs.f.b.s = (res >> 7) & 1;
  }

  // Flag Carry
  if (cmd->fmask & FMASK_C) {
    if (IS_POSOP(cmd->type)) {
      cu->cpu.regs.f.b.c = (res > 0xFF);
    } else if (IS_NEGOP(cmd->type)) {
      cu->cpu.regs.f.b.c = (res > 0xFF);
    } else if (IS_SROP(cmd->type)) {
      cu->cpu.regs.f.b.c = (res & 1);
    }
  }

  // Flag Overflow

  if (cmd->fmask & FMASK_O) {
    if (IS_POSOP(cmd->type))
      cu->cpu.regs.f.b.o = (((va ^ res8) & (vb ^ res8)) >> 7) & 1;
    else if (IS_NEGOP(cmd->type))
      cu->cpu.regs.f.b.o = (((va ^ vb) & (va ^ res8)) >> 7) & 1;
  }
}

void cu_jmp(control_unit_t *cu, cmd_jmp_t *cmd) {
  // Get destination
  uint8_t dest;

  if (cmd->reg_dest) // Get from register
    dest = REGISTER(cmd->reg_dest);
  else {
    // Load from RAM (ip + 1)
    dest = GET_NEAR();
  }

  // Check if must jump
  uint8_t flag = GETFLAG(cmd->flag);

  // Flag is same as needeed or not needed
  if (cmd->noflag || (flag ^ cmd->invert)) {
    // Jump (set ip)
    cu->cpu.regs.ip = dest;
  }
}

void cu_str(control_unit_t *cu, cmd_str_t *cmd) {
  uint8_t val, dest;
  val = REGISTER(cmd->src);

  // Load dest from RAM
  if (cmd->single) {
    dest = GET_NEAR();
  } else {
    dest = REGISTER(cmd->dest);
  }

  WRITE_RAM(dest, val);
}

void cu_ldr(control_unit_t *cu, cmd_ldr_t *cmd) {
  uint8_t val, src;

  // Load src from RAM
  if (cmd->single) {
    src = GET_NEAR();
  } else {
    src = REGISTER(cmd->src);
  }

  // Load value from ram if not imm
  if (cmd->imm) {
    val = src;
  } else {
    val = READ_RAM(src);
  }
  REGISTER(cmd->dest) = val;
}

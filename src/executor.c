#define LOG_MODULE "EXECUTOR"
#include <logger.h>

#include <executor.h>
#include <ram.h>
#include <stdint.h>

#define IS_CMD_SUB(op)                                                         \
  ((op) == IALU_SUB || (op) == IALU_SBB || (op) == IALU_DEC ||                 \
   (op) == IALU_TEST)

#define NEAR() (ram_read(&cu->RAM, cu->CPU.ip++))
#define REGISTER(idx) (cu->CPU.gp[idx])
#define GETFLAG(idx) ((cu->CPU.f.w >> idx) & 1)
#define CLRFLAG(idx) cu->CPU.f.w &= ~(1 << idx)

#define VALFLAG(idx, v)                                                        \
  do {                                                                         \
    CLRFLAG(idx);                                                              \
    cu->CPU.f.w |= (1 << idx);                                                 \
  } while (0)

static uint8_t get_random(void) { return 0; }

void execute_jmp(control_unit_t *cu, cmd_jmp_t instr) {
  uint8_t ip;

  if (instr.imm) // Dest is value in next byte
    ip = NEAR();
  else // Dest is value in register <dest>
    ip = REGISTER(instr.dest);

  // Check flag
  int flag = GETFLAG(instr.flag) ^ instr.invert;
  LOG_DEBUG("F %d = %d | I %d, RES %d", instr.flag, GETFLAG(instr.flag),
            instr.invert, flag);

  // Jump if flag matches or flag is not needed
  if (instr.noflag || (flag ^ instr.invert)) {
    cu->CPU.ip = ip;
  }
}

void execute_ldr(control_unit_t *cu, cmd_ldr_t instr) {
  uint8_t src;

  LOG_DEBUG("SRC: %02x", instr.src);

  if (instr.imm)
    src = NEAR();
  else
    src = REGISTER(instr.src);

  LOG_DEBUG(""
            "LDR {\r\n"
            "  .dest=%d,\r\n"
            "  .src=%02x,\r\n"
            "  .imm=%d,\r\n"
            "  .dir=%d,\r\n"
            "};",

            instr.dest, instr.src, instr.imm, instr.direct);

  if (instr.direct)
    REGISTER(instr.dest) = src;
  else
    REGISTER(instr.dest) = ram_read(&cu->RAM, src);
}

void execute_str(control_unit_t *cu, cmd_str_t instr) {
  uint8_t dest;

  if (instr.imm)
    dest = NEAR();
  else
    dest = REGISTER(instr.dest);

  LOG_DEBUG(""
            "STR {\r\n"
            "  .dest=%d,\r\n"
            "  .src=%02x,\r\n"
            "  .imm=%d,\r\n"
            "};",

            instr.dest, instr.src, instr.imm);

  ram_write(&cu->RAM, dest, REGISTER(instr.src));
}

void execute_alu(control_unit_t *cu, cmd_alu_t instr) {
  uint32_t result;
  uint8_t a, b = 0;

  a = REGISTER(instr.a);
  if (instr.dbl)
    b = REGISTER(instr.b);

  switch (instr.type) {
  case IALU_CLR:
    result = 0x00;
    break;

  case IALU_MOV:
    result = b;
    break;

  case IALU_AND:
    result = a & b;
    break;

  case IALU_OR:
    result = a = b;
    break;

  case IALU_XOR:
    result = a ^ b;
    break;

  case IALU_ADD:
    result = a + b;
    break;

  case IALU_ADC:
    result = a + b + cu->CPU.f.b.c;
    break;

  case IALU_SUB:
    result = a - b;
    break;

  case IALU_SBB:
    result = a - b - cu->CPU.f.b.c;
    break;

  case IALU_TEST:
    result = a;
    break;

  case IALU_INC:
    result = a + 1;
    break;

  case IALU_DEC:
    result = a - 1;
    break;

  case IALU_NOT:
    result = ~a;
    break;

  case IALU_NEG:
    result = a ^ (1 << 7);
    break;

  case IALU_RND:
    result = get_random();
    break;

  case IALU_SHL:
    result = (a << 1) & 0xFE;
    break;

  case IALU_SHR:
    result = (a >> 1) & 0x7F;
    break;

  case IALU_SAR:
    result = (a >> 1);
    break;

  case IALU_RCL:
    result = ((a << 1) & 0xFE) | (cu->CPU.f.b.c << 0);
    break;

  case IALU_RCR:
    result = ((a << 1) & 0xFE) | (cu->CPU.f.b.c << 7);
    break;

  default:
    result = a;
    break;
  }

  LOG_DEBUG("EXECUTOR (A=%02x, B=%02x) => RESULT=%02x (%04x)", a, b, result,
            result & 0xFF);

  // Zero
  if (instr.fmask & (1 << IFLAG_Z))
    cu->CPU.f.b.z = (result == 0);

  // Sign
  if (instr.fmask & (1 << IFLAG_S))
    cu->CPU.f.b.s = (result >> 7) & 1;

  // Carry
  if (instr.fmask & (1 << IFLAG_C))
    cu->CPU.f.b.c = (result > 0xFF);

  // Overflow
  if (instr.fmask & (1 << IFLAG_O)) {
    if (IS_CMD_SUB(instr.type)) {
      cu->CPU.f.b.o = (((a ^ b) & (a ^ result)) >> 7) & 1;
    } else {
      cu->CPU.f.b.o = (((a ^ result) & (b ^ result)) >> 7) & 1;
    }
  }

  REGISTER(instr.a) = result & 0xFF;
}

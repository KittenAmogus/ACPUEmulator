#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <control_unit.h>
#include <interpreter.h>
#include <stdint.h>

void execute_jmp(control_unit_t *cu, cmd_jmp_t instr);
void execute_ldr(control_unit_t *cu, cmd_ldr_t instr);
void execute_str(control_unit_t *cu, cmd_str_t instr);
void execute_alu(control_unit_t *cu, cmd_alu_t instr);

#endif // EXECUTOR_H

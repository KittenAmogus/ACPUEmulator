#ifndef HANDLER_H
#define HANDLER_H

#include <control.h>
#include <interpreter.h>

// Handlers
void cu_jmp(control_unit_t *cu, cmd_jmp_t *cmd);
void cu_str(control_unit_t *cu, cmd_str_t *cmd);
void cu_ldr(control_unit_t *cu, cmd_ldr_t *cmd);
void cu_alu(control_unit_t *cu, cmd_alu_t *cmd);

#endif // HANDLER_H

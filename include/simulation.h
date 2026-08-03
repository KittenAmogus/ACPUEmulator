#ifndef SIMULATION_H
#define SIMULATION_H

#include <control.h>

typedef enum {
  SIM_NOT_INIT = 0, // Not initialized
  SIM_PAUSED = 1,   // Initialized but not running
  SIM_RUNNING = 2,  // Initialized and running
  SIM_CLOSED = 3,   // Stopped because of halt
} sim_state_e;

int sim_init(control_unit_t *cu); // Init simulation
int sim_close(void);              // Close simulation

int sim_toggle(void);   // Toggle pause
int sim_pause(void);    // Pause executing
int sim_continue(void); // Continue executing or restart if stopped

int sim_lock(void);   // Lock simulation (mutex)
int sim_unlock(void); // Unlock simulation (mutex)

sim_state_e sim_state(void); // Get simulation state

#endif // SIMULATION_H

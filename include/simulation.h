#ifndef SIMULATION_H
#define SIMULATION_H

#include <control_unit.h>

typedef enum {
  SIM_NONE = 0,     // Not initialized  // !running !need_stop
  SIM_STOPPED = 1,  // Halted           // !running need_stop
  SIM_RUNNING = 2,  // Running          // running !need_stop
  SIM_STOPPING = 3, // Stopping         // running need_stop
} sim_state_e;

typedef struct {
  control_unit_t *cu;

  uint64_t start;
  uint64_t end;

  uint64_t freq;
  uint64_t target_tps;
  uint64_t target_ticks;

  uint64_t ticks;
  uint64_t tps_start;
  double tps;
} sim_mgr_t;

int sim_init(uint64_t target_tps); // Init simulation
int sim_close(void);               // Set to close and wait to close

int sim_toggle(void);   // Toggle pause
int sim_pause(void);    // Set pause
int sim_continue(void); // Clear pause

int sim_lock(void);   // Lock mutex
int sim_unlock(void); // Unlock mutex

sim_state_e sim_state(void);          // Get state
double sim_tps(void);                 // Get TPS
int sim_set_target_tps(uint64_t tps); // Set target TPS

#endif // SIMULATION_H

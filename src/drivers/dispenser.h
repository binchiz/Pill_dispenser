#ifndef dispenser_H
#define dispenser_H

#include <pico/types.h>

#define dispenser_TOTAL_PINS 4
#define dispenser_TOTAL_STEPS 8
typedef enum { CLOCKWISE, COUNTER_CLOCKWISE } direction_t;

typedef struct {
  bool calibrated;
  uint opto_fork;
  uint piezo;
  uint pins[dispenser_TOTAL_PINS];
  uint step_bits[dispenser_TOTAL_STEPS];
  direction_t default_direction;
  int step;
  int step_per_rev;
  int pills_left;
} dispenser_t;

void setup_dispenser(const dispenser_t *dispenser);
void run_dispenser(dispenser_t *dispenser);

#endif //dispenser_H

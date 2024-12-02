#ifndef MOTOR_H
#define MOTOR_H

#define MOTOR_TOTAL_PINS 4
#define MOTOR_TOTAL_STEPS 8
typedef enum { CLOCKWISE, COUNTER_CLOCKWISE } direction_t;

typedef struct {
  bool calibrated;
  uint pins[MOTOR_TOTAL_PINS];
  uint step_bits[MOTOR_TOTAL_STEPS];
  direction_t direction;
  int step;
  int step_per_rev;
} motor_t;

void setup_motor(const motor_t *motor);
void run_motor(motor_t *motor);

#endif //MOTOR_H

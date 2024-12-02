#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <pico/types.h>

#define MOTOR_TOTAL_PINS 4
#define MOTOR_TOTAL_STEPS 8

typedef enum { CLOCKWISE, COUNTER_CLOCKWISE } direction_t;

typedef struct {
    // todo: define the stepper motor fields, the below are just examples
    uint pins[MOTOR_TOTAL_PINS];
    uint steps_pattern[MOTOR_TOTAL_STEPS];
    direction_t direction;
    int step_per_rev;
    int step; // to keep track of the current step
    bool calibrated;
} stepper_motor_t;

// todo: define other functions to control the stepper motor, the below are just examples

/**
 * \brief Initialize the stepper motor by init the GPIOs
 */
void setup_stepper_motor(stepper_motor_t *stepper_motor);

/**
 * \brief Calibrate the stepper motor. The `step_per_rev` and `calibrated` will be updated.
 */
void calibrate_stepper_motor(stepper_motor_t *stepper_motor, int revolutions);

/**
 * \brief Run the stepper motor by one step. The `step` will be updated.
 */
void run_stepper_motor(stepper_motor_t *stepper_motor, int steps, direction_t direction);

#endif // STEPPER_MOTOR_H

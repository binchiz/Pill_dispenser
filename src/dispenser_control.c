#include "dispenser_control.h"
#include "drivers/stepper_motor.h"

// create an "instance" of the stepper motor here
// and use it in this file globally
static stepper_motor_t stepper_motor = {
    .pins = {2, 3, 6, 13},
    .steps_pattern = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001},
    .direction = CLOCKWISE,
    .step_per_rev = 0,
    .step = 0,
    .calibrated = false,
};

// todo: implement the functions, the below are just examples

void init_dispenser() {
    setup_stepper_motor(&stepper_motor);
    align_dispenser();
}

void align_dispenser() {
    direction_t direction = (stepper_motor.direction == CLOCKWISE) ? COUNTER_CLOCKWISE : CLOCKWISE;
    run_stepper_motor(&stepper_motor, 100, direction);
}
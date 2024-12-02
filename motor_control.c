#include <stdbool.h>
#include "motor.h"

static motor_t stepper_motor = {
    .pins = {2, 3, 6, 13},
    .step_bits = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001},
    .direction = COUNTER_CLOCKWISE,
    .step_per_rev = 0,
    .step = 0,
    .calibrated = false,
};

static int slices = 8;

void init_motor() {
    setup_motor(&stepper_motor);//td
}

void align_dispenser() {
    //td
}

void run_motor_nth_rev(const int n) {
    int steps_to_run = (stepper_motor.step_per_rev / slices)*n;
    for (int i = 0; i < steps_to_run; i++) {
        run_motor(&stepper_motor);
    }
}

void dispense_pill() {
    run_motor_nth_rev(1);
}
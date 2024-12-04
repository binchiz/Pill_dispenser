#include "dispenser_control.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "dispenser.h"

static dispenser_t stepper_dispenser = {
    .pins = {2, 3, 6, 13},
    .opto_fork = 28,
    .piezo = 27,
    .step_bits = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001},
    .default_direction = COUNTER_CLOCKWISE,
    .step_per_rev = 4096,
    .step = 0,
    .calibrated = false,
};

static int slices = 8;

void init_dispenser() {
    setup_dispenser(&stepper_dispenser);
    align_dispenser(1);
}

void align_dispenser(int rev) {
    int steps_count = 0;
    int count = 0;
    int previous_read = gpio_get(stepper_dispenser.opto_fork);
    int current_read;
    bool aligned = false;
    while (!aligned) {
        current_read = gpio_get(stepper_dispenser.opto_fork);
        if (previous_read == 1 && current_read == 0) aligned = true;
        else {
            previous_read = current_read;
            run_dispenser(&stepper_dispenser);
        }
    }
    while (count < rev) {
        run_dispenser(&stepper_dispenser);
        previous_read = current_read;
        current_read = gpio_get(stepper_dispenser.opto_fork);
        steps_count ++;
        if (previous_read==1&&current_read==0) count++;
    }
    stepper_dispenser.step_per_rev = steps_count/rev;
    stepper_dispenser.calibrated = true;
}

bool dispense_pill(int n) {
    bool pill = false;
    int steps_to_run = (stepper_dispenser.step_per_rev / slices) * n;
    uint start_time = to_ms_since_boot(get_absolute_time());
    for (int i = 0; i < steps_to_run; i++) {
        run_dispenser(&stepper_dispenser);
        if (!gpio_get(stepper_dispenser.piezo)) {
            uint current_time = to_ms_since_boot(get_absolute_time());
            if (current_time - start_time > 85) {
                pill = true;
                start_time = current_time;
            }
        }
    }
    return pill;
}
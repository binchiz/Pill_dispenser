#include "dispenser_control.h"
#include "drivers/dispenser.h"
#include "pico/stdlib.h"
#include "storage.h"
#include <stdio.h>

static dispenser_t dispenser = {
    .calibrated = false,
    .opto_fork = 28,
    .piezo = 27,
    .pins = {2, 3, 6, 13},
    .step_bits = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001},
    .direction = COUNTER_CLOCKWISE,
    .step = 0,
    .step_per_rev = 4096,
    .slices_runned = 0
};

static int slices = 8;

void init_dispenser() {
    setup_dispenser(&dispenser);
    init_storage();
    align_dispenser(1);
}

void align_dispenser(int rev) {
    int steps_count = 0;
    int count = 0;
    int previous_read = gpio_get(dispenser.opto_fork);
    int current_read;
    bool aligned = false;
    save_dispenser_state(DISPENSER_TURNING);
    while (!aligned) {
        current_read = gpio_get(dispenser.opto_fork);
        if (previous_read == 1 && current_read == 0) aligned = true;
        else {
            previous_read = current_read;
            run_dispenser(&dispenser);
        }
    }
    while (count < rev) {
        run_dispenser(&dispenser);
        previous_read = current_read;
        current_read = gpio_get(dispenser.opto_fork);
        steps_count++;
        if (previous_read == 1 && current_read == 0) count++;
    }
    if (rev > 0) dispenser.step_per_rev = steps_count / rev;
    dispenser.calibrated = true;
    save_dispenser_state(DISPENSER_IDLE);
}

void error_calibration(int n) {
    dispenser.direction = !(dispenser.direction);
    align_dispenser(0);
    dispenser.direction = !(dispenser.direction);
    run_n_slice(n);
}

void run_n_slice(int n) {
    int steps_to_run = (dispenser.step_per_rev / slices) * n;
    save_dispenser_state(DISPENSER_TURNING);
    for (int i = 0; i < steps_to_run; i++) {
        run_dispenser(&dispenser);
    }
    save_dispenser_state(DISPENSER_IDLE);
}

bool dispense_pill() {
    bool pill = false;
    int steps_to_run = dispenser.step_per_rev / slices;
    uint start_time = to_ms_since_boot(get_absolute_time());
    save_dispenser_state(DISPENSER_TURNING);
    for (int i = 0; i < steps_to_run; i++) {
        run_dispenser(&dispenser);
        if (!gpio_get(dispenser.piezo)) {
            uint current_time = to_ms_since_boot(get_absolute_time());
            if (current_time - start_time > 85) {
                pill = true;
                start_time = current_time;
            }
        }
    }
    save_dispenser_state(DISPENSER_IDLE);
    (dispenser.slices_runned)++;
    save_dispenser_slice_ran(dispenser.slices_runned);
    return pill;
}
#include "dispenser.h"
#include "led.h"
#include "lib/debug.h"
#include "lora.h"
#include "pico/stdlib.h"
#include "storage.h"
#include <stdio.h>

#define dispenser_TOTAL_PINS 4
#define dispenser_TOTAL_STEPS 8
typedef enum { CLOCKWISE, COUNTER_CLOCKWISE } direction_t;

typedef struct {
    bool calibrated;
    uint opto_fork;
    uint piezo;
    uint pins[dispenser_TOTAL_PINS];
    uint step_bits[dispenser_TOTAL_STEPS];
    direction_t direction;
    int step;
    int step_per_rev;
    int slices_ran;
} dispenser_t;

static dispenser_t dispenser = {
    .calibrated = false,
    .opto_fork = 28,
    .piezo = 27,
    .pins = {2, 3, 6, 13},
    .step_bits = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001},
    .direction = COUNTER_CLOCKWISE,
    .step = 0,
    .step_per_rev = 4096,
    .slices_ran = 0
};

static void run_dispenser();
static void stop_dispenser();
static void run_n_slice(int n);
static void piezo_handler();
static void enable_piezo();
static void disable_piezo();

static int slices = 8;
static int total_pills = 7;
static int falling_time = 85;
static int error_compensation = 110;
static bool piezo_triggered = false;

void init_dispenser() {
    for (int i = 0; i < sizeof(dispenser.pins) / sizeof(dispenser.pins[0]); i++) {
        gpio_init(dispenser.pins[i]);
        gpio_set_dir(dispenser.pins[i], GPIO_OUT);
    }
    gpio_init(dispenser.opto_fork);
    gpio_set_dir(dispenser.opto_fork, GPIO_IN);
    gpio_pull_up(dispenser.opto_fork);
    gpio_init(dispenser.piezo);
    gpio_set_dir(dispenser.piezo, GPIO_IN);
    gpio_pull_up(dispenser.piezo);
    // setup piezo interrupt handler
    gpio_add_raw_irq_handler(dispenser.piezo, piezo_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

void restore_dispenser_slices_ran(int slices_ran) {
    dispenser.slices_ran = slices_ran;
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
            run_dispenser();
        }
    }
    while (count < rev) {
        run_dispenser();
        previous_read = current_read;
        current_read = gpio_get(dispenser.opto_fork);
        steps_count++;
        if (previous_read == 1 && current_read == 0) count++;
    }
    // modify error compensation
    for (int i = 0; i < error_compensation; i++) {
        run_dispenser();
    }
    if (rev > 0) dispenser.step_per_rev = steps_count / rev;
    dispenser.calibrated = true;
    save_dispenser_state(DISPENSER_IDLE);
    stop_dispenser(); // set all pins to 0 to avoid overheating
}

void error_calibration() {
    dispenser.direction = !(dispenser.direction);
    align_dispenser(0);
    dispenser.direction = !(dispenser.direction);
    run_n_slice(dispenser.slices_ran);
    sleep_ms(1000);
}

bool dispense_pill() {
    bool pill = false;
    save_dispenser_slice_ran(dispenser.slices_ran);
    dispenser.slices_ran++;
    enable_piezo();
    run_n_slice(1);
    stop_dispenser(); // set all pins to 0 to avoid overheating
    // check if piezo has triggered
    if ((pill = piezo_triggered) == true) {
        dprintf(DEBUG_LEVEL_DEBUG, "Piezo detected during motor run\n");
    } else {
        // if no pill during turning, wait a little more time within the falling time
        dprintf(DEBUG_LEVEL_DEBUG, "Waiting for a little extra time\n");
        uint32_t start_time = to_ms_since_boot(get_absolute_time());
        while (!pill && to_ms_since_boot(get_absolute_time()) - start_time < falling_time) {
            if ((pill = piezo_triggered) == true) {
                dprintf(DEBUG_LEVEL_DEBUG, "Piezo detected after motor run: +%d ms\n",
                        to_ms_since_boot(get_absolute_time()) - start_time);
            }
        }
    }
    disable_piezo();
    return pill;
}

void dispense_all_pills() {
    int pills_left = total_pills - dispenser.slices_ran;
    for (int i = 0; i < pills_left; i++) {
        bool pill_dispensed = dispense_pill();
        if (pill_dispensed) {
            send_message(PILL_DISPENSED, "Pill Dispensed");
        } else {
            toggle_led_n_times(5);
            send_message(CNOT_DISPENSED, "Pill Not Dispensed");
        }
        sleep_ms(3000);
    }
    dispenser.slices_ran = 0;
    save_dispenser_slice_ran(0);
    send_message(DISPENSER_EMPTY, "Dispenser Empty");
}

/* -------------------
 * Private functions
 * ------------------- */

static void run_dispenser() {
    if (dispenser.direction == COUNTER_CLOCKWISE) {
        dispenser.step = (dispenser.step + 1) % 8;
    } else {
        dispenser.step = (dispenser.step - 1 + 8) % 8;
    }
    uint next_step = dispenser.step_bits[dispenser.step];
    for (int i = 0; i < sizeof(dispenser.pins) / sizeof(uint); i++) {
        gpio_put(dispenser.pins[i], (next_step >> i) & 1);
    }
    sleep_ms(2);
}

static void stop_dispenser() {
    for (int i = 0; i < sizeof(dispenser.pins) / sizeof(uint); i++) {
        gpio_put(dispenser.pins[i], 0);
    }
}

static void run_n_slice(int n) {
    int steps_to_run = (dispenser.step_per_rev / slices) * n;
    save_dispenser_state(DISPENSER_TURNING);
    for (int i = 0; i < steps_to_run; i++) {
        run_dispenser();
    }
    save_dispenser_state(DISPENSER_IDLE);
    stop_dispenser(); // set all pins to 0 to avoid overheating
}

static void piezo_handler() {
    if (gpio_get_irq_event_mask(dispenser.piezo) & GPIO_IRQ_EDGE_FALL) {
        gpio_acknowledge_irq(dispenser.piezo, GPIO_IRQ_EDGE_FALL);
        piezo_triggered = true;
    }
}

static void enable_piezo() {
    dprintf(DEBUG_LEVEL_DEBUG, "Piezo enabled\n");
    gpio_set_irq_enabled(dispenser.piezo, GPIO_IRQ_EDGE_FALL, true);
}

static void disable_piezo() {
    dprintf(DEBUG_LEVEL_DEBUG, "Piezo disabled\n");
    gpio_set_irq_enabled(dispenser.piezo, GPIO_IRQ_EDGE_FALL, false);
    piezo_triggered = false;
}

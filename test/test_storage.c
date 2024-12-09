#include "storage.h"
#include "pico/stdlib.h"
#include "utils/debug.h"
#include <stdio.h>

int main(void) {
    stdio_init_all();
    set_debug_level(DEBUG_LEVEL_DEBUG);

    printf("test_storage\n");
    int dispensed_slices = 0;
    dispenser_state_t dispenser_state = DISPENSER_IDLE;

    init_storage();

    // turning the dispenser
    save_dispenser_state(DISPENSER_TURNING);
    // turn_dispenser();
    load_dispenser_state(&dispenser_state);
    printf("dispenser state: %s\n", dispenser_state == DISPENSER_TURNING ? "DISPENSER_TURNING" : "DISPENSER_IDLE");

    // finished turning the dispenser
    save_dispenser_state(DISPENSER_IDLE);
    load_dispenser_state(&dispenser_state);
    printf("dispenser state: %s\n", dispenser_state == DISPENSER_TURNING ? "DISPENSER_TURNING" : "DISPENSER_IDLE");

    save_dispenser_slice_ran(5);
    load_dispenser_slice_ran(&dispensed_slices);
    printf("dispensed slices: %d\n", dispensed_slices);

    return 0;
}
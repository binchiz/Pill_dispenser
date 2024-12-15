#include "buttons.h"
#include "dispenser.h"
#include "led.h"
#include "lora.h"
#include "pico/stdlib.h"
#include "pill_dispenser_sm.h"
#include "storage.h"
#include "lib/debug.h"
#include <stdio.h>

int main(void) {
    timer_hw->dbgpause = 0;
    stdio_init_all();
    set_debug_level(DEBUG_LEVEL_DEBUG);
    init_led();
    init_storage();
    init_dispenser();
    init_buttons();
    lora_init_and_connect();

    dispenser_sm dsm = {stStart};
    send_message(BOOT, "Dispenser Boot");
    dprintf(DEBUG_LEVEL_DEBUG, "Enter state machine\n");
    while (true) {
        run_dispenser_sm(&dsm);
        sleep_ms(500);
    }
    return 0;
}
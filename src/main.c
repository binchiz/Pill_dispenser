#include <stdio.h>
#include "pico/stdlib.h"
#include "utils/debug.h"
#include "drivers/led.h"
#include "buttons.h"
#include "dispenser_control.h"
#include "lora.h"
#include "storage.h"
#include "pill_dispenser_sm.h"


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
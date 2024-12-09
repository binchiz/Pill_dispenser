#include "buttons.h"
#include "dispenser_control.h"
#include "lora.h"
#include "pico/stdlib.h"
#include "storage.h"
#include "utils/debug.h"
#include <stdio.h>

int main(void) {
    stdio_init_all();
    set_debug_level(DEBUG_LEVEL_DEBUG);

    init_storage();
    init_dispenser();
    init_buttons();
    lora_init_and_connect();

    printf("Hello from main!\n");
    return 0;
}
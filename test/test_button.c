#include "buttons.h"
#include "pico/stdlib.h"
#include "utils/debug.h"
#include <stdio.h>

int main(void) {
    stdio_init_all();
    set_debug_level(DEBUG_LEVEL_DEBUG);

    printf("test_user_event\n");

    init_buttons();
    enable_buttons();

    uint32_t time = 0;
    bool enabled = true;

    while (true) {
        event_t event = get_button_event();
        if (event != EVENT_NONE) {
            printf("button event: %d\n", event);
        }

        if (to_ms_since_boot(get_absolute_time()) - time > 3000) {
            if (enabled) {
                printf("Disabling buttons\n");
                disable_buttons();
                enabled = false;
            } else {
                printf("Enabling buttons\n");
                enable_buttons();
                enabled = true;
            }
            time = to_ms_since_boot(get_absolute_time());
        }
    }

    return 0;
}
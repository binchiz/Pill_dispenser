#include "pico/stdlib.h"
#include "lib/debug.h"

int main(void) {
    stdio_init_all();
    set_debug_level(DEBUG_LEVEL_DEBUG);
    dprintf(DEBUG_LEVEL_DEBUG, "Test msg: debug level: debug\n");
    dprintf(DEBUG_LEVEL_INFO, "Test msg: debug level: info\n");
    dprintf(DEBUG_LEVEL_WARN, "Test msg: debug level: warn\n");
    dprintf(DEBUG_LEVEL_ERROR, "Test msg: debug level: error\n");
    return 0;
}
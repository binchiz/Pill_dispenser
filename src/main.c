#include <stdio.h>
#include "pico/stdlib.h"

int main(void) {
    stdio_init_all();
    printf("Hello from main!\n");
    return 0;
}
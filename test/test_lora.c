#include "pico/stdlib.h"
#include "lora.h"

 int main(void) {
     stdio_init_all();
     lora_init_and_connect();
     send_message(BOOT, "sheng,bc,zzy!");
     return 0;
 }

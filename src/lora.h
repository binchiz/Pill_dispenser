#ifndef lora_H
#define lora_H
#include "hardware/uart.h"

#define ENABLE_LORA 1

enum event_codes {
    BOOT = 100,
    PILL_DISPENSED = 200,
    CNOT_DISPENSED = 300,
    DISPENSER_EMPTY = 400,
    POWER_OFF_DURING_TURNING = 500,
    HEARTBEAT = 999
};



void lora_init_and_connect();

int send_message(int event_code, char *message);
#endif //lora_H

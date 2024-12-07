#include "hardware/uart.h"

enum event_codes {
    BOOT = 100,
    PILL_DISPENSED = 200,
    CNOT_DISPENSED = 300,
    DISPENSER_EMPTY = 400,
    POWER_OFF_DURING_TURNING = 500,
    HEARTBEAT = 999
};

typedef struct {
    int baud_rate;
    int tx_pin;
    int rx_pin;
    int uart_nr;
    int event_code;
    char *message;
    char *app_key;
    uint times_limit;
    uint timeout_us;
} lora_t;

void lora_init_and_connect();
int send_message(int event_code, char *message);

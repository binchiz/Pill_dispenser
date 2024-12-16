#include "lib/debug.h"
#include "lib/eeprom.h"
#include "pico/stdlib.h"
#include "storage.h"
#include <stdio.h>

#define MEM_DISPENSER_STATE 0x7FFF
#define MEM_DISPENSER_STATE_CHECKSUM 0x7FFE
#define MEM_DISPENSED_SLICES 0x7FFD
#define MEM_DISPENSED_SLICES_CHECKSUM 0x7FFC
#define MEM_DISPENSER_CALIBRATED 0x7FFB
#define MEM_DISPENSER_CALIBRATED_CHECKSUM 0x7FFA

static eeprom_t eeprom = {
    .i2c = NULL,
    .baudrate = (400 * 1000),
    .gpio_sda = 16,
    .gpio_scl = 17,
    .device_addr = 0b1010000,
    .page_size = 64,
};

int main(void) {
    init_eeprom(&eeprom, i2c0);
    eeprom_write(&eeprom, (uint8_t *)"#", MEM_DISPENSER_STATE, 1);
    eeprom_write(&eeprom, (uint8_t *)"#", MEM_DISPENSER_STATE_CHECKSUM, 1);
    eeprom_write(&eeprom, (uint8_t *)"#", MEM_DISPENSED_SLICES, 1);
    eeprom_write(&eeprom, (uint8_t *)"#", MEM_DISPENSED_SLICES_CHECKSUM, 1);
    eeprom_write(&eeprom, (uint8_t *)"#", MEM_DISPENSER_CALIBRATED, 1);
    eeprom_write(&eeprom, (uint8_t *)"#", MEM_DISPENSER_CALIBRATED_CHECKSUM, 1);
    return 0;
}
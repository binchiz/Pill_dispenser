#include "storage.h"
#include "drivers/eeprom.h"
#include "utils/debug.h"

// EEPROM memory map
#define MEM_DISPENSER_STATE 0x7FFF
#define MEM_DISPENSER_STATE_CHECKSUM 0x7FFE
#define MEM_DISPENSED_SLICES 0x7FFD
#define MEM_DISPENSED_SLICES_CHECKSUM 0x7FFC

#define EEPROM_WRITE_TIMEOUT 1000 // ms

static eeprom_t eeprom = {
    .i2c = NULL,
    .baudrate = (400 * 1000),
    .gpio_sda = 16,
    .gpio_scl = 17,
    .device_addr = 0b1010000,
    .page_size = 64,
};

void init_storage() { init_eeprom(&eeprom, i2c0); }

bool save_dispenser_state(dispenser_state_t state) {
    dprintf(DEBUG_LEVEL_DEBUG, "Saving dispenser state: %d\n", state);
    return write_byte_with_checksum(&eeprom, MEM_DISPENSER_STATE, MEM_DISPENSER_STATE_CHECKSUM, (uint8_t)state);
}

bool load_dispenser_state(dispenser_state_t *state) {
    uint8_t data;
    dprintf(DEBUG_LEVEL_DEBUG, "Loading dispenser state\n");
    if (read_byte_with_checksum(&eeprom, MEM_DISPENSER_STATE, MEM_DISPENSER_STATE_CHECKSUM, &data)) {
        *state = (dispenser_state_t)data;
        dprintf(DEBUG_LEVEL_DEBUG, "Loading dispenser state success: %d\n", *state);
        return true;
    } else {
        dprintf(DEBUG_LEVEL_WARN, "Loading dispenser state failed\n");
        return false;
    }
}

bool save_dispenser_slice_ran(int slices_ran) {
    dprintf(DEBUG_LEVEL_DEBUG, "Saving dispensed slices: %d\n", slices_ran);
    return write_byte_with_checksum(&eeprom, MEM_DISPENSED_SLICES, MEM_DISPENSED_SLICES_CHECKSUM, (uint8_t)slices_ran);
}

bool load_dispenser_slice_ran(int *slices_ran) {
    uint8_t data;
    dprintf(DEBUG_LEVEL_DEBUG, "Loading dispensed slices\n");
    if (read_byte_with_checksum(&eeprom, MEM_DISPENSED_SLICES, MEM_DISPENSED_SLICES_CHECKSUM, &data)) {
        *slices_ran = (int)data;
        dprintf(DEBUG_LEVEL_DEBUG, "Loading dispensed slices success: %d\n", *slices_ran);
        return true;
    } else {
        dprintf(DEBUG_LEVEL_WARN, "Loading dispensed slices failed\n");
        return false;
    }
}
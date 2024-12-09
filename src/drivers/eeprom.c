#include "eeprom.h"
#include "pico/stdlib.h"
#include "utils/debug.h"
#include <string.h>

#define EEPROM_WRITE_TIMEOUT 1000 // ms

void init_eeprom(eeprom_t *eeprom, i2c_inst_t *i2c) {
    i2c_init(i2c, eeprom->baudrate);
    eeprom->i2c = i2c;
    gpio_set_function(eeprom->gpio_sda, GPIO_FUNC_I2C);
    gpio_set_function(eeprom->gpio_scl, GPIO_FUNC_I2C);
    gpio_pull_up(eeprom->gpio_sda);
    gpio_pull_up(eeprom->gpio_scl);
}

int eeprom_write(const eeprom_t *eeprom, const uint8_t *data, uint16_t mem_addr, uint8_t len) {
    uint8_t buf[eeprom->page_size + 2]; // additional 2 bytes for memory address
    int ret;
    buf[0] = (uint8_t)(mem_addr >> 8);
    buf[1] = (uint8_t)mem_addr;
    if (len > eeprom->page_size) {
        len = eeprom->page_size;
    }
    memcpy(&buf[2], data, len);
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    ret = i2c_write_blocking(eeprom->i2c, eeprom->device_addr, buf, len + 2, false);
    if (ret == len + 2) {
        ret = len;
    } // the number of actual data bytes transmitted

    while (i2c_write_blocking(eeprom->i2c, eeprom->device_addr, 0x00, 1, false) == PICO_ERROR_GENERIC) {
        // polling from the EEPROM to check if the writing is finished:
        // ACK signal is received when the EEPROM is ready to accept the next command, i.e. the writing is finished
        // while PICO_ERROR_GENERIC means the ACK signal is not received, i.e. the EEPROM is busy
        if (to_ms_since_boot(get_absolute_time()) - start_time > EEPROM_WRITE_TIMEOUT) {
            dprintf(DEBUG_LEVEL_WARN, "EEPROM write timeout\n");
            return PICO_ERROR_TIMEOUT;
        }
    }
    return ret;
}

int eeprom_read(const eeprom_t *eeprom, uint8_t *buffer, uint16_t mem_addr, uint8_t len) {
    uint8_t mem_addr_8[2] = {(uint8_t)(mem_addr >> 8), (uint8_t)mem_addr};
    int ret;
    i2c_write_blocking(eeprom->i2c, eeprom->device_addr, mem_addr_8, 2, true);
    if (len > eeprom->page_size) {
        len = eeprom->page_size;
    }
    ret = i2c_read_blocking(eeprom->i2c, eeprom->device_addr, buffer, len, false);
    return ret;
}

bool write_byte_with_checksum(const eeprom_t *eeprom, uint16_t mem_addr, uint16_t checksum_addr, uint8_t data) {
    uint8_t checksum = (uint8_t)~data;
    int ret_data = eeprom_write(eeprom, &data, mem_addr, 1);
    int ret_checksum = eeprom_write(eeprom, &checksum, checksum_addr, 1);
    return ret_data == 1 && ret_checksum == 1;
}

bool read_byte_with_checksum(const eeprom_t *eeprom, uint16_t mem_addr, uint16_t checksum_addr, uint8_t *data) {
    uint8_t checksum;
    int ret_data = eeprom_read(eeprom, data, mem_addr, 1);
    int ret_checksum = eeprom_read(eeprom, &checksum, checksum_addr, 1);
    bool is_checksum_correct = checksum == (uint8_t)~(*data);
    if (is_checksum_correct) {
        dprintf(DEBUG_LEVEL_DEBUG, "Checksum is correct\n");
    } else {
        dprintf(DEBUG_LEVEL_DEBUG, "Checksum is incorrect\n");
    }
    return ret_data == 1 && ret_checksum == 1 && is_checksum_correct;
}

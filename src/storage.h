#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>

typedef enum { DISPENSER_IDLE, DISPENSER_TURNING } dispenser_state_t;

void init_storage();
bool save_dispenser_state(dispenser_state_t state);
bool load_dispenser_state(dispenser_state_t *state);
bool save_dispenser_slice_ran(int slices_ran);
bool load_dispenser_slice_ran(int *slices_ran);
bool save_dispenser_calibrated(bool calibrated);
bool load_dispenser_calibrated(bool *calibrated);

#endif // STORAGE_H

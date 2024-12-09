#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>

typedef enum { DISPENSER_TURNING, DISPENSER_IDLE } dispenser_state_t;

void init_storage();
bool save_dispenser_state(dispenser_state_t state);
bool load_dispenser_state(dispenser_state_t *state);
bool save_dispenser_slice_ran(int slices_ran);
bool load_dispenser_slice_ran(int *slices_ran);

#endif // STORAGE_H

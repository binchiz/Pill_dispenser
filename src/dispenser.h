
#ifndef dispenser_CONTROL_H
#define dispenser_CONTROL_H
#include <stdbool.h>

void init_dispenser();
void restore_dispenser_slices_ran(int slices_ran);
void align_dispenser(int rev);
bool dispense_pill();
void dispense_all_pills();
void error_calibration();

#endif

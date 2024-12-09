
#ifndef dispenser_CONTROL_H
#define dispenser_CONTROL_H
#include <stdbool.h>

void init_dispenser();
void restore_dispenser();
void align_dispenser(int rev);
bool dispense_pill();
void dispense_all_pills();
void run_n_slice(int n);
void error_calibration();

#endif

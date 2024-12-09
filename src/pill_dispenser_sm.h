#ifndef PILL_DISPENSER_SM_H
#define PILL_DISPENSER_SM_H

typedef enum {
    stStart,
    stCalibWait,
    stError,
    stCalib,
    stDispenseWait,
    stDispense
} dispenser_st;

typedef struct dispenser_sm {
    dispenser_st state;
} dispenser_sm;

void run_dispenser_sm (dispenser_sm *dispenser_sm_ptr);

#endif //PILL_DISPENSER_SM_H

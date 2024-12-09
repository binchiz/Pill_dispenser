#include "dispenser_control.h"
#include "lora.h"
#include "storage.h"
#include "buttons.h"
#include "drivers/led.h"
#include "pill_dispenser_sm.h"

static dispenser_state_t data;
static int slices_ran;


void run_dispenser_sm (dispenser_sm *dispenser_sm_ptr) {
    switch (dispenser_sm_ptr->state) {
        case stStart:
            load_dispenser_slice_ran(&slices_ran);
            restore_dispenser_slices_ran(slices_ran);
            load_dispenser_state(&data);
            if (data == DISPENSER_TURNING) {
                //send_message(POWER_OFF_DURING_TURNING, "Powered Off During Turn");
                dispenser_sm_ptr->state = stError;
            }
            else if (slices_ran == 0) dispenser_sm_ptr -> state = stCalibWait;
            else dispenser_sm_ptr->state = stDispense;
            break;
        case stError:
            error_calibration();
            dispenser_sm_ptr->state = stDispense;
            break;
        case stCalibWait:
            toggle_led();
            if (get_button_event() != EVENT_NONE) dispenser_sm_ptr->state = stCalib;
            break;
        case stCalib:
            align_dispenser(1);
            dispenser_sm_ptr->state = stDispenseWait;
            break;
        case stDispenseWait:
            set_led(true);
            if (get_button_event() != EVENT_NONE) dispenser_sm_ptr->state = stDispense;
            break;
        case stDispense:
            dispense_all_pills();
            dispenser_sm_ptr->state = stStart;
            break;
    }
}
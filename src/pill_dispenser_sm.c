#include "dispenser_control.h"
#include "lora.h"
#include "storage.h"
#include "buttons.h"
#include "drivers/led.h"
#include "pill_dispenser_sm.h"
#include "utils/debug.h"

static dispenser_state_t data;
static int slices_ran;


void run_dispenser_sm (dispenser_sm *dispenser_sm_ptr) {
    switch (dispenser_sm_ptr->state) {
        case stStart:
            load_dispenser_slice_ran(&slices_ran);
            restore_dispenser_slices_ran(slices_ran);
            load_dispenser_state(&data);
            if (slices_ran == 0) {
                dispenser_sm_ptr -> state = stCalibWait;
                enable_buttons();
                dprintf(DEBUG_LEVEL_INFO, "Wait calib\n");
            }
            else if (data == DISPENSER_TURNING) {
                send_message(POWER_OFF_DURING_TURNING, "Powered Off During Turn");
                dispenser_sm_ptr->state = stError;
                dprintf(DEBUG_LEVEL_INFO, "Re-calib from err\n");
            }
            else {
                dispenser_sm_ptr->state = stDispense;
                dprintf(DEBUG_LEVEL_INFO, "Ready\n");
            }
            break;
        case stError:
            error_calibration();
            dispenser_sm_ptr->state = stDispense;
            dprintf(DEBUG_LEVEL_INFO, "Error resolved, ready\n");
            break;
        case stCalibWait:
            toggle_led();
            if (get_button_event() != EVENT_NONE) {
                dispenser_sm_ptr->state = stCalib;
                dprintf(DEBUG_LEVEL_INFO, "Calib start\n");
                disable_buttons();
            }
            break;
        case stCalib:
            align_dispenser(1);
            dispenser_sm_ptr->state = stDispenseWait;
            dprintf(DEBUG_LEVEL_INFO, "Calib done, ready\n");
            enable_buttons();
            break;
        case stDispenseWait:
            set_led(true);
            if (get_button_event() != EVENT_NONE) {
                dispenser_sm_ptr->state = stDispense;
                dprintf(DEBUG_LEVEL_INFO, "Dispense start\n");
                disable_buttons();
            }
            break;
        case stDispense:
            dispense_all_pills();
            dispenser_sm_ptr->state = stStart;
            dprintf(DEBUG_LEVEL_INFO, "Dispense done\n");
            break;
    }
}
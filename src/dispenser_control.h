#ifndef DISPENSER_CONTROL_H
#define DISPENSER_CONTROL_H

#include <stdbool.h>
#include <stddef.h>

/**
 * \brief Initialize the dispenser by calibrating the stepper motor and stop at the initial position
 *
 */
void init_dispenser();

/**
 * \brief Align the dispenser with the drop tube by rotating the stepper motor.
 * \Note The direction will be the opposite of the dispense direction, so it won't dispense pills that remain.
 */
void align_dispenser();

/**
 * \brief Dispense a pill by rotating the wheel
 * \return Whether a pill is dispensed, by detecting piezo sensor
 */
bool dispense_pill();

#endif // DISPENSER_CONTROL_H

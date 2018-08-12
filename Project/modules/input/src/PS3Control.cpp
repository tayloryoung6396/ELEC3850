/*
 * This should handle the pairing and take the ps3 controller inputs
 * This mimics the autonomous controller.
 * Inputs:
 * Outputs: Distance to move/path
 *          Position to grip
 */
/*
 * Possible ideas: Auto position gripper button
 */

#include "PS3Control.hpp"

void PS3Control_init() {
    std::cout << "Initilising PS3 CONTROLLER" << std::endl;
}

int PS3Control_main() {
    // Check the controller is connected
    // Check the inputs
    // Store the values in the respective variables

    // TODO Not sure how to handle this
    // I would probably take the xy coordinates of the joystick, and convert it into polar coordinates
    // from that it can be fed into the motorcontroller.
    // However this means rotation on the spot doesn't work.. so not sure how to handle that input
    // Goodluck..
    return 0;
}
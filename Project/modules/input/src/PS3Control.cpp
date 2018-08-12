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
    // I would probably take the xy coordinates of the joystick, and interpret it as foward and rotation
    // The up down diraction would be forward backward
    // The left right the rotation
    // from that it can be fed into the motorcontroller.
    // Goodluck..
    return 0;
}
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

#define FORWARD_SPEED 1   // TODO Change
#define ROTATION_SPEED 1  // TODO Change

void PS3Control_init() {
    std::cout << "Initilising PS3 CONTROLLER" << std::endl;
}

int PS3Control_main() {
    // Check the controller is connected
    // if (connected) {
    //     check the 'mode' button
    // }
    // If we are in remote mode
    // if (mode_ps3) {


    // Check the inputs
    // Store the values in the respective variables

    // TODO Not sure how to handle this
    // I would probably take the xy coordinates of the joystick, and interpret it as foward and rotation
    // The up down diraction would be forward backward
    // The left right the rotation
    // from that it can be fed into the motorcontroller.
    // Goodluck..

    double L_joystick_x;  // Up positive, Down negative
    double L_joystick_y;  // Left positive, Right negative

    PathPlanner pplanner;

    double Forward  = FORWARD_SPEED * L_joystick_x;
    double Rotation = ROTATION_SPEED * L_joystick_y;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    // }

    return 0;
}
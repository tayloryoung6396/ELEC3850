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
#include <iostream>

#define FORWARD_SPEED 1   // TODO Change
#define ROTATION_SPEED 1  // TODO Change

void PS3Control_init() {
    std::cout << "Initilising PS3 CONTROLLER" << std::endl;
}

int PS3Control_main() {
    // Check the controller is connected
    // if (connected) {
//    checkconnection()
//    while (ps3_connected=1)
//{

//        checkconnection()

  //  }
    }
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
/*
    double L_joystick_x;  // Up positive, Down negative
    double L_joystick_y;  // Left positive, Right negative

    PathPlanner pplanner;

    double Forward  = FORWARD_SPEED * L_joystick_x;
    double Rotation = ROTATION_SPEED * L_joystick_y;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    // }

    Gripper::Goal[0] = 0;
    Gripper::Goal[1] = 0;
    Gripper::Goal[2] = 0;

return 0;
}
*/
// Initilise
// Connect to the controller

// Running

// Step 1
// Check that the controller is connected
// If it's not then don't worry about the rest of this
// Probably some safety feature to stop it driving away
// If we were in ps3 mode and we have disconnected, we might try to reconnect
// If that fails stop the vehicle

// Step 2
// Read in all of the buttons
// bytes_read = read(ps3_handle, data, sizeof(data))
// if we read in all of the bytes expected then good

// Step 3
// Check to see if we have switched from autonomous mode to ps3 or vise versa
// If we are in ps3 mode, then look at all of the other buttons
// Perform some action/tell someone else that a button has been pressed

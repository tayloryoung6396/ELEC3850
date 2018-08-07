/*
 * This should handle the autonomous control of the robot.
 * From the sensor inputs and classified objects decide how to handle everything.
 * Using the localisation and path planner
 * Inputs:  Sensor data
 *          Vector to objects of interest
 * Outputs: Distance to move/path
 *          Position to grip
 */

#include "AutonomousControl.hpp"

void AutonomousControl_init() {}

int AutonomousControl_main() {
    // There should be a state machine here,
    // States:
    // Searching
    // Collecting
    // Rescuing

    // These will go into the states
    // Figure out if there's stuff around me
    Localisation_main();
    // Decide how to move
    SimplePathPlanner_main();
    return 0;
}
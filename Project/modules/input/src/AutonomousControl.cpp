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

void AutonomousControl_init() {
    std::cout << "Initilising AUTONOMOUS CONTROLLER" << std::endl;
}

int AutonomousControl_main() {
    // TODO
    // There should be a state machine here,
    // States:
    // Searching
    // Collecting
    // Rescuing

    // These will go into the states
    // Figure out if there's stuff around me
    Localisation_main();
    // Decide how to move
    // TODO make this a variable in the autonomous controller
    double wGoal[2] = {0};
    SimplePathPlanner(wGoal);
    return 0;
}
/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.h"

void main() {

    // Initialise all modules
    HardwareIO_init();
    Kinematics_init();
    SensorFilter_init();
    MotorController_init();
    IKGripper_init();
    Localisation_init();
    Camera_init();
    IRCamera_init();
    Classifier_init();
    SimplePathPlanner_init();

    // Loop forever to run robot
    // Execute all code here
    while (1) {
        // For each iteration
        // Check camera
        // Check IR
        // Check Ultrasonic
        // Move
        //
    }
}
/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.hpp"

int main() {

    // Initialise all modules
    HardwareIO_init();
    PS3Control_init();
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
    HardwareIO_main();
    PS3Control_main();
    Kinematics_main();
    SensorFilter_main();
    MotorController_main();
    IKGripper_main();
    Localisation_main();
    Camera_main();
    IRCamera_main();
    Classifier_main();
    SimplePathPlanner_main();
    // while (1) {
    //     // For each iteration
    //     // Check camera
    //     // Check IR
    //     // Check Ultrasonic
    //     // Move
    //     //
    // }
}
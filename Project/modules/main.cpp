/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.hpp"

int main() {

    // Initialise all modules
    HardwareIO_init();
    PS3Control_init();
    AutonomousControl_init();
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
    // while (1) {
    // For each iteration
    // Check sensors
    Camera_main();
    IRCamera_main();
    Classifier_main();

    // If we are in remote mode
    if (1) {
        PS3Control_main();
    }
    else {
        // We must be in autonomous mode
        // But we still need to check the remote, if it's connected, for a mode change command
        AutonomousControl_main();
    }
    // Based on the above inputs come up with the motor commands
    MotorController_main();
    // Based on the above inputs come up with the gripper commands
    double Goal[3] = {2, 5, 4};
    IKGripper_main(Goal);
    printf("base_pitch %lf \nelbow_pitch %lf \nwrist_pitch %lf\n",
           Gripper_angles::base_pitch,
           Gripper_angles::elbow_pitch,
           Gripper_angles::wrist_pitch);
    HardwareIO_main();
    // }
}
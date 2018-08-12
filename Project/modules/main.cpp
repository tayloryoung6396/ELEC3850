/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.hpp"

int main() {

    // Initialise all modules
    printf("Starting Initilisation\n");
    // wiringPiSetupSys();
    UART();
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
    printf("Finished Initilisation\n");

    // Loop forever to run robot
    // Execute all code here

    while (1) {
        // For each iteration
        // Check sensors
        Camera_main();
        IRCamera_main();
        Classifier_main();

        // If we are in remote mode
        // if (1) {
        //     PS3Control_main();
        // }
        // else {
        // We must be in autonomous mode
        // But we still need to check the remote, if it's connected, for a mode change command
        AutonomousControl_main();
        // }

        // TODO Check that just spamming this doesn't affect the path
        // If spamming does cause a bad path, check to see if we're at our goal before sending the next one
        // Based on the above inputs come up with the motor commands
        MotorController();
        // Based on the above inputs come up with the gripper commands
        double Goal[3] = {0.2, 0, 0.1};  // TODO remove
        IKGripper_main(Goal);
        printf("Goal %g, %g, %g\n", Goal[0], Goal[1], Goal[2]);
        printf("base_yaw %g \nbase_pitch %g \nelbow_pitch %g \nwrist_pitch %g\n",
               Gripper_angles::base_yaw * 180 / M_PI,
               Gripper_angles::base_pitch * 180 / M_PI,
               Gripper_angles::elbow_pitch * 180 / M_PI,
               Gripper_angles::wrist_pitch * 180 / M_PI);

        break;
    }
}
/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.hpp"

int main() {

    // Initialise all modules
    printf("Starting Initilisation\n");
    wiringPiSetupSys();
    UART();
    PS3Control_init();
    AutonomousControl_init();
    Kinematics_init();
    SensorFilter_init();
    MotorController_init();
    FKGripper_init();
    IKGripper_init();
    Localisation_init();
    Camera_init();
    InfraredSensor_init();
    UltrasonicSensor_init();
    Classifier_init();
    SimplePathPlanner_init();
    printf("Finished Initilisation\n");

    // Loop forever to run robot
    // Execute all code here
    while (1) {
        std::cout << "time -> " << (double) millis() << std::endl;

        // For each iteration
        // Check sensors
        Camera_main();
        InfraredSensor_main();
        Classifier_main();

        // The motor director overseas the current status of the drive motors
        // It's job is to check whether the drive motors have reached the goal position
        // Track the number of revolutions performed
        // Update the localisation model about where we currently are.
        // If need be, the motor director can perform the final positioning itself, independent of the loop
        MotorDirector();

        // Check if we are connected, if we are then check the mode
        // If we are in ps3 control mode then don't run the autonomous controller
        PS3Control_main();
        // TODO This should be some mode variable
        if (1) {
            // We must be in autonomous mode
            // But we still need to check the remote, if it's connected, for a mode change command
            AutonomousControl_main();
        }

        // Neither of these directly control the driving or the gripper.
        // The driving is controlled by a vector of movement commands
        // The gripper is controlled by seeting some goal position with velocity
        // Based on the above inputs come up with the motor commands
        MotorController();
        // Based on the above inputs come up with the gripper commands
        double Goal[3] = {0.2, 0, 0.1};  // TODO remove
        IKGripper_main(Goal);            // TODO output goal from autonomous/PS3
        break;
    }
}

/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.hpp"

bool Input::Autonomous_Enabled FALSE;

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received" << std::endl;

    // cleanup and close up stuff here
    std::cout << "Velocity reset" << std::endl;
    uint8_t count       = 2;
    uint8_t servo_ID[2] = {Motor_L, Motor_R};
    for (int i = 0; i < count; i++) {
        executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), 0);
        delay(20);
    }
    std::cout << "Disabling torque" << std::endl;
    uint8_t data;
    for (int servo_ID = 1; servo_ID < 8; servo_ID++) {
        // TODO This should probably be a ping, but i dont think i have a function to handle it
        if (executeReadSingle(servo_ID, MX64_ADDRESS_VALUE(ID), MX64_SIZE_VALUE(ID), data) == 0) {
            delay(20);
            executeWriteSingle(servo_ID, MX64_ADDRESS_VALUE(TORQUE_ENABLE), 0);
            delay(20);
        }
        else {
            std::cout << "ERROR: Failed to ping servo " << (int) servo_ID << std::endl;
        }
    }

    // TODO Remove probably
    std::cout << "Printing occupancy map" << std::endl;
    Print_Occupancy_Map();

    // terminate program

    exit(signum);
}

int main() {

    signal(SIGINT, signalHandler);

    // Initialise all modules
    printf("Starting Initilisation\n");

    // System Init
    wiringPiSetup();
    UART();

    // Odometry Init
    Localisation_init();
    SimplePathPlanner_init();

    // Controll Init
    PS3Control_init();
    AutonomousControl_init();
    Kinematics_init();

    // Servo Init
    Dynamixel_init();
    MotorController_init();
    FKGripper_init();
    IKGripper_init();

    // Sensors Init
    // Camera_init();
    InfraredSensor_init();
    UltrasonicSensor_init();
    SensorFilter_init();
    Classifier_init();

    printf("Finished Initilisation\n");

    static int frame_count = 0;      // This is a crude method for slowing some tasks
    const int frame_max    = 60000;  // LCM of all frame divisors

    double previous_time = (double) millis();
    double current_time  = (double) millis();

    bool temp = TRUE;

    while (1) {
        current_time = (double) millis();
        // std::cout << "FPS -> " << (double) (1000.0 / (current_time - previous_time)) << std::endl;
        previous_time = current_time;

        // For each iteration
        // Check sensors
        // if (frame_count % 5 == 0) {
        // Camera_main();
        // InfraredSensor_main();
        // Classifier_main();
        //}

        if (frame_count % 60000 == 0) {
            if (PathPlanner::moving_flag[0] == 0 && PathPlanner::moving_flag[1] == 0) {
                std::cout << "Localisation::w_Tank_Position[0] " << Localisation::w_Tank_Position[0] << std::endl;
                std::cout << "Localisation::w_Tank_Position[1] " << Localisation::w_Tank_Position[1] << std::endl;
                std::cout << "Localisation::w_Tank_Rotation " << Localisation::w_Tank_Rotation << std::endl;
            }
        }

        UltrasonicSensor_main();

        if (frame_count % 3 == 0) {
            // If we are moving
            if (PathPlanner::moving_flag[0] != 0 || PathPlanner::moving_flag[1] != 0) {
                MotorDirector();
            }
        }

        if (frame_count % 5 == 0) {
            // TODO We probably want to store the last 5 US readings
            Localisation_main();  // TODO Maybe this should happen all of the time?
        }

        // Check if we are connected, if we are then check the mode
        // If we are in ps3 control mode then don't run the autonomous controller
        PS3Control_main();

        if (Input::Autonomous_Enabled && temp) {
            // We must be in autonomous mode
            // But we still need to check the remote, if it's connected, for a mode change command
            AutonomousControl_main();

            // The motor director overseas the current status of the drive motors
            // It's job is to check whether the drive motors have reached the goal position
            // Track the number of revolutions performed
            // Update the localisation model about where we currently are.
            // If need be, the motor director can perform the final positioning itself, independent of the loop

            // TODO Remove
            temp = FALSE;
        }
        //      break;
        frame_count++;
        if (frame_count % frame_max == 0) {
            frame_count = 0;  // Reset frame count
        }
    }
}

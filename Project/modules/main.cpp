/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.hpp"

bool Input::Autonomous_Enabled FALSE;

std::ofstream myfile;  // TODO Remove

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received" << std::endl;

    Light_Sequence();

    // cleanup and close up stuff here
    std::cout << "Velocity reset" << std::endl;
    uint8_t count       = 2;
    uint8_t servo_ID[2] = {Motor_L, Motor_R};
    for (int i = 0; i < count; i++) {
        executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), 0);
        delay(DELAY_TIME);
    }
    std::cout << "Disabling torque" << std::endl;
    uint8_t data;
    for (int servo_ID = 1; servo_ID < 8; servo_ID++) {
        // NOTE This should probably be a ping, but i dont think i have a function to handle it
        if (executeReadSingle(servo_ID, MX64_ADDRESS_VALUE(ID), MX64_SIZE_VALUE(ID), data) == 0) {
            delay(DELAY_TIME);
            executeWriteSingle(servo_ID, MX64_ADDRESS_VALUE(TORQUE_ENABLE), 0);
            delay(DELAY_TIME);
        }
        else {
            std::cout << "ERROR: Failed to ping servo " << (int) servo_ID << std::endl;
        }
    }

    // TODO Remove probably
    std::cout << "Printing occupancy map" << std::endl;
    Print_Occupancy_Map();

    myfile.close();  // TODO Remove

    // terminate program

    exit(signum);
}

int main() {

    signal(SIGINT, signalHandler);

    myfile.open("Sample_time.txt");  // TODO Remove

    // Initialise all modules
    myfile << "Starting Initilisation .\n";
    printf("Starting Initilisation\n");

    // System Init
    wiringPiSetup();
    LedStatus_init();
    UART();

    // Odometry Init
    Localisation_init();
    SimplePathPlanner_init();

    // Control Init
    PS3Control_init();
    AutonomousControl_init();
    Kinematics_init();

    // Servo Init
    Dynamixel_init();
    MotorController_init();
    FKGripper_init();
    IKGripper_init();

    // Sensors Init
    Camera_init();
    InfraredSensor_init();
    UltrasonicSensor_init();
    SensorFilter_init();
    Classifier_init();

    printf("Finished Initilisation\n");
    myfile << "Finished Initilisation.\n";

    static int frame_count = 0;      // This is a crude method for slowing some tasks
    const int frame_max    = 60000;  // LCM of all frame divisors

    double previous_time = (double) millis();
    double current_time  = (double) millis();

    double prev_time = 0;  // TODO Remove

    while (1) {
        current_time = (double) millis();
        // std::cout << "FPS -> " << (double) (1000.0 / (current_time - previous_time)) << std::endl;
        // myfile << "Current time " << current_time << ",\tFPS " << 1000.0 / (current_time - previous_time) << "\n"; //
        // TODO Remove
        previous_time = current_time;

        // Check sensors
        // TODO This will be within the autonomous enable section
        if (frame_count % 5 == 0) {
            // Camera_main();
            // Classifier_main();
            // if(AutoState::object_classified){
            // InfraredSensor_main();
            // // returns IRsense::IRpresent = TRUE if ir object found
            // }
        }

        // prev_time = (double) millis();  // TODO Remove
        // UltrasonicSensor_main();
        // myfile << "UltrasonicSensor_main() -> end time " << (double) millis() - prev_time << "\n";  // TODO Remove

        if (frame_count % 10 == 0) {
            // If we are moving
            if (PathPlanner::moving_flag[0] != 0 || PathPlanner::moving_flag[1] != 0) {
                // The motor director overseas the current status of the drive motors
                // It's job is to check whether the drive motors have reached the goal position
                // Track the number of revolutions performed
                // Update the localisation model about where we currently are.
                // If need be, the motor director can perform the final positioning itself, independent of the loop

                // prev_time = (double) millis();  // TODO Remove
                MotorDirector();
                // myfile << "MotorDirector() -> end time " << (double) millis() - prev_time << "\n";  // TODO Remove
            }
        }

        // TODO This will be within the autonomous enable section
        if (frame_count % 50 == 0) {
            // TODO We probably want to store the last 5 US readings
            // prev_time = (double) millis();  // TODO Remove
            UltrasonicSensor_main();
            Localisation_main();
            // myfile << "Localisation_main() -> end time " << (double) millis() - prev_time << "\n";  // TODO Remove
        }

        // Check if we are connected, if we are then check the mode
        // If we are in ps3 control mode then don't run the autonomous controller
        // prev_time = (double) millis();  // TODO Remove
        PS3Control_main();
        // myfile << "PS3Control_main() -> end time " << (double) millis() - prev_time << "\n";  // TODO Remove
        if (!Input::Autonomous_Enabled) {
            // Force the system to update less often
            if (frame_count % 10 == 0) {
                if (PS3Walk::drive_flag | PathPlanner::moving_flag[0] != 0 | PathPlanner::moving_flag[1] != 0) {
                    MotorDriver_Velocity(PS3Walk::Forward, PS3Walk::Rotation);
                }
            }
            if (frame_count % 10 == 0) {
                if (PS3Walk::Gripper_flag) {
                    if (IKGripper_move(PS3Walk::Gripper_Goal) != 0) {
                        std::cout << "ERROR: Could not move gripper" << std::endl;
                    }
                    PS3Walk::Gripper_flag = FALSE;
                }
            }
            if (!PS3Walk::drive_flag) {
                MotorDriver_Velocity(0, 0);
            }
        }
        if (Input::Autonomous_Enabled) {
            // We must be in autonomous mode
            // But we still need to check the remote, if it's connected, for a mode change command
            // prev_time = (double) millis();  // TODO Remove
            AutonomousControl_main();
            // myfile << "AutonomousControl_main() -> end time " << (double) millis() - prev_time << "\n";  // TODO
            // Remove
        }
        //	break;
        frame_count++;
        if (frame_count % frame_max == 0) {
            frame_count = 0;  // Reset frame count
        }
    }
}

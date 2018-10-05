/*
 * This should loop indefinetly.
 * Call all high level functions from here
 */

#include "main.hpp"

bool Input::Autonomous_Enabled FALSE;

int main() {

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

    static int frame_count = 0;   // This is a crude method for slowing some tasks
    const int frame_max    = 15;  // LCM of all frame divisors

    double previous_time = (double) millis();
    double current_time  = (double) millis();

    while (1) {
        current_time = (double) millis();
        std::cout << "FPS -> " << (double) (1000.0 / (current_time - previous_time)) << std::endl;
        previous_time = current_time;

        // For each iteration
        // Check sensors
        if (frame_count % 5 == 0) {
            // Camera_main();
            // InfraredSensor_main();
            // Classifier_main();
        }

        UltrasonicSensor_main();

        if (frame_count % 5 == 0) {
            // TODO We probably want to store the last 5 US readings
            Localisation_main();  // TODO Maybe this should happen all of the time?
        }

        // Check if we are connected, if we are then check the mode
        // If we are in ps3 control mode then don't run the autonomous controller
        PS3Control_main();

        if (Input::Autonomous_Enabled) {
            // We must be in autonomous mode
            // But we still need to check the remote, if it's connected, for a mode change command
            AutonomousControl_main();

            // The motor director overseas the current status of the drive motors
            // It's job is to check whether the drive motors have reached the goal position
            // Track the number of revolutions performed
            // Update the localisation model about where we currently are.
            // If need be, the motor director can perform the final positioning itself, independent of the loop
            if (frame_count % 3 == 0) {
                MotorDirector();
            }

            Input::Autonomous_Enabled = !Input::Autonomous_Enabled;  // TODO remove
        }
        break;
        frame_count++;
        if (frame_count % frame_max == 0) {
            frame_count = 0;  // Reset frame count
        }
    }
    double Goal_pos1[3] = {0, 0, 0};
    IK_Calculate(Goal_pos1[3]);
    std::cout << "x " << Goal_pos1[0] << " y " << Goal_pos1[1] << " z " << Goal_pos1[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos2[3] = {0.2, 0, 0};
    IK_Calculate(Goal_pos2[3]);
    std::cout << "x " << Goal_pos2[0] << " y " << Goal_pos2[1] << " z " << Goal_pos2[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos3[3] = {0, 0.2, 0};
    IK_Calculate(Goal_pos3[3]);
    std::cout << "x " << Goal_pos3[0] << " y " << Goal_pos3[1] << " z " << Goal_pos3[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos4[3] = {0, 0, 0.2};
    IK_Calculate(Goal_pos4[3]);
    std::cout << "x " << Goal_pos4[0] << " y " << Goal_pos4[1] << " z " << Goal_pos4[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos5[3] = {0.2, 0.2, 0};
    IK_Calculate(Goal_pos5[3]);
    std::cout << "x " << Goal_pos5[0] << " y " << Goal_pos5[1] << " z " << Goal_pos5[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos6[3] = {0.2, 0, 0.2};
    IK_Calculate(Goal_pos6[3]);
    std::cout << "x " << Goal_pos6[0] << " y " << Goal_pos6[1] << " z " << Goal_pos6[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos7[3] = {0, 0.2, 0.2};
    IK_Calculate(Goal_pos7[3]);
    std::cout << "x " << Goal_pos7[0] << " y " << Goal_pos7[1] << " z " << Goal_pos7[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;

    double Goal_pos8[3] = {-0.2, 0, 0};
    IK_Calculate(Goal_pos8[3]);
    std::cout << "x " << Goal_pos8[0] << " y " << Goal_pos8[1] << " z " << Goal_pos8[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos9[3] = {0, -0.2, 0};
    IK_Calculate(Goal_pos9[3]);
    std::cout << "x " << Goal_pos9[0] << " y " << Goal_pos9[1] << " z " << Goal_pos9[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos10[3] = {0, 0, -0.2};
    IK_Calculate(Goal_pos10[3]);
    std::cout << "x " << Goal_pos10[0] << " y " << Goal_pos10[1] << " z " << Goal_pos10[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos11[3] = {-0.2, 0.2, 0};
    IK_Calculate(Goal_pos11[3]);
    std::cout << "x " << Goal_pos11[0] << " y " << Goal_pos11[1] << " z " << Goal_pos11[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos12[3] = {-0.2, 0, 0.2};
    IK_Calculate(Goal_pos12[3]);
    std::cout << "x " << Goal_pos12[0] << " y " << Goal_pos12[1] << " z " << Goal_pos12[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos13[3] = {0, -0.2, 0.2};
    IK_Calculate(Goal_pos13[3]);
    std::cout << "x " << Goal_pos13[0] << " y " << Goal_pos13[1] << " z " << Goal_pos13[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos14[3] = {0.2, 0.2, 0.2};
    IK_Calculate(Goal_pos14[3]);
    std::cout << "x " << Goal_pos14[0] << " y " << Goal_pos14[1] << " z " << Goal_pos14[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;

    double Goal_pos15[3] = {0.2, -0.2, 0};
    IK_Calculate(Goal_pos15[3]);
    std::cout << "x " << Goal_pos15[0] << " y " << Goal_pos15[1] << " z " << Goal_pos15[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos16[3] = {0.2, 0, -0.2};
    IK_Calculate(Goal_pos16[3]);
    std::cout << "x " << Goal_pos16[0] << " y " << Goal_pos16[1] << " z " << Goal_pos16[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos17[3] = {0, 0.2, -0.2};
    IK_Calculate(Goal_pos17[3]);
    std::cout << "x " << Goal_pos17[0] << " y " << Goal_pos17[1] << " z " << Goal_pos17[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;

    double Goal_pos18[3] = {-0.2, -0.2, 0};
    IK_Calculate(Goal_pos18[3]);
    std::cout << "x " << Goal_pos18[0] << " y " << Goal_pos18[1] << " z " << Goal_pos18[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos19[3] = {-0.2, 0, -0.2};
    IK_Calculate(Goal_pos19[3]);
    std::cout << "x " << Goal_pos19[0] << " y " << Goal_pos19[1] << " z " << Goal_pos19[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos20[3] = {0, -0.2, -0.2};
    IK_Calculate(Goal_pos20[3]);
    std::cout << "x " << Goal_pos20[0] << " y " << Goal_pos20[1] << " z " << Goal_pos20[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
    double Goal_pos21[3] = {-0.2, -0.2, -0.2};
    IK_Calculate(Goal_pos21[3]);
    std::cout << "x " << Goal_pos21[0] << " y " << Goal_pos21[1] << " z " << Goal_pos21[2] << std::endl;
    std::cout << "base_yaw " << Gripper_angles::base_yaw * 180 / M_PI << " base_pitch "
              << Gripper_angles::base_pitch * 180 / M_PI << " elbow_pitch " << Gripper_angles::elbow_pitch * 180 / M_PI
              << " wrist_pitch " << Gripper_angles::wrist_pitch * 180 / M_PI << std::endl;
}

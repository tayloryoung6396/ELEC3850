/*
 * This should calculate servo positions required to place the gripper at a desired place.
 * Inputs:  Gripper position request as vec3d
 * Outputs: Servo positions as angles
 */

/*
 * Inverse Kinematics - Given a position in some coordinate system, what do the angles of my servos need to be?
 * This section of the model ignores 3 of the 6 servos
 *
 * Servos:
 * Base_Yaw = 1
 * Base_Pitch = 2
 * Elbow_Pitch = 3
 * Elbow_Roll = 4
 * Wrist_Pitch = 5
 * Gripper = 6
 *
 * Kinematics.base_pos[3] - vec3 xyz position of the base servo from the origin
 * Kinematics.arm_len_1 - Length from pivot to pivot of first part
 * Kinematics.arm_len_2 - Length from pivot to pivot of next part
 * Kinematics.grip_cen - Distance between last servo and the center of the gripper grip
 *
 * Figure out some length parameter
 * arm_len_3 = sqrt((Goal_pos[0] - Kinematics.base_pos[0] - Kinematics.grip_cen),2 +
                    (Goal_pos[1] - Kinematics.base_pos[1]),2 +
                    (Goal_pos[2] - Kinematics.base_pos[2]),2)
 * theta_elbow_pitch "C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
 * theta_wrist_pitch
 * theta_base_pitch
 * base_elevation = cos(sqrt((Goal_pos[0] - Kinematics.base_pos[0] - Kinematics.grip_cen),2 +
                             (Goal_pos[1] - Kinematics.base_pos[1]),2)
 * wrist_declination = 90 - base_elevation
 *
 *
 * base_pitch = 90 - base_elevation - theta_base_pitch
 * elbow_pitch = 180 - theta_wrist_pitch
 * wrist_pitch = -90 + base_pitch + elbow_pitch
 *
 * This should all result in the gripper staying parallel to the ground and able to move to any xyz coordinate in reach
 */

#include "IKGripper.hpp"
#include "../../platform/src/DynamixelProtocol.hpp"
#include "../../platform/src/Kinematics.hpp"

double Gripper_angles::base_pitch  = 0;
double Gripper_angles::base_yaw    = 0;
double Gripper_angles::elbow_pitch = 0;
double Gripper_angles::wrist_pitch = 0;
double Gripper_angles::grip        = 0;

void IKGripper_init() {}

int IKGripper_main(double Goal_pos[3]) {
    if (Open_Gripper() != 0) {
        printf("Error could not open Gripper\n");
        return -1;
    }

    else if (IK_Calculate(Goal_pos) != 0) {
        printf("Error could not calculate Gripper IK\n");
        return -1;
    }
    uint8_t count           = 4;
    uint8_t servo_ID[count] = {Base_Yaw, Base_Pitch, Elbow_Pitch, Wrist_Pitch};
    auto address            = MX28_GOAL_POSITION;
    double data[count]      = {
        Gripper_angles::base_yaw, Gripper_angles::base_pitch, Gripper_angles::elbow_pitch, Gripper_angles::wrist_pitch};
    printf("Outside2\nBase_Yaw %lf, %lf\nBase_Pitch %lf, %lf\nElbow_Pitch %lf, %lf\nWrist_Pitch %lf, %lf\n",
           data[0],
           Gripper_angles::base_yaw,
           data[1],
           Gripper_angles::base_pitch,
           data[2],
           Gripper_angles::elbow_pitch,
           data[3],
           Gripper_angles::wrist_pitch);
    executeWriteMulti(servo_ID, address, data, count);
    if (Grip_Object() != 0) {
        printf("Error could not Grip Object\n");
        return -1;
    }
    return 0;
}

int IK_Calculate(double Goal_pos[3]) {
    double theta_elbow_pitch = 0;
    double theta_wrist_pitch = 0;
    double theta_base_pitch  = 0;

    Goal_pos[0] -= Kinematics::base_pos[0];
    Goal_pos[1] -= Kinematics::base_pos[1];
    Goal_pos[2] -= Kinematics::base_pos[2];

    Gripper_angles servo;

    // Calculate our base yaw rotation
    servo.base_yaw = std::atan2(Goal_pos[1], Goal_pos[0]);

    // Now create a plane on this rotation and the z axis
    // Find our new horizontal goal point (To the wrist servo, ignore the gripper)
    double rGoal_xy = std::sqrt(std::pow((Goal_pos[0]), 2) + std::pow((Goal_pos[1]), 2)) - Kinematics::grip_cen;

    // Calculate the straight line distance to the wrist servo
    double arm_len_3 = std::sqrt(std::pow(rGoal_xy, 2) + std::pow(Goal_pos[2], 2));

    // Our arm needs to be fully extended
    if (arm_len_3 > Kinematics::arm_len_1 + Kinematics::arm_len_2 + DELTA_GRIP) {
        theta_elbow_pitch = 0;
        theta_wrist_pitch = 0;
        theta_base_pitch  = 0;
    }
    else {
        theta_elbow_pitch = SSS_triangle(Kinematics::arm_len_1, Kinematics::arm_len_2, arm_len_3);
        theta_wrist_pitch = SSS_triangle(Kinematics::arm_len_2, arm_len_3, Kinematics::arm_len_1);
        theta_base_pitch  = SSS_triangle(Kinematics::arm_len_1, arm_len_3, Kinematics::arm_len_2);
    }

    servo.base_pitch  = std::acos(rGoal_xy / arm_len_3) - theta_base_pitch;
    servo.elbow_pitch = M_PI - theta_elbow_pitch;
    servo.wrist_pitch = servo.base_pitch + servo.elbow_pitch - M_PI / 2;
    printf("Base_Yaw %lf\nBase_Pitch %lf\nElbow_Pitch %lf\nWrist_Pitch %lf\n",
           servo.base_yaw,
           servo.base_pitch,
           servo.elbow_pitch,
           servo.wrist_pitch);
    return 0;
}

double SSS_triangle(double a, double b, double c) {
    //"C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
    return std::acos((std::pow(a, 2) + std::pow(b, 2) - std::pow(c, 2)) / (2 * a * b));
}

int Grip_Object() {
    // Grip until the gripper has some defined load
    bool Gripped     = false;
    uint8_t servo_ID = Gripper;
    auto address     = MX28_GOAL_POSITION;
    uint32_t data    = 0;
    while (!Gripped) {
        Gripper_angles::grip++;
        // Check if the object is in gripper
        if (executeReadSingle(servo_ID, address, &data) >= Kinematics::grip_load) {
            Gripped = true;
        }
        else if (Gripper_angles::grip >= Kinematics::grip_closed) {
            printf("Error: Gripper failed to grab object\n");
            return -1;
        }
    }
    return 0;
}

int Open_Gripper() {
    uint8_t servo_ID = Gripper;
    auto address     = MX28_GOAL_POSITION;
    auto data        = Kinematics::grip_open;
    executeWriteSingle(servo_ID, address, data);
    return 0;
}

int Close_Gripper() {
    uint8_t servo_ID = Gripper;
    auto address     = MX28_GOAL_POSITION;
    auto data        = Kinematics::grip_closed;
    executeWriteSingle(servo_ID, address, data);
    return 0;
}
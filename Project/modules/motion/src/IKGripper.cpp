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
#include "../../platform/src/Kinematics.hpp"

double Gripper_angles::base_pitch  = 0;
double Gripper_angles::base_yaw    = 0;
double Gripper_angles::elbow_pitch = 0;
double Gripper_angles::elbow_roll  = 0;
double Gripper_angles::wrist_pitch = 0;
double Gripper_angles::grip        = 0;

void IKGripper_init() {}

int IKGripper_main(double Goal_pos[3]) {
    if (IK_Calculate(Goal_pos) != 0) {
        printf("Error could not calculate Gripper IK\n");
        return -1;
    }
    return 0;
}

// TODO This has a lot of holes that re unaccounted for..
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
    double rGoal_xy = std::sqrt(pow((Goal_pos[0]), 2) + pow((Goal_pos[1]), 2)) - Kinematics::grip_cen;

    // Calculate the straight line distance to the wrist servo
    double arm_len_3 = std::sqrt(pow(rGoal_xy, 2) + pow(Goal_pos[2], 2));

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
}

double SSS_triangle(double a, double b, double c) {
    //"C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
    return std::acos((std::pow(a, 2) + std::pow(b, 2) - std::pow(c, 2)) / (2 * a * b));
}

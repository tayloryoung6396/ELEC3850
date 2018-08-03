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

void IKGripper_init() {}

int IKGripper_main() {

    double Goal_pos[3] = {0, 0, 20};

    Kinematics model;

    // Figure out some length parameter
    double arm_len_3 = sqrt(pow((Goal_pos[0] - model.base_pos[0] - model.grip_cen), 2)
                            + pow((Goal_pos[1] - model.base_pos[1]), 2) + pow((Goal_pos[2] - model.base_pos[2]), 2));

    double theta_elbow_pitch = SSS_triangle(model.arm_len_1, model.arm_len_2, arm_len_3);
    double theta_wrist_pitch = SSS_triangle(model.arm_len_2, arm_len_3, model.arm_len_1);
    double theta_base_pitch  = SSS_triangle(model.arm_len_1, arm_len_3, model.arm_len_2);

    double base_elevation = std::cos(std::sqrt(pow((Goal_pos[0] - model.base_pos[0] - model.grip_cen), 2)
                                               + pow((Goal_pos[1] - model.base_pos[1]), 2))
                                     * M_PI / 180);

    double wrist_declination = 90 - base_elevation;
    double base_pitch        = 90 - base_elevation - theta_base_pitch * 180 / M_PI;
    double elbow_pitch       = 180 - theta_wrist_pitch * 180 / M_PI;
    double wrist_pitch       = (-90) + base_pitch + elbow_pitch;

    printf("theta_elbow_pitch %lf \ntheta_wrist_pitch %lf \ntheta_base_pitch %lf\n",
           180 * theta_elbow_pitch / 3.14,
           180 * theta_wrist_pitch / 3.14,
           180 * theta_base_pitch / 3.14);
    printf("base_pitch %lf \nelbow_pitch %lf \nwrist_pitch %lf\n", base_pitch, elbow_pitch, wrist_pitch);

    return 0;
}

// TODO Fix nan
double SSS_triangle(double a, double b, double c) {
    //"C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
    return std::acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
}
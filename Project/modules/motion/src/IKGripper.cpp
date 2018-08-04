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

int IK_Calculate(double Goal_pos[3]) {

    double theta_elbow_pitch = 0;
    double theta_wrist_pitch = 0;
    double theta_base_pitch  = 0;
    double base_elevation    = 0;
    double wrist_declination = 0;

    // Kinematics model;

    printf("%lf\n", Kinematics::grip_cen);

    Gripper_angles servo;

    // Figure out some length parameter
    double arm_len_3 =
        sqrt(pow((Goal_pos[0] - Kinematics::base_pos[0] - Kinematics::grip_cen), 2)
             + pow((Goal_pos[1] - Kinematics::base_pos[1]), 2) + pow((Goal_pos[2] - Kinematics::base_pos[2]), 2));

    printf("Arm Length 3 %lf\n", arm_len_3);


    if (arm_len_3 >= (Kinematics::arm_len_1 + Kinematics::arm_len_2)) {
        servo.base_pitch  = 0;
        servo.elbow_pitch = 0;
        servo.wrist_pitch = 0;

        // TODO what if the goal position is too far for the arm?
        printf("Arm is vertical or goal position is too far\n");
        printf("Arm lengths a1 %lf, a2 %lf, a3 %lf\n", arm_len_3, Kinematics::arm_len_1, Kinematics::arm_len_2);
    }
    else {
        theta_elbow_pitch = SSS_triangle(Kinematics::arm_len_1, Kinematics::arm_len_2, arm_len_3);
        theta_wrist_pitch = SSS_triangle(Kinematics::arm_len_2, arm_len_3, Kinematics::arm_len_1);
        theta_base_pitch  = SSS_triangle(Kinematics::arm_len_1, arm_len_3, Kinematics::arm_len_2);

        base_elevation = std::cos(std::sqrt(pow((Goal_pos[0] - Kinematics::base_pos[0] - Kinematics::grip_cen), 2)
                                            + pow((Goal_pos[1] - Kinematics::base_pos[1]), 2))
                                  * M_PI / 180);

        wrist_declination = 90 - base_elevation;
        servo.base_pitch  = 90 - base_elevation - theta_base_pitch * 180 / M_PI;
        servo.elbow_pitch = 180 - theta_wrist_pitch * 180 / M_PI;
        servo.wrist_pitch = (-90) + servo.base_pitch + servo.elbow_pitch;

        printf("theta_elbow_pitch %lf \ntheta_wrist_pitch %lf \ntheta_base_pitch %lf\n",
               180 * theta_elbow_pitch / 3.14,
               180 * theta_wrist_pitch / 3.14,
               180 * theta_base_pitch / 3.14);
    }

    if ((isnan(servo.base_pitch) == 1) | (isnan(servo.elbow_pitch) == 1) | (isnan(servo.wrist_pitch) == 1)) {
        servo.base_pitch  = 0;
        servo.elbow_pitch = 0;
        servo.wrist_pitch = 0;
        return -1;
    }

    printf(
        "base_pitch %lf \nelbow_pitch %lf \nwrist_pitch %lf\n", servo.base_pitch, servo.elbow_pitch, servo.wrist_pitch);

    return 0;
}

// TODO Fix nan
double SSS_triangle(double a, double b, double c) {
    //"C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
    return std::acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
}
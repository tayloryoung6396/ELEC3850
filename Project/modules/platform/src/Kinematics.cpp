/*
 * This should set the kinematics of the platform from some origin point
 * Outputs: Platform parameters
 */

#include "Kinematics.hpp"

// Gripper specific parameters
// TODO Actually measure these
double Kinematics::base_pos[3] = {0};
double Kinematics::arm_len_1   = 0.2;
double Kinematics::arm_len_2   = 0.2;
double Kinematics::grip_cen    = 0.1;
double Kinematics::grip_closed = M_PI / 4.0;
double Kinematics::grip_open   = -M_PI / 4.0;
double Kinematics::grip_load   = 0;

double Kinematics::grip_home[3] = {0.11, 0, 0.3};

// Tank specific parameters
// TODO Actually measure these
double Kinematics::origin_offset[3] = {0};  // Mid point of each axis
double Kinematics::tank_length      = 0.355;
double Kinematics::tank_width       = 0.180;
double Kinematics::tank_height      = 0.150;

double Kinematics::Servo_max_limit[5] = {M_PI, M_PI / 2.0, M_PI / 2.0, M_PI / 2.0, M_PI / 4.0};
double Kinematics::Servo_min_limit[5] = {-M_PI, -M_PI / 2.0, -M_PI / 2.0, -M_PI / 2.0, -M_PI / 4.0};
double Kinematics::Servo_offset[5]    = {0, 0, 0, 0, 0};

double Kinematics::ultrasonic_offset[4] = {Kinematics::tank_width / 2.0,
                                           Kinematics::tank_length / 2.0,
                                           Kinematics::tank_width / 2.0,
                                           Kinematics::tank_length / 2.0};  // R, B, L, F

void Kinematics_init() {
    std::cout << "Initilising KINEMATICS" << std::endl;
}

int Kinematics_main() {
    return 0;
}

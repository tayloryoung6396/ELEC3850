/*
 * This should set the kinematics of the platform from some origin point
 * Outputs: Platform parameters
 */

#include "Kinematics.hpp"

// Gripper specific parameters
// TODO Actually measure these
double Kinematics::base_pos[3] = {0};
double Kinematics::arm_len_1   = 0.19;
double Kinematics::arm_len_2   = 0.208;
double Kinematics::grip_cen    = 0.1;
double Kinematics::grip_closed = M_PI / 4.0;
double Kinematics::grip_open   = -M_PI / 4.0;
double Kinematics::grip_load   = std::numeric_limits<uint16_t>::max() / 4.0;

double Kinematics::grip_home[3]    = {0, 0, 0.2};
double Kinematics::Rest_Gripper[3] = {0, 0, 0.05};

// Tank specific parameters
// TODO Actually measure these
double Kinematics::origin_offset[3] = {0.05, 0, 0.05};  // Mid point of each axis
double Kinematics::tank_length      = 0.355;
double Kinematics::tank_width       = 0.180;
double Kinematics::tank_height      = 0.150;

double Kinematics::Servo_max_limit[5] = {M_PI, M_PI / 2.0, M_PI / 2.0, M_PI / 2.0, M_PI / 4.0};
double Kinematics::Servo_min_limit[5] = {-M_PI, -M_PI / 2.0, -M_PI / 2.0, -M_PI / 2.0, -M_PI / 4.0};
double Kinematics::Servo_offset[5]    = {0, 0, 0, 0, 0};

double Kinematics::ultrasonic_offset[4] = {Kinematics::tank_length / 2.0,
                                           Kinematics::tank_width / 2.0,
                                           Kinematics::tank_length / 2.0,
                                           Kinematics::tank_width / 2.0};  // F, R, B, L

const double Kinematics::cam_phi    = 15 * M_PI / 180.0;
const double Kinematics::cam_pos[3] = {Kinematics::tank_length / 2.0, 0, -0.05};
const double Kinematics::cam_height = 0.05;

void Kinematics_init() {
    std::cout << "Initilising KINEMATICS" << std::endl;
}

int Kinematics_main() {
    return 0;
}

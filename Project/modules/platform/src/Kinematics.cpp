/*
 * This should set the kinematics of the platform from some origin point
 * Outputs: Platform parameters
 */

#include "Kinematics.hpp"

double Kinematics::base_pos[3] = {0};
double Kinematics::arm_len_1   = 0.2;
double Kinematics::arm_len_2   = 0.2;
double Kinematics::grip_cen    = 0.1;
double Kinematics::grip_closed = M_PI / 2;
double Kinematics::grip_open   = 0;
double Kinematics::grip_load   = 0;

void Kinematics_init() {}

int Kinematics_main() {
    return 0;
}
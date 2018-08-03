/*
 * This should set the kinematics of the platform from some origin point
 * Outputs: Platform parameters
 */

#include "Kinematics.hpp"

void Kinematics_init() {

    // Populate kinematic model
    Kinematics model;

    model.base_pos[3] = {0};
    model.grip_cen    = 2;
    model.arm_len_1   = 10;
    model.arm_len_2   = 10;
}

int Kinematics_main() {
    return 0;
}
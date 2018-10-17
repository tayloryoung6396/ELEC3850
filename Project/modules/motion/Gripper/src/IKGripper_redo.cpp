#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

double SSS_triangle(double a, double b, double c) {
    // std::cout << "SSS_triangle function" << std::endl;
    //"C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
    // std::cout << "a b c " << a << " " << b << " " << c << std::endl;
    return std::acos((std::pow(a, 2) + std::pow(b, 2) - std::pow(c, 2)) / (2.0 * a * b));
}

int IK_Calculate(double Goal_pos[]) {

    double arm_len_1 = 0.2;
    double arm_len_2 = 0.2;

    double theta_elbow_pitch = 0;
    double theta_wrist_pitch = 0;
    double theta_base_pitch  = 0;

    Goal_pos[0] -= 0;  // Kinematics::base_pos[0];  // Some origin offset, currently 0
    Goal_pos[1] -= 0;  // Kinematics::base_pos[1];  // Some origin offset, currently 0
    Goal_pos[2] -= 0;  // Kinematics::base_pos[2];  // Some origin offset, currently 0

    // Gripper_angles servo;

    // Check to see if our z position is in the ground..
    // TODO These should probably be thrown out in the validation
    if (Goal_pos[2] < -0.1) {
        std::cout << "Goal Pos " << Goal_pos[0] << ", " << Goal_pos[1] << ", " << Goal_pos[2] << std::endl;
        std::cout << "Arm in gorund" << std::endl;
        return -1;
    }

    double rGoal_xy =
        std::sqrt(std::pow(Goal_pos[0], 2) + std::pow(Goal_pos[1], 2));  // - 0.1;  // Kinematics::grip_cen;

    double arm_len_3 = std::sqrt(std::pow(rGoal_xy, 2) + std::pow(Goal_pos[2], 2));

    double alpha = 0;

    // Our arm needs to be fully extended
    if (arm_len_3 > arm_len_1 + arm_len_2) {
        std::cout << "Goal Pos " << Goal_pos[0] << ", " << Goal_pos[1] << ", " << Goal_pos[2] << std::endl;
        std::cout << "Length too long" << std::endl;
        theta_base_pitch  = 0;
        theta_elbow_pitch = 0;
        theta_wrist_pitch = 0;

        // just Calculate the triangle straight from this hypotinuse
        // kind of like this
        alpha = std::acos(rGoal_xy / arm_len_3);
        if (isnan(alpha)) {
            alpha = 0;
        }
        // return -1;
    }
    // TODO These should probably be thrown out in the validation
    else if (arm_len_3 < std::sqrt(std::pow(arm_len_1, 2) + std::pow(arm_len_2, 2))) {
        std::cout << "Goal Pos " << Goal_pos[0] << ", " << Goal_pos[1] << ", " << Goal_pos[2] << std::endl;
        std::cout << "Length too short" << std::endl;
        return -1;
    }
    else {
        theta_base_pitch  = SSS_triangle(arm_len_1, arm_len_3, arm_len_2);
        theta_elbow_pitch = SSS_triangle(arm_len_1, arm_len_2, arm_len_3);
        theta_wrist_pitch = SSS_triangle(arm_len_2, arm_len_3, arm_len_1);

        alpha = std::acos(rGoal_xy / arm_len_3);
        if (isnan(alpha)) {
            alpha = 0;
        }
    }

    double G_base_pitch  = 0;
    double G_elbow_pitch = 0;
    double G_wrist_pitch = 0;

    if (Goal_pos[2] > 0) {
        G_base_pitch  = M_PI_2 - theta_base_pitch - alpha;
        G_elbow_pitch = M_PI - theta_elbow_pitch;
        G_wrist_pitch = -M_PI_2 + theta_base_pitch + theta_elbow_pitch - alpha;
    }
    else {
        G_base_pitch  = M_PI_2 - theta_base_pitch + alpha;
        G_elbow_pitch = M_PI - theta_elbow_pitch;
        G_wrist_pitch = -M_PI + theta_base_pitch + theta_elbow_pitch - alpha;
    }


    std::cout << "Goal Pos " << Goal_pos[0] << ", " << Goal_pos[1] << ", " << Goal_pos[2] << std::endl;
    std::cout << "alpha " << alpha * 180 / M_PI << std::endl;
    std::cout << "Theta bp, ep, wp " << theta_base_pitch * 180 / M_PI << ", " << theta_elbow_pitch * 180 / M_PI << ", "
              << theta_wrist_pitch * 180 / M_PI << std::endl;
    std::cout << "Phi bp, ep, wp " << G_base_pitch * 180 / M_PI << ", " << G_elbow_pitch * 180 / M_PI << ", "
              << G_wrist_pitch * 180 / M_PI << std::endl;

    return 0;
}

int main() {

    double Goal_pos1[3] = {0.15, 0, 0};
    IK_Calculate(Goal_pos1);

    double Goal_pos2[3] = {0.2, 0, 0};
    IK_Calculate(Goal_pos2);

    double Goal_pos3[3] = {0.25, 0, 0};
    IK_Calculate(Goal_pos3);

    double Goal_pos4[3] = {0.3, 0, 0};
    IK_Calculate(Goal_pos4);

    double Goal_pos5[3] = {0.35, 0, 0};
    IK_Calculate(Goal_pos5);


    double Goal_pos6[3] = {0.3, 0, -0.2};
    IK_Calculate(Goal_pos6);

    double Goal_pos7[3] = {0.3, 0, -0.1};
    IK_Calculate(Goal_pos7);

    double Goal_pos21[3] = {0.3, 0, -0.02};
    IK_Calculate(Goal_pos21);

    double Goal_pos8[3] = {0.3, 0, 0};
    IK_Calculate(Goal_pos8);

    double Goal_pos22[3] = {0.3, 0, 0.02};
    IK_Calculate(Goal_pos22);

    double Goal_pos9[3] = {0.3, 0, 0.1};
    IK_Calculate(Goal_pos9);

    double Goal_pos10[3] = {0.3, 0, 0.2};
    IK_Calculate(Goal_pos10);


    double Goal_pos11[3] = {0.15, 0, -0.2};
    IK_Calculate(Goal_pos11);

    double Goal_pos12[3] = {0.2, 0, -0.2};
    IK_Calculate(Goal_pos12);

    double Goal_pos13[3] = {0.25, 0, -0.2};
    IK_Calculate(Goal_pos13);

    double Goal_pos14[3] = {0.3, 0, -0.2};
    IK_Calculate(Goal_pos14);

    double Goal_pos115[3] = {0.35, 0, -0.2};
    IK_Calculate(Goal_pos115);


    double Goal_pos16[3] = {0.15, 0, 0.2};
    IK_Calculate(Goal_pos16);

    double Goal_pos17[3] = {0.2, 0, 0.2};
    IK_Calculate(Goal_pos17);

    double Goal_pos18[3] = {0.25, 0, 0.2};
    IK_Calculate(Goal_pos18);

    double Goal_pos19[3] = {0.3, 0, 0.2};
    IK_Calculate(Goal_pos19);

    double Goal_pos120[3] = {0.35, 0, 0.2};
    IK_Calculate(Goal_pos120);
}
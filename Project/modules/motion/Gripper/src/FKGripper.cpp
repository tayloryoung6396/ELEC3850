#include "FKGripper.hpp"

void FKGripper_init() {
    std::cout << "Initilising FK GRIPPER" << std::endl;
}

int FKGripper_main() {
    return 0;
}


// TODO Fix this once IK confirmed
void FK_Calculate(double base_yaw, double base_pitch, double elbow_pitch, double wrist_pitch, double* Pres_pos) {

    // Angle sum
    double theta_elbow_pitch = M_PI - elbow_pitch;
    // Cosine rule
    double arm_len_3 = SAS_triangle(Kinematics::arm_len_1, Kinematics::arm_len_2, theta_elbow_pitch);
    // Sine rule
    double theta_base_pitch = std::asin(Kinematics::arm_len_2 * std::sin(theta_elbow_pitch) / arm_len_3);
    // Angle sum
    double alpha = M_PI_2 - theta_base_pitch - base_pitch;
    // rGoal_xy to wrist pitch
    double rGoal_xy = std::cos(alpha) * arm_len_3;
    // wrist height
    double wrist_z = std::sin(alpha) * arm_len_3;
    // Find the true angle of the wrist
    double wrist_theta = -M_PI_2 + base_pitch + elbow_pitch - wrist_pitch;  // TODO check this?
    // Calculate horizontal offset to wrist center
    rGoal_xy += std::cos(wrist_theta) * DELTA_GRIP;
    // Calculate vertical offset to wrist center
    wrist_z += std::sin(wrist_theta) * DELTA_GRIP;

    // Now we have our 2D rotated frame, lets
    Pres_pos[0] = rGoal_xy * std::cos(base_yaw);
    Pres_pos[1] = rGoal_xy * std::sin(base_yaw);
    Pres_pos[2] = wrist_z;
}

double SAS_triangle(double b, double c, double angle_A) {
    double a = std::sqrt(std::pow(b, 2) + std::pow(c, 2) - 2 * b * c * std::cos(angle_A));
    return a;
}

#include "FKGripper.hpp"

void FKGripper_init() {
    std::cout << "Initilising FK GRIPPER" << std::endl;
}

int FKGripper_main() {
    return 0;
}

// TODO This is incomplete. it only calculates the position of the wrist pitch servo
double* FK_Calculate(double base_yaw, double base_pitch, double elbow_pitch, double wrist_pitch) {

    double elevation_base_pitch = 90 - base_pitch;
    double theta_elbow_pitch    = 180 - elbow_pitch;
    double arm_len_3            = SAS_triangle(Kinematics::arm_len_1, Kinematics::arm_len_2, theta_elbow_pitch);
    double theta_base_pitch     = SSS_triangle(Kinematics::arm_len_1, arm_len_3, Kinematics::arm_len_2);
    double Pres_pos[3]          = {0};
    Pres_pos[2]                 = std::sin(elevation_base_pitch - theta_base_pitch) * arm_len_3;
    double arm_len_xy           = std::cos(elevation_base_pitch - theta_base_pitch) * arm_len_3;
    Pres_pos[1]                 = std::sin(base_yaw) * arm_len_xy;
    Pres_pos[0]                 = std::cos(base_yaw) * arm_len_xy;

    return Pres_pos;
}

double SAS_triangle(double b, double c, double angle_A) {
    double a = std::sqrt(std::pow(b, 2) + std::pow(c, 2) - 2 * b * c * std::cos(angle_A));
    return a;
}
#ifndef FK_GRIPPER_HPP_
#define FK_GRIPPER_HPP_

#include "main.hpp"

extern void FKGripper_init();
extern int FKGripper_main();

extern double* FK_Calculate(double base_yaw, double base_pitch, double elbow_pitch, double wrist_pitch);
extern double SAS_triangle(double b, double c, double angle_A);

#endif /* FK_GRIPPER_HPP_ */
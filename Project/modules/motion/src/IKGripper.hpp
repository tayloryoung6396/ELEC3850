#ifndef IK_GRIPPER_HPP_
#define IK_GRIPPER_HPP_

#include "../../main.hpp"

#define DELTA_GRIP 0.03

extern void IKGripper_init();
extern int IKGripper_main(double Goal_pos[3]);
extern int IK_Calculate(double Goal_pos[3]);
extern double SSS_triangle(double a, double b, double c);
extern int Grip_Object();
extern int Open_Gripper();
extern int Close_Gripper();

class Gripper_angles {
public:
    static double base_pitch;
    static double base_yaw;
    static double elbow_pitch;
    static double wrist_pitch;
    static double grip;
};

#endif /* IK_GRIPPER_HPP_ */
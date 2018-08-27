#ifndef IK_GRIPPER_HPP_
#define IK_GRIPPER_HPP_

#include "main.hpp"

#define DELTA_GRIP 0.03

extern void IKGripper_init();
extern int Gripper_home();
extern int IKGripper_main(double Goal_pos[3]);
extern int IK_Calculate(double Goal_pos[3]);
extern double SSS_triangle(double a, double b, double c);
extern int Grip_Object();
extern int Open_Gripper();
extern int Close_Gripper();
extern uint32_t convert_rad_pos(uint8_t servo_ID, double angle);
extern double convert_pos_rad(uint8_t servo_ID, uint32_t angle);
extern int Validate_Pos(double Goal_pos[3]);

class Gripper_angles {
public:
    static double base_pitch;
    static double base_yaw;
    static double elbow_pitch;
    static double wrist_pitch;
    static double grip;
};

#endif /* IK_GRIPPER_HPP_ */
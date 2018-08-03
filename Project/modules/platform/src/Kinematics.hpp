#ifndef KINEMATICS_H_
#define KINEMATICS_H_

#include "../../main.hpp"

extern void Kinematics_init();
extern int Kinematics_main();

enum Servos { Base_Yaw = 1, Base_Pitch = 2, Elbow_Pitch = 3, Elbow_Roll = 4, Wrist_Pitch = 5, Gripper = 6 };

class Kinematics {
public:
    double base_pos[3];
    double grip_cen;
    double arm_len_1;
    double arm_len_2;
};


#endif /* KINEMATICS_H_ */
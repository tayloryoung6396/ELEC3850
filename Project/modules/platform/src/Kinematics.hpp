#ifndef KINEMATICS_H_
#define KINEMATICS_H_

#include "../../main.hpp"

extern void Kinematics_init();
extern int Kinematics_main();

enum Servos { Base_Yaw = 1, Base_Pitch = 2, Elbow_Pitch = 3, Elbow_Roll = 4, Wrist_Pitch = 5, Gripper = 6 };

class Kinematics {
public:
    // Gripper specific parameters
    static double base_pos[3];
    static double arm_len_1;
    static double arm_len_2;
    static double grip_cen;
    static double grip_closed;
    static double grip_open;
    static double grip_load;

    // Tank specific parameters
    static double origin_offset[3];
    static double tank_length;
    static double tank_width;
    static double tank_height;
};


#endif /* KINEMATICS_H_ */
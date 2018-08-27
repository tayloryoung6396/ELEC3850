#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

#include "main.hpp"

#define DistToRev 1  // Meters per rotation

class PathPlanner {

public:
    static std::vector<std::pair<double, double>> path_vec;

    void emplace_path(double Forward, double Rotation);
    bool check_path();
    std::vector<std::pair<double, double>>::const_iterator get_first_path();
    void path_erase_first();

    static uint32_t goal_pos[2];
    static uint32_t prev_pos[2];
    static uint32_t curr_pos[2];
    static uint8_t curr_revolution[2];
    static int moving_flag[2];
};

extern int MotorController();
extern void MotorController_init();
extern int MotorDriver(double Forward, double Rotation);
extern int MotorDirector();
extern double ConvertDistanceToRotation(double Goal_Dist);
extern double ConvertRotationToArclen(double Rotation);

#endif /* MOTOR_CONTROLLER_H_ */
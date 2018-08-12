/*
 * This should calculate how to move the motors given a movement command
 * Inputs:  Path to follow as some sort of vector function
 * Outputs: Motor Commands to HardwareIO
 */

#include "MotorController.hpp"
#include "../../behaviour/src/SimplePathPlanner.hpp"
#include "../../localisation/src/Localisation.hpp"
#include "../../platform/src/Kinematics.hpp"

void MotorController_init() {
    std::cout << "Initilising MOTOR CONTROLLER" << std::endl;
}

int MotorController() {
    PathPlanner pplanner;

    double Forward  = 1;
    double Rotation = M_PI / 2;

    pplanner.emplace_path(Forward, Rotation);
    if (!pplanner.check_path()) {
        std::vector<std::pair<double, double>>::const_iterator ret_vec = pplanner.get_first_path();
        pplanner.path_erase_first();

        if (MotorDriver(ret_vec->first, ret_vec->second) != 0) {
            std::cout << "Error Could not calculate motor driver" << std::endl;
            return -1;
        }
    }
    return 0;
}

// Convention here would say Forward is positive, Left Rotation is positive
// Essentially takes a polar vector in world coordinates
int MotorDriver(double Forward, double Rotation) {
    // Take current vehicle vector
    // Take the goal vehicle vector
    // Take the difference to figure out the movement needed

    double wGoal[2] = {Forward, Rotation - Localistation::wTank_theta};
    double Goal_Dist[2];  // 0 is the left, 1 is the right
    Goal_Dist[0] = -ConvertRotationToArclen(wGoal[1]);
    Goal_Dist[1] = -Goal_Dist[0];
    std::cout << "Left wheel " << Goal_Dist[0] << ", Right wheel " << Goal_Dist[1] << std::endl;

    // Now account for the forward distance required
    Goal_Dist[0] += Forward;
    Goal_Dist[1] += Forward;

    std::cout << "Left wheel " << Goal_Dist[0] << ", Right wheel " << Goal_Dist[1] << std::endl;
    return 0;
}

double ConvertDistanceToRotation(double Goal_Dist) {
    return (DistToRev * Goal_Dist);  // meters per rotation * goal meters
}

double ConvertRotationToArclen(double Rotation) {
    return (Rotation * Kinematics::tank_width * 0.5);
}

// Path planner takes current world position
// Takes any avoiding obstacles
// Takes goal position

// Forms vectors betten all of these as a list
// This list is formed by looking at local maximum and minimum/ gradient inversions along the path
// This list then is passed to the motor controller

// the motor controller calculates the drive for each wheel
// and executes it

// There is a priviso for being able to take as many parts to the path as you please
// hence if you wanted to turn on the spot and drive to xy, you could have 2 components to the list
// the first being the zero length vector with a rotation
// the second being the length of the goal
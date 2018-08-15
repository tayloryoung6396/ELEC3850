/*
 * This should calculate how to move the motors given a movement command
 * Inputs:  Path to follow as some sort of vector function
 * Outputs: Motor Commands to HardwareIO
 */

#include "MotorController.hpp"
std::vector<std::pair<double, double>> PathPlanner::path_vec;

void MotorController_init() {
    std::cout << "Initilising MOTOR CONTROLLER" << std::endl;
}

int MotorController() {
    PathPlanner pplanner;

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

    double wGoal[2] = {Forward, Rotation};
    double Goal_Dist[2];  // 0 is the left, 1 is the right
    Goal_Dist[0] = -ConvertRotationToArclen(wGoal[1]);
    Goal_Dist[1] = -Goal_Dist[0];

    // Now account for the forward distance required
    Goal_Dist[0] += Forward;
    Goal_Dist[1] += Forward;

    std::cout << "Left wheel " << Goal_Dist[0] << ", Right wheel " << Goal_Dist[1] << std::endl;

    // Given that i actually get to the goal position
    // Set the new tank odometry rotation to the expected one..
    Localistation::wTank_theta += Rotation;
    // Localistation::TankToWorld(Goal_Dist);

    // TODO Here i should send the goal positions to the servos

    return 0;
}

int MotorDirector() {
    // Check all drive motors are in the last known position
    // And or check that we have reached our goal

    // TODO
    // Bulk read the 2 motor servos
    // Check the current servo position
    // if (moving) {
    //     update the number of revolutions we've done
    //     if(moving_forward && last_pos < 3500 && curr_pos > 0){
    //         revolution++
    //     }
    //     else if(moving_backwards && last_pos > 500 && curr_pos < 3500){
    //         revolution++
    //     }
    //     if (we're on the correct revolution && pres_pos ~= goal_pos){
    //         stop driving
    //         update moving
    //     }
    //     else if (we are on the correct revolution){
    //         maybe take control and watch?
    //         while(pres_pos !~= goal pos){
    //            keep polling etc
    //         }
    //     }
    // update locatisation with the actual position we finished at
    // }
    // else{
    //     check we are where we think we are
    // }
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

void PathPlanner::emplace_path(double Forward, double Rotation) {
    path_vec.emplace_back(std::make_pair(Forward, Rotation));
}

bool PathPlanner::check_path() {
    if (path_vec.empty()) {
        return true;
    }
    return false;
}

std::vector<std::pair<double, double>>::const_iterator PathPlanner::get_first_path() {
    return path_vec.begin();
}

void PathPlanner::path_erase_first() {
    path_vec.erase(path_vec.begin());
}
/*
 * This should calculate how to move the motors given a movement command
 * Inputs:  Path to follow as some sort of vector function
 * Outputs: Motor Commands to HardwareIO
 */

#include "MotorController.hpp"
std::vector<std::pair<double, double>> PathPlanner::path_vec;  // Vector of pairs of X Y goal coordinates in tank space

uint32_t PathPlanner::goal_pos[2]       = {0};
uint32_t PathPlanner::prev_pos[2]       = {0};
uint32_t PathPlanner::curr_pos[2]       = {0};
uint8_t PathPlanner::curr_revolution[2] = {0};
int PathPlanner::moving_flag[2]         = {0};

void MotorController_init() {
    std::cout << "Initilising MOTOR CONTROLLER" << std::endl;
    // Initialise current motor position
    executeReadSingle(
        Motor_L, MX64_ADDRESS_VALUE(PRESENT_POSITION), MX64_SIZE_VALUE(PRESENT_POSITION), PathPlanner::curr_pos[0]);
    executeReadSingle(
        Motor_R, MX64_ADDRESS_VALUE(PRESENT_POSITION), MX64_SIZE_VALUE(PRESENT_POSITION), PathPlanner::curr_pos[1]);
}

int MotorController() {
    // Iterate through each vector of pairs
    // Convert it to a polar vector and execute it - all in tank space
    PathPlanner pplanner;

    if (!pplanner.check_path()) {
        std::vector<std::pair<double, double>>::const_iterator ret_vec = pplanner.get_first_path();
        pplanner.path_erase_first();

        if (MotorDriver_Distance(ret_vec->first, ret_vec->second) != 0) {
            std::cout << "Error Could not calculate motor driver" << std::endl;
            return -1;
        }
    }
    return 0;
}

// Convention here would say Forward is positive, Left Rotation is positive
// Essentially takes a polar vector in world coordinates
int MotorDriver_Distance(double Forward, double Rotation) {
    // Take the forward and roation relative to the tank and convert it into a pair of motor commands
    // This will be a revolution amount/position - this might just be a total amount irrelevant of revolutions and have
    // it accounted for later

    // Set tank to moving[2] if we get a non zero input

    double wGoal[2] = {Forward, Rotation};
    double Goal_Dist[2];  // 0 is the left, 1 is the right
    Goal_Dist[0] = -ConvertRotationToArclen(wGoal[1]);
    Goal_Dist[1] = -Goal_Dist[0];

    // Now account for the forward distance required
    Goal_Dist[0] += Forward;
    Goal_Dist[1] += Forward;

    std::cout << "Left wheel " << Goal_Dist[0] << ", Right wheel " << Goal_Dist[1] << std::endl;

    uint8_t servo_ID[2] = {Motor_L, Motor_R};
    for (int i = 0; i < 2; i++) {
        // Set moving flag
        PathPlanner::moving_flag[i] = (Goal_Dist[i] == 0) ? 0 : ((Goal_Dist[i] < 0) ? (-1) : 1);
        // Set goal position
        // PathPlanner::moving_flag[i] = Goal_Dist[i];
        // // Set goal revolution
        // // Convert goal distance to number of revolutions
        // // This counts down to 0, so add it to our outstanding revolutions
        // PathPlanner::curr_revolution[i] += ConvertDistanceToRotation(Goal_Dist[i]);

        executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), 20);
        delay(10);
    }
    return 0;
}

// Note in this configuration, it is not a polar vector
int MotorDriver_Velocity(double Forward, double Rotation) {

    double Goal_Vel[2];  // 0 is the left, 1 is the right
    Goal_Vel[0] = -Rotation;
    Goal_Vel[1] = Rotation;

    // Now account for the forward distance required
    Goal_Vel[0] += Forward;
    Goal_Vel[1] += Forward;

    std::cout << "Left wheel " << Goal_Vel[0] << ", Right wheel " << Goal_Vel[1] << std::endl;

    uint8_t servo_ID[2] = {Motor_L, Motor_R};
    for (int i = 0; i < 2; i++) {
        // Set moving flag
        PathPlanner::moving_flag[i] = (Goal_Vel[i] == 0) ? 0 : ((Goal_Vel[i] < 0) ? (-1) : 1);

        executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), (uint32_t) Goal_Vel[i]);
        delay(10);
    }
    return 0;
}

int MotorDirector() {
    // Check all drive motors are in the last known position
    // And or check that we have reached our goal
    // The current revolution counts down to 0

    // TODO
    // Bulk read the 2 motor servos
    // MOTOR_T motor_data;
    // executeReadMulti(servo_ID, address, motor_data, size, count);
    const int offset               = 200;
    const uint16_t max_position[2] = {(uint16_t) std::numeric_limits<uint32_t>::max() / 2 - offset,
                                      (uint16_t) std::numeric_limits<uint32_t>::min() / 2 + offset};
    const uint16_t min_position[2] = {(uint16_t) std::numeric_limits<uint32_t>::min() / 2 + offset,
                                      (uint16_t) std::numeric_limits<uint32_t>::max() / 2 - offset};

    uint8_t count       = 2;
    uint8_t servo_ID[2] = {Motor_L, Motor_R};

    for (int i = 0; i < count; i++) {
        executeReadSingle(servo_ID[i],
                          MX64_ADDRESS_VALUE(PRESENT_POSITION),
                          MX64_SIZE_VALUE(PRESENT_POSITION),
                          PathPlanner::curr_pos[i]);
        std::cout << "Servo " << (int) servo_ID[i] << " Present position " << (int) PathPlanner::curr_pos[i]
                  << std::endl;
        delay(10);
    }


    // Check the current servo position
    if (PathPlanner::moving_flag[0] != 0 | PathPlanner::moving_flag[1] != 0) {
        std::cout << "Moving flag " << (int) PathPlanner::moving_flag[0] << " " << (int) PathPlanner::moving_flag[1]
                  << std::endl;
        for (int i = 0; i < 2; i++) {
            // update the number of revolutions weve done
            if (PathPlanner::moving_flag[i] == 1 && PathPlanner::prev_pos[i] < max_position[i]
                && PathPlanner::curr_pos[i] > min_position[i]) {
                PathPlanner::curr_revolution[i]--;
                std::cout << "1 Decrement revolutions " << PathPlanner::curr_revolution[i] << " ID " << i << std::endl;
            }
            else if (PathPlanner::moving_flag[i] == -1 && PathPlanner::prev_pos[i] > min_position[i]
                     && PathPlanner::curr_pos[i] < max_position[i]) {
                PathPlanner::curr_revolution[i]--;
                std::cout << "2 Decrement revolutions " << PathPlanner::curr_revolution[i] << " ID " << i << std::endl;
            }
            // were on the correct revolution
            if (PathPlanner::curr_revolution[i] == 0 && (PathPlanner::curr_pos[i] == PathPlanner::goal_pos[i] + 100)
                || (PathPlanner::curr_pos[i] == PathPlanner::goal_pos[i] - 100)) {  // Goal pos +- some delta
                // stop driving update moving = 0
                PathPlanner::moving_flag[i] = 0;
                executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), 0);
                std::cout << "Stopped moving"
                          << " ID " << i << std::endl;
            }
            // TODO Implement this
            // else if (PathPlanner::curr_revolution[i] == 0) {
            //     // maybe take control and watch ?
            //     while (PathPlanner::curr_pos[i] != PathPlanner::goal_pos[i]) {  // TODO Goal pos +- some delta
            //         // keep polling etc
            //         break;
            //     }
            // }
        }
    }
    // TODO update locatisation with the actual position we finished at
    // Localisation::w_Tank_Position[0] =
    // Need to somehow convert a change in rotation to a vector?
    else {
        // TODO check we are where we think we are
    }
    return 0;
}

double ConvertDistanceToRotation(double Goal_Dist) {
    return (Goal_Dist / DistToRev);  // Return revolutions
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

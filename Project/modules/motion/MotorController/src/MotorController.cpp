/*
 * This should calculate how to move the motors given a movement command
 * Inputs:  Path to follow as some sort of vector function
 * Outputs: Motor Commands to HardwareIO
 */

#include "MotorController.hpp"
std::vector<std::pair<double, double>> PathPlanner::path_vec;  // Vector of pairs of X Y goal coordinates in tank space

int32_t PathPlanner::goal_pos[2]       = {0};
int32_t PathPlanner::prev_pos[2]       = {0};
int32_t PathPlanner::curr_pos[2]       = {0};
int8_t PathPlanner::curr_revolution[2] = {0};
int PathPlanner::moving_flag[2]        = {0};

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

    // If we're not moving
    if (PathPlanner::moving_flag[0] == 0 && PathPlanner::moving_flag[1] == 0) {
        if (!pplanner.check_path()) {
            std::vector<std::pair<double, double>>::const_iterator ret_vec = pplanner.get_first_path();
            pplanner.path_erase_first();

            std::cout << "New point " << ret_vec->first << ", " << ret_vec->second << std::endl;

            if (MotorDriver_Distance(ret_vec->first, ret_vec->second) != 0) {
                std::cout << "Error Could not calculate motor driver" << std::endl;
                return -1;
            }
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

    // Set moving flag
    PathPlanner::moving_flag[0] = (Goal_Dist[0] == 0) ? 0 : ((Goal_Dist[0] < 0) ? (-1) : 1);
    PathPlanner::moving_flag[1] = (Goal_Dist[1] == 0) ? 0 : ((Goal_Dist[1] < 0) ? (1) : -1);

    std::cout << "Left wheel " << Goal_Dist[0] << ", Right wheel " << Goal_Dist[1] << std::endl;

    uint8_t servo_ID[2] = {Motor_L, Motor_R};
    for (int i = 0; i < 2; i++) {
        // Set goal position
        // // Set goal revolution
        // // Convert goal distance to number of revolutions
        // // This counts down to 0

        executeReadSingle(servo_ID[i],
                          MX64_ADDRESS_VALUE(PRESENT_POSITION),
                          MX64_SIZE_VALUE(PRESENT_POSITION),
                          PathPlanner::curr_pos[i]);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    // Flip our negative motor to be positive

    PathPlanner::curr_pos[1] = -PathPlanner::curr_pos[1];

    for (int i = 0; i < 2; i++) {
        // We currently have a goal distance in meters Goal_Dist
        // Convert the meter to a position end value
        double Pos_required = ConvertDistanceToPosition(Goal_Dist[i]);
        // Add this to the current position
        double Pos_total = Pos_required + PathPlanner::curr_pos[i];
        // If this is greater than the end limit account for that..
        if (Pos_total > std::numeric_limits<uint32_t>::max()) {
            Pos_total = std::numeric_limits<int32_t>::min() + Pos_total - std::numeric_limits<uint32_t>::max();
        }
        PathPlanner::goal_pos[i] = Pos_total;

        std::cout << "Pos_required " << Pos_required << std::endl;
        std::cout << "Pos_current " << PathPlanner::curr_pos[i] << std::endl;
        std::cout << "Pos_total " << Pos_total << std::endl;
        std::cout << "Goal position " << (int) PathPlanner::goal_pos[i] << std::endl;
    }

    // Reverse the flip of our negative motor
    // Remember that our revolutions are both positive in the forward direction

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < 2; i++) {

        std::cout << "Wheel " << i << " Current position" << (int) PathPlanner::curr_pos[i] << std::endl;
        std::cout << "Wheel " << i << " Expected revolutions" << (int) PathPlanner::curr_revolution[i] << std::endl;
        std::cout << "Wheel " << i << " Final Position" << (int) PathPlanner::goal_pos[i] << std::endl;

        executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), (i == 0 ? 20 : -20));
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
    Goal_Vel[1] = -Goal_Vel[1];

    std::cout << "Left wheel " << Goal_Vel[0] << ", Right wheel " << Goal_Vel[1] << std::endl;

    uint8_t servo_ID[2] = {Motor_L, Motor_R};
    for (int i = 0; i < 2; i++) {
        // Set moving flag
        PathPlanner::moving_flag[i] = (Goal_Vel[i] == 0) ? 0 : ((Goal_Vel[i] < 0) ? (-1) : 1);

        executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), (int32_t) Goal_Vel[i]);
        delay(10);
    }
    return 0;
}

int MotorDirector() {

    // For each servo, lets run the director
    uint8_t count = 2;
    // The left servo is conventional with positive rotation forward
    // The right servo is reversed
    uint8_t servo_ID[2] = {Motor_L, Motor_R};

    // Get the current position of each servo should be between min and max position
    for (int i = 0; i < count; i++) {
        executeReadSingle(servo_ID[i],
                          MX64_ADDRESS_VALUE(PRESENT_POSITION),
                          MX64_SIZE_VALUE(PRESENT_POSITION),
                          PathPlanner::curr_pos[i]);
        delay(10);
    }

    // For both motors the revolutions are position in the forward direction
    // Read in the position

    // // Invert our negative motor
    PathPlanner::curr_pos[1] = -PathPlanner::curr_pos[1];
    for (int i = 0; i < count; i++) {
        if (PathPlanner::moving_flag[i] != 0) {
            // Check to see if our goal is our current position
            if ((PathPlanner::curr_pos[i] <= PathPlanner::goal_pos[i] + 10)
                && (PathPlanner::curr_pos[i] >= PathPlanner::goal_pos[i] - 10)) {
                // If that is satisfied then stop the motor
                // std::cout << "Servo " << i << " at goal position" << std::endl;
                PathPlanner::moving_flag[i] = 0;
                executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), 0);
                delay(10);
                std::cout << "Stopped moving"
                          << " ID " << i << std::endl;

                // Check to see if there's more path info
                PathPlanner pplanner;
                if (!pplanner.check_path()) {
                    std::vector<std::pair<double, double>>::const_iterator ret_vec = pplanner.get_first_path();
                    pplanner.path_erase_first();

                    std::cout << "New point " << ret_vec->first << ", " << ret_vec->second << std::endl;

                    if (MotorDriver_Distance(ret_vec->first, ret_vec->second) != 0) {
                        std::cout << "Error Could not calculate motor driver" << std::endl;
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}

double ConvertDistanceToPosition(double Goal_Dist) {
    return (Goal_Dist / DistToRev * std::numeric_limits<uint32_t>::max());  // Return revolutions
}

double ConvertDistanceToRotation_r(double Goal_Dist) {
    return (fmod(Goal_Dist, DistToRev));  // Return revolutions
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

void PathPlanner::path_erase_last() {
    path_vec.erase(path_vec.end());
}
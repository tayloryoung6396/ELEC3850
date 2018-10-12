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

    if (!pplanner.check_path()) {
        std::vector<std::pair<double, double>>::const_iterator ret_vec = pplanner.get_first_path();
        pplanner.path_erase_first();

        if (MotorDriver_Distance(ret_vec->first, ret_vec->second) != 0) {
            std::cout << "Error Could not calculate motor driver" << std::endl;
            return -1;
        }
    }
    else {
        // We must be at our goal?
        // Or we don't have a goal
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
    Goal_Dist[1] = -Goal_Dist[1];

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
        // Convert this to the number of revolutions required
        double rev_required = ConvertDistanceToRotation(Goal_Dist[i]) * 2.0 * std::numeric_limits<int32_t>::max();
        // Convert our current position to the current amount through a revolution
        double rev_current =
            ConvertDistanceToRotation(PathPlanner::curr_pos[i] / (2.0 * std::numeric_limits<int32_t>::max())) * 2.0 * std::numeric_limits<int32_t>::max();
        // Add these together to get the total amnount of revolutions
        double rev_total = rev_required + rev_current;
        // This value can then be converted to revolutions and remaining amount of revolution
        PathPlanner::curr_revolution[i] += (std::floor(rev_total)); //* 2.0 * std::numeric_limits<int32_t>::max();
        // Convert this remaining amount into motor position goal
        PathPlanner::goal_pos[i] = (rev_total - std::floor(rev_total));// * 2.0 * std::numeric_limits<int32_t>::max();

	std::cout << "rev_required " << rev_required << std::endl;
	std::cout << "rev_current " << rev_current << std::endl;
	std::cout << "rev_ total " << rev_total << std::endl;
	std::cout << "Current revolutions " << (int)PathPlanner::curr_revolution[i] << std::endl;
	std::cout << "Goal position " << (int)PathPlanner::goal_pos[i] << std::endl;

    }

    // Reverse the flip of our negative motor
    // Remember that our revolutions are both positive in the forward direction
    PathPlanner::goal_pos[1] = -PathPlanner::goal_pos[1];

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


// TODO Overhaul this, it's missing some conversions

// 0.48m per revolution

int MotorDirector() {


    // When heading in a forward rotation, these are the last values we see
    const int32_t max_position[2] = {std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min()};
    // These are the other end of that rotation
    const int32_t min_position[2] = {std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()};

    // Sanity check my directions
    // std::cout << "Servo 1 max: " << max_position[0] << " min: " << min_position[0] << std::endl;
    // std::cout << "Servo 2 max: " << max_position[1] << " min: " << min_position[1] << std::endl;

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
        std::cout << "Servo " << (int) servo_ID[i] << " Present position " << (int) PathPlanner::curr_pos[i]
                  << std::endl;
        delay(10);
    }

    // For both motors the revolutions are position in the forward direction
    // Read in the position

    // TODO Don't think i need to invert this one
    // // Invert our negative motor
    PathPlanner::curr_pos[1] = -PathPlanner::curr_pos[1];
    for (int i = 0; i < count; i++) {
        // If we are moving forward, check if we have overflowed
        if (PathPlanner::moving_flag[i] > 0 && PathPlanner::prev_pos[i] < max_position[i]
            && PathPlanner::curr_pos[i] > min_position[i] && PathPlanner::curr_revolution[i] != 0) {
            // Decrement revolutions
            std::cout << "Decrement servo " << i << " to " << PathPlanner::curr_revolution[i] - 1 << std::endl;
            PathPlanner::curr_revolution[i]--;
        }
        // If we are moving backwards, check if we have underflowed
        if (PathPlanner::moving_flag[i] < 0 && PathPlanner::prev_pos[i] > min_position[i]
            && PathPlanner::curr_pos[i] < max_position[i] && PathPlanner::curr_revolution[i] != 0) {
            // Increment revolutions
            std::cout << "Increment servo " << i << " to " << PathPlanner::curr_revolution[i] + 1 << std::endl;
            PathPlanner::curr_revolution[i]++;
        }
        // For each motor check to see if our revolutions are 0
        if (PathPlanner::curr_revolution[i] == 0) {
            std::cout << "Servo " << i << " revolution 0" << std::endl;
            // If they are 0, then check to see if our goal is our current position
            if ((PathPlanner::curr_pos[i] == PathPlanner::goal_pos[i] + 100)
                | (PathPlanner::curr_pos[i] == PathPlanner::goal_pos[i] - 100)) {
                // If that is satisfied then stop the motor
                std::cout << "Servo " << i << " at goal position" << std::endl;
                PathPlanner::moving_flag[i] = 0;
                executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), 0);
		delay(10);
                std::cout << "Stopped moving"
                          << " ID " << i << std::endl;
            }
        }
    }

    return 0;
}

double ConvertDistanceToRotation(double Goal_Dist) {
    return (Goal_Dist / DistToRev);  // Return revolutions
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

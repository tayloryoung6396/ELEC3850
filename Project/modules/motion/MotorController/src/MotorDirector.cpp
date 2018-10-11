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
        double rev_required = ConvertDistanceToRotation(Goal_Dist[i]);
        // Convert our current position to the current amount through a revolution
        double rev_current =
            ConvertDistanceToRotation(PathPlanner::curr_pos[i] / (2 * std::numeric_limits<int32_t>::max()));
        // Add these together to get the total amnount of revolutions
        double rev_total = rev_required + rev_current;
        // This value can then be converted to revolutions and remaining amount of revolution
        PathPlanner::curr_revolution[i] += (std::floor(rev_total)) * std::numeric_limits<int32_t>::max();
        // Convert this remaining amount into motor position goal
        PathPlanner::goal_pos[i] = (rev_total - std::floor(rev_total)) * std::numeric_limits<int32_t>::max();
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


        // For both motors the revolutions are position in the forward direction
        // Read in the position

        // TODO Don't think i need to invert this one
        // // Invert our negative motor
        // PathPlanner::curr_pos[1] = -PathPlanner::curr_pos[1];

        // If we are moving forward, check if we have overflowed
        if (PathPlanner::moving_flag[i] > 0) {
            // Decrement revolutions
            std::cout << "Decrement servo " << i << " to " << PathPlanner::curr_revolution[i] - 1 << std::endl;
            PathPlanner::curr_revolution[i]--;
        }
        // If we are moving backwards, check if we have underflowed
        if (PathPlanner::moving_flag[i] > 0) {
            // Increment revolutions
            std::cout << "Increment servo " << i << " to " << PathPlanner::curr_revolution[i] + 1 << std::endl;
            PathPlanner::curr_revolution[i]++;
        }
        // For each motor check to see if our revolutions are 0
        if (PathPlanner::curr_revolution[i] == 0) {
            std::cout << "Servo " << i << " revolution 0" << std::endl;
            // If they are 0, then check to see if our goal is our current position
            if ((PathPlanner::curr_pos[1] == PathPlanner::goal_pos[1] + 100)
                | (PathPlanner::curr_pos[1] == PathPlanner::goal_pos[1] - 100)) {
                // If that is satisfied then stop the motor
                std::cout << "Servo " << i << " at goal position" << std::endl;
                PathPlanner::moving_flag[i] = 0;
                executeWriteSingle(servo_ID[i], MX64_ADDRESS_VALUE(GOAL_VELOCITY), 0);
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
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


    // Check the current servo position
    if (PathPlanner::moving_flag[0] != 0 | PathPlanner::moving_flag[1] != 0) {
        std::cout << "Moving flag " << (int) PathPlanner::moving_flag[0] << " " << (int) PathPlanner::moving_flag[1]
                  << std::endl;
        for (int i = 0; i < 2; i++) {
            // update the number of revolutions weve done
            if (PathPlanner::moving_flag[i] == 1 && PathPlanner::prev_pos[i] < max_position[i]
                && PathPlanner::curr_pos[i] > min_position[i]) {
                PathPlanner::curr_revolution[i]--;
                std::cout << "Decrement positive revolutions " << (int) PathPlanner::curr_revolution[i] << " ID " << i
                          << std::endl;
            }
            else if (PathPlanner::moving_flag[i] == -1 && PathPlanner::prev_pos[i] > min_position[i]
                     && PathPlanner::curr_pos[i] < max_position[i]) {
                PathPlanner::curr_revolution[i]--;
                std::cout << "Decrement negative revolutions " << (int) PathPlanner::curr_revolution[i] << " ID " << i
                          << std::endl;
            }
            // were on the correct revolution
            if (PathPlanner::curr_revolution[i] == 0
                && ((PathPlanner::curr_pos[i] == PathPlanner::goal_pos[i] + 100)
                    || (PathPlanner::curr_pos[i] == PathPlanner::goal_pos[i] - 100))) {  // Goal pos +- some delta
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
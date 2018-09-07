// clang-format off
// Given you know your world position
// Someone tells me a goal position in the world to go to
// Then calculate the vector/ series of vectors to get from the current world pos to the goal world pos. This is now in tank coordinates
// Then iterate through that list of vectors
// Each time convert the vector that is in tank space to the left and right motor distances.
// Then the distances to revolutions/positions.
// Check on each loop whether we have completed the vector yet 
    // - update the localisation position either way to the current position
    // This is done by taking the last localisation position (which might have been updated along the path) 
    // as well as the last motor position that is linked with that same localisation position
    // and taking the difference between the current and prev motor positions and reverse engineering the vector position
// If that specific vector has been "completed" check to see if the executed final position matches the goal revolution/position
// If it does no need to update any of the other vectors continue but still update the localisation position
// If it doesn't then we need to adjust our list of vectors to put us back on path.

double Localisation::w_Tank_Position[2];  // X Y Current position in world coordinates
double Localisation::w_Tank_Rotation;     // Rotation about the w_Tank_Position in the anticlockwise direction (radians)

double Localisation::w_Goal_Position[2];  // X Y Goal position in world coordinates

std::vector<std::pair<double, double>> PathPlanner::path_vec;  // Vector of pairs of X Y goal coordinates in tank space

// Functions
/******************************Simple_Path_Planner******************************/.
// Given w_Goal_Position
Simple_Path_Planner(double w_Goal_Position[2]) {
    // Calculate path from w_Tank_Position to w_Goal_Position
    // This will be a vector of pairs X Y in tank space
    double t_Goal_Position[2] = {w_Goal_Position[0] - Localistation::w_Tank_Position[0], w_Goal_Position[1] - Localistation::w_Tank_Position[1]};
	// Calculate the rotation difference
    double t_Goal_Rotation = std::atan2(t_Goal_Position[1], t_Goal_Position[0]);

    // Convert into a forward and rotation command
    double Forward  = std::sqrt(std::pow(t_Goal_Position[0], 2) + std::pow(t_Goal_Position[1], 2));
    double Rotation = Localistation::wTank_theta - t_Goal_Rotation;
    PathPlanner pplanner;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
}

Recalculate_path() {
	// So i didn't end up where i wanted to, 
	// but i should be able to take my position and the goal and recalculate a new vector
}

/********************************Motor_Controller********************************/

void MotorController_init() {
    std::cout << "Initilising MOTOR CONTROLLER" << std::endl;
    // TODO Initialise current motor position
    executeReadSingle(Motor_L,
                          MX64_ADDRESS_VALUE(PRESENT_POSITION),
                          MX64_SIZE_VALUE(PRESENT_POSITION),
                          PathPlanner::curr_pos[0]);
    executeReadSingle(Motor_R,
                          MX64_ADDRESS_VALUE(PRESENT_POSITION),
                          MX64_SIZE_VALUE(PRESENT_POSITION),
                          PathPlanner::curr_pos[1]);
}

int Motor_Controller(){
	// Iterate through each vector of pairs
	// Convert it to a polar vector and execute it - all in tank space
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

Motor_Driver(Forward, Rotation){
	// Take the forward and roation relative to the tank and convert it into a pair of motor commands
	// This will be a revolution amount/position - this might just be a total amount irrelevant of revolutions and have it accounted for later

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

Motor_Director(){
	// Remembering that this has to happen over both motors, not sure how this will affect the current plan

	// Essentially if either controller is running, present position and previous position to figure out the distance step

	// We might only want to look at this when we are moving, if we aren't we dont really care
	if(moving){
	    // update the localisation position either way to the current position
	    // This is done by taking the last localisation position (which might have been updated along the path) 
	    // as well as the last motor position that is linked with that same localisation position
	    // and taking the difference between the current and prev motor positions and reverse engineering the vector position
	    
	    // this needs to keep track of the revolutions the motor has done
	    // ALTERNATIVELY
		// use just a total distance and subtract some distance travelled accounting for the overflowing

	    if(Autonomour_Controller){
			// Check whether we have completed the vector yet 		
			if(complete){
				// If that specific vector has been "completed" check to see if the executed final position matches the goal revolution/position
				// If it does no need to update any of the other vectors continue but still update the localisation position
				// If it doesn't then we need to adjust our list of vectors to put us back on path.
			}
		}
	}
}

/********************************Autonomous_Controller********************************/

// This will be a state machine that runs based on what case we are in


if(!have object){
    if(known object pos){
        // go to the object
        path planner
    }
    else if(unknown object pos){
        // search for it
        weighted_search()
    }
}

else if (have object){
    if(known goal pos){
        // go to the goal
        path planner
    }
    else if(unknown goal pos){
        // search for it
        weighted_search()
    }
}

/********************************PS3_Controller********************************/

// Here we read in buttons and what not.
// The joystick is some scaled velocity value
// Maybe between some min velocity and max velocity

// This then directly set the motor speed
// ALTERNATIVELY
// Use the values to set Motor_Driver(Polar_Vector); - tank space
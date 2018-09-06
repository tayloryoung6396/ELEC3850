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
Simple_Path_Planner() {
    // Calculate path from w_Tank_Position to w_Goal_Position
    // This will be a vector of pairs X Y in tank space
}

Recalculate_path() {}

/********************************Motor_Controller********************************/

Motor_Controller(){
	// Iterate through each vector of pairs
	// Convert it to a polar vector and execute it
	Motor_Driver(Polar_Vector);
}

Motor_Driver(Forward, Rotation){
	// Take the forward and roation relative to the tank and convert it into a pair of motor commands
	// This will be a revolution amount/position
}

Motor_Director(){
	
}
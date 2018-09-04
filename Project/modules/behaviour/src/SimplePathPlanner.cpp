/*
 * This should handle the Inputs and Outputs to the world.
 * Communication between the perpherials and the MCU.
 * Inputs:  Goal of robot relative to robot as vector
            Any obstacle for the robot to avoid as vector
            Kinematics
 * Outputs: Path to follow as some sort of vector function
 */

#include "SimplePathPlanner.hpp"

void SimplePathPlanner_init() {}

int SimplePathPlanner(double wGoal[2]) {
    // Given a list of obstacles
    // Given a goal position
    // Knowing the current position
    // double Localisation::wTank_pos[2];

    // Calculate our vector from out current position to our goal position
    double tGoal[2] = {wGoal[0] - Localistation::wTank_pos[0], wGoal[1] - Localistation::wTank_pos[1]};

    // Calculate the rotation difference
    double rGoal = std::atan2(tGoal[1], tGoal[0]);

    double Forward  = std::sqrt(std::pow(tGoal[0], 2) + std::pow(tGoal[1], 2));
    double Rotation = Localistation::wTank_theta - rGoal;
    PathPlanner pplanner;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    return 0;
}

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
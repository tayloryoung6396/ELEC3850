/*
 * This should take the occupancy map and plan a path to a given goal
 * Inputs:  Goal of robot relative to robot as vector
            Any obstacle for the robot to avoid as vector
            Kinematics
 * Outputs: Path to follow as some sort of vector function
 */

#include "SimplePathPlanner.hpp"

void SimplePathPlanner_init() {}

int SimplePathPlanner() {
    // TODO use the occupancy map to find an appropriate path

    // Calculate path from w_Tank_Position to w_Goal_Position
    // This will be a vector of pairs X Y in tank space
    double t_Goal_Position[2] = {Localisation::w_Goal_Position[0] - Localistation::w_Tank_Position[0],
                                 Localisation::w_Goal_Position[1] - Localistation::w_Tank_Position[1]};
    // Calculate the rotation difference
    double t_Goal_Rotation = std::atan2(t_Goal_Position[1], t_Goal_Position[0]);

    // Convert into a forward and rotation command
    double Forward  = std::sqrt(std::pow(t_Goal_Position[0], 2) + std::pow(t_Goal_Position[1], 2));
    double Rotation = Localistation::w_Tank_Rotation - t_Goal_Rotation;
    PathPlanner pplanner;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    return 0;
}

void Recalculate_path() {
    // So i didn't end up where i wanted to,
    // but i should be able to take my position and the goal and recalculate a new vector
}
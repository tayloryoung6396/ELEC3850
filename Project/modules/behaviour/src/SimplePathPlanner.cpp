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
    // TODO these should be read in from localisation parameter
    double Localistation::wTank_pos[2];

    // Calculate our vector from out current position to our goal position
    double tGoal[2] = {wGoal[0] - wTank[0], wGoal[1] - wTank[1]};

    // Calculate the rotation difference
    double rGoal = std::atan2(tGoal[1], tGoal[0]);

    double Forward  = std::sqrt(std::pow(tGoal[0], 2) + std::pow(tGoal[1], 2));
    double Rotation = Localistation::wTank_theta - rGoal;
    PathPlanner pplanner;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    return 0;
}
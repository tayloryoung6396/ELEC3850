/*
 * This should handle the Inputs and Outputs to the world.
 * Communication between the perpherials and the MCU.
 * Inputs:  Goal of robot relative to robot as vector
            Any obstacle for the robot to avoid as vector
            Kinematics
 * Outputs: Path to follow as some sort of vector function
 */

#include "SimplePathPlanner.hpp"

std::vector<std::pair<double, double>> PathPlanner::path_vec;  // = {{1, M_PI / 2}, {0, -M_PI / 2}, {-1, 0}};

void SimplePathPlanner_init() {}

int SimplePathPlanner_main() {
    // Given a list of obstacles
    // Given a goal position
    // Knowing the current position
    double Forward  = 1;
    double Rotation = M_PI / 2;
    PathPlanner pplanner;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    return 0;
}

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
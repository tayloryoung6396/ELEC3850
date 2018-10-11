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
    double t_Goal_Position[2] = {Localisation::w_Goal_Position[0] - Localisation::w_Tank_Position[0],
                                 Localisation::w_Goal_Position[1] - Localisation::w_Tank_Position[1]};
    // Calculate the rotation difference
    double t_Goal_Rotation = std::atan2(t_Goal_Position[1], t_Goal_Position[0]);

    // Convert into a forward and rotation command
    double Forward  = std::sqrt(std::pow(t_Goal_Position[0], 2) + std::pow(t_Goal_Position[1], 2));
    double Rotation = Localisation::w_Tank_Rotation - t_Goal_Rotation;
    PathPlanner pplanner;

    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    return 0;
}

void Recalculate_path() {
    // So i didn't end up where i wanted to,
    // but i should be able to take my position and the goal and recalculate a new vector
}

// How do we use the occupancy map for path planning..
// in grid space we can locate our current and end goal position
//
// The method currently conceptualised is a weighted 'bayer' style search.
// Each cell is looked at and all surrounding cells also considered.
// For each cell calculate how close that cell and the 8 surrounding cells are to the middle value (0.5)
// To do this, any value in the 9 cells that is 0.5 +- some delta, is totalled to find the number of cells close to this
// average weight, this is then continued over the whole space.

// However the problem with this method is, it doesn't actually find where the clusters of unsearched space are, and
// becomes recursive


// Lets talk A* and maze solving..

// This aims to find a path from a known position to a known goal within out occupany map.
// This is broken into two stages:

// Stage 1 - Node conversion
// Serach over the rows of our maze and find all cells that satisfy this set of rules:
// TODO insert rules
// The nodes will consist of the following structure:
// struct {
//     position[2];                 // Cell
//     left_neighbour[3]   = {-1};  // Cell, distance
//     right_neighbour[3]  = {-1};  // Cell, distance
//     top_neighbour[3]    = {-1};  // Cell, distance
//     bottom_neighbour[3] = {-1};  // Cell, distance
//     goal_dist;
// }
// Then for each node calculate the euclidian distance to the goal
// These will be stored as a vector of structs
// This is then fed into stage 2

// Stage 2 - A* Search alg
// Should probably just google how this works..
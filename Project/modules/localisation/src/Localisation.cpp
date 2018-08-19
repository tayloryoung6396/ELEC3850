/*
 * This should take the goal objects and motor commands to calculate the vector between the two.
 * Inputs:  Motor Commands
            Objects
 * Outputs: Goal Position as vector
 */

#include "Localisation.hpp"

double Localistation::wTank_theta  = 0;
double Localistation::wTank_pos[2] = {0};

void Localisation_init() {}

int Localisation_main() {
    // Goal_pos vec2d
    return 0;
}

void TankToWorld(double Goal_Dist[2]) {}

// 2D Map
// This is a crude method of creating a 2D localisation map
// Given some fixed workspace, divide it into a square grid section
// For this implementation 1.5m x 1.5m with a grid spacing of 0.025m

// grid_map[60][60]
// grid_map[x][y] = transform_us_world(us_id, get_dist(us_id));

// int transfor_us_world(int us_id, double dist){
//     	using kinematics for the specific us pos
//		using the current odometry position of the tank
//		transform the distance of the us_id to world coordinates
//		populate the cell closest to that x,y pos
//		return cell id
// }

// TODO this doesnt handle a changing environment
// need to account for objects that were on the map that have now been moved
// if we had an object beentween us and the new object, then clearly it's not there anymore

// TODO this doesnt account for objects seen..
// maybe there needs to be a second layer to the map that contains visually seen objects
// or another map

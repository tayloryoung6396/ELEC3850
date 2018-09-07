/*
 * This should take the Ultrasonic readings and the current position and populate an occupancy map of the surroundings
 * Inputs:  Ultrasonic sensor readings
 *          Position
 * Outputs: Occupancy map
 *			Position
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

// Occupancy mapping - apparently the name for doing ^^
// Suppose we build a grid fixed to the world.
// We start at some position withing the grid (probably initilise to some rough estimation of the center)
// Now we fill the grid with all equally weighted probabilities of an object being in that location
// Lets call this 0.5
// When we detect an object at a distance D on for instance or left hand side.
// Take the current grid position of the tank
// Figure out the grid position of the object (its probably somewhere relative to the known position)
// Now for each cell between us and the object position, calculate a new probability for that cell to be occupied
// Essentially make a function that takes these inputs:
// - Distance between the robot and the obstacle / the cell
// - Previous probability of the cell
// And outputs an updated probability
// This function might work over some exponential of the distance or something

// This grid spacing is the problem... do you make the grid the size of the radius of the robot
// which means for path planning you only need a single cell space to make it through,
// or do you make the size of the grid some smaller amount and need to do some harder path planning
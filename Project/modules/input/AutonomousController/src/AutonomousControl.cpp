/*
 * This should handle the autonomous control of the robot.
 * From the sensor inputs and classified objects decide how to handle everything.
 * Using the localisation and path planner
 * Inputs:  Sensor data
 *          Vector to objects of interest
 * Outputs: Distance to move/path
 *          Position to grip
 */

#include "AutonomousControl.hpp"

void AutonomousControl_init() {
    std::cout << "Initilising AUTONOMOUS CONTROLLER" << std::endl;
}

int AutonomousControl_main() {
    // TODO
    // There should be a state machine here,
    // States:
    // Searching
    // Collecting
    // Rescuing

    // These will go into the states
    // Figure out if there's stuff around me
    Localisation_main();
    // Decide how to move
    // TODO make this a variable in the autonomous controller
    double wGoal[2] = {0};
    SimplePathPlanner(wGoal);


    Gripper::Goal[0] = 0;
    Gripper::Goal[1] = 0;
    Gripper::Goal[2] = 0;

    return 0;
}

// if(!have object){
//     if(known object pos){
//         go to the object
//         path planner
//     }
//     else if(unknown object pos){
//         search for it
//         weighted_search()
//     }
// }

// else if (have object){
//     if(known goal pos){
//         go to the goal
//         path planner
//     }
//     else if(unknown goal pos){
//         search for it
//         weighted_search()
//     }
// }

// weighted_search()
// take the grid map and perform a weighted gradient over the map
// output a goal position to send to path planner

// #define OBSTACLE 1
// #define GRID_X 60
// #define GRID_Y 60

// // TODO this is slow, find a way to avoid the pow and sqrt
// int* weighted_search() {
//     int goal_x      = 0;
//     int goal_y      = 0;
//     double max_dist = 0;
//     curr_max_grid   = 0;

//     int tank_grid_x = 0;  // TODO populate from localisation
//     int tank_grid_x = 0;  // TODO populate from localisation

//     // Search each vertical column
//     for (int x = 0; x < GRID_X; x++) {
//         // Search each horizontal row
//         for (int y = 0; y < GRID_Y; y++) {
//             // If the cell doesnt have an obstacle curently in it
//             if (grid_map[x][y] != OBSTACLE) {
//                 max_dist = std::sqrt(std::pow(x - tank_grid_x, 2) + std::pow(y - tank_grid_y, 2));
//                 if (max_dist > curr_max_grid) {
//                     curr_max_grid = max_dist;
//                     goal_x        = x;
//                     goal_y        = y;
//                 }
//             }
//         }
//     }
// }

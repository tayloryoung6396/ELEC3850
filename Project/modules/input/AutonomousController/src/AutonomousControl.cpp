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

    // What do we want to do.. hence STATE MACHINE!

    // flags for conditions
    bool have_object = FALSE;

    double Goal_pos[3] = {0};  // TODO remove

    Localistation::w_Goal_Position[0] = 1;  // TODO remove
    Localistation::w_Goal_Position[1] = 0;  // TODO remove

    SimplePathPlanner();  // TODO remove

    MotorController();  // TODO remove

    // // TODO this is not optimized in any way.. you're an idiot
    // // TODO Account for being at the object already/goal and needing to do something
    // if (!have_object) {
    //     // known object pos
    //     if (Localistation::w_Goal_Position[0] >= 0 && Localistation::w_Goal_Position[1] >= 0) {
    //         // Check to see if we're at the goal or not
    //         if (1) {  // at goal) {
    //             // grab object TODO
    //             // check where the object is exactly
    //             // either use last image or do a whoe new classification?
    //             // perform grab

    //             IKGripper_Grab_Object(Goal_pos);
    //         }
    //         else {
    //             // go to the object path planner
    //             SimplePathPlanner();
    //         }
    //     }
    //     // unknown object pos
    //     // If our goal is negative then it means we don't have a valid goal/don't know a goal
    //     else if (Localistation::w_Goal_Position[0] < 0 | Localistation::w_Goal_Position[1] < 0) {
    //         // search for it
    //         // TODO // weighted_search();
    //     }
    // }

    // else if (have_object) {
    //     // known goal pos
    //     // If our goal is negative then it means we don't have a valid goal/don't know a goal
    //     if (Localistation::w_Goal_Position[0] >= 0 && Localistation::w_Goal_Position[1] >= 0) {
    //         // Check to see if we're at the goal or not
    //         if ((Localistation::w_Goal_Position[0] == Localistation::w_Tank_Position[0])
    //             && (Localistation::w_Goal_Position[1] == Localistation::w_Tank_Position[1])) {
    //             // Do something
    //             // Figure out where we are putting it TODO
    //             IKGripper_Place_Object(Goal_pos);
    //         }
    //         else {
    //             // go to the goal path planner
    //             SimplePathPlanner();
    //         }
    //     }
    //     // unknown goal pos
    //     else if (Localistation::w_Goal_Position[0] < 0 | Localistation::w_Goal_Position[1] < 0) {
    //         // search for it
    //         // TODO // weighted_search();
    //     }
    // }

    return 0;
}


// // TODO
// // weighted_search() {
// // take the grid map and perform a weighted gradient over the map
// // output a goal position to send to path planner

// #define OBSTACLE 1
// #define GRID_X 60  // TODO these are defined already in localisation
// #define GRID_Y 60  // TODO these are defined already in localisation

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

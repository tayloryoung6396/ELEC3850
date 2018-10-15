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

    Localisation::w_Goal_Position[0] = 1;  // TODO remove
    Localisation::w_Goal_Position[1] = 0;  // TODO remove

    SimplePathPlanner();  // TODO remove

    MotorController();  // TODO remove

    // // TODO this is not optimized in any way.. you're an idiot
    // // TODO Account for being at the object already/goal and needing to do something
    // if (!have_object) {
    //     // known object pos
    //     if (Localisation::w_Goal_Position[0] >= 0 && Localisation::w_Goal_Position[1] >= 0) {
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
    //     else if (Localisation::w_Goal_Position[0] < 0 | Localisation::w_Goal_Position[1] < 0) {
    //         // search for it
    //         // TODO // weighted_search();
    //     }
    // }

    // else if (have_object) {
    //     // known goal pos
    //     // If our goal is negative then it means we don't have a valid goal/don't know a goal
    //     if (Localisation::w_Goal_Position[0] >= 0 && Localisation::w_Goal_Position[1] >= 0) {
    //         // Check to see if we're at the goal or not
    //         if ((Localisation::w_Goal_Position[0] == Localisation::w_Tank_Position[0])
    //             && (Localisation::w_Goal_Position[1] == Localisation::w_Tank_Position[1])) {
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
    //     else if (Localisation::w_Goal_Position[0] < 0 | Localisation::w_Goal_Position[1] < 0) {
    //         // search for it
    //         // TODO // weighted_search();
    //     }
    // }

    return 0;
}

// TODO Account for the end being all 1
// void weighted_search(int Search_cell[]) {
//     int weighted_map = 0;
//     int prev_weight  = 0;

//     for (int rows = 0; rows < Map_m; rows++) {
//         for (int cols = 0; cols < Map_n; cols++) {
//             weighted_map = (int) Grid::map[rows - 1][cols - 1] * 10;
//             weighted_map += (int) Grid::map[rows - 1][cols] * 14;
//             weighted_map += (int) Grid::map[rows - 1][cols + 1] * 10;
//             weighted_map += (int) Grid::map[rows][cols - 1] * 14;
//             weighted_map += (int) Grid::map[rows][cols] * 20;
//             weighted_map += (int) Grid::map[rows][cols + 1] * 14;
//             weighted_map += (int) Grid::map[rows + 1][cols - 1] * 10;
//             weighted_map += (int) Grid::map[rows + 1][cols] * 14;
//             weighted_map += (int) Grid::map[rows + 1][cols + 1] * 10;
//             if (weighted_map > prev_weight) {
//                 Search_cell[0] = rows;
//                 Search_cell[0] = cols;
//             }
//         }
//     }
// }
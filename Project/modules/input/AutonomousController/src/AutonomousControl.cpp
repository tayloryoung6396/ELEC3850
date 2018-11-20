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

#define GOAL_CELL_X 0
#define GOAL_CELL_Y 0

// flags for conditions
bool AutoState::have_object       = FALSE;
bool AutoState::known_object      = FALSE;
bool AutoState::at_goal           = FALSE;
bool AutoState::known_goal        = FALSE;
bool AutoState::on_route          = FALSE;
bool AutoState::object_classified = FALSE;

void AutonomousControl_init() {
    std::cout << "Initilising AUTONOMOUS CONTROLLER" << std::endl;
    // Initialise
    // without the object
    // unknown goal
    // not at goal
}

int AutonomousControl_main() {

    // What do we want to do.. hence STATE MACHINE!

    // Do i have the object?
    if (AutoState::have_object) {
        // I have the object
        // Do i know where i am taking the object
        std::cout << "Have Object" << std::endl;
        if (AutoState::known_goal) {
            // I know where i am taking it
            // Am i on my way to the place?
            std::cout << "Known Goal" << std::endl;
            if (AutoState::on_route) {
                // Don't do anything just keep going
                std::cout << "On Route" << std::endl;
                // TODO Check our goal is still where we think it is
            }
            // Am i at the place?
            else if (AutoState::at_goal) {
                // I must be at the place
                // Lets place the object down
                std::cout << "At Goal" << std::endl;
                // IKGripper_Place_Object(Goal_pos);
            }
            else {
                // I'm not at the place
                // Lets find our way there
                std::cout << "Path Planning" << std::endl;
                // NOTE This should be set by the camera classifier / actually be a goal. But for now just return it to
                // a set location
                Localisation::w_Goal_Position[0] = GOAL_CELL_X;
                Localisation::w_Goal_Position[1] = GOAL_CELL_Y;
                SimplePathPlanner();
                MotorController();
            }
        }
        else {
            // No idea where to take it
            // Let's find where to take it
            // NOTE For now.. Spin to win

            std::cout << "Weighted Search" << std::endl;
            SpintoWin();
            MotorController();
        }
    }

    else {
        // I don't have the object
        // Do i know where an object is?
        std::cout << "Don't have object" << std::endl;
        if (AutoState::known_object) {
            // I know where an object is
            std::cout << "Known Object" << std::endl;
            if (AutoState::on_route) {
                // Don't do anything just keep going
                std::cout << "On Route" << std::endl;
                // TODO Check our goal is still where we think it is
            }
            // Am i at the object?
            else if (AutoState::at_goal) {
                // I must be at the object
                if (check_front_distance() > Kinematics::ultrasonic_offset[4] + 0.1) {
                    // Lets pick it up
                    std::cout << "At Goal" << std::endl;
                    // IKGripper_Grab_Object(Goal_pos);
                }
                else {
                    // TODO Correct position
                    std::cout << "Object too far" << std::endl;
                }
            }
            else {
                // I'm not at the object
                // Lets find our way there
                std::cout << "Path Planning" << std::endl;
                // The distance is found in the classifier and transformed to world coordinates

                find_distance(Classifier::object[0][0], Classifier::object[0][1]);

                // Our path should just be a rotation and forward movement
                SimplePathPlanner();
                MotorController();
            }
        }
        else {
		if(AutoState::on_route){}
		else{
            // No idea where the object is
            // Lets search for one
            // NOTE For now.. Spin to win!
            std::cout << "Weighted Search" << std::endl;
            SpintoWin();
	    //SimplePathPlanner();
            MotorController();
        }
}
    }

    return 0;
}

// void weighted_search() {
//     // Not so weighted search
//     // Grid::map[Grid::m][Grid::n

//     int x_pos = ;  // Cell we are currently in
//     int y_pos = ;  // Cell we are currently in

//     int x_step = 1;
//     int y_step = 1;
//     // Check until we find a cell that hasn't been seen
//     while (Grid::map[][] != 0.5) {
//         for (x = x_pos; x < x_pos + x_step; x++) {
//             // if () }
//         }
//     }
// }

void SpintoWin() {
    // Set our goal to be a 45 degree rotation
    Localisation::w_Goal_Position[0] = 0;
    Localisation::w_Goal_Position[1] = 45 * M_PI / 180.0;
    
    PathPlanner pplanner;

    pplanner.emplace_path(0, 45 * M_PI / 180.0);
    if (AutoState::have_object) {
        AutoState::known_goal = FALSE;
        std::cout << "AutoState::known_goal = FALSE" << std::endl;
    }
    else {
        AutoState::known_object = FALSE;
        std::cout << "AutoState::known_object = FALSE" << std::endl;
    }
    AutoState::on_route = TRUE;
    std::cout << "AutoState::on_route = TRUE" << std::endl;
}

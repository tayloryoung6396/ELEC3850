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

// flags for conditions
bool AutoState::have_object  = FALSE;
bool AutoState::known_object = FALSE;
bool AutoState::at_goal      = FALSE;
bool AutoState::known_goal   = FALSE;
bool AutoState::on_route     = FALSE;

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
            }
            // Am i at the place?
            else if (AutoState::at_goal) {
                // I must be at the place
                // Lets place the object down
                std::cout << "At Goal" << std::endl;
                IKGripper_Place_Object(Goal_pos);
            }
            else {
                // I'm not at the place
                // Lets find our way there
                std::cout << "Path Planning" << std::endl;
                // TODO This should be set by the camera classifier
                Localisation::w_Goal_Position[0] = 1;
                Localisation::w_Goal_Position[1] = 0;
                SimplePathPlanner();
                AutoState::on_route = TRUE;
            }
        }
        else {
            // No idea where to take it
            // Let's find where to take it
            std::cout << "Weighted Search" << std::endl;
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
            }
            // Am i at the object?
            else if (AutoState::at_goal) {
                // I must be at the object
                // Lets pick it up
                std::cout << "At Goal" << std::endl;
                IKGripper_Grab_Object(Goal_pos);
            }
            else {
                // I'm not at the object
                // Lets find our way there
                std::cout << "Path Planning" << std::endl;
                // TODO This should be set by the camera classifier
                Localisation::w_Goal_Position[0] = 1;
                Localisation::w_Goal_Position[1] = 0;
                SimplePathPlanner();
                AutoState::on_route = TRUE;
            }
        }
        else {
            // No idea where the object is
            // Lets search for one
            // TODO
            std::cout << "Weighted Search" << std::endl;
        }
    }

    return 0;
}

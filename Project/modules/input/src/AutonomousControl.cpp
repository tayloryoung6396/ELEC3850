
#include "AutonomousControl.hpp"

void AutonomousControl_init() {
    printf("Test\n");
}

int AutonomousControl_main() {
    // There should be a state machine here,
    // States:
    // Searching
    // Collecting
    // Rescuing

    // These will go into the states
    // Figure out if there's stuff around me
    Localisation_main();
    // Decide how to move
    SimplePathPlanner_main();
    return 0;
}
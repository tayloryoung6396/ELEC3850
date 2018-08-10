/*
 * This should calculate how to move the motors given a movement command
 * Inputs:  Path to follow as some sort of vector function
 * Outputs: Motor Commands to HardwareIO
 */

#include "MotorController.hpp"

void MotorController_init() {
    std::cout << "Initilising MOTOR CONTROLLER" << std::endl;
}

// Convention here would say Forward is positive, Left Rotation is positive
int MotorController_main() {  // double Forward, double Rotation) {

    return 0;
}

double ConvertDistanceToRotation(double Goal_Dist) {

    return (DistToRev * Goal_Dist);
}
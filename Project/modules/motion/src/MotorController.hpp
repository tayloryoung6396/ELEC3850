#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

#include "../../main.hpp"

#define DistToRev 1  // Meters per rotation

extern int MotorController();
extern void MotorController_init();
extern int MotorDriver(double Forward, double Rotation);
extern double ConvertDistanceToRotation(double Goal_Dist);
extern double ConvertRotationToArclen(double Rotation);

#endif /* MOTOR_CONTROLLER_H_ */
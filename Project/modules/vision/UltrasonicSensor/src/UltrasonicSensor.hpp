#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

// LIBRARIES
#include "main.hpp"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

// DEFINITIONS
#define TRUE 1
#define SENSORS 4   // AMOUNT OF US SENSORS
#define READINGS 4  // AMOUNT OF EXPECTED READINGS FROM US SENSORS
#define TRIG 29     // TRIGGER PIN FOR US SENSORS
#define ECHO0 31    // ECHO PINS FOR US SENSORS (1-4)
#define ECHO1 33
#define ECHO2 35
#define ECHO3 37

// GLOBAL VARIABLES
int DETECTION[SENSORS];  // ARRAY TO STORE DISTANCE CALCULATIONS
int sensor = 0;        // START AT FIRST SENSOR
int LeftLim = 0.05;     // LEFT OBJECT AVOIDANCE LIMIT (5CM)
int RightLim = 0.05;     // RIGHT OBJECT AVOIDANCE LIMIT (5CM)
int BackLim = 0.1;      // BACK OBJECT AVOIDANCE LIMIT (10CM)
int LFlag = 0;        // FLAG FOR OBJECT DETECTION TO THE LEFT
int BFlag = 0;        // FLAG FOR OBJECT DETECTION TO THE BACK
int RFlag = 0;        // FLAG FOR OBJECT DETECTION TO THE RIGHT

unsigned long Start;         // START TIME (US)
unsigned short MaxedOut;     // PIN MAX
unsigned long sysStart;      // THE SYSTEM START TIME (US)
unsigned long end[SENSORS];  // ECHO END TIME (US)


extern void UltrasonicSensor_init();
extern int UltrasonicSensor_main();

#endif /* ULTRASONIC_SENSOR_H_ */
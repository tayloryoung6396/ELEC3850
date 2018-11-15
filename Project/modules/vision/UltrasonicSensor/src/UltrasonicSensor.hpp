#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

// LIBRARIES
#include "main.hpp"

extern void UltrasonicSensor_init();
extern int UltrasonicSensor_main();
extern void Sendpulse();
extern void DistanceM();
extern double check_front_distance();

#define SENSORS 4  // AMOUNT OF US SENSORS
#define TRIG 21    // TRIGGER PIN FOR US SENSORS
#define ECHO0 22   // Front
#define ECHO1 23   // Right
#define ECHO2 24   // Back
#define ECHO3 25   // Left
#define TIMEOUT 23000

class Ultrasonic {
public:
    static uint32_t Start_time;              // START TIME (US)
    static uint32_t sensor_return[SENSORS];  // ECHO END TIME (US)
    static double Detection_distances[SENSORS];
};

#endif /* ULTRASONIC_SENSOR_H_ */
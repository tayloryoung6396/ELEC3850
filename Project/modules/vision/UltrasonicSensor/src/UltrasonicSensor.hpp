#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

#include "main.hpp"

#define SENSORS 3   // AMOUNT OF US SENSORS
#define READINGS 3  // AMOUNT OF EXPECTED READINGS FROM US SENSORS
#define TRIG 29     // TRIGGER PIN FOR US SENSORS
#define ECHO0 31    // ECHO PINS FOR US SENSORS (1-3)
#define ECHO1 33
#define ECHO2 35

class Ultrasonic {
public:
    uint32_t Start_time;              // START TIME (US)
    uint32_t sensor_return[SENSORS];  // ECHO END TIME (US)
    double Detection_distances[SENSORS];
};

extern void UltrasonicSensor_init();
extern int UltrasonicSensor_main();

#endif /* ULTRASONIC_SENSOR_H_ */
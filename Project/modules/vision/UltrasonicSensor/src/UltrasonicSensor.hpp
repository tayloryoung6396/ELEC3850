#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

#include "main.hpp"

#define SENSORS 4  // AMOUNT OF US SENSORS
#define TRIG 21    // TRIGGER PIN FOR US SENSORS
#define ECHO0 22   // ECHO PINS FOR US SENSORS (1-3)
#define ECHO1 23
#define ECHO2 24
#define ECHO3 25
#define TIMEOUT 23000

class Ultrasonic {
public:
    static uint32_t Start_time;              // START TIME (US)
    static uint32_t sensor_return[SENSORS];  // ECHO END TIME (US)
    static double Detection_distances[SENSORS];
};

extern void UltrasonicSensor_init();
extern int UltrasonicSensor_main();
extern void Sendpulse();
extern void DistanceM();
extern void myInterrupt0(void);
extern void myInterrupt1(void);
extern void myInterrupt2(void);
extern void myInterrupt3(void);

#endif /* ULTRASONIC_SENSOR_H_ */

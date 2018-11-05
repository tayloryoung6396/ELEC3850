#ifndef INFRARED_SENSOR_H_
#define INFRARED_SENSOR_H_

#include "main.hpp"

#define IR_TRIG 4
// 4 is the IR trig
// this is 16 on the map

extern void InfraredSensor_init();
extern int InfraredSensor_main();


class IRsense {
public:
    static int IR_attempts;
    static int IR_sensor_return[IR_attempts];
    static double meanIR;
    static bool IRpresent;
};
#endif /* INFRARED_SENSOR_H_ */

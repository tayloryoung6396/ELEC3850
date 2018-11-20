#ifndef INFRARED_SENSOR_H_
#define INFRARED_SENSOR_H_

#include "main.hpp"

#define IR_ATTEMPTS 4
#define IRTRIG 26

extern void InfraredSensor_init();
extern int InfraredSensor_main();
extern void IRdetect();


class IRsense {
public:
    static int IR_attempts;
    static int IR_sensor_return[4];
    static double meanIR;
    static bool IRpresent;
};
#endif /* INFRARED_SENSOR_H_ */

#ifndef INFRARED_SENSOR_H_
#define INFRARED_SENSOR_H_

#include "main.hpp"

extern void InfraredSensor_init();
extern int InfraredSensor_main();


class IRsense {
public:
    int IR_attempts=3;
    int IR_sensor_return[IR_attempts];
    double meanIR;
    bool IRpresent;
};
#endif /* INFRARED_SENSOR_H_ */

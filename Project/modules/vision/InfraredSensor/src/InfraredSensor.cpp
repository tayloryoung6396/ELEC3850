/*
 * This should handle the initial processing of the IR images.
 * Inputs:  Camera Images - IR
 * Outputs:
 */

#include "InfraredSensor.hpp"

void InfraredSensor_init() {
    std::cout << "Initilising INFRARED SENSOR" << std::endl;
    // pinMode(IRTRIG, INPUT);  // IRTRIG IS AN INPUT FROM IR TO RPI
}

int InfraredSensor_main() {

    // TODO Should these not be digital reads?

    // digitalWrite(IR_TRIG, HIGH);
    // delayMicroseconds(10);  // 10us Delay
    // digitalWrite(IR_TRIG, LOW);

    return 0;
}

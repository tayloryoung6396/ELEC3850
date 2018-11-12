/*
 * This should handle the initial processing of the IR images.
 * Inputs:  Camera Images - IR
 * Outputs: TODO
 */

#include "InfraredSensor.hpp"

// Looking front on
// OUT, GND, VCC
// Physical pins
// 32,  34,  17

int IRsense::IR_attempts         = IR_ATTEMPTS;
int IRsense::IR_sensor_return[4] = {0};
double IRsense::meanIR           = 0;
bool IRsense::IRpresent          = FALSE;

void InfraredSensor_init() {
    std::cout << "Initilising INFRARED SENSOR" << std::endl;
    pinMode(IRTRIG, INPUT);  // IRTRIG IS AN INPUT FROM IR TO RPI
}

int InfraredSensor_main() {

    IRdetect();
    if (IRsense::meanIR >= 2) {
        IRsense::IRpresent = TRUE;
        std::cout << "IR Found object" << std::endl;
    }
    else {
        IRsense::IRpresent = FALSE;
    }

    return 0;
}


void IRdetect() {
    // Reset all of the times
    IRsense::IR_sensor_return[0] = -1;
    IRsense::IR_sensor_return[1] = -1;
    IRsense::IR_sensor_return[2] = -1;
    IRsense::IR_sensor_return[3] = -1;
    IRsense::meanIR              = 0;

    for (int sensor = 0; sensor < IRsense::IR_attempts; sensor++) {
        IRsense::IR_sensor_return[sensor] = digitalRead(IRTRIG);

        if (IRsense::IR_sensor_return[sensor] = -1) {
            std::cout << "Error w/ IR" << std::endl;
        }
        else
            IRsense::meanIR += IRsense::IR_sensor_return[sensor];
    }

    // there are 4 tests, if majority are true (1) then probs have an IR present.
    IRsense::meanIR = IRsense::meanIR / IRsense::IR_attempts;

    delayMicroseconds(5);  // chuck in a delay for good measure am i right
}

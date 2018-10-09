/*
 * This should handle the initial processing of the IR images.
 * Inputs:  Camera Images - IR
 * Outputs: TODO
 */

#include "InfraredSensor.hpp"
// LIBRARIES
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

#define IR_TRIG 4

//4 is the IR trig
//this is 16 on the map


void InfraredSensor_init() {
    std::cout << "Initilising INFRARED SENSOR" << std::endl;
    pinMode(IRTRIG, INPUT);  // IRTRIG IS AN INPUT FROM IR TO RPI

}

int InfraredSensor_main() {

    digitalWrite(IR_TRIG, HIGH);
    delayMicroseconds(10);  // 10us Delay
    digitalWrite(IR_TRIG, LOW);


    return 0;
}

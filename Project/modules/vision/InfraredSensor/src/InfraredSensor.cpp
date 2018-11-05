/*
 * This should handle the initial processing of the IR images.
 * Inputs:  Camera Images - IR
 * Outputs:
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
<<<<<<< HEAD
    pinMode(IRTRIG, INPUT);  // IRTRIG IS AN INPUT FROM IR TO RPI
=======
>>>>>>> test_branch_alex

}

int InfraredSensor_main() {

    digitalWrite(IR_TRIG, HIGH);
    delayMicroseconds(10);  // 10us Delay
    digitalWrite(IR_TRIG, LOW);


    // TODO Should these not be digital reads?

    // digitalWrite(IR_TRIG, HIGH);
    // delayMicroseconds(10);  // 10us Delay
    // digitalWrite(IR_TRIG, LOW);

    return 0;
}

    }




#include "InfraredSensor.hpp"

#define IRTRIG 4

//4 is the IR trig
//this is 16 on the map

int IRsense::IR_attempts = 3;
    std::cout << "Initilising INFRARED SENSOR" << std::endl;
    pinMode(IRTRIG, INPUT);  // IRTRIG IS AN INPUT FROM IR TO RPI
int InfraredSensor_main() {

    IRdetect();
    if (IRsense::meanIR>=2){
        IRsense::IRpresent= 1;
    }
    else {
        IRsense::IRpresent=0;
    }
}


void IRdetect() {
    // Reset all of the times
    IRsense::IR_sensor_return[0] =-1;
    IRsense::IR_sensor_return[1] =-1;
    IRsense::IR_sensor_return[2] =-1;
    IRsense::IR_sensor_return[3] =-1;
    IRsense::meanIR=0;

        for(int sensor = 0; sensor < IRsense::IR_attempts; sensor++)
        {
            IRsense::IR_sensor_return[sensor]=digitalRead(IR_TRIG);

            if (IRsense::IR_sensor_return[i]=-1)
            {
                std::cout << "Error w/ IR" <<std::endl; }
            else
                IRsense::meanIR+=IRsense::IR_sensor_return[sensor];
        }

    IRsense::meanIR=IRsense::meanIR/IRsense::IR_attempts;   //there are 4 tests, if majority are true (1) then probs have an IR present.

    delayMicroseconds(10);          //chuck in a delay for good measure am i right
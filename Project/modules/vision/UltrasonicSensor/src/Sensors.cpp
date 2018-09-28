// Sensors.cpp : Ultrasonic Sensors

/* This program is designed to determine if there are objects to the sides and rear of the
T-rex chassis using three stationary sensors. Currently the distance calaculations aren't
averaged between pulse sets to more accurately determine distance (distance function can
easily be altered to sample pulse readings and average the results).

The three sensors act on the same trigger but have three seperate echo pins. This can
be altered to have individual triggers for each sensor if required

This program can also be altered to include a fourth sensor to the front of the chassis
for object aquisition. An additional function in the program can also be added to sort
found objects by proximity. */

// LIBRARIES
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

// DEFINITIONS
#define SENSORS 1   // AMOUNT OF US SENSORS
#define READINGS 3  // AMOUNT OF EXPECTED READINGS FROM US SENSORS
#define TRIG 21     // TRIGGER PIN FOR US SENSORS
#define ECHO0 22    // ECHO PINS FOR US SENSORS (1-3)
#define ECHO1 23
#define ECHO2 24
#define ECHO3 25

// GLOBAL VARIABLES
int DETECTION[SENSORS];  // ARRAY TO STORE DISTANCE CALCULATIONS
int sensor   = 0;        // START AT FIRST SENSOR
int LeftLim  = 0.05;     // LEFT OBJECT AVOIDANCE LIMIT (5CM)
int RightLim = 0.05;     // RIGHT OBJECT AVOIDANCE LIMIT (5CM)
int BackLim  = 0.1;      // BACK OBJECT AVOIDANCE LIMIT (10CM)
int LFlag    = 0;        // FLAG FOR OBJECT DETECTION TO THE LEFT
int BFlag    = 0;        // FLAG FOR OBJECT DETECTION TO THE BACK
int RFlag    = 0;        // FLAG FOR OBJECT DETECTION TO THE RIGHT

unsigned long Start;         // START TIME (US)
unsigned short MaxedOut;     // PIN MAX
unsigned long sysStart;      // THE SYSTEM START TIME (US)
unsigned long end[SENSORS];  // ECHO END TIME (US)

// TIMING FUNCIONS
// unsigned int micros();
// void delay();
// void delayMicroseconds();

void myInterrupt1(void) {  // INTERRUPT FOR SENSOR 2
    std::cout << "INT 1" << std::endl;
    end[0] = micros();
}

// FUNCTIONS
void Setup() {
    // SET UP GPIO
    wiringPiSetup();

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO1, INPUT);

    // SET UP INTERRUPTS
    if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
        std::cout << "Unable to setup ISR 1: " << strerror(errno) << std::endl;
    }

    // Initialisations
    digitalWrite(TRIG, LOW);  // TRIGGER PIN MUST START LOW
    delayMicroseconds(60);    // DELAY TO ALLOW PINS TO SET

    return;
}

// SEND PULSE TO SENSORS
void Sendpulse() {
    // PULSE
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);  // 10us Delay
    digitalWrite(TRIG, LOW);

    // WAIT FOR ECHO START
    delayMicroseconds(400);

    // WAIT FOR ECHO END
    Start    = micros();
    MaxedOut = 0;
    end[0]   = 0;

    while ((end[0] == 0) && ((micros() - Start) < 23000)) {
    }
    return;
}

// SENSOR DISTANCE CALCULATION
void DistanceM() {
    Sendpulse();  // SEND PULSE
    sensor = 0;
    while (sensor <= 0) {
        long traveltime   = end[sensor] - Start;  // TIME CALCULATION
        int distance      = traveltime * 1.7;     // DISTANCE CALCULATION IN METRES
        DETECTION[sensor] = distance;             // SAVE SENSOR READINGS TO ARRAY
        sensor++;
    }
    printf("Out of while\n");
    return;
}


int main() {
    Setup();              // SET UP GPIO PINS
    sysStart = micros();  // START SYSTEM


    while (1) {

        delayMicroseconds(60);  // DELAY BETWEEN READINGS
        DistanceM();            // DISTANCE DETECTION
        printf("Out of DistanceM\n");
        if (DETECTION[0] <= RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            RFlag = 1;                   // SET OBJECT DETECTION FLAG
        }

        if (DETECTION[0] > RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            RFlag = 0;                  // RESET OBJECT DETECTION FLAG
        }


        // DEBUG DISTANCE CALCULATIONS
        printf("Right Sensor Distance \t %d m\n", DETECTION[0]);
    }

    return 0;
}

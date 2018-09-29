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
#include <iostream>

// DEFINITIONS
#define SENSORS 2   // AMOUNT OF US SENSORS
#define READINGS 3  // AMOUNT OF EXPECTED READINGS FROM US SENSORS
#define TRIG 21     // TRIGGER PIN FOR US SENSORS
#define ECHO0 22    // ECHO PINS FOR US SENSORS (1-3)
#define ECHO1 23
#define ECHO2 24
#define ECHO3 25

// GLOBAL VARIABLES
double DETECTION[SENSORS];  // ARRAY TO STORE DISTANCE CALCULATIONS
int sensor   = 0;           // START AT FIRST SENSOR
int LeftLim  = 0.05;        // LEFT OBJECT AVOIDANCE LIMIT (5CM)
int RightLim = 0.05;        // RIGHT OBJECT AVOIDANCE LIMIT (5CM)
int BackLim  = 0.1;         // BACK OBJECT AVOIDANCE LIMIT (10CM)
int LFlag    = 0;           // FLAG FOR OBJECT DETECTION TO THE LEFT
int BFlag    = 0;           // FLAG FOR OBJECT DETECTION TO THE BACK
int RFlag    = 0;           // FLAG FOR OBJECT DETECTION TO THE RIGHT

unsigned long Start;         // START TIME (US)
unsigned short MaxedOut;     // PIN MAX
unsigned long sysStart;      // THE SYSTEM START TIME (US)
unsigned long end[SENSORS];  // ECHO END TIME (US)

// TIMING FUNCIONS
// unsigned int micros();
// void delay();
// void delayMicroseconds();

void myInterrupt0(void) {  // INTERRUPT FOR SENSOR 2
    std::cout << "INT 0" << std::endl;
    end[0] = micros();
}

void myInterrupt1(void) {  // INTERRUPT FOR SENSOR 2
    std::cout << "INT 1" << std::endl;
    end[1] = micros();
}

// FUNCTIONS
void Setup() {
    // SET UP GPIO
    wiringPiSetup();

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO0, INPUT);
    pinMode(ECHO1, INPUT);

    // SET UP INTERRUPTS
    if (wiringPiISR(ECHO0, INT_EDGE_FALLING, &myInterrupt0) < 0) {
        std::cout << "Unable to setup ISR 0: " << strerror(errno) << std::endl;
    }
    else if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
        std::cout << "Unable to setup ISR 1: " << strerror(errno) << std::endl;
    }

    // Initialisations
    digitalWrite(TRIG, LOW);  // TRIGGER PIN MUST START LOW
    delayMicroseconds(60);    // DELAY TO ALLOW PINS TO SET
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
    end[1]   = 0;

    while ((end[0] == 0 || end[1] == 0) && ((micros() - Start) < 23000)) {
    }
}

// SENSOR DISTANCE CALCULATION
void DistanceM() {
    Sendpulse();  // SEND PULSE
    sensor = 0;
    while (sensor <= 1) {
        long traveltime   = end[sensor] - Start;   // TIME CALCULATION
        double distance   = traveltime * 0.00017;  // DISTANCE CALCULATION IN METRES
        DETECTION[sensor] = distance;              // SAVE SENSOR READINGS TO ARRAY
        sensor++;
    }
}


int main() {
    Setup();              // SET UP GPIO PINS
    sysStart = micros();  // START SYSTEM


    while (1) {

        delayMicroseconds(60);  // DELAY BETWEEN READINGS
        DistanceM();            // DISTANCE DETECTION

        // DEBUG DISTANCE CALCULATIONS
        printf("Sensor 0 Distance \t %lf m\n", DETECTION[0]);
        printf("Sensor 1 Distance \t %lf m\n", DETECTION[1]);
    }

    return 0;
}

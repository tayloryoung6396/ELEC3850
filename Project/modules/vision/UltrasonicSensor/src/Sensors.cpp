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
#define SENSORS 4   // AMOUNT OF US SENSORS
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

// Interrupts
void myInterrupt0(void) {  // INTERRUPT FOR SENSOR 1
    end[0] = micros();
}

void myInterrupt1(void) {  // INTERRUPT FOR SENSOR 2
    end[1] = micros();
}

void myInterrupt2(void) {  // INTERRUPT FOR SENSOR 3
    end[2] = micros();
}

// FUNCTIONS
void Setup() {
    // SET UP GPIO
    wiringPiSetupGpio();  // CHECK CORRECT WIRING PI SETUP FUNCTION

    // DEFINE GPIO PINS FOR I/O MODE
    pinMode(TRIG, OUTPUT);  // TRIGGER IS AN OUTPUT FOR RPI
    pinMode(ECHO0, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 1
    pinMode(ECHO1, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 2
    pinMode(ECHO2, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 3

    // SET UP INTERRUPTS
    //    if (wiringPiISR(ECHO0, INT_EDGE_FALLING, &myInterrupt0) < 0) {
    //       fprintf(stderr, "Unable to setup ISR 0: %s\n", strerror(errno));
    //   }

    // if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
    //   fprintf(stderr, "Unable to setup ISR 1: %s\n", strerror(errno));
    //  }

    //  if (wiringPiISR(ECHO2, INT_EDGE_FALLING, &myInterrupt2) < 0) {
    //      fprintf(stderr, "Unable to setup ISR 2: %s\n", strerror(errno));
    //  }

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
    end[1]   = 0;
    end[2]   = 0;

    while ((end[0] == 0 || end[1] == 0 || end[2] == 0) && ((micros() - Start) < 23000)) {
        delayMicroseconds(100);
    }
    return;
}

// SENSOR DISTANCE CALCULATION
void DistanceM() {
    Sendpulse();  // SEND PULSE
    sensor = 0;
    while (sensor <= 2) {
        long traveltime   = end[sensor] - Start;  // TIME CALCULATION
        int distance      = traveltime * 1.7;     // DISTANCE CALCULATION IN METRES
        DETECTION[sensor] = distance;             // SAVE SENSOR READINGS TO ARRAY
        sensor++;
    }
    printf("Out of while");
    sensor = 0;  // RESET SENSOR STARTING POINT
    return;
}


int main() {
    Setup();              // SET UP GPIO PINS
    sysStart = micros();  // START SYSTEM


    while (1) {

        delayMicroseconds(60);  // DELAY BETWEEN READINGS
        DistanceM();            // DISTANCE DETECTION
        printf("Out of DistanceM");
        if (DETECTION[0] <= RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            RFlag = 1;                   // SET OBJECT DETECTION FLAG
        }

        if (DETECTION[0] > RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            RFlag = 0;                  // RESET OBJECT DETECTION FLAG
        }

        if (DETECTION[1] <= BackLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            BFlag = 1;                  // SET OBJECT DETECTION FLAG
        }

        if (DETECTION[1] > BackLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            BFlag = 0;                 // RESET OBJECT DETECTION FLAG
        }

        if (DETECTION[2] <= LeftLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            LFlag = 1;                  // SET OBJECT DETECTION FLAG
        }

        if (DETECTION[2] > LeftLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
            LFlag = 0;                 // RESET OBJECT DETECTION FLAG
        }

        // DEBUG DISTANCE CALCULATIONS
        printf(
            "Right Sensor Distance \t %d \t m \n Back Sensor Distance \t %d \t m\n Left Sensor Distance \t %d \t m \n",
            DETECTION[0],
            DETECTION[1],
            DETECTION[2]);

        // DEBUG OBJECT DETECTION FLAGS
        printf("\n Right flag status \t %d \n Back flag status \t %d \n Left flag status \t %d", RFlag, BFlag, LFlag);
    }

    return 0;
}

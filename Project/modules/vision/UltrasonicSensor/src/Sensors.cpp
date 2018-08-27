// Sensors.cpp : Ultrasonic Sensors

//LIBRARIES
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

//DEFINITIONS
#define TRUE 1
#define SENSORS 3
#define READINGS 3
#define TRIG 23			//TRIGGER PIN (NEED TO CHECK CORRECT PIN FOR RPI3 B+
#define ECHO1 24		//ECHO PINS FOR SENSORS (CHECK PIN AVAILABILITY)
#define ECHO2 25
#define ECHO3 16


//GLOBAL VARIABLES
int DETECTION[SENSORS];
int sensor = 1;						//START AT FIRST SENSOR 
int LeftLim = 0.05;					//LEFT OBJECT AVOIDANCE LIMIT (5CM)
int RightLim = 0.05;				//RIGHT OBJECT AVOIDANCE LIMIT (5CM)
int BackLim = 0.1;					//BACK OBJECT AVOIDANCE LIMIT (10CM)
int RFlag = 0;						//FLAG FOR OBJECT DETECTION TO THE RIGHT
int LFlag = 0;						//FLAG FOR OBJECT DETECTION TO THE LEFT
int BFlag = 0;						//FLAG FOR OBJECT DETECTION TO THE BACK 

unsigned long Start;				//START TIME (US)
unsigned short MaxedOut;			//PIN MAX
unsigned long sysStart;				//THE SYSTEM START TIME (US)
unsigned long end[SENSORS];			//ECHO END TIME (US)

//CHECK WIRING PI TIMING FUNCION REQUIREMENTS
unsigned int micros();
void delay(); 
void delayMicroseconds();

//Interrupts
void myInterrupt1(void) {		//Interrupt for sensor 1
		end[0] = micros();
}

void myInterrupt2(void) {		//Interrupt for sensor 2
		end[1] = micros();
}

void myInterrupt3(void) {		//Interrupt for sensor 3
		end[2] = micros();
}

//FUNCTIONS
void Setup() {
	//SET UP THE LIBRARY FOR GPIO
	wiringPiSetupGpio();				//CHECK CORRECT WIRING PI SETUP FUNCTION

	//SET UP GPIO PINS FOR I/O MODE
	pinMode(TRIG, OUTPUT);				//MAY NEED FUNCTION VOID PINMODE(INT INPUT, INT OUTPUT)
	pinMode(ECHO1, INPUT);
	pinMode(ECHO2, INPUT);
	pinMode(ECHO3, INPUT);

	//SET UP INTERRUPTS
	if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
		fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
	}

	if (wiringPiISR(ECHO2, INT_EDGE_FALLING, &myInterrupt2) < 0) {
		fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
	}

	if (wiringPiISR(ECHO3, INT_EDGE_FALLING, &myInterrupt3) < 0) {
		fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
	}

	//TRIG PIN 
	digitalWrite(TRIG, LOW);		//TRIG PIN MUST START LOW

	//PIN SET DELAY 
	delayMicroseconds(60);			//DELAY TO ALLOW PINS TO SET
}

//SEND TRIG PULSE 
void Sendpulse() {
	//PULSE
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);				//10us Delay
	digitalWrite(TRIG, LOW);
		
	//WAIT FOR ECHO TO START
	delayMicroseconds(400);

	//WAIT FOR ECHO TO END
	Start = micros();
	MaxedOut = 0;
	end[0] = 0;
	end[1] = 0;
	end[2] = 0;

	while ((end[0] == 0 || end[1] == 0 || end[2] == 0) && (MaxedOut = (micros() - Start)) < 23000) {
		delayMircoseconds(100);
	}
}

//DISTANCE CALCULATION
int DistanceM() {
	Sendpulse();

	while (sensor <= 3) {
		long traveltime = end[sensor] - Start;
		int distance = traveltime * 1.7;			//DISTANCE IN METRES
		DETECTION[sensor] = distance;
		sensor++;
	}

	sensor = 0;
	return;
}


int main()
{
	Setup();								//SET UP GPIO PINS
	sysStart = micros();						//START SYSTEM
											
	
	while (1) {
		delayMicroseconds(60);					//DELAY BETWEEN READINGS 
		DistanceM();									//DISTANCE DETECTION

		if (DETECTION[1] == RightLim) {		//CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
			RFlag = 1;								//SET OBJECT DETECTION FLAG
		}

		if (DETECTION[2] == BackLim) {		//CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
			BFlag = 1;								//SET OBJECT DETECTION FLAG
		}

		if (DETECTION[3] == LeftLim) {		//CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
			LFlag = 1;								//SET OBJECT DETECTION FLAG 
		}
		
	}
	
    return 0;
}


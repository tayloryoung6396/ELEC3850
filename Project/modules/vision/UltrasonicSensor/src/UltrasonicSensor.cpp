/*
 * This should handle the initial processing of the IR images.
 * Inputs:  Camera Images - IR
 * Outputs: TODO
 */

#include "UltrasonicSensor.hpp"

void UltrasonicSensor_init() {
    std::cout << "Initilising ULTRASONIC SENSOR" << std::endl;
   
	// SET UP GPIO
		wiringPiSetupGpio();  // CHECK CORRECT WIRING PI SETUP FUNCTION

							  // DEFINE GPIO PINS FOR I/O MODE
		pinMode(TRIG, OUTPUT);  // TRIGGER IS AN OUTPUT FOR RPI
		pinMode(ECHO0, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 1
		pinMode(ECHO1, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 2
		pinMode(ECHO2, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 3
		pinMode(ECHO3, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 3

								// SET UP INTERRUPTS
		if (wiringPiISR(ECHO0, INT_EDGE_FALLING, &myInterrupt0) < 0) {
			fprintf(stderr, "Unable to setup ISR 0: %s\n", strerror(errno));
		}

		if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
			fprintf(stderr, "Unable to setup ISR 1: %s\n", strerror(errno));
		}

		if (wiringPiISR(ECHO2, INT_EDGE_FALLING, &myInterrupt2) < 0) {
			fprintf(stderr, "Unable to setup ISR 2: %s\n", strerror(errno));
		}

		if (wiringPiISR(ECHO3, INT_EDGE_FALLING, &myInterrupt3) < 0) {
			fprintf(stderr, "Unable to setup ISR 3: %s\n", strerror(errno));
		}

		// Initialisations
		digitalWrite(TRIG, LOW);  // TRIGGER PIN MUST START LOW
		delayMicroseconds(60);    // DELAY TO ALLOW PINS TO SET

		return;
	
}

int UltrasonicSensor_main() {
	UltrasonicSensor_in();              // SET UP GPIO PINS
	sysStart = micros();			// START SYSTEM


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
		printf("Right Sensor Distance \t %d \t m \n Back Sensor Distance \t %d \t m\n Left Sensor Distance \t %d \t m \n",
			DETECTION[0],
			DETECTION[1],
			DETECTION[2],
			DETECTION[3]);

		// DEBUG OBJECT DETECTION FLAGS
		printf("\n Right flag status \t %d \n Back flag status \t %d \n Left flag status \t %d", RFlag, BFlag, LFlag);
	}

	return 0;
}
    

void Sendpulse() {
	// PULSE
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);  // 10us Delay
	digitalWrite(TRIG, LOW);

	// WAIT FOR ECHO START
	delayMicroseconds(400);

	// WAIT FOR ECHO END
	Start = micros();
	MaxedOut = 0;
	end[0] = 0;
	end[1] = 0;
	end[2] = 0;
	end[3] = 0;

	while ((end[0] == 0 || end[1] == 0 || end[2] == 0 || end[3] == 0) && (MaxedOut = (micros() - Start)) < 23000) {
		delayMircoseconds(100);
	}
	return;
}

// SENSOR DISTANCE CALCULATION
int DistanceM() {

	Sendpulse();  // SEND PULSE

	while (sensor <= 4) {
		long traveltime = end[sensor] - Start;  // TIME CALCULATION
		int distance = traveltime * 1.7;     // DISTANCE CALCULATION IN METRES
		DETECTION[sensor] = distance;             // SAVE SENSOR READINGS TO ARRAY
		sensor++;
	}
	printf("Out of while");
	sensor = 0;  // RESET SENSOR STARTING POINT
	return;
}

void myInterrupt0(void) {  // INTERRUPT FOR SENSOR 1
	end[0] = micros();
}

void myInterrupt1(void) {  // INTERRUPT FOR SENSOR 2
	end[1] = micros();
}

void myInterrupt2(void) {  // INTERRUPT FOR SENSOR 3
	end[2] = micros();
}

void myInterrupt4(void) {  // INTERRUPT FOR SENSOR 4
	end[3] = micros();
}
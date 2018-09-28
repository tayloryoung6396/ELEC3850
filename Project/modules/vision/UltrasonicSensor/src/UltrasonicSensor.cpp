/*
 * This should handle the initial processing of the IR images.
 * Inputs:  Camera Images - IR
 * Outputs: TODO
 */

#include "UltrasonicSensor.hpp"


void UltrasonicSensor_init() {
    std::cout << "Initilising ULTRASONIC SENSOR" << std::endl;
    // Here we setup the gpio pins
    // Set inputs/outputs
    // Set and ISR functions etc.

    // DEFINE GPIO PINS FOR I/O MODE
    pinMode(TRIG, OUTPUT);  // TRIGGER IS AN OUTPUT FOR RPI
    pinMode(ECHO0, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 1
    pinMode(ECHO1, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 2
    pinMode(ECHO2, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 3

    // SET UP INTERRUPTS
    if (wiringPiISR(ECHO0, INT_EDGE_FALLING, &myInterrupt0) < 0) {
        std::cout << "Unable to setup ISR 0: " << strerror(errno) << std::endl;
    }

    if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
        std::cout << "Unable to setup ISR 1: " << strerror(errno) << std::endl;
    }

    if (wiringPiISR(ECHO2, INT_EDGE_FALLING, &myInterrupt2) < 0) {
        std::cout << "Unable to setup ISR 2: " << strerror(errno) << std::endl;
    }

    // Initialisations
    digitalWrite(TRIG, LOW);  // TRIGGER PIN MUST START LOW
    delayMicroseconds(60);    // DELAY TO ALLOW PINS TO SET
}

int UltrasonicSensor_main() {
    // Send a pulse out
    // Wait for a timeout
    // Convert distances to nice unit
    // Store the distances in some sort of array
    // Probably some sort of relative position to the localisation
    // Stuff

    int LFlag    = 0;     // FLAG FOR OBJECT DETECTION TO THE LEFT
    int BFlag    = 0;     // FLAG FOR OBJECT DETECTION TO THE BACK
    int RFlag    = 0;     // FLAG FOR OBJECT DETECTION TO THE RIGHT
    int LeftLim  = 0.05;  // LEFT OBJECT AVOIDANCE LIMIT (5CM)
    int RightLim = 0.05;  // RIGHT OBJECT AVOIDANCE LIMIT (5CM)
    int BackLim  = 0.1;   // BACK OBJECT AVOIDANCE LIMIT (10CM)


    delayMicroseconds(60);  // DELAY BETWEEN READINGS
    Sendpulse();            // SEND PULSE
    DistanceM();            // DISTANCE DETECTION
    printf("Out of DistanceM");
    if (Detection_distances[0] <= RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        RFlag = 1;                             // SET OBJECT DETECTION FLAG
    }

    else if (Detection_distances[0] > RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        RFlag = 0;                                 // RESET OBJECT DETECTION FLAG
    }

    if (Detection_distances[1] <= BackLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        BFlag = 1;                            // SET OBJECT DETECTION FLAG
    }

    else if (Detection_distances[1] > BackLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        BFlag = 0;                                // RESET OBJECT DETECTION FLAG
    }

    if (Detection_distances[2] <= LeftLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        LFlag = 1;                            // SET OBJECT DETECTION FLAG
    }

    else if (Detection_distances[2] > LeftLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        LFlag = 0;                                // RESET OBJECT DETECTION FLAG
    }

    // DEBUG DISTANCE CALCULATIONS
    std::cout << "Right Sensor Distance " << Detection_distances[0] << " m"
              << " Flag " << RFlag << std::endl;
    std::cout << "Back Sensor Distance " << Detection_distances[1] << " m"
              << " Flag " << BFlag << std::endl;
    std::cout << "Left Sensor Distance " << Detection_distances[2] << " m"
              << " Flag " << LFlag << std::endl;
    return 0;
}

// Interrupts
void myInterrupt0(void) {  // INTERRUPT FOR SENSOR 1
    Ultrasonic::sensor_return[0] = micros();
}

void myInterrupt1(void) {  // INTERRUPT FOR SENSOR 2
    Ultrasonic::sensor_return[1] = micros();
}

void myInterrupt2(void) {  // INTERRUPT FOR SENSOR 3
    Ultrasonic::sensor_return[2] = micros();
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
    Ultrasonic::Start_time       = micros();
    Ultrasonic::sensor_return[0] = 0;
    Ultrasonic::sensor_return[1] = 0;
    Ultrasonic::sensor_return[2] = 0;

    while ((Ultrasonic::sensor_return[0] == 0 || Ultrasonic::sensor_return[1] == 0 || Ultrasonic::sensor_return[2] == 0)
           && ((micros() - Ultrasonic::Start_time) < 23000)) {
        delayMicroseconds(100);
    }
}

// SENSOR DISTANCE CALCULATION
void DistanceM() {
    for (int sensor = 0; sensor < SENSOR; sensor++) {
        uint32_t traveltime         = Ultrasonic::sensor_return[sensor] - Ultrasonic::Start_time;  // TIME CALCULATION
        double distance             = traveltime * 1.7;  // DISTANCE CALCULATION IN METRES
        Detection_distances[sensor] = distance;          // SAVE SENSOR READINGS TO ARRAY
    }
}
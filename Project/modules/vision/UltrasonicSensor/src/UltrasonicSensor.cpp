/*
 * This should handle the initial processing of the IR images.
 * Inputs:  Camera Images - IR
 * Outputs: TODO
 */

#include "UltrasonicSensor.hpp"

uint32_t Ultrasonic::Start_time                 = 0;
uint32_t Ultrasonic::sensor_return[SENSORS]     = {0};
double Ultrasonic::Detection_distances[SENSORS] = {0};

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
    if (Ultrasonic::Detection_distances[0] <= RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        RFlag = 1;                                         // SET OBJECT DETECTION FLAG
    }

    else if (Ultrasonic::Detection_distances[0] > RightLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        RFlag = 0;                                             // RESET OBJECT DETECTION FLAG
    }

    if (Ultrasonic::Detection_distances[1] <= BackLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        BFlag = 1;                                        // SET OBJECT DETECTION FLAG
    }

    else if (Ultrasonic::Detection_distances[1] > BackLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        BFlag = 0;                                            // RESET OBJECT DETECTION FLAG
    }

    if (Ultrasonic::Detection_distances[2] <= LeftLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        LFlag = 1;                                        // SET OBJECT DETECTION FLAG
    }

    else if (Ultrasonic::Detection_distances[2] > LeftLim) {  // CHECK DISTANCES AGAINST MOVEMENT LIMITATIONS
        LFlag = 0;                                            // RESET OBJECT DETECTION FLAG
    }

    // DEBUG DISTANCE CALCULATIONS
    std::cout << "Right Sensor Distance " << Ultrasonic::Detection_distances[0] << " m"
              << " Flag " << RFlag << std::endl;
    std::cout << "Back Sensor Distance " << Ultrasonic::Detection_distances[1] << " m"
              << " Flag " << BFlag << std::endl;
    std::cout << "Left Sensor Distance " << Ultrasonic::Detection_distances[2] << " m"
              << " Flag " << LFlag << std::endl;
    return 0;
}

// SEND PULSE TO SENSORS
void Sendpulse() {
    for (int sensor = 0; sensor < SENSORS; sensor++) {
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);  // 10us Delay
        digitalWrite(TRIG, LOW);

        // WAIT FOR ECHO START
        delayMicroseconds(400);

        Ultrasonic::Start_time = micros();
        while (digitalRead(echo_pin[sensor]) == LOW && micros() - Ultrasonic::Start_time < TIMEOUT) {
            Ultrasonic::sensor_return[sensor] = micros() - Ultrasonic::Start_time;
        }
    }
}

// SENSOR DISTANCE CALCULATION
void DistanceM() {
    for (int sensor = 0; sensor < SENSORS; sensor++) {
        Ultrasonic::Detection_distances[sensor] =
            Ultrasonic::sensor_return[sensor] * 1.7;  // DISTANCE CALCULATION IN METRES
    }
}
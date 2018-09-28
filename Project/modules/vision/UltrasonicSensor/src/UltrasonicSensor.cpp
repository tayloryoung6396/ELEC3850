/*
 * This should handle the ultrasonic
 * Inputs:  US
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

    Sendpulse();  // SEND PULSE
    DistanceM();  // DISTANCE DETECTION

    // DEBUG DISTANCE CALCULATIONS
    std::cout << "Right Sensor Distance " << Ultrasonic::Detection_distances[0] << " m" << std::endl;
    std::cout << "Back Sensor Distance " << Ultrasonic::Detection_distances[1] << " m" << std::endl;
    std::cout << "Left Sensor Distance " << Ultrasonic::Detection_distances[2] << " m" << std::endl;
    return 0;
}

void Sendpulse() {
    int echo_pin[SENSORS] = {ECHO0, ECHO1, ECHO2};
    int trig_pin[SENSORS] = {TRIG, TRIG, TRIG};
    for (int sensor = 0; sensor < SENSORS; sensor++) {
        digitalWrite(trig_pin[sensor], HIGH);
        delayMicroseconds(10);  // 10us Delay
        digitalWrite(trig_pin[sensor], LOW);

        // WAIT FOR ECHO START
        delayMicroseconds(400);

        Ultrasonic::Start_time = micros();
        while (1) {
            if (micros() - Ultrasonic::Start_time < TIMEOUT) {
                break;
            }
            else if (digitalRead(echo_pin[sensor]) == HIGH) {
                Ultrasonic::sensor_return[sensor] = micros() - Ultrasonic::Start_time;
            }
        }

        std::cout << "Start time " << Ultrasonic::Start_time << " Return time " << Ultrasonic::sensor_return
                  << std::endl;
    }
}

void DistanceM() {
    for (int sensor = 0; sensor < SENSORS; sensor++) {
        Ultrasonic::Detection_distances[sensor] =
            Ultrasonic::sensor_return[sensor] * 1.7
            + Kinematics::ultrasonic_offset[sensor];  // DISTANCE CALCULATION IN METRES
    }
}

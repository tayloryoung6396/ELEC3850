/*
 * This should handle the ultrasonic
 * Inputs:  US
 * Outputs: TODO
 */

#include "UltrasonicSensor.hpp"

uint32_t Ultrasonic::Start_time                 = 0;
uint32_t Ultrasonic::sensor_return[SENSORS]     = {0};
double Ultrasonic::Detection_distances[SENSORS] = {0};

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

void myInterrupt3(void) {  // INTERRUPT FOR SENSOR 4
    Ultrasonic::sensor_return[3] = micros();
}


void UltrasonicSensor_init() {
    std::cout << "Initilising ULTRASONIC SENSOR" << std::endl;
    // Here we setup the gpio pins
    // Set inputs/outputs
    // Set and ISR functions etc.

    pinMode(TRIG, OUTPUT);  // TRIGGER IS AN OUTPUT FOR RPI
    pinMode(ECHO0, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 1
    pinMode(ECHO1, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 2
    pinMode(ECHO2, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 3
    pinMode(ECHO3, INPUT);  // ECH0 IS AN OUTPUT FOR RPI TO SENSOR 4

    // SET UP INTERRUPTS
    if (wiringPiISR(ECHO0, INT_EDGE_FALLING, &myInterrupt0) < 0) {
        std::cout << "ERROR: Unable to setup ISR 0: " << strerror(errno) << std::endl;
    }
    else if (wiringPiISR(ECHO1, INT_EDGE_FALLING, &myInterrupt1) < 0) {
        std::cout << "ERROR: Unable to setup ISR 1: " << strerror(errno) << std::endl;
    }
    else if (wiringPiISR(ECHO2, INT_EDGE_FALLING, &myInterrupt2) < 0) {
        std::cout << "ERROR: Unable to setup ISR 2: " << strerror(errno) << std::endl;
    }
    else if (wiringPiISR(ECHO3, INT_EDGE_FALLING, &myInterrupt3) < 0) {
        std::cout << "ERROR: Unable to setup ISR 3: " << strerror(errno) << std::endl;
    }

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
    // std::cout << "Sensor 0 'Front' Distance " << Ultrasonic::Detection_distances[0] << " m" << std::endl;
    // std::cout << "Sensor 1 'Right' Distance " << Ultrasonic::Detection_distances[1] << " m" << std::endl;
    // std::cout << "Sensor 2 'Back'  Distance " << Ultrasonic::Detection_distances[2] << " m" << std::endl;
    // std::cout << "Sensor 3 'Left'  Distance " << Ultrasonic::Detection_distances[3] << " m" << std::endl;
    return 0;
}

void Sendpulse() {
    // Reset all of the times
    Ultrasonic::sensor_return[0] = -1;
    Ultrasonic::sensor_return[1] = -1;
    Ultrasonic::sensor_return[2] = -1;
    Ultrasonic::sensor_return[3] = -1;

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(100);  // 10us Delay
    digitalWrite(TRIG, LOW);

    // WAIT FOR ECHO START
    delayMicroseconds(400);

    Ultrasonic::Start_time = micros();

    while ((Ultrasonic::sensor_return[0] == -1 || Ultrasonic::sensor_return[1] == -1
            || Ultrasonic::sensor_return[2] == -1 || Ultrasonic::sensor_return[3] == -1)
           && ((micros() - Ultrasonic::Start_time < TIMEOUT))) {
    }
}

void DistanceM() {
    for (int sensor = 0; sensor < SENSORS; sensor++) {
        // Only calculate it if the distance was not a timeout
        if (Ultrasonic::sensor_return[sensor] != -1) {
            Ultrasonic::Detection_distances[sensor] =
                (Ultrasonic::sensor_return[sensor] - Ultrasonic::Start_time) * 0.00017
                + Kinematics::ultrasonic_offset[sensor];  // DISTANCE CALCULATION IN METRES
        }
        else {
            Ultrasonic::Detection_distances[sensor] = -1;
        }
    }
}

double check_front_distance() {
    Ultrasonic::sensor_return[0] = -1;

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(100);  // 10us Delay
    digitalWrite(TRIG, LOW);

    // WAIT FOR ECHO START
    delayMicroseconds(400);

    Ultrasonic::Start_time = micros();

    while ((Ultrasonic::sensor_return[0] == -1) && ((micros() - Ultrasonic::Start_time < TIMEOUT))) {
    }

    // Only calculate it if the distance was not a timeout
    if (Ultrasonic::sensor_return[0] != -1) {
        Ultrasonic::Detection_distances[0] = (Ultrasonic::sensor_return[0] - Ultrasonic::Start_time) * 0.00017
                                             + Kinematics::ultrasonic_offset[0];  // DISTANCE CALCULATION IN METRES
    }
    else {
        Ultrasonic::Detection_distances[0] = -1;
    }

    return Ultrasonic::Detection_distances[0];
}

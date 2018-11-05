#include "LedStatus.hpp"


void LedStatus_init() {
    std::cout << "Initilising LED LIGHTS" << std::endl;
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    Light_Sequence();
}

void Enable_RED() {
    digitalWrite(LED_RED, HIGH);
}

void Disable_RED() {
    digitalWrite(LED_RED, LOW);
}

void Enable_BLUE() {
    digitalWrite(LED_BLUE, HIGH);
}

void Disable_BLUE() {
    digitalWrite(LED_BLUE, LOW);
}

void Enable_GREEN() {
    digitalWrite(LED_GREEN, HIGH);
}

void Disable_GREEN() {
    digitalWrite(LED_GREEN, LOW);
}


void Light_Sequence() {

    Enable_RED();
    Delay(500);

    Disable_RED();

    Enable_BLUE();
    Delay(500);

    Disable_BLUE();

    Enable_GREEN();
    Delay(500);

    Disable_GREEN();
}

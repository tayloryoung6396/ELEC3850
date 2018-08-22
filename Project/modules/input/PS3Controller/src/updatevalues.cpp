#include "updates.h"  //include header file to update values

// this will ensure that any input that is read in
// is within the range of a short
short bound_short(int num) {
    if (num > SHRT_MAX)
        return SHRT_MAX;
    else if (num < SHRT_MIN)
        return SHRT_MIN;
    else
        return num;
}

// take input value and use this to map the output
short map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// map to the range of the short variable
short map(long x) {
    return (x - SHRT_MIN) * (2 * SHRT_MAX - 2 * SHRT_MIN) / (SHRT_MAX - SHRT_MIN) + 2 * SHRT_MIN;
}


// this function will check to see what is being pressed
void update_buttons(Joystick* joy) {
    ButtonId update    = joy->getUpdatedButton();
    unsigned int value = joy->getButtonState(update);

    switch (update) {
        case BUTTON_SELECT:
        case BUTTON_LEFT_JOYSTICK:
        case BUTTON_RIGHT_JOYSTICK:
        case BUTTON_START:
        case BUTTON_DPAD_UP:
        case BUTTON_DPAD_RIGHT:
        case BUTTON_DPAD_DOWN:
        case BUTTON_DPAD_LEFT:
        case BUTTON_LEFT_TRIGGER:
        case BUTTON_RIGHT_TRIGGER:
        case BUTTON_LEFT_BUMPER:
        case BUTTON_RIGHT_BUMPER:
        case BUTTON_TRIANGLE:
        case BUTTON_CIRCLE:
        case BUTTON_X:
        case BUTTON_SQUARE:
        case BUTTON_PS3:
        default: std::cout << "button: " << update << " value: " << value << std::endl; break;
    }
}

// this function will look at joystick direction
void update_axes(Joystick* joy) {
    AxisId update = joy->getUpdatedAxis();
    int16_t value = joy->getAxisState(update);

    switch (update) {

        case AXIS_LEFT_STICK_HORIZONTAL:
        case AXIS_LEFT_STICK_VERTICAL:
        case AXIS_RIGHT_STICK_HORIZONTAL:
        case AXIS_RIGHT_STICK_VERTICAL:

        case AXIS_TRIANGLE:
        case AXIS_CIRCLE:
        case AXIS_X:
        case AXIS_SQUARE:
        default: std::cout << "axis: " << update << " value: " << value << std::endl; break;
    }
}

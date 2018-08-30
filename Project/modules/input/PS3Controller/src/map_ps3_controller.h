#ifndef MAP_PS3_CONTROLLER_H_INCLUDED
#define MAP_PS3_CONTROLLER_H_INCLUDED

#pragma once

#define button_press 1
#define button_release 0

#define button_update 0x01  // Button state update
#define axis_update 0x02    // Axis state update
#define event_init 0x80     // non-input event- ignore

#define max_button_count 18  // total of 18 available controller buttons- i dont think we will use them all
#define max_axis_cound 27    // axis max is 27

// Axis definitions
// Assumption: right= positive direction
// Assumption: stick up= positive direction

enum AxisIdentification {
    AXIS_LEFT_STICK_HORIZONTAL,   // 0
    AXIS_LEFT_STICK_VERTICAL,     // 1
    AXIS_RIGHT_STICK_HORIZONTAL,  // 2
    AXIS_RIGHT_STICK_VERTICAL,    // 3

    AXIS_TRIANGLE,  // 16
    AXIS_CIRCLE,    // 17
    AXIS_X,         // 18
    AXIS_SQUARE,    // 19
};

#define axis_count 8

enum ButtonIdentification {
    BUTTON_SELECT,          //  0
    BUTTON_LEFT_JOYSTICK,   //  1
    BUTTON_RIGHT_JOYSTICK,  //  2
    BUTTON_START,           //  3
    BUTTON_DPAD_UP,         //  4
    BUTTON_DPAD_RIGHT,      //  5
    BUTTON_DPAD_DOWN,       //  6
    BUTTON_DPAD_LEFT,       //  7
    BUTTON_LEFT_TRIGGER,    //  8
    BUTTON_RIGHT_TRIGGER,   //  9
    BUTTON_LEFT_BUMPER,     // 10
    BUTTON_RIGHT_BUMPER,    // 11
    BUTTON_TRIANGLE,        // 12
    BUTTON_CIRCLE,          // 13
    BUTTON_X,               // 14
    BUTTON_SQUARE,          // 15
    BUTTON_PS3,             // 16
};

#define button_count 17  // how many are initialised

#endif  // MAP_PS3_CONTROLLER_H_INCLUDED

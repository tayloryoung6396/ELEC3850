/*
 * This file is part of PS3Walk.
 *
 * PS3Walk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PS3Walk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PS3Walk.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2013 NUbots <nubots@nubots.net>
 */

#ifndef PS3_CONTROL_H_
#define PS3_CONTROL_H_

#include "Joystick.h"
#include "main.hpp"

class PS3Walk {
public:
    // axes
    static constexpr uint32_t AXIS_LEFT_JOYSTICK_HORIZONTAL  = 0;
    static constexpr uint32_t AXIS_LEFT_JOYSTICK_VERTICAL    = 1;
    static constexpr uint32_t AXIS_RIGHT_JOYSTICK_HORIZONTAL = 2;
    static constexpr uint32_t AXIS_RIGHT_JOYSTICK_VERTICAL   = 3;
    static constexpr uint32_t AXIS_L2                        = 12;
    static constexpr uint32_t AXIS_R2                        = 13;
    static constexpr uint32_t AXIS_ACCEL_Y                   = 23;
    static constexpr uint32_t AXIS_ACCEL_X                   = 24;
    static constexpr uint32_t AXIS_ACCEL_Z                   = 25;

    // buttons
    static constexpr uint8_t BUTTON_SELECT         = 0;
    static constexpr uint8_t BUTTON_LEFT_JOYSTICK  = 1;
    static constexpr uint8_t BUTTON_RIGHT_JOYSTICK = 2;
    static constexpr uint8_t BUTTON_START          = 3;
    static constexpr uint8_t BUTTON_DPAD_UP        = 4;
    static constexpr uint8_t BUTTON_DPAD_RIGHT     = 5;
    static constexpr uint8_t BUTTON_DPAD_DOWN      = 6;
    static constexpr uint8_t BUTTON_DPAD_LEFT      = 7;
    static constexpr uint8_t BUTTON_L2             = 8;
    static constexpr uint8_t BUTTON_R2             = 9;
    static constexpr uint8_t BUTTON_L1             = 10;
    static constexpr uint8_t BUTTON_R1             = 11;
    static constexpr uint8_t BUTTON_TRIANGLE       = 12;
    static constexpr uint8_t BUTTON_CIRCLE         = 13;
    static constexpr uint8_t BUTTON_CROSS          = 14;
    static constexpr uint8_t BUTTON_SQUARE         = 15;
};

extern void PS3Control_init();
extern int PS3Control_main();

//Joystick joystick;  // TODO: make configurable

#endif /* PS3_CONTROL_H_ */

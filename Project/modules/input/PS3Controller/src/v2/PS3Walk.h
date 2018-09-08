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
    static constexpr uint32_t AXIS_RIGHT_JOYSTICK_HORIZONTAL = 3;
    static constexpr uint32_t AXIS_RIGHT_JOYSTICK_VERTICAL   = 4;
    static constexpr uint32_t AXIS_L2                        = 20;
    static constexpr uint32_t AXIS_R2                        = 21;
    static constexpr uint32_t AXIS_ACCEL_Y                   = 22;
    static constexpr uint32_t AXIS_ACCEL_X                   = 23;
    static constexpr uint32_t AXIS_ACCEL_Z                   = 24;

    // buttons
    static constexpr uint8_t BUTTON_SELECT         = 8;
    static constexpr uint8_t BUTTON_LEFT_JOYSTICK  = 11;
    static constexpr uint8_t BUTTON_RIGHT_JOYSTICK = 12;
    static constexpr uint8_t BUTTON_START          = 9;
    static constexpr uint8_t BUTTON_DPAD_UP        = 13;
    static constexpr uint8_t BUTTON_DPAD_RIGHT     = 16;
    static constexpr uint8_t BUTTON_DPAD_DOWN      = 14;
    static constexpr uint8_t BUTTON_DPAD_LEFT      = 15;
    static constexpr uint8_t BUTTON_L2             = 6;
    static constexpr uint8_t BUTTON_R2             = 7;
    static constexpr uint8_t BUTTON_L1             = 4;
    static constexpr uint8_t BUTTON_R1             = 5;
    static constexpr uint8_t BUTTON_TRIANGLE       = 2;
    static constexpr uint8_t BUTTON_CIRCLE         = 1;
    static constexpr uint8_t BUTTON_CROSS          = 0;
    static constexpr uint8_t BUTTON_SQUARE         = 3;
};

extern void PS3Control_init();
extern int PS3Control_main();

extern int32_t map_values(int32_t input,
                          int32_t output_end,
                          int32_t output_start,
                          int32_t input_end,
                          int32_t input_start);

static uint32_t axis_left_joystick_vertical    = 0;
static uint32_t axis_left_joystick_horizontal  = 0;
static uint32_t axis_right_joystick_vertical   = 0;
static uint32_t axis_right_joystick_horizontal = 0;
static uint8_t button_select                   = 0;
static uint8_t button_left_joystick            = 0;
static uint8_t button_right_joystick           = 0;
static uint8_t button_start                    = 0;
static uint8_t button_dpad_up                  = 0;
static uint8_t button_dpad_right               = 0;
static uint8_t button_dpad_down                = 0;
static uint8_t button_dpad_left                = 0;
static uint8_t button_l2                       = 0;
static uint8_t button_r2                       = 0;
static uint8_t button_l1                       = 0;
static uint8_t button_r1                       = 0;
static uint8_t button_triangle                 = 0;
static uint8_t button_circle                   = 0;
static uint8_t button_cross                    = 0;
static uint8_t button_square                   = 0;

// Joystick joystick;  // TODO: make configurable

#endif /* PS3_CONTROL_H_ */

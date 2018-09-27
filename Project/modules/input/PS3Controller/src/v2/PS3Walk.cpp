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

#include "PS3Walk.h"

#define AXIS_MAX_LIMIT 32768
#define AXIS_MIN_LIMIT -32767
#define FORWARD_MAX_LIMIT 100
#define FORWARD_MIN_LIMIT -100
#define ROTATION_MAX_LIMIT 100
#define ROTATION_MIN_LIMIT -100

class Joystick joystick;

void PS3Control_init() {
    std::cout << "Initilising PS3 CONTROLLER" << std::endl;
}

int PS3Control_main() {
    static double Forward  = 0;
    static double Rotation = 0;

    // TODO, I know this is not a good implementation, but time is short
    static double Gripper_Goal[3] = {Kinematics::grip_home[0], Kinematics::grip_home[1], Kinematics::grip_home[2]};

    JoystickEvent event;
    Input ip;
    // read from joystick
    if (joystick.sample(&event)) {
        if (event.isAxis() && !ip.Autonomous_Enabled) {
            // event was an axis event
            switch (event.number) {
                case PS3Walk::AXIS_LEFT_JOYSTICK_HORIZONTAL:
                    std::cout << "AXIS_LEFT_JOYSTICK_HORIZONTAL" << (int) event.value << std::endl;
                    axis_left_joystick_horizontal = (int) event.value;
                    // Call some function probably or something
                    // Call motor function
                    // Need some scaled values
                    Forward = -(double) map_values(axis_left_joystick_horizontal,
                                                   FORWARD_MAX_LIMIT,
                                                   FORWARD_MIN_LIMIT,
                                                   AXIS_MAX_LIMIT,
                                                   AXIS_MIN_LIMIT);
                    std::cout << "Forward " << Forward << " Rotation " << Rotation << std::endl;
                    MotorDriver_Velocity(Forward, Rotation);
                    break;
                case PS3Walk::AXIS_LEFT_JOYSTICK_VERTICAL:
                    std::cout << "AXIS_LEFT_JOYSTICK_VERTICAL" << (int) event.value << std::endl;
                    axis_left_joystick_vertical = (int) event.value;
                    // Call some function probably or something
                    // Call motor function
                    // Need some scaled values
                    Rotation = -(double) map_values(axis_left_joystick_vertical,
                                                    ROTATION_MAX_LIMIT,
                                                    ROTATION_MIN_LIMIT,
                                                    AXIS_MAX_LIMIT,
                                                    AXIS_MIN_LIMIT);
                    std::cout << "Forward " << Forward << " Rotation " << Rotation << std::endl;
                    MotorDriver_Velocity(Forward, Rotation);
                    break;
                case PS3Walk::AXIS_RIGHT_JOYSTICK_VERTICAL:
                    std::cout << "AXIS_RIGHT_JOYSTICK_VERTICAL" << (int) event.value << std::endl;
                    axis_right_joystick_vertical = (int) event.value;
                    // Call some function probably or something
                    // Gripper forward backwards
                    // Need some intermediate variable to be incrememted
                    // Move it positive
                    if (axis_right_joystick_vertical < 0) {
                        Gripper_Goal[1] += (double) axis_right_joystick_vertical / 10000.0;
                    }
                    else if (axis_right_joystick_vertical > 0) {
                        Gripper_Goal[1] -= (double) axis_right_joystick_vertical / 10000.0;
                    }
                    if (IKGripper_move(Gripper_Goal) != 0) {
                        std::cout << "ERROR: Could not move gripper" << std::endl;
                    }
                    std::cout << "Gripper set to" << Gripper_Goal[0] << " " << Gripper_Goal[1] << " " << Gripper_Goal[2]
                              << std::endl;
                    break;
                case PS3Walk::AXIS_RIGHT_JOYSTICK_HORIZONTAL:
                    std::cout << "AXIS_RIGHT_JOYSTICK_HORIZONTAL" << (int) event.value << std::endl;
                    axis_right_joystick_horizontal = (int) event.value;
                    // Call some function probably or something
                    // Gripper either left right or rotate
                    // Need some intermediate variable to be incrememted
                    // Move it positive
                    if (axis_right_joystick_horizontal < 0) {
                        Gripper_Goal[0] += (double) axis_right_joystick_horizontal / 10000.0;
                    }
                    else if (axis_right_joystick_horizontal > 0) {
                        Gripper_Goal[0] -= (double) axis_right_joystick_horizontal / 10000.0;
                    }
                    if (IKGripper_move(Gripper_Goal) != 0) {
                        std::cout << "ERROR: Could not move gripper" << std::endl;
                    }
                    std::cout << "Gripper set to" << Gripper_Goal[0] << " " << Gripper_Goal[1] << " " << Gripper_Goal[2]
                              << std::endl;
                    break;
            }
        }
        else if (event.isButton()) {
            // event was a button event
            switch (event.number) {
                case PS3Walk::BUTTON_SELECT:
                    button_select = (int) event.value;
                    // Call some function probably or something
                    // Change between autonomous and ps3
                    if (event.value > 0) {  // button down
                        std::cout << "BUTTON_SELECT" << std::endl;
                    }
                    break;
                case PS3Walk::BUTTON_LEFT_JOYSTICK:
                    if (!ip.Autonomous_Enabled) {
                        button_left_joystick = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_LEFT_JOYSTICK" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_RIGHT_JOYSTICK:
                    if (!ip.Autonomous_Enabled) {
                        button_right_joystick = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_RIGHT_JOYSTICK" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_START:
                    if (!ip.Autonomous_Enabled) {
                        button_start = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_START" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_DPAD_UP:
                    if (!ip.Autonomous_Enabled) {
                        button_dpad_up = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_DPAD_UP" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_DPAD_RIGHT:
                    if (!ip.Autonomous_Enabled) {
                        button_dpad_right = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_DPAD_RIGHT" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_DPAD_DOWN:
                    if (!ip.Autonomous_Enabled) {
                        button_dpad_down = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_DPAD_DOWN" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_DPAD_LEFT:
                    if (!ip.Autonomous_Enabled) {
                        button_dpad_left = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_DPAD_LEFT" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_L2:
                    if (!ip.Autonomous_Enabled) {
                        button_l2 = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_L2" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_R2:
                    if (!ip.Autonomous_Enabled) {
                        button_r2 = (int) event.value;
                        // Call some function probably or something
                        // Gripper up down
                        // Need some intermediate variable to be incrememted
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_R2" << std::endl;
                            Gripper_Goal[2] += 0.01;
                            if (IKGripper_move(Gripper_Goal) != 0) {
                                std::cout << "ERROR: Could not move gripper" << std::endl;
                            }
                            std::cout << "Gripper set to" << Gripper_Goal[0] << " " << Gripper_Goal[1] << " "
                                      << Gripper_Goal[2] << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_L1:
                    if (!ip.Autonomous_Enabled) {
                        button_l1 = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_L1" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_R1:
                    if (!ip.Autonomous_Enabled) {
                        button_r1 = (int) event.value;
                        // Call some function probably or something
                        // Gripper up down
                        // Need some intermediate variable to be incrememted
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_R1" << std::endl;
                            Gripper_Goal[2] -= 0.01;
                            if (IKGripper_move(Gripper_Goal) != 0) {
                                std::cout << "ERROR: Could not move gripper" << std::endl;
                            }
                            std::cout << "Gripper set to" << Gripper_Goal[0] << " " << Gripper_Goal[1] << " "
                                      << Gripper_Goal[2] << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_TRIANGLE:
                    if (!ip.Autonomous_Enabled) {
                        button_triangle = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_TRIANGLE" << std::endl;
                        }
                    }
                    break;
                case PS3Walk::BUTTON_CIRCLE:
                    if (!ip.Autonomous_Enabled) {
                        button_circle = (int) event.value;
                        // Call some function probably or something
                        // Grip open

                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_CIRCLE" << std::endl;
                            Open_Gripper();
                        }
                    }
                    break;
                case PS3Walk::BUTTON_CROSS:
                    if (!ip.Autonomous_Enabled) {
                        button_cross = (int) event.value;
                        // Call some function probably or something
                        // Grip close

                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_CROSS" << std::endl;
                            Close_Gripper();
                        }
                    }
                    break;
                case PS3Walk::BUTTON_SQUARE:
                    if (!ip.Autonomous_Enabled) {
                        button_square = (int) event.value;
                        // Call some function probably or something
                        if (event.value > 0) {  // button down
                            std::cout << "BUTTON_SQUARE" << std::endl;
                        }
                    }
                    break;
            }
        }
    }
    // If it's closed then we should try to reconnect
    else if (!joystick.valid()) {
        joystick.reconnect();
    }
}

int32_t map_values(int32_t input, int32_t output_end, int32_t output_start, int32_t input_end, int32_t input_start) {
    return (int32_t)((double) output_start
                     + (((double) (output_end - output_start)) / ((double) (input_end - input_start)))
                           * (double) ((input - input_start)));
}

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

#define FORWARD_SPEED 1   // TODO Change
#define ROTATION_SPEED 1  // TODO Change

class Joystick joystick;

void PS3Control_init() {
    std::cout << "Initilising PS3 CONTROLLER" << std::endl;
}

int PS3Control_main() {
    static int32_t A_L_J_H = 0;
    static int32_t A_L_J_V = 0;

    if (A_L_J_V > 100) {
        executeWriteSingle(Motor_L, MX64_ADDRESS_VALUE(GOAL_VELOCITY), -20);
        delay(10);
        executeWriteSingle(Motor_R, MX64_ADDRESS_VALUE(GOAL_VELOCITY), -20);
        delay(10);
    }
    else if (A_L_J_V < -100) {
        executeWriteSingle(Motor_L, MX64_ADDRESS_VALUE(GOAL_VELOCITY), 20);
        delay(10);
        executeWriteSingle(Motor_R, MX64_ADDRESS_VALUE(GOAL_VELOCITY), 20);
        delay(10);
    }


    JoystickEvent event;
    // read from joystick
    if (joystick.sample(&event)) {
        if (event.isAxis()) {
            // event was an axis event
            switch (event.number) {
                case PS3Walk::AXIS_LEFT_JOYSTICK_HORIZONTAL:
                    std::cout << "AXIS_LEFT_JOYSTICK_HORIZONTAL" << (int) event.value << std::endl;
                    break;
                case PS3Walk::AXIS_LEFT_JOYSTICK_VERTICAL:
                    std::cout << "AXIS_LEFT_JOYSTICK_VERTICAL" << (int) event.value << std::endl;
                    A_L_J_V = (int) event.value;
                    break;
                case PS3Walk::AXIS_RIGHT_JOYSTICK_VERTICAL:
                    std::cout << "AXIS_RIGHT_JOYSTICK_VERTICAL" << (int) event.value << std::endl;
                    break;
                case PS3Walk::AXIS_RIGHT_JOYSTICK_HORIZONTAL:
                    std::cout << "AXIS_RIGHT_JOYSTICK_HORIZONTAL" << (int) event.value << std::endl;
                    break;
            }
        }
        else if (event.isButton()) {
            // event was a button event
            switch (event.number) {
                case PS3Walk::BUTTON_TRIANGLE:
                    if (event.value > 0) {  // button down
                        std::cout << "BUTTON_TRIANGLE" << std::endl;
                    }
                    break;
                case PS3Walk::BUTTON_SQUARE:
                    if (event.value > 0) {  // button down
                        std::cout << "BUTTON_SQUARE" << std::endl;
                    }
                    break;
                    /*case BUTTON_L1:
                        if (event.value > 0) { // button down
                            NUClear::log("Requesting Left Side Kick");
                            emit(std::make_unique<KickScriptCommand>(KickScriptCommand{
                                {0, -1, 0}, // vector pointing right relative to robot
                                LimbID::LEFT_LEG
                            }));
                        }
                        break;
                    case BUTTON_L2:
                        if (event.value > 0) { // button down
                            NUClear::log("Requesting Left Front Kick");
                            emit(std::make_unique<KickScriptCommand>(KickScriptCommand{
                                {1, 0, 0}, // vector pointing forward relative to robot
                                LimbID::LEFT_LEG
                            }));
                        }
                        break;
                    case BUTTON_R1:
                        if (event.value > 0) { // button down
                            NUClear::log("Requesting Right Side Kick");
                            emit(std::make_unique<KickScriptCommand>(KickScriptCommand{
                                {0, 1, 0}, // vector pointing left relative to robot
                                LimbID::RIGHT_LEG
                            }));
                        }
                        break;
                    case BUTTON_R2:
                        if (event.value > 0) {  // button down
                            NUClear::log("Requesting Right Front Kick");
                            emit(std::make_unique<KickScriptCommand>(
                                KickScriptCommand(Eigen::Vector3d(1, 0, 0),  // vector pointing forward relative to
                    robot LimbID::RIGHT_LEG)));
                        }
                        break;*/
            }
        }
    }
    // If it's closed then we should try to reconnect
    else if (!joystick.valid()) {
        joystick.reconnect();
    }
}

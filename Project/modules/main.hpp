#ifndef MAIN_H_
#define MAIN_H_

#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "behaviour/src/SimplePathPlanner.hpp"
#include "dynamixel/v2/Dynamixel.hpp"
#include "dynamixel/v2/MX_28.hpp"
#include "dynamixel/v2/MX_64.hpp"
#include "input/src/AutonomousControl.hpp"
#include "input/src/PS3Control.hpp"
#include "localisation/src/Localisation.hpp"
#include "motion/src/IKGripper.hpp"
#include "motion/src/MotorController.hpp"
#include "platform/src/DynamixelProtocol.hpp"
#include "platform/src/Kinematics.hpp"
#include "platform/src/SensorFilter.hpp"
#include "platform/src/uart.hpp"
#include "vision/src/Camera.hpp"
#include "vision/src/Classifier.hpp"
#include "vision/src/IRCamera.hpp"

#define ADDRESS_VALUE(VAL) ADD_##VAL
#define SIZE_VALUE(VAL) SIZE_##VAL
#define MX28_ADDRESS_VALUE(VAL) ADDRESS_VALUE(MX28_##VAL)
#define MX28_SIZE_VALUE(VAL) SIZE_VALUE(MX28_##VAL)
#define MX64_ADDRESS_VALUE(VAL) ADDRESS_VALUE(MX64_##VAL)
#define MX64_SIZE_VALUE(VAL) SIZE_VALUE(MX64_##VAL)

#define DXL_MAKEWORD(a, b)                                                                                             \
    ((uint16_t)(((uint8_t)(((uint64_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint64_t)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)                                                                                            \
    ((uint32_t)(((uint16_t)(((uint64_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint64_t)(b)) & 0xffff))) << 16))


extern double getCurrentTime();
extern double getTimeSinceStart();

#endif /* MAIN_H_ */

#ifndef MAIN_H_
#define MAIN_H_

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

#define ADDRESS_VALUE(VAL) ADD_##VAL
#define SIZE_VALUE(VAL) SIZE_##VAL
#define MX28_ADDRESS_VALUE(VAL) ADDRESS_VALUE(MX28_##VAL)
#define MX28_SIZE_VALUE(VAL) SIZE_VALUE(MX28_##VAL)
#define MX64_ADDRESS_VALUE(VAL) ADDRESS_VALUE(MX64_##VAL)
#define MX64_SIZE_VALUE(VAL) SIZE_VALUE(MX64_##VAL)

#endif /* MAIN_H_ */
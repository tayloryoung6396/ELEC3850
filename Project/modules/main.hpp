#ifndef MAIN_H_
#define MAIN_H_

#include <errno.h>
#include <malloc.h>
#include <math.h>
#include <raspicam/raspicam.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include <cfloat>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <sstream>

#include "behaviour/src/SimplePathPlanner.hpp"
#include "dynamixel/v2/Dynamixel.hpp"
#include "dynamixel/v2/MX_28.hpp"
#include "dynamixel/v2/MX_64.hpp"
#include "input/AutonomousController/src/AutonomousControl.hpp"
#include "input/PS3Controller/src/v2/PS3Walk.hpp"
#include "localisation/src/Localisation.hpp"
#include "motion/Gripper/src/FKGripper.hpp"
#include "motion/Gripper/src/IKGripper.hpp"
#include "motion/MotorController/src/MotorController.hpp"
#include "platform/src/DynamixelProtocol.hpp"
#include "platform/src/Kinematics.hpp"
#include "platform/src/SensorFilter.hpp"
#include "platform/src/uart.hpp"
#include "vision/Camera/src/Camera.hpp"
#include "vision/Classifier/src/Classifier.hpp"
#include "vision/InfraredSensor/src/InfraredSensor.hpp"
#include "vision/LedStatus/src/LedStatus.hpp"
#include "vision/UltrasonicSensor/src/UltrasonicSensor.hpp"

#define ADDRESS_VALUE(VAL) ADD_##VAL
#define SIZE_VALUE(VAL) SIZE_##VAL
#define MX28_ADDRESS_VALUE(VAL) ADDRESS_VALUE(MX28_##VAL)
#define MX28_SIZE_VALUE(VAL) SIZE_VALUE(MX28_##VAL)
#define MX64_ADDRESS_VALUE(VAL) ADDRESS_VALUE(MX64_##VAL)
#define MX64_SIZE_VALUE(VAL) SIZE_VALUE(MX64_##VAL)

// Servo delay time
#define DELAY_TIME 20

class Input {
public:
    static bool Autonomous_Enabled;
};


#endif /* MAIN_H_ */

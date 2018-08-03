#ifndef MX_28_H_
#define MX_28_H_

#include "main.h"

enum Address {
    MODEL_NUMBER           = 0,
    MODEL_INFORMATION      = 2,
    VERSION_OF_FIRMWARE    = 6,
    ID                     = 7,
    BAUD_RATE              = 8,
    RETURN_DELAY_TIME      = 9,
    DRIVE_MODE             = 10,
    OPERATING_MODE         = 11,
    SECONDARY_SHADOW_ID    = 12,
    PROTOCOL_VERSION       = 13,
    HOMING_OFFSET          = 20,
    MOVING_THRESHOLD       = 24,
    TEMPERATURE_LIMIT      = 31,
    MAX_VOLTAGE_LIMIT      = 32,
    MIN_VOLTAGE_LIMIT      = 34,
    PWM_LIMIT              = 36,
    RESERVED               = 38,
    ACCELERATION_LIMIT     = 40,
    VELOCITY_LIMIT         = 44,
    MAX_POSITION_LIMIT     = 48,
    MIN_POSITION_LIMIT     = 52,
    SHUTDOWN               = 63,
    TORQUE_ENABLE          = 64,
    LED                    = 65,
    STATUS_RETURN_LEVEL    = 68,
    REGISTERED_INSTRUCTION = 69,
    HARDWARE_ERROR_STATUS  = 70,
    VELOCITY_I_GAIN        = 76,
    VELOCITY_P_GAIN        = 78,
    POSITION_D_GAIN        = 80,
    POSITION_I_GAIN        = 82,
    POSITION_P_GAIN        = 84,
    FEEDFORWARD_2ND_GAIN   = 88,
    FEEDFORWARD_1ST_GAIN   = 90,
    BUS_WATCHDOG           = 98,
    GOAL_PWM               = 100,
    RESERVED               = 102,
    GOAL_VELOCITY          = 104,
    PROFILE_ACCELERATION   = 108,
    PROFILE_VELOCITY       = 112,
    GOAL_POSITION          = 116,
    REALTIME_TICK          = 120,
    MOVING                 = 122,
    MOVING_STATUS          = 123,
    PRESENT_PWM            = 124,
    PRESENT_LOAD           = 126,
    PRESENT_VELOCITY       = 128,
    PRESENT_POSITION       = 132,
    VELOCITY_TRAJECTORY    = 136,
    POSITION_TRAJECTORY    = 140,
    PRESENT_INPUT_VOLTAGE  = 144,
    PRESENT_TEMPERATURE    = 146
};

enum Address_Size {
    MODEL_NUMBER           = 2,
    MODEL_INFORMATION      = 4,
    VERSION_OF_FIRMWARE    = 1,
    ID                     = 1,
    BAUD_RATE              = 1,
    RETURN_DELAY_TIME      = 1,
    DRIVE_MODE             = 1,
    OPERATING_MODE         = 1,
    SECONDARY_SHADOW_ID    = 1,
    PROTOCOL_VERSION       = 1,
    HOMING_OFFSET          = 4,
    MOVING_THRESHOLD       = 4,
    TEMPERATURE_LIMIT      = 1,
    MAX_VOLTAGE_LIMIT      = 2,
    MIN_VOLTAGE_LIMIT      = 2,
    PWM_LIMIT              = 2,
    RESERVED               = 2,
    ACCELERATION_LIMIT     = 4,
    VELOCITY_LIMIT         = 4,
    MAX_POSITION_LIMIT     = 4,
    MIN_POSITION_LIMIT     = 4,
    SHUTDOWN               = 1,
    TORQUE_ENABLE          = 1,
    LED                    = 1,
    STATUS_RETURN_LEVEL    = 1,
    REGISTERED_INSTRUCTION = 1,
    HARDWARE_ERROR_STATUS  = 1,
    VELOCITY_I_GAIN        = 2,
    VELOCITY_P_GAIN        = 2,
    POSITION_D_GAIN        = 2,
    POSITION_I_GAIN        = 2,
    POSITION_P_GAIN        = 2,
    FEEDFORWARD_2ND_GAIN   = 2,
    FEEDFORWARD_1ST_GAIN   = 2,
    BUS_WATCHDOG           = 1,
    GOAL_PWM               = 2,
    RESERVED               = 2,
    GOAL_VELOCITY          = 4,
    PROFILE_ACCELERATION   = 4,
    PROFILE_VELOCITY       = 4,
    GOAL_POSITION          = 4,
    REALTIME_TICK          = 2,
    MOVING                 = 1,
    MOVING_STATUS          = 1,
    PRESENT_PWM            = 2,
    PRESENT_LOAD           = 2,
    PRESENT_VELOCITY       = 4,
    PRESENT_POSITION       = 4,
    VELOCITY_TRAJECTORY    = 4,
    POSITION_TRAJECTORY    = 4,
    PRESENT_INPUT_VOLTAGE  = 2,
    PRESENT_TEMPERATURE    = 1
};

#endif /* MX_28_H_ */
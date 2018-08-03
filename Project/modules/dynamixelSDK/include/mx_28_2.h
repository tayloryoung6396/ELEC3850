#ifndef MX_28_H_
#define MX_28_H_

#include "main.h"

enum {
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

enum {
    MODEL_NUMBER           = uint16_t,
    MODEL_INFORMATION      = uint32_t,
    VERSION_OF_FIRMWARE    = uint8_t,
    ID                     = uint8_t,
    BAUD_RATE              = uint8_t,
    RETURN_DELAY_TIME      = uint8_t,
    DRIVE_MODE             = uint8_t,
    OPERATING_MODE         = uint8_t,
    SECONDARY_SHADOW_ID    = uint8_t,
    PROTOCOL_VERSION       = uint8_t,
    HOMING_OFFSET          = uint32_t,
    MOVING_THRESHOLD       = uint32_t,
    TEMPERATURE_LIMIT      = uint8_t,
    MAX_VOLTAGE_LIMIT      = uint16_t,
    MIN_VOLTAGE_LIMIT      = uint16_t,
    PWM_LIMIT              = uint16_t,
    RESERVED               = uint16_t,
    ACCELERATION_LIMIT     = uint32_t,
    VELOCITY_LIMIT         = uint32_t,
    MAX_POSITION_LIMIT     = uint32_t,
    MIN_POSITION_LIMIT     = uint32_t,
    SHUTDOWN               = uint8_t,
    TORQUE_ENABLE          = uint8_t,
    LED                    = uint8_t,
    STATUS_RETURN_LEVEL    = uint8_t,
    REGISTERED_INSTRUCTION = uint8_t,
    HARDWARE_ERROR_STATUS  = uint8_t,
    VELOCITY_I_GAIN        = uint16_t,
    VELOCITY_P_GAIN        = uint16_t,
    POSITION_D_GAIN        = uint16_t,
    POSITION_I_GAIN        = uint16_t,
    POSITION_P_GAIN        = uint16_t,
    FEEDFORWARD_2ND_GAIN   = uint16_t,
    FEEDFORWARD_1ST_GAIN   = uint16_t,
    BUS_WATCHDOG           = uint8_t,
    GOAL_PWM               = uint16_t,
    RESERVED               = uint16_t,
    GOAL_VELOCITY          = uint32_t,
    PROFILE_ACCELERATION   = uint32_t,
    PROFILE_VELOCITY       = uint32_t,
    GOAL_POSITION          = uint32_t,
    REALTIME_TICK          = uint16_t,
    MOVING                 = uint8_t,
    MOVING_STATUS          = uint8_t,
    PRESENT_PWM            = uint16_t,
    PRESENT_LOAD           = uint16_t,
    PRESENT_VELOCITY       = uint32_t,
    PRESENT_POSITION       = uint32_t,
    VELOCITY_TRAJECTORY    = uint32_t,
    POSITION_TRAJECTORY    = uint32_t,
    PRESENT_INPUT_VOLTAGE  = uint16_t,
    PRESENT_TEMPERATURE    = uint8_t
};

#endif /* MX_28_H_ */
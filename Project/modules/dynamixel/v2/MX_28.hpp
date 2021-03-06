#ifndef MX_28_H_
#define MX_28_H_

#define SIZE_MX28_MODEL_NUMBER 2
#define SIZE_MX28_MODEL_INFORMATION 4
#define SIZE_MX28_VERSION_OF_FIRMWARE 1
#define SIZE_MX28_ID 1
#define SIZE_MX28_BAUD_RATE 1
#define SIZE_MX28_RETURN_DELAY_TIME 1
#define SIZE_MX28_DRIVE_MODE 1
#define SIZE_MX28_OPERATING_MODE 1
#define SIZE_MX28_SECONDARY_SHADOW_ID 1
#define SIZE_MX28_PROTOCOL_VERSION 1
#define SIZE_MX28_HOMING_OFFSET 4
#define SIZE_MX28_MOVING_THRESHOLD 4
#define SIZE_MX28_TEMPERATURE_LIMIT 1
#define SIZE_MX28_MAX_VOLTAGE_LIMIT 2
#define SIZE_MX28_MIN_VOLTAGE_LIMIT 2
#define SIZE_MX28_PWM_LIMIT 2
#define SIZE_MX28_RESERVED_1 2
#define SIZE_MX28_ACCELERATION_LIMIT 4
#define SIZE_MX28_VELOCITY_LIMIT 4
#define SIZE_MX28_MAX_POSITION_LIMIT 4
#define SIZE_MX28_MIN_POSITION_LIMIT 4
#define SIZE_MX28_SHUTDOWN 1
#define SIZE_MX28_TORQUE_ENABLE 1
#define SIZE_MX28_LED 1
#define SIZE_MX28_STATUS_RETURN_LEVEL 1
#define SIZE_MX28_REGISTERED_INSTRUCTION 1
#define SIZE_MX28_HARDWARE_ERROR_STATUS 1
#define SIZE_MX28_VELOCITY_I_GAIN 2
#define SIZE_MX28_VELOCITY_P_GAIN 2
#define SIZE_MX28_POSITION_D_GAIN 2
#define SIZE_MX28_POSITION_I_GAIN 2
#define SIZE_MX28_POSITION_P_GAIN 2
#define SIZE_MX28_FEEDFORWARD_2ND_GAIN 2
#define SIZE_MX28_FEEDFORWARD_1ST_GAIN 2
#define SIZE_MX28_BUS_WATCHDOG 1
#define SIZE_MX28_GOAL_PWM 2
#define SIZE_MX28_RESERVED_2 2
#define SIZE_MX28_GOAL_VELOCITY 4
#define SIZE_MX28_PROFILE_ACCELERATION 4
#define SIZE_MX28_PROFILE_VELOCITY 4
#define SIZE_MX28_GOAL_POSITION 4
#define SIZE_MX28_REALTIME_TICK 2
#define SIZE_MX28_MOVING 1
#define SIZE_MX28_MOVING_STATUS 1
#define SIZE_MX28_PRESENT_PWM 2
#define SIZE_MX28_PRESENT_LOAD 2
#define SIZE_MX28_PRESENT_VELOCITY 4
#define SIZE_MX28_PRESENT_POSITION 4
#define SIZE_MX28_VELOCITY_TRAJECTORY 4
#define SIZE_MX28_POSITION_TRAJECTORY 4
#define SIZE_MX28_PRESENT_INPUT_VOLTAGE 2
#define SIZE_MX28_PRESENT_TEMPERATURE 1

enum {
    ADD_MX28_MODEL_NUMBER           = 0,
    ADD_MX28_MODEL_INFORMATION      = 2,
    ADD_MX28_VERSION_OF_FIRMWARE    = 6,
    ADD_MX28_ID                     = 7,
    ADD_MX28_BAUD_RATE              = 8,
    ADD_MX28_RETURN_DELAY_TIME      = 9,
    ADD_MX28_DRIVE_MODE             = 10,
    ADD_MX28_OPERATING_MODE         = 11,
    ADD_MX28_SECONDARY_SHADOW_ID    = 12,
    ADD_MX28_PROTOCOL_VERSION       = 13,
    ADD_MX28_HOMING_OFFSET          = 20,
    ADD_MX28_MOVING_THRESHOLD       = 24,
    ADD_MX28_TEMPERATURE_LIMIT      = 31,
    ADD_MX28_MAX_VOLTAGE_LIMIT      = 32,
    ADD_MX28_MIN_VOLTAGE_LIMIT      = 34,
    ADD_MX28_PWM_LIMIT              = 36,
    ADD_MX28_RESERVED_1             = 38,
    ADD_MX28_ACCELERATION_LIMIT     = 40,
    ADD_MX28_VELOCITY_LIMIT         = 44,
    ADD_MX28_MAX_POSITION_LIMIT     = 48,
    ADD_MX28_MIN_POSITION_LIMIT     = 52,
    ADD_MX28_SHUTDOWN               = 63,
    ADD_MX28_TORQUE_ENABLE          = 64,
    ADD_MX28_LED                    = 65,
    ADD_MX28_STATUS_RETURN_LEVEL    = 68,
    ADD_MX28_REGISTERED_INSTRUCTION = 69,
    ADD_MX28_HARDWARE_ERROR_STATUS  = 70,
    ADD_MX28_VELOCITY_I_GAIN        = 76,
    ADD_MX28_VELOCITY_P_GAIN        = 78,
    ADD_MX28_POSITION_D_GAIN        = 80,
    ADD_MX28_POSITION_I_GAIN        = 82,
    ADD_MX28_POSITION_P_GAIN        = 84,
    ADD_MX28_FEEDFORWARD_2ND_GAIN   = 88,
    ADD_MX28_FEEDFORWARD_1ST_GAIN   = 90,
    ADD_MX28_BUS_WATCHDOG           = 98,
    ADD_MX28_GOAL_PWM               = 100,
    ADD_MX28_RESERVED_2             = 102,
    ADD_MX28_GOAL_VELOCITY          = 104,
    ADD_MX28_PROFILE_ACCELERATION   = 108,
    ADD_MX28_PROFILE_VELOCITY       = 112,
    ADD_MX28_GOAL_POSITION          = 116,
    ADD_MX28_REALTIME_TICK          = 120,
    ADD_MX28_MOVING                 = 122,
    ADD_MX28_MOVING_STATUS          = 123,
    ADD_MX28_PRESENT_PWM            = 124,
    ADD_MX28_PRESENT_LOAD           = 126,
    ADD_MX28_PRESENT_VELOCITY       = 128,
    ADD_MX28_PRESENT_POSITION       = 132,
    ADD_MX28_VELOCITY_TRAJECTORY    = 136,
    ADD_MX28_POSITION_TRAJECTORY    = 140,
    ADD_MX28_PRESENT_INPUT_VOLTAGE  = 144,
    ADD_MX28_PRESENT_TEMPERATURE    = 146
};


#endif /* MX_28_H_ */
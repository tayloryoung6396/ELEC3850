/*
 * This should handle the Inputs and Outputs to the world.
 * Communication between the perpherials and the MCU.
 * Inputs:  Camera - I2c
 *			Ultrasonic Sensors - I2C
 *			IR Sensor - I2C
 *			Force Sensor - ADC
 *			Servos - Commands
                   - ADC or I2C
            Motor Controller - Commands
 * Outputs: Servos - ADC or I2C
 *			Motor Controller - GPIO
 */

#include "HardwareIO.hpp"

void HardwareIO_init() {

    UART com;
    com.DynamixelProtocol_init();
}
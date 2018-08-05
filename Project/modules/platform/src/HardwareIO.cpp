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

    connect();

    DynamixelProtocol_init();
    printf("Test\n");
}

void HardwareIO::connect() {
    double baud = 1000000;  // (1mb/s)

    fd = open(devName, O_RDWR | O_NOCTTY | O_NONBLOCK);

    // If we have a valid file handle, and were able to configure it correctly (custom baud)
    if (fd < 0) {
        // There was an exception connecting
        throw std::runtime_error("There was an error setting up the serial connection to the USB Controller");
    }

    printf("Device '%s' successfully opened.\n", devName);

    fp = fd;
}

void HardwareIO::reconnect() {
    printf("Failed to read from '%s' Error: '%s'\n", devName, strerror(errno));

    // Close the connection.
    // close(fd);

    // Connect to the serial port again.
    connect();
}
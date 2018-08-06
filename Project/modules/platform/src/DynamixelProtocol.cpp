/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"

#define device "/dev/ttyACM0"
#define baud 115200

void UART::DynamixelProtocol_init() {
    uart.open(device, baud);
}

void UART::DynamixelProtocol_main() {}

// Function that takes servo ID, address, and data
// Calls dynamixel execute functions
void executeWriteSingle(uint8_t servo_ID, uint8_t address, T data) {
    uart.native_handle();
    if (uart.good()) {
        uart.write(Write());
    }
}

void executeWriteMulti(uint8_t servo_ID[], uint8_t address, T data, uint8_t count) {
    uart.native_handle();
    if (uart.good()) {
        uart.write(BulkWrite());
    }
}

void executeReadSingle(uint8_t servo_ID, uint8_t address, T data) {
    uart.native_handle();
    if (uart.good()) {
        uart.write(Write());
    }
}

void executeReadMulti(uint8_t servo_ID[], uint8_t address, T data, uint8_t count) {
    uart.native_handle();
    if (uart.good()) {
        uart.write(BulkWrite());
    }
}
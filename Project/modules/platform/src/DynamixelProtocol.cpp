/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"

#define UART_DEVICE "/dev/ttyUSB"
#define UART_BAUD 115200

void UART::DynamixelProtocol_init() : device(UART_DEVICE), baud(UART_BAUD) {
    uart.open(device, baud);
}

void UART::DynamixelProtocol_main() {}

// Function that takes servo ID, address, and data
// Calls dynamixel execute functions
void executeWriteSingle(uint8_t servo_ID, uint8_t address, T data) {
    auto buf = dynamixel::v2::Write(servo_ID, address, &data);
    if (uart.good()) {
        uart.write(&buf, sizeof(buf));
    }
}

void executeWriteMulti(uint8_t servo_ID[], uint8_t address, T data, uint8_t count) {

    auto buf;
    for (int i = 0; i < count; i++) {
        buf = dynamixel::v2::BulkWriteData(servo_ID[i], address, &data[i]);
    }
    auto group_buf = dynamixel::v2::BulkWriteCommand();

    if (uart.good()) {
        uart.write(&group_buf, sizeof(group_buf));
    }
}

void executeReadSingle(uint8_t servo_ID, uint8_t address, T data) {
    // auto buf = dynamixel::v2::Read(servo_ID, address);
    // if (uart.good()) {
    //     uart.read(&buf, sizeof(buf));
    // }
}

void executeReadMulti(uint8_t servo_ID[], uint8_t address, T data, uint8_t count) {
    // auto buf = dynamixel::v2::BulkRead();
    // if (uart.good()) {
    //     uart.read(&buf, sizeof(buf));
    // }
}
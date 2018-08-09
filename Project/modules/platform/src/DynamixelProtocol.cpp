/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"

#define UART_DEVICE "/dev/ttyUSB"
#define UART_BAUD 115200

void UART::DynamixelProtocol_init() : device(UART_DEVICE), baud(UART_BAUD) {
    uart.open(device, baud);
    printf("Initilising UART\n");
}

void UART::DynamixelProtocol_main() {}

// Function that takes servo ID, address, and data
// Calls dynamixel execute functions
int executeWriteSingle(uint8_t servo_ID, uint8_t address, double data) {
    auto buf = dynamixel::v2::Write(servo_ID, address, &data);
    if (uart.good()) {
        uart.write(&buf, sizeof(buf));
        return 0;
    }
    return -1;
}

int executeWriteMulti(uint8_t* servo_ID, uint8_t address, double** data, uint8_t count) {

    auto buf[count];
    for (int i = 0; i < count; i++) {
        buf[i] = dynamixel::v2::BulkWriteData(servo_ID[i], address, &data[i]);
    }
    auto group_buf = dynamixel::v2::BulkWriteCommand(&buf, count);

    if (uart.good()) {
        uart.write(&group_buf, sizeof(group_buf));
        return 0;
    }
    return -1;
}

int executeReadSingle(uint8_t servo_ID, uint8_t address, uint* data) {
    // auto buf = dynamixel::v2::Read(servo_ID, address);
    // if (uart.good()) {
    //     uart.read(&buf, sizeof(buf));
    //		return 0;
    // }
    return -1;
}

int executeReadMulti(uint8_t* servo_ID, uint8_t address, uint* data, uint8_t count) {
    // auto buf = dynamixel::v2::BulkRead();
    // if (uart.good()) {
    //     uart.read(&buf, sizeof(buf));
    // 		return 0;
    // }
    return -1;
}
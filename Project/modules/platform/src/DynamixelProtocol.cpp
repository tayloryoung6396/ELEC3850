/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"
#include "uart.hpp"

#define UART_DEVICE "/dev/ttyUSB0"
#define UART_BAUD 115200


class utility::io::uart uart;  // uart;

UART::UART() : device(UART_DEVICE), baud(UART_BAUD) {
    uart.open(device, baud);
    printf("Initilising UART\n");
}

// Function that takes servo ID, address, and data
// Calls dynamixel execute functions
int executeWriteSingle(uint8_t servo_ID, uint16_t address, double data) {
    auto buf = dynamixel::v2::WriteCommand<double>(servo_ID, address, data);

    if (uart.good()) {
        uart.write(&buf, sizeof(buf));
        return 0;
    }
    return -1;
}

int executeWriteMulti(uint8_t* servo_ID, uint16_t address, double* data, uint8_t count) {

    // double buf[count];
    // for (int i = 0; i < count; i++) {
    //     buf[i] = dynamixel::v2::BulkWriteData<double>(servo_ID[i], address, &data[i]);
    // }
    // auto group_buf = dynamixel::v2::BulkWriteCommand<double>(&buf, count);

    // if (uart.good()) {
    //     uart.write(&group_buf, sizeof(group_buf));
    //     return 0;
    // }
    return -1;
}

// TODO Fix
int executeReadSingle(uint8_t servo_ID, uint16_t address, uint* data) {
    auto buf = dynamixel::v2::ReadCommand(servo_ID, address, 1);
    if (uart.good()) {
        uart.read(&buf, sizeof(buf));
        return 0;
    }
    return -1;
}

int executeReadMulti(uint8_t* servo_ID, uint16_t address, uint* data, uint8_t count) {
    // auto buf = dynamixel::v2::BulkRead();
    // if (uart.good()) {
    //     uart.read(&buf, sizeof(buf));
    // 		return 0;
    // }
    return -1;
}
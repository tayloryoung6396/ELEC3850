/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"
#include "uart.hpp"

#define UART_DEVICE "/dev/ttyUSB0"
#define UART_BAUD 115200


class utility::io::uart uart;

UART::UART() : device(UART_DEVICE), baud(UART_BAUD) {
    printf("Initilising UART\n");
    uart.open(device, baud);
}

// Function that takes servo ID, address, and data
// Calls dynamixel execute functions
int executeWriteSingle(uint8_t servo_ID, uint16_t address, uint8_t* data) {
    auto buf = dynamixel::v2::WriteCommand<uint32_t>(servo_ID, address, *data);
    if (uart.good()) {
        uart.write(&buf, sizeof(buf));
        return 0;
    }
    return -1;
}

int executeWriteMulti(uint8_t* servo_ID, uint16_t address, uint8_t* data, uint32_t* count) {

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
int executeReadSingle(uint8_t servo_ID, uint16_t address, uint size, uint32_t* data) {
    auto buf = dynamixel::v2::ReadCommand(servo_ID, address, size);
    if (uart.good()) {
        uart.write(&buf, sizeof(buf));


        // Read
        return 0;
    }
    return -1;
}

int executeReadMulti(uint8_t* servo_ID, uint16_t address, uint32_t* data, uint8_t count) {
    // auto buf = dynamixel::v2::BulkRead();
    // if (uart.good()) {
    //     uart.read(&buf, sizeof(buf));
    // 		return 0;
    // }
    return -1;
}
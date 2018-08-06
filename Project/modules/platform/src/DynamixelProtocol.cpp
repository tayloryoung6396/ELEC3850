/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"

#define device "/dev/ttyACM0"
#define baud 115200

void UART::DynamixelProtocol_init() {

    uart.open(device, baud);
}

void UART::DynamixelProtocol_main() {

    uart.native_handle();

    if (uart.good()) {
        auto c = uart.get();
    }

    uart.write(command.c_str(), command.length());
}


// void UART::DynamixelProtocol_init() {

// connect();
// }

// int Write_Servo(int servo_id, int address, uint8_t data) {
//     if (fwrite(dynamixel::v2::WriteCommand<uint8_t>(servo_id, address, data), 1, 1, UART::fp) != 1) {
//         printf("Error: Could not write to uart\n");
//         return -1;
//     }
//     return 0;
// }

// // TODO use bulk write
// int Write_Servo_Multi(int servo_id[], int address, uint8_t data[], int count) {
//     for (int i = 0; i < count; i++) {
//         if (fwrite(dynamixel::v2::WriteCommand<uint8_t>(servo_id[i], address, data[i]), count, 1, UART::fp) != 1) {
//             printf("Error: Could not write to uart\n");
//             return -1;
//         }
//     }
//     return 0;
// }

// int Read_Servo(int servo_id, int address, uint8_t data) {
//     return 0;
// }

// // TODO use bulk read
// int Read_Servo_Multi(int servo_id[], int address, uint8_t data[], int count) {
//     return 0;
//}

// void UART::connect() {
//     double baud = 1000000;  // (1mb/s)

//     fd = open(devName, O_RDWR | O_NOCTTY | O_NONBLOCK);

//     // If we have a valid file handle, and were able to configure it correctly (custom baud)
//     if (fd < 0) {
//         // There was an exception connecting
//         throw std::runtime_error("There was an error setting up the serial connection to the USB Controller");
//     }

//     printf("Device '%s' successfully opened.\n", devName);
// }

// void UART::reconnect() {
//     printf("Failed to read from '%s' Error: '%s'\n", devName, strerror(errno));

//     // Close the connection.
//     // close(fd);

//     // Connect to the serial port again.
//     connect();
// }

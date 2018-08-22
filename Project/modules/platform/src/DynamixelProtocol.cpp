/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"

#define UART_DEVICE "/dev/ttyUSB0"
#define UART_BAUD 115200

class utility::io::uart uart;

UART::UART() : device(UART_DEVICE), baud(UART_BAUD) {
    printf("Initilising UART\n");
    uart.open(device, baud);
    tx_time_per_byte = (1000.0 / (double) baud) * 10.0;
    std::cout << "Baud rate set to: " << baud << std::endl;
}

// Function that takes servo ID, address, and data
// Calls dynamixel execute functions
int executeWriteSingle(uint8_t servo_ID, uint16_t address, uint8_t* data) {

    auto buf = dynamixel::v2::WriteCommand<uint8_t>(servo_ID, address, *data);

    if (uart.good()) {
        uart.write(&buf, sizeof(buf));
        return 0;
    }
    return -1;
}

int executeWriteBuffer(uint8_t servo_ID, uint16_t address, uint8_t* data, uint8_t* buf) {
    //*buf = dynamixel::v2::BulkWriteData<uint8_t>(servo_ID, address, *data);
    return 0;
}

int executeWriteMulti(uint8_t* buf) {
    // auto group_buf = dynamixel::v2::BulkWriteCommand<uint8_t, sizeof(buf)>(&buf, sizeof(buf));
    if (uart.good()) {
        // uart.write(&group_buf, sizeof(group_buf));
        return 0;
    }
    return -1;
}

#define PKT_HEADER0 0
#define PKT_HEADER1 1
#define PKT_HEADER2 2
#define PKT_RESERVED 3
#define PKT_ID 4
#define PKT_LENGTH_L 5
#define PKT_LENGTH_H 6
#define PKT_INSTRUCTION 7
#define PKT_ERROR 8
#define PKT_PARAMETER0 8

// Communication Result
#define COMM_SUCCESS 0            // tx or rx packet communication success
#define COMM_PORT_BUSY -1000      // Port is busy (in use)
#define COMM_TX_FAIL -1001        // Failed transmit instruction packet
#define COMM_RX_FAIL -1002        // Failed get status packet
#define COMM_TX_ERROR -2000       // Incorrect instruction packet
#define COMM_RX_WAITING -3000     // Now recieving status packet
#define COMM_RX_TIMEOUT -3001     // There is no status packet
#define COMM_RX_CORRUPT -3002     // Incorrect status packet
#define COMM_NOT_AVAILABLE -9000  //

#define data_MAX_LEN (4 * 1024)

#define LATENCY_TIMER 16  // Random value taken from SDK
#define INST_READ 2       // Actually use our enums

#define BYTE_WAIT 1200
#define PACKET_WAIT 100000

// TODO Fix
// template <typename T>
// int executeReadSingle(uint8_t servo_ID, uint16_t address, uint16_t size, T& rx_data) {


//     auto tx_buf = dynamixel::v2::ReadCommand(servo_ID, address, size);
//     std::cout << __LINE__ << std::endl;
//     dynamixel::v2::StatusReturnCommand<T> stat;

//     int rx_result      = COMM_TX_FAIL;
//     uint16_t rx_length = 0;

//     // Check that our UART is still conected
//     if (uart.good()) {
//         std::cout << __LINE__ << " uart is good" << std::endl;

//         // Now lets write our packet
//         uart.write(&tx_buf, sizeof(tx_buf));
//         std::cout << __LINE__ << " uart written" << std::endl;

//         // First we find the packet magic number in order to sync with the channel
//         setPacketTimeout((uint16_t)(PACKET_WAIT));
//         for (int sync = 0; sync < 2;) {
//             if (isPacketTimeout() != true) {
//                 uint8_t byte;

//                 if (uart.read(&byte, 1) > 0) {
//                     sync = (byte == 0xFF) ? (sync + 1) : 0;
//                 }
//             }
//             else {
//                 // The result is pre initialized as a timeout
//                 std::cout << "failed to sync" << std::endl;
//                 return rx_result;
//             }
//         }

//         // We now are now waiting for 4 bytes
//         setPacketTimeout((uint16_t)(BYTE_WAIT * sizeof(Header)));
//         uint8_t* headerBytes = reinterpret_cast<uint8_t*>(&result.header);
//         for (size_t done = 0; done < sizeof(Header);) {
//             if (isPacketTimeout() != true) {
//                 done += uart.read(&headerBytes[done], sizeof(Header) - done);
//             }
//             else {
//                 // The result is pre initialized as a timeout
//                 return COMM_RX_TIMEOUT;
//             }
//         }

//         // Here we adjust our "length" to mean the length of the payload rather then the length of bytes after the
//         // length
//         int length = 0;
//         if (result.header.length < 2) {
//             std::cout << "Length: " << (int) result.header.length << ", " << (int) (result.header.length - 2) << "\r"
//                       << std::endl;
//         }

//         else {
//             length = result.header.length - 2;
//         }

//         // We now are now waiting for our data
//         setPacketTimeout((uint16_t)(BYTE_WAIT * length));
//         result.data.resize(length);
//         for (int done = 0; done < length;) {
//             if (isPacketTimeout() == true) {
//                 done += uart.read(&result.data[done], length - done);
//             }
//             else {
//                 // Set our packet header to timeout and return it
//                 return COMM_RX_TIMEOUT;
//             }
//         }

//         // We just read the checksum now
//         setPacketTimeout((uint16_t)(2000));
//         if (isPacketTimeout() != true) {
//             // If we fail to read the checksum then just assume corrupt data.
//             if (uart.read(&result.checksum, 1) != 1) {
//                 return COMM_RX_CORRUPT;
//             }
//         }
//         else {
//             // If all we are missing is the checksum, just assume the data is corrupt
//             return COMM_RX_CORRUPT;
//         }

//         // Validate our checksum
//         uint16_t crc = dynamixel::v2::calculateChecksum(result);
//         if (result.checksum != crc) {
//             std::cout << "Checksum corrupt got " << result.checksum << " calculated " << crc << std::endl;
//             return COMM_RX_CORRUPT;
//         }

//         // Return the packet we recieved
//         return COMM_SUCCESS;
//     }
// }

template <typename T>
int executeReadSingle(uint8_t servo_ID, uint16_t address, uint16_t size, T& rx_data) {


    auto tx_buf = dynamixel::v2::ReadCommand(servo_ID, address, size);
    std::cout << __LINE__ << std::endl;
    dynamixel::v2::StatusReturnCommand<T> stat;

    int rx_result      = COMM_TX_FAIL;
    uint16_t rx_length = 0;

    // Check that our UART is still conected
    if (uart.good()) {
        std::cout << __LINE__ << " uart is good" << std::endl;

        // Now lets write our packet
        uart.write(&tx_buf, sizeof(tx_buf));
        std::cout << __LINE__ << " uart written" << std::endl;

        // First we find the packet magic number in order to sync with the channel
        // setPacketTimeout((uint16_t)(PACKET_WAIT));
        // for (int sync = 0; sync < 2;) {
        //     if (isPacketTimeout() != true) {
        //         uint8_t byte;

        //         if (uart.read(&byte, 1) > 0) {
        //             sync = (byte == 0xFF) ? (sync + 1) : 0;
        //         }
        //     }
        //     else {
        //         // The result is pre initialized as a timeout
        //         std::cout << "failed to sync" << std::endl;
        //         return rx_result;
        //     }
        // }

        // We now are now waiting for 4 bytes
        setPacketTimeout((uint16_t)((BYTE_WAIT * sizeof(stat.magic)) + (BYTE_WAIT * size) + (2000) + PACKET_WAIT));
        while (true) {
            if (isPacketTimeout() != true) {
                rx_length +=
                    uart.read((reinterpret_cast<uint8_t*>(&stat) + 4), sizeof(stat) - sizeof(stat.magic) - rx_length);
                if (rx_length == sizeof(stat) - sizeof(stat.magic)) {
                    break;
                }
            }
            else {
                // The result is pre initialized as a timeout
                std::cout << "failed to read packet " << rx_length << " of " << sizeof(stat) - sizeof(stat.magic)
                          << std::endl;
                return rx_result;
            }
        }
        // Validate our checksum
        uint16_t crc = dynamixel::v2::calculateChecksum(&stat);
        if (stat.checksum != crc) {
            std::cout << "Checksum corrupt got " << stat.checksum << " calculated " << crc << std::endl;
            return COMM_RX_CORRUPT;
        }

        // Return the packet we recieved
        std::cout << "Success" << std::endl;
        return COMM_SUCCESS;
    }
}

int executeReadMulti(uint8_t* servo_ID, uint16_t address, uint32_t* data, uint8_t count) {
    // auto buf = dynamixel::v2::BulkRead();
    // if (uart.good()) {
    //     uart.read(&buf, sizeof(buf));
    // 		return 0;
    // }
    return -1;
}


void setPacketTimeout(uint16_t packet_length) {
    packet_start_time_ = getCurrentTime();
    packet_timeout_    = (tx_time_per_byte * (double) packet_length) + (LATENCY_TIMER * 2.0) + 2.0;
    // std::cout << "start " << packet_start_time_ << " timeout " << packet_timeout_ << std::endl;
}

void setPacketTimeout(double msec) {
    // std::cout << "I should call this??" << std::endl;
    packet_start_time_ = getCurrentTime();
    packet_timeout_    = msec;
    // std::cout << "S " << packet_start_time_ << " timeout " << packet_timeout_ << std::endl;
}

bool isPacketTimeout() {
    //    std::cout << "Packet Timeout" << packet_timeout_ << std::endl;
    if (getTimeSinceStart() > packet_timeout_) {
        std::cout << __LINE__ << "timeout" << std::endl;
        packet_timeout_ = 0;
        return true;
    }

    return false;
}

double getCurrentTime() {
    // return 0;
    return (double) millis();  // From wiringPi Library
}

double getTimeSinceStart() {
    double elapsed_time;
    elapsed_time = getCurrentTime() - packet_start_time_;
    // std::cout << __LINE__ << " " << elapsed_time << " " << packet_start_time_ << " " << getCurrentTime() <<
    // std::endl;
    if (elapsed_time < 0.0) packet_start_time_ = getCurrentTime();

    return elapsed_time;
}

// template int executeReadSingle<PID>(uint8_t, uint16_t, uint16_t, PID&);
template int executeReadSingle<uint8_t>(uint8_t, uint16_t, uint16_t, uint8_t&);
template int executeReadSingle<uint16_t>(uint8_t, uint16_t, uint16_t, uint16_t&);

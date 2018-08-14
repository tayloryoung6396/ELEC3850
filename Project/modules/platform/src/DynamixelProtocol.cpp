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
    tx_time_per_byte = (1000.0 / (double) baud) * 10.0;
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
#define DXL_MAKEWORD(a, b)                                                                                             \
    ((uint16_t)(((uint8_t)(((uint64_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint64_t)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)                                                                                            \
    ((uint32_t)(((uint16_t)(((uint64_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint64_t)(b)) & 0xffff))) << 16))

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

// TODO Fix
int executeReadSingle(uint8_t servo_ID, uint16_t address, uint size, uint32_t* data) {

    auto buf = dynamixel::v2::ReadCommand(servo_ID, address, size);
    // Check that our UART is still conected
    if (uart.good()) {
        // Figure out our packet length and some appropriate timeout
        // set packet timeout

        setPacketTimeout((uint16_t)(size + 11));  // CRC + Min length?

        // Now lets write our packet
        uart.write(&buf, sizeof(buf));
        // Now lets listen for a return
        int rx_result        = COMM_TX_FAIL;
        uint16_t rx_length   = 0;
        uint16_t wait_length = 11;  // minimum length (H0 H1 H2 RESERVED ID LEN_L LEN_H INST ERROR CRC16_L CRC16_H)

        while (true) {
            // read into data the minimum amount expected, increment the amount read
            rx_length += uart.read(&data[rx_length], wait_length - rx_length);
            if (rx_length >= wait_length) {
                uint16_t idx = 0;

                // Find packet header
                for (idx = 0; idx < (rx_length - 3); idx++) {
                    if ((data[idx] == 0xFF) && (data[idx + 1] == 0xFF) && (data[idx + 2] == 0xFD)
                        && (data[idx + 3] != 0xFD)) {
                        break;
                    }
                }
                if (idx == 0)  // found at the beginning of the packet
                {
                    // TODO Maybe i should be forming the struct here, and then decoding it later...??
                    if (data[PKT_RESERVED] != 0x00 || data[PKT_ID] > 0xFC
                        || DXL_MAKEWORD(data[PKT_LENGTH_L], data[PKT_LENGTH_H]) > data_MAX_LEN
                        || data[PKT_INSTRUCTION] != 0x55) {
                        // remove the first byte in the packet
                        for (uint16_t s = 0; s < rx_length - 1; s++) {
                            data[s] = data[1 + s];
                        }
                        rx_length -= 1;
                        continue;
                    }

                    // re-calculate the exact length of the rx packet
                    if (wait_length != DXL_MAKEWORD(data[PKT_LENGTH_L], data[PKT_LENGTH_H]) + PKT_LENGTH_H + 1) {
                        wait_length = DXL_MAKEWORD(data[PKT_LENGTH_L], data[PKT_LENGTH_H]) + PKT_LENGTH_H + 1;
                        continue;
                    }

                    // Check that we haven't been waiting too long
                    if (rx_length < wait_length) {
                        // check timeout
                        if (isPacketTimeout() == true) {
                            if (rx_length == 0) {
                                rx_result = COMM_RX_TIMEOUT;
                            }
                            else {
                                rx_result = COMM_RX_CORRUPT;
                            }
                            break;
                        }
                        else {
                            continue;
                        }
                    }

                    // verify CRC16
                    uint16_t crc = DXL_MAKEWORD(data[wait_length - 2], data[wait_length - 1]);
                    // TODO this should check the checksum??
                    if (dynamixel::v2::calculateChecksum(data, 0) == crc) {
                        rx_result = COMM_SUCCESS;
                    }
                    else {
                        rx_result = COMM_RX_CORRUPT;
                    }
                    break;
                }
                else {
                    // remove unnecessary packets
                    for (uint16_t s = 0; s < rx_length - idx; s++) {
                        data[s] = data[idx + s];
                    }
                    rx_length -= idx;
                }
            }

            else {
                // check timeout
                if (isPacketTimeout() == true) {
                    if (rx_length == 0) {
                        rx_result = COMM_RX_TIMEOUT;
                    }
                    else {
                        rx_result = COMM_RX_CORRUPT;
                    }
                    break;
                }
            }
        }
    }

    return 0;
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
}

void setPacketTimeout(double msec) {
    packet_start_time_ = getCurrentTime();
    packet_timeout_    = msec;
}

bool isPacketTimeout() {
    if (getTimeSinceStart() > packet_timeout_) {
        packet_timeout_ = 0;
        return true;
    }

    return false;
}

double getCurrentTime() {
    return 0;
    // return (double) millis();  // From wiringPi Library
}

double getTimeSinceStart() {
    double elapsed_time;

    elapsed_time = getCurrentTime() - packet_start_time_;
    if (elapsed_time < 0.0) packet_start_time_ = getCurrentTime();

    return elapsed_time;
}
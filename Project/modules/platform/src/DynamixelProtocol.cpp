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

// TODO Fix
template <typename T>
int executeReadSingle(uint8_t servo_ID, uint16_t address, uint16_t size, T& rx_data) {

    auto tx_buf = dynamixel::v2::ReadCommand(servo_ID, address, size);
    std::cout << __LINE__ << std::endl;
    dynamixel::v2::StatusReturnCommand<T> stat;

    uint8_t* data;
    int rx_result        = COMM_TX_FAIL;
    uint16_t rx_length   = 0;
    uint16_t wait_length = 11;  // minimum length (H0 H1 H2 RESERVED ID LEN_L LEN_H INST ERROR CRC16_L CRC16_H)

    // Check that our UART is still conected
    if (uart.good()) {
        std::cout << __LINE__ << " uart is good" << std::endl;
        // Figure out our packet length and some appropriate timeout
        // set packet timeout
        setPacketTimeout((uint16_t)(11));//size + 11));  // CRC + Min length?
        // Now lets write our packet
        uart.write(&tx_buf, sizeof(tx_buf));
        std::cout << __LINE__ << " uart written" << std::endl;
        // Now lets listen for a return
        while (true) {
            // std::cout << __LINE__ << std::endl;
            // read into data the minimum amount expected, increment the amount read
            rx_length += uart.read(&data[rx_length], wait_length - rx_length);
            if (rx_length >= wait_length) {
                std::cout << __LINE__ << " This would be good" << std::endl;
                uint16_t idx = 0;
                // Find packet header
                for (idx = 0; idx < (rx_length - 3); idx++) {
                    if ((data[idx] == 0xFF) && (data[idx + 1] == 0xFF) && (data[idx + 2] == 0xFD)
                        && (data[idx + 3] != 0xFD)) {
                        std::cout << __LINE__ << std::endl;
                        break;
                    }
                }
                if (idx == 0)  // found at the beginning of the packet
                {
                    std::cout << __LINE__ << std::endl;
                    uart.read(&stat, sizeof(stat));
                    // verify CRC16
                    uint16_t crc = stat.checksum;
                    // TODO this should check the checksum??
                    std::cout << __LINE__ << std::endl;
                    if (dynamixel::v2::calculateChecksum(&stat) == crc) {
                        rx_result = COMM_SUCCESS;
                        std::cout << __LINE__ << std::endl;
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
//                std::cout << __LINE__ << "else" << std::endl;
                if (isPacketTimeout() == true) {
		    std::cout << __LINE__ << "packet timeout" << std::endl;
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
    std::cout << __LINE__ << std::endl;
    rx_data = stat.data;
    std::cout << __LINE__ << std::endl;
    return rx_result;
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
    std::cout << "start " << packet_start_time_ << " timeout " << packet_timeout_ << std::endl;
}

void setPacketTimeout(double msec) {
    std::cout << "I should call this??" << std::endl;
    packet_start_time_ = getCurrentTime();
    packet_timeout_    = msec;
    std::cout << "S " << packet_start_time_ << " timeout " << packet_timeout_ << std::endl;
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
    std::cout << __LINE__ << " " << elapsed_time << " " << packet_start_time_ << " " << getCurrentTime() << std::endl;
    if (elapsed_time < 0.0) packet_start_time_ = getCurrentTime();

    return elapsed_time;
}

template int executeReadSingle<PID>(uint8_t, uint16_t, uint16_t, PID&);
template int executeReadSingle<uint8_t>(uint8_t, uint16_t, uint16_t, uint8_t&);
template int executeReadSingle<uint16_t>(uint8_t, uint16_t, uint16_t, uint16_t&);

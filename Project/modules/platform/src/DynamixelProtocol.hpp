#ifndef DYNAMIXEL_PROTOCOL_H_
#define DYNAMIXEL_PROTOCOL_H_

#include "../../main.hpp"
#include "uart.hpp"

class UART {
private:
    const char* device;
    const int baud;

    // We will wait this long for an initial packet header
    int PACKET_WAIT = 10000;
    // We will only wait a maximum of BYTE_WAIT microseconds between bytes in a packet (assumes baud of 1000000bps)
    int BYTE_WAIT              = 1000;
    int BUS_RESET_WAIT_TIME_uS = 100000;

    /// @brief The file descriptor for the USB TTY device we use to communicate with the devices
    int fd;

    /**
     * @brief Connects to the serial port
     */
    void connect();

    /**
     * @brief Reconnects to the serial port
     */
    void reconnect();

public:
    explicit UART();
    void DynamixelProtocol_init();
};


extern int executeWriteSingle(uint8_t servo_ID, uint16_t address, uint8_t* data);
extern int executeWriteMulti(uint8_t* buf);
extern int executeWriteBuffer(uint8_t servo_ID, uint16_t address, uint8_t* data, uint8_t* buf);
extern int executeReadSingle(uint8_t servo_ID, uint16_t address, uint size, uint32_t* data);
extern int executeReadMulti(uint8_t* servo_ID, uint16_t address, uint32_t* data, uint8_t count);

// SDK Functions

static int tx_time_per_byte;
static int packet_start_time_;
static int packet_timeout_;

extern void setPacketTimeout(uint16_t packet_length);
extern void setPacketTimeout(double msec);
extern bool isPacketTimeout();
extern double getCurrentTime();
extern double getTimeSinceStart();

#endif /* DYNAMIXEL_PROTOCOL_H_ */
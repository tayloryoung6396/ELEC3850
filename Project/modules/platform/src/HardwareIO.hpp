#ifndef HARDWARE_IO_H_
#define HARDWARE_IO_H_

#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>
#include "../../main.hpp"

extern void HardwareIO_init();

class HardwareIO {
private:
    const char* devName;

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
    static int fp;
};

#endif /* HARDWARE_IO_H_ */
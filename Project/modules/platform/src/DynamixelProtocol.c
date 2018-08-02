/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.h"

void DynamixelProtocol_init() {


    // Now that the dynamixels should have started up, set their delay time to 0 (it may not have been configured
    // before)
    executeWrite(WriteCommand<uint8_t>(ID::BROADCAST, CM730::Address::RETURN_DELAY_TIME, 0));

    // Set the dynamixels to not return a status packet when written to (to allow consecutive writes)
    executeWrite(WriteCommand<uint8_t>(ID::BROADCAST, CM730::Address::RETURN_LEVEL, 1));


    return 0;
}

int DynamixelProtocol_main() {
    return 0;
}


template <typename TPacket>
void executeWrite(const TPacket& command) {

    // Lock the mutex
    std::lock_guard<std::mutex> lock(mutex);

    // Write our command to the UART
    int written = write(fd, &command, sizeof(TPacket));

    // Wait until we finish writing before continuing (no buffering)
    tcdrain(fd);

    assert(written == sizeof(TPacket));
    // If compiled with NDEBUG then technically written is unused, suppress that warning
    (void) written;
}
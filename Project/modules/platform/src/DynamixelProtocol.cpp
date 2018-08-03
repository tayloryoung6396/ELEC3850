/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.h"

void DynamixelProtocol_init() {

    // TODO populate PORT_NUM
    return 0;
}

uint8_t calculateChecksum(void* command) {

    uint8_t* data    = static_cast<uint8_t*>(command);
    uint8_t checksum = 0x00;
    // Skip over the magic numbers and checksum the rest of the packet
    for (int i = 2; i < data[Packet::LENGTH] + 3; ++i) {
        checksum += data[i];
    }
    return (~checksum);
}

uint8_t calculateChecksum(const CommandResult& result) {

    uint8_t checksum = 0x00;

    checksum += result.header.id;
    checksum += result.header.length;
    checksum += result.header.errorcode;

    for (size_t i = 0; i < result.data.size(); ++i) {
        checksum += result.data[i];
    }

    return (~checksum);
}

bool DarwinDevice::ping() {

    // Ping and get the result
    CommandResult result = coms.executeRead(PingCommand(id));

    // Check if there was an error code
    return result.header.errorcode == ErrorCode::NONE;
}
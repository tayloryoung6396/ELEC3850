#ifndef DYNAMIXEL_PROTOCOL_H_
#define DYNAMIXEL_PROTOCOL_H_

#include "dynamixel_sdk.h"  // Uses Dynamixel SDK library
#include "main.hpp"


namespace Packet {
enum { MAGIC = 0, ID = 2, LENGTH = 3, INSTRUCTION = 4, ERRBIT = 4, PARAMETER = 5 };
}  // namespace Packet

namespace ErrorCode {
enum {
    NONE          = 0x00,
    INPUT_VOLTAGE = 0x01,
    ANGLE_LIMIT   = 0x02,
    OVERHEATING   = 0x04,
    RANGE         = 0x08,
    CHECKSUM      = 0x10,
    OVERLOAD      = 0x20,
    INSTRUCTION   = 0x40,
    CORRUPT_DATA  = 0x80,
    NO_RESPONSE   = 0xFF
};
}  // namespace ErrorCode

// This is the header that is contained in the CommandResult
#pragma pack(push, 1)  // Make sure that this struct is not cache aligned
struct Header {
    Header() {}
    uint8_t id        = -1;
    uint8_t length    = 0;
    uint8_t errorcode = -1;
};
// Check that this struct is not cache aligned
static_assert(sizeof(Header) == 3, "The compiler is adding padding to this struct, Bad compiler!");
#pragma pack(pop)

// This is the object that is returned when a command is run
struct CommandResult {
    CommandResult() : header(), data(), checksum(0) {}
    Header header;
    std::vector<uint8_t> data;
    uint8_t checksum;
};

// This value calculates the checksum for a packet (the command argument is assumed to be in the CM730 format)
uint8_t calculateChecksum(void* command);
uint8_t calculateChecksum(const CommandResult& result);

/**
 * @brief reads a single packet back from the uart, and returns error codes if they timeout
 *
 * @return The command result, or a command result with an error flag if there was an error
 */
CommandResult readPacket();

/**
 * @brief Executes a passed packet and then waits for a response, Used for single commands (read write ping)
 *
 * @tparam TPacket the type of packet we are executing
 *
 * @param command the command we are executing
 *
 * @return the return value from executing this command
 */
template <typename TPacket>
CommandResult executeRead(const TPacket& command) {

    // Lock the mutex
    std::lock_guard<std::mutex> lock(mutex);

    // Write our command to the UART
    int written = write(fd, &command, sizeof(TPacket));

    // Wait until we finish writing before continuing (no buffering)
    tcdrain(fd);

    // We flush our buffer, just in case there was anything random in it
    tcflush(fd, TCIFLUSH);

    assert(written == sizeof(TPacket));
    // If compiled with NDEBUG then technically written is unused, suppress that warning
    (void) written;

    // Read the packet that we get in response
    return readPacket();
}

/**
 * @brief Executes a passed packet and does not wait for a response (for writes)
 *
 * @tparam TPacket the type of packet we are executing
 *
 * @param command the command we are executing
 */
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

/**
 * @brief This is used to execute a bulk read request.
 *
 * @param command the packet that we are going to send to get the response
 *
 * @return a vector of command results, one for each of the responding devices
 */
std::vector<CommandResult> executeBulk(const std::vector<uint8_t>& command);

/**
 * @brief This is used to execute a broadcast command (to the broadcast address), these expect no response
 *
 * @param command the command to execute
 */
void executeBroadcast(const std::vector<uint8_t>& command);

/**
 * @brief Reads a specified number of bytes from the serial port.
 *
 * @param buf Pointer to a location to store the read data.
 *
 * @param count Number of bytes to read from the serial port.
 *
 * @return the number of bytes.
 */
size_t readBytes(void* buf, size_t count);

/**
 * @brief Writes a specified number of bytes to the serial port.
 *
 * @param buf Pointer to the data to write.
 *
 * @param count Number of bytes to write to the serial port.
 *
 * @return the number of bytes.
 */
size_t writeBytes(const void* buf, size_t count);

/**********************************************************************************************************************/
enum Instruction {
    PING       = 1,
    READ       = 2,
    WRITE      = 3,
    REG_WRITE  = 4,
    ACTION     = 5,
    RESET      = 6,
    SYNC_READ  = 130,
    SYNC_WRITE = 131,
    BULK_READ  = 146,
    BULK_WRITE = 147
};

/**
 * @brief Reads from this device at the given memory address.
 *
 * @details
 *  This will read from the current device at the given memory address, and return an object of type TType.
 *  It will read enough consecutive bytes to fill the TType with data. For example, if you read a 2 byte wide
 *  datatype, it will read 2 consecutive bytes from the device to fill it.
 *
 * @tparam TType the type of data we are reading
 *
 * @param address the address of the data that we are reading
 *
 * @return the data that was read from the device
 */
template <typename TType>
TType read(uint8_t address) {

    // Check that this struct is not cache alligned
    static_assert(sizeof(ReadCommand<TType>) == 8, "The compiler is adding padding to this struct, Bad compiler!");

    // Execute our Read command through the uart
    CommandResult result = coms.executeRead(ReadCommand<TType>(id, address));

    // If there was an error then try reading again
    if (result.data.size() != sizeof(TType)) {
        result = coms.executeRead(ReadCommand<TType>(id, address));
    }

    // If it's still bad then throw a runtime error
    if (result.data.size() != sizeof(TType)) {
        throw std::runtime_error("There was an error while trying to read from the device");
    }

    // Copy our resulting data into our return type
    TType data;
    memcpy(&data, result.data.data(), sizeof(TType));

    // Return our data
    return data;
}

/**
 * @brief Writes data to this device at the given memory address.
 *
 * @details
 *  This will write the passed object to the current device at the given memory address. It will write
 *  consecutive bytes for the size of TType. For example, if you write a 2 byte wide datatype, it will write 2
 *  consecutive bytes from the device.
 *
 * @tparam TType the type of data we are writing
 *
 * @param address   the address of the data that we are reading
 * @param data      the data that we are writing to the device
 */
template <typename TType>
void write(uint8_t address, TType data) {

    // Check that this struct is not cache alligned
    static_assert(sizeof(WriteCommand<TType>) == 7 + sizeof(TType),
                  "The compiler is adding padding to this struct, Bad compiler!");

    // Write our data over the UART
    coms.executeWrite(WriteCommand<TType>(id, address, data));
}

/**
 * @brief Reads a specified number of bytes from this device.
 *
 * @param data Vector to store the read data in. Vector will be resized to hold the requested data.
 *       Contents of the vector will be overwritten.
 *
 * @param count Number of bytes to read from this device.
 *
 * @return the number of bytes.
 */
size_t readBytes(std::vector<uint8_t>& data, size_t count) {

    // Make sure the vector has enough space in it.
    data.resize(count);
    return coms.readBytes(data.data(), count);
}

/**
 * @brief Writes a specified number of bytes to this device.
 *
 * @param data Vector of bytes to send.
 *
 * @return the number of bytes.
 */
size_t writeBytes(const std::vector<uint8_t>& data) {

    return coms.writeBytes(data.data(), data.size());
}

/**
 * @brief This will send a ping request to the device.
 *
 * @return true if the device is working, false if the device is not working
 */
bool ping();

#endif /* DYNAMIXEL_PROTOCOL_H_ */
#ifndef DYNAMIXEL_PROTOCOL_H_
#define DYNAMIXEL_PROTOCOL_H_

#include "main.h"

/**
 * @brief The darwin device is a device on the serial port that will respond to the command types.
 *
 * @details
 *  This class is extended by the CM730, MX28 and FSR's, this allows them to access various memory locations
 *  using the common functionality provided by this class
 *
 * @author Trent Houliston
 */

/**
 * @brief The list of valid instructions for the CM730 and related components.
 */
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
 * @brief This struct mimics the expected data structure for a Write command.
 *
 * @details
 *  This type has it's members arranged in the same way as a raw array of this command would. Because of this
 *  you cannot add or remove members from this type (unless for some reason the API to the CM730 changes). The
 *  template argument and parameter allows you to read any type. For example if you read with a uint16_t then
 *  it will read two bytes to the device. And if you use a struct with 3 uint16_t's in it, then you can directly
 *  read a paramter with an x, y and z bytes (e.g. the accelerometer)
 * @tparam TType the type of data to be written
 *
 * @author Trent Houliston
 */
#pragma pack(push, 1)  // Make it so that the compiler reads this struct "as is" (no padding bytes)
struct ReadCommand {

    // ReadCommand(uint8_t id, uint8_t address) : id(id), address(address), size(sizeof(TType)) {}

    /// Magic number that heads up every packet
    const uint16_t magic = 0xFFFF;
    /// The ID of the device that we are communicating with
    const uint8_t id;
    /// The total length of the data packet (always 4)
    const uint8_t length = 4;
    /// The instruction that we will be executing (The READ instruction)
    const uint8_t instruction = Instruction::READ;
    /// The address to read from
    const uint8_t address;
    /// The number of bytes to read
    const uint8_t size;
    /// Our checksum for this command
    const uint8_t checksum = calculateChecksum(this);
};
#pragma pack(pop)

/**
 * @brief This struct mimics the expected data structure for a Write command.
 *
 * @details
 *  This type has it's members arranged in the same way as a raw array of this command would. Because of this
 *  you cannot add or remove members from this type (unless for some reason the API to the CM730 changes). The
 *  template argument and parameter allows you to write any type. For example if you write with a uint16_t then
 *  it will write two bytes to the device. And if you use a struct with 3 uint16_t's in it, then you directly
 *  write to something with an x, y and z byte.
 *
 * @tparam TType the type of data to be written
 *
 * @author Trent Houliston
 */
#pragma pack(push, 1)  // Make it so that the compiler reads this struct "as is" (no padding bytes)
struct WriteCommand {

    // WriteCommand(uint8_t id, uint8_t address, TType data)
    //     : id(id), length(3 + sizeof(TType)), address(address), data(data) {}

    /// Magic number that heads up every packet
    const uint16_t magic = 0xFFFF;
    /// The ID of the device that we are communicating with
    const uint8_t id;
    /// The total length of the data packet (3 plus however many bytes we are writing)
    const uint8_t length;
    /// The instruction that we will be executing (The WRITE instruction)
    const uint8_t instruction = Instruction::WRITE;
    /// The address we are writing to
    const uint8_t address;
    /// The bytes that we are writing
    const uint8_t data[];
    /// Our checksum for this command
    // const uint8_t checksum = calculateChecksum(this);
};
#pragma pack(pop)

/**
 * @brief This struct mimics the expected data structure for a Ping command.
 *
 * @details
 *  This type has it's members arranged in the same way as a raw array of this command would. Because of this
 *  you cannot add or remove members from this type (unless for some reason the API to the CM730 changes).
 *
 * @author Trent Houliston
 */
#pragma pack(push, 1)  // Make it so that the compiler reads this struct "as is" (no padding bytes)
struct PingCommand {
    explicit PingCommand(uint8_t id) : id(id) {}

    /// Magic number that heads up every packet
    const uint16_t magic = 0xFFFF;
    /// The ID of the device that we are communicating with
    const uint8_t id;
    /// The total length of the data packet (always 2)
    const uint8_t length = 2;
    /// The instruction that we will be executing (The PING instruction)
    const uint8_t instruction = Instruction::PING;
    /// Our checksum for this command
    const uint8_t checksum = calculateChecksum(this);
};
// Check that this struct is not cache alligned
static_assert(sizeof(PingCommand) == 6, "The compiler is adding padding to this struct, Bad compiler!");
#pragma pack(pop)

int id;

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
uint8_t read(uint8_t address) {

    // Check that this struct is not cache alligned
    static_assert(sizeof(ReadCommand<uint8_t>) == 8, "The compiler is adding padding to this struct, Bad compiler!");

    // Execute our Read command through the uart
    CommandResult result = coms.executeRead(ReadCommand<uint8_t>(id, address));

    // If there was an error then try reading again
    if (result.data.size() != sizeof(uint8_t)) {
        result = coms.executeRead(ReadCommand<uint8_t>(id, address));
    }

    // If it's still bad then throw a runtime error
    if (result.data.size() != sizeof(uint8_t)) {
        throw std::runtime_error("There was an error while trying to read from the device");
    }

    // Copy our resulting data into our return type
    uint8_t data;
    memcpy(&data, result.data.data(), sizeof(uint8_t));

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
void write(uint8_t address, uint8_t data) {

    // Check that this struct is not cache alligned
    static_assert(sizeof(WriteCommand<uint8_t>) == 7 + sizeof(uint8_t),
                  "The compiler is adding padding to this struct, Bad compiler!");

    // Write our data over the UART
    executeWrite(WriteCommand<uint8_t>(id, address, data));
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
size_t readBytes(<uint8_t>& data[], size_t count) {

    // Make sure the vector has enough space in it.
    data.resize(count);
    return readBytes(data.data(), count);
}

/**
 * @brief Writes a specified number of bytes to this device.
 *
 * @param data Vector of bytes to send.
 *
 * @return the number of bytes.
 */
size_t writeBytes(const<uint8_t>& data[]) {

    return writeBytes(data.data(), data.size());
}

/**
 * @brief This will send a ping request to the device.
 *
 * @return true if the device is working, false if the device is not working
 */
bool ping();


/************************************************************************************************************************/
// Packet
enum { MAGIC = 0, ID = 2, LENGTH = 3, INSTRUCTION = 4, ERRBIT = 4, PARAMETER = 5 };

// ErrorCode
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
    Header header;
    uint8_t data[];  // Includes checksum value
    // uint8_t checksum;
};

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

CommandResult executeRead(const uint8_t& command) {

    // Lock the mutex
    std::lock_guard<std::mutex> lock(mutex);

    // Write our command to the UART
    int written = write(fd, &command, sizeof(uint8_t));

    // Wait until we finish writing before continuing (no buffering)
    tcdrain(fd);

    // We flush our buffer, just in case there was anything random in it
    tcflush(fd, TCIFLUSH);

    assert(written == sizeof(uint8_t));
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

#endif /* DYNAMIXEL_PROTOCOL_H_ */
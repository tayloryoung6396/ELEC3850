#ifndef DYNAMIXEL_V2_BULKREAD_HPP
#define DYNAMIXEL_V2_BULKREAD_HPP

#include <array>
#include <type_traits>

#include "Dynamixel.hpp"

namespace dynamixel {
namespace v2 {
/**
 * @brief This struct mimics the expected data structure for a Bulk Read command.
 *
 * @details
 *  This type has it's members arranged in the same way as a raw array of this command would. Because of this
 *  you cannot add or remove members from this type. The template argument and parameter allows you to read any
 *  type. For example if you read with a uint16_t then it will read two bytes to the device. And if you use a
 *  struct with 3 uint16_t's in it, then you can directly read a parameter with an x, y and z bytes (e.g. the
 *  accelerometer)
 * @tparam T the type of data to be read from
 * @tparam N the number of devices to read from
 *
 * @author Alex Biddulph
 */
#pragma pack(push, 1)  // Make it so that the compiler reads this struct "as is" (no padding bytes)
    struct BulkReadData {
        BulkReadData(uint8_t id, uint16_t address, uint16_t size) : id(id), address(address), size(size) {}

        /// The ID of the device that we are communicating with
        uint8_t id;
        /// The starting address to read from
        uint16_t address;
        /// The number of bytes to read
        uint16_t size;
    };
    // Check that this struct is not cache aligned
    static_assert(sizeof(BulkReadData) == 5, "The compiler is adding padding to this struct, Bad compiler!");
#pragma pack(pop)

#pragma pack(push, 1)  // Make it so that the compiler reads this struct "as is" (no padding bytes)
    template <size_t N>
    struct BulkReadCommand {

        BulkReadCommand(const std::array<BulkReadData, N>& data)
            : magic(0x00FDFFFF)
            , id(0xFE)
            , length(3 + N * data[0])
            , instruction(Instruction::BULK_READ)
            , data(data)
            , checksum(calculateChecksum(this)) {}

        /// Magic number that heads up every packet
        const uint32_t magic;
        /// The ID of the device that we are communicating with
        const uint8_t id;
        /// The total length of the data packet (always 5)
        const uint16_t length;
        /// The instruction that we will be executing
        const uint8_t instruction;
        /// List of device IDs to read from
        const std::array<BulkReadData, N> data;
        /// Our checksum for this command
        const uint16_t checksum;
    };
#pragma pack(pop)


}  // namespace v2
}  // namespace dynamixel

#endif  // DYNAMIXEL_V2_BULKREAD_HPP

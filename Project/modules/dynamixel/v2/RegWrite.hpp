#ifndef DYNAMIXEL_V2_REGWRITE_HPP
#define DYNAMIXEL_V2_REGWRITE_HPP

#ifndef DYNAMIXEL_V2_INTERNAL
#error Do not include this file on its own. Include Dynamixel.hpp instead.
#endif

namespace dynamixel {
namespace v2 {
/**
 * @brief This struct mimics the expected data structure for a RegWrite command.
 *
 * @details
 *  Instruction that registers the Instruction Packet to a standby status; Packet is later executed through
 *  the Action command. This type has it's members arranged in the same way as a raw array of this command would.
 *  Because of this you cannot add or remove members from this type. The template argument and parameter allows you
 *  to write any type. For example if you write with a uint16_t then it will write two bytes to the device. And if
 *  you use a struct with 3 uint16_t's in it, then you directly write to something with an x, y and z byte.
 *
 * @tparam T the type of data to be written
 *
 * @author Alex Biddulph
 */
#pragma pack(push, 1)  // Make it so that the compiler reads this struct "as is" (no padding bytes)
    template <typename T>
    struct RegWriteCommand {

        RegWriteCommand(uint8_t id, uint16_t address, T data)
            : magic(0x00FDFFFF)
            , id(id)
            , length(3 + sizeof(address) + sizeof(T))
            , instruction(Instruction::REG_WRITE)
            , address(address)
            , data(data)
            , checksum(calculateChecksum(this)) {}

        /// Magic number that heads up every packet
        const uint32_t magic;
        /// The ID of the device that we are communicating with
        const uint8_t id;
        /// The total length of the data packet (3 plus however many bytes we are writing)
        const uint16_t length;
        /// The instruction that we will be executing
        const uint8_t instruction;
        /// The address we are writing to
        const uint16_t address;
        /// The bytes that we are writing
        const T data;
        /// Our checksum for this command
        const uint16_t checksum;
    };
#pragma pack(pop)

}  // namespace v2
}  // namespace dynamixel

#endif  // DYNAMIXEL_V2_REGWRITE_HPP

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
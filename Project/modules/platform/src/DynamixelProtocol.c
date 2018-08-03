/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.h"

void DynamixelProtocol_init() {

    // TODO populate PORT_NUM
    return 0;
}


// Enable Dynamixel Torque
void Enable_Torque(uint8_t DXL_ID) {
    write1ByteTxRx(PORT_NUM, PROTOCOL_VERSION, DXL_ID, TORQUE_ENABLE, 1);
    if ((dxl_comm_result = getLastTxRxResult(PORT_NUM, PROTOCOL_VERSION)) != COMM_SUCCESS) {
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(PORT_NUM, PROTOCOL_VERSION)) != 0) {
        printf("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }
    else {
        printf("Dynamixel has been successfully connected \n");
    }
}

// Write goal position TODO
void Write_Goal(uint8_t DXL_ID, uint8_t dxl_goal_position) {
    write4ByteTxRx(PORT_NUM, PROTOCOL_VERSION, DXL_ID, GOAL_POSITION, dxl_goal_position[index]);
    if ((dxl_comm_result = getLastTxRxResult(PORT_NUM, PROTOCOL_VERSION)) != COMM_SUCCESS) {
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(PORT_NUM, PROTOCOL_VERSION)) != 0) {
        printf("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }
}


// Read present position TODO
uint8_t Read_Present_Pos(uint8_t DXL_ID) {
    dxl_present_position = read4ByteTxRx(PORT_NUM, PROTOCOL_VERSION, DXL_ID, PRESENT_POSITION);
    if ((dxl_comm_result = getLastTxRxResult(PORT_NUM, PROTOCOL_VERSION)) != COMM_SUCCESS) {
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(PORT_NUM, PROTOCOL_VERSION)) != 0) {
        printf("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }

    printf("[ID:%03d] PresPos:%03d\n", DXL_ID, dxl_present_position);

    return dxl_present_position;
}


// Disable Dynamixel Torque
void Disable_Torque(uint8_t DXL_ID) {
    write1ByteTxRx(PORT_NUM, PROTOCOL_VERSION, DXL_ID, TORQUE_ENABLE, 0);
    if ((dxl_comm_result = getLastTxRxResult(PORT_NUM, PROTOCOL_VERSION)) != COMM_SUCCESS) {
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(PORT_NUM, PROTOCOL_VERSION)) != 0) {
        printf("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }
}

// Try to ping the Dynamixel
uint16_t Ping(uint8_t DXL_ID) {
    // Get Dynamixel model number
    uint16_t dxl_model_number = pingGetModelNum(port_num, PROTOCOL_VERSION, DXL_ID);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS) {
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0) {
        printf("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }
    else {
        printf("[ID:%03d] ping Succeeded. Dynamixel model number : %d\n", DXL_ID, dxl_model_number);
    }

    return dxl_model_number;
}

// Try to broadcast ping the Dynamixel
void Broadcast_Ping() {
    broadcastPing(PORT_NUM, PROTOCOL_VERSION);
    if ((dxl_comm_result = getLastTxRxResult(PORT_NUM, PROTOCOL_VERSION)) != COMM_SUCCESS)
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));

    printf("Detected Dynamixel : \n");
    for (id = 0; id < MAX_ID; id++) {
        if (getBroadcastPingResult(PORT_NUM, PROTOCOL_VERSION, id)) printf("[ID:%03d]\n", id);
    }
}

// TODO
void Write_Multi(uint8_t* DXL_ID[], uint8_t Address, uint8_t* dxl_data[], uint8_t num_servos) {

    // Possibly unused variables TODO
    int index                   = 0;
    int dxl_comm_result         = COMM_TX_FAIL;                                              // Communication result
    uint8_t dxl_addparam_result = False;                                                     // AddParam result
    uint8_t dxl_getdata_result  = False;                                                     // GetParam result
    int dxl_goal_position[2]    = {DXL_MINIMUM_POSITION_VALUE, DXL_MAXIMUM_POSITION_VALUE};  // Goal position

    uint8_t dxl_error             = 0;             // Dynamixel error
    uint8_t dxl_led_value[2]      = {0x00, 0xFF};  // Dynamixel LED value for write
    int32_t dxl1_present_position = 0;             // Present position
    uint8_t dxl2_led_value_read;                   // Dynamixel LED value for read

    // Initialize GroupBulkWrite Struct
    int groupwrite_num = groupBulkWrite(port_num, PROTOCOL_VERSION);

    for (int i = 0; i < num_servos; i++) {
        // Add parameter storage for Dynamixel#1 goal position
        dxl_addparam_result = groupBulkWriteAddParam(groupwrite_num,
                                                     DXL_ID[i],
                                                     GOAL_POSITION,
                                                     LEN_PRO_GOAL_POSITION,
                                                     dxl_goal_position[index],
                                                     LEN_PRO_GOAL_POSITION);
        if (dxl_addparam_result != True) {
            printf("[ID:%03d] groupBulkWrite addparam failed", DXL_ID[i]);
            return 0;
        }
    }

    // Bulkwrite goal position and LED value
    groupBulkWriteTxPacket(groupwrite_num);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));

    // Clear bulkwrite parameter storage
    groupBulkWriteClearParam(groupwrite_num);
}

// TODO
uint8_t* Read_Multi(uint8_t* DXL_ID[], uint8_t Address, uint8_t** dxl_data, uint8_t num_servos) {

    // Possibly unused variables TODO
    int index                   = 0;
    int dxl_comm_result         = COMM_TX_FAIL;                                              // Communication result
    uint8_t dxl_addparam_result = False;                                                     // AddParam result
    uint8_t dxl_getdata_result  = False;                                                     // GetParam result
    int dxl_goal_position[2]    = {DXL_MINIMUM_POSITION_VALUE, DXL_MAXIMUM_POSITION_VALUE};  // Goal position

    uint8_t dxl_error             = 0;             // Dynamixel error
    uint8_t dxl_led_value[2]      = {0x00, 0xFF};  // Dynamixel LED value for write
    int32_t dxl1_present_position = 0;             // Present position
    uint8_t dxl2_led_value_read;                   // Dynamixel LED value for read

    // Initialize Groupbulkread Struct
    int groupread_num = groupBulkRead(port_num, PROTOCOL_VERSION);

    // Bulkread present position and LED status
    groupBulkReadTxRxPacket(groupread_num);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
        printf("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));

    for (int i = 0; i < num_servos; i++) {
        // Check if groupbulkread data of Dynamixel#1 is available
        dxl_getdata_result =
            groupBulkReadIsAvailable(groupread_num, DXL_ID[i], PRESENT_POSITION, LEN_PRO_PRESENT_POSITION);
        if (dxl_getdata_result != True) {
            printf("[ID:%03d] groupBulkRead getdata failed", DXL_ID[i]);
            return 0;
        }

        // Get present position value
        dxl_result = groupBulkReadGetData(groupread_num, DXL_ID[i], PRESENT_POSITION, LEN_PRO_PRESENT_POSITION);
    }
    return *;
}

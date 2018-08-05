/*
 * This is some dark magic, probably just close the file
 */

#include "DynamixelProtocol.hpp"

void DynamixelProtocol_init() {}

void DynamixelProtocol_main() {}

int Write_Servo(int servo_id, int address, uint8_t data) {
    if (fwrite(WriteCommand(servo_id, address, data), , 1, servo_uart) != 1) {
        printf("Error: Could not write to uart\n");
        return -1;
    }
    return 0;
}

// TODO use bulk write
int Write_Servo_Multi(int servo_id[], int address, uint8_t data[], int count) {
    for (int i = 0; i < count; i++) {
        if (fwrite(WriteCommand(servo_id[i], address, data[i]), , 1, servo_uart) != 1) {
            printf("Error: Could not write to uart\n");
            return -1;
        }
    }
    return 0;
}

int Read_Servo(int servo_id, int address, uint8_t data) {
    return 0;
}

// TODO use bulk read
int Read_Servo_Multi(int servo_id[], int address, uint8_t data[], int count) {
    return 0;
}
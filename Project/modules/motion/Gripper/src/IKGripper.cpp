/*
 * This should calculate servo positions required to place the gripper at a desired place.
 * Inputs:  Gripper position request as vec3d
 * Outputs: Servo positions as angles
 */

/*
 * Inverse Kinematics - Given a position in some coordinate system, what do the angles of my servos need to be?
 * This section of the model ignores 3 of the 6 servos
 *
 * Servos:
 * Base_Yaw = 1
 * Base_Pitch = 2
 * Elbow_Pitch = 3
 * Elbow_Roll = 4
 * Wrist_Pitch = 5
 * Gripper = 6
 *
 * Kinematics.base_pos[3] - vec3 xyz position of the base servo from the origin
 * Kinematics.arm_len_1 - Length from pivot to pivot of first part
 * Kinematics.arm_len_2 - Length from pivot to pivot of next part
 * Kinematics.grip_cen - Distance between last servo and the center of the gripper grip
 *
 * Figure out some length parameter
 * arm_len_3 = sqrt((Goal_pos[0] - Kinematics.base_pos[0] - Kinematics.grip_cen),2 +
                    (Goal_pos[1] - Kinematics.base_pos[1]),2 +
                    (Goal_pos[2] - Kinematics.base_pos[2]),2)
 * theta_elbow_pitch "C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
 * theta_wrist_pitch
 * theta_base_pitch
 * base_elevation = cos(sqrt((Goal_pos[0] - Kinematics.base_pos[0] - Kinematics.grip_cen),2 +
                             (Goal_pos[1] - Kinematics.base_pos[1]),2)
 * wrist_declination = 90 - base_elevation
 *
 *
 * base_pitch = 90 - base_elevation - theta_base_pitch
 * elbow_pitch = 180 - theta_wrist_pitch
 * wrist_pitch = -90 + base_pitch + elbow_pitch
 *
 * This should all result in the gripper staying parallel to the ground and able to move to any xyz coordinate in reach
 */

#include "IKGripper.hpp"

double Gripper_angles::base_pitch  = 0;
double Gripper_angles::base_yaw    = 0;
double Gripper_angles::elbow_pitch = 0;
double Gripper_angles::wrist_pitch = 0;
double Gripper_angles::grip        = 0;

double Gripper::Goal[3] = {0};

void IKGripper_init() {
    std::cout << "Initilising IK GRIPPER" << std::endl;
    // Run homing script
    if (Gripper_home() != 0) {
        std::cout << "ERROR: Homing Gripper" << std::endl;
    }
}

int Gripper_home() {
    // std::cout << "Gripper_home function" << std::endl;
    // Read servo present positions
    int32_t base_yaw;
    int32_t base_pitch;
    int32_t elbow_pitch;
    int32_t wrist_pitch;

    // NOTE This should probably be a bulk read
    executeReadSingle(1, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), base_yaw);
    delay(DELAY_TIME);
    executeReadSingle(2, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), base_pitch);
    delay(DELAY_TIME);
    executeReadSingle(3, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), elbow_pitch);
    delay(DELAY_TIME);
    executeReadSingle(4, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), wrist_pitch);

    // Do FK to figure out where we are
    std::cout << "FK Read: base_yaw" << base_yaw << " base_pitch " << base_pitch << " elbow_pitch " << elbow_pitch
              << " wrist_pitch " << wrist_pitch << std::endl;
    double Pres_pos[3];
    FK_Calculate(convert_pos_rad(Base_Yaw, double(base_yaw)),
                 convert_pos_rad(Base_Pitch, double(base_pitch)),
                 convert_pos_rad(Elbow_Pitch, double(elbow_pitch)),
                 convert_pos_rad(Wrist_Pitch, double(wrist_pitch)),
                 Pres_pos);
    // std::cout << "Present Position FK: x " << Pres_pos[0] << " y " << Pres_pos[1] << " z " << Pres_pos[2] <<
    // std::endl;

    // Plan a path to the homing positions
    double Goal_pos[3] = {Kinematics::grip_home[0], Kinematics::grip_home[1], Kinematics::grip_home[2]};
    std::cout << "Goal Position Homing " << Goal_pos[0] << " " << Goal_pos[1] << " " << Goal_pos[2] << std::endl;
    IK_Calculate(Goal_pos);

    std::cout << "IK Result: b_y " << Gripper_angles::base_yaw << " b_p " << Gripper_angles::base_pitch << " e_p "
              << Gripper_angles::elbow_pitch << " w_p " << Gripper_angles::wrist_pitch << std::endl;

    std::cout << "Converted Result: b_y " << convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw) << " b_p "
              << convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch) << " e_p "
              << convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch) << " w_p "
              << convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch) << std::endl;

    // Send servos to positions
    // NOTE This should probably be a bulk write
    executeWriteSingle(
        Base_Yaw, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw));
    delay(DELAY_TIME);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(DELAY_TIME);
    Close_Gripper();

    return 0;
}

int IKGripper_Grab_Object(double Goal_pos[3]) {
    // std::cout << "IKGripper_Grab_Object function" << std::endl;

    // std::cout << "Goal pos " << Goal_pos[0] << " " << Goal_pos[1] << " " << Goal_pos[2] << std::endl;

    if (Validate_Pos(Goal_pos) != 0) {
        std::cout << "Error Gripper Position Invalid" << std::endl;
        return -1;
    }


    else if (Open_Gripper() != 0) {
        std::cout << "Error could not open Gripper" << std::endl;
        return -1;
    }

    else if (IK_Calculate(Goal_pos) != 0) {
        std::cout << "Error could not calculate Gripper IK" << std::endl;
        return -1;
    }
    executeWriteSingle(
        Base_Yaw, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw));
    delay(DELAY_TIME);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(DELAY_TIME);
    if (Grip_Object() != 0) {
        std::cout << "Error could not Grip Object" << std::endl;
        return -1;
    }
    return 0;
}

int IKGripper_move(double Goal_pos[3]) {
    // std::cout << "IKGripper_move function" << std::endl;

    // std::cout << "Goal pos " << Goal_pos[0] << " " << Goal_pos[1] << " " << Goal_pos[2] << std::endl;

    if (Validate_Pos(Goal_pos) != 0) {
        std::cout << "Error Gripper Position Invalid" << std::endl;
        return -1;
    }

    else if (IK_Calculate(Goal_pos) != 0) {
        std::cout << "Error could not calculate Gripper IK" << std::endl;
        return -1;
    }

    // std::cout << "IK Result: b_y " << Gripper_angles::base_yaw << " b_p " << Gripper_angles::base_pitch << " e_p "
    // << Gripper_angles::elbow_pitch << " w_p " << Gripper_angles::wrist_pitch << std::endl;

    // std::cout << "Converted Result: b_y " << convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw) << " b_p "
    // << convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch) << " e_p "
    // << convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch) << " w_p "
    // << convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch) << std::endl;
    executeWriteSingle(
        Base_Yaw, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw));
    delay(DELAY_TIME);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(DELAY_TIME);
    return 0;
}

int IKGripper_Place_Object(double Goal_pos[3]) {
    // std::cout << "IKGripper_Place_Object function" << std::endl;

    if (Validate_Pos(Goal_pos) != 0) {
        std::cout << "Error Gripper Position Invalid" << std::endl;
        return -1;
    }

    else if (IK_Calculate(Goal_pos) != 0) {
        std::cout << "Error could not calculate Gripper IK" << std::endl;
        return -1;
    }
    executeWriteSingle(
        Base_Yaw, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw));
    delay(DELAY_TIME);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(DELAY_TIME);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(DELAY_TIME);
    if (Open_Gripper() != 0) {
        std::cout << "Error could not open Gripper" << std::endl;
        return -1;
    }

    // TODO make rest position
    else if (IK_Calculate(Kinematics::Rest_Gripper) != 0) {
        std::cout << "Error could not calculate Gripper IK" << std::endl;
        return -1;
    }
    data[0] = convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw);
    data[1] convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch);
    data[2] convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch);
    data[3] convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch);
    // NOTE Bulkwrite
    for (int i = 0; i < count; i++) {
        executeWriteSingle(servo_ID[count], address, data[count]);
        delay(DELAY_TIME);
    }

    if (Close_Gripper() != 0) {
        std::cout << "Error could not Grip Object" << std::endl;
        return -1;
    }
    return 0;
}

int IK_Calculate(double Goal_pos[3]) {
    double theta_elbow_pitch = 0;
    double theta_wrist_pitch = 0;
    double theta_base_pitch  = 0;

    Goal_pos[0] -= 0;  // Kinematics::base_pos[0];  // Some origin offset, currently 0
    Goal_pos[1] -= 0;  // Kinematics::base_pos[1];  // Some origin offset, currently 0
    Goal_pos[2] -= 0;  // Kinematics::base_pos[2];  // Some origin offset, currently 0

    Gripper_angles servo;

    // TODO x must always be positive
    if (Goal_pos[0] < 0) {
        std::cout << "X pos < 0" << std::endl;
        return -1;
    }

    double rGoal_xy = std::sqrt(std::pow(Goal_pos[0], 2) + std::pow(Goal_pos[1], 2));  // - Kinematics::grip_cen;

    double arm_len_3 = std::sqrt(std::pow(rGoal_xy, 2) + std::pow(Goal_pos[2], 2));

    double alpha = 0;

    // Our arm needs to be fully extended
    if (arm_len_3 > Kinematics::arm_len_1 + Kinematics::arm_len_2) {
        std::cout << "Length too long" << std::endl;
        theta_base_pitch  = 0;
        theta_elbow_pitch = M_PI;
        theta_wrist_pitch = 0;

        // just Calculate the triangle straight from this hypotinuse
        // kind of like this
        alpha = std::acos(rGoal_xy / arm_len_3);
        if (isnan(alpha)) {
            alpha = 0;
        }
        // return -1;
    }
    // Arm can never be this close
    else if (arm_len_3 < 0.05) {
        std::cout << "Arm too close" << std::endl;
        // Throw this out
        return -1;
    }
    else {
        theta_base_pitch  = SSS_triangle(Kinematics::arm_len_1, arm_len_3, Kinematics::arm_len_2);
        theta_elbow_pitch = SSS_triangle(Kinematics::arm_len_1, Kinematics::arm_len_2, arm_len_3);
        theta_wrist_pitch = SSS_triangle(Kinematics::arm_len_2, arm_len_3, Kinematics::arm_len_1);

        alpha = std::acos(rGoal_xy / arm_len_3);
        if (isnan(alpha)) {
            alpha = 0;
        }
    }

    if (Goal_pos[2] > 0) {
        Gripper_angles::base_pitch  = M_PI_2 - theta_base_pitch - alpha - 0.1745;  // Account for weird arm
        Gripper_angles::elbow_pitch = M_PI_2 - theta_elbow_pitch;
        Gripper_angles::wrist_pitch = theta_base_pitch - theta_elbow_pitch + alpha - 0.1745;
    }
    else {
        Gripper_angles::base_pitch  = M_PI_2 - theta_base_pitch + alpha - 0.1745;  // Account for weird arm
        Gripper_angles::elbow_pitch = M_PI_2 - theta_elbow_pitch;
        Gripper_angles::wrist_pitch = -M_PI + theta_base_pitch + theta_elbow_pitch - alpha;
    }
    return 0;
}

double SSS_triangle(double a, double b, double c) {
    //"C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
    // std::cout << "a b c " << a << " " << b << " " << c << std::endl;
    return std::acos((std::pow(a, 2) + std::pow(b, 2) - std::pow(c, 2)) / (2.0 * a * b));
}

int Grip_Object() {
    // Grip until the gripper has some defined load
    bool Gripped     = false;
    uint8_t servo_ID = Gripper;
    uint16_t grip_load;
    while (!Gripped) {
        Gripper_angles::grip++;
        // Check if the object is in gripper
        if (executeReadSingle(servo_ID, MX28_ADDRESS_VALUE(PRESENT_LOAD), MX28_SIZE_VALUE(PRESENT_LOAD), grip_load)
            == 0) {
            if (grip_load >= Kinematics::grip_load) {
                Gripped = true;
            }
            else if (Gripper_angles::grip >= Kinematics::grip_closed) {
                std::cout << "Error: Gripper failed to grab object" << std::endl;
                return -1;
            }
            delay(DELAY_TIME);
        }
    }
    return 0;
}

int Open_Gripper() {
    uint8_t servo_ID = Gripper;
    uint16_t address = MX28_ADDRESS_VALUE(GOAL_POSITION);
    int32_t data     = convert_rad_pos(servo_ID, Kinematics::grip_open);
    std::cout << "Open_Gripper " << data << std::endl;
    executeWriteSingle(servo_ID, address, data);
    delay(DELAY_TIME);
    return 0;
}

int Close_Gripper() {
    uint8_t servo_ID = Gripper;
    uint16_t address = MX28_ADDRESS_VALUE(GOAL_POSITION);
    int32_t data     = convert_rad_pos(servo_ID, Kinematics::grip_closed);
    std::cout << "Close_Gripper " << data << std::endl;
    executeWriteSingle(servo_ID, address, data);
    delay(DELAY_TIME);
    return 0;
}

int32_t convert_rad_pos(uint8_t servo_ID, double angle) {

    // Get the limits for the specific servo
    // Get the offset for the servo
    double max_limit = Kinematics::Servo_max_limit[servo_ID - 1];
    double min_limit = Kinematics::Servo_min_limit[servo_ID - 1];
    double offset    = Kinematics::Servo_offset[servo_ID - 1];

    // Add the offset to the angle
    angle += offset;

    if (angle > max_limit) {
        angle = max_limit;
    }
    else if (angle < min_limit) {
        angle = min_limit;
    }
    angle = (angle + M_PI) / (2 * M_PI);

    return ((int) (angle * 4096));
}

double convert_pos_rad(uint8_t servo_ID, uint32_t angle) {
    return ((double) ((angle * 2.0 * M_PI / 4960.0) - (2 * M_PI)));
}

int Validate_Pos(double Goal_pos[3]) {

    double x_limits[2] = {Kinematics::origin_offset[0] - (Kinematics::tank_length / 2),
                          (Kinematics::tank_length / 2) + Kinematics::origin_offset[0]};
    double y_limits[2] = {Kinematics::origin_offset[1] - (Kinematics::tank_width / 2),
                          (Kinematics::tank_width / 2) + Kinematics::origin_offset[1]};
    double z_limits[2] = {Kinematics::origin_offset[2] - (Kinematics::tank_height / 2),
                          (Kinematics::tank_height / 2) + Kinematics::origin_offset[2]};

    if ((Goal_pos[0] < x_limits[0]) | (Goal_pos[0] > x_limits[1])) {
        // std::cout << "Gripper Position Valid" << std::endl;
        return 0;
    }
    else if ((Goal_pos[1] < y_limits[0]) | (Goal_pos[1] > y_limits[1])) {
        // std::cout << "Gripper Position Valid" << std::endl;
        return 0;
    }
    else if ((Goal_pos[2] < z_limits[0]) | (Goal_pos[2] > z_limits[1])) {
        // std::cout << "Gripper Position Valid" << std::endl;
        return 0;
    }
    std::cout << "Goal X " << Goal_pos[0] << ", " << x_limits[0] << ", " << x_limits[1] << std::endl;
    std::cout << "Goal Y " << Goal_pos[1] << ", " << y_limits[0] << ", " << y_limits[1] << std::endl;
    std::cout << "Goal Z " << Goal_pos[2] << ", " << z_limits[0] << ", " << z_limits[1] << std::endl;
    std::cout << "Error goal position invalid" << std::endl;

    // TODO Fix validation
    // return 0;
    return -1;
}

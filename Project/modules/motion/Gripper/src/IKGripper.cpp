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

    // TODO This should probably be a bulk read
    executeReadSingle(1, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), base_yaw);
    delay(10);
    executeReadSingle(2, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), base_pitch);
    delay(10);
    executeReadSingle(3, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), elbow_pitch);
    delay(10);
    executeReadSingle(4, MX28_ADDRESS_VALUE(PRESENT_POSITION), MX28_SIZE_VALUE(PRESENT_POSITION), wrist_pitch);

    // Do FK to figure out where we are
    // std::cout << "FK Read: base_yaw" << base_yaw << " base_pitch " << base_pitch << " elbow_pitch " << elbow_pitch
    //           << " wrist_pitch " << wrist_pitch << std::endl;
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
    // std::cout << "Goal Position Homing " << Goal_pos[0] << " " << Goal_pos[1] << " " << Goal_pos[2] << std::endl;
    IK_Calculate(Goal_pos);

    // std::cout << "IK Result: b_y " << Gripper_angles::base_yaw << " b_p " << Gripper_angles::base_pitch << " e_p "
    // << Gripper_angles::elbow_pitch << " w_p " << Gripper_angles::wrist_pitch << std::endl;

    // std::cout << "Converted Result: b_y " << convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw) << " b_p "
    // << convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch) << " e_p "
    // << convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch) << " w_p "
    // << convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch) << std::endl;

    // Send servos to positions
    // TODO This should probably be a bulk write
    executeWriteSingle(
        Base_Yaw, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw));
    delay(10);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(10);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(10);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(10);
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
    delay(10);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(10);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(10);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(10);
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
    delay(10);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(10);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(10);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(10);
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
    delay(10);
    executeWriteSingle(
        Base_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch));
    delay(10);
    executeWriteSingle(
        Elbow_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch));
    delay(10);
    executeWriteSingle(
        Wrist_Pitch, MX28_ADDRESS_VALUE(GOAL_POSITION), convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch));
    delay(10);
    if (Open_Gripper() != 0) {
        std::cout << "Error could not open Gripper" << std::endl;
        return -1;
    }

    // TODO make rest position
    // else if (IK_Calculate(Kinematics::Rest_Gripper) != 0) {
    //     std::cout << "Error could not calculate Gripper IK" << std::endl;
    //     return -1;
    // }
    // data[0] = convert_rad_pos(Base_Yaw, Gripper_angles::base_yaw);
    // data[1] convert_rad_pos(Base_Pitch, Gripper_angles::base_pitch);
    // data[2] convert_rad_pos(Elbow_Pitch, Gripper_angles::elbow_pitch);
    // data[3] convert_rad_pos(Wrist_Pitch, Gripper_angles::wrist_pitch);
    // // TODO Bulkwrite
    // for (int i = 0; i < count; i++) {
    //     executeWriteSingle(servo_ID[count], address, data[count]);
    //     delay(10);
    // }

    if (Close_Gripper() != 0) {
        std::cout << "Error could not Grip Object" << std::endl;
        return -1;
    }
    return 0;
}

int IK_Calculate(double Goal_pos[3]) {
    // std::cout << "IK_Calculate function" << std::endl;
    double theta_elbow_pitch = 0;
    double theta_wrist_pitch = 0;
    double theta_base_pitch  = 0;

    Goal_pos[0] -= Kinematics::base_pos[0];  // Some origin offset, currently 0
    Goal_pos[1] -= Kinematics::base_pos[1];  // Some origin offset, currently 0
    Goal_pos[2] -= Kinematics::base_pos[2];  // Some origin offset, currently 0

    Gripper_angles servo;

    // Calculate our base yaw rotation
    Gripper_angles::base_yaw = std::atan2(Goal_pos[1], Goal_pos[0]);

    // Now create a plane on this rotation and the z axis
    // Find our new horizontal goal point (To the wrist servo, ignore the gripper)
    double rGoal_xy = std::sqrt(std::pow((Goal_pos[0]), 2) + std::pow((Goal_pos[1]), 2));  // - Kinematics::grip_cen;

    // Calculate the straight line distance to the wrist servo
    double arm_len_3 = std::sqrt(std::pow(rGoal_xy, 2) + std::pow(Goal_pos[2], 2)) - DELTA_GRIP;

    std::cout << "arm 3 " << arm_len_3 << std::endl;
    // This forces our closest position to be 90 deg at the elbow, as we can't bend more than that anyway
    // Essentially forms a right angled triangle
    double min_dist = std::sqrt(std::pow(Kinematics::arm_len_1, 2) + std::pow(Kinematics::arm_len_2, 2));
    if (arm_len_3 <= min_dist) {
        // Find point on the same line
        double curr_dist = std::sqrt(std::pow(rGoal_xy, 2) + std::pow(Goal_pos[2], 2));
        // Find the new rGoal_xy distance
        rGoal_xy = rGoal_xy * min_dist / curr_dist;
        // Find the new height
        Goal_pos[2] = Goal_pos[2] * min_dist / curr_dist;
        // Recalculate the arm length
        arm_len_3 = curr_dist - DELTA_GRIP;
    }

    // Our arm needs to be fully extended
    if (arm_len_3 > Kinematics::arm_len_1 + Kinematics::arm_len_2) {
        std::cout << "Length too long" << std::endl;
        // TODO This doesn't work, not sure why?
        theta_elbow_pitch = 0;
        theta_wrist_pitch = 0;
        theta_base_pitch  = 0;
    }
    else {
        // std::cout << "Length within limits" << std::endl;
        theta_elbow_pitch = SSS_triangle(Kinematics::arm_len_1, Kinematics::arm_len_2, arm_len_3);
        theta_wrist_pitch = SSS_triangle(Kinematics::arm_len_2, arm_len_3, Kinematics::arm_len_1);
        theta_base_pitch  = SSS_triangle(Kinematics::arm_len_1, arm_len_3, Kinematics::arm_len_2);
    }

    double alpha = std::acos(rGoal_xy / arm_len_3);
    if (isnan(alpha)) {
        alpha = 0;
    }
    if (Goal_pos[2] > 0) {
        alpha = -alpha;
    }

    Gripper_angles::base_pitch  = M_PI_2 - theta_base_pitch + alpha;
    Gripper_angles::elbow_pitch = M_PI - theta_elbow_pitch;
    // Gripper_angles::wrist_pitch = theta_elbow_pitch - theta_base_pitch + alpha;
    Gripper_angles::wrist_pitch = -(Gripper_angles::elbow_pitch - theta_base_pitch + alpha);


    std::cout << "Theta bp " << theta_base_pitch << " ep " << theta_elbow_pitch << " wp " << theta_wrist_pitch
              << std::endl;

    return 0;
}

double SSS_triangle(double a, double b, double c) {
    // std::cout << "SSS_triangle function" << std::endl;
    //"C = acos((a,2 + b,2 - c,2)/(2 * a * b))"
    // std::cout << "a b c " << a << " " << b << " " << c << std::endl;
    return std::acos((std::pow(a, 2) + std::pow(b, 2) - std::pow(c, 2)) / (2.0 * a * b));
}

int Grip_Object() {
    // std::cout << "Grip_Object function" << std::endl;
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
            delay(10);
        }
    }
    return 0;
}

int Open_Gripper() {
    // std::cout << "Open_Gripper function" << std::endl;
    uint8_t servo_ID = Gripper;
    uint16_t address = MX28_ADDRESS_VALUE(GOAL_POSITION);
    int32_t data     = convert_rad_pos(servo_ID, Kinematics::grip_open);
    std::cout << "Open_Gripper " << data << std::endl;
    executeWriteSingle(servo_ID, address, data);
    delay(10);
    return 0;
}

int Close_Gripper() {
    // std::cout << "Close_Gripper function" << std::endl;
    uint8_t servo_ID = Gripper;
    uint16_t address = MX28_ADDRESS_VALUE(GOAL_POSITION);
    int32_t data     = convert_rad_pos(servo_ID, Kinematics::grip_closed);
    std::cout << "Close_Gripper " << data << std::endl;
    executeWriteSingle(servo_ID, address, data);
    delay(10);
    return 0;
}

int32_t convert_rad_pos(uint8_t servo_ID, double angle) {
    // std::cout << "convert_rad_pos function" << std::endl;

    // Get the limits for the specific servo
    // Get the offset for the servo
    // TODO actually read limits
    // TODO convert to use radians
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


    // std::cout << "convert servo_ID " << (int) servo_ID << " from " << angle << " to " << (int) (angle * 4096)
    // << std::endl;
    // std::cout << "limits max, min " << max_limit << " " << min_limit << std::endl;
    return ((int) (angle * 4096));
}

double convert_pos_rad(uint8_t servo_ID, uint32_t angle) {
    // std::cout << "convert_pos_rad function" << std::endl;
    return ((double) ((angle * 2.0 * M_PI / 4960.0) - (2 * M_PI)));
}

int Validate_Pos(double Goal_pos[3]) {
    // std::cout << "Validate_Pos function" << std::endl;

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
    return -1;
}

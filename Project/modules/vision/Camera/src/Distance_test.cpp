
#include <math.h>
#include <cmath>
#include <iostream>

double tank_length = 0.355;
double cam_phi     = 15;
double cam_pos[3]  = {tank_length / 2.0, 0, -0.05};
double cam_height  = 0.05;


int Width  = 1280;
int Height = 960;

double pixel_x      = 1.12;  // um
double pixel_y      = 1.12;  // um
double resolution_x = 3280;
double resolution_y = 2464;
double focal_len    = 3.04;  // mm

int main() {
    int u = 0;  // Some pixel that you are interested in
    int v = 0;  // Some pixel that you are interested in

    double p = (pixel_x * 0.000001) * (u - Width / 2.0);   // The screen coordinates returned
    double q = (pixel_y * 0.000001) * (v - Height / 2.0);  // The screen coordinates returned

    // Now find the angle the pixel is offset from the screen origin
    double theta = std::atan(p / focal_len);  // The vertical angle
    double phi   = std::atan(q / focal_len);  // The horizontal angle

    // Now convert the offsets into world coordinates
    // Vertical
    double plane_offset = 0;
    double dist_x       = (cam_height - plane_offset) * std::tan(M_PI_2 - theta - cam_phi);
    std::cout << "Distance x " << dist_x << std::endl;

    // Horizontal
    double dist_y = (cam_height - plane_offset) / std::cos(M_PI_2 - theta - cam_phi) * std::tan(phi);
    std::cout << "Distance y " << dist_y << std::endl;

    // These should be the components of x and y of the object from a pixel
}
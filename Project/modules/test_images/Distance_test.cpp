
#include <math.h>
#include <cmath>
#include <iostream>

double tank_length = 0.355;
double cam_phi     = 15 * M_PI / 180.0;
double cam_pos[3]  = {tank_length / 2.0, 0, -0.05};
double cam_height  = 0.05;


int Width  = 1280;
int Height = 960;

double pixel_x      = 1.12e-6;  // um
double pixel_y      = 1.12e-6;  // um
double resolution_x = 3280;
double resolution_y = 2464;
double focal_len    = 3.04e-3;  // mm

int main() {
    int u = 0;  // Some pixel that you are interested in
    int v = 0;  // Some pixel that you are interested in
    std::cout << "Enter x ";
    std::cin >> u;
    std::cout << "Enter y ";
    std::cin >> v;
    std::cout << std::endl;

    double screen_p = (u - Width / 2.0);
    double screen_q = (v - Height / 2.0);

    std::cout << "screen_p " << screen_p << std::endl;
    std::cout << "screen_q " << screen_q << std::endl;

    double p = (resolution_x / (double) Width) * pixel_x * (u - Width / 2.0);    // The screen coordinates returned
    double q = (resolution_y / (double) Height) * pixel_y * (v - Height / 2.0);  // The screen coordinates returned

    std::cout << "p " << p << std::endl;
    std::cout << "q " << q << std::endl;

    // Now find the angle the pixel is offset from the screen origin
    double phi   = -std::atan2(p, focal_len);
    double theta = -std::atan2(q, focal_len);

    std::cout << "Theta " << theta << std::endl;
    std::cout << "Phi " << phi << std::endl;
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
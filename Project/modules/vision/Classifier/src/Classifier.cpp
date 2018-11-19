/*
 * This should handle object detection of the images.
 * Figue out vector to object of interest from image.
 * Inputs:  3 x Greyscale images
 * Outputs: Vector to objects of interest
 * Red: (255, 0, 0)
 * Green: (0, 255, 0)
 * Blue: (0, 0, 255)
 * Yellow: (255, 255, 0)
 * Brown: (165, 42, 42)
 * Orange: (255, 165, 0)
 * Black: (0, 0, 0)
 * White: (255, 255, 255)
 */

// http://www.cse.psu.edu/~rtc12/CSE486/lecture12.pdf
// http://www.cse.psu.edu/~rtc12/CSE486/lecture13.pdf

#include "Classifier.hpp"

#define OFFSET 5

//#define WIDTH_MIN 100
//#define WIDTH_MAX 300
//#define HEIGHT_MIN 100
//#define HEIGHT_MAX 200

int Classifier::colours[3][3][2] = {0};
int Classifier::seed[1000][2];
int seed[1000][2];
int Classifier::object[10][4];

typedef struct {
    double r;  // a fraction between 0 and 1
    double g;  // a fraction between 0 and 1
    double b;  // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;  // angle in degrees
    double s;  // a fraction between 0 and 1
    double v;  // a fraction between 0 and 1
} hsv;

static hsv rgb2hsv(rgb in);
static rgb hsv2rgb(hsv in);


void Classifier_init() {
    std::cout << "Initilising CLASSIFIER" << std::endl;
    const int colours[3][3][2] = {
        {{64, 97}, {19, 48}, {19, 47}},        // Red maximum and minimum pixel parameters (RGB Image)
        {{75, 190}, {41, 225}, {14, 54}},      // Green maximum and minimum pixel parameters (HSV Image)
        {{200, 218}, {104, 239}, {58, 146}}};  // Blue maximum and minimum pixel parameters (HSV Image)
    for (int q = 0; q < 3; q++) {
        for (int w = 0; w < 3; w++) {
            for (int e = 0; e < 2; e++) {
                Classifier::colours[q][w][e] = colours[q][w][e];
            }
        }
    }
}

void Classifier(uint8_t* data) {

    int pixel         = 0;  // start at the first pixel of the image
    int colour        = 0;
    int i             = 0;
    int k             = 0;
    int objects_found = 0;

    rgb rgb_colour;
    hsv hsv_colour;
    int h_value = 0;
    int s_value = 0;
    int v_value = 0;

    for (pixel = 0; pixel <= Image::Height * Image::Width * 3; pixel += 4200) {
        // printf("into pixel for loop \n");
        for (colour = 0; colour < 3; colour++) {
            // printf("into colour for loop \n");

            if (colour == 0 && data[pixel] >= Classifier::colours[colour][0][0]
                && data[pixel] <= Classifier::colours[colour][0][1]
                && data[(pixel + 1)] >= Classifier::colours[colour][1][0]
                && data[(pixel + 1)] <= Classifier::colours[colour][1][1]
                && data[(pixel + 2)] >= Classifier::colours[colour][2][0]
                && data[(pixel + 2)] <= Classifier::colours[colour][2][1]) {
                seed[i][0] = colour;
                seed[i][1] = pixel;
                i++;
                break;
            }
            else if (colour == 1 || colour == 2) {
                rgb_colour.r = data[pixel] / 255.0;
                rgb_colour.g = data[pixel + 1] / 255.0;
                rgb_colour.b = data[pixel + 2] / 255.0;
                hsv_colour   = rgb2hsv(rgb_colour);
                h_value      = hsv_colour.h;
                s_value      = hsv_colour.s * 255;
                v_value      = hsv_colour.v * 255;

                if (h_value >= Classifier::colours[colour][0][0] && h_value <= Classifier::colours[colour][0][1]
                    && s_value >= Classifier::colours[colour][1][0] && s_value <= Classifier::colours[colour][1][1]
                    && v_value >= Classifier::colours[colour][2][0] && v_value <= Classifier::colours[colour][2][1]) {
                    seed[i][0] = colour;
                    seed[i][1] = pixel;
                    i++;
                    break;
                }
            }
        }
    }
    printf("seeds = %d\n", i);

    // cycle through seed points
    for (int j = 0; j <= (i - 5); j++) {
        colour              = seed[j][0];
        pixel               = seed[j][1];
        int m               = 0;
        int seed_y          = (seed[j][1] / 3.0) / Image::Width;
        int seed_x          = (seed[j][1] / 3.0) - seed_y * Image::Width;
        bool seed_in_object = 0;

        if (objects_found > 0) {
            for (m = 0; m <= objects_found; m++) {
                // printf("into seed object check \n");
                int errorx = 200;
                int errory = 50;

                int object_center = Image::Width * 3 * Classifier::object[m][1] + Classifier::object[m][0] * 3;
                int ax            = Classifier::object[m][1] - Classifier::object[m][3] * 3 / 2.0;
                int ay            = Classifier::object[m][0] - Classifier::object[m][2] * 3 / 2.0;
                int bx            = Classifier::object[m][1] + Classifier::object[m][3] * 3 / 2.0;
                int by            = Classifier::object[m][0] + Classifier::object[m][2] * 3 / 2.0;

                // printf("Object Center = %d \t ax = %d \t ay = %d \t bx = %d \t by = %d \t\n", object_center, ax, ay,
                // bx, by); printf("Seed = %d \t Seed_x = %d \t Seed_y = %d \t\n\n", pixel, seed_x, seed_y);

                if (seed_x >= (ax - errorx) && seed_x <= (bx + errorx) && seed_y >= ay - (errory)
                    && seed_y <= (by + errory)) {
                    // seed is already in an object
                    seed_in_object = 1;
                    // printf("SEED IN OBJECT \n\n");
                    break;
                }
            }
        }

        if (seed_in_object == 1) {
            // printf("SEED IN OBJECT IF LOOP BREAK \n");
        }

        else {
            int left  = 0;
            int right = 0;
            int up    = 0;
            int down  = 0;

            int R_min = Classifier::colours[colour][0][0];
            int R_max = Classifier::colours[colour][0][1];
            int G_min = Classifier::colours[colour][1][0];
            int G_max = Classifier::colours[colour][1][1];
            int B_min = Classifier::colours[colour][2][0];
            int B_max = Classifier::colours[colour][2][1];


            // cluster right
            int error = 0;

            while (error < 11) {

                int a = 0;
                if (seed[j][0] == 0) {

                    h_value = data[pixel];
                    s_value = data[pixel + 1];
                    v_value = data[pixel + 2];
                }

                if (seed[j][0] == 1 || seed[j][0] == 2) {

                    rgb_colour.r = data[pixel] / 255.0;
                    rgb_colour.g = data[pixel + 1] / 255.0;
                    rgb_colour.b = data[pixel + 2] / 255.0;
                    hsv_colour   = rgb2hsv(rgb_colour);
                    h_value      = hsv_colour.h;
                    s_value      = hsv_colour.s * 255;
                    v_value      = hsv_colour.v * 255;
                }

                if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
                    && B_max >= v_value) {
                    a = 1;
                }
                if (a != 1) {
                    error++;
                }
                right++;

                pixel += 3;
            }


            // cluster left
            error = 0;
            pixel = seed[j][1];

            while (error < 11) {

                int a = 0;
                if (seed[j][0] == 0) {

                    h_value = data[pixel - 3];
                    s_value = data[pixel - 2];
                    v_value = data[pixel - 1];
                }

                if (seed[j][0] == 1 || seed[j][0] == 2) {
                    rgb_colour.r = data[pixel - 3] / 255.0;
                    rgb_colour.g = data[pixel - 2] / 255.0;
                    rgb_colour.b = data[pixel - 1] / 255.0;
                    hsv_colour   = rgb2hsv(rgb_colour);
                    h_value      = hsv_colour.h;
                    s_value      = hsv_colour.s * 255;
                    v_value      = hsv_colour.v * 255;
                }

                if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
                    && B_max >= v_value) {

                    a = 1;
                }

                if (a != 1) {
                    error++;
                }

                left++;

                pixel = pixel - 3;
            }

            // cluster up
            error = 0;
            pixel = seed[j][1];

            while (error < 11) {

                int a = 0;
                if (seed[j][0] == 0) {

                    h_value = data[pixel];
                    s_value = data[pixel + 1];
                    v_value = data[pixel + 2];
                }

                if (seed[j][0] == 1 || seed[j][0] == 2) {

                    rgb_colour.r = data[pixel] / 255.0;
                    rgb_colour.g = data[pixel + 1] / 255.0;
                    rgb_colour.b = data[pixel + 2] / 255.0;
                    hsv_colour   = rgb2hsv(rgb_colour);
                    h_value      = hsv_colour.h;
                    s_value      = hsv_colour.s * 255;
                    v_value      = hsv_colour.v * 255;
                }

                if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
                    && B_max >= v_value) {

                    int p = (pixel / 3.0) / Image::Width;
                    int q = (pixel / 3.0) - p * Image::Width;

                    a = 1;
                }

                if (a != 1) {
                    error++;
                }
                up++;

                pixel = pixel - Image::Width * 3;
            }

            // cluster down
            error = 0;
            pixel = seed[j][1];
            while (error < 11) {

                int a = 0;
                if (seed[j][0] == 0) {

                    h_value = data[pixel];
                    s_value = data[pixel + 1];
                    v_value = data[pixel + 2];
                }

                if (seed[j][0] == 1 || seed[j][0] == 2) {
                    rgb_colour.r = data[pixel] / 255.0;
                    rgb_colour.g = data[pixel + 1] / 255.0;
                    rgb_colour.b = data[pixel + 2] / 255.0;
                    hsv_colour   = rgb2hsv(rgb_colour);
                    h_value      = hsv_colour.h;
                    s_value      = hsv_colour.s * 255;
                    v_value      = hsv_colour.v * 255;
                }

                if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
                    && B_max >= v_value) {

                    a = 1;
                }

                if (a != 1) {
                    error++;
                }
                down++;

                pixel = pixel + Image::Width * 3;
            }

            // calulate object width and height from clustering
            int width  = left + right;
            int height = up + down;

            if (width >= WIDTH_MIN && width <= WIDTH_MAX && height >= HEIGHT_MIN && height <= HEIGHT_MAX) {

                objects_found++;

                // Calculate centers
                pixel = seed[j][1];
                int y = (seed[j][1] / 3.0) / Image::Width;
                int x = (seed[j][1] / 3.0) - y * Image::Width;

                int center_x = ((x - left) + (x + right)) / 2.0;
                int center_y = ((y - up) + (y + down)) / 2.0;

                Classifier::object[k][0] = center_x;
                Classifier::object[k][1] = center_y;
                Classifier::object[k][2] = width;
                Classifier::object[k][3] = height;

                // printf("Object %d, width %d, height %d, center x %d, center y %d, width %d height %d\n",
                // objects_found, width, height, center_x, center_y, width, height); printf("pixel = %d, left = %d,
                // right = %d, y = %d, x = %d, up = %d, down = %d\n",  pixel, left, right, y, x, up, down);

                printf("Object found seed = %d\n", Classifier::object[k][1]);

                k++;
            }
        }
    }
    printf("Objects found = %d \n", objects_found);
    return;
}

// void Classifier(uint8_t* data) {

//     int pixel         = 0;  // start at the first pixel of the image
//     int colour        = 0;
//     int i             = 0;
//     int k             = 0;
//     int objects_found = 0;

//     rgb rgb_colour;
//     hsv hsv_colour;
//     int h_value = 0;
//     int s_value = 0;
//     int v_value = 0;

//     for (pixel = 0; pixel <= Image::Height * Image::Width * 3; pixel += 4200) {
//         for (colour = 0; colour < 1; colour++) {

//             if (colour == 0 && data[pixel] >= Classifier::colours[colour][0][0]
//                 && data[pixel] <= Classifier::colours[colour][0][1]
//                 && data[(pixel + 1)] >= Classifier::colours[colour][1][0]
//                 && data[(pixel + 1)] <= Classifier::colours[colour][1][1]
//                 && data[(pixel + 2)] >= Classifier::colours[colour][2][0]
//                 && data[(pixel + 2)] <= Classifier::colours[colour][2][1]) {
//                 Classifier::seed[i][0] = colour;
//                 Classifier::seed[i][1] = pixel;
//                 i++;
//                 break;
//             }
//             else if (colour == 1 || colour == 2) {
//                 rgb_colour.r = data[pixel] / 255.0;
//                 rgb_colour.g = data[pixel + 1] / 255.0;
//                 rgb_colour.b = data[pixel + 2] / 255.0;
//                 hsv_colour   = rgb2hsv(rgb_colour);
//                 h_value      = hsv_colour.h;
//                 s_value      = hsv_colour.s * 255;
//                 v_value      = hsv_colour.v * 255;

//                 if (h_value >= Classifier::colours[colour][0][0] && h_value <= Classifier::colours[colour][0][1]
//                     && s_value >= Classifier::colours[colour][1][0] && s_value <= Classifier::colours[colour][1][1]
//                     && v_value >= Classifier::colours[colour][2][0] && v_value <= Classifier::colours[colour][2][1])
//                     { Classifier::seed[i][0] = colour; Classifier::seed[i][1] = pixel; i++; break;
//                 }
//             }
//         }
//     }

// //    printf("seeds = %d\n", i);

//     // cycle through seed points
//     for (int j = 0; j <= (i - 5); j++) {
//         colour              = Classifier::seed[j][0];
//         pixel               = Classifier::seed[j][1];
//         int m               = 0;
//         int seed_y          = (Classifier::seed[j][1] / 3.0) / Image::Width;
//         int seed_x          = (Classifier::seed[j][1] / 3.0) - seed_y * Image::Width;
//         bool seed_in_object = 0;

//         if (objects_found > 0) {
//             for (m = 0; m <= objects_found; m++) {
//                 int errorx = 200;
//                 int errory = 50;

//                 int object_center = Image::Width * 3 * Classifier::object[m][1] + Classifier::object[m][0] * 3;
//                 int ax            = Classifier::object[m][1] - Classifier::object[m][3] * 3 / 2.0;
//                 int ay            = Classifier::object[m][0] - Classifier::object[m][2] * 3 / 2.0;
//                 int bx            = Classifier::object[m][1] + Classifier::object[m][3] * 3 / 2.0;
//                 int by            = Classifier::object[m][0] + Classifier::object[m][2] * 3 / 2.0;

//                 if (seed_x >= (ax - errorx) && seed_x <= (bx + errorx) && seed_y >= ay - (errory)
//                     && seed_y <= (by + errory)) {
//                     // seed is already in an object
//                     seed_in_object = 1;
//                     break;
//                 }
//             }
//         }

//         if (seed_in_object == 1) {
//         }

//         else {
//             int left  = 0;
//             int right = 0;
//             int up    = 0;
//             int down  = 0;

//             int R_min = Classifier::colours[colour][0][0];
//             int R_max = Classifier::colours[colour][0][1];
//             int G_min = Classifier::colours[colour][1][0];
//             int G_max = Classifier::colours[colour][1][1];
//             int B_min = Classifier::colours[colour][2][0];
//             int B_max = Classifier::colours[colour][2][1];

//             // cluster right
//             int error = 0;
//             while (error < 11) {

//                 if (Classifier::seed[j][0] == 0) {

//                     h_value = data[pixel];
//                     s_value = data[pixel + 1];
//                     v_value = data[pixel + 2];
//                 }

//                 else if (Classifier::seed[j][0] == 1 || Classifier::seed[j][0] == 2) {

//                     rgb_colour.r = data[pixel] / 255.0;
//                     rgb_colour.g = data[pixel + 1] / 255.0;
//                     rgb_colour.b = data[pixel + 2] / 255.0;
//                     hsv_colour   = rgb2hsv(rgb_colour);
//                     h_value      = hsv_colour.h;
//                     s_value      = hsv_colour.s * 255;
//                     v_value      = hsv_colour.v * 255;
//                 }

//                 if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
//                     && B_max >= v_value) {
//                 }
//                 else {
//                     error++;
//                 }
//                 right++;

//                 pixel += 3;
// //		std::cout << "Cluster right" << std::endl;
//             }

//             // cluster left
//             error = 0;
//             pixel = Classifier::seed[j][1];

//             while (error < 11) {
//                 if (Classifier::seed[j][0] == 0) {

//                     h_value = data[pixel - 3];
//                     s_value = data[pixel - 2];
//                     v_value = data[pixel - 1];
//                 }

//                 else if (Classifier::seed[j][0] == 1 || Classifier::seed[j][0] == 2) {
//                     rgb_colour.r = data[pixel - 3] / 255.0;
//                     rgb_colour.g = data[pixel - 2] / 255.0;
//                     rgb_colour.b = data[pixel - 1] / 255.0;
//                     hsv_colour   = rgb2hsv(rgb_colour);
//                     h_value      = hsv_colour.h;
//                     s_value      = hsv_colour.s * 255;
//                     v_value      = hsv_colour.v * 255;
//                 }

//                 if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
//                     && B_max >= v_value) {
//                 }
//                 else {
//                     error++;
//                 }

//                 left++;
//                 pixel -= 3;
// //		std::cout << "Cluster left" << std::endl;
//             }

//             // cluster up
//             error = 0;
//             pixel = Classifier::seed[j][1];
//             while (error < 11) {
//                 if (Classifier::seed[j][0] == 0) {
//                     h_value = data[pixel];
//                     s_value = data[pixel + 1];
//                     v_value = data[pixel + 2];
//                 }

//                 else if (Classifier::seed[j][0] == 1 || Classifier::seed[j][0] == 2) {
//                     rgb_colour.r = data[pixel] / 255.0;
//                     rgb_colour.g = data[pixel + 1] / 255.0;
//                     rgb_colour.b = data[pixel + 2] / 255.0;
//                     hsv_colour   = rgb2hsv(rgb_colour);
//                     h_value      = hsv_colour.h;
//                     s_value      = hsv_colour.s * 255;
//                     v_value      = hsv_colour.v * 255;
//                 }

//                 if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
//                     && B_max >= v_value) {
//                     int p = (pixel / 3.0) / Image::Width;
//                     int q = (pixel / 3.0) - p * Image::Width;
//                 }
//                 else {
//                     error++;
//                 }

//                 up++;
//                 pixel = pixel - Image::Width * 3;
//   //          	std::cout << "Cluster up" << std::endl;
// 	    }

//             // cluster down
//             error = 0;
//             pixel = Classifier::seed[j][1];
//             while (error < 11) {
//                 if (Classifier::seed[j][0] == 0) {

//                     h_value = data[pixel];
//                     s_value = data[pixel + 1];
//                     v_value = data[pixel + 2];
//                 }

//                 else if (Classifier::seed[j][0] == 1 || Classifier::seed[j][0] == 2) {
//                     rgb_colour.r = data[pixel] / 255.0;
//                     rgb_colour.g = data[pixel + 1] / 255.0;
//                     rgb_colour.b = data[pixel + 2] / 255.0;
//                     hsv_colour   = rgb2hsv(rgb_colour);
//                     h_value      = hsv_colour.h;
//                     s_value      = hsv_colour.s * 255;
//                     v_value      = hsv_colour.v * 255;
//                 }

//                 if (R_min <= h_value && R_max >= h_value && G_min <= s_value && G_max >= s_value && B_min <= v_value
//                     && B_max >= v_value) {
//                 }
//                 else {
//                     error++;
//                 }

//                 down++;
//                 pixel = pixel + Image::Width * 3;
//     //            std::cout << "Cluster down" << std::endl;
//             }

//             // calulate object width and height from clustering
//             int width  = left + right;
//             int height = up + down;

//             if (width >= WIDTH_MIN && width <= WIDTH_MAX && height >= HEIGHT_MIN && height <= HEIGHT_MAX) {

//                 objects_found++;

//                 // Calculate centers
//                 pixel = Classifier::seed[j][1];
//                 int y = (Classifier::seed[j][1] / 3.0) / Image::Width;
//                 int x = (Classifier::seed[j][1] / 3.0) - y * Image::Width;

//                 int center_x = ((x - left) + (x + right)) / 2.0;
//                 int center_y = ((y - up) + (y + down)) / 2.0;

//                 Classifier::object[k][0] = center_x;
//                 Classifier::object[k][1] = center_y;
//                 Classifier::object[k][2] = width;
//                 Classifier::object[k][3] = height;

//                 printf("Object found seed = %d\n", Classifier::object[k][1]);

//                 k++;
//             }
//         }
//     }
//     printf("Objects found = %d \n", objects_found);

//     if (objects_found > 0) {
//         AutoState::known_object = TRUE;
//     }
//     else {
//         AutoState::known_object = FALSE;
//     }

//     return;
// }


void find_distance(int u, int v) {

    double p = (Camera::resolution_x / (double) Image::Width) * Camera::pixel_x
               * (u - Image::Width / 2.0);  // The screen coordinates returned
    double q = (Camera::resolution_y / (double) Image::Height) * Camera::pixel_y
               * (v - Image::Height / 2.0);  // The screen coordinates returned


    // Now find the angle the pixel is offset from the screen origin
    double phi   = -std::atan2(p, Camera::focal_len);
    double theta = -std::atan2(q, Camera::focal_len);

    // Now convert the offsets into world coordinates
    // Vertical
    double plane_offset = 0;  // TODO Remove shitty hardcoded value

    double dist_x = (Kinematics::cam_height - plane_offset) * std::tan(M_PI_2 - theta - Kinematics::cam_phi);
    std::cout << "Distance x " << dist_x << std::endl;

    // Horizontal
    double dist_y =
        (Kinematics::cam_height - plane_offset) / std::cos(M_PI_2 - theta - Kinematics::cam_phi) * std::tan(phi);
    std::cout << "Distance y " << dist_y << std::endl;

    // Set the tank goal
    // These distances are relative to the tank, however i need them in world coordinates

    Localisation::w_Goal_Position[0] = dist_x * std::cos(Localisation::w_Tank_Rotation)
                                       - dist_x * std::sin(Localisation::w_Tank_Rotation)
                                       + Localisation::w_Tank_Position[0];
    Localisation::w_Goal_Position[1] = dist_y * std::cos(Localisation::w_Tank_Rotation)
                                       - dist_y * std::sin(Localisation::w_Tank_Rotation)
                                       + Localisation::w_Tank_Position[1];
}


void Output_Segmentation(uint8_t* seg_image_array,
                         int img_width,
                         int img_height,
                         int obj_width,
                         int obj_height,
                         int center[2]) {

    // Find the start of our lines to draw
    int top_line[3] = {center[0] - obj_width / 2.0, center[1] - obj_height / 2.0, obj_width};
    int start_pixel = (top_line[0] + top_line[1] * img_width) * 3;
    int end_pixel   = (start_pixel) + top_line[2] * 3;

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
    }

    int bottom_line[3] = {center[0] - obj_width / 2.0, center[1] + obj_height / 2.0, obj_width};
    start_pixel        = (bottom_line[0] + bottom_line[1] * img_width) * 3;
    end_pixel          = (start_pixel) + bottom_line[2] * 3;

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
    }

    int left_line[3] = {center[0] - obj_width / 2.0, center[1] - obj_height / 2.0, obj_height};
    start_pixel      = (left_line[0] + left_line[1] * img_width) * 3;
    end_pixel        = (start_pixel) + (left_line[2] * img_width * 3);

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        pixel += img_width * 3 - 3;
    }

    int right_line[3] = {center[0] + obj_width / 2.0, center[1] - obj_height / 2.0, obj_height};
    start_pixel       = (right_line[0] + right_line[1] * img_width) * 3;
    end_pixel         = (start_pixel) + (right_line[2] * img_width * 3);

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        pixel += img_width * 3 - 3;
    }
}

hsv rgb2hsv(rgb in) {
    hsv out;
    double min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min < in.b ? min : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max > in.b ? max : in.b;

    out.v = max;  // v
    delta = max - min;
    if (delta < 0.00001) {
        out.s = 0;
        out.h = 0;  // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) {            // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);  // s
    }
    else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;  // its now undefined
        return out;
    }
    if (in.r >= max)                    // > is bogus, just keeps compilor happy
        out.h = (in.g - in.b) / delta;  // between yellow & magenta
    else if (in.g >= max)
        out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

    out.h *= 60.0;  // degrees

    if (out.h < 0.0) out.h += 360.0;

    return out;
}

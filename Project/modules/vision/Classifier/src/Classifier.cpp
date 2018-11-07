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

#include "Classifier.hpp"

int Classifier::colours[3][3][2] = {
    {{64, 97}, {19, 48}, {19, 47}},    // Red maximum and minimum pixel parameters (RGB Image)
    {{1, 9}, {11, 49}, {8, 44}},       // Green maximum and minimum pixel parameters (RGB Image)
    {{14, 33}, {44, 61}, {79, 111}}};  // Blue maximum and minimum pixel parameters (RGB Image)
int Classifier::seed[1000][2];
int Classifier::object[10][4];


void Classifier_init() {
    std::cout << "Initilising CLASSIFIER" << std::endl;
}

void Classifier(uint8_t* data) {
    printf("into classifier \n");

    uint8_t* data_mod = new uint8_t[3 * Image::Width * Image::Height];
    data_mod          = data;

    int pixel         = 0;  // start at the first pixel of the image
    int colour        = 0;
    int i             = 0;
    int k             = 0;
    int objects_found = 0;
    int error         = 0;

    for (pixel = 0; pixel <= Image::Height * Image::Width * 3; pixel += 1800) {
        // printf("into pixel for loop \n");
        for (colour = 0; colour < 3; colour++) {
            // printf("into colour for loop \n");
            if (data[pixel] >= Classifier::colours[colour][0][0] && data[pixel] <= Classifier::colours[colour][0][1]
                && data[(pixel + 1)] >= Classifier::colours[colour][1][0]
                && data[(pixel + 1)] <= Classifier::colours[colour][1][1]
                && data[(pixel + 2)] >= Classifier::colours[colour][2][0]
                && data[(pixel + 2)] <= Classifier::colours[colour][2][1]) {
                Classifier::seed[i][0] = colour;
                Classifier::seed[i][1] = pixel;
                i++;
                break;
            }
        }
    }
    printf("seeds = %d\n", i);

    // cycle through seed points
    for (int j = 0; j <= i; j++) {
        // printf("into seed point \n");
        colour             = Classifier::seed[j][0];
        pixel              = Classifier::seed[j][1];
        int m              = 0;
        int seed_y         = (Classifier::seed[j][1] / 3.0) / Image::Width;
        int seed_x         = (Classifier::seed[j][1] / 3.0) - seed_y * Image::Width;
        int seed_in_object = 0;

        for (m = 0; m <= objects_found; m++) {
            // printf("into seed object check \n");
            int object_center = Image::Width * 3 * Classifier::object[m][1] + Classifier::object[m][0] * 3;
            int ax            = Classifier::object[m][1] - Classifier::object[m][3] * 3 / 2;
            int ay            = Classifier::object[m][0] - Classifier::object[m][2] * 3 / 2;
            int bx            = Classifier::object[m][1] + Classifier::object[m][3] * 3 / 2;
            int by            = Classifier::object[m][0] + Classifier::object[m][2] * 3 / 2;

            if (seed_x >= ax && seed_x <= bx && seed_y >= ay && seed_y <= by) {
                // seed is already in an object
                seed_in_object = 1;
                break;
            }
        }

        if (seed_in_object == 1) {
            // printf("SEED IN OBJECT IF LOOP BREAK \n");
        }

        else {
            // printf("into else, seed not in object \n");
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
            error = 0;
            while (error < 10) {
                // while (R_min <= data[pixel] && R_max >= data[pixel] &&
                //        G_min <= data[(pixel + 1)] && G_max >= data[(pixel + 1)] &&
                //        B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]) {
                if (R_min <= data[pixel] && R_max >= data[pixel] && G_min <= data[(pixel + 1)]
                    && G_max >= data[(pixel + 1)] && B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]) {
                }
                else {
                    error++;
                }
                right++;

                data_mod[pixel]     = {255};
                data_mod[pixel + 1] = {255};
                data_mod[pixel + 2] = {0};

                pixel += 3;
            }

            // cluster left
            pixel = Classifier::seed[j][1];
            error = 0;
            while (error < 10) {
                // while (R_min <= data[(pixel - 3)] && R_max >= data[(pixel - 3)] &&
                //        G_min <= data[(pixel - 2)] && G_max >= data[(pixel - 2)] &&
                //        B_min <= data[(pixel - 1)] && B_max >= data[(pixel - 1)]) {
                if (R_min <= data[(pixel - 3)] && R_max >= data[(pixel - 3)] && G_min <= data[(pixel - 2)]
                    && G_max >= data[(pixel - 2)] && B_min <= data[(pixel - 1)] && B_max >= data[(pixel - 1)]) {
                }
                else {
                    error++;
                }
                left++;

                data_mod[(pixel - 3)] = {100};
                data_mod[(pixel - 2)] = {255};
                data_mod[(pixel - 1)] = {50};

                pixel = pixel - 3;
            }

            // cluster up
            pixel = Classifier::seed[j][1];
            error = 0;
            while (error < 10) {
                // while (R_min <= data[pixel] && R_max >= data[pixel] &&
                //        G_min <= data[(pixel + 1)] && G_max >= data[(pixel + 1)] &&
                //        B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]) {
                if (R_min <= data[pixel] && R_max >= data[pixel] && G_min <= data[(pixel + 1)]
                    && G_max >= data[(pixel + 1)] && B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]) {
                }
                else {
                    error++;
                }
                up++;
                data_mod[pixel]       = {25};
                data_mod[(pixel + 1)] = {255};
                data_mod[(pixel + 2)] = {200};

                pixel = pixel - Image::Width * 3;
            }

            // cluster down
            pixel = Classifier::seed[j][1];
            error = 0;
            while (error < 10) {
                // while (R_min <= data[pixel] && R_max >= data[pixel] &&
                //        G_min <= data[(pixel + 1)] && G_max >= data[(pixel + 1)] &&
                //        B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]) {
                if (R_min <= data[pixel] && R_max >= data[pixel] && G_min <= data[(pixel + 1)]
                    && G_max >= data[(pixel + 1)] && B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]) {
                }
                else {
                    error++;
                }
                down++;

                data_mod[pixel]     = {255};
                data_mod[pixel + 1] = {25};
                data_mod[pixel + 2] = {200};

                pixel = pixel + Image::Width * 3;
            }

            // calulate object width and height from clustering
            int width  = left + right;
            int height = up + down;

            if (width >= WIDTH_MIN && width <= WIDTH_MAX) {
                printf("\nWIDTH IS GOOD\n\n");
                printf("Seed = %d\n", j);
                printf("pixel = %d\n", Classifier::seed[j][1]);
                int p = (Classifier::seed[j][1] / 3.0) / Image::Width;
                int q = (Classifier::seed[j][1] / 3.0) - p * Image::Width;

                printf("Pixel new %d, %d\n", q, p);

                printf("Left = %d \t Right = %d\n", left, right);
                printf("Up = %d \t \t Down = %d\n", up, down);
                printf("Width = %d \t Height = %d\n", width, height);

                data_mod[Classifier::seed[j][1]]     = {255};
                data_mod[Classifier::seed[j][1] + 1] = {255};
                data_mod[Classifier::seed[j][1] + 2] = {0};
            }

            // Check object size to target object parameters

            if (width >= WIDTH_MIN && width <= WIDTH_MAX && height >= HEIGHT_MIN && height <= HEIGHT_MAX) {

                objects_found++;

                printf("Hey! Shit's great %d, %d\n", up, down);

                // Calculate centers
                pixel        = Classifier::seed[j][1];
                int center_y = ((pixel - up * (Image::Width / 3)) + (pixel + down * (Image::Width / 3))) / 2;
                center_y     = (center_y / 3.0) / Image::Width;
                int center_x = ((pixel - left) + (pixel + right)) / 2;
                center_x     = (center_x / 3.0) - center_y * Image::Width;

                Classifier::object[k][0] = center_x;
                Classifier::object[k][1] = center_y;
                Classifier::object[k][2] = width;
                Classifier::object[k][3] = height;

                printf("Object center %d, %d\n", center_x, center_y);
                printf("Object %d, width %d, height %d\n", objects_found, width, height);

                int obj_center[2] = {Classifier::object[k][0], Classifier::object[k][1]};
                int obj_width     = Classifier::object[k][2];
                int obj_height    = Classifier::object[k][3];
                Output_Segmentation(data, 1280, 960, obj_width, obj_height, obj_center);

                k++;
            }
        }
    }
    printf("Objects found = %d \n", objects_found);

    printf("Saving file\n");
    std::ofstream outfile("Classified_Image.ppm", std::ios::binary);
    outfile << "P6\n" << Image::Width << " " << Image::Height << " 255\n";  // dont know
    outfile.write((char*) data_mod, (Image::Height * Image::Width * 3));
    printf("Segmented_Image Saved\n");

    return;
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
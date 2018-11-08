
#include "Classifier_tester.hpp"

#define WIDTH_MIN 100
#define WIDTH_MAX 300
#define HEIGHT_MIN 100
#define HEIGHT_MAX 300

#define IMAGE_HEIGHT 960
#define IMAGE_WIDTH 1280

int Image::Width  = 1280;
int Image::Height = 960;

const double Camera::pixel_x      = 1.12;  // um
const double Camera::pixel_y      = 1.12;  // um
const double Camera::resolution_x = 3280;
const double Camera::resolution_y = 2464;
const double Camera::focal_len    = 3.04;  // mm

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

void Find_distance(int u, int v) {

    double p = (Camera::pixel_x * 0.000001) * (u - Image::Width / 2.0);   // The screen coordinates returned
    double q = (Camera::pixel_y * 0.000001) * (v - Image::Height / 2.0);  // The screen coordinates returned

    // Now find the angle the pixel is offset from the screen origin
    double theta = std::atan(p / Camera::focal_len);  // The vertical angle
    double phi   = std::atan(q / Camera::focal_len);  // The horizontal angle

    // Now convert the offsets into world coordinates
    // Vertical
    double plane_offset = 0;
    double dist_x       = (Kinematics::cam_height - plane_offset) * std::tan(M_PI_2 - theta - Kinematics::cam_phi);
    std::cout << "Distance x " << dist_x << std::endl;

    // Horizontal
    double dist_y =
        (Kinematics::cam_height - plane_offset) / std::cos(M_PI_2 - theta - Kinematics::cam_phi) * std::tan(phi);
    std::cout << "Distance y " << dist_y << std::endl;

    // These should be the components of x and y of the object from a pixel
}

int Image_Size = IMAGE_HEIGHT * IMAGE_WIDTH * 3;
// colours[How many colours][R,G,B][Min,Max]
int colours[3][3][2] = {
    {{51, 100}, {0, 40}, {0, 41}},  // Red maximum and minimum pixel parameters (RGB Image)
    {{2, 33}, {0, 54}, {0, 40}},    // Green maximum and minimum pixel parameters (RGB Image)
    {{5, 65}, {0, 102}, {0, 146}}   // Blue maximum and minimum pixel parameters (RGB Image)
};

// int seed[][] = { {colour, pixel}, {} }
int seed[1000][2];

// int object[][] = {{center_x, center_y, width, height}, {} }
int object[40][4];

void Classifier(uint8_t* data) {
    printf("into classifier \n");

    uint8_t* data_mod = new uint8_t[3 * IMAGE_WIDTH * IMAGE_HEIGHT];
    data_mod          = data;

    int pixel         = 0;  // start at the first pixel of the image
    int colour        = 0;
    int i             = 0;
    int k             = 0;
    int objects_found = 0;
    int error         = 0;

    for (pixel = 0; pixel <= Image_Size; pixel += 4200) {
        // printf("into pixel for loop \n");
        for (colour = 0; colour < 3; colour++) {
            // printf("into colour for loop \n");
            if (data[pixel] >= colours[colour][0][0] && data[pixel] <= colours[colour][0][1]
                && data[(pixel + 1)] >= colours[colour][1][0] && data[(pixel + 1)] <= colours[colour][1][1]
                && data[(pixel + 2)] >= colours[colour][2][0] && data[(pixel + 2)] <= colours[colour][2][1]) {
                seed[i][0] = colour;
                seed[i][1] = pixel;
                i++;
                break;
            }
        }
    }
    printf("seeds = %d\n", i);

    // cycle through seed points
    for (int j = 0; j <= i; j++) {
        // printf("into seed point \n");
        colour             = seed[j][0];
        pixel              = seed[j][1];
        int m              = 0;
        int seed_y         = (seed[j][1] / 3.0) / IMAGE_WIDTH;
        int seed_x         = (seed[j][1] / 3.0) - seed_y * IMAGE_WIDTH;
        int seed_in_object = 0;

        for (m = 0; m <= objects_found; m++) {
            // printf("into seed object check \n");
            int object_center = IMAGE_WIDTH * 3 * object[m][1] + object[m][0] * 3;
            int ax            = object[m][1] - object[m][3] * 3 / 2;
            int ay            = object[m][0] - object[m][2] * 3 / 2;
            int bx            = object[m][1] + object[m][3] * 3 / 2;
            int by            = object[m][0] + object[m][2] * 3 / 2;

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

            int R_min = colours[colour][0][0];
            int R_max = colours[colour][0][1];
            int G_min = colours[colour][1][0];
            int G_max = colours[colour][1][1];
            int B_min = colours[colour][2][0];
            int B_max = colours[colour][2][1];

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
            pixel = seed[j][1];
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
            pixel = seed[j][1];
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

                pixel = pixel - IMAGE_WIDTH * 3;
            }

            // cluster down
            pixel = seed[j][1];
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

                pixel = pixel + IMAGE_WIDTH * 3;
            }

            // calulate object width and height from clustering
            int width  = left + right;
            int height = up + down;

            if (width >= WIDTH_MIN && width <= WIDTH_MAX) {
                // printf("\nWIDTH IS GOOD\n\n");
                // printf("Seed = %d\n", j);
                // printf("pixel = %d\n", seed[j][1]);
                int p = (seed[j][1] / 3.0) / IMAGE_WIDTH;
                int q = (seed[j][1] / 3.0) - p * IMAGE_WIDTH;

                // printf("Pixel new %d, %d\n", q, p);

                // printf("Left = %d \t Right = %d\n", left, right);
                // printf("Up = %d \t \t Down = %d\n", up, down);
                // printf("Width = %d \t Height = %d\n", width, height);

                data_mod[seed[j][1]]     = {255};
                data_mod[seed[j][1] + 1] = {255};
                data_mod[seed[j][1] + 2] = {0};
            }

            // Check object size to target object parameters

            if (width >= WIDTH_MIN && width <= WIDTH_MAX && height >= HEIGHT_MIN && height <= HEIGHT_MAX) {

                objects_found++;

                printf("Hey! Shit's great %d, %d\n", up, down);

                // Calculate centers
                pixel        = seed[j][1];
                int center_y = ((pixel - up * (IMAGE_WIDTH / 3)) + (pixel + down * (IMAGE_WIDTH / 3))) / 2;
                center_y     = (center_y / 3.0) / IMAGE_WIDTH;
                int center_x = ((pixel - left) + (pixel + right)) / 2;
                center_x     = (center_x / 3.0) - center_y * IMAGE_WIDTH;

                object[k][0] = center_x;
                object[k][1] = center_y;
                object[k][2] = width;
                object[k][3] = height;

                printf("Object center %d, %d\n", center_x, center_y);
                printf("Object %d, width %d, height %d\n", objects_found, width, height);

                int obj_center[2] = {object[k][0], object[k][1]};
                int obj_width     = object[k][2];
                int obj_height    = object[k][3];
                Output_Segmentation(data, 1280, 960, obj_width, obj_height, obj_center);

                find_distance(obj_center[0], obj_center[1]);

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

/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

uint8_t* readPPM(const char* fileName, char* pSix, int* width, int* height, int* maximum) {

    // open the file to read just the header reading
    FILE* fr = fopen(fileName, "rb");

    // formatted read of header
    fscanf(fr, "%s", pSix);

    // check to see if it's a PPM image file
    if (strncmp(pSix, "P6", 10) != 0) {
        printf("They are not the same\n");
    }
    else {
        printf("They are the same\n");
    }

    // read the rest of header
    fscanf(fr, "%d\n %d\n", width, height);

    fscanf(fr, "%d\n", maximum);

    // check to see if they were stored properly
    printf("PSix: %s\n", pSix);
    printf("Width: %d\n", *width);
    printf("Height: %d\n", *height);
    printf("maximum: %d\n", *maximum);

    int size = (*width) * (*height) * 3;
    // int size = 423800;
    printf("Size: %d\n", size);
    // allocate array for pixels
    uint8_t* pixels = new uint8_t[size];

    // unformatted read of binary pixel data
    while (fread(pixels, sizeof(uint8_t), size, fr)) {
        // printf("%s", pixels);
    }  // end of for loop

    // close file
    fclose(fr);

    // return the array
    return pixels;

}  // end of readPPM

int main() {

    char fileName[50] = "Image_2.ppm";
    char pSix[10];      // indicates this is a PPM image
    int width   = 0;    // width of the image
    int height  = 0;    // heigt of the image
    int maximum = 0;    // maximum pixel value
    int size    = 128;  // size of the array

    // read the PPM file and store its contents inside an array and return the
    // pointer to that array to pixelArray
    uint8_t* data = readPPM(fileName, pSix, &width, &height, &maximum);

    Image::Height = height;
    Image::Width  = width;

    // TODO Do classifier stuff here
    // classifier returns a list of objects, and some information about them
    // segmentation can draw debug rectangles around them

    Classifier(data);

    // int obj_width = 79;             // Pixel values
    // int obj_height = 105;           // Pixel values
    // int obj_center[2] = {447, 218}; // Pixel values

    // find_distance(obj_center[0], obj_center[1]);

    // Output_Segmentation(data, Image::Width, Image::Height, obj_width,
    // obj_height,
    //                     obj_center);

    // printf("Saving file\n");
    // std::ofstream outfile("Classified_Image.ppm", std::ios::binary);
    // outfile << "P6\n" << Image::Width << " " << Image::Height << " 255\n";  // dont know
    // outfile.write((char*) data, (Image::Height * Image::Width * 3));
    // printf("Segmented_Image Saved\n");

    // delete seg_image_array;

    return 0;
}

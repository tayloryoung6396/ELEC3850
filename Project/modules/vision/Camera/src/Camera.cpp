/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

#include "Camera.hpp"

int Image::Width  = 1280;
int Image::Height = 960;

const double Camera::pixel_x      = 1.12e-6;  // um
const double Camera::pixel_y      = 1.12e-6;  // um
const double Camera::resolution_x = 3280;
const double Camera::resolution_y = 2464;
const double Camera::focal_len    = 3.04e-3;  // mm

int seed[1000][2];
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


void Camera_init() {
    std::cout << "Initilising CAMERA" << std::endl;

    if (!Camera.open()) {  // Error mesage if camera didnt open

        printf("Error: Camera could not open\n");
        return;
    }
    Camera.setCaptureSize(640, 480);
    Camera.setExposure(raspicam::RASPICAM_EXPOSURE_AUTO);
    Camera.setHorizontalFlip(1);
    Camera.setVerticalFlip(1);
    Image::Height = Camera.getHeight();
    Image::Width  = Camera.getWidth();

    delay(3000);  // Delay 3 seconds camera to stabalise
}

uint8_t* data = new uint8_t[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];

int Camera_main() {
    static int count = 0;
    Camera.grab();
    //uint8_t* data = new uint8_t[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];

    Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);  // Extract image in rgb format
//    printf("data size %d\n", Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
//    printf("data size %d\n", Image::Height * Image::Width * 3);
//    Classifier(data);
    //count++;
    printf("Saving file\n");
    std::ofstream outfile("Image.ppm", std::ios::binary);
    outfile << "P6\n" << Image::Width << " " << Image::Height << " 255\n";  // dont know
    outfile.write((char*) data, (Image::Height * Image::Width * 3));
    printf("Segmented_Image Saved\n");
printf("Running Classifier\n");  
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

/*    std::ofstream outfile("Image_Classified.ppm", std::ios::binary);
    outfile << "P6\n" << Image::Width << " " << Image::Height << " 255\n";  // dont know
    outfile.write((char*) data, (Image::Height * Image::Width * 3));
    printf("Classified_Image Saved\n");
    delete data;
*/
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
                printf("into seed object check \n");
                int errorx = 200;
                int errory = 50;

                int object_center = Image::Width * 3 * Classifier::object[m][1] + Classifier::object[m][0] * 3;
                int ax            = Classifier::object[m][1] - Classifier::object[m][3] * 3 / 2.0;
                int ay            = Classifier::object[m][0] - Classifier::object[m][2] * 3 / 2.0;
                int bx            = Classifier::object[m][1] + Classifier::object[m][3] * 3 / 2.0;
                int by            = Classifier::object[m][0] + Classifier::object[m][2] * 3 / 2.0;

                printf("Object Center = %d \t ax = %d \t ay = %d \t bx = %d \t by = %d \t\n",
                       object_center,
                       ax,
                       ay,
                       bx,
                       by);
                printf("Seed = %d \t Seed_x = %d \t Seed_y = %d \t\n\n", pixel, seed_x, seed_y);

                if (seed_x >= (ax - errorx) && seed_x <= (bx + errorx) && seed_y >= ay - (errory)
                    && seed_y <= (by + errory)) {
                    // seed is already in an object
                    seed_in_object = 1;
                    printf("SEED IN OBJECT \n\n");
                    break;
                }
            }
        }

        if (seed_in_object == 1) {
            printf("SEED IN OBJECT IF LOOP BREAK \n");
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

                printf("Object %d, width %d, height %d, center x %d, center y %d, width %d height %d\n",
                       objects_found,
                       width,
                       height,
                       center_x,
                       center_y,
                       width,
                       height);
                printf("pixel = %d, left = %d, right = %d, y = %d, x = %d, up = %d, down = %d\n",
                       pixel,
                       left,
                       right,
                       y,
                       x,
                       up,
                       down);

                printf("Object found seed = %d\n", Classifier::object[k][1]);

                k++;
            }
        }
    }
    printf("Objects found = %d \n", objects_found);

  //delete data;

    return 0;
}

std::string make_filename(const std::string& basename, int index, const std::string& ext) {
    std::ostringstream result;
    result << basename << index << ext;
    return result.str();
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


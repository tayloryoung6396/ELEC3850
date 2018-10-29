/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

#include "FakeCamera.hpp"

int Image::Width  = 1280;
int Image::Height = 960;

const double Camera::pixel_x      = 1.12;  // um
const double Camera::pixel_y      = 1.12;  // um
const double Camera::resolution_x = 3280;
const double Camera::resolution_y = 2464;
const double Camera::focal_len    = 3.04;  // mm

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


int main() {

    char fileName[50] = "test.ppm";
    char pSix[10];      // indicates this is a PPM image
    int width   = 0;    // width of the image
    int height  = 0;    // heigt of the image
    int maximum = 0;    // maximum pixel value
    int size    = 128;  // size of the array

    // read the PPM file and store its contents inside an array and return the pointer to that array to pixelArray
    uint8_t* data = readPPM(fileName, pSix, &width, &height, &maximum);

    Image::Height = height;
    Image::Width  = width;

    // TODO Do classifier stuff here
    // classifier returns a list of objects, and some information about them
    // segmentation can draw debug rectangles around them

    int obj_width     = 100;         // Pixel values
    int obj_height    = 100;         // Pixel values
    int obj_center[2] = {100, 500};  // Pixel values

    // uint8_t* seg_image_array = new uint8_t[(Image::Height * Image::Width * 3)];

    Output_Segmentation(data, Image::Width, Image::Height, obj_width, obj_height, obj_center);

    printf("Saving file\n");
    std::ofstream outfile("Segmented_Image.ppm", std::ios::binary);
    outfile << "P6\n" << Image::Width << " " << Image::Height << " 255\n";  // dont know
    outfile.write((char*) data, (Image::Height * Image::Width * 3));
    printf("Segmented_Image Saved\n");

    // delete seg_image_array;

    return 0;
}

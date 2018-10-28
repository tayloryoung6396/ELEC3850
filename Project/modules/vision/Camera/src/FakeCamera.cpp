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

unsigned char* readPPM(const char* fileName, char* pSix, int* width, int* height, int* maximum) {

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
    unsigned char* pixels = new unsigned char[size];

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

    char fileName[50] = "test.ppm";
    char pSix[10];      // indicates this is a PPM image
    int width   = 0;    // width of the image
    int height  = 0;    // heigt of the image
    int maximum = 0;    // maximum pixel value
    int size    = 128;  // size of the array

    // read the PPM file and store its contents inside an array and return the pointer to that array to pixelArray
    unsigned char* data = readPPM(fileName, pSix, &width, &height, &maximum);

    Image::Height = height;
    Image::Width  = width;

    // TODO Do classifier stuff here

    printf("Saving file\n");
    std::ofstream outfile("Red_Image.ppm", std::ios::binary);
    outfile << "P6\n" << Image::Width << " " << Image::Height << " 255\n";  // dont know
    outfile.write((char*) data, (Image::Height * Image::Width * 3));
    printf("Red Image Saved\n");

    delete data;

    return 0;
}

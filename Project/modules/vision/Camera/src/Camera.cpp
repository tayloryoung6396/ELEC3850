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

void Camera_init() {
    std::cout << "Initilising CAMERA" << std::endl;

    if (!Camera.open()) {  // Error mesage if camera didnt open

        printf("Error: Camera could not open\n");
        return;
    }
    Camera.setExposure(RASPICAM_EXPOSURE_AUTO);
    Camera.setHorizontalFlip(1);
    Camera.setVerticalFlip(1);
    Image::Height = Camera.getHeight();
    Image::Width  = Camera.getWidth();

    delay(3000);  // Delay 3 seconds camera to stabalise
}

int Camera_main() {
    static int count = 0;
    Camera.grab();
    uint8_t* data = new uint8_t[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];

    Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);  // Extract image in rgb format

    Classifier(data);
    count++;
    printf("Saving file\n");
    std::ofstream outfile(make_filename("Raw_image_", count, ".ppm").c_str(), std::ios::binary);
    outfile << "P6\n" << Image::Width << " " << Image::Height << " 255\n";  // dont know
    outfile.write((char*) data, (Image::Height * Image::Width * 3));
    printf("Segmented_Image Saved\n");
    delete data;

    return 0;
}

std::string make_filename(const std::string& basename, int index, const std::string& ext) {
    std::ostringstream result;
    result << basename << index << ext;
    return result.str();
}

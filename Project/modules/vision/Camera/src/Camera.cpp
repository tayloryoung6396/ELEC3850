/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

#include "Camera.hpp"

int Image::Width  = 1280;
int Image::Height = 960;

void Camera_init() {
    std::cout << "Initilising CAMERA" << std::endl;
    //	raspicam::RaspiCam Camera;
    if (!Camera.open()) {  // Error mesage if camera didnt open

        printf("Error: Camera could not open\n");
        return;
    }
    Camera.setHorizontalFlip(1);
    Camera.setVerticalFlip(1);
    Image::Height = Camera.getHeight();
    Image::Width  = Camera.getWidth();

    delay(3000);  // Delay 3 seconds camera to stabalise
}

int Camera_main() {

    delay(3000);  // Delay 3 seconds camera to stabalise

    Camera.grab();
    uint8_t* data           = new uint8_t[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
    uint8_t* Red_data_array = new uint8_t[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];

    Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);  // Extract image in rgb format
                                                           // for (int Height = 0; Height < Image::Height; Height++) {
    // for (int Width = 0; Width < Image::Width;) {
    for (int pixel = 0; pixel < Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB);) {
        Red_data_array[pixel] = data[pixel]]; // Width * Image::Height + Height
        pixel++;
        Red_data_array[pixel] = 0;  // Width * Image::Height + Height
        pixel++;
        Red_data_array[pixel] = 0;  // Width * Image::Height + Height
        pixel++;
        //}
        //}

        printf("Saving file\n");
        std::ofstream outfile("Red_Image.ppm", std::ios::binary);                         // Save Image
        outfile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";  // dont know
        outfile.write((char*) Red_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
        printf("Red Image Saved\n");

        delete Red_data_array;
        delete data;

        return 0;
    }

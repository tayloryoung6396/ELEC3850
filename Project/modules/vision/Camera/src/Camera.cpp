/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

#include "Camera.hpp"

void Camera_init() {
    std::cout << "Initilising CAMERA" << std::endl;
    //	raspicam::RaspiCam Camera;
    if (!Camera.open()) {  // Error mesage if camera didnt open

        printf("Error: Camera could not open\n");
        return;
    }
    Camera.setHorizontalFlip(1);
    Camera.setVerticalFlip(1);

    delay(3000);  // Delay 3 seconds camera to stabalise
    // sleep(3);
    // capture
    // Camera.grab();
    // allocate memory
    //  unsigned char* data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
    // extract the image in rgb format
    // Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);  // get camera image
    // save
    //    std::ofstream outFile("raspicam_image.ppm", std::ios::binary);
    //  outFile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";
    //  outFile.write((char*) data, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
    //  std::cout << "Image saved at raspicam_image.ppm" << std::endl;
    // free resrources
    //  delete data;
}

int Camera_main() {
    // VARIABLES
    unsigned char* Red_data_array = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];

    delay(3000);  // Delay 3 seconds camera to stabalise

    Camera.grab();
    unsigned char* data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];

    Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);  // Extract image in rgb format
    int i = 0;
    for (int Height = 0; Height < IMAGE_HEIGHT; Height++) {

        for (int Width = 0; Width < IMAGE_WIDTH;) {
            Red_data_array[2 * Height + Width] = data[2 * Height + Width];
            std::cout << 2 * Height + Width << std::endl;
            Width++;
            Red_data_array[2 * Height + Width] = 0;
            std::cout << 2 * Height + Width << std::endl;
            Width++;
            Red_data_array[2 * Height + Width] = 0;
            std::cout << 2 * Height + Width << std::endl;
            Width++;
        }
    }

    printf("Saving file\n");
    std::ofstream outfile("Red_Image.ppm", std::ios::binary);                         // Save Image
    outfile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";  // dont know
    outfile.write((char*) Red_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
    outfile.write((char*) data, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
    printf("Red Image Saved\n");

    delete Red_data_array;
    delete data;

    return 0;
}

/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

#include "CameraCV.hpp"

void Camera_init() {
    std::cout << "Initilising CAMERA" << std::endl;
    //	raspicam::RaspiCam Camera;
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);

    if (!Camera.open()) {  // Error mesage if camera didnt open

        printf("Error: Camera could not open\n");
        return;
    }

    delay(3000);
}

int Camera_main() {


    cv::Mat image;
    Camera.grab();
    Camera.retrieve(image);
    cv::imwrite("raspicam_cv_image.jpg", image);
    std::cout << "Image saved at raspicam_cv_image.jpg" << std::endl;


    return 0;
}

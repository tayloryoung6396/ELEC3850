#ifndef CAMERA_H_
#define CAMERA_H_

#include "main.hpp"

static raspicam::RaspiCam Camera;  // Open camera
class Image {
public:
    static int Height;
    static int Width;
};

class Camera {
public:
    static const double pixel_x;
    static const double pixel_y;
    static const double resolution_x;
    static const double resolution_y;
    static const double focal_len;
};

extern void Camera_init();
extern int Camera_main();

#endif /* CAMERA_H_ */

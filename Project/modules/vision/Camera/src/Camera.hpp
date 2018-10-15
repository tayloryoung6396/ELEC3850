#ifndef CAMERA_H_
#define CAMERA_H_

#include "main.hpp"

static raspicam::RaspiCam Camera;  // Open camera
class Image {
public:
    static int Height;
    static int Width;
};


extern void Camera_init();
extern int Camera_main();

#endif /* CAMERA_H_ */

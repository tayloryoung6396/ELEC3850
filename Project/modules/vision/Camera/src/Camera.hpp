#ifndef CAMERA_H_
#define CAMERA_H_

#include "main.hpp"

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 520

static raspicam::RaspiCam Camera;  // Open camera

extern void Camera_init();
extern int Camera_main();

#endif /* CAMERA_H_ */

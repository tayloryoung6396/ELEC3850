#ifndef CAMERA_H_
#define CAMERA_H_

#include "main.hpp"

#define IMAGE_WIDTH 1280
#define IMAGE_HEIGHT 960

// static raspicam::RaspiCam Camera;  // Open camera
static raspicam::RaspiCam_Cv Camera;

extern void Camera_init();
extern int Camera_main();

#endif /* CAMERA_H_ */

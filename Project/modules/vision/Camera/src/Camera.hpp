#ifndef CAMERA_H_
#define CAMERA_H_

#include "main.hpp"

#include <cstdio>
#include <string>


#define IMAGE_WIDTH 320			
#define IMAGE_HEIGHT 520

raspicam::RaspiCam Camera;						//Open camera

extern void Camera_init();
extern int Camera_main(int argc, const char **argv);

#endif /* CAMERA_H_ */

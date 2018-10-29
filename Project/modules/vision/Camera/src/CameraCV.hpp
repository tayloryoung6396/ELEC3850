#ifndef CAMERACV_H_
#define CAMERACV_H_

#include "main.hpp"

#define IMAGE_WIDTH 1280
#define IMAGE_HEIGHT 960

static raspicam::RaspiCam_Cv Camera;

extern void Camera_init();
extern int Camera_main();

#endif /* CAMERACV_H_ */

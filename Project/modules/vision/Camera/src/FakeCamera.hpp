// #ifndef CAMERA_H_
// #define CAMERA_H_

// #include "main.hpp"

// static raspicam::RaspiCam Camera;  // Open camera
#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

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

// #endif /* CAMERA_H_ */

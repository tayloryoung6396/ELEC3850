// #ifndef CAMERA_H_
// #define CAMERA_H_

// #include "main.hpp"

// static raspicam::RaspiCam Camera;  // Open camera
#include <errno.h>
#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cfloat>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <stack>
#include <string>
#include <vector>

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

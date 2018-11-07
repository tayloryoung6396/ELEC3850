void Output_Segmentation(uint8_t* seg_image_array,
                         int img_width,
                         int img_height,
                         int obj_width,
                         int obj_height,
                         int center[2]) {

    // Find the start of our lines to draw
    int top_line[3] = {center[0] - obj_width / 2.0, center[1] - obj_height / 2.0, obj_width};
    int start_pixel = (top_line[0] + top_line[1] * img_width) * 3;
    int end_pixel   = (start_pixel) + top_line[2] * 3;

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
    }

    int bottom_line[3] = {center[0] - obj_width / 2.0, center[1] + obj_height / 2.0, obj_width};
    start_pixel        = (bottom_line[0] + bottom_line[1] * img_width) * 3;
    end_pixel          = (start_pixel) + bottom_line[2] * 3;

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
    }

    int left_line[3] = {center[0] - obj_width / 2.0, center[1] - obj_height / 2.0, obj_height};
    start_pixel      = (left_line[0] + left_line[1] * img_width) * 3;
    end_pixel        = (start_pixel) + (left_line[2] * img_width * 3);

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        pixel += img_width * 3 - 3;
    }

    int right_line[3] = {center[0] + obj_width / 2.0, center[1] - obj_height / 2.0, obj_height};
    start_pixel       = (right_line[0] + right_line[1] * img_width) * 3;
    end_pixel         = (start_pixel) + (right_line[2] * img_width * 3);

    // Draw rectangle around our object
    for (int pixel = start_pixel; pixel < end_pixel;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 0;  // TODO select a colour
        pixel++;
        pixel += img_width * 3 - 3;
    }
}

// Camera rotation stuffs
// There are 4 coordinate systems..
// The backwards projection
// Image -> Screen -> Camera -> World

// Image coordinates
// These are the pixels in the image
// Easiest to measure from the top left
// Image[u, v]

// Screen coordinates
// These are measured from the center of the image, the reason of which will be revealed later.
// The orientation of this system doesn't matter, but for simplicity let it be the same as the image coordinates
// Screen[p, q]

// Camera coordinates
// This is the rotation and translation of the camera, this is usually represented as a 4x4 matrix
// However, since we only have 1 rotation and the translation doesn't matter, let's kind of ignore this

// World coordinates (tank coordinates)
// This is the physical distance that the pixel represents in the world


void test() {
    int u;  // Some pixel that you are interested in
    int v;  // Some pixel that you are interested in

    double p = (Camera::pixel_x * 0.000001) * (u - Image::Width / 2.0);   // The screen coordinates returned
    double q = (Camera::pixel_y * 0.000001) * (v - Image::Height / 2.0);  // The screen coordinates returned

    // Now find the angle the pixel is offset from the screen origin
    double theta = std::atan(p / Camera::focal_len);  // The vertical angle
    double phi   = std::atan(q / Camera::focal_len);  // The horizontal angle

    // Now convert the offsets into world coordinates
    // Vertical
    double plane_offset = 0;
    double dist_x       = (Kinematics::cam_height - plane_offset) * std::tan(M_PI_2 - theta - Kinematics::cam_phi);

    // Horizontal
    double dist_y =
        (Kinematics::cam_height - plane_offset) / std::cos(M_PI_2 - theta - Kinematics::cam_phi) * std::tan(phi);

    // These should be the components of x and y of the object from a pixel
}
int Output_Segmentation(uint8_t* seg_image_array, int width, int height, int center[2]) {

    // Find the start of our lines to draw
    int top_line[3] = {center[0] - width / 2.0, center[1] - height / 2.0, width};

    // Draw rectangle around our object
    for (int pixel = (); pixel < (top_line[3] * 3) + ;) {
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
        seg_image_array[pixel] = 255;  // TODO select a colour
        pixel++;
    }

    int bottom_line[3] = {center[0] - width / 2.0, center[1] + height / 2.0, width};
    int left_line[3]   = {center[0] - width / 2.0, center[1] - height / 2.0, heigth};
    int right_line[3]  = {center[0] + width / 2.0, center[1] - height / 2.0, heigth};

    printf("Saving file\n");
    std::ofstream outfile("Segmented_image.ppm", std::ios::binary);                   // Save Image
    outfile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";  // dont know
    outfile.write((char*) seg_image_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
    printf("Segmented Image Saved\n");

    delete seg_image_array;
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
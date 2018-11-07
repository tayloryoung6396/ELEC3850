#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
typedef struct {
  double r; // a fraction between 0 and 1
  double g; // a fraction between 0 and 1
  double b; // a fraction between 0 and 1
} rgb;

typedef struct {
  double h; // angle in degrees
  double s; // a fraction between 0 and 1
  double v; // a fraction between 0 and 1
} hsv;

static hsv rgb2hsv(rgb in);
static rgb hsv2rgb(hsv in);

hsv rgb2hsv(rgb in) {
  hsv out;
  double min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min < in.b ? min : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max > in.b ? max : in.b;

  out.v = max; // v
  delta = max - min;
  if (delta < 0.00001) {
    out.s = 0;
    out.h = 0; // undefined, maybe nan?
    return out;
  }
  if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max); // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, h is undefined
    out.s = 0.0;
    out.h = NAN; // its now undefined
    return out;
  }
  if (in.r >= max)                 // > is bogus, just keeps compilor happy
    out.h = (in.g - in.b) / delta; // between yellow & magenta
  else if (in.g >= max)
    out.h = 2.0 + (in.b - in.r) / delta; // between cyan & yellow
  else
    out.h = 4.0 + (in.r - in.g) / delta; // between magenta & cyan

  out.h *= 60.0; // degrees

  if (out.h < 0.0)
    out.h += 360.0;

  return out;
}

rgb hsv2rgb(hsv in) {
  double hh, p, q, t, ff;
  long i;
  rgb out;

  if (in.s <= 0.0) { // < is bogus, just shuts up warnings
    out.r = in.v;
    out.g = in.v;
    out.b = in.v;
    return out;
  }
  hh = in.h;
  if (hh >= 360.0)
    hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = in.v * (1.0 - in.s);
  q = in.v * (1.0 - (in.s * ff));
  t = in.v * (1.0 - (in.s * (1.0 - ff)));

  switch (i) {
  case 0:
    out.r = in.v;
    out.g = t;
    out.b = p;
    break;
  case 1:
    out.r = q;
    out.g = in.v;
    out.b = p;
    break;
  case 2:
    out.r = p;
    out.g = in.v;
    out.b = t;
    break;

  case 3:
    out.r = p;
    out.g = q;
    out.b = in.v;
    break;
  case 4:
    out.r = t;
    out.g = p;
    out.b = in.v;
    break;
  case 5:
  default:
    out.r = in.v;
    out.g = p;
    out.b = q;
    break;
  }
  return out;
}

uint8_t *readPPM(const char *fileName, char *pSix, int *width, int *height,
                 int *maximum) {

  // open the file to read just the header reading
  FILE *fr = fopen(fileName, "rb");

  // formatted read of header
  fscanf(fr, "%s", pSix);

  // check to see if it's a PPM image file
  if (strncmp(pSix, "P6", 10) != 0) {
    printf("They are not the same\n");
  } else {
    printf("They are the same\n");
  }

  // read the rest of header
  fscanf(fr, "%d\n %d\n", width, height);

  fscanf(fr, "%d\n", maximum);

  // check to see if they were stored properly
  printf("PSix: %s\n", pSix);
  printf("Width: %d\n", *width);
  printf("Height: %d\n", *height);
  printf("maximum: %d\n", *maximum);

  int size = (*width) * (*height) * 3;
  // int size = 423800;
  printf("Size: %d\n", size);
  // allocate array for pixels
  uint8_t *pixels = new uint8_t[size];

  // unformatted read of binary pixel data
  while (fread(pixels, sizeof(uint8_t), size, fr)) {
    // printf("%s", pixels);
  } // end of for loop

  // close file
  fclose(fr);

  // return the array
  return pixels;

} // end of readPPM

int main() {
  int current_max[3] = {0};
  int current_min[3] = {255};

  // int min_row = 98 * 3;
  // int max_row = 308 * 3;
  // int min_col = 303 * 3;
  // int max_col = 429 * 3;

  // int min_row = 590 * 3;
  // int max_row = 717 * 3;
  // int min_col = 266 * 3;
  // int max_col = 356 * 3;

  int min_row = 1046 * 3;
  int max_row = 1170 * 3;
  int min_col = 260 * 3;
  int max_col = 347 * 3;

  // int min_row = 413 * 3;
  // int max_row = 521 * 3;
  // int min_col = 215 * 3;
  // int max_col = 288 * 3;

  // int min_row = 633 * 3;
  // int max_row = 714 * 3;
  // int min_col = 211 * 3;
  // int max_col = 272 * 3;

  char fileName[50] = "Image_2.ppm";
  char pSix[10];   // indicates this is a PPM image
  int width = 0;   // width of the image
  int height = 0;  // heigt of the image
  int maximum = 0; // maximum pixel value
  int size = 128;  // size of the array

  // read the PPM file and store its contents inside an array and return the
  // pointer to that array to pixelArray
  uint8_t *data = readPPM(fileName, pSix, &width, &height, &maximum);

  // for (int row = min_row; row < max_row; row += 3) {
  //   for (int col = min_col; col < max_col; col += 3) {
  //     for (int colour = 0; colour < 3; colour++) {
  //       if (data[col * width + row + colour] > current_max[colour]) {
  //         current_max[colour] = data[col * width + row + colour];
  //       } else if (data[col * width + row + colour] < current_min[colour]) {
  //         current_min[colour] = data[col * width + row + colour];
  //       }
  //     }
  //   }
  // }
  // std::cout << "Red \t" << current_min[0] << "\tGreen " << current_min[1]
  //           << "\tBlue " << current_min[2] << "\t" << std::endl;
  // std::cout << "Red \t" << current_max[0] << "\tGreen " << current_max[1]
  //           << "\tBlue " << current_max[2] << "\t" << std::endl;
  int max_h = 0;
  int min_h = 255;
  int max_s = 0;
  int min_s = 255;
  int max_v = 0;
  int min_v = 255;

  for (int row = min_row; row < max_row; row += 3) {
    for (int col = min_col; col < max_col; col += 3) {
      rgb rgb_colour;
      hsv hsv_colour;
      rgb_colour.r = data[col * width + row + 0] / (double)255;
      rgb_colour.g = data[col * width + row + 1] / (double)255;
      rgb_colour.b = data[col * width + row + 2] / (double)255;
      hsv_colour = rgb2hsv(rgb_colour);
      if (hsv_colour.h > max_h) {
        max_h = hsv_colour.h;
        // std::cout << "max_h " << max_h << "\t" << hsv_colour.h << std::endl;
      } else if (hsv_colour.h < min_h) {
        min_h = hsv_colour.h;
        // std::cout << "min_h " << min_h << "\t" << hsv_colour.h << std::endl;
      }
      if (hsv_colour.s * 255 > max_s) {
        max_s = hsv_colour.s * 255;
        // std::cout << "max_s " << max_s << "\t" << hsv_colour.s << std::endl;
      } else if (hsv_colour.s * 255 < min_s) {
        min_s = hsv_colour.s * 255;
        // std::cout << "min_s " << min_s << "\t" << hsv_colour.s << std::endl;
      }
      if (hsv_colour.v * 255 > max_v) {
        max_v = hsv_colour.v * 255;
        // std::cout << "max_v " << max_v << "\t" << hsv_colour.v << std::endl;
      } else if (hsv_colour.v * 255 < min_v) {
        min_v = hsv_colour.v * 255;
        // std::cout << "min_v " << min_v << "\t" << hsv_colour.v << std::endl;
      }
    }
  }

  std::cout << "h " << max_h << "\t" << min_h << std::endl;
  std::cout << "s " << max_s << "\t" << min_s << std::endl;
  std::cout << "v " << max_v << "\t" << min_v << std::endl;

  // printf("Saving file\n");
  // std::ofstream outfile("Segmented_Image.ppm", std::ios::binary);
  // outfile << "P6\n" << width << " " << height << " 255\n"; // dont know
  // outfile.write((char *)data, (height * width * 3));
  // printf("Segmented_Image Saved\n");
}
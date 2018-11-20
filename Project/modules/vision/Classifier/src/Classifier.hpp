#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "main.hpp"

#define WIDTH_MIN 50
#define WIDTH_MAX 150
#define HEIGHT_MIN 50
#define HEIGHT_MAX 150

extern void Classifier_init();
extern int Classifier_main();
extern void Classifier(uint8_t* data);
extern void find_distance(int u, int v);
extern void Output_Segmentation(uint8_t* seg_image_array,
                                int img_width,
                                int img_height,
                                int obj_width,
                                int obj_height,
                                int center[2]);

class Classifier {
public:
    static int colours[3][3][2];
    static int seed[1000][2];
    static int object[10][4];
};

#endif /* CLASSIFIER_H_ */

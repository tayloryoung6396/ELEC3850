#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "main.hpp"

#define WIDTH_MIN 100
#define WIDTH_MAX 300
#define HEIGHT_MIN 100
#define HEIGHT_MAX 300

extern void Classifier_init();
extern int Classifier_main();

int colours[3][3][2] = {
    {{64, 97}, {19, 48}, {19, 47}},  // Red maximum and minimum pixel parameters (RGB Image)
    {{1, 9}, {11, 49}, {8, 44}},     // Green maximum and minimum pixel parameters (RGB Image)
    {{14, 33}, {44, 61}, {79, 111}}  // Blue maximum and minimum pixel parameters (RGB Image)
};

int seed[1000][2];

int object[10][4];

#endif /* CLASSIFIER_H_ */
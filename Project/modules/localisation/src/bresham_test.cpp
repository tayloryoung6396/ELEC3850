// // NOTE, not used as this doesn't use the vertical distanc

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

#define SENSORS 1

double gridspace          = 0.1;
double w_Tank_Position[2] = {1, 1};
const int start_row       = 6;
const int start_column    = 6;

void dda(double object_cell_m, double object_cell_n, double tank_cell_m, double tank_cell_n) {
    double x;
    double y;
    double x1 = tank_cell_n;
    double y1 = tank_cell_m;
    double x2 = object_cell_n;
    double y2 = object_cell_m;
    double dx;
    double dy;
    double step;
    int i;

    dx = (x2 - x1);
    dy = (y2 - y1);
    if (std::abs(dx) >= std::abs(dy)) {
        step = std::abs(dx);
    }
    else {
        step = std::abs(dy);
    }
    dx = dx / step;
    dy = dy / step;
    x  = x1;
    y  = y1;
    i  = 1;
    while (i <= step) {
        std::cout << "Cell " << std::round(x) << "\t" << std::round(y) << std::endl;
        x = x + dy;
        y = y + dx;
        i = i + 1;
    }
    std::cout << "Cell " << std::round(x) << "\t" << std::round(y) << std::endl;
}

void nonbresh(double object_cell_m, double object_cell_n, double tank_cell_m, double tank_cell_n) {
    // this is not breshams algorithim, this is a crude mx+b approx that should
    // work ok accuracy of this method can be improved by decreasing the size of
    // cell
    int dx      = object_cell_n - tank_cell_n;  // dx=x2-x1
    int dy      = object_cell_m - tank_cell_m;  // dy=y2-y1
    double inc  = ((double) dy / (double) dx);
    double xval = 0;
    double yval = 0;  // this is used to increment because it will help account for
                      // differing slopes

    int multiplier = 0;

    std::cout << "Tank " << tank_cell_m << " " << tank_cell_n << std::endl;
    std::cout << "Goal " << object_cell_m << " " << object_cell_n << std::endl;
    std::cout << "delta " << dy << " " << dx << std::endl;

    if (dx == 0) {
        std::cout << "vertical cell" << std::endl;
        if (dy > 0) {
            for (double i = tank_cell_m; i <= object_cell_m; i++) {
                xval = tank_cell_n;
                yval = i;
                std::cout << "x val " << xval << " \ty val " << yval << std::endl;
                // Grid::cell_list.emplace_back(std::make_pair(xval, yval));
            }
        }
        else {
            for (double i = tank_cell_m; i >= object_cell_m; i--) {
                xval = tank_cell_n;
                yval = i;
                std::cout << "x val " << xval << " \ty val " << yval << std::endl;
                // Grid::cell_list.emplace_back(std::make_pair(xval, yval));
            }
        }
    }
    else if (dy == 0) {
        std::cout << "horizontal cell" << std::endl;
        if (dx > 0) {
            for (double i = tank_cell_n; i <= object_cell_n; i++) {
                yval = tank_cell_m;
                xval = i;
                std::cout << "x val " << xval << " \ty val " << yval << std::endl;
                // Grid::cell_list.emplace_back(std::make_pair(xval, yval));
            }
        }
        else {
            for (double i = tank_cell_n; i >= object_cell_n; i--) {
                yval = tank_cell_m;
                xval = i;
                std::cout << "x val " << xval << " \ty val " << yval << std::endl;
                // Grid::cell_list.emplace_back(std::make_pair(xval, yval));
            }
        }
    }
    // need if statement here to account for the situation where it is straight
    // ahead and dx/dy=infinity
    else {
        std::cout << "other cell" << std::endl;
        dda(object_cell_m, object_cell_n, tank_cell_m, tank_cell_n);
    }
}

int main() {

    int tank_cell_m = std::floor(w_Tank_Position[0] / gridspace) + start_row;
    int tank_cell_n = std::floor(w_Tank_Position[1] / gridspace) + start_column;

    double sen_hori[SENSORS]  = {1};
    double sen_vert[SENSORS]  = {-1};
    double sen_theta[SENSORS] = {0};

    for (int i = 0; i < SENSORS; i++) {
        int object_cell_m = tank_cell_m + std::floor(sen_hori[i] / gridspace);
        int object_cell_n = tank_cell_n + std::floor(sen_vert[i] / gridspace);
        std::cout << "Tank " << tank_cell_m << " " << tank_cell_n << std::endl;
        std::cout << "Goal " << object_cell_m << " " << object_cell_n << std::endl;
        nonbresh(object_cell_m, object_cell_n, tank_cell_m, tank_cell_n);
    }
}

#ifndef LOCALISATION_H_
#define LOCALISATION_H_

#include "main.hpp"

#define Grid_m 15

extern void Localisation_init();
extern int Localisation_main();
extern void probability(int cell_column, int cell_row, double cell_dist, double obj_dist);
extern void breshams_alg(int i, double sen_hori[], double sen_vert[]);

class Localisation {
public:
    static double w_Tank_Position[2];  // where i am in world, read in as {x,y} where x is forward and y si to the left.
    static double w_Tank_Rotation;     // angle in radians counterclockwise
    static double w_Goal_Position[2];
    // wrt front of tank and the world static double gens_sonics[SENSORS];
    // this variable will need to be names and connected up with gens sonics
};

class Grid {
public:
    static const int m;
    static const int n;
    static const int start_row;
    static const int start_column;
    static double area;
    static double gridspace;
    static double map[Grid_m][Grid_m];

    static std::vector<std::pair<int, int>> cell_list;
};


#endif /* LOCALISATION_H_ */

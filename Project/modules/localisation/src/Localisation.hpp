#ifndef LOCALISATION_H_
#define LOCALISATION_H_

#include "main.hpp"

extern void Localisation_init();
extern int Localisation_main();

class Localistation {
public:
    static double w_Tank_Position[2];
    static double w_Tank_Rotation;
    static double w_Goal_Position[2];
};

#endif /* LOCALISATION_H_ */
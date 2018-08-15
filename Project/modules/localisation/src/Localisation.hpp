#ifndef LOCALISATION_H_
#define LOCALISATION_H_

#include "main.hpp"

extern void Localisation_init();
extern int Localisation_main();

class Localistation {
public:
    static double wTank_theta;
    static double wTank_pos[2];
};

#endif /* LOCALISATION_H_ */
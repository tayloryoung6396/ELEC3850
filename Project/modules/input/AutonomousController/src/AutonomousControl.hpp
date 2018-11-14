#ifndef AUTONOMOUS_CONTROL_H_
#define AUTONOMOUS_CONTROL_H_

#include "main.hpp"

extern void AutonomousControl_init();
extern int AutonomousControl_main();

class AutoState {
public:
    static bool have_object;
    static bool known_object;
    static bool at_goal;
    static bool known_goal;
    static bool on_route;
    static bool object_classified;
};

#endif /* AUTONOMOUS_CONTROL_H_ */

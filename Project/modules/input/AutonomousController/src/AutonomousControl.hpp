#ifndef AUTONOMOUS_CONTROL_H_
#define AUTONOMOUS_CONTROL_H_

#include "main.hpp"

extern void AutonomousControl_init();
extern int AutonomousControl_main();

class AutoStates {
public:
    static bool AutoState::have_object;
    static bool AutoState::known_object;
    static bool AutoState::at_goal;
    static bool AutoState::known_goal;
    static bool AutoState::on_route;
};

#endif /* AUTONOMOUS_CONTROL_H_ */
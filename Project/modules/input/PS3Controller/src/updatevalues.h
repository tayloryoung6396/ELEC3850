#ifndef UPDATEVALUES_H_INCLUDED
#define UPDATEVALUES_H_INCLUDED

#pragma once

#include <climits>
#include <iostream>
#include "joystick.h"

void update_buttons(Joystick* joy);
void update_axes(Joystick* joy);

short bound_short(int num);
short map(long x, long in_min, long in_max, long out_min, long out_max);
short short_map(long x);

#endif  // UPDATEVALUES_H_INCLUDED

#ifndef JOYSTICK_H_INCLUDED
#define JOYSTICK_H_INCLUDED

#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "map_ps3_controller.h"

struct joystick_move_t
{
    uint32_t time;     /* movement event timestamp in milliseconds */
    int16_t value;     /* value */
    uint8_t type;      /* event type */
    uint8_t id;        /* axis/button number */
};

const size_t joystick_event_size =sizeof(joystick_move_t);  //size of move event object in bytes

class Joystick
{
public:
    Joystick(std::string filename="/dev/input/js0");
    ~Joystick();

    // reads in the next available joystick update. Blocks until it receives an update.
    void Update();

    // Checks if the most recent call to this->Update() was a Button or an Axis
    bool hasButtonUpdate();
    bool hasAxisUpdate();

    // Get the ID of the updated button or axis
    ButtonId getUpdatedButton();
    AxisId getUpdatedAxis();

    // get the state of a particular button or axis
    uint8_t getButtonState(ButtonIdentification button_id);
    int16_t getAxisState(AxisIdentification axis_id);

private:
    int file_descriptor;
    joystick_move_t event;

    ButtonIdentification updated_button;
    AxisIdentification updated_axis;

    bool is_axis_update;
    bool is_button_update;

    uint8_t button_values[max_button_count] = {0};
    int16_t axis_values[max_axis_count] = {0};
};

#endif // JOYSTICK_H_INCLUDED

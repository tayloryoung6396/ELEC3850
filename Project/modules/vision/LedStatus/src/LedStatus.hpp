#ifndef LED_STATUS_H_
#define LED_STATUS_H_

#include "main.hpp"

#define LED_RED 28
#define LED_BLUE 27
#define LED_GREEN 29

extern void Enable_RED();
extern void Disable_RED();
extern void Enable_BLUE();
extern void Disable_BLUE();
extern void Enable_GREEN();
extern void Disable_GREEN();

extern void LedStatus_init();
extern void Light_Sequence();

#endif /* LED_STATUS_H_ */

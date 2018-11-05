#ifndef LED_STATUS_H_
#define LED_STATUS_H_

#include "main.hpp"

// 17 27 22
#define LED_RED 17
#define LED_BLUE 22
#define LED_GREEN 27

extern void Enable_RED();
extern void Disable_RED();
extern void Enable_BLUE();
extern void Disable_BLUE();
extern void Enable_GREEN();
extern void Disable_GREEN();

extern void LedStatus_init();
extern void Light_Sequence();

#endif /* LED_STATUS_H_ */

#ifndef _ULTRA_H_
#define _ULTRA_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "Std_Types.h"



void ultrasonic_init(void);
void enable_ex_interrupt(void);
void UltraSonic_Trigger(void);
long ReturnDistance();
void CarMovingForward(); 
void TurnLeft(); 
void TurnRight(); 
void StopCar();
void CarMovingBackward(); 

#endif

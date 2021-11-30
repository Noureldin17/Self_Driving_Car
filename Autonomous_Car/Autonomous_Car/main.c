#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Bit_Math.h"
#include "Std_Types.h"
#include "DIO_interface.h"
#include "ultrasonic.h"
#include <util/delay.h>

long RightDistance =0;
long MidDistance =0;
long LeftDistance =0;

void LooKRight()
{			
	      OCR1A=97;   //0 degree
		  _delay_ms(1000);
}
void LookLeft() 
{
		  OCR1A=570;  //180 degree
		  _delay_ms(1000);
}

void LookForward()
{
	      OCR1A=350;  //90 degree
	      _delay_ms(1000);
}

int main(void)
{		
	TIMSK |= (1 << TOIE0);  // enable timer interrupt
DIO_voidSetPortDirection(PORT_A,OUTPUT);
DIO_voidSetPortDirection(PORT_B,OUTPUT);

	ultrasonic_init(); 
	sei(); 
	 
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)
	ICR1=4999;  //fPWM=50Hz (Period = 20ms Standard).
	DDRD|=(1<<PD4)|(1<<PD5);   //PWM Pins as Out*/

	 while(1)
	 {
		LookForward();
		UltraSonic_Trigger(); 
		MidDistance = ReturnDistance(); 
		if (MidDistance > 20)
		{
			CarMovingForward();
		} 
		else 
		{ 
			CarMovingBackward();
			_delay_ms(500);
			StopCar(); 
			LooKRight(); 
			_delay_ms(50);
			UltraSonic_Trigger();
			_delay_ms(100); 
			RightDistance = ReturnDistance(); 
			_delay_ms(100); 
			LookLeft();
			_delay_ms(50); 
			UltraSonic_Trigger();
			_delay_ms(100); 
			LeftDistance = ReturnDistance();
			LookForward();
			
			if (LeftDistance > 20 || RightDistance > 20 )
			{
				if (LeftDistance > RightDistance)
				{
					TurnLeft(); 
					_delay_ms(800);
					StopCar(); 
				} 
				else
				{
					TurnRight();
					_delay_ms(800);
					StopCar();
				}
			}
		}
	 }

}

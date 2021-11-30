#include "ultrasonic.h"
#include "DIO_interface.h"
#include "Std_Types.h"
//#include "LCD_interface.h"

//uint8_t distance_str[10];
volatile long avg = 0;
volatile unsigned char up = 0;
volatile u32 running = 0;
volatile u32 timercounter =0;
int turn = 0;


void ultrasonic_init(void){

  DIO_voidSetPinDirection(PORT_D,PIN0,OUTPUT);
  DIO_voidSetPinDirection(PORT_D,PIN3,INPUT);
  DIO_voidSetPinValue(PORT_D,PIN3,HIGH);
  enable_ex_interrupt();
}

void enable_ex_interrupt(void){

    MCUCR |= (0 << ISC11) | (1 << ISC10); // enable interrupt on any(rising/dropping) edge
    GICR |= (1 << INT1);  //Turns on INT1
}

void UltraSonic_Trigger(void){

  if(running == 0){
    DIO_voidSetPinValue(PORT_D,PIN0,HIGH);
    _delay_us(10);
	DIO_voidSetPinValue(PORT_D,PIN0,LOW);
    running=1;
  }
}

SIGNAL(INT1_vect){
	if(running){ //accept interrupts only when sonar was started
		if (up == 0) { // voltage rise, start time measurement
			up = 1;
			timercounter = 0;
			TCCR0 |= (0 << CS02)|(0 << CS01)|(1 << CS00); // Start/initialize timer with pre-scalar 1
			TCNT0 = 0; // Initialize Counter
			} else { // voltage drop, stop time measurement
			up = 0;
			//avg = (timercounter*255+TCNT0)/466;
			avg = TCNT0 + (255 * timercounter);
			avg = (u64)((34300 * (avg/2))/16000000);
			running = 0;
		}
	}

}

ISR(TIMER0_OVF_vect){
	 if (up) {
		 timercounter++;
	    }
}
long ReturnDistance()
{
	return avg; 
}

void CarMovingForward()
{
	DIO_voidSetPortDirection(PORT_B, OUTPUT); 
	DIO_voidSetPinValue(PORT_B, PIN0,HIGH); // ENA
	DIO_voidSetPinValue(PORT_B, PIN1,HIGH); // Input 1
	DIO_voidSetPinValue(PORT_B, PIN2,LOW); // Input 2
	DIO_voidSetPinValue(PORT_B, PIN3,HIGH); // Input 3
	DIO_voidSetPinValue(PORT_B, PIN4,LOW); // Input 4
	DIO_voidSetPinValue(PORT_B, PIN5,HIGH); // ENB 
	
	DIO_voidSetPortDirection(PORT_C, OUTPUT); 
	DIO_voidSetPinValue(PORT_C, PIN0,HIGH); // ENA2
	DIO_voidSetPinValue(PORT_C, PIN1,HIGH); // Input 1
	DIO_voidSetPinValue(PORT_C, PIN2,LOW); // Input 2
	DIO_voidSetPinValue(PORT_C, PIN3,HIGH); // Input 3
	DIO_voidSetPinValue(PORT_C, PIN4,LOW); // Input 4
	DIO_voidSetPinValue(PORT_C, PIN5,HIGH); // ENB2
		
}

void TurnLeft()
{
	// Disable Left Motors for the car to turn left
	
	DIO_voidSetPortDirection(PORT_B, OUTPUT); 
	DIO_voidSetPinValue(PORT_B, PIN0,LOW); // ENA
	DIO_voidSetPinValue(PORT_B, PIN5,LOW); // ENB
		
	DIO_voidSetPortDirection(PORT_C, OUTPUT); 
	DIO_voidSetPinValue(PORT_C, PIN0,HIGH); // ENA2
	DIO_voidSetPinValue(PORT_C, PIN1,HIGH); // Input 1
	DIO_voidSetPinValue(PORT_C, PIN2,LOW); // Input 2
	DIO_voidSetPinValue(PORT_C, PIN3,HIGH); // Input 3
	DIO_voidSetPinValue(PORT_C, PIN4,LOW); // Input 4
	DIO_voidSetPinValue(PORT_C, PIN5,HIGH); // ENB2
}

void TurnRight()
{
		DIO_voidSetPortDirection(PORT_B, OUTPUT); 
		DIO_voidSetPinValue(PORT_B, PIN0,HIGH); // ENA
		DIO_voidSetPinValue(PORT_B, PIN1,HIGH); // Input 1
		DIO_voidSetPinValue(PORT_B, PIN2,LOW); // Input 2
		DIO_voidSetPinValue(PORT_B, PIN3,HIGH); // Input 3
		DIO_voidSetPinValue(PORT_B, PIN4,LOW); // Input 4
		DIO_voidSetPinValue(PORT_B, PIN5,HIGH); // ENB
		
		// Disable Right Motors for the car to turn Right
		
		DIO_voidSetPortDirection(PORT_C, OUTPUT); 
		DIO_voidSetPinValue(PORT_C, PIN0,LOW); // ENA2
		DIO_voidSetPinValue(PORT_C, PIN5,LOW); // ENB2
}

void StopCar() 
{
		DIO_voidSetPortDirection(PORT_B, OUTPUT); 
		DIO_voidSetPinValue(PORT_B, PIN0,LOW); // ENA
		DIO_voidSetPinValue(PORT_B, PIN5,LOW); // ENB
	
		DIO_voidSetPortDirection(PORT_C, OUTPUT); 
		DIO_voidSetPinValue(PORT_C, PIN0,LOW); // ENA2
		DIO_voidSetPinValue(PORT_C, PIN5,LOW); // ENB2
}

void CarMovingBackward()
{
		DIO_voidSetPortDirection(PORT_B, OUTPUT);
		DIO_voidSetPinValue(PORT_B, PIN0,HIGH); // ENA
		DIO_voidSetPinValue(PORT_B, PIN1,LOW); // Input 1
		DIO_voidSetPinValue(PORT_B, PIN2,HIGH); // Input 2
		DIO_voidSetPinValue(PORT_B, PIN3,LOW); // Input 3
		DIO_voidSetPinValue(PORT_B, PIN4,HIGH); // Input 4
		DIO_voidSetPinValue(PORT_B, PIN5,HIGH); // ENB
		
		DIO_voidSetPortDirection(PORT_C, OUTPUT);
		DIO_voidSetPinValue(PORT_C, PIN0,HIGH); // ENA2
		DIO_voidSetPinValue(PORT_C, PIN1,LOW); // Input 1
		DIO_voidSetPinValue(PORT_C, PIN2,HIGH); // Input 2
		DIO_voidSetPinValue(PORT_C, PIN3,LOW); // Input 3
		DIO_voidSetPinValue(PORT_C, PIN4,HIGH); // Input 4
		DIO_voidSetPinValue(PORT_C, PIN5,HIGH); // ENB2
}
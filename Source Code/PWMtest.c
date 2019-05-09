

/* 
		Kevin Lopez, Cristian Lopez 
		Robot car with bluetooth module and IR
		 Project 2 
		
		UART7_Tx() 				to send data
		UART7_Rx() 				receive data
		UART7_Transmit_String() send a string
		
	Using IR protocol to use protocol 
 */

#include <stdint.h>
#include <math.h>
#include "PLL.h"
#include "PWM.h"
#include "tm4c123gh6pm.h"
#include "INIT.c"  //UART PORTc PWM TIMER initialization 



/*
		 USED PINS 
			
			PC 6  PC.7   		wheel
			PC 5  PC 4		  	wheel
 			PB.6  PB.7   	 	wheels speed 

			PB4 				IR signal 
			PE.0  PE.1  		UART (RX, TX) 	


*/



int main(void){
		uint8_t bt_data;
		PLL_Init();                  		// 	bus clock at 80 MHz
		PWM0A_Init(40000, 20000);    		// 	initialize PWM0, 1000 Hz, 37% duty	 		RIGHT WHEEL
		PWM0B_Init(40000, 20000);     	// 	initialize PWM0, 1000 Hz, 37% duty			LEFT WHEEL
		UART7_Init();										// 	 57600, 8 Data bits, 1 stop, odd parity
		Switch_Init();
		PWM1A_Init (); 									// 40Khz 50% duty cycle  									PB4
		Timer1A_Int(1000); 							//MicroSeconds
	
	

	
	
	uint8_t addr; 	//default address is device0 
	uint8_t ZERO[] = 	"Trasmited 0 \n\r";
	uint8_t ONE[] = 	"Trasmited 1 \n\r";
	uint8_t TWO[] = 	"Trasmited 2 \n\r";
	uint8_t THREE[] = "Trasmited 3  \n\r";
	uint8_t FOUR[] = 	"Trasmited 4  \n\r";
	uint8_t FIVE[] = 	"Trasmited 5  \n\r";
	uint8_t SIX[] = 	"Trasmited 6  \n\r";
	uint8_t SEVEN[] = "Trasmited 7  \n\r";
	uint8_t EIGHT[] = "Trasmited 8  \n\r";
	uint8_t NINE[] = 	"Trasmited 9  \n\r";
	uint8_t A_HEX[] = "Trasmited A  \n\r";
	uint8_t B_HEX[] = "Trasmited B  \n\r";
	uint16_t A_INIT_SPEED = 17000, B_INIT_SPEED = 20000;
	addr = 1;
	addr = 0;
		
		while(1){	

			bt_data = UART7_Rx();
			
			switch(bt_data){
				case 'w' 	:  GPIO_PORTC_DATA_R |= 0xC0;	GPIO_PORTC_DATA_R &= ~0x30;//FOWARDS
										PWM0_0_CMPA_R = A_INIT_SPEED; PWM0_0_CMPB_R = B_INIT_SPEED;
										UART7_Transmit_String(FOWARD);	break; 
				
				case 'a' 	:	GPIO_PORTC_DATA_R |= 0xC0;	GPIO_PORTC_DATA_R &= ~0x30; // LEFT TURN
										PWM0_0_CMPA_R =  14000;	PWM0_0_CMPB_R = 8000;	
										UART7_Transmit_String(LEFT);	break; 
				
				case 'd' 	: GPIO_PORTC_DATA_R |= 0xC0;	GPIO_PORTC_DATA_R &= ~0x30; //RIGHT TURN
										PWM0_0_CMPA_R = 7000; PWM0_0_CMPB_R = 15000;
										UART7_Transmit_String(RIGHT);	break; 
				
				case 's' 	: 	GPIO_PORTC_DATA_R &= ~0xC0;	 GPIO_PORTC_DATA_R |= 0x30; // BACKWARDS
										PWM0_0_CMPA_R =  A_INIT_SPEED;	PWM0_0_CMPB_R = B_INIT_SPEED; 
										UART7_Transmit_String(BACKWARDS);	break; 
				
				case 'q' 	:	PWM0_0_CMPA_R =  0; PWM0_0_CMPB_R = 0; // stop the car from movement
										GPIO_PORTC_DATA_R |= 0xC0;	GPIO_PORTC_DATA_R &= ~0x30;//FOWARDS
										UART7_Transmit_String(STOP);	break; 
				
				case	'k'	:	A_INIT_SPEED -= 4000; B_INIT_SPEED -= 4000; break; //DECREASE SPEED 
				
				case 	'i'	:	A_INIT_SPEED += 4000; B_INIT_SPEED += 4000; break; //INCREASE SPEED
				
				case 'c'	:	UART7_Transmit_String(ADD); addr = UART7_Rx(); break;   // change address
							
				case '0'	: startbit(); address(addr); logic(0); logic(0); logic(0); logic(0); 
										UART7_Transmit_String(ZERO); break;	//command0
									
				case '1'	: startbit(); address(addr); logic(0); logic(0); logic(0); logic(1); 
										UART7_Transmit_String(ONE); break;	//command1
									
				case '2'	: startbit(); address(addr); logic(0); logic(0); logic(1); logic(0); 
										UART7_Transmit_String(TWO); break;	//command2
									
				case '3'	: startbit(); address(addr); logic(0); logic(0); logic(1); logic(1); 
										UART7_Transmit_String(THREE); break;	//command3
									
				case '4'	: startbit(); address(addr); logic(0); logic(1); logic(0); logic(0); 
										UART7_Transmit_String(FOUR); break;	//command4
				
				case '5' 	:	startbit(); address(addr); logic(0); logic(1); logic(0); logic(1); 
										UART7_Transmit_String(FIVE); break;	//command5
										
				case '6' 	:	startbit(); address(addr); logic(0); logic(1); logic(1); logic(0); 
										UART7_Transmit_String(SIX); break;	//command6
										
				case '7' 	:	startbit(); address(addr); logic(0); logic(1); logic(1); logic(1); 
										UART7_Transmit_String(SEVEN); break;	//command7
										
				case '8' 	:	startbit(); address(addr); logic(1); logic(0); logic(0); logic(0); 
										UART7_Transmit_String(EIGHT); break;	//command8
										
				case '9' 	: startbit(); address(addr); logic(1); logic(0); logic(0); logic(1); 
										UART7_Transmit_String(NINE); break;		//command9
				
				case 'A' 	: startbit(); address(addr); logic(1); logic(0); logic(1); logic(0); 
										UART7_Transmit_String(A_HEX); break;	//commandA
										
				case 'B' 	: startbit(); address(addr); logic(1); logic(0); logic(1); logic(1); 
										UART7_Transmit_String(B_HEX); break;	//commandB

				default : break;
			}
				Timer1A_Int( 1000 ); // SO WE DONT SEND TO MUCH DATA BACK TO BACK TO IR



		}//	while
}// main



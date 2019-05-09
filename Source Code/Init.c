// Kevin Lopez, Cristian Lopez
// Feb, 27
// Lab2 piano

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define SW1       0x10                      // on the left side of the Launchpad board
#define SW2       0x01                      // on the right side of the Launchpad board
#define RED  	0x02
#define BLUE 	0X04
#define GREEN 	0x08 
#define OFF 	0x00
uint32_t waveFrecuency  = 3636;
uint32_t PortB;

////// ARRAY FOR FRECUENCY  PIANO  //////////////
/////////// 			  262   294     330  349     392   440   492
uint32_t tapr[] = {	6106, 5442,  4848, 4584,   4081, 3636,  3252 };
uint32_t Tapr = 3636;
uint8_t mode = 5; // sawtooth, triangle, sine, square and piano 
uint8_t note = 0; //
uint8_t upDown = 1; //1 = up  	 0 = down
uint8_t sinC = 0;
uint8_t sqUD = 0, on = 1;
int portCNum;


uint32_t sinF[] = {0x74,0x7B,0x83,0x8B,0x93,0x9B,0x9E,0xA5,0xAB,0xB3,0xBB,0xBE,0xC4,0xCB,0xD3,0xD4,0xDB,
0xDD,0xE3,0xE6,0xEB,0xED,0xF3,0xF3,0xF6,0xFB,0xFB,0xFC,0xFD,0xFE,0xFE,0xFF,0xFE,0xFE,0xFD,0xFC,0xFB,0xFB,
0xF6,0xF3,0xF3,0xED,0xEB,0xE6,0xE3,0xDD,0xDB,0xD4,0xD3,0xCB,0xC4,0xBE,0xBB,0xB3,0xAB,0xA5,0x9E,0x9B,0x93,
0x8B,0x83,0x7B,0x74,0x6D,0x66,0x63,0x5B,0x53,0x4B,0x43,0x3C,0x35,0x33,0x2B,0x23,0x1C,0x16,0x13,0xB,0x6,
0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x2,0x1,0x0,0x0,0x0,0x0,0x0,0x1,0x2,0x3,0x3,0x3,0x3,0x3,0x3,
0x3,0x3,0x3,0x3,0x3,0xB,0x13,0x16,0x1C,0x23,0x2B,0x33,0x35,0x3C,0x43,0x53,0x5B,0x63,0x66,0x6D
};
 



//////////////////////////////////////////////////////////
/////////////////////// UART2 ///////////////////////////	PED6,7  rx,tx
//////////////////////////////////////////////////////////
      // 9600, 8 Data bits, 1 stop, NO pari 
void UART2_Init(void){ //PD6 7  rx,tx
  SYSCTL_RCGCUART_R |=  0x04;
  SYSCTL_RCGCGPIO_R |=  0x08; // CLOCK FOR PORT D
  UART2_CTL_R = 0;
  UART2_IBRD_R = 104; // 16E6 / 16*9600
  UART2_FBRD_R = 10; //0.1666*0.64 + 0.05
  UART2_CC_R = 0;
  UART2_LCRH_R |= 0X60;  // 8 bits,  NO Paraty enable 
  UART2_CTL_R |= 0x300;  // ENABLE RX TX 
  UART2_CTL_R |= 1;      // ENABLE UART 

  GPIO_PORTD_DEN_R |= 0xC0;  
  GPIO_PORTD_AFSEL_R |= 0xC0; // ALTERNATE FUNTION 
  GPIO_PORTD_PCTL_R |= 0x11000000;

}

void UART2_Tx(uint8_t ByteToSend){
  while((UART2_FR_R & UART_FR_TXFF) != 0);
    UART2_DR_R = ByteToSend;
}

uint8_t UART2_Rx(void){
  while((UART2_FR_R & UART_FR_RXFE) != 0);
  return UART2_DR_R;
}

//////////////////////////////////////////////////////////
/////////////////////// PORTCnE  ///////////////////////////
//////////////////////////////////////////////////////////
////////////// BOTH edge trigger interrupt 2 PORTC.4-7  n PORTE.0-3
void PortCnE(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x00000014;  // 1) activate clock for Port E and C
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
	GPIO_PORTC_DEN_R |= 0xF0;          //Enable PC.4-7  input
	GPIO_PORTC_DIR_R |= 0x00;          //	PC.4-7  input

	GPIO_PORTE_DEN_R |= 0x0E;          //Enable PE0-3 input
	GPIO_PORTE_DIR_R |= 0x00;          //	PC.4-7  input

	
		// configure PORTC4-7 for BOTH edge trigger interrupt
	GPIO_PORTC_IS_R &= ~0xF0;				// make PC.4-7 edge sensitive
	GPIO_PORTC_IBE_R |= 0xF0;		// trigger on both edges
	GPIO_PORTC_ICR_R |= 0xF0;				// clear any prior interrupt
	GPIO_PORTC_IM_R |= 0xF0;				// unmask interrupt
	NVIC_PRI2_R = 3 << 5;					// set interrupt priority to 3
	NVIC_EN0_R |= 0x00000004;					// enable IRQ30 bit 30 of EN0

			// configure PE1-3 for BOTH edge trigger interrupt
	GPIO_PORTE_IS_R &= ~0x0E;				// make PE0-3 edge sensitive
	GPIO_PORTE_IBE_R |= 0x0E;			// trigger on both edges
	GPIO_PORTE_ICR_R |= 0x0E;				// clear any prior interrupt
	GPIO_PORTE_IM_R |= 0x0E;				// unmask interrupt
	
	NVIC_EN0_R |= 0x00000010;					// enable IRQ4 bit 4 of EN0----- 4rth bit

}


//////////////////////////////////////////////////////////
/////////////////////// PORTB  ///////////////////////////
//////////////////////////////////////////////////////////
//////// PORTB.0 - 7 OUTPUT 
void PortB_Init(){ //PORT B INITIALIZION ( INPUT FOR DAC )
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;		  //Enalbe port B clock
	delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTB_DEN_R = 0xFF;          // enable digital I/O on PB0-7
  GPIO_PORTB_DIR_R = 0xFF;          // PB0-7 out

}



//////////////////////////////////////////////////////////
/////////////////////// PORTF  ///////////////////////////
//////////////////////////////////////////////////////////
//////// INTERUPT FOR 1ST SWITCH
void PortF_Init(void){
 volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 		1, 2) PF4 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0


  	// configure PORTF4 for falling edge trigger interrupt
	GPIO_PORTF_IS_R &= ~0x10;				// make PF4 edge sensitive
	GPIO_PORTF_IBE_R &= ~0x10;			// trigger is controlled by IEV
	GPIO_PORTF_IEV_R &= ~0x10;			// falling edge trigger
	GPIO_PORTF_ICR_R |= 0x10;				// clear any prior interrupt
	GPIO_PORTF_IM_R |= 0x10;				// unmask interrupt
	
	// enable interrupt in NVIC and set priority to 3
	NVIC_PRI30_R = 3 << 5;					// set interrupt priority to 3
	NVIC_EN0_R |= 0x40000000;				// enable IRQ30 bit 30 of EN0
}

//////////////////////////////////////////////////////////
/////////////////////// TIMER1A  ///////////////////////////
//////////////////////////////////////////////////////////
// INTERUPT ENABLE
void Timer1A_Int(){

		// Timer1A configuration
	SYSCTL_RCGCTIMER_R |= 0x02;	// enable clock to timer Block 1
	TIMER1_CTL_R = 0;						// disable Timer1 during configuration
	TIMER1_CFG_R = 0x04;				// 16-bit timer
	TIMER1_TAMR_R = 0x02;				// periodic countdown mode
	
	TIMER1_TAPR_R = 10;					// 16MHZ / 10 = 160 KHZ
	TIMER1_TAILR_R = Tapr/256;			// 160 KHZ / Tapr 		
	
	TIMER1_ICR_R |= 0x1;					// clear Timer1A timeout flag
	TIMER1_IMR_R |= 0x01;				// enable Timer1A timeout interrupt 
	TIMER1_CTL_R |= 0x01;				// enable Timer1A
	NVIC_EN0_R |= 0x00200000;		// enable IRQ21

}



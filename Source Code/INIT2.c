#include "tm4c123gh6pm.h"
#include <stdint.h>

    uint8_t FOWARD[]   = "MOVING FOWARD\n\r";
    uint8_t LEFT[]       = "TURNING LEFT\n\r";
    uint8_t RIGHT[]    = "TURNING RIGHT\n\r";
    uint8_t BACKWARDS[]  = "MOVING BACKWARDS\n\r"; 
    uint8_t STOP[]     = "STOP\n\r";
    uint8_t CHANGEaDDRE[] = "CHANGE DEVICE ADDRESS\n\r";
    uint8_t COMMAND0[] = "COMMAND0";
    uint8_t COMMAND1[] = "COMMAND1";
    uint8_t COMMAND2[] = "COMMAND2";
    uint8_t COMMAND3[] = "COMMAND3";
    uint8_t COMMAND4[] = "COMMAND4";




//////////////////////////////////////////////////////////
/////////////////////// SYSTICK ///////////////////////////
//////////////////////////////////////////////////////////
void Systick(void){ // Systick interrupt for ADC
  NVIC_ST_CTRL_R = 0;               // Disables SysTick timer in order to change reload value
  NVIC_ST_RELOAD_R = 80000-1;   // reload reg with about .5 seconds 
  NVIC_ST_CTRL_R = 7;               // enable SysTick timer, and interrupt, system clock source
}


//////////////////////////////////////////////////////////
/////////////////////// PORTF ///////////////////////////
//////////////////////////////////////////////////////////
void Switch_Init(void){  unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x00000024; // (a) activate clock for port F,A
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R |= 0x0E;    // (c) ENABLE LIGHTS
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x1F;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000F000F; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x11;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
  
  GPIO_PORTC_DEN_R |= 0xC0;     //     enable digital I/O on PC4,5
  GPIO_PORTC_DIR_R |= 0xC0;     // digital output for PA3,4
  GPIO_PORTC_DEN_R |= 0x30;     //     enable digital I/O on PC6,7
  GPIO_PORTC_DIR_R |= 0x30;     // digital output for PA6,7
}


//////////////////////////////////////////////////////////
/////////////////////// UART7 ///////////////////////////
//////////////////////////////////////////////////////////
      // 57600, 8 Data bits, 1 stop, odd pari
void UART7_Init(void){ //PE0,1  rx,tx
  SYSCTL_RCGCUART_R |=  SYSCTL_RCGCUART_R7;
  SYSCTL_RCGCGPIO_R |=  0x10; // CLOCK FOR PORT E
  UART7_CTL_R = 0;
  UART7_IBRD_R = 86;   //17;
  UART7_FBRD_R = 51; //731
  UART7_CC_R = 0;
  UART7_LCRH_R |= 0X62;  // 8 bits,  Odd Paraty enable 
  UART7_CTL_R |= 0x300;  // ENABLE RX TX 
  UART7_CTL_R |= 1;      // ENABLE UART
  GPIO_PORTE_DEN_R |= 3;  
  GPIO_PORTE_AFSEL_R |= 3; // ALTERNATE FUNTION 
  GPIO_PORTE_PCTL_R |= 0x00000011;
}

void UART7_Tx(uint8_t ByteToSend){
  while((UART7_FR_R & UART_FR_TXFF) != 0);
    UART7_DR_R = ByteToSend;
}

uint8_t UART7_Rx(void){
  while((UART7_FR_R & UART_FR_RXFE) != 0);
  return UART7_DR_R;
}

void UART7_Transmit_String(const uint8_t *MessageString){
  while(*MessageString){ // while there is a character
    UART7_Tx(*MessageString); //send character
    MessageString++;
  }
}




/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// 	PWM ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void PWM1A_Init( ){ ////////// PB4        40Khz
//                                                  pmw2  Gen1A
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B

  while((SYSCTL_PRGPIO_R&0x02) == 0){}; //delay

  /////////////////////////port B INIT
  GPIO_PORTB_AFSEL_R |= 0x10;           // enable alt funct on PB4
  GPIO_PORTB_PCTL_R &= ~0x000F0000;     // configure PB4 as PWM0
  GPIO_PORTB_PCTL_R |= 0x00040000;
  GPIO_PORTB_AMSEL_R &= ~0x10;          // disable analog functionality on PB6
  GPIO_PORTB_DEN_R |= 0x10;             // enable digital I/O on PB6
	GPIO_PORTB_DIR_R |= 0x10;							// set  PB4 as OUTPUT
  //////////////////////// PWM CONFIG

  PWM0_1_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_1_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_1_LOAD_R = 2000;                 // 5) 80Mhz/40k = 2000    
  PWM0_1_CMPA_R = 0;                 // 6) count value when output rises
  PWM0_1_CTL_R |= 0x00000001;           // 7) start PWM0
	PWM0_ENABLE_R |= 0x00000004;          // enable PB6/M0PWM0

}

//////////////////////////////////////////////////////////
/////////////////////// TIMER ///////////////////////////
//////////////////////////////////////////////////////////


void Timer1A_Int(uint32_t delay){//micro seconds
  uint32_t i = 0;
  // Timer1A configuration
  SYSCTL_RCGCTIMER_R |= 0x02; // enable clock to timer Block 1
  TIMER1_CTL_R = 0;           // disable Timer1 during configuration
  TIMER1_CFG_R = 0x04;        // 16-bit timer
  TIMER1_TAMR_R = 0x02;       // periodic countdown mode
  //TIMER1_TAPR_R = 10;         // 16MHZ / 10 = 160 KHZ
  TIMER1_TAILR_R = 80 - 1;      //  80E6/80 = 1E6    period = 1MicroSecond     
  TIMER1_ICR_R |= 0x1;          // clear Timer1A timeout flag
  //TIMER1_IMR_R |= 0x01;       // enable Timer1A timeout interrupt 
  TIMER1_CTL_R |= 0x01;       // enable Timer1A
  NVIC_EN0_R |= 0x00200000;   // enable IRQ21


  while(i < delay){
    while((TIMER1_RIS_R & 0x01) == 0); // wait for timeout
    TIMER1_ICR_R = 0x01;    // clear timerA timeout flag  
    i++;
  }


}





//////////////////////////////////////////////////////////
///////////////////////  IR FUNTIONS /////////////////////
//////////////////////////////////////////////////////////
uint8_t ADD[] = "Enter address  bit\n\r" ;
uint8_t ran[] = "press another key\n\r";


void startbit(){
//  • Start pulse: 1ms high, 500us low  
  PWM0_1_CMPA_R = 1000;//set bit high 
  Timer1A_Int( 1000  );  /////////// coount for 1ms 
  PWM0_1_CMPA_R = 0;//set bit low
   Timer1A_Int(  500 );// count for .5ms 
}



 void logic(uint8_t bitToSend){            
/*  • Logical 1: 1.1ms high, 400us low 
    • Logical 0: 400us high, 400us low    */
              
    if (bitToSend == 1){
        PWM0_1_CMPA_R = 1000;
        Timer1A_Int(1100); //1100 microSeconds
        PWM0_1_CMPA_R = 0;//set bit low
        Timer1A_Int(  400 );// count for .4ms 
    }else if (bitToSend == 0){
        PWM0_1_CMPA_R = 1000;
        Timer1A_Int(400); //1100 microSeconds
        PWM0_1_CMPA_R = 0;//set bit low
        Timer1A_Int( 400 );// count for .4ms 
    }
 }




void address(uint8_t add){
	
      uint8_t msb_bin, lsb_bin; 
	
      switch(add){
        case '0' : msb_bin = 0; lsb_bin = 0; break;
        case '1' : msb_bin = 0; lsb_bin = 1; break;
        case '2' : msb_bin = 1; lsb_bin = 0; break;
        case '3' : msb_bin = 1; lsb_bin = 1; break;
      }

      logic( msb_bin );
      logic( lsb_bin );
}



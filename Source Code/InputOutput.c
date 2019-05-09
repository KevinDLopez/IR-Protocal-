// Kevin Lopez, Cristian Lopez
// Feb, 27
// Lab2 piano

#include "Init.c"
uint8_t data;
int main(void){ 
	PortB_Init();
  PortF_Init();   
  Timer1A_Int();           
	PortCnE();   
	UART2_Init(); // UART 2 	8-bits, NO parity, 1 STOP

	
	mode = 5;
	note = 10; // OFF STATE
  while(1){

	
data = UART2_Rx();
switch(data){ //UART GETS THE NOTES WE WANT TO PLAY
				case 'C': note = 0; mode = 5; Tapr =  6106*1.8; break;
				case 'D': note = 1; mode = 5; Tapr =  5442*1.8; break;
				case 'E': note = 2; mode = 5; Tapr =  4848*1.8; break; 
				case 'F': note = 3; mode = 5; Tapr =  4584*1.8; break; 
				case 'G': note = 4; mode = 5; Tapr =  4081*1.8;	 break; 
				case 'A': note = 5; mode = 5; Tapr =  3636*1.8; break; 
				case 'B': note = 6; mode = 5; Tapr =  3252*1.8; break; 
				default : note = 10; // ANYOTHER CHARACTER TURN OFF PIANO
			}
			TIMER1_TAILR_R = Tapr/256;

			

	}//while
}//main


	
	void GPIOPortC_Handler(void){ 
		volatile int readback;
		if (mode == 5){

				if (on ==1){
					on = 0;
					portCNum = GPIO_PORTC_MIS_R & 0xF0;
					switch(portCNum ){
						case 0x10: note = 0; break;
						case 0x20: note = 1;break;
						case 0x40: note = 2;break;
						case 0x80: note = 3;break;
						default: break;
					}//switch
				}else if(on ==0){
					on = 1;
					note = 10; // this will turn off PORT C
				}//else if
				
				
						switch(note) { //it will be set by interupts in port C 
							case 0: Tapr =  6106 *1.8;	break;//c
							case 1:	Tapr =  5442 *1.8;	break;//D
							case 2:	Tapr =  4848 *1.8;	break;//E
							case 3:	Tapr =  4584 *1.8;	break;//F
							case 4:	Tapr =  4081 *1.8;	break;//G
							case 5:	Tapr =  3636 *1.8;	break;//A
							case 6: Tapr =  3252 *1.8;	break;//B
							default: break;
							
						}//note switch 
						
					
						TIMER1_TAILR_R = Tapr/256;

		}//mode = 5



		GPIO_PORTC_ICR_R |= 0xF0;				// clear the interrupt flag
		readback = GPIO_PORTC_ICR_R;		// read to force interrupt flag clear
	}



	void GPIOPortE_Handler(void){ 
		volatile int readback;
		if (mode == 5){

			if (on ==1){
				on = 0;
				
					 portCNum = GPIO_PORTE_MIS_R & 0x0E;
					switch(portCNum ){
						case 0x02: note = 4; break;
						case 0x04: note = 5;break;
						case 0x08: note = 6;break;
						default: break;
				}//switch
			}else if(on ==0){
				on = 1;
				note = 10; // this will turn off PORT C

			}//else if
		
						switch(note) { //it will be set by interupts in port C 
							case 0: Tapr =  6106*1.8;;	break;//c
							case 1:	Tapr =  5442*1.8;;	break;//D
							case 2:	Tapr =  4848*1.8;;	break;//E
							case 3:	Tapr =  4584*1.8;;	break;//F
							case 4:	Tapr =  4081*1.8;;	break;//G
							case 5:	Tapr =  3636*1.8;;	break;//A
							case 6: Tapr =  3252*1.8;;	break;//B
							default: break;
							
						}//note switch 
						TIMER1_TAILR_R = Tapr/256;
					
		}//mode = 5



		GPIO_PORTE_ICR_R |= 0x0F;				// clear the interrupt flag
		readback = GPIO_PORTC_ICR_R;		// read to force interrupt flag clear
	}


	void GPIOPortF_Handler(void){
	volatile int readback;
	
	///////////////// CHANGE MODES ///////
	if(mode >= 5 ){
		mode = 1;
	}else {mode++;}

	/////////// CHANGE FRECUENCY MODE /////////////////
		switch(mode){ // frecuency 
			case 1: Tapr = 	waveFrecuency/1.15;		break;//sawtooth  good 
			case 2:	Tapr = 	waveFrecuency/1.15;		break; //triangle good 
			case 3:	Tapr = 	waveFrecuency*1.8;		break;//sine
			case 4:	Tapr = 	waveFrecuency/1.15;		break;// square wave  		
			case 5:	note = 10;  //piano mode

				break;
		default:  break; 
	}//switch
		
	TIMER1_TAILR_R = Tapr/256;

	GPIO_PORTF_ICR_R |= 0x10;				// clear the interrupt flag
	readback = GPIO_PORTF_ICR_R;		// read to force interrupt flag clear
}




void Timer1A_Handler(void){ 
	volatile uint32_t readback;
		if(TIMER1_MIS_R & 0x01){			// Timer1A timeout flag is set	




			
		
	/////////// CHANGE INCREASING MODE /////////////////
		switch(mode){ // frecuency 
			case 1:	PortB += 0x01; if( PortB >= 0xFF){ PortB = 0x00;}		break;//sawtooth
			case 2:	if(upDown == 1){ PortB += 0x2; if(PortB >= 0xFE) {upDown = 0;}	} //up
					else if (upDown == 0){ PortB -= 0x02; if ( PortB <= 0x01) { upDown = 1;} } 		break; //triangle
			case 3:	sinC += 1; if (sinC >= 127){ sinC = 0;} PortB = sinF[sinC];	break;//sine 0-127
			case 4:	if (sqUD <= 127){PortB = 0xFF; sqUD +=1;} else {PortB = 0x00; sqUD +=1; if(sqUD >= 255){ sqUD = 0;} }break;// square wave  		
			case 5:	//piano mode
				switch(note) { //it will be set by interupts in port C & E
					case 0: 	PortB = sinF[sinC];	sinC += 1; if (sinC >= 127){ sinC = 0;}	break;//c
					case 1:	 	PortB = sinF[sinC];	sinC += 1; if (sinC >= 127){ sinC = 0;} break;//D
					case 2:		PortB = sinF[sinC];	sinC += 1; if (sinC >= 127){ sinC = 0;} break;//E
					case 3:		PortB = sinF[sinC];	sinC += 1; if (sinC >= 127){ sinC = 0;} break;//F
					case 4:		PortB = sinF[sinC];	sinC += 1; if (sinC >= 127){ sinC = 0;} break;//G
					case 5:		PortB = sinF[sinC];	sinC += 1; if (sinC >= 127){ sinC = 0;} break;//A
					case 6: 	PortB = sinF[sinC];	sinC += 1; if (sinC >= 127){ sinC = 0;} break;//B
					default:  	PortB = 0; break;					
				}//note switch 
				break;
		default:  PortB = 0; break; 
	}//switch

	GPIO_PORTB_DATA_R =  PortB;

			
		TIMER1_ICR_R = 0x01;				// clear TimerA timeout flag
		readback = TIMER1_ICR_R;		// force interrupt flag clear
	}
	else{
		TIMER1_ICR_R = TIMER1_MIS_R;	// clear all flags
		readback = TIMER1_ICR_R;			// force interrupt flags clear
	}
}







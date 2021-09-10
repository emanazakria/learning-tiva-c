#include <stdint.h>
#include "C:\keil\EE319Kware\inc\tm4c123gh6pm.h"

#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register
#define RED       0x02
#define BLUE      0x04
#define GREEN     0x08

void SystemInit(){}
	

void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x00000020;                // 1) activate clock for Port F
  while ((SYSCTL_PRGPIO_R & 0x20)== 0) {};	      // check if clock is activated 
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;              // 2) unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F;                         // allow changes to PF4:PF0
  GPIO_PORTF_AMSEL_R = 0x00;                      // 3) disable analog on PF
	GPIO_PORTF_PCTL_R = 0x00000000;                 // 4) PCTL GPIO on PF4:PF0
	GPIO_PORTF_DIR_R = 0x0E;                        // 5) PF4,PF0 input, PF3:PF1 output
	GPIO_PORTF_AFSEL_R = 0x00;                      // 6) disable alt funct on PF7:PF0
  GPIO_PORTF_PUR_R = 0x11;                        // enable pull-up on PF0 and PF4
	GPIO_PORTF_DEN_R = 0x1F;                        // 7) enable digital I/O on PF4:PF0
	}
	
	
	
uint32_t PortF_Input(void){ 
  return (GPIO_PORTF_DATA_R&0x11);  // read PF4,PF0 inputs
}

void PortF_Output(uint32_t data){ // write PF3:PF1 outputs
  GPIO_PORTF_DATA_R = data;
}



int main(void){ uint32_t status;
  PortF_Init();              // initialize PF0 and PF4 and make them inputs(built-in switches)
	                           // make PF3:PF1 outputs (PF3:PF1 built-in LEDs)
  while(1){
    status = PortF_Input();
		
    switch(status){       
		                                         	// note: both switches are negative logic 
      case 0x01: PortF_Output(BLUE); break;   // SW1 pressed
      case 0x10: PortF_Output(RED); break;    // SW2 pressed
      case 0x00: PortF_Output(GREEN); break;  // both switches pressed
      case 0x11: PortF_Output(0); break;      // neither switch pressed
			default : PortF_Output(0); break; 
    }
  }
}
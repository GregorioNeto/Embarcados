
/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2017 20:05:55
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 *         Author:  Gregório Neto, g_neto01@hotmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include	"hw_types.h"
#include	"soc_AM335x.h"
#include	"defines.h"



//unsigned int flagBlink;
unsigned int flagMode;
//unsigned int value;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();	
static void kickTheDog();
void clkConfigure(unsigned int GPIO_MODULE,unsigned int GPIO_CLK,unsigned int GPIO_CLK_ENABLE);
void muxControl(unsigned int MODE, unsigned int REGISTER_OFFSET);
void gpioDirection(unsigned int MODULE,unsigned int REGISTER_OFFSET, unsigned int STATUS, unsigned int PIN);
static void ledOnOff(unsigned int LED, unsigned int STATUS,unsigned int MODULE);
static int verifyButton(unsigned int PIN,unsigned int MODULE);



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(void){  
	flagMode=0;	
	/* Configure the green LED control pin. */
  	kickTheDog();

  	clkConfigure(CM_PER_GPIO1,CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK,CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE);

  	muxControl(GPIO,CM_conf_gpmc_a5);
  	muxControl(GPIO,CM_conf_gpmc_a6);
  	muxControl(GPIO,CM_conf_gpmc_a7);
  	muxControl(GPIO,CM_conf_gpmc_a8);
  	muxControl(GPIO,CM_conf_gpmc_ben1);

  	gpioDirection(SOC_GPIO_1_REGS,CM_conf_gpmc_ben1,INPUT,PIN_BTN_01);
  	gpioDirection(SOC_GPIO_1_REGS,CM_conf_gpmc_a5,OUTPUT,PIN_LED_01);
  	gpioDirection(SOC_GPIO_1_REGS,CM_conf_gpmc_a6,OUTPUT,PIN_LED_02);
  	gpioDirection(SOC_GPIO_1_REGS,CM_conf_gpmc_a7,OUTPUT,PIN_LED_03);
  	gpioDirection(SOC_GPIO_1_REGS,CM_conf_gpmc_a8,OUTPUT,PIN_LED_04);

 	

  	while(1){

  		if(verifyButton(PIN_BTN_01,SOC_GPIO_1_REGS)){
  			flagMode++;
  		}
  		if(flagMode>2){
  			flagMode=0;
  		}
  		switch(flagMode){

  			case 0:
	  			ledOnOff(PIN_LED_01,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_01,LOW,SOC_GPIO_1_REGS);
				delay();
				ledOnOff(PIN_LED_02,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_02,LOW,SOC_GPIO_1_REGS);
				delay();
				ledOnOff(PIN_LED_03,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_03,LOW,SOC_GPIO_1_REGS);
				delay();
				ledOnOff(PIN_LED_04,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_04,LOW,SOC_GPIO_1_REGS);
	  			delay();
				break;
			case 1:
				ledOnOff(PIN_LED_04,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_04,LOW,SOC_GPIO_1_REGS);
				delay();
				ledOnOff(PIN_LED_03,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_03,LOW,SOC_GPIO_1_REGS);
				delay();
				ledOnOff(PIN_LED_02,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_02,LOW,SOC_GPIO_1_REGS);
				delay();
				ledOnOff(PIN_LED_01,HIGH,SOC_GPIO_1_REGS);
	  			delay();
	  			ledOnOff(PIN_LED_01,LOW,SOC_GPIO_1_REGS);
				delay();
				break;
			case 2:
				ledOnOff(PIN_LED_01,HIGH,SOC_GPIO_1_REGS);
				ledOnOff(PIN_LED_02,HIGH,SOC_GPIO_1_REGS);
				ledOnOff(PIN_LED_03,HIGH,SOC_GPIO_1_REGS);
				ledOnOff(PIN_LED_04,HIGH,SOC_GPIO_1_REGS);
				delay();
				ledOnOff(PIN_LED_01,LOW,SOC_GPIO_1_REGS);
				ledOnOff(PIN_LED_02,LOW,SOC_GPIO_1_REGS);
				ledOnOff(PIN_LED_03,LOW,SOC_GPIO_1_REGS);
				ledOnOff(PIN_LED_04,LOW,SOC_GPIO_1_REGS);
				delay();
  		}

  	}

	return(0);
} /* ----------  end of function main  ---------- */


/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  Delay
 *  Description:  
 * =====================================================================================
 */
static void delay(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}/* -----  end of function Delay  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  clkConfigure
 *  Description:  
 * =====================================================================================
 */
void clkConfigure(unsigned int GPIO_MODULE,unsigned int GPIO_CLK,unsigned int GPIO_CLK_ENABLE){
HWREG(SOC_CM_PER_REGS+GPIO_MODULE) |= (GPIO_CLK | GPIO_CLK_ENABLE);
}/*-----  end of function clkConfigure  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  muxControl
 *  Description:  
 * =====================================================================================
 */
void muxControl(unsigned int MODE, unsigned int REGISTER_OFFSET){
		HWREG(SOC_CONTROL_REGS+REGISTER_OFFSET) |= MODE;
}/*-----  end of function muxControl  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  gpioDirection
 *  Description:  
 * =====================================================================================
 */
 void gpioDirection(unsigned int MODULE,unsigned int REGISTER_OFFSET, unsigned int STATUS, unsigned int PIN){
 	if(STATUS){
 		
 		HWREG(MODULE+GPIO_OE) |= (1<<PIN);
 		HWREG(SOC_CONTROL_REGS+REGISTER_OFFSET)  &= ~(1<<3);		// ATIVAR PULL UP/PULL DOWN
 		HWREG(SOC_CONTROL_REGS+REGISTER_OFFSET)  &= ~(1<<4);		// SELECIONA PULL DOWN
 		HWREG(SOC_CONTROL_REGS+REGISTER_OFFSET)  |= (1<<5);			// ATIVAR RECEIVER

 	}else{

 		HWREG(MODULE+GPIO_OE) &= ~(1<<PIN);
 	}
 }/*-----  end of function gpioDirection  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledOnOff
 *  Description:  
 * =====================================================================================
 */
static void ledOnOff(unsigned int LED, unsigned int STATUS,unsigned int MODULE){
		if(STATUS){
			HWREG(MODULE+GPIO_SETDATAOUT) = (1<<LED);
		}else{
			HWREG(MODULE+GPIO_CLEARDATAOUT) = (1<<LED);
		}
}	/*-----  end of function ledOnOff  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  kickTheDog
 *  Description:  disable the watchdog
 * =====================================================================================
 */
static void kickTheDog(){
		
		HWREG(WDT1+WDT_WSPR) = 0xaaaa;
		delay();
		HWREG(WDT1+WDT_WSPR) = 0x5555;
		delay();
}/* -----  end of function kickTheDog   ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  verifyButton
 *  Description:  
 * =====================================================================================
 */
static int verifyButton(unsigned int PIN,unsigned int MODULE){
	unsigned int value;
	value = HWREG(MODULE+GPIO_DATAIN);
	value &= (1<<PIN);

	return value;
}/* -----  end of function verifyButton   ----- */
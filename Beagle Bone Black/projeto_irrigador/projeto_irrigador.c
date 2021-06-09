/*
 * =====================================================================================
 *
 *       Filename:  projeto_irrigador.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/06/2017 09:36:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gregório Neto, g_neto01@hotmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "uart_irda_cir.h"
#include "soc_AM335x.h"
#include "interrupt.h"
#include "board.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "consoleUtils.h"
#include "dmtimer.h"
#include "hw_watchdog.h"
#include "hw_types.h"
#include "hw_control_AM335x.h"
#include "hw_cm_per.h"
#include "hw_cm_wkup.h"
#include "hw_tsc_adc_ss.h"
#include "tsc_adc.h"



/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned int flagIsr;

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define			PIN_HIGH   	1
#define        	PIN_LOW		0

/* Values denoting the Interrupt Line number to be used. */
#define GPIO_INTC_LINE_1                  (0x0)
#define GPIO_INTC_LINE_2                  (0x1)

/*
** Values used to enable/disable interrupt generation due to level
** detection on an input GPIO pin.
*/
#define GPIO_INTC_TYPE_NO_LEVEL           (0x01)
#define GPIO_INTC_TYPE_LEVEL_LOW          (0x04)
#define GPIO_INTC_TYPE_LEVEL_HIGH         (0x08)
#define GPIO_INTC_TYPE_BOTH_LEVEL         (0x0C)

/*
** Values used to enable/disable interrupt generation due to edge
** detection on an input GPIO pin.
*/

#define GPIO_INTC_TYPE_NO_EDGE            (0x80)
#define GPIO_INTC_TYPE_RISE_EDGE          (0x10)
#define GPIO_INTC_TYPE_FALL_EDGE          (0x20)
#define GPIO_INTC_TYPE_BOTH_EDGE          (0x30)

/*Config kickTheDog*/
#define WDT1                                                    0x44E35000
/*End config kickTheDog*/


#define TIMER_INITIAL_COUNT            (0xFF000000u)
#define TIMER_RLD_COUNT                (0xFFFFFF83u) //(0xFF000000u)

#define T_1MS_COUNT                     (0x5DC0u) 
#define OVERFLOW                        (0xFFFFFFFFu)
#define TIMER_1MS_COUNT                 (0x5DC0u) 

/* Pin of I/O definition */
#define PIN_LED_BOARD_01                                              21
#define PIN_LED_BOARD_02                                              22
#define PIN_LED_BOARD_03                                              23
#define PIN_LED_BOARD_04                                              24
#define PIN_BTN_01                                                    28
/*END definition of pin*/ 

/* Module Gpio */
#define GPIO_1                                                        1
/*END Module Gpio*/



/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void			Delay(volatile unsigned int mSec);
static void 		initSerial(void);
static void 		initLed(unsigned int, unsigned int, unsigned int);
static void 		initButton(unsigned int, unsigned int, unsigned int);
static void			gpioAintcConf(void);
//static unsigned int getAddr(unsigned int);
static void 		gpioIsr(void);
static int			gpioPinIntConf(unsigned int, unsigned int, unsigned int);
static void 		gpioPinIntEnable(unsigned int, unsigned int, unsigned int); 
static void 		gpioIntTypeSet(unsigned int, unsigned int, unsigned int);
static void kickTheDog();
//funções para configurar o ADC
void TSCADCModuleClkConfig(void);
unsigned int TSCADCPinMuxSetUp(void);
void TSCADCConfigureAFEClock(unsigned int baseAdd, unsigned int moduleClk,
                          unsigned int inputClk);
void TSCADCTSTransistorConfig(unsigned int baseAdd, unsigned int enableTSTransistor);
void TSCADCStepIDTagConfig(unsigned int baseAdd, unsigned int enableStepIDTag);
void TSCADCStepConfigProtectionDisable(unsigned int baseAdd);
void TSCADCTSModeConfig(unsigned int baseAdd, unsigned int tsMode);
void TSCADCConfigureStepEnable(unsigned int baseAdd, unsigned int stepSel,
                        unsigned int stepEn_Dis);
void TSCADCModuleStateSet(unsigned int baseAdd, unsigned int enableModule);
void TSCADCTSStepOperationModeControl(unsigned int baseAdd, unsigned int mode,
                                   unsigned int stepSelect);
void TSCADCTSStepConfig(unsigned int baseAdd, unsigned int stepSelect,
                unsigned int adcNegativeRef, unsigned int adcPositiveInp,
                        unsigned int adcNegativeInp, unsigned int adcPositiveRef);

void TSCADCTSStepFIFOSelConfig(unsigned int baseAdd, unsigned int stepSel,
                        unsigned int FIFOSel);
void TSCADCTSStepModeConfig(unsigned int baseAdd, unsigned int stepSel,
                     unsigned int mode);

unsigned int TSCADCFIFOADCDataRead(unsigned int baseAdd, unsigned int FIFOSel);
void ADCConfigure(void);
void StepConfigure(unsigned int stepSel, unsigned int fifo,
                   unsigned int positiveInpChannel);
//-----END FUNÇÃO PARA O ADC-----


void seq1();
void seq2();
void seq3();
static int			vel = 10;
static int			seq = 1;
int value;

 
/******************************************************************************
**              FUNCTION DEFINITIONS
******************************************************************************/
int main(){

	kickTheDog();

    flagIsr = 0;

    initSerial();

    //ADCConfigure();

    while(1){
        ADCConfigure();
    value = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
    //UART_printf("Valor do sensor: %d\n", value);
    ConsoleUtilsPrintf("Valor do sensor: %d\n", value);
    Delay(100);
	}

    	
	/* Enable IRQ in CPSR */
   	IntMasterIRQEnable();

   	GPIOModuleClkConfig(GPIO_1); //GPIOModuleClkConfig(1);(MODIFICADO)
   	

   	//---->TIRADO GPIOModuleClkConfig(2);
	
	initLed(SOC_GPIO_1_REGS, GPIO_1, PIN_LED_BOARD_01); //initLed(SOC_GPIO_2_REGS, 2, 3);(MODIFICADO)
	initLed(SOC_GPIO_1_REGS, GPIO_1, PIN_LED_BOARD_02); 
	initLed(SOC_GPIO_1_REGS, GPIO_1, PIN_LED_BOARD_03); 
    initLed(SOC_GPIO_1_REGS, GPIO_1, PIN_LED_BOARD_04); 
    initButton(SOC_GPIO_1_REGS,GPIO_1,PIN_BTN_01); //initButton(SOC_GPIO_1_REGS, 1, 28);(MODIFICADO)
	

	// ENABLE PIN TO INTERRUPT   	
	gpioAintcConf();
	gpioPinIntConf(SOC_GPIO_1_REGS, GPIO_INTC_LINE_1, PIN_BTN_01); //gpioPinIntConf(SOC_GPIO_1_REGS, GPIO_INTC_LINE_1, 16);(MODIFICADO)
   	gpioIntTypeSet(SOC_GPIO_1_REGS, PIN_BTN_01, GPIO_INTC_TYPE_RISE_EDGE); //gpioIntTypeSet(SOC_GPIO_1_REGS, 16, GPIO_INTC_TYPE_RISE_EDGE);(MODIFICADO) //PARTE DE CONFIGURAR O TIPO DE BORDA

    
    ConsoleUtilsPrintf("\n Esta na Sequencia ---> 1 \n");
   	while(1){
		if(flagIsr){
        	flagIsr = 0;			
			}       	
			switch(seq){
				case 1:	
					seq1();
				break;
				case 2:
					seq2();
				break;
				case 3:
					seq3();
				break;
			}
    	}
	return(0);
}




void seq1(){
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_01,PIN_HIGH); //GPIOPinWrite(SOC_GPIO_2_REGS,3,1);(MODIFICADO)	
	Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_01,PIN_LOW);      
    Delay(vel);
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_02,PIN_HIGH); 
	Delay(vel);
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_02,PIN_LOW); 
	Delay(vel);
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_03,PIN_HIGH); 		
	Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_03,PIN_LOW); 
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_04,PIN_HIGH); 
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_04,PIN_LOW);        
    Delay(vel);
}

void seq2(){
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_04,PIN_HIGH);    
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_04,PIN_LOW);      
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_03,PIN_HIGH); 
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_03,PIN_LOW); 
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_02,PIN_HIGH);        
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_02,PIN_LOW); 
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_01,PIN_HIGH); 
    Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_01,PIN_LOW);        
    Delay(vel);
}

void seq3(){
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_01,PIN_HIGH);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_02,PIN_HIGH);		
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_03,PIN_HIGH);
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_04,PIN_HIGH);
	Delay(vel);
    GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_01,PIN_LOW);
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_02,PIN_LOW);		
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_03,PIN_LOW);
	GPIOPinWrite(SOC_GPIO_1_REGS,PIN_LED_BOARD_04,PIN_LOW);
	Delay(vel);
}

static void Delay(volatile unsigned int mSec){
   while(mSec != 0){
        DMTimerCounterSet(SOC_DMTIMER_2_REGS, 0); // configura  o contador.
        DMTimerEnable(SOC_DMTIMER_2_REGS); // incia a contagem(habilita o contador)
        while(DMTimerCounterGet(SOC_DMTIMER_2_REGS) < TIMER_1MS_COUNT);//enquanto for menor que um mSec, ele fica parado.
        DMTimerDisable(SOC_DMTIMER_2_REGS); //desabilita o timer(contador)
        mSec--; //decrementa a variavel
    }
}


/*FUNCTION*-------------------------------------------------------
*
* A function which is used to initialize UART.
*END*-----------------------------------------------------------*/
static void initSerial(){
	/* Initialize console for communication with the Host Machine */
    	ConsoleUtilsInit();
    	/* Select the console type based on compile time check */
    	ConsoleUtilsSetType(CONSOLE_UART);
}

/*FUNCTION*-------------------------------------------------------
*
* A function which is used to initialize GPIO like LED.
*END*-----------------------------------------------------------*/
static void initLed(unsigned int baseAdd, unsigned int module, unsigned int pin){
    
    	/* Selecting GPIO pin for use. */
    	GPIOPinMuxSetup(module, pin);
    
    	/* Setting the GPIO pin as an output pin. */
    	GPIODirModeSet(baseAdd, pin, GPIO_DIR_OUTPUT);
}

/*FUNCTION*-------------------------------------------------------
*
* A function which is used to initialize GPIO like BUTTON.
*END*-----------------------------------------------------------*/
static void initButton(unsigned int baseAdd, unsigned int module, unsigned int pin){
    
    	/* Selecting GPIO pin for use. */
    	GPIOPinMuxSetup(module, pin);
    
    	/* Setting the GPIO pin as an output pin. */
    	GPIODirModeSet(baseAdd, pin, GPIO_DIR_INPUT);
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : gpioAintcconfigure
* Comments      : Do the necessary gpio configurations on to AINTC.
*END*-----------------------------------------------------------*/
static void gpioAintcConf(void){

    /* Initialize the ARM interrupt control */
    IntAINTCInit();
 
    /* Registering gpioIsr */
    IntRegister(SYS_INT_GPIOINT1A, gpioIsr);
    
    /* Set the priority */
    IntPrioritySet(SYS_INT_GPIOINT1A, 0, AINTC_HOSTINT_ROUTE_IRQ);
    
    /* Enable the system interrupt */
    IntSystemEnable(SYS_INT_GPIOINT1A);
   
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : gpioIsr
* Comments      : DMTimer interrupt service routine. This will 
* send a character to serial.
*END*-----------------------------------------------------------*/    
static void gpioIsr(void) {
	flagIsr = 1;	
    	/*	Clear wake interrupt	*/
	//HWREG(SOC_GPIO_1_REGS + 0x3C) = 0x1000;
	//HWREG(SOC_GPIO_1_REGS + 0x34) = 0x1000;
	//HWREG(SOC_GPIO_1_REGS + 0x2C) = 0x10000;

    ConsoleUtilsPrintf("\n ##### APERTOU O BOTAO #####");
    
    HWREG(SOC_GPIO_1_REGS + 0x2C)= 1<<PIN_BTN_01;
	//seq == 3 ? seq = 1 : seq++;//(MODIFICADO)
     if(seq == 3){
         seq = 1;
     }else{
         seq++;
     }
     /* SHOW ACTUALLY SEQUENCE*/
     ConsoleUtilsPrintf("\n Esta na Sequencia ---> %d \n",seq);
     /*END*/

//	vel += 100;

}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : gpioPinIntConfig
* Comments      :
*END*-----------------------------------------------------------*/
static int gpioPinIntConf(unsigned int baseAdd, unsigned int intLine,
                  unsigned int pinNumber){

    	/* Setting interrupt GPIO pin. */
    	gpioPinIntEnable(baseAdd,
               intLine,
               pinNumber);
    
    	return(0);
}

/*END*-----------------------------------------------------------*/
static void gpioPinIntEnable(unsigned int baseAdd,
                      unsigned int intLine,
                      unsigned int pinNumber){
    if(GPIO_INTC_LINE_1 == intLine){
        HWREG(baseAdd + GPIO_IRQSTATUS_SET(0)) = (1 << pinNumber);
    }else{
        HWREG(baseAdd + GPIO_IRQSTATUS_SET(1)) = (1 << pinNumber);
    }
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  kickTheDog
 *  Description:  disable the watchdog
 * =====================================================================================
 */
static void kickTheDog(){
        HWREG(WDT1+WDT_WSPR) = 0xaaaa;
        Delay(vel);
        HWREG(WDT1+WDT_WSPR) = 0x5555;
        Delay(vel);
}/* -----  end of function kickTheDog   ----- */


static void gpioIntTypeSet(unsigned int baseAdd,
                    unsigned int pinNumber,
                    unsigned int eventType){
    eventType &= 0xFF;

    switch(eventType)
    {

        case GPIO_INT_TYPE_NO_LEVEL:

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(0)) &= ~(1 << pinNumber);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(1)) &= ~(1 << pinNumber);

        break;

        case GPIO_INT_TYPE_LEVEL_LOW:

            /* Enabling logic LOW level detect interrupt geenration. */
            HWREG(baseAdd + GPIO_LEVELDETECT(0)) |= (1 << pinNumber);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(1)) &= ~(1 << pinNumber);

            /* Disabling rising edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_RISINGDETECT) &= ~(1 << pinNumber);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_FALLINGDETECT) &= ~(1 << pinNumber);

        break;

        case GPIO_INT_TYPE_LEVEL_HIGH:

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(0)) &= ~(1 << pinNumber);

            /* Enabling logic HIGH level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(1)) |= (1 << pinNumber);

            /* Disabling rising edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_RISINGDETECT) &= ~(1 << pinNumber);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_FALLINGDETECT) &= ~(1 << pinNumber);
        
        break;

        case GPIO_INT_TYPE_BOTH_LEVEL:
            
            /* Enabling logic LOW level detect interrupt geenration. */
            HWREG(baseAdd + GPIO_LEVELDETECT(0)) |= (1 << pinNumber);

            /* Enabling logic HIGH level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(1)) |= (1 << pinNumber);

            /* Disabling rising edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_RISINGDETECT) &= ~(1 << pinNumber);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_FALLINGDETECT) &= ~(1 << pinNumber);
            
        break;

        case GPIO_INT_TYPE_NO_EDGE:
            
            /* Disabling rising edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_RISINGDETECT) &= ~(1 << pinNumber);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_FALLINGDETECT) &= ~(1 << pinNumber);

        break;

        case GPIO_INT_TYPE_RISE_EDGE:

            /* Enabling rising edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_RISINGDETECT) |= (1 << pinNumber);

            /* Disabling falling edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_FALLINGDETECT) &= ~(1 << pinNumber);

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(0)) &= ~(1 << pinNumber);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(1)) &= ~(1 << pinNumber);

        break;

        case GPIO_INT_TYPE_FALL_EDGE:

            /* Disabling rising edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_RISINGDETECT) &= ~(1 << pinNumber);

            /* Enabling falling edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_FALLINGDETECT) |= (1 << pinNumber);

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(0)) &= ~(1 << pinNumber);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(1)) &= ~(1 << pinNumber);

        break;

        case GPIO_INT_TYPE_BOTH_EDGE:

            /* Enabling rising edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_RISINGDETECT) |= (1 << pinNumber);

            /* Enabling falling edge detect interrupt generation. */
            HWREG(baseAdd + GPIO_FALLINGDETECT) |= (1 << pinNumber);

            /* Disabling logic LOW level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(0)) &= ~(1 << pinNumber);

            /* Disabling logic HIGH level detect interrupt generation. */
            HWREG(baseAdd + GPIO_LEVELDETECT(1)) &= ~(1 << pinNumber);

        break;

        default:
        break;
    }
}


// FUNCOES PARA CONFIGURAR O ADC

void TSCADCModuleClkConfig(void)
{
    /* Configuring L3 Interface Clocks. */

    /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
          CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

    /* Checking fields for necessary values.  */

    /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
    while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) & CM_PER_L3_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
           CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
    ** attain the desired value.
    */
    while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
    ** register to attain the desired value.
    */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
          CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));


    /* Configuring registers related to Wake-Up region. */

    /* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
          CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
          CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
          CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));

    /* Writing to MODULEMODE field of CM_WKUP_TSC_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) |=
          CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE));

    /* Verifying if the other bits are set to required settings. */

    /*
    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
    ** to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
           CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK));

    /*
    ** Waiting for CLKACTIVITY_ADC_FCLK field in CM_WKUP_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_ADC_TSC_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST));
}


unsigned int TSCADCPinMuxSetUp(void)
{

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN0) = CONTROL_CONF_AIN0_CONF_AIN0_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN1) = CONTROL_CONF_AIN1_CONF_AIN1_RXACTIVE;

    HWREG( SOC_CONTROL_REGS +  CONTROL_CONF_AIN2)= CONTROL_CONF_AIN2_CONF_AIN2_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN3) = CONTROL_CONF_AIN3_CONF_AIN3_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN4) = CONTROL_CONF_AIN4_CONF_AIN4_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN5) = CONTROL_CONF_AIN5_CONF_AIN5_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN6) = CONTROL_CONF_AIN6_CONF_AIN6_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN7) = CONTROL_CONF_AIN7_CONF_AIN7_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_VREFP)= CONTROL_CONF_VREFP_CONF_VREFP_RXACTIVE;

    HWREG( SOC_CONTROL_REGS +  CONTROL_CONF_VREFN)= CONTROL_CONF_VREFN_CONF_VREFN_RXACTIVE;
    return TRUE;
}

void TSCADCConfigureAFEClock(unsigned int baseAdd, unsigned int moduleClk,
                          unsigned int inputClk)
{
    unsigned int clkDiv;

    clkDiv = moduleClk / inputClk;

    HWREG(baseAdd + TSC_ADC_SS_ADC_CLKDIV) &=
                        ~TSC_ADC_SS_ADC_CLKDIV_ADC_CLK_DIV;

    HWREG(baseAdd + TSC_ADC_SS_ADC_CLKDIV) = (clkDiv - 1);
}

void TSCADCTSTransistorConfig(unsigned int baseAdd, unsigned int enableTSTransistor)
{
    HWREG(baseAdd + TSC_ADC_SS_CTRL) &= ~TSC_ADC_SS_CTRL_TOUCH_SCREEN_ENABLE;

    HWREG(baseAdd + TSC_ADC_SS_CTRL) |= enableTSTransistor <<
                                   TSC_ADC_SS_CTRL_TOUCH_SCREEN_ENABLE_SHIFT;
}

void TSCADCStepIDTagConfig(unsigned int baseAdd, unsigned int enableStepIDTag)
{
    HWREG(baseAdd + TSC_ADC_SS_CTRL) &= ~TSC_ADC_SS_CTRL_STERP_ID_TAG;
    HWREG(baseAdd + TSC_ADC_SS_CTRL) |= enableStepIDTag <<
                                     TSC_ADC_SS_CTRL_STERP_ID_TAG_SHIFT;
}

void TSCADCStepConfigProtectionDisable(unsigned int baseAdd)
{
    HWREG(baseAdd + TSC_ADC_SS_CTRL) |=
                      TSC_ADC_SS_CTRL_STEPCONFIG_WRITEPROTECT_N;
}

void TSCADCTSModeConfig(unsigned int baseAdd, unsigned int tsMode)
{
    HWREG(baseAdd + TSC_ADC_SS_CTRL) &= ~TSC_ADC_SS_CTRL_AFE_PEN_CTRL;

    HWREG(baseAdd + TSC_ADC_SS_CTRL) |= tsMode <<
                                   TSC_ADC_SS_CTRL_AFE_PEN_CTRL_SHIFT;
}

void TSCADCConfigureStepEnable(unsigned int baseAdd, unsigned int stepSel,
                        unsigned int stepEn_Dis)
{
    HWREG(baseAdd + TSC_ADC_SS_STEPENABLE) &= ~(1 << stepSel);
    HWREG(baseAdd + TSC_ADC_SS_STEPENABLE) |= stepEn_Dis << stepSel;
}

void TSCADCModuleStateSet(unsigned int baseAdd, unsigned int enableModule)
{
    HWREG(baseAdd + TSC_ADC_SS_CTRL) &= ~TSC_ADC_SS_CTRL_ENABLE;
    HWREG(baseAdd + TSC_ADC_SS_CTRL) |= enableModule;
}

void TSCADCTSStepOperationModeControl(unsigned int baseAdd, unsigned int mode,
                                   unsigned int stepSelect)
{
    if(mode)
    {
         HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) |=
                                      TSC_ADC_SS_STEPCONFIG_DIFF_CNTRL;
    }
    else
    {
         HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) &=
                                    ~TSC_ADC_SS_STEPCONFIG_DIFF_CNTRL;
    }
}

void TSCADCTSStepConfig(unsigned int baseAdd, unsigned int stepSelect,
                unsigned int adcNegativeRef, unsigned int adcPositiveInp,
                        unsigned int adcNegativeInp, unsigned int adcPositiveRef)
{

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) &=
                                   ~TSC_ADC_SS_STEPCONFIG_SEL_RFM_SWC;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) |=
                   adcNegativeRef << TSC_ADC_SS_STEPCONFIG_SEL_RFM_SWC_SHIFT;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) &=
                               ~TSC_ADC_SS_STEPCONFIG_SEL_INP_SWC;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) |=
           adcPositiveInp << TSC_ADC_SS_STEPCONFIG_SEL_INP_SWC_SHIFT;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) &=
                                   ~TSC_ADC_SS_STEPCONFIG_SEL_INM_SWM;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) |=
                         adcNegativeInp << TSC_ADC_SS_STEPCONFIG_SEL_INM_SWM_SHIFT;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) &=
                               ~TSC_ADC_SS_STEPCONFIG_SEL_RFP_SWC;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSelect)) |=
                          adcPositiveRef << TSC_ADC_SS_STEPCONFIG_SEL_RFP_SWC_SHIFT;
}

void TSCADCTSStepFIFOSelConfig(unsigned int baseAdd, unsigned int stepSel,
                        unsigned int FIFOSel)
{
    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSel)) &=
                          ~TSC_ADC_SS_STEPCONFIG_FIFO_SELECT;

    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSel)) |=
                           FIFOSel << TSC_ADC_SS_STEPCONFIG_FIFO_SELECT_SHIFT;
}

void TSCADCTSStepModeConfig(unsigned int baseAdd, unsigned int stepSel,
                     unsigned int mode)
{
    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSel))
           &= ~TSC_ADC_SS_STEPCONFIG_MODE;
    HWREG(baseAdd + TSC_ADC_SS_STEPCONFIG(stepSel))
           |= mode << TSC_ADC_SS_STEPCONFIG_MODE_SHIFT;
}

unsigned int TSCADCFIFOADCDataRead(unsigned int baseAdd, unsigned int FIFOSel)
{
    return (HWREG(baseAdd + TSC_ADC_SS_FIFODATA(FIFOSel)) &
                        TSC_ADC_SS_FIFODATA_ADC_DATA);
}

void ADCConfigure(void) {
    /* Enable the clock for touch screen */
        TSCADCModuleClkConfig();

        TSCADCPinMuxSetUp();

        /* Configures ADC to 3Mhz */
        TSCADCConfigureAFEClock(SOC_ADC_TSC_0_REGS, 24000000, 3000000);

        /* Enable Transistor bias */
        TSCADCTSTransistorConfig(SOC_ADC_TSC_0_REGS, TSCADC_TRANSISTOR_ENABLE);

        TSCADCStepIDTagConfig(SOC_ADC_TSC_0_REGS, 1);

        /* Disable Write Protection of Step Configuration regs*/
        TSCADCStepConfigProtectionDisable(SOC_ADC_TSC_0_REGS);

        /* Configure step 1 for channel 1(AN0)*/
        StepConfigure(0, TSCADC_FIFO_0, TSCADC_POSITIVE_INP_CHANNEL1);

        /* General purpose inputs */
        TSCADCTSModeConfig(SOC_ADC_TSC_0_REGS, TSCADC_GENERAL_PURPOSE_MODE);

        /* Enable step 1 */
        TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 1, 1);

        /* Enable the TSC_ADC_SS module*/
        TSCADCModuleStateSet(SOC_ADC_TSC_0_REGS, TSCADC_MODULE_ENABLE);
}

void StepConfigure(unsigned int stepSel, unsigned int fifo,
                   unsigned int positiveInpChannel){
        /* Configure ADC to Single ended operation mode */
        TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                  TSCADC_SINGLE_ENDED_OPER_MODE , stepSel);

        /* Configure step to select Channel, refernce voltages */
        TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSel, TSCADC_NEGATIVE_REF_VSSA,
                    positiveInpChannel, TSCADC_NEGATIVE_INP_CHANNEL1, TSCADC_POSITIVE_REF_VDDA);

    /* XPPSW Pin is on, Which pull up the AN0 line*/
        //TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON, TSCADC_XNPSW_PIN_OFF,
        //                        TSCADC_YPPSW_PIN_OFF, stepSel);

        /* XNNSW Pin is on, Which pull down the AN1 line*/
        //TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_ON, TSCADC_YPNSW_PIN_OFF,
        //                        TSCADC_YNNSW_PIN_OFF,  TSCADC_WPNSW_PIN_OFF, stepSel);

        /* select fifo 0 or 1*/
        TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSel, fifo);

        /* Configure ADC to one short mode */
        TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSel, TSCADC_CONTINIOUS_SOFTWARE_ENABLED);
}

//-----END FUNCOES PARA CONFIGURAR ADC


/******************************* End of file *********************************/

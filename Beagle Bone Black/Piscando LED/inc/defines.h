/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/

#ifndef _DEFINES_
#define _DEFINES_


#define TIME													1000000
#define TOGGLE          										(0x01u)

#define WDT1													0x44E35000
#define WDT_WSPR												0x48

#define CM_WKUP_GPIO											0x8		
#define CM_WKUP_GPIO_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_WKUP_GPIO_CLKCTRL_OPTFCLKEN_GPIO_0_GDBCLK   			(1<<18)

#define CM_PER_GPIO1											0xAC		
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   			(1<<18)

#define CM_PER_GPIO2											0xB0		
#define CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK   			(1<<18)

#define CM_PER_GPIO3											0xB4		
#define CM_PER_GPIO3_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO3_CLKCTRL_OPTFCLKEN_GPIO_3_GDBCLK   			(1<<18)



#define CM_conf_gpmc_ben1      	 								0x0878
#define CM_conf_gpmc_a5         								0x0854
#define CM_conf_gpmc_a6         								0x0858
#define CM_conf_gpmc_a7         								0x085C
#define CM_conf_gpmc_a8         								0x0860

#define GPIO_OE                 								0x134
#define GPIO_CLEARDATAOUT       								0x190
#define GPIO_SETDATAOUT         								0x194
#define GPIO_DATAIN 											0x138


#define PIN_LED_01												21
#define PIN_LED_02												22
#define PIN_LED_03												23
#define PIN_LED_04												24
#define PIN_BTN_01												28

#define GPIO 													7

#define HIGH													1
#define LOW														0
#define OUTPUT													0
#define INPUT													1

#endif  /* _DEFINES_ */
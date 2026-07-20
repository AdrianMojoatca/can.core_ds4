/*<#======================================================================#>*/
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: 800_gpio.h 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

#ifndef __933_GPIO_H__
#define __933_GPIO_H__

#include "device.h"
#include "config_gpio.h"
#include "config_ngmm_feature.h"
//#include "ngmm_feature.h"

/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/

/*==========================================================================*/

/*==========================================================================*/

#if CONFIG_GPIO_VERSION != 2  // must be changed when config file is modified
    #error "config_gpio is not up to date"
#endif

typedef UInt32 Gpio_Pl_T;

enum{
	GPIO_PL_PORT_0,
	GPIO_PL_PORT_1,
	GPIO_PL_PORT_2,
	GPIO_PL_PORT_3,
	GPIO_PL_PORT_4
};




 enum{
	GPIO_PL_PIN_0,
	GPIO_PL_PIN_1,
	GPIO_PL_PIN_2,
	GPIO_PL_PIN_3,
	GPIO_PL_PIN_4,
	GPIO_PL_PIN_5,
	GPIO_PL_PIN_6,
	GPIO_PL_PIN_7,
	GPIO_PL_PIN_8,
	GPIO_PL_PIN_9,
	GPIO_PL_PIN_10,
	GPIO_PL_PIN_11,
	GPIO_PL_PIN_12,
	GPIO_PL_PIN_13,
	GPIO_PL_PIN_14,
	GPIO_PL_PIN_15,
	GPIO_PL_PIN_16,
	GPIO_PL_PIN_17,
	GPIO_PL_PIN_18,
	GPIO_PL_PIN_19,
	GPIO_PL_PIN_20,
	GPIO_PL_PIN_21,
	GPIO_PL_PIN_22,
	GPIO_PL_PIN_23,
	GPIO_PL_PIN_24,
	GPIO_PL_PIN_25,
	GPIO_PL_PIN_26,
	GPIO_PL_PIN_27,
	GPIO_PL_PIN_28,
	GPIO_PL_PIN_29,
	GPIO_PL_PIN_30,
	GPIO_PL_PIN_31
};



 enum{
	GPIO_PL_INP,
	GPIO_PL_OUTP
};


//00 Pin has an on-chip pull-up resistor enabled. 00
//01 Repeater mode (see text below).
//10 Pin has neither pull-up nor pull-down resistor enabled.
//11 Pin has an on-chip pull-down resistor enabled.

 enum{
	GPIO_PL_PULLUP,
	GPIO_PL_REPEATER,
	GPIO_PL_NO_PULLUP_PULLDWN,
	GPIO_PL_PULLDWN	
};


 enum{
	GPIO_PL_GPIO,
	GPIO_PL_FUNC_1,
	GPIO_PL_FUNC_2,
	GPIO_PL_FUNC_3
};



 enum{
	GPIO_PL_CHAN_0,
	GPIO_PL_CHAN_1,
	GPIO_PL_CHAN_2,
	GPIO_PL_CHAN_3,
	GPIO_PL_CHAN_4,
	GPIO_PL_CHAN_5,
	GPIO_PL_CHAN_6,
	GPIO_PL_CHAN_7
};

// internal macros for bit positions
#define GPIO_PL_POS_PIN  0
#define GPIO_PL_POS_PORT 5
#define GPIO_PL_POS_FUNC 8
#define GPIO_PL_POS_MODE 10
#define GPIO_PL_POS_CHAN 12
#define GPIO_PL_POS_DIR  15

// internal macros describing number of bits in field
#define GPIO_PL_BITS_PIN  5
#define GPIO_PL_BITS_PORT 3
#define GPIO_PL_BITS_FUNC 2
#define GPIO_PL_BITS_MODE 2
#define GPIO_PL_BITS_CHAN 3
#define GPIO_PL_BITS_DIR  1


// internal macros for normalized mask names
#define GPIO_PL_MASK_PIN  ((1<<GPIO_PL_BITS_PIN)-1)
#define GPIO_PL_MASK_PORT ((1<<GPIO_PL_BITS_PORT)-1)
#define GPIO_PL_MASK_FUNC ((1<<GPIO_PL_BITS_FUNC)-1)
#define GPIO_PL_MASK_MODE ((1<<GPIO_PL_BITS_MODE)-1)
#define GPIO_PL_MASK_CHAN ((1<<GPIO_PL_BITS_CHAN)-1)
#define GPIO_PL_MASK_DIR  ((1<<GPIO_PL_BITS_DIR)-1)



#define GPIO_PL_PORT_NUM(pl)		((pl>>GPIO_PL_POS_PORT) & GPIO_PL_MASK_PORT)
#define GPIO_PL_PIN_NUM(pl)			((pl>>GPIO_PL_POS_PIN) & GPIO_PL_MASK_PIN)
#define GPIO_PL_FUNC_NUM(pl)		((pl>>GPIO_PL_POS_FUNC) & GPIO_PL_MASK_FUNC)
#define GPIO_PL_PIN_MODE(pl)		((pl>>GPIO_PL_POS_MODE) & GPIO_PL_MASK_MODE)
#define GPIO_PL_CHAN_NUM(pl)		((pl>>GPIO_PL_POS_CHAN) & GPIO_PL_MASK_CHAN)


// pinlabel macros for each signal

#define GPIO_PL( port, pin, dir, mode, func, chan )		(((dir & GPIO_PL_MASK_DIR)<< GPIO_PL_POS_DIR) |\
													 ((chan & GPIO_PL_MASK_CHAN) << GPIO_PL_POS_CHAN) |\
													 ((mode & GPIO_PL_MASK_MODE) << GPIO_PL_POS_MODE) |\
													 ((func & GPIO_PL_MASK_FUNC)<< GPIO_PL_POS_FUNC) |\
													 ((port & GPIO_PL_MASK_PORT) << GPIO_PL_POS_PORT) |\
													 ((pin & GPIO_PL_MASK_PIN) << GPIO_PL_POS_PIN))
																							 
																							 
																							 
#define GPIO_PL_EXT_CAN_HS_RX				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_0,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_0)				
#define GPIO_PL_EXT_CAN_HS_TX				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_1,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_0)			
#define GPIO_PL_P0_D2D0_TX    		    GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_2,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_D2D_TX								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_2,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, 0)		
#define GPIO_PL_AD7_NU								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_2,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_7)		
#define GPIO_PL_P0_D2D0_RX    		    GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_3,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_D2D_RX								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_3,  GPIO_PL_INP,	 GPIO_PL_PULLUP, GPIO_PL_FUNC_1, 0)			
#define GPIO_PL_AD6_NU								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_3,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_6)			
#define GPIO_PL_J1850_RX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_4,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			
#define GPIO_PL_CAN_SW_RX						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_4,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)			
#define GPIO_PL_CAN_SW_TX						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_5,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)		
#define GPIO_PL_CAN_SW_TX_NU				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_5,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_J1850_TX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_6,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_J1850_TX_NU						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_6,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_NEG_OUT12						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_7,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)					// Anti Grind
//#define GPIO_PL_RFLOOP_POWER					GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_7,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_IN_POS2							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_8,  GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, GPIO_PL_CHAN_1)		// Brake input
#define GPIO_PL_IN_POS3				     		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_9,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, GPIO_PL_CHAN_1)		// Ignition input		
#define GPIO_PL_ISO1_TX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_10, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			
#define GPIO_PL_ISO2_TX           		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_11, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_IN_NEG5			     		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_15,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		// Hood Input
#define GPIO_PL_IN_DOOR_NEG1						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_16, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// DOOR SENSE INPUT (-) IN_POS_NEG_1
//#define GPIO_PL_IN_AUX4							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_17, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			
#define GPIO_PL_NEG_OUT9         	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_18, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				// Accessory
//#define GPIO_PL_DAC_PWR_CTL         	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_18, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_PULL_5V_ISO1         GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_19, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			
#define GPIO_PL_PULL_5V_ISO2         GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_20, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			
#define GPIO_PL_PULL_12V_ISO1        GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_21, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			
#define GPIO_PL_IN_NEG3_WAKE    			GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_22, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		// no default	
#define GPIO_PL_ISO1_RX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_23,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_AD0_NU								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_23,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, 0)	
#define GPIO_PL_ISO2_RX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_24,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_AD1_NU								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_24,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_1)	
#define GPIO_PL_AD_INP					   		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_25,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_2)		
//#define GPIO_PL_DAC_OUT							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_26,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, 0)		
#define GPIO_PL_DAC_OUT							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_26,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, 0)		
#define GPIO_PL_INPUT_TACH						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_27,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)					
#define GPIO_PL_UART3_WAKE						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_28,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)
#define GPIO_PL_MODE0           	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_29, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN						, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_NEG_OUT8           	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_29, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN						, GPIO_PL_GPIO, 0)	
#define GPIO_PL_STB_HSCAN           	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_30, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN						, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_NEG_OUT9           	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_30, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN						, GPIO_PL_GPIO, 0)	

#define GPIO_PL_RF_D2D_EN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_0,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_RFLOOP_POWER							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_1,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_EN_FTCAN							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_1,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_DAC_PWR_CTL						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_4,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_STB_HSCAN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_4,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_LED_BLUE						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_8,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_STB_FTCAN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_8,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_MODE1								GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_8,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_LED_GREEN								GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_9,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_MODE0								GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_9,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_LED_RED							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_10, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_LED_BLUE							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_10, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_NEG_OUT10						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_14, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// ignition main
//#define GPIO_PL_LED_GREEN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_14, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_NEG_OUT8							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_15, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// ignition flex
//#define GPIO_PL_LED_RED							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_15, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_RLY3_CTL						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_16, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_OUT_SIREN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_16, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_NEG_OUT11					GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_17, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// starter
//#define GPIO_PL_DAC_RLY_CTL					GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_17, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_EN_FTCAN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_18, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 1)	
//#define GPIO_PL_NEG_OUT11						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_18, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_RLY2_CTL						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_19, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_NEG_OUT12						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_19, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_OUT_NEG14							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_20, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// Horn output
//#define GPIO_PL_RLY1_CTL							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_20, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_NEG_OUT4							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_21, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// trunk release
//#define GPIO_PL_PWM_OUT							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_21, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_NEG_OUT1  						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_22, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// FAA OUTPUT
//#define GPIO_PL_RLY3_CTL  						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_22, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_NEG_OUT3							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_23, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// Lock output
//#define GPIO_PL_NEG_OUT1							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_23, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_RLY1_CTL							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_24, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
//#define GPIO_PL_NEG_OUT2							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_24, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_NEG_OUT2  						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_25, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// GWR OUTPUT
//#define GPIO_PL_NEG_OUT3  						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_25, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_IN_NEG3_CAP						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_26, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		// PORT1 no rising/falling edge register
#define GPIO_PL_IN_NEG3   						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_22, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		// no default value
#define GPIO_PL_POS_OUT7							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_27, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
//#define GPIO_PL_NEG_OUT4							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_27, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
//#define GPIO_PL_ISO3_TX          		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_28, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, 0)	
#define GPIO_PL_NEG_OUT5          		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_28, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// UNLOCK OUT
#define GPIO_PL_NEG_OUT13			    		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_29, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				
//#define GPIO_PL_RLY2_CTL			    		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_29, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_A_D_BATTERY	    		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_30, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_3, GPIO_PL_CHAN_4)	
#define GPIO_PL_A_D_TEMP_SENSOR   		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_31, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_3, GPIO_PL_CHAN_5)	

#define GPIO_PL_P1_D2D0_TX    		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_0,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_RF_D2D_TX		     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_0,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)		
#define GPIO_PL_P1_D2D0_RX    		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_1,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_RF_D2D_RX		     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_1,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)			
#define GPIO_PL_IN_DOOR_POS1     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_2,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, GPIO_PL_CHAN_1)				// Door sense input (+)
#define GPIO_PL_PUSH_IN		     			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_3,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	
#define GPIO_PL_IN_NEG1 							GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_4,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)	// Activation input
#define GPIO_PL_IN_NEG4			        GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_5, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		// Parking brake
#define GPIO_PL_IN_POS1							GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_6, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, GPIO_PL_CHAN_1)		// Instant Trigger - new input added to 933
#define GPIO_PL_EXT_CAN_FT_RX     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_7,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_1)	
#define GPIO_PL_EXT_CAN_FT_TX     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_8,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_1)	
#define GPIO_PL_IN_NEG7 			   			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_9,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// TRUNK SENSE INPUT
#define GPIO_PL_ISP_PROG    					GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_10,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_PULL_12V_ISO2   			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_11,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_OUT_SIREN       			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_12,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// Siren Output
//#define GPIO_PL_POS_OUT6       			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_12,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)
#define GPIO_PL_POS_OUT6       			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_13,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)

#define GPIO_PL_STB_FTCAN						GPIO_PL(GPIO_PL_PORT_3, GPIO_PL_PIN_25, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_MODE1								GPIO_PL(GPIO_PL_PORT_3, GPIO_PL_PIN_25, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
//#define GPIO_PL_NEG_OUT10						GPIO_PL(GPIO_PL_PORT_3, GPIO_PL_PIN_25, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
#define GPIO_PL_DAC_RLY_CTL						GPIO_PL(GPIO_PL_PORT_3, GPIO_PL_PIN_26, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		
//#define GPIO_PL_NEG_OUT13						GPIO_PL(GPIO_PL_PORT_3, GPIO_PL_PIN_26, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		

#define GPIO_PL_DBG_PORT_TX					GPIO_PL(GPIO_PL_PORT_4, GPIO_PL_PIN_28, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_3, GPIO_PL_CHAN_3)		
#define GPIO_PL_DBG_PORT_RX					GPIO_PL(GPIO_PL_PORT_4, GPIO_PL_PIN_29, GPIO_PL_INP,	 GPIO_PL_PULLUP, GPIO_PL_FUNC_3, GPIO_PL_CHAN_3)		
//--------------------------------------------------------------------------//
//EXTERNAL INTERRUPTS
//--------------------------------------------------------------------------//

#define EXT_CAN_HS_RX                    (1u << GPIO_PL_PIN_NUM(GPIO_PL_EXT_CAN_HS_RX))	//P0.0
#define EXT_RX_D2D                       (1u << GPIO_PL_PIN_NUM(GPIO_PL_D2D_RX))					//P0.3
#define EXT_CAN_SW_RX                    (1u << GPIO_PL_PIN_NUM(GPIO_PL_CAN_SW_RX))				//P0.4
#define EXT_BRAKE                        (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_POS2))				//P0.8 //brake input
#define EXT_IGN                          (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_POS3))					//P0.9
#define EXT_NEGATIVE_5                   (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEG5))				//P0.15
#define EXT_DOOR_NEG_1                   (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_DOOR_NEG1))			//P0.16
#define EXT_NEGATIVE_3              			(1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEG3_WAKE))				//P0.22
#define EXT_PHAL1_RX                     (1u << GPIO_PL_PIN_NUM(GPIO_PL_ISO1_RX))				//P0.23 
#define EXT_PHAL2_RX                     (1u << GPIO_PL_PIN_NUM(GPIO_PL_ISO2_RX))				//P0.24
#define EXT_TACH                         (1u << GPIO_PL_PIN_NUM(GPIO_PL_INPUT_TACH))			//P0.27
#define EXT_UART3_WAKE                   (1u << GPIO_PL_PIN_NUM(GPIO_PL_UART3_WAKE))			//P0.28


    
#define EXT_RF                           (1u << GPIO_PL_PIN_NUM(GPIO_PL_RF_D2D_RX))			//P2.1
#define EXT_DOOR_POS_1                   (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_DOOR_POS1))	//P2.2
#define EXT_NEGATIVE_1                   (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEG1))					//P2.4
#define EXT_PARKING_BRAKE                (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEG4))				//P2.5
#define EXT_INST_ALARM                   (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_POS1))				//P2.6
#define EXT_CAN_FT_RX                    (1u << GPIO_PL_PIN_NUM(GPIO_PL_EXT_CAN_FT_RX))	//P2.7
#define EXT_TRUNK                        (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEG7))					//P2.9

//--------------------------------------------------------------------------//
#define GPIO_WAKE_EXT0_MASK   ( EXT_CAN_HS_RX   			|\
                                EXT_RX_D2D      			|\
                                EXT_CAN_SW_RX   			|\
                                EXT_BRAKE    					|\
                                EXT_IGN    						|\
                                EXT_NEGATIVE_5   			|\
                                EXT_DOOR_NEG_1				|\
                                EXT_NEGATIVE_3				|\
                                EXT_PHAL1_RX    			|\
                                EXT_PHAL2_RX    			|\
                                EXT_TACH        			|\
																EXT_UART3_WAKE )

#define GPIO_WAKE_EXT2_MASK   ( EXT_NEGATIVE_1				|\
                                EXT_DOOR_POS_1  			|\
                                EXT_CAN_FT_RX					|\
                                EXT_INST_ALARM   			|\
                                EXT_PARKING_BRAKE			|\
                                EXT_RF								|\
                                EXT_TRUNK	)
																
//D2D------------------------------------------------------------------------//
#define IN_RX_D2D                       gpio_pl_pin_read(GPIO_PL_D2D_RX)														//P0.3
#define OUT_TX_D2D(x)                   (x ? (gpio_pl_set(GPIO_PL_D2D_TX)) : (gpio_pl_clr(GPIO_PL_D2D_TX)))		//P0.2
//---------------------------------------------------------------------------//


//RF PORT--------------------------------------------------------------------//
#define IN_RX_RF                        gpio_pl_pin_read(GPIO_PL_RF_D2D_RX)																//   P2.1
#define OUT_TX_RF(x)                    (x ? (gpio_pl_set(GPIO_PL_RF_D2D_TX)) : (gpio_pl_clr(GPIO_PL_RF_D2D_TX)))		//P2.0

//---------------------------------------------------------------------------//


//INPUTS---------------------------------------------------------------------//
#define IN_TACH                         gpio_pl_pin_read(GPIO_PL_INPUT_TACH)			

#define IN_POSITIVE_1                   gpio_pl_pin_read(GPIO_PL_IN_POS1) 	// new Input added to 933- default as Instant trigger
																					// inverted signal read on CM800 gpio_pl_pin_read(GPIO_PL_IN_POSITIVE_1)		Signal removed from NGMM, This signal is for GLow plug in deisel cars
																					// decide to handle it with more delay in startup time for deisel cars 
#define IN_POSITIVE_2                   gpio_pl_pin_read(GPIO_PL_IN_POS2)			
#define IN_POSITIVE_3                   gpio_pl_pin_read(GPIO_PL_IN_POS3)				


#define IN_NEGATIVE_1                   gpio_pl_pin_read(GPIO_PL_IN_NEG1)			
#define IN_NEGATIVE_2                   0	// signal removed from NGMM		
#define IN_NEGATIVE_3                   gpio_pl_pin_read(GPIO_PL_IN_NEG3)		
#define IN_NEGATIVE_4                   gpio_pl_pin_read(GPIO_PL_IN_NEG4)			
#define IN_NEGATIVE_5                   gpio_pl_pin_read(GPIO_PL_IN_NEG5)			
#define IN_NEGATIVE_6                   0			//signal removed from NGMM , neutral safety switch on main board removed , It will be handled from IVU button			
#define IN_NEGATIVE_7                   gpio_pl_pin_read(GPIO_PL_IN_NEG7)		

#define IN_NEG_AND_POS1                 (gpio_pl_pin_read(GPIO_PL_IN_DOOR_POS1) & gpio_pl_pin_read(GPIO_PL_IN_DOOR_NEG1))			


#define IN_BATT_VOLTAGE                 ADC_CHANNEL_4
#define IN_ANALOG_VOLTAGE               ADC_CHANNEL_2
#define IN_TEMP_SENSOR									ADC_CHANNEL_5
#define DAC_OUT													ADC_CHANNEL_3


#define IN_BATT_VOLTAGE_ENA                 (1u<<ADC_CHANNEL_4)
#define IN_ANALOG_VOLTAGE_ENA               (1u<<ADC_CHANNEL_2)
#define IN_TEMP_SENSOR_ENA									(1u<<ADC_CHANNEL_5)
#define DAC_OUT_ENA													(1u<<ADC_CHANNEL_3)

//--------------------------------------------------------------------------//


//J1850---------------------------------------------------------------------//
#define J1850_RX            		gpio_pl_pin_read(GPIO_PL_CAN_SW_RX)			
#define J1850_TX_SET           	gpio_pl_set(GPIO_PL_CAN_SW_TX)					
#define J1850_TX_CLR           	gpio_pl_clr(GPIO_PL_CAN_SW_TX)					

//PHAL1---------------------------------------------------------------------//
#define IN_PHAL1_RX                     gpio_pl_pin_read(GPIO_PL_ISO1_RX)		
#define OUT_PHAL1_TX(x)                 (x ? (gpio_pl_set(GPIO_PL_ISO1_TX)) : (gpio_pl_clr(GPIO_PL_ISO1_TX)))		
#define OUT_PHAL1_12(x)                 (x ? (gpio_pl_set(GPIO_PL_PULL_12V_ISO1)) : (gpio_pl_clr(GPIO_PL_PULL_12V_ISO1)))		
#define OUT_PHAL1_5(x)                  (x ? (gpio_pl_set(GPIO_PL_PULL_5V_ISO1)) : (gpio_pl_clr(GPIO_PL_PULL_5V_ISO1)))		
//--------------------------------------------------------------------------//


//PHAL1---------------------------------------------------------------------//
#define IN_PHAL2_RX                     gpio_pl_pin_read(GPIO_PL_ISO2_RX)		
#define OUT_PHAL2_TX(x)                 (x ? (gpio_pl_set(GPIO_PL_ISO2_TX)) : (gpio_pl_clr(GPIO_PL_ISO1_TX)))		
#define OUT_PHAL2_12(x)                 (x ? (gpio_pl_set(GPIO_PL_PULL_12V_ISO2)) : (gpio_pl_clr(GPIO_PL_PULL_12V_ISO2)))		
#define OUT_PHAL2_5(x)                  (x ? (gpio_pl_set(GPIO_PL_PULL_5V_ISO2)) : (gpio_pl_clr(GPIO_PL_PULL_5V_ISO2)))		
//--------------------------------------------------------------------------//


//---CORE OUTPUTS-------------------------------------------------------------------//

#define _800_OUT_1(x)                   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT1)) : (gpio_pl_clr(GPIO_PL_NEG_OUT1)))		
#define _800_OUT_2(x)                   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT2)) : (gpio_pl_clr(GPIO_PL_NEG_OUT2)))		
#define _800_OUT_3(x)                   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT3)) : (gpio_pl_clr(GPIO_PL_NEG_OUT3)))		
#define _800_OUT_4(x)                   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT4)) : (gpio_pl_clr(GPIO_PL_NEG_OUT4)))		
#define _800_OUT_5(x)                   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT5)) : (gpio_pl_clr(GPIO_PL_NEG_OUT5)))		
#define _800_OUT_6(x)                   (x ? (gpio_pl_set(GPIO_PL_POS_OUT6)) : (gpio_pl_clr(GPIO_PL_POS_OUT6)))		
#define _800_OUT_7(x)                   (x ? (gpio_pl_set(GPIO_PL_POS_OUT7)) : (gpio_pl_clr(GPIO_PL_POS_OUT7)))		
#define _800_OUT_8(x)                   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT8)) : (gpio_pl_clr(GPIO_PL_NEG_OUT8)))		
#define _800_OUT_9(x)                   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT9)) : (gpio_pl_clr(GPIO_PL_NEG_OUT9)))		
#define _800_OUT_10(x)                  (x ? (gpio_pl_set(GPIO_PL_NEG_OUT10)) : (gpio_pl_clr(GPIO_PL_NEG_OUT10)))	
#define _800_OUT_11(x)                  (x ? (gpio_pl_set(GPIO_PL_NEG_OUT11)) : (gpio_pl_clr(GPIO_PL_NEG_OUT11)))	
#define _800_OUT_12(x)                  (x ? (gpio_pl_set(GPIO_PL_NEG_OUT12)) : (gpio_pl_clr(GPIO_PL_NEG_OUT12)))	
#define _800_OUT_13(x)                  (x ? (gpio_pl_set(GPIO_PL_NEG_OUT13)) : (gpio_pl_clr(GPIO_PL_NEG_OUT13)))	
#define _800_OUT_14(x)                  (x ? (gpio_pl_set(GPIO_PL_OUT_NEG14)) : (gpio_pl_clr(GPIO_PL_OUT_NEG14)))			

#define _800_OUT_RELAY1(x)              (x ? (gpio_pl_set(GPIO_PL_RLY1_CTL)) : (gpio_pl_clr(GPIO_PL_RLY1_CTL)))		
#define _800_OUT_RELAY2(x)              (x ? (gpio_pl_set(GPIO_PL_RLY2_CTL)) : (gpio_pl_clr(GPIO_PL_RLY2_CTL)))		
#define _800_OUT_RELAY3(x)              (x ? (gpio_pl_set(GPIO_PL_RLY3_CTL)) : (gpio_pl_clr(GPIO_PL_RLY3_CTL)))		

#define OUT_1(x)       _800_OUT_1(x) 
#define OUT_2(x)       _800_OUT_2(x) 
#define OUT_3(x)       _800_OUT_3(x) 
#define OUT_4(x)       _800_OUT_4(x) 
#define OUT_5(x)       _800_OUT_5(x) 
#define OUT_6(x)       _800_OUT_6(x) 
#define OUT_7(x)       _800_OUT_7(x) 
#define OUT_8(x)       _800_OUT_8(x) 
#define OUT_9(x)       _800_OUT_9(x) 
#define OUT_10(x)      _800_OUT_10(x)
#define OUT_11(x)      _800_OUT_11(x)
#define OUT_12(x)      _800_OUT_12(x)
#define OUT_13(x)      _800_OUT_13(x)
#define OUT_14(x)      _800_OUT_14(x)
#define OUT_RELAY1(x)  _800_OUT_RELAY1(x)
#define OUT_RELAY2(x)  _800_OUT_RELAY2(x)
#define OUT_RELAY3(x)  _800_OUT_RELAY3(x)

#define OUT_TACHO(x)                    (x ? (gpio_pl_set(GPIO_PL_OUT_NEG14)) : (gpio_pl_clr(GPIO_PL_OUT_NEG14)))			
#define OUT_SIREN(x)                    (x ? (gpio_pl_set(GPIO_PL_OUT_SIREN)) : (gpio_pl_clr(GPIO_PL_OUT_SIREN)))	


#define OUT_1_PLUS(x)                   OUT_6(x)
#define OUT_2_PLUS(x)                   OUT_7(x)

//--------------------------------------------------------------------------//

#define IN_PUSH                          gpio_pl_pin_read(GPIO_PL_PUSH_IN)		

//#define OUT_LED_GREEN(x)                (x ? (gpio_pl_set(GPIO_PL_LED_GREEN)) : (gpio_pl_clr(GPIO_PL_LED_GREEN)))		
//#define OUT_LED_RED(x)                  (x ? (gpio_pl_set(GPIO_PL_LED_RED)) : (gpio_pl_clr(GPIO_PL_LED_RED)))		
#define OUT_LED_GREEN(x)                (x ? (gpio_pl_clr(GPIO_PL_LED_GREEN)) : (gpio_pl_set(GPIO_PL_LED_GREEN)))		
#define OUT_LED_RED(x)                  (x ? (gpio_pl_clr(GPIO_PL_LED_RED)) : (gpio_pl_set(GPIO_PL_LED_RED)))		
#define OUT_LED_BLUE(x)                  (x ? (gpio_pl_clr(GPIO_PL_LED_BLUE)) : (gpio_pl_set(GPIO_PL_LED_BLUE)))		

#define OUT_RF_LOOP(x)                  (x ? (gpio_pl_set(GPIO_PL_RFLOOP_POWER)) : (gpio_pl_clr(GPIO_PL_RFLOOP_POWER)))			
//#define OUT_DAC_RELAY(x)                (x ? (gpio_pl_set(GPIO_PL_DAC_RELAY)) : (gpio_pl_clr(GPIO_PL_DAC_RELAY)))		
#define OUT_DAC_RELAY(x)                (x ? (gpio_pl_set(GPIO_PL_DAC_PWR_CTL)) : (gpio_pl_clr(GPIO_PL_DAC_PWR_CTL)))	// ADI M - vers. 1_00	


#define OUT_STB_HS(x)                   (x ? (gpio_pl_set(GPIO_PL_STB_HSCAN)) : (gpio_pl_clr(GPIO_PL_STB_HSCAN)))		
#define OUT_EN_FT(x)                    (x ? (gpio_pl_set(GPIO_PL_EN_FTCAN)) : (gpio_pl_clr(GPIO_PL_EN_FTCAN)))		
#define OUT_STB_FT(x)                   (x ? (gpio_pl_set(GPIO_PL_STB_FTCAN)) : (gpio_pl_clr(GPIO_PL_STB_FTCAN)))		

#define OUT_MODE0_SW(x)                 (x ? (gpio_pl_set(GPIO_PL_MODE_0)) : (gpio_pl_clr(GPIO_PL_MODE_0)))		
#define OUT_MODE1_SW(x)                 (x ? (gpio_pl_set(GPIO_PL_MODE_1)) : (gpio_pl_clr(GPIO_PL_MODE_1)))		
    
//--------------------------------------------------------------------------//
//DBALL DATASTART DEFINES

#if NEG_OUT1_CFG == 1
//  #warning "No NEG_OUT1"
  #define NEG_OUT1_SET      
  #define NEG_OUT1_CLR      
#elif NEG_OUT1_CFG == 2
  #define NEG_OUT1_SET      OUT_1(TRUE)
  #define NEG_OUT1_CLR      OUT_1(FALSE)
#endif
#if NEG_OUT2_CFG == 1
 // #warning "No NEG_OUT2"
  #define NEG_OUT2_SET      
  #define NEG_OUT2_CLR      
#elif NEG_OUT2_CFG == 2
  #define NEG_OUT2_SET      OUT_2(TRUE)
  #define NEG_OUT2_CLR      OUT_2(FALSE)
#endif
#if NEG_OUT3_CFG == 1
  //#warning "No NEG_OUT3"
  #define NEG_OUT3_SET      
  #define NEG_OUT3_CLR      
#elif NEG_OUT3_CFG == 2
  #define NEG_OUT3_SET      OUT_3(TRUE)
  #define NEG_OUT3_CLR      OUT_3(FALSE)
#endif
#if NEG_OUT4_CFG == 1
 // #warning "No NEG_OUT4"
  #define NEG_OUT4_SET      
  #define NEG_OUT4_CLR      
#elif NEG_OUT4_CFG == 2
  #define NEG_OUT4_SET      OUT_4(TRUE)
  #define NEG_OUT4_CLR      OUT_4(FALSE) 
#endif
#if NEG_OUT5_CFG == 1
  //#warning "No NEG_OUT5"
  #define NEG_OUT5_SET      
  #define NEG_OUT5_CLR      
#elif NEG_OUT5_CFG == 2
  #define NEG_OUT5_SET      OUT_5(TRUE)
  #define NEG_OUT5_CLR      OUT_5(FALSE)
#endif
#if POS_OUT1_CFG == 1
  //#warning "No POS_OUT1"
  #define POS_OUT1_SET      
  #define POS_OUT1_CLR      
#elif POS_OUT1_CFG == 2
  #define POS_OUT1_SET      OUT_6(TRUE)
  #define POS_OUT1_CLR      OUT_6(FALSE)
#endif
#if POS_OUT2_CFG == 1
  //#warning "No POS_OUT2"
  #define POS_OUT2_SET      
  #define POS_OUT2_CLR      
#elif POS_OUT2_CFG == 2
  #define POS_OUT2_SET      OUT_7(TRUE)
  #define POS_OUT2_CLR      OUT_7(FALSE) 
#endif

#if RLY1_CTL_CFG == 1
  //#warning "No RLY1_CTL"
  #define RLY1_CTL_SET      
  #define RLY1_CTL_CLR      
#elif RLY1_CTL_CFG == 2
  #define RLY1_CTL_SET      OUT_RELAY1(TRUE)
  #define RLY1_CTL_CLR      OUT_RELAY1(FALSE)
#endif

#if RLY2_CTL_CFG == 1
  //#warning "No RLY2_CTL"
  #define RLY2_CTL_SET      
  #define RLY2_CTL_CLR      
#elif RLY2_CTL_CFG == 2
  #define RLY2_CTL_SET      OUT_RELAY2(TRUE)
  #define RLY2_CTL_CLR      OUT_RELAY2(FALSE)
#endif

#if RLY3_CTL_CFG == 1
  //#warning "No RLY3_CTL"
  #define RLY3_CTL_SET      
  #define RLY3_CTL_CLR      
#elif RLY3_CTL_CFG == 2
  #define RLY3_CTL_SET      OUT_RELAY3(TRUE)
  #define RLY3_CTL_CLR      OUT_RELAY3(FALSE)
#endif

#if ISO1_CFG == 1
 // #warning "No ISO1"
  #define ISO1_TX_SET                            
  #define ISO1_TX_CLR                   
  #define ISO1_RX                   
  #define PULL_12V_ISO1_SET            
  #define PULL_12V_ISO1_CLR           
  #define PULL_5V_ISO1_SET            
  #define PULL_5V_ISO1_CLR       
#elif ISO1_CFG == 2
  #define ISO1_TX_SET              	gpio_pl_set(GPIO_PL_ISO1_TX)		
  #define ISO1_TX_CLR              	gpio_pl_clr(GPIO_PL_ISO1_TX)		
  #define ISO1_RX               	gpio_pl_pin_read(GPIO_PL_ISO1_RX)	
  #define PULL_12V_ISO1_SET      	gpio_pl_set(GPIO_PL_PULL_12V_ISO1)	
  #define PULL_12V_ISO1_CLR      	gpio_pl_clr(GPIO_PL_PULL_12V_ISO1)		
  #define PULL_5V_ISO1_SET       	gpio_pl_set(GPIO_PL_PULL_5V_ISO1)			
  #define PULL_5V_ISO1_CLR       	gpio_pl_clr(GPIO_PL_PULL_5V_ISO1)			
#endif

#if ISO2_CFG == 1
  //#warning "No ISO2"
  #define ISO2_TX_SET       
  #define ISO2_TX_CLR
  #define ISO2_RX       
#elif ISO2_CFG == 2
  #define ISO2_TX_SET             	gpio_pl_set(GPIO_PL_ISO2_TX) 	
  #define ISO2_TX_CLR             	gpio_pl_clr(GPIO_PL_ISO2_TX)	
  #define ISO2_RX              		gpio_pl_pin_read(GPIO_PL_ISO2_RX)	
  #define PULL_12V_ISO2_SET         gpio_pl_set(GPIO_PL_PULL_12V_ISO2)  
  #define PULL_12V_ISO2_CLR         gpio_pl_clr(GPIO_PL_PULL_12V_ISO2)  
  #define PULL_5V_ISO2_SET          gpio_pl_set(GPIO_PL_PULL_5V_ISO2)   
  #define PULL_5V_ISO2_CLR          gpio_pl_clr(GPIO_PL_PULL_5V_ISO2)   
#endif

#if DAC_RLY_CTL_CFG == 1
  //#warning "No DAC_RLY_CTL"
  #define DAC_RLY_CTL_SET       
  #define DAC_RLY_CTL_CLR       
#elif DAC_RLY_CTL_CFG == 2
  #define DAC_RLY_CTL_IN        	gpio_pl_pin_read(GPIO_PL_DAC_RLY_CTL)	
  #define DAC_RLY_CTL_SET        gpio_pl_set(GPIO_PL_DAC_RLY_CTL)			
  #define DAC_RLY_CTL_CLR        gpio_pl_clr(GPIO_PL_DAC_RLY_CTL)			
#endif

#define PUSH_IN             gpio_pl_pin_read(GPIO_PL_PUSH_IN)		

#define LED_GREEN_TOGGLE    ((gpio_pl_pin_read(GPIO_PL_LED_GREEN)) ? (gpio_pl_clr(GPIO_PL_LED_GREEN)) : (gpio_pl_set(GPIO_PL_LED_GREEN)));		
#define LED_RED_TOGGLE      ((gpio_pl_pin_read(GPIO_PL_LED_RED)) ? (gpio_pl_clr(GPIO_PL_LED_RED)) : (gpio_pl_set(GPIO_PL_LED_RED)));			
#define LED_BOTH_TOGGLE     LED_GREEN_TOGGLE LED_RED_TOGGLE

#define LED_GREEN_SET       OUT_LED_GREEN(TRUE)
#define LED_GREEN_CLR       OUT_LED_GREEN(FALSE)
#define LED_RED_SET         OUT_LED_RED(TRUE)
#define LED_RED_CLR         OUT_LED_RED(FALSE)
#define STB_FTCAN_SET       OUT_STB_FT(TRUE)
#define STB_FTCAN_CLR       OUT_STB_FT(FALSE)
#define EN_FTCAN_SET        OUT_EN_FT(TRUE)
#define EN_FTCAN_CLR        OUT_EN_FT(FALSE)
#define STB_HSCAN_SET       OUT_STB_HS(TRUE)
#define STB_HSCAN_CLR       OUT_STB_HS(FALSE)


#if LOCK_IN_CFG == 2
  #define CUSTOM_NEG_IN1    IN_NEGATIVE_5         
  #define CMD_NEG5_ON       (INPUT_CUSTOM_NEG1 | INPUT_ON)
  #define CMD_NEG5_OFF      INPUT_CUSTOM_NEG1
#else 
//#warning "No LOCK_IN"  
#endif

#if UNLOCK_IN_CFG == 2
  #define CUSTOM_NEG_IN2    IN_NEGATIVE_4         
  #define CMD_NEG4_ON       (INPUT_CUSTOM_NEG2 | INPUT_ON)
  #define CMD_NEG4_OFF      INPUT_CUSTOM_NEG2
#else 
//#warning "No UNLOCK_IN"
#endif

#if TRUNK_IN_CFG == 2
  #define CUSTOM_NEG_IN3    IN_NEGATIVE_7         
  #define CMD_NEG7_ON       (INPUT_CUSTOM_NEG3 | INPUT_ON)
  #define CMD_NEG7_OFF      INPUT_CUSTOM_NEG3
#else 
//#warning "No TRUNK_IN_"
#endif

#if AUX1_IN_CFG == 2
  #define CUSTOM_NEG_IN4    IN_NEGATIVE_1         
  #define CMD_NEG1_ON       (INPUT_CUSTOM_NEG4 | INPUT_ON)
  #define CMD_NEG1_OFF      INPUT_CUSTOM_NEG4
#else 
//#warning "No AUX1_IN"
#endif

#if AUX2_IN_CFG == 2
  #define CUSTOM_NEG_IN5    IN_NEGATIVE_3         
  #define CMD_NEG3_ON       (INPUT_CUSTOM_NEG5 | INPUT_ON)
  #define CMD_NEG3_OFF      INPUT_CUSTOM_NEG5
#else 
//#warning "No AUX2_IN"
#endif

#if AUX3_IN_CFG == 2
  #define CUSTOM_NEG_IN6    IN_NEGATIVE_2         
  #define CMD_NEG2_ON       (INPUT_CUSTOM_NEG6 | INPUT_ON)
  #define CMD_NEG2_OFF      INPUT_CUSTOM_NEG6
#else 
//#warning "No AUX3_IN"
#endif

		// TODO : need to add new cofiguration for neutral safety switch
#if GWR_IN_CFG == 2
  #define CUSTOM_NEG_IN7    IN_NEGATIVE_6         
  #define CMD_NEG6_ON       (INPUT_CUSTOM_NEG7 | INPUT_ON)
  #define CMD_NEG6_OFF      INPUT_CUSTOM_NEG7
#else
//#warning "No GWR_IN"
#endif

#if POS_IN1_CFG == 2
  #define CUSTOM_POS_IN1    IN_POSITIVE_1         
  #define CMD_POS1_ON       (INPUT_CUSTOM_POS1 | INPUT_ON)
  #define CMD_POS1_OFF      INPUT_CUSTOM_POS1
#else 
//#warning "No POS_IN1"
#endif

#if IGN_IN_CFG == 2
  #define CUSTOM_POS_IN2    IN_POSITIVE_2         
  #define CMD_POS2_ON       (INPUT_CUSTOM_POS2 | INPUT_ON)
  #define CMD_POS2_OFF      INPUT_CUSTOM_POS2
#else 
//#warning "No IGN_IN"
#endif

#if START_IN_CFG == 2
  #define CUSTOM_POS_IN3    IN_POSITIVE_3         
  #define CMD_POS3_ON       (INPUT_CUSTOM_POS3 | INPUT_ON)
  #define CMD_POS3_OFF      INPUT_CUSTOM_POS3
#else 
//#warning "No START_IN"
#endif

void gpio_init(void);
void gpio_pl_init(Gpio_Pl_T pl);


#include "gpio_pl.h"
#endif


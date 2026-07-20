/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: gpio.h 33394 2015-08-26 15:15:31Z suchita.yadav $
/*==========================================================================*/

#ifndef __GPIO_1700_H__
#define __GPIO_1700_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "core_contract_db3_gpio_config.h"

#if CORE_DB3_GPIO_CONFIG_VERSION != 2  // must be changed when config file is modified
    #warning config_gpio is not up to date
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


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
	GPIO_PL_CHAN_5
};


 // pin label type
typedef UInt32 Gpio_Pl_T;


//EXT INTERRUPTS-------------------------------------------------------------------//
/*
  A pin label is an integer we want to associate with a named signal.
  The named signal might appear on various port/pins depending on platform.

  bits 0..4   = pin number 0..31
  bits 5..7   = port number 0..7
  bits 8..9   = function number for PINSELx
  bits 10..11 = mode bits for PINMODEx
  bits 12..13 =  channel number (ex: for uart it selects which uart uart0..uart3)
  bit 14       (free)
  bit 15      = data direction for PINDIR

  bit 16..31   ( reserved if ever we exceed UInt16 )

*/


// pinlabel macros for each signal

#define GPIO_PL( port, pin, dir, mode, func, chan )		(((dir & GPIO_PL_MASK_DIR)<< GPIO_PL_POS_DIR) |\
													 ((chan & GPIO_PL_MASK_CHAN) << GPIO_PL_POS_CHAN) |\
													 ((mode & GPIO_PL_MASK_MODE) << GPIO_PL_POS_MODE) |\
													 ((func & GPIO_PL_MASK_FUNC)<< GPIO_PL_POS_FUNC) |\
													 ((port & GPIO_PL_MASK_PORT) << GPIO_PL_POS_PORT) |\
													 ((pin & GPIO_PL_MASK_PIN) << GPIO_PL_POS_PIN))
																							 
																							 
																							 
#define GPIO_PL_EXT_CAN_HS_RX				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_0,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_0)				// correct
#define GPIO_PL_EXT_CAN_HS_TX				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_1,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_0)			// correct
#define GPIO_PL_D2D_TX								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_2,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_0)		// correct
#define GPIO_PL_D2D_RX								GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_3,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_0)			// correct
#define GPIO_PL_J1850_RX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_4,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_CAN_SW_RX						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_4,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)			// correct
#define GPIO_PL_CAN_SW_TX						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_5,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)				// correct
#define GPIO_PL_CAN_SW_TX_NU				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_5,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				// correct
#define GPIO_PL_J1850_TX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_6,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				// correct
#define GPIO_PL_J1850_TX_NU						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_6,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				// correct
#define GPIO_PL_RFLOOP_POWER					GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_7,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				// correct
#define GPIO_PL_IN_POSITIVE_2     		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_9,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		// correct	// start		
#define GPIO_PL_IN_POSITIVE_3				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_8,  GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct	// car ignition
#define GPIO_PL_ISO1_TX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_10, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_ISO2_TX           		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_11, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_IN_NEGATIVE_1     		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_15,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_IN_NEGATIVE_2				GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_16, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_IN_AUX4							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_17, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_DAC_PWR_CTL         	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_18, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_PULL_5V_ISO1         GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_19, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_PULL_5V_ISO2         GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_20, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_PULL_12V_ISO1        GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_21, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_IN_NEGATIVE_6     		GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_22, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// check	// GWR
#define GPIO_PL_AUX3			      			GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_22, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_ISO1_RX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_23,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)						// correct
#define GPIO_PL_ISO2_RX							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_24,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)						// correct
#define GPIO_PL_A_D_INP							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_25,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_2)						// correct
#define GPIO_PL_DAC_OUT							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_26,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, 0)						// correct
#define GPIO_PL_INPUT_TACH						GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_27,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)						//check
#define GPIO_PL_FREE_PIN							GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_28,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)
#define GPIO_PL_NEG_OUT8           	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_29, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN						, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_NEG_OUT9           	GPIO_PL(GPIO_PL_PORT_0, GPIO_PL_PIN_30, GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN						, GPIO_PL_GPIO, 0)			// correct

// mapped by DB3 core contract
#define GPIO_PL_RF_D2D_EN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_0,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_EN_FTCAN							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_1,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 1)			// correct
#define GPIO_PL_STB_HSCAN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_4,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_STB_FTCAN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_8,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_MODE1								GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_8,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_MODE0								GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_9,  GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// check
#define GPIO_PL_LED_BLUE							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_10, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// only for dev board
#define GPIO_PL_LED_GREEN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_14, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_LED_RED							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_15, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_OUT_SIREN						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_16, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_DAC_RLY_CTL					GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_17, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			//correct
#define GPIO_PL_NEG_OUT11						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_18, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			//correct
#define GPIO_PL_NEG_OUT12						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_19, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			//correct
#define GPIO_PL_RLY1_CTL							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_20, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		////correct
#define GPIO_PL_PWM_OUT							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_21, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_3)		//correct
#define GPIO_PL_RLY3_CTL  						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_22, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_NEG_OUT1							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_23, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_NEG_OUT2							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_24, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_NEG_OUT3  						GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_25, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			//correct
#define GPIO_PL_ISO3_RX							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_26, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_NEG_OUT4							GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_27, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_ISO3_TX          		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_28, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_NEG_OUT5          		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_28, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_RLY2_CTL			    		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_29, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			//correct
#define GPIO_PL_A_D_BATTERY	    		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_30, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_4)			//correct
#define GPIO_PL_A_D_TEMP_SENSOR   		GPIO_PL(GPIO_PL_PORT_1, GPIO_PL_PIN_31, GPIO_PL_INP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_5)			//correct




#define GPIO_PL_RF_D2D_TX		     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_0,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)				// correct				
#define GPIO_PL_RF_D2D_RX		     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_1,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_2, GPIO_PL_CHAN_1)				// correct				
#define GPIO_PL_IN_POSITIVE_1     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_2,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				// correct				
#define GPIO_PL_PUSH_IN		     			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_3,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)		// correct
#define GPIO_PL_IN_NEGATIVE_7  			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_4,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 1)			// correct
#define GPIO_PL_IN_NEGATIVE_5        GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_5, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct			// Input lock
#define GPIO_PL_IN_NEGATIVE_4				GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_6, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct			// unlock
#define GPIO_PL_EXT_CAN_FT_RX     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_7,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_1)			// correct
#define GPIO_PL_EXT_CAN_FT_TX     		GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_8,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_1, GPIO_PL_CHAN_1)			// correct
#define GPIO_PL_IN_NEGATIVE_3   			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_9,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_ISP_PROG    					GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_10,  GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_PULL_12V_ISO2   			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_11,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)			// correct
#define GPIO_PL_POS_OUT1       			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_12,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)
#define GPIO_PL_POS_OUT2       			GPIO_PL(GPIO_PL_PORT_2, GPIO_PL_PIN_13,  GPIO_PL_OUTP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)


#define GPIO_PL_NEG_OUT10						GPIO_PL(GPIO_PL_PORT_3, GPIO_PL_PIN_25, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)					// correct		
#define GPIO_PL_NEG_OUT13						GPIO_PL(GPIO_PL_PORT_3, GPIO_PL_PIN_26, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_GPIO, 0)				// correct


#define GPIO_PL_DBG_PORT_TX					GPIO_PL(GPIO_PL_PORT_4, GPIO_PL_PIN_28, GPIO_PL_OUTP, GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_3, GPIO_PL_CHAN_3)			// correct
#define GPIO_PL_DBG_PORT_RX					GPIO_PL(GPIO_PL_PORT_4, GPIO_PL_PIN_29, GPIO_PL_INP,	 GPIO_PL_NO_PULLUP_PULLDWN, GPIO_PL_FUNC_3, GPIO_PL_CHAN_3)			// correct




// port 0 wake up
#define EXT_RX_D2D                   (1u << GPIO_PL_PIN_NUM(GPIO_PL_D2D_RX) )
#define EXT_CAN_HS_RX                (1u << GPIO_PL_PIN_NUM(GPIO_PL_EXT_CAN_HS_RX) ) 
#define EXT_PHAL2_RX                 (1u << GPIO_PL_PIN_NUM(GPIO_PL_ISO2_RX) ) 
#define EXT_CAN_SW_RX                (1u << GPIO_PL_PIN_NUM(GPIO_PL_CAN_SW_RX) ) 
#define EXT_PHAL1_RX                 (1u << GPIO_PL_PIN_NUM(GPIO_PL_ISO1_RX) )  
#define EXT_W2W_UNLOCK               (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEGATIVE_6) )	
#define EXT_W2W_TRUNK                (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEGATIVE_1) ) 
#define EXT_W2W_AUX2                 (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEGATIVE_2) )
#define EXT_W2W_INPUT1               (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_POSITIVE_3) )
#define EXT_W2W_IGN                  (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_POSITIVE_2) )	


// Port 2 wake up
#define EXT_W2W_START                (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_POSITIVE_1) )
#define EXT_CAN_FT_RX                (1u << GPIO_PL_PIN_NUM(GPIO_PL_EXT_CAN_FT_RX) )
#define EXT_W2W_LOCK                 (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEGATIVE_5) )	
#define EXT_W2W_AUX1                 (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEGATIVE_4) )
#define EXT_W2W_AUX3                 (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEGATIVE_3) )
#define EXT_W2W_GWR                  (1u << GPIO_PL_PIN_NUM(GPIO_PL_IN_NEGATIVE_7) ) 

//--------------------------------------------------------------------------//
#define GPIO_WAKE_EXT0_MASK            EXT_RX_D2D			|\
																			 EXT_CAN_HS_RX  |\
																			 EXT_PHAL2_RX   |\
																			 EXT_CAN_SW_RX  |\
																			 EXT_PHAL1_RX   |\
																			 EXT_W2W_UNLOCK |\
																			 EXT_W2W_TRUNK  |\
																			 EXT_W2W_AUX2   |\
																			 EXT_W2W_INPUT1 |\
																			 EXT_W2W_IGN 


#define GPIO_WAKE_EXT2_MASK              EXT_W2W_START		|\
																				 EXT_CAN_FT_RX    |\
																				 EXT_W2W_LOCK     |\
																				 EXT_W2W_AUX1     |\
																				 EXT_W2W_AUX3     |\
																				 EXT_W2W_GWR                 


//--------------------------------------------------------------------------//
#ifndef CORE_DB3_ENABLE_LEGACY_GPIO_API
#define CORE_DB3_ENABLE_LEGACY_GPIO_API 0
#endif

#if CORE_DB3_ENABLE_LEGACY_GPIO_API

#define LED_GREEN_TOGGLE gpio_pl_tog(GPIO_PL_LED_GREEN);
#define LED_RED_TOGGLE   gpio_pl_tog(GPIO_PL_LED_RED);
#define LED_BOTH_TOGGLE  LED_GREEN_TOGGLE LED_RED_TOGGLE
//--------------------------------------------------------------------------//


#if NEG_OUT1_CFG != 0
   #define NEG_OUT1_SET        gpio_pl_set(GPIO_PL_NEG_OUT1) 
   #define NEG_OUT1_CLR        gpio_pl_clr(GPIO_PL_NEG_OUT1) 
#endif
#if NEG_OUT2_CFG != 0
    #define NEG_OUT2_SET       gpio_pl_set(GPIO_PL_NEG_OUT2) 
    #define NEG_OUT2_CLR       gpio_pl_clr(GPIO_PL_NEG_OUT2) 
#endif
#if NEG_OUT3_CFG != 0
    #define NEG_OUT3_SET       gpio_pl_set(GPIO_PL_NEG_OUT3) 
    #define NEG_OUT3_CLR       gpio_pl_clr(GPIO_PL_NEG_OUT3) 
#endif
#if NEG_OUT4_CFG != 0
    #define NEG_OUT4_SET       gpio_pl_set(GPIO_PL_NEG_OUT4) 
    #define NEG_OUT4_CLR       gpio_pl_clr(GPIO_PL_NEG_OUT4) 
#endif
#if NEG_OUT5_CFG != 0
    #define NEG_OUT5_SET       gpio_pl_set(GPIO_PL_NEG_OUT5) 
    #define NEG_OUT5_CLR       gpio_pl_clr(GPIO_PL_NEG_OUT5) 
#endif
#if POS_OUT1_CFG != 0
    #define POS_OUT1_SET       gpio_pl_set(GPIO_PL_POS_OUT1) 
    #define POS_OUT1_CLR       gpio_pl_clr(GPIO_PL_POS_OUT1) 
#endif
#if POS_OUT2_CFG != 0
    #define POS_OUT2_SET       gpio_pl_set(GPIO_PL_POS_OUT2) 
    #define POS_OUT2_CLR       gpio_pl_clr(GPIO_PL_POS_OUT2) 
#endif
#if RLY1_CTL_CFG != 0
    #define RLY1_CTL_SET       gpio_pl_set(GPIO_PL_RLY1_CTL) 
    #define RLY1_CTL_CLR       gpio_pl_clr(GPIO_PL_RLY1_CTL) 
#endif
#if RLY2_CTL_CFG != 0
    #define RLY2_CTL_SET       gpio_pl_set(GPIO_PL_RLY2_CTL) 
    #define RLY2_CTL_CLR       gpio_pl_clr(GPIO_PL_RLY2_CTL) 
#endif
#if ISO1_CFG != 0
    #define ISO1_RX            gpio_pl_pin_read(GPIO_PL_ISO1_RX) 
    #define PULL_12V_ISO1_SET  gpio_pl_set(GPIO_PL_PULL_12V_ISO1) 
    #define PULL_12V_ISO1_CLR  gpio_pl_clr(GPIO_PL_PULL_12V_ISO1) 
    #define PULL_5V_ISO1_SET   gpio_pl_set(GPIO_PL_PULL_5V_ISO1) 
    #define PULL_5V_ISO1_CLR   gpio_pl_clr(GPIO_PL_PULL_5V_ISO1) 
    #define ISO1_TX_SET        gpio_pl_set(GPIO_PL_ISO1_TX) 
    #define ISO1_TX_CLR        gpio_pl_clr(GPIO_PL_ISO1_TX) 
#endif
#if ISO2_CFG != 0
    #define ISO2_RX            gpio_pl_pin_read(GPIO_PL_ISO2_RX) 		/*  P0[11]  INTERRUPT SOURCE / RX ISO2          */    
    #define ISO2_TX_SET        gpio_pl_set(GPIO_PL_ISO2_TX) 				/*  P0[10]  TX ISO2                             */
    #define ISO2_TX_CLR        gpio_pl_clr(GPIO_PL_ISO2_TX)   			/*  P0[10]  TX ISO2                             */
    #define PULL_12V_ISO2_SET  gpio_pl_set(GPIO_PL_PULL_12V_ISO2)  /*  P1[25]  PULL-UP 12V 1K ISO2                */
    #define PULL_12V_ISO2_CLR  gpio_pl_clr(GPIO_PL_PULL_12V_ISO2)  /*  P1[25]  PULL-UP 12V 1K ISO2               */
    #define PULL_5V_ISO2_SET   gpio_pl_set(GPIO_PL_PULL_5V_ISO2)   /*  P1[26]  PULL-UP 5V 1K ISO2                 */
    #define PULL_5V_ISO2_CLR   gpio_pl_clr(GPIO_PL_PULL_5V_ISO2)   /*  P1[26]  PULL-UP 5V 1K ISO2                 */

#endif
#if DAC_RLY_CTL_CFG != 0
    #define DAC_RLY_CTL_SET    gpio_pl_set(GPIO_PL_DAC_RLY_CTL)		
    #define DAC_RLY_CTL_CLR    gpio_pl_clr(GPIO_PL_DAC_RLY_CTL)		
#endif

//--------------------------------------------------------------------------//

#define EN_FTCAN_SET           gpio_pl_set(GPIO_PL_EN_FTCAN) 
#define EN_FTCAN_CLR           gpio_pl_clr(GPIO_PL_EN_FTCAN) 
#define STB_FTCAN_SET           		//gpio_pl_set(GPIO_PL_STB_FTCAN) 
#define STB_FTCAN_CLR           		//gpio_pl_clr(GPIO_PL_STB_FTCAN) 
#define PUSH_IN             	 gpio_pl_pin_read(GPIO_PL_PUSH_IN) 

#define OUT_LED_GREEN(x)       (x ? (gpio_pl_clr(GPIO_PL_LED_GREEN)) : (gpio_pl_set(GPIO_PL_LED_GREEN)))  
#define OUT_LED_RED(x)         (x ? (gpio_pl_clr(GPIO_PL_LED_RED)) : (gpio_pl_set(GPIO_PL_LED_RED)))  

#define LED_GREEN_SET          gpio_pl_set(GPIO_PL_LED_GREEN) 
#define LED_GREEN_CLR          gpio_pl_clr(GPIO_PL_LED_GREEN) 
#define LED_RED_SET            gpio_pl_clr(GPIO_PL_LED_RED) 
#define LED_RED_CLR            gpio_pl_set(GPIO_PL_LED_RED) 
#define STB_HSCAN_SET          gpio_pl_set(GPIO_PL_STB_HSCAN) 
#define STB_HSCAN_CLR          gpio_pl_clr(GPIO_PL_STB_HSCAN) 

#define J1850_RX            	 gpio_pl_pin_read(GPIO_PL_J1850_RX) 
#define J1850_TX_SET           gpio_pl_set(GPIO_PL_J1850_TX) 
#define J1850_TX_CLR           gpio_pl_clr(GPIO_PL_J1850_TX) 

#define RFLOOP_POWER_SET       gpio_pl_set(GPIO_PL_RFLOOP_POWER) 
#define RFLOOP_POWER_CLR       gpio_pl_clr(GPIO_PL_RFLOOP_POWER) 

#define IN_NEGATIVE_1        gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_1) 
#define IN_NEGATIVE_2        gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_2) 
#define IN_NEGATIVE_3        gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_3) 
#define IN_NEGATIVE_4        gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_4) 
#define IN_NEGATIVE_5        gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_5) 
#define IN_NEGATIVE_6        gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_6) 
#define IN_NEGATIVE_7        gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_7)
#define IN_POSITIVE_1        gpio_pl_pin_read(GPIO_PL_IN_POSITIVE_1) 
#define IN_POSITIVE_2        gpio_pl_pin_read(GPIO_PL_IN_POSITIVE_2) 
#define IN_POSITIVE_3        gpio_pl_pin_read(GPIO_PL_IN_POSITIVE_3) 

#if LOCK_IN_CFG == 0 && defined LOCK_IN_CFG                     
  #define W2W_NEG1_CMD      INPUT_INVALID
#elif LOCK_IN_CFG == 1 || !defined LOCK_IN_CFG 
  #define LOCK_IN           IN_NEGATIVE_1                      
  #define W2W_NEG1_CMD      INPUT_LOCK1
#elif LOCK_IN_CFG == 2
  #define CUSTOM_NEG_IN1    IN_NEGATIVE_1               
  #define W2W_NEG1_CMD      INPUT_CUSTOM_NEG1
#endif

#if UNLOCK_IN_CFG == 0 && defined UNLOCK_IN_CFG                     
  #define W2W_NEG2_CMD      INPUT_INVALID
#elif UNLOCK_IN_CFG == 1 || !defined UNLOCK_IN_CFG 
  #define UNLOCK_IN         IN_NEGATIVE_2              
  #define W2W_NEG2_CMD      INPUT_UNLOCK_DRIVER1
#elif UNLOCK_IN_CFG == 2
  #define CUSTOM_NEG_IN2    IN_NEGATIVE_2         
  #define W2W_NEG2_CMD      INPUT_CUSTOM_NEG2
#endif

#if TRUNK_IN_CFG == 0 && defined TRUNK_IN_CFG                   
  #define W2W_NEG3_CMD      INPUT_INVALID
#elif TRUNK_IN_CFG == 1 || !defined TRUNK_IN_CFG
  #define TRUNK_IN          IN_NEGATIVE_3              
  #define W2W_NEG3_CMD      INPUT_TRUNK
#elif TRUNK_IN_CFG == 2
  #define CUSTOM_NEG_IN3    IN_NEGATIVE_3         
  #define W2W_NEG3_CMD      INPUT_CUSTOM_NEG3
#endif

#if AUX1_IN_CFG == 0 && defined AUX1_IN_CFG                 
  #define W2W_NEG4_CMD      INPUT_INVALID
#elif AUX1_IN_CFG == 1 || !defined AUX1_IN_CFG
  #define AUX1_IN           IN_NEGATIVE_4                
  #define W2W_NEG4_CMD      INPUT_AUX1
#elif AUX1_IN_CFG == 2
  #define CUSTOM_NEG_IN4    IN_NEGATIVE_4        
  #define W2W_NEG4_CMD      INPUT_CUSTOM_NEG4
#endif

#if AUX2_IN_CFG == 0 && defined AUX2_IN_CFG                 
  #define W2W_NEG5_CMD      INPUT_INVALID
#elif AUX2_IN_CFG == 1 || !defined AUX2_IN_CFG
  #define AUX2_IN           IN_NEGATIVE_5                
  #define W2W_NEG5_CMD      INPUT_AUX2
#elif AUX2_IN_CFG == 2
  #define CUSTOM_NEG_IN5    IN_NEGATIVE_5         
  #define W2W_NEG5_CMD      INPUT_CUSTOM_NEG5
#endif

#if AUX3_IN_CFG == 0 && defined AUX3_IN_CFG                 
  #define W2W_NEG6_CMD      INPUT_INVALID
#elif AUX3_IN_CFG == 1 || !defined AUX3_IN_CFG
  #define AUX3_IN           IN_NEGATIVE_6                
  #define W2W_NEG6_CMD      INPUT_AUX3
#elif AUX3_IN_CFG == 2
  #define CUSTOM_NEG_IN6    IN_NEGATIVE_6         
  #define W2W_NEG6_CMD      INPUT_CUSTOM_NEG6
#endif

#if GWR_IN_CFG == 0 && defined GWR_IN_CFG               
  #define W2W_NEG7_CMD      INPUT_INVALID
#elif GWR_IN_CFG == 1 || !defined GWR_IN_CFG
  #define GWR_IN            IN_NEGATIVE_7                 
  #define W2W_NEG7_CMD      INPUT_GWR
#elif GWR_IN_CFG == 2
  #define CUSTOM_NEG_IN7    IN_NEGATIVE_7         
  #define W2W_NEG7_CMD      INPUT_CUSTOM_NEG7
#endif

#if POS_IN1_CFG == 0 && defined POS_IN1_CFG                 
  #define W2W_POS1_CMD      INPUT_INVALID
#elif POS_IN1_CFG == 1 || !defined POS_IN1_CFG
  #define POS_IN1           IN_POSITIVE_1                 
  #define W2W_POS1_CMD      INPUT_POS1
#elif POS_IN1_CFG == 2
  #define CUSTOM_POS_IN1    IN_POSITIVE_1         
  #define W2W_POS1_CMD      INPUT_CUSTOM_POS1
#endif

#if IGN_IN_CFG == 0 && defined IGN_IN_CFG               
  #define W2W_POS2_CMD      INPUT_INVALID
#elif IGN_IN_CFG == 1 || !defined IGN_IN_CFG
  #define IGN_IN            IN_POSITIVE_2                 
  #define W2W_POS2_CMD      INPUT_IGNITION
#elif IGN_IN_CFG == 2
  #define CUSTOM_POS_IN2    IN_POSITIVE_2         
  #define W2W_POS2_CMD      INPUT_CUSTOM_POS2
#endif

#if START_IN_CFG == 0 && defined START_IN_CFG                   
  #define W2W_POS3_CMD      INPUT_INVALID
#elif START_IN_CFG == 1 || !defined START_IN_CFG
  #define START_IN          IN_POSITIVE_3                 
  #define W2W_POS3_CMD      INPUT_STARTER
#elif START_IN_CFG == 2
  #define CUSTOM_POS_IN3    IN_POSITIVE_3         
  #define W2W_POS3_CMD      INPUT_CUSTOM_POS3
#endif

#define OUT_1(x)   (x ? (gpio_pl_set(GPIO_PL_NEG_OUT1)) : (gpio_pl_clr(GPIO_PL_NEG_OUT1)))

#endif /* CORE_DB3_ENABLE_LEGACY_GPIO_API */

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void gpio_init     (void) ;
void xk401_io_init (void)	;
void 		gpio_pl_init						(Gpio_Pl_T pl);


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#include "gpio_pl.h"
#endif



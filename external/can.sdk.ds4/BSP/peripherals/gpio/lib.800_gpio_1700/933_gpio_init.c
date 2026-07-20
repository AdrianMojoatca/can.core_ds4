/*==========================================================================*/
// $Id: 800_gpio_init.c 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "933_gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
UInt32 pin_init_val[] = 
{ 
	0x08418866,		//0x08458826,
	0x00000702,
	0x00000271,
	0x00000000,
	0x00000000,
};


const Gpio_Pl_T gpio_pl_table [100] = 
{
 GPIO_PL_EXT_CAN_HS_RX				,
 GPIO_PL_EXT_CAN_HS_TX				,
 GPIO_PL_D2D_TX							,
 GPIO_PL_D2D_RX							,
 GPIO_PL_J1850_RX						,
 GPIO_PL_CAN_SW_RX						,
 GPIO_PL_CAN_SW_TX_NU				,
 GPIO_PL_J1850_TX						,
 GPIO_PL_RFLOOP_POWER				,
 GPIO_PL_IN_POS3				     	,
 GPIO_PL_IN_POS2						,
 GPIO_PL_ISO1_TX							,
 GPIO_PL_ISO2_TX           	,
	GPIO_PL_IN_NEG5						,
	GPIO_PL_IN_DOOR_NEG1			,
	GPIO_PL_IN_NEG4			     		,
// GPIO_PL_IN_AUX4							,
 GPIO_PL_DAC_PWR_CTL         ,
 GPIO_PL_PULL_5V_ISO1        ,
 GPIO_PL_PULL_5V_ISO2        ,
 GPIO_PL_PULL_12V_ISO1       ,
 GPIO_PL_IN_NEG3_WAKE      		,
 GPIO_PL_ISO1_RX							,
 GPIO_PL_ISO2_RX							,
// GPIO_PL_AD_INP					   	,
// GPIO_PL_DAC_OUT							,
 GPIO_PL_INPUT_TACH					,
 GPIO_PL_NEG_OUT8           	,
 GPIO_PL_NEG_OUT9           	,

 GPIO_PL_RF_D2D_EN						,
 GPIO_PL_EN_FTCAN						,
 GPIO_PL_STB_HSCAN						,
 GPIO_PL_STB_FTCAN						,
 GPIO_PL_MODE1								,
 GPIO_PL_MODE0								,
 GPIO_PL_LED_BLUE						,
 GPIO_PL_LED_GREEN						,
 GPIO_PL_LED_RED							,
 GPIO_PL_OUT_NEG14						,
 GPIO_PL_OUT_SIREN						,
 GPIO_PL_DAC_RLY_CTL					,
 GPIO_PL_NEG_OUT11						,
 GPIO_PL_NEG_OUT12						,
 GPIO_PL_RLY1_CTL						,
 GPIO_PL_IN_NEG3						,
 GPIO_PL_NEG_OUT4							,
 GPIO_PL_RLY3_CTL  					,
 GPIO_PL_NEG_OUT1						,
 GPIO_PL_NEG_OUT2						,
 GPIO_PL_NEG_OUT3  					,
 GPIO_PL_RLY1_CTL							,
 GPIO_PL_NEG_OUT4						,
// GPIO_PL_ISO3_TX          		,
 GPIO_PL_NEG_OUT5          	,
 GPIO_PL_RLY2_CTL			    	,
// GPIO_PL_A_D_BATTERY	    		,
// GPIO_PL_A_D_TEMP_SENSOR   		,

 GPIO_PL_RF_D2D_TX		     		,
 GPIO_PL_RF_D2D_RX		     		,
 GPIO_PL_IN_DOOR_POS1     		,
 GPIO_PL_PUSH_IN		     			,
 GPIO_PL_IN_NEG1 							,
 GPIO_PL_IN_POS1							,
 GPIO_PL_EXT_CAN_FT_RX     		,
 GPIO_PL_EXT_CAN_FT_TX     		,
 GPIO_PL_IN_NEG7			   			,
 GPIO_PL_ISP_PROG    					,
 GPIO_PL_PULL_12V_ISO2   			,
 GPIO_PL_POS_OUT7      			,
 GPIO_PL_POS_OUT6       			,

 GPIO_PL_NEG_OUT10						,
 GPIO_PL_NEG_OUT13						,

 GPIO_PL_DBG_PORT_TX					,
 GPIO_PL_DBG_PORT_RX					,
 GPIO_PL_UART3_WAKE           ,


};

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void output_set_not_active(void)
{
	gpio_pl_set(GPIO_PL_STB_HSCAN);
	gpio_pl_clr(GPIO_PL_EN_FTCAN);
	gpio_pl_clr(GPIO_PL_DAC_PWR_CTL);
	
	gpio_pl_set(GPIO_PL_MODE0);
	gpio_pl_set(GPIO_PL_MODE1);
	
	gpio_pl_clr(GPIO_PL_EXT_CAN_HS_TX);
	gpio_pl_clr(GPIO_PL_EXT_CAN_FT_TX);
	
	gpio_pl_clr(GPIO_PL_CAN_SW_TX);
	gpio_pl_clr(GPIO_PL_DBG_PORT_TX);
	gpio_pl_clr(GPIO_PL_PULL_12V_ISO1);
	gpio_pl_clr(GPIO_PL_PULL_5V_ISO1);
	gpio_pl_clr(GPIO_PL_ISO1_TX);
	gpio_pl_clr(GPIO_PL_PULL_5V_ISO2);
	gpio_pl_clr(GPIO_PL_ISO2_TX);
	gpio_pl_clr(GPIO_PL_PULL_12V_ISO2);
	gpio_pl_clr(GPIO_PL_DAC_RLY_CTL);

	gpio_pl_clr(GPIO_PL_RFLOOP_POWER);
	gpio_pl_clr(GPIO_PL_NEG_OUT1);
	gpio_pl_clr(GPIO_PL_NEG_OUT2);
	gpio_pl_clr(GPIO_PL_NEG_OUT3);
	gpio_pl_clr(GPIO_PL_NEG_OUT4);
	gpio_pl_clr(GPIO_PL_OUT_NEG14);
	gpio_pl_clr(GPIO_PL_POS_OUT7);
	gpio_pl_clr(GPIO_PL_POS_OUT6);
	gpio_pl_clr(GPIO_PL_NEG_OUT5);
	gpio_pl_clr(GPIO_PL_NEG_OUT13);
	gpio_pl_clr(GPIO_PL_OUT_SIREN);

	gpio_pl_clr(GPIO_PL_NEG_OUT10);
	gpio_pl_clr(GPIO_PL_NEG_OUT8);
	gpio_pl_clr(GPIO_PL_NEG_OUT9);
	gpio_pl_clr(GPIO_PL_NEG_OUT11);
	gpio_pl_clr(GPIO_PL_NEG_OUT12);
	gpio_pl_clr(GPIO_PL_RLY3_CTL);
	gpio_pl_clr(GPIO_PL_RLY2_CTL);
	gpio_pl_clr(GPIO_PL_RLY1_CTL);
	
}


//--------------------------------------------------------------------------//

void gpio_init(void)
{
	// Initialize Port Pin settings to output register value to LOW
	UInt32 *addr ;
	UInt32 addr_ofs;
	Gpio_Pl_T pl;
	int count;
	
	addr = (UInt32 *) &FIO0PIN;
	for( count = 0; count < 5; count++)
	{
			addr_ofs = count * 8 ;
			*(addr + addr_ofs) = pin_init_val[count];
	}

	output_set_not_active();

	// Initialize Port Pin direction
	for( count = 0; gpio_pl_table[count] != NULL; count++)
	{
		pl = gpio_pl_table[count];
		gpio_pl_direction(pl);
	}
	
	//// IO PULL UP/DOWN SETTING
	for( count = 0; gpio_pl_table[count] != NULL; count++)
	{
		pl = gpio_pl_table[count];
		gpio_pl_mode(pl);
	}
	
	// Set Port pins for ISO2_TX, J1850_TX, CAN_HS_STB
	
	gpio_pl_set(GPIO_PL_ISO2_TX);
	gpio_pl_set(GPIO_PL_CAN_SW_TX);
	gpio_pl_set(GPIO_PL_STB_HSCAN);
	
	
 // IO SELECTION SETTING	  
 // select UART0, AD2, DAC, UART3
	
	gpio_pl_select(GPIO_PL_D2D_TX);
	gpio_pl_select(GPIO_PL_A_D_BATTERY);
	gpio_pl_select(GPIO_PL_DAC_OUT);
	gpio_pl_select(GPIO_PL_DBG_PORT_TX);
	gpio_pl_select(GPIO_PL_DBG_PORT_RX);
	
}

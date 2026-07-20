/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: gpio_init.c 33281 2015-08-19 19:52:03Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "isr.h"
#include "gpio.h"


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

const Gpio_Pl_T gpio_pl_table [100] = 
{
 GPIO_PL_EXT_CAN_HS_RX	,			
 GPIO_PL_EXT_CAN_HS_TX	,			
 GPIO_PL_D2D_TX				,			
 GPIO_PL_D2D_RX				,			
 GPIO_PL_J1850_RX			,			
 GPIO_PL_CAN_SW_RX			,		
 GPIO_PL_CAN_SW_TX_NU			,		
 GPIO_PL_J1850_TX			,			
 GPIO_PL_RFLOOP_POWER	,			
 GPIO_PL_IN_POSITIVE_2 ,    	
 GPIO_PL_IN_POSITIVE_3	,			
 GPIO_PL_ISO1_TX				,			
 GPIO_PL_ISO2_TX       ,    	
 GPIO_PL_IN_NEGATIVE_5 ,      
 GPIO_PL_IN_NEGATIVE_4	,
	GPIO_PL_IN_AUX4	,
 GPIO_PL_DAC_PWR_CTL   ,      
 GPIO_PL_PULL_5V_ISO1  ,      
 GPIO_PL_PULL_5V_ISO2  ,      
 GPIO_PL_PULL_12V_ISO1 ,      
 GPIO_PL_AUX3			    ,  		
 GPIO_PL_ISO1_RX				,			
 GPIO_PL_ISO2_RX				,			
 GPIO_PL_A_D_INP				,			
 GPIO_PL_DAC_OUT				,			
 GPIO_PL_INPUT_TACH		,	
 GPIO_PL_FREE_PIN 		,
 GPIO_PL_NEG_OUT8      ,     	
 GPIO_PL_NEG_OUT9      ,     	

 GPIO_PL_RF_D2D_EN			,		
 GPIO_PL_EN_FTCAN			,		
 GPIO_PL_STB_HSCAN			,		
 GPIO_PL_STB_FTCAN			,		
 GPIO_PL_MODE1					,		
 GPIO_PL_MODE0					,		
 GPIO_PL_LED_BLUE			,		
 GPIO_PL_LED_GREEN			,		
 GPIO_PL_LED_RED				,		
 GPIO_PL_OUT_SIREN			,		
 GPIO_PL_DAC_RLY_CTL		,		
 GPIO_PL_NEG_OUT11			,	
 GPIO_PL_NEG_OUT12			,		
 GPIO_PL_RLY1_CTL			,		
 GPIO_PL_PWM_OUT				,		
 GPIO_PL_RLY3_CTL  		,		
 GPIO_PL_NEG_OUT1			,		
 GPIO_PL_NEG_OUT2			,		
 GPIO_PL_NEG_OUT3  		,	
	GPIO_PL_ISO3_RX,
	GPIO_PL_IN_NEGATIVE_2	,		
 GPIO_PL_NEG_OUT4			,		
 GPIO_PL_ISO3_TX       ,   	
 GPIO_PL_NEG_OUT5      ,    
 GPIO_PL_RLY2_CTL			,    
 GPIO_PL_A_D_BATTERY	  ,  	
 GPIO_PL_A_D_TEMP_SENSOR ,  




 GPIO_PL_RF_D2D_TX		     ,	
 GPIO_PL_RF_D2D_RX		     ,	
 GPIO_PL_IN_POSITIVE_1    , 
 GPIO_PL_PUSH_IN		     	,	
 GPIO_PL_IN_NEGATIVE_7  	,	
 GPIO_PL_EXT_CAN_FT_RX    , 
 GPIO_PL_EXT_CAN_FT_TX    ,
 GPIO_PL_IN_NEGATIVE_3   ,	
 GPIO_PL_ISP_PROG    		,	
 GPIO_PL_PULL_12V_ISO2   ,	
 GPIO_PL_POS_OUT1       	,	
 GPIO_PL_POS_OUT2       		,
 GPIO_PL_IN_NEGATIVE_1    , 
 GPIO_PL_IN_NEGATIVE_6    , 


 GPIO_PL_NEG_OUT10					,
 GPIO_PL_NEG_OUT13					,


 GPIO_PL_DBG_PORT_TX				,
 GPIO_PL_DBG_PORT_RX			,	

};


UInt32 pin_init_val[] = 
{ 
	0x08458866,
	0x0000C402,
	0x00000270,
	0x00000000,
	0x00000000,
};


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void gpio_init (void)
{

// dummy function to keep same as DBALL2	
}


void xk401_io_init (void)
{
	// Initialize Port Pin settings to output register value to LOW
	Gpio_Pl_T pl;
	UInt32 * addr ;
	UInt32 addr_ofs = 0;
	int count;
	
	addr = (UInt32 *)& FIO0PIN;

	for( count = 0; count < 5; count++)
	{
		addr_ofs = count*0x8;
		*(addr+addr_ofs) = pin_init_val[count];
	}

	// Initialize Port Pin direction
	for( count = 0; gpio_pl_table[count] != NULL; count++)
	//for( count = 0; count < 73; count++)
	{
		pl = gpio_pl_table[count];
		gpio_pl_direction(pl);
	}
	
	// IO PULL UP/DOWN SETTING
	for( count = 0; gpio_pl_table[count] != NULL; count++)
	//for( count = 0; count < 73; count++)
	{
		pl = gpio_pl_table[count];
		gpio_pl_mode(pl);
	}
	
}


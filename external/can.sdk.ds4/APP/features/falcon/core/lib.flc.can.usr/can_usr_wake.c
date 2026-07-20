/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_wake.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_usr_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void wake ( void );
static void sleep( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void can_usr_wake( Wake_Status status )
{
    (status == s_WAKE) ? wake() : sleep() ;
}

//--------------------------------------------------------------------------//

void wake( void )
{        
    if( can_use[ CAN_DEV_0 ]  == TRUE )
    {        
        #if CAN_DEV_0_FT_SW == FALSE //SW CAN bus   
            gpio_pl_select         (GPIO_PL_CAN_SW_RX);
			gpio_pl_int_clr        (GPIO_PL_CAN_SW_RX);
			gpio_pl_falling_irq_dis(GPIO_PL_CAN_SW_RX);        
        #else
            gpio_pl_select         (GPIO_PL_EXT_CAN_FT_RX);
            gpio_pl_int_clr        (GPIO_PL_EXT_CAN_FT_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_EXT_CAN_FT_RX);
        #endif
                        
        //can_dev_ena( CAN_DEV_0 );
                        
        //CAN_BASE_0[CANMOD] &= ~(0x01 << 0)        ; // Enable CAN TX & RX operations
        
        OUT_EN_FT (TRUE);
        OUT_STB_FT(TRUE); 			                
    }
    
    
    if( can_use[ CAN_DEV_1 ]  == TRUE )
    {        
        gpio_pl_select         (GPIO_PL_EXT_CAN_HS_RX);
        gpio_pl_int_clr        (GPIO_PL_EXT_CAN_HS_RX);
		gpio_pl_falling_irq_dis(GPIO_PL_EXT_CAN_HS_RX);         
        
        //can_dev_ena( CAN_DEV_1 );
                
        //CAN_BASE_1[CANMOD] &= ~(0x01 << 0)        ; // Enable CAN TX & RX operations	
		        
        OUT_STB_HS( FALSE );                
    }    
}

//--------------------------------------------------------------------------//

void sleep( void )
{            
    if( can_use[ CAN_DEV_0 ]  == TRUE )
    {
        UInt16 i ; 
        
        //CAN_BASE_0[CANMOD] |= (0x01 << 0)        ;  // Disable CAN TX & RX operations					

		//CAN_BASE_0[CANICR] |= CAN_BASE_0[CANICR] ;  // Ack any pennding interrupt
        
        //can_dev_dis( CAN_DEV_0 );
                       
        OUT_EN_FT ( FALSE );    
        OUT_STB_FT( FALSE ); 

        #if CAN_DEV_0_FT_SW == FALSE //SW CAN bus   
            gpio_pl_func_cfg_gpio  (GPIO_PL_J1850_RX);
			gpio_pl_int_clr        (GPIO_PL_J1850_RX);
			gpio_pl_falling_irq_ena(GPIO_PL_J1850_RX);    
        #else
            gpio_pl_func_cfg_gpio  (GPIO_PL_CAN_SW_RX);
			gpio_pl_int_clr        (GPIO_PL_CAN_SW_RX);
			gpio_pl_falling_irq_ena(GPIO_PL_CAN_SW_RX);
        #endif        
    }    
    
    if( can_use[ CAN_DEV_1 ]  == TRUE )
    {        
       // CAN_BASE_1[CANMOD] |= (0x01 << 0)        ;  // Disable CAN TX & RX operations	
				
		//CAN_BASE_1[CANICR] |= CAN_BASE_1[CANICR] ;  // Ack any pennding interrupt
           
        //can_dev_dis( CAN_DEV_1 );

        OUT_STB_HS( TRUE );     

        gpio_pl_func_cfg_gpio  (GPIO_PL_EXT_CAN_HS_RX);
		gpio_pl_int_clr        (GPIO_PL_EXT_CAN_HS_RX);
		gpio_pl_falling_irq_ena(GPIO_PL_EXT_CAN_HS_RX);        
    }        
}

//--------------------------------------------------------------------------//            


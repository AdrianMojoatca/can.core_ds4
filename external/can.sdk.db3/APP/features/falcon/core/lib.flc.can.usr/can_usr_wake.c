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
#include "core_contract_db3_can_gpio.h"
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
        if( !CAN_USR_DEV0_IS_FT() )
        {
            core_contract_db3_can_gpio_irq_select_disable(CORE_DB3_CAN_GPIO_IRQ_SW_RX);
        }
        else
        {
            core_contract_db3_can_gpio_irq_select_disable(CORE_DB3_CAN_GPIO_IRQ_FT_RX);
        }
                        
        //can_dev_ena( CAN_DEV_0 );
                        
        //CAN_BASE_0[CANMOD] &= ~(0x01 << 0)        ; // Enable CAN TX & RX operations
        
        core_contract_db3_can_gpio_ft_enable(TRUE);
        core_contract_db3_can_gpio_ft_standby(TRUE);
    }
    
    
    if( can_use[ CAN_DEV_1 ]  == TRUE )
    {        
        core_contract_db3_can_gpio_irq_select_disable(CORE_DB3_CAN_GPIO_IRQ_HS_RX);
        
        //can_dev_ena( CAN_DEV_1 );
                
        //CAN_BASE_1[CANMOD] &= ~(0x01 << 0)        ; // Enable CAN TX & RX operations	
		        
        core_contract_db3_can_gpio_hs_active(TRUE);
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
                       
        core_contract_db3_can_gpio_ft_enable(FALSE);
        core_contract_db3_can_gpio_ft_standby(FALSE);

        if( !CAN_USR_DEV0_IS_FT() )
        {
            core_contract_db3_can_gpio_irq_gpio_enable(CORE_DB3_CAN_GPIO_IRQ_J1850_RX);
        }
        else
        {
            core_contract_db3_can_gpio_irq_gpio_enable(CORE_DB3_CAN_GPIO_IRQ_SW_RX);
        }        
    }    
    
    if( can_use[ CAN_DEV_1 ]  == TRUE )
    {        
       // CAN_BASE_1[CANMOD] |= (0x01 << 0)        ;  // Disable CAN TX & RX operations	
				
		//CAN_BASE_1[CANICR] |= CAN_BASE_1[CANICR] ;  // Ack any pennding interrupt
           
        //can_dev_dis( CAN_DEV_1 );

                core_contract_db3_can_gpio_hs_active(FALSE);

                core_contract_db3_can_gpio_irq_gpio_enable(CORE_DB3_CAN_GPIO_IRQ_HS_RX);
    }        
}

//--------------------------------------------------------------------------//            


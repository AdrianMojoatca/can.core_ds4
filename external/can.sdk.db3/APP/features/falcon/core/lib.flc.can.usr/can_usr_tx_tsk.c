/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_tx_tsk.c 19497 2013-02-25 13:41:22Z martin.bouchard $
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
static void high_voltage_setup( Can_Msg *can_msg );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean was_high_voltage = FALSE ; 
static Boolean is_high_voltage  = FALSE ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void can_usr_tx_tsk( void* dev_id )
{    
    #pragma push
    #pragma diag_suppress 767
        UInt8 DEV_ID = (UInt8)dev_id;
    #pragma pop

    Can_Msg can_msg ;
    UInt8 tx_retry_count ; 
    
    for( ; ; )
    {        
        pipe_remove( can_usr[DEV_ID].tx_pipe , (Pipe_Data)&can_msg , 0 ) ; 

        wake_enter();
        {                                   
            if ((DEV_ID == CAN_DEV_0) && !CAN_USR_DEV0_IS_FT())
            {
                high_voltage_setup( &can_msg );
            }
            
            can_usr_tx_one_shot( (Can_Dev_Id)(DEV_ID) , &can_msg , 200 );                       
        }
        wake_leave();
    }   
    
    #undef DEV_ID                      
}

//--------------------------------------------------------------------------//

void high_voltage_setup( Can_Msg *can_msg )
{
    is_high_voltage = can_msg->is_rtr ; 

    can_msg->is_rtr = FALSE ; 

    if( is_high_voltage && !was_high_voltage )
    {
        #ifdef DEV_LPC_236x
            core_contract_db3_can_gpio_mode1_set(TRUE);
            core_contract_db3_can_gpio_mode0_set(FALSE);
        #else
            core_contract_db3_can_gpio_mode0_set(FALSE);
            core_contract_db3_can_gpio_mode1_set(TRUE);
        #endif
        os_tsk_wait(10);        
    }

    if( !is_high_voltage && was_high_voltage )
    {                  
        #ifdef DEV_LPC_236x
            core_contract_db3_can_gpio_mode0_set(TRUE);
            core_contract_db3_can_gpio_mode1_set(TRUE);
		#else
            core_contract_db3_can_gpio_mode0_set(TRUE);
			core_contract_db3_can_gpio_mode1_set(TRUE);
        #endif
        
        os_tsk_wait(10);
    }

    was_high_voltage = is_high_voltage ; 
}

//--------------------------------------------------------------------------//


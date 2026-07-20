/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_handler_fiq.c 31193 2015-05-06 17:14:56Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ask_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static __inline void dei_ask_bit_time_update_fiq_mode(Boolean is_rising_edge);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ask_handler_fiq(void)
{
    if(DEI_ASK_EXT_ACTIVE())
    {    
        Boolean is_rising_edge; 
    
        dei_ask.current_timestamp = fiq_get_tc(FIQ_TC_TIMER_1);
        
        is_rising_edge = DEI_ASK_EXT_IS_RISING_EDGE();
        
        dei_ask_bit_time_update_fiq_mode(is_rising_edge);
                
        if(dei_ask.state)
        {
            (dei_ask.state)(is_rising_edge);
        }
        
        is_rising_edge ? DEI_ASK_EXT_ENAF() : DEI_ASK_EXT_ENAR();
                
        DEI_ASK_EXT_CLR();
    }
}

//--------------------------------------------------------------------------//    

__inline void dei_ask_bit_time_update_fiq_mode(Boolean is_rising_edge)
{
    UInt32 time_diff;
    
    if(dei_ask.current_timestamp > dei_ask.last_timestamp)
    {
        time_diff = dei_ask.current_timestamp - dei_ask.last_timestamp;
    }
    else
    {
        time_diff = (0xFFFFFFFF - dei_ask.last_timestamp)+dei_ask.current_timestamp ; 
    }
    
    if(is_rising_edge)
    {
        dei_ask.t_low = time_diff;                               
    }
    else
    {
        dei_ask.t_high = time_diff;
    }
    
    dei_ask.last_timestamp = dei_ask.current_timestamp;
}

//----------------------------------------------------------------------------//
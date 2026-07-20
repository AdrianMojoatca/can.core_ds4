/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_receive.c 31193 2015-05-06 17:14:56Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ask_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define     DEI_SC_MASTER_KEY           0xCB4195F222922334
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean dei_ask_receive( TskTimeout timeout)
{    
    while(TRUE)
    {
        DEI_ASK_EXT_ENA();
                        
        if(os_irq_wait( dei_ask.rx_irq,TRUE,timeout))
        {
            //we receive a frame , check if the frame is valid                  
            dei_ask.data_enc.data = keeloq_decrypt(dei_ask.data_enc.data , DEI_SC_MASTER_KEY); 
            
            dei_ask_trace_frame();
                                                
            if(dei_ask.data_enc.bits.button == dei_ask.data_fix.bits.button)
            {
                //we succesfully decoded the remote 
                
                if(dei_ask.data_fix.bits.repeat == TRUE)
                {
                    //we want to force the user to press long on the button , so will handle only the extended frames
                    
                    return TRUE ;
                }
                //else stay in loop and wait a valid frame or the timeout condition
            }            
        }
        else
        {
            return FALSE;
        }
    }                                                  
}

//--------------------------------------------------------------------------//    



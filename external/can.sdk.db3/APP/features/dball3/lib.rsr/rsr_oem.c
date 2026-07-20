/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_oem.c 18996 2013-01-24 22:12:09Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "cmd.h"
#include "status.h"
#include "prog.h"
#include "rf.h"
#include "display.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rsr_oem_count (void * arg) ;
                                      
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void rsr_oem_lock (void * arg)
{
#if DBG_RSR == 1
	dbg_tx_text ("\nrsr oem lock") ; 
#endif	
rsr_oem_count (0) ;   

}

//--------------------------------------------------------------------------//

void rsr_oem_unlock (void * arg)
{
   
   if(rpm.word && !os_flg_state (rsr_vars.flg_start))
    {
		rsr_oem_count (0) ;  
	}
	else
	{
		timeout_stop (rsr_vars.lock_start_timeout) ;
	}
#if DBG_RSR == 1
	dbg_tx_text ("\nrsr oem unlock") ;
#endif
}

//--------------------------------------------------------------------------//

void rsr_oem_trunk (void * arg)
{
    timeout_stop (rsr_vars.lock_start_timeout) ;
#if DBG_RSR == 1
	dbg_tx_text ("\nrsr oem trunk") ;
#endif
}

//--------------------------------------------------------------------------//

void rsr_oem_aux1 (void * arg)
{
    timeout_stop (rsr_vars.lock_start_timeout) ;
#if DBG_RSR == 1
	dbg_tx_text ("\nrsr oem aux1") ;
#endif
}

//--------------------------------------------------------------------------//

void rsr_oem_aux2 (void * arg)
{
    timeout_stop (rsr_vars.lock_start_timeout) ;
#if DBG_RSR == 1
	dbg_tx_text ("\nrsr oem aux2") ;
#endif
}

//--------------------------------------------------------------------------//

static void rsr_oem_count (void * arg)
{
 	static UInt8 rsr_oem_count ;
	
    if(!timeout_test(rsr_vars.lock_start_timeout))
    {
        if(++rsr_oem_count >= 3)
        {
            timeout_stop (rsr_vars.lock_start_timeout) ;
            
            if (!rsr_vars.gwr_is_detect && prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF)
            {
                display_error(7 ,1) ;
            }
            else if(prog_retrieve_option (FEAT_RF_OUTPUT) != NO_RF)
            {
              if(!os_flg_state (rsr_vars.flg_start))
                {
                  timeout_start(rsr_vars.start_delay_timeout)  ; 
                  
                  #if DBG_RSR == 1
                    dbg_tx_text ("\nRsr Start Delay Start") ;
                  #endif                   
                }
              else
                {
                  rsr_start_delay_timeout(arg) ;
                }                
              
              
               
            }
            
        }
    }
    else
    {
      rsr_oem_count = 1 ;
      timeout_start(rsr_vars.lock_start_timeout) ;
    }
}
//--------------------------------------------------------------------------//
void rsr_start_delay_timeout (void * arg)
{
  cmd_q_insert ((Cmd_List)(INPUT_START | INPUT_ON), COMMAND_INSERT_TIMEOUT) ;

  #if DBG_RSR == 1
	dbg_tx_text ("\nRsr Start Delay Timeout Done") ;
  #endif
  
}

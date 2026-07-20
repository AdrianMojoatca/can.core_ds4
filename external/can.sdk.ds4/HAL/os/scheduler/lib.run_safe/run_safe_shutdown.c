
#include "run_safe_private.h"
#include "config_gpio.h"
#include "cmd.h"
#include "sense_notify.h"
#include "status.h"
#include "rf.h"
#include "prog.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define RF_IS_DIS !!(prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                                   
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void run_safe_shutdown (void)
{
	os_tsk_wait (100)           ;	// add delay to avoid sending door open and brake press too close in D2D
    
    sense_notify_brake_skip(TRUE) ;
     
    #ifdef BRAKE_SENSE_SET
      if(RF_IS_DIS)
        {
          BRAKE_SENSE_SET ;
					
        }
    #endif
	
    ATOMIC (brake.bit.brake = 1);
    					  
    cmd_q_insert(INPUT_BRAKE_STATUS_PRESS, COMMAND_INSERT_TIMEOUT);
	
	os_tsk_wait (500)           ;
	
	cmd_q_insert(INPUT_BRAKE_STATUS_RELEASE, COMMAND_INSERT_TIMEOUT);

    ATOMIC (brake.bit.brake = 0);
    
    #ifdef BRAKE_SENSE_SET
	  if (RF_IS_DIS)
	    {
	      BRAKE_SENSE_CLR          ;
	    }
    #endif
    
    sense_notify_brake_skip(FALSE) ;
    
    #if DBG_RUN_SAFE == 1
        dbg_tx_text("\r\nRunSafe Shutdown") ;
    #endif 

}
/*==========================================================================*/


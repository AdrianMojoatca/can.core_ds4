/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_hood.c 32362 2015-06-16 18:52:36Z jean-francois.dube $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "status.h"
#include "cmd.h"
#include "rf.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                                      
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Boolean rsr_hood = FALSE ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void rsr_hood_open (void * arg)
{
	
      rsr_hood      = TRUE ;
      door.bit.hood = TRUE ;
     

	rf_icon_set (ICON_HOOD, FALSE) ;
	cmd_q_insert((Cmd)INPUT_HOOD_STATUS_OPEN, COMMAND_INSERT_TIMEOUT);

  #if DBG_RSR == 1
	  dbg_tx_text ("\nrsr hood open") ;
  #endif
}

//--------------------------------------------------------------------------//

void rsr_hood_close (void * arg)
{	
	
      rsr_hood      = FALSE ;
      door.bit.hood = FALSE ;
     

	rf_icon_clr (ICON_HOOD, FALSE) ;   
  cmd_q_insert((Cmd)INPUT_HOOD_STATUS_CLOSE, COMMAND_INSERT_TIMEOUT);

  #if DBG_RSR == 1
	  dbg_tx_text ("\nrsr hood close") ;
  #endif
}

//--------------------------------------------------------------------------//

void rsr_hood_detect (void * arg)
{
	if(rsr_hood != door.bit.hood) // data hood is detected, switch analog detection to valet
	  {
	  	#if DBG_RSR == 1
	  	  dbg_tx_text ("\nrsr hood switched to data") ;
	  	#endif
    
	  	cmd_lut_remove (RSR_CMD_INPUT_HOOD_OPEN       , rsr_hood_open ) ;
	  	cmd_lut_remove (RSR_CMD_INPUT_HOOD_CLOSE      , rsr_hood_close) ;
      cmd_lut_remove (INPUT_HOOD_STATUS_OPEN        , rsr_hood_detect) ;
      cmd_lut_remove (INPUT_HOOD_STATUS_CLOSE       , rsr_hood_detect) ;
	  }
}



/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_input_init.c 18996 2013-01-24 22:12:09Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "cmd.h"
#include "rf.h"
#include "prog.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//#define RSR_GET_INPUT_LEVEL(x)      !!(FIO2PIN & (1u << x))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean rsr_get_input_level(UInt8 input) ;                   
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void rsr_input_init (void)  
{
  static Boolean do_once = TRUE ;

  if (do_once && (prog_retrieve_option (FEAT_RF_OUTPUT) != NO_RF))
    {
	  do_once = FALSE;

      cmd_lut_insert (RSR_CMD_INPUT_VALET_ON     , rsr_valet_on     , EXEC_MODE_NORMAL) ; 
      cmd_lut_insert (RSR_CMD_INPUT_VALET_OFF    , rsr_valet_off    , EXEC_MODE_NORMAL) ;    	
      cmd_lut_insert (RSR_CMD_INPUT_HOOD_OPEN    , rsr_hood_open    , EXEC_MODE_NORMAL) ; 
      cmd_lut_insert (RSR_CMD_INPUT_HOOD_CLOSE   , rsr_hood_close   , EXEC_MODE_NORMAL) ; 

      if(!RSR_HOOD_POLARITY)  // don't do init on reverse polarity to allow disconnected input
        {
          rsr_get_input_level(RSR_HOOD_SEL) ? rsr_hood_close(0) : rsr_hood_open (0) ; // w2w init is done before rsr-init so HOOD may not be up to date...
	    }
            
      rsr_get_input_level(RSR_VALET_SEL)? rsr_valet_off(0)  : rsr_valet_on  (0) ; // w2w init is done before rsr-init so VALET may not be up to date...
      
    }
}
//--------------------------------------------------------------------------//
static Boolean rsr_get_input_level(UInt8 input)
{
  #if DBG_RSR == 1
	  TRACE ("\nRsr Get Input Level %d " , input) ;
  #endif  
  
  switch(input)
    {      
      #ifdef GWR_IN
        case 0 : return GWR_IN    ;  
      #endif
      
      #ifdef IGN_IN
        case 1 : return !IGN_IN    ;  
      #endif
      
      #ifdef POS_IN1
        case 2 : return !POS_IN1  ;   
      #endif
      
      #ifdef AUX3_IN
        case 3 : return AUX3_IN   ;   
      #endif
      
      #ifdef AUX2_IN
        case 4 : return AUX2_IN   ;   
      #endif
      
      #ifdef AUX1_IN
        case 5 : return AUX1_IN   ;   
      #endif
      #ifdef TRUNK_IN
        case 6 : return TRUNK_IN  ;    
      #endif
      
      #ifdef UNLOCK_IN
        case 7 : return UNLOCK_IN ;     
      #endif
      
      #ifdef LOCK_IN
        case 8 : return LOCK_IN   ;  
      #endif
        
      default: return NULL      ;
    }
}

/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_set_state.c 18996 2013-01-24 22:12:09Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rsr_dbg_print_state (Rsr_State state)  ;
static void rsr_dbg_print_err   (Rsr_Err_Code err_code) ;
 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void rsr_set_state(Rsr_State state)
{      
     
      rsr_vars.last_state = rsr_vars.state ;
      rsr_vars.state      = state          ;
    
  
  switch(state)
    {
      case RSR_STATE_STOP    :  rsr_set_callback(rsr_state_stop)     ; break ;
      case RSR_STATE_DISARM  :  rsr_set_callback(rsr_state_disarm)   ; break ;
      case RSR_STATE_GWR     :  rsr_set_callback(rsr_state_gwr_on)   ; break ;
      case RSR_STATE_ACC     :  rsr_set_callback(rsr_state_acc_on)   ; break ;
      case RSR_STATE_IGN     :  rsr_set_callback(rsr_state_ign_on)   ; break ;
      case RSR_STATE_START   :  rsr_set_callback(rsr_state_start_on) ; break ;
      case RSR_STATE_RUN     :  rsr_set_callback(rsr_state_run)      ; break ;
      case RSR_STATE_FAIL    :  rsr_set_callback(rsr_state_fail)     ; break ;
      case RSR_STATE_ARM     :  rsr_set_callback(rsr_state_arm)      ; break ;
    }
  
  rsr_timing_reset() ;

  #if DBG_RSR == 1
    rsr_dbg_print_state(state);

    if(state == RSR_STATE_FAIL)
      {
        rsr_dbg_print_err (rsr_vars.err_code) ;
      }
  #endif

}

//--------------------------------------------------------------------------//

#if DBG_RSR == 1
static void rsr_dbg_print_state(Rsr_State state)
{
  const char * dbg_state[] =
  {
    {"rsr_state_stop"} ,   
    {"rsr_state_disarm"} , 
    {"rsr_state_gwr_on"} , 
    {"rsr_state_acc_on"} , 
    {"rsr_state_ign_on"} ,
    {"rsr_state_start_on"},
    {"rsr_state_run"} ,    
    {"rsr_state_fail"},    
    {"rsr_state_arm"} ,
  } ;

  dbg_tx_crlf() ;
  dbg_tx_text(dbg_state[state]) ;
}
#endif

//--------------------------------------------------------------------------//

#if DBG_RSR == 1
static void rsr_dbg_print_err(Rsr_Err_Code err_code)
{
  const char * dbg_state[] =
  { 
    {""} ,   
	{"ERR_RUNTIME"} ,   
	{"ERR_OVER"} ,   
	{"ERR_NO_RPM"} ,   
	{"ERR_RKE_STOP"} ,   
	{"ERR_BRAKE"} ,   
	{"ERR_HOOD"} ,   
	{"ERR_MANUAL"} ,   
	{"ERR_TRANS"} ,   
	{"ERR_LOW_BAT"} ,   
	{"ERR_SS_TRIGG"} ,   
	{"ERR_W2"} ,       	
	{"ERR_DOOR_OPEN"} ,   		
	{"ERR_NO_ACC"} ,   
	{"ERR_NO_IGN"} ,   
	{"ERR_NO_START"} ,   
	{"ERR_NO_BRAKE"} ,   
	{"ERR_DTC"} ,      
  } ;
  dbg_tx_text(" ") ;
  dbg_tx_text(dbg_state[err_code]) ;
}
#endif


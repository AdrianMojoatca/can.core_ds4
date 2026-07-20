/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "prog_private.h"
#include "nvram.h"
#include "string.h"
#include "timeout_f.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean prog_init_cm800     (void) ;
static Boolean prog_init_dball     (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean prog_init(void)
{
  #if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800
    return prog_init_cm800() ;

  #else
    return prog_init_dball() ;
  #endif
}

//--------------------------------------------------------------------------//

static Boolean prog_init_cm800 (void)
{
  prog_vars.exit_func = NULL ;    
  
  if(!prog_init_nvfs())
    {
      #if DBG_PROG == 1

        dbg_tx_text("\r\nProg init nvfs fail");

      #endif

      return FALSE;
    }

  #if DBG_PROG == 1
  
    dbg_tx_text("\r\nProg init Cm800");
  
  #endif


  return TRUE;
}
//--------------------------------------------------------------------------//

static Boolean prog_init_dball (void)
{
  static const Cmd_Array2 cmd_array[] = 
    {        
      {INPUT_IGNITION       | INPUT_ON, prog_input_ign_on       , EXEC_MODE_NORMAL               } ,
      {INPUT_IGNITION                 , prog_input_ign_off      , EXEC_MODE_NORMAL               } ,
      {INPUT_PUSH           | INPUT_ON, prog_input_push_press   , EXEC_MODE_PROG                 } ,
      {INPUT_PUSH                     , prog_input_push_release , EXEC_MODE_PROG                 } ,
      {INPUT_LOCK1          | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_LOCK2          | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_LOCK3          | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_UNLOCK_ALL1    | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_UNLOCK_ALL2    | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_UNLOCK_ALL3    | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_UNLOCK_DRIVER1 | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_UNLOCK_DRIVER2 | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_UNLOCK_DRIVER3 | INPUT_ON, prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_OEM_LOCK                 , prog_input_change_option, EXEC_MODE_PROG                 } ,
      {INPUT_OEM_UNLOCK               , prog_input_change_option, EXEC_MODE_PROG                 } 
   };

  if(!prog_init_nvfs())
    {
        //error ?
    }

  pip_prog = os_pip_create (PROG_PIP_SIZE) ;

  prog_ign_timeout = timeout_f_create (8000, prog_input_ign_on) ;

  tsk_prog  = os_tsk_create (prog_task, TASK_PROG_PRI , TASK_PROG_STK, 0, TASK_PROG_NAME) ;   

  prog_vars.exit_func = NULL ;    

  os_tsk_start (tsk_prog, 0) ;    // Start task prog to begin debounce

  CMD_LUT_INSERT2(cmd_array); 


  #if DBG_PROG == 1
    dbg_tx_text("\r\nProg init DBALL");
  #endif

  return TRUE ;
}

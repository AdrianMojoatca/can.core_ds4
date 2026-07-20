/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_input_process.c 23628 2013-11-15 18:18:39Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "prog_private.h"
#include "display.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void prog_input_ign_on (void * arg)
{
    prog_vars.prog_enabled = FALSE ;

  #if DBG_PROG == 1
    dbg_tx_text("\r\nProg Ign On");
  #endif

}

//--------------------------------------------------------------------------//

void prog_input_ign_off (void * arg)
{
    prog_vars.prog_enabled = TRUE ;

    timeout_start (prog_ign_timeout) ;

  #if DBG_PROG == 1
    dbg_tx_text("\r\nProg Ign Off");
  #endif
}

//--------------------------------------------------------------------------//

void prog_input_long_push (void * arg)
{
    if (prog_vars.prog_enabled)
    {
      os_pip_insert (pip_prog, ENTER_LEAVE_EVENT, 0) ;

      timeout_stop  (prog_ign_timeout) ;

      #if DBG_PROG == 1
        dbg_tx_text("\r\nProg Long Push");
      #endif


    }
}

//--------------------------------------------------------------------------//

void prog_input_push_press (void * arg)
{
    if (prog_vars.prog_enabled && prog_vars.prog_entered)
    {
      os_pip_insert (pip_prog, PREPARE_CHANGE_FEATURE_EVENT, 0) ;

      #if DBG_PROG == 1
        dbg_tx_text("\r\nProg Push Press");
      #endif

    }
}

//--------------------------------------------------------------------------//

void prog_input_push_release (void * arg)
{
    if (prog_vars.prog_enabled && prog_vars.prog_entered)
    {
      os_pip_insert (pip_prog, CHANGE_FEATURE_EVENT, 0) ;

      #if DBG_PROG == 1
        dbg_tx_text("\r\nProg Push Release");
      #endif

    }
}

//--------------------------------------------------------------------------//

void prog_input_change_option (void * arg)
{
    if (prog_vars.prog_enabled && prog_vars.prog_entered)
    {
      os_pip_insert (pip_prog, CHANGE_OPTION_EVENT, 0) ;


      #if DBG_PROG == 1
        dbg_tx_text("\r\nProg Change Option");
      #endif

    }
}

void prog_input_change_custom1option1(void * arg)
{
    if (prog_vars.prog_enabled && prog_vars.prog_entered)
    {
	  prog_set_option(RUNTIME1, FEAT_RUNTIME) ; 

	  display_prog (prog_vars.feature_current, feature_table[prog_vars.feature_current - 1].option_current, FALSE) ;
      #if DBG_PROG == 1
        dbg_tx_text("\r\nProg Change Runtime option1");
      #endif
      

    }
}

void prog_input_change_custom1option2 (void * arg)
{
    if (prog_vars.prog_enabled && prog_vars.prog_entered)
    {
	  prog_set_option(RUNTIME2, FEAT_RUNTIME) ; 

	  display_prog (prog_vars.feature_current, feature_table[prog_vars.feature_current - 1].option_current, FALSE) ;

      #if DBG_PROG == 1
        dbg_tx_text("\r\nProg Change Runtime option2");
      #endif

    }
}

void prog_input_change_custom1option3 (void * arg)
{
    if (prog_vars.prog_enabled && prog_vars.prog_entered)
    {
	  prog_set_option(RUNTIME3, FEAT_RUNTIME) ; 

	  display_prog (prog_vars.feature_current, feature_table[prog_vars.feature_current - 1].option_current, FALSE) ;

      #if DBG_PROG == 1
        dbg_tx_text("\r\nProg Change Runtime option3");
      #endif

    }
}

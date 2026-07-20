/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id$
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "prog_private.h"
#include "nvram_usr.h"
#include "display.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define PROG_PIP_TIMEOUT         5  
#define PROG_TASK_TIMEOUT       (30000/ PROG_PIP_TIMEOUT)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void prog_vars_init          (void) ;
void prog_enter              (void) ;
void prog_change_feature     (void) ;
void prog_change_option      (void) ;
void prog_leave              (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

const TskTimeout time = PROG_TASK_TIMEOUT ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void prog_task (void * tsk_arg)
{   
    for(;;)
    {
        switch (os_pip_remove (pip_prog, PROG_PIP_TIMEOUT))
        {
            case ENTER_LEAVE_EVENT:
                if (prog_vars.prog_entered)
                {
                    prog_leave () ;
                }
                else
                {
                    prog_enter () ;
                }
                break;

            case CHANGE_FEATURE_EVENT:
                prog_change_feature () ;
                prog_vars.timeout = time ;
                break;

            case CHANGE_OPTION_EVENT:
                prog_change_option () ;
                prog_vars.timeout = time ;
                break;

            case PREPARE_CHANGE_FEATURE_EVENT:
                prog_vars.do_change_feature = TRUE ;
                break;

            default:
                if (prog_vars.prog_entered)
                {
                    if (-- prog_vars.timeout == 0)
                        prog_leave () ;
                }
                break;
        }
    }
}

/*==========================================================================*/
void prog_vars_init (void)
{
    prog_vars.timeout           = time  ;
    prog_vars.feature_current   = 1     ;
    prog_vars.do_change_feature = FALSE ;
}

/*==========================================================================*/
void prog_enter (void)
{
    wake_enter () ;

    cmd_lut_clr_mode((Exec_Mode) (EXEC_MODE_NORMAL | EXEC_MODE_PROG)) ; 

    cmd_lut_set_mode( EXEC_MODE_PROG ); 

    display_once (BOTH_LED, 1000) ;

    prog_feat_table_read () ;
    
    prog_vars.prog_entered = TRUE ;

    prog_vars_init () ; 


    #if DBG_PROG == 1
      dbg_tx_text("\r\nProg Enter");
    #endif
}

/*==========================================================================*/
void prog_leave (void)
{
    prog_vars.prog_entered = FALSE ;
    prog_vars.prog_enabled = FALSE ;

    display_once (BOTH_LED, 2000) ;

    prog_feat_table_write () ;

    nvram_usr_commit () ;   // need to finish nvram_usr_commit before enabling back functions that can send CAN msg

    cmd_lut_clr_mode((Exec_Mode) (EXEC_MODE_NORMAL | EXEC_MODE_PROG)) ; 

    cmd_lut_set_mode( EXEC_MODE_NORMAL );   

    if(prog_vars.exit_func != NULL)
      {
        prog_vars.exit_func() ;
      }
    
    os_tsk_wait (2000) ;    // patch to avoid cpu to sleep before leds are off

    wake_leave () ;


    #if DBG_PROG == 1
      dbg_tx_text("\r\nProg Leave");
    #endif

}

/*==========================================================================*/
void prog_change_feature (void)
{
  if (prog_vars.do_change_feature)
    {       
		prog_vars.do_change_feature = FALSE ;
		
  		do
		{
			if ((++prog_vars.feature_current) > FEATURE_NUM)
			{
            	prog_vars.feature_current = 1 ;
        	}
		}
        while(feature_table[prog_vars.feature_current-1].user_changeble == 0) ;	// display only features that can be changed manually  
    }

  display_prog (prog_vars.feature_current, feature_table[prog_vars.feature_current - 1].option_current, TRUE) ;

  #if DBG_PROG == 1
    dbg_tx_text("\r\nProg Change Feat");
  #endif

}

/*==========================================================================*/
void prog_change_option (void)
{
  #define TABLE_ENTRY     feature_table[prog_vars.feature_current - 1]

  if ((++ TABLE_ENTRY.option_current) > TABLE_ENTRY.option_number)
    {
        TABLE_ENTRY.option_current = 1 ; 
    }

  display_prog (prog_vars.feature_current, feature_table[prog_vars.feature_current - 1].option_current, FALSE) ;

  #undef TABLE_ENTRY 

  #if DBG_PROG == 1
    dbg_tx_text("\r\nProg Change Opt");
  #endif

                 
}


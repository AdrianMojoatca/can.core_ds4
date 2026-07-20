/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: evt_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "evt_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void evt_ign_on_delayed ( void );
static void evt_ign_off_delayed( void );
static void evt_do_update_ign  ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Timeout evt_timeout_ign ; 
Boolean evt_ign_status  ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void evt_init( void )
{
    static Boolean do_evt_init = TRUE ;
    
    if( do_evt_init )
    {
        evt_timeout_ign  = timeout_f_create ( EVT_TIMEOUT_UPDATE_IGN , (Exec_Func)evt_do_update_ign  ) ;        
        
        cmd_register:
        {
            static const Cmd_Array1 cmd_array[] = 
            {
                { INPUT_IGN_STATUS_ON  , (Exec_Func) evt_ign_on_delayed  },
                { INPUT_IGN_STATUS_OFF , (Exec_Func) evt_ign_off_delayed }
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
        }


        do_evt_init = FALSE ; 
    }
}

/*--------------------------------------------------------------------------*/

void evt_ign_on_delayed( void )
{
    timeout_stop ( evt_timeout_ign );

    ATOMIC( evt_ign_status = TRUE );

    timeout_start( evt_timeout_ign );
}

/*--------------------------------------------------------------------------*/

void evt_ign_off_delayed( void )
{
    timeout_stop ( evt_timeout_ign );

    ATOMIC( evt_ign_status = FALSE );

    timeout_start( evt_timeout_ign );
}

/*--------------------------------------------------------------------------*/

void evt_do_update_ign( void )
{

}

/*--------------------------------------------------------------------------*/

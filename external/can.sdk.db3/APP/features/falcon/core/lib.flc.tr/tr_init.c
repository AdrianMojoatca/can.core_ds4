/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: out_create.c 1046 2010-04-08 12:51:11Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "tr_private.h"
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

//--------------------------------------------------------------------------//

void tr_init( void )
{
   tr_oem         = FALSE;
   tr_lock_status = FALSE;
   tr_status      = FALSE;

   cmd_register:
   {
       static const Cmd_Array1 cmd_array[] =
       {
            {INPUT_OEM_TRUNK                 , (Exec_Func)tr_trigger_oem },
            {INPUT_TRUNK         | INPUT_ON  , (Exec_Func)tr_trigger_rs  },
    	    {INPUT_OEM_LOCK                  , (Exec_Func)tr_reset_lock  },
    	    {INPUT_OEM_UNLOCK                , (Exec_Func)tr_reset_unlock},
            
            {INPUT_LOCK1         | INPUT_ON  , (Exec_Func)tr_reset_lock  },
            {INPUT_UNLOCK_DRIVER1| INPUT_ON  , (Exec_Func)tr_reset_unlock},
    	    {INPUT_UNLOCK_ALL1   | INPUT_ON  , (Exec_Func)tr_reset_unlock},
            
            {INPUT_LOCK2         | INPUT_ON  , (Exec_Func)tr_reset_lock  },
            {INPUT_UNLOCK_DRIVER2| INPUT_ON  , (Exec_Func)tr_reset_unlock},
    	    {INPUT_UNLOCK_ALL2   | INPUT_ON  , (Exec_Func)tr_reset_unlock},
            
            {INPUT_LOCK3         | INPUT_ON  , (Exec_Func)tr_reset_lock  },
            {INPUT_UNLOCK_DRIVER3| INPUT_ON  , (Exec_Func)tr_reset_unlock},
    	    {INPUT_UNLOCK_ALL3   | INPUT_ON  , (Exec_Func)tr_reset_unlock},
            
            {INPUT_ALL_DOOR_STATUS_OPEN      , (Exec_Func)tr_reset_doors },              
            
            {INPUT_TRUNK_STATUS_CLOSE        , (Exec_Func)tr_timeout     },
                
            {INPUT_TRIGGER_OFF               , (Exec_Func)tr_reset       },
            {INPUT_PANIC                     , (Exec_Func)tr_reset       }
       };

       CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
   }
}

//--------------------------------------------------------------------------//


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: lf_init.c 24423 2014-01-27 17:57:47Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "lf_private.h"
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

void lf_init( void  )
{
    Tsk tsk ; 

    static Boolean do_one_time = TRUE  ;

    if( do_one_time )
    {

        lf_pip = os_pip_create( LF_PIP_MAX_SIZE ); 
    
        cmd_register:
        {        
            static const Cmd_Array1 cmd_array[] = 
            {
                {INPUT_PARKING_ON                , (Exec_Func)lf_on    },
                {INPUT_PARKING_OFF               , (Exec_Func)lf_off   },    
    		    {INPUT_LOCK1         | INPUT_ON  , (Exec_Func)lf_flush },
                {INPUT_UNLOCK_ALL1   | INPUT_ON  , (Exec_Func)lf_flush },
                {INPUT_UNLOCK_DRIVER1| INPUT_ON  , (Exec_Func)lf_flush },		
    		    {INPUT_LOCK2         | INPUT_ON  , (Exec_Func)lf_flush },
                {INPUT_UNLOCK_ALL2   | INPUT_ON  , (Exec_Func)lf_flush },
                {INPUT_UNLOCK_DRIVER2| INPUT_ON  , (Exec_Func)lf_flush },        
    		    {INPUT_LOCK3         | INPUT_ON  , (Exec_Func)lf_flush },
                {INPUT_UNLOCK_ALL3   | INPUT_ON  , (Exec_Func)lf_flush },
                {INPUT_UNLOCK_DRIVER3| INPUT_ON  , (Exec_Func)lf_flush },
                {INPUT_GWR_OFF                   , (Exec_Func)lf_flush },
                {INPUT_TRIGGER_OFF               , (Exec_Func)lf_flush },
                {INPUT_PANIC                     , (Exec_Func)lf_flush }
            };

            CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
        }
    
        tsk = os_tsk_create( lf_tsk , LF_TSK_PRI  , LF_TSK_STK  , 0 , LF_TSK_NAME) ;        
        
        os_tsk_start( tsk , NULL );            

        do_one_time = FALSE ; 
    }
}		

//--------------------------------------------------------------------------//

void lf_autopulse_enable(Boolean enable)
{
    lf_do_autopulse = enable;
}

//--------------------------------------------------------------------------//


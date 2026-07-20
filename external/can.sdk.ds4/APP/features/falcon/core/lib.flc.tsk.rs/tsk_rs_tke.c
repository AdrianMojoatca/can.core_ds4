/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tsk_rs_tke.c 26902 2014-08-27 16:09:06Z daniel.erhan $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "tsk_rs_private.h"
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

/*--------------------------------------------------------------------------*/

void tsk_rs_tke_init( void )
{
    TRACE("TSK_RS_TKE_INIT: Initializing takeover system");
    
    cmd_register:
    {
        static const Cmd_Array1 cmd_array[] =
        {
            {INPUT_OEM_LOCK                  , (Exec_Func)tsk_rs_tke_not_allowed         },
            {INPUT_LOCK1          | INPUT_ON , (Exec_Func)tsk_rs_tke_not_allowed         },        
            {INPUT_LOCK2          | INPUT_ON , (Exec_Func)tsk_rs_tke_not_allowed         },        
            {INPUT_LOCK3          | INPUT_ON , (Exec_Func)tsk_rs_tke_not_allowed         },                        
            {INPUT_UNLOCK_DRIVER1 | INPUT_ON , (Exec_Func)tsk_rs_tke_allowed_aftermarket },
            {INPUT_UNLOCK_ALL1    | INPUT_ON , (Exec_Func)tsk_rs_tke_allowed_aftermarket },
            {INPUT_UNLOCK_DRIVER2 | INPUT_ON , (Exec_Func)tsk_rs_tke_allowed_aftermarket },
            {INPUT_UNLOCK_ALL2    | INPUT_ON , (Exec_Func)tsk_rs_tke_allowed_aftermarket },
            {INPUT_UNLOCK_DRIVER3 | INPUT_ON , (Exec_Func)tsk_rs_tke_allowed_aftermarket },
            {INPUT_UNLOCK_ALL3    | INPUT_ON , (Exec_Func)tsk_rs_tke_allowed_aftermarket },            
            {INPUT_OEM_UNLOCK                , (Exec_Func)tsk_rs_tke_allowed_oem         },             
            {INPUT_ALL_DOOR_STATUS_OPEN      , (Exec_Func)tsk_rs_tke_door_shutdown       },              
            {INPUT_TRUNK          | INPUT_ON , (Exec_Func)tsk_rs_tke_allowed_aftermarket },                
        };

        TRACE("TSK_RS_TKE_INIT: Registering INPUT_ALL_DOOR_STATUS_OPEN handler");
        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
    	        
    tsk_rs_tke_shutdown_req = TRUE ;
    
    TRACE("TSK_RS_TKE_INIT: Initialization complete, shutdown_req=TRUE");
}

/*--------------------------------------------------------------------------*/

void tsk_rs_tke_allowed_aftermarket( void )
{
    if(tsk_rs_do_oem_disarm && !get_rs_gwr() && !get_ign_status() && tsk_rs_lock_status)
    {
        UInt8 retry_count = 15 ; 
        
        cmd_q_insert( INPUT_GWR_ON           , SENSE_RS_CAN_TIMEOUT_CMD_Q);		
        cmd_q_insert( INPUT_RS_IGNITION_ON   , SENSE_RS_CAN_TIMEOUT_CMD_Q);
        
        while( retry_count )
        {
            retry_count --;
            
            os_tsk_wait(100);
            
            if(get_ign_status())
            {
                retry_count = 0 ;
            }            
        }
        
        os_tsk_wait(100);
        
        cmd_q_insert( INPUT_RS_IGNITION_OFF  , SENSE_RS_CAN_TIMEOUT_CMD_Q);               
        cmd_q_insert( INPUT_GWR_OFF          , SENSE_RS_CAN_TIMEOUT_CMD_Q);        
    }
    
    ATOMIC(tsk_rs_lock_status=FALSE);
        
    if(TSK_RS_TAKEOVER_ALLOWED_BY_AFTERMARKET())
    {
        tsk_rs_tke_allowed();        
    }       
}



/*--------------------------------------------------------------------------*/

void tsk_rs_tke_allowed_oem( void )
{
    ATOMIC(tsk_rs_lock_status=FALSE);
    
    if(TSK_RS_TAKEOVER_ALLOWED_BY_OEM())
    {
        tsk_rs_tke_allowed();        
    }    
}

/*--------------------------------------------------------------------------*/

void tsk_rs_tke_allowed( void )
{
    ATOMIC(tsk_rs_tke_shutdown_req = FALSE) ;
    
    timeout_q_remove( (void *)tsk_rs_tke_timeout );
        
    timeout_q_loop  ( (Timeout_Q_Func)tsk_rs_tke_timeout , NULL , tsk_rs_takeover_timeout / 10 , 1 ); 
        
    TRACE_TRS("\n\r[TSK_RS_TKE_ALLOWED]\n\r");       
}

/*--------------------------------------------------------------------------*/

void tsk_rs_tke_not_allowed( void )
{
    ATOMIC(tsk_rs_lock_status=TRUE);
        
    timeout_q_remove( (void *)tsk_rs_tke_timeout );
    
    ATOMIC(tsk_rs_tke_shutdown_req = TRUE) ;

    TRACE_TRS("\n\r[TSK_RS_TKE_NOT_ALLOWED]\n\r");
    
}
        
/*--------------------------------------------------------------------------*/        

Timeout_Q_Status tsk_rs_tke_timeout( void )
{
    if(TSK_RS_TAKEOVER_MANAGER_ENABLED())
    {
        tsk_rs_tke_not_allowed() ;
    }

    TRACE_TRS("\n\r[TSK_RS_TKE_TIMEOUT]\n\r");
   
    return TIMEOUT_Q_COMPLETE ; 
}

/*--------------------------------------------------------------------------*/ 

void tsk_rs_tke_door_shutdown( void )
{
    TRACE("TKE_DOOR_SHUTDOWN: FUNCTION CALLED");
    TRACE("TKE_DOOR_SHUTDOWN: CONDITIONS: ENABLED=%d, shutdown_req=%d, idle=%d, gwr=%d", 
          TSK_RS_TAKEOVER_MANAGER_ENABLED(), tsk_rs_tke_shutdown_req, tsk_rs_idle, get_rs_gwr());
    
    if( TSK_RS_TAKEOVER_MANAGER_ENABLED() && tsk_rs_tke_shutdown_req && !tsk_rs_idle && get_rs_gwr() )
    {          	        
        TRACE_TRS("\n\r[TSK_RS_TKE_DO_DOOR_SHUTDOWN]\n\r");
        
        timeout_q_remove( (void *) tsk_rs_tke_brake_set );

        timeout_q_loop  ( (Timeout_Q_Func)tsk_rs_tke_brake_set  , NULL  , TSK_RS_TKE_BRAKE_SET_TIMEOUT   , 1 ) ;

        timeout_q_remove( (void *) tsk_rs_tke_brake_reset );
																					
        timeout_q_loop  ( (Timeout_Q_Func)tsk_rs_tke_brake_reset , NULL , TSK_RS_TKE_BRAKE_RESET_TIMEOUT , 1 ) ;
    }
}

/*--------------------------------------------------------------------------*/

Timeout_Q_Status tsk_rs_tke_brake_reset( void )
{
    TRACE_TRS("\n\r[TSK_RS_TKE_BRAKE_RESET]\n\r");
	
	tsk_rs_brake_off();
    
    return TIMEOUT_Q_COMPLETE ; 
}        

/*--------------------------------------------------------------------------*/

Timeout_Q_Status tsk_rs_tke_brake_set( void )
{
    TRACE_TRS("\n\r[TSK_RS_TKE_BRAKE_SET]\n\r");
	
	tsk_rs_brake_on();
    
    return TIMEOUT_Q_COMPLETE ; 
}        

/*--------------------------------------------------------------------------*/


       


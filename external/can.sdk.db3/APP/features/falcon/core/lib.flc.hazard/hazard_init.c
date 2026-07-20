/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hazard_init.c 19197 2013-02-12 10:50:11Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "hazard_private.h"
#include "per_exec.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void  hazard_light_on  ( void );
void  hazard_light_off ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean hazard_was_on = FALSE ; 

Can_Msg  frd_test_lights_light_on 	=	{0x7FF, 0, 0, 8, 0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
Can_Msg  frd_test_lights_light_off 	=	{0x7FF, 0, 0, 8, 0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void hazard_init( Hazard_Pulse_Func pulse_func , Hazard_Cfg cfg )
{
    Tsk tsk ;

	TRACE("\r\nHAZARD INIT\r\n");
    
    if( hazard_was_instantiated == FALSE )
    {
        ATOMIC(hazard_was_instantiated = TRUE);
    } 
    
    hazard_enable = TRUE ; 

    hazard_cfg    = cfg  ;

    lf_init();
    
    hazard_pulse_func = pulse_func ; 

    hazard_evt = os_evt_create();
    
    //tsk = os_tsk_create ( hazard_tsk , HAZARD_TSK_PRI , HAZARD_TSK_STK , 0 , HAZARD_TSK_NAME ) ;
	tsk = os_tsk_create ( hazard_tsk , /*HAZARD_TSK_PRI*/50, HAZARD_TSK_STK , 0 , HAZARD_TSK_NAME ) ;

    cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {            
            {LF_PARKING_LIGHT_ON             , (Exec_Func)hazard_light_on       }, 
            {LF_PARKING_LIGHT_OFF            , (Exec_Func)hazard_light_off      }, 
            {INPUT_LOCK1          | INPUT_ON , (Exec_Func)hazard_disable_lock   },
            {INPUT_UNLOCK_DRIVER1 | INPUT_ON , (Exec_Func)hazard_disable_unlock },
            {INPUT_UNLOCK_ALL1    | INPUT_ON , (Exec_Func)hazard_disable_unlock },        
            {INPUT_LOCK2          | INPUT_ON , (Exec_Func)hazard_disable_lock   },
            {INPUT_UNLOCK_DRIVER2 | INPUT_ON , (Exec_Func)hazard_disable_unlock },
            {INPUT_UNLOCK_ALL2    | INPUT_ON , (Exec_Func)hazard_disable_unlock },        
        	{INPUT_LOCK2          | INPUT_ON , (Exec_Func)hazard_disable_lock   },
            {INPUT_UNLOCK_DRIVER2 | INPUT_ON , (Exec_Func)hazard_disable_unlock },
            {INPUT_UNLOCK_ALL2    | INPUT_ON , (Exec_Func)hazard_disable_unlock },
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
    }
	
	os_tsk_start( tsk , 0 ) ;
        
}

//--------------------------------------------------------------------------//

void hazard_light_on( void )
{
	if( hazard_enable )
    {
		//can_usr_tx (CAN_DEV_1 , &frd_test_lights_light_on   , FALSE, 100) ; 
		hazard_request_on() ;
    }
        
    ATOMIC(hazard_was_on = hazard_enable);  
}

//--------------------------------------------------------------------------//


void hazard_light_off( void )
{
	
	if( hazard_was_on )
    {
		//can_usr_tx (CAN_DEV_1 , &frd_test_lights_light_off   , FALSE, 100) ;
		hazard_request_off() ; 
    }

    ATOMIC(hazard_was_on = FALSE) ;
}

//--------------------------------------------------------------------------//


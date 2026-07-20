/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hazard_tsk.c 19197 2013-02-12 10:50:11Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "hazard_private.h"
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

void hazard_tsk( void )
{
    UInt16 evt ; 

    for( ;  ; )
    {
		hazard_wait_event :
        {        
            evt = os_evt_wait( hazard_evt , HAZARD_EVT_STATUS_ON | HAZARD_EVT_REQ_ON , HAZARD_EVT_REQ_ON | HAZARD_EVT_REQ_ON , 0 ) ; 
        }
        wake_enter() ; 
  
        hazard_is_active :
        {
            if( evt & HAZARD_EVT_STATUS_ON )
            {
                check_for_on:  

                evt = os_evt_wait( hazard_evt , HAZARD_EVT_STATUS_ON , 0 , 1000 ) ;             

                if( !evt )
                {                    
                    wake_leave() ; 

                    goto hazard_wait_event ;
                }
                else
                {
                    goto check_for_on ; 
                }                     
            }
        }

        wait_status_on :
        {
           (hazard_pulse_func)();
			
			evt = os_evt_wait( hazard_evt , HAZARD_EVT_STATUS_ON , HAZARD_EVT_STATUS_ON , 1000 ) ; 

            if( !evt )
            {
                 TRACE("\n\r[HAZARD_STATUS_ON_ERROR]\n\r");                                 
            }
            else
            {
                 TRACE("\n\r[HAZARD_STATUS_ON]\n\r");
            }
        }

        wait_status_off : 
        {
			evt = os_evt_wait( hazard_evt , HAZARD_EVT_STATUS_OFF , HAZARD_EVT_STATUS_OFF , 1000 ) ; 
                         
            if( evt )
            {
				(hazard_pulse_func)();

                TRACE("\n\r[HAZARD_STATUS_OFF]\n\r"); 
            }
            else
            {
                TRACE("\n\r[HAZARD_STATUS_OFF_ERROR]\n\r") ;                                                                 
            }
        }
        
        
        wait_req_on:
        {
            evt = os_evt_wait( hazard_evt , HAZARD_EVT_REQ_ON , 0 , 5000 ) ;             

            if( !evt )
            {
                TRACE("\n\r[HAZARD_REQ_ON_TIMEOUT]\n\r");                                                                       

                check_error :
                {
                    evt = os_evt_wait( hazard_evt , HAZARD_EVT_STATUS_ON , HAZARD_EVT_STATUS_ON , 3000 ) ;             

                    if( evt ) 
                    {
                        TRACE("\n\r[HAZARD_STATUS_OFF_ERROR - END]\n\r");                                                                       
                        
                        (hazard_pulse_func)();

                        goto check_error ;
                    }
                }
            }
            else
            {
                goto wait_status_on ; 
            }
        }                        

        wake_leave();
    }
}

//--------------------------------------------------------------------------//


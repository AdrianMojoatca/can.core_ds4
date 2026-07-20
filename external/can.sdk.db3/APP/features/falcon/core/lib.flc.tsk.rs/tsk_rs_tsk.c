/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tsk_rs_tsk.c 27636 2014-09-30 17:41:00Z daniel.erhan $
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

//--------------------------------------------------------------------------//

void tsk_rs_tsk( void )
{    
    #define DO_SHUTDOWN()       (cause[0]||cause[1]||cause[2]||cause[3]||cause[4])
	#define DO_SHUTDOWN_SKD()   (!get_skd_present() || !get_brake_status() || get_ss_lock())
    #define SHUTDOWN_RESET()    (cause[0]=cause[1]=cause[2]=cause[3]=cause[4]=0)
    #define SKD_SHUTDOWN()      ((tsk_rs_gwr==TRUE)&&(cause[0]==FALSE)&&(cause[1]==FALSE)&&(cause[2]==FALSE)&&(cause[3]==FALSE)&&(cause[4]==FALSE))
    
    Reg_Id evt ; 

    static Boolean cause[5];
    static Boolean do_one_time = TRUE;
   
    while( TRUE )
    {
		evt = os_pip_remove( tsk_rs_pip , 0 ) ; 

        wake_enter();
		{
			if( evt == TSK_RS_IGN_OFF )
            {                                                                
                cause[0] = tsk_rs_shutdown_req     ; // EIPS SHUTDOWN
                cause[1] = tsk_rs_tke_shutdown_req ; // RUN SAFE error (takeover failed)
                cause[2] = (tsk_rs_takeover_cfg&TSK_RS_TAKEOVER_SKD) ? !get_skd_present() : FALSE ; // KEY MISSING
                cause[3] = get_ss_lock     ()      ; // SECURITY SHUTDOWN, "DOOR BROKEN, CAR STOLEN"
                cause[4] = (!tsk_rs_tke_shutdown_req && !get_brake_status()) ;// RS SHUTDOWN
                
                if( SKD_SHUTDOWN() )
                {
                	set_door_skip();
                	
                	reg_execute( tsk_rs_reg , (Reg_Id)TSK_RS_DOOR_OPEN,NULL); //open the door
                	
                	os_tsk_wait(500);
                	
                	reg_execute( tsk_rs_reg , (Reg_Id)TSK_RS_DOOR_CLOSE,NULL); //close the door
                	
                	os_tsk_wait(1000);
                	
                	clr_door_skip();
                	
                	cause[2] = (tsk_rs_takeover_cfg&TSK_RS_TAKEOVER_SKD) ? !get_skd_present() : FALSE ;
                }                		
            }
            
            if(TSK_RS_TAKEOVER_MANAGER_DISABLED())
            {
                reg_execute( tsk_rs_reg , (Reg_Id)evt , (void *)(get_ss_lock() || !get_brake_status()) );
            }
            else
            {
				if(tsk_rs_takeover_cfg&TSK_RS_TAKEOVER_SKD)
				{
					reg_execute( tsk_rs_reg , (Reg_Id)evt , (void *)DO_SHUTDOWN_SKD() );
				}
				else
				{
					reg_execute( tsk_rs_reg , (Reg_Id)evt , (void *)DO_SHUTDOWN() );
				}
            }
            
            if( evt == TSK_RS_GWR_OFF )
            {
					 if( cause[0] ){led_usr_op_err( LED_USR_ERR_COUNT,   1) ; TRACE_CAN_TSK_RS( "\r\nER_EIP" );} // EIPS SHUTDOWN
                else if( cause[1] ){led_usr_op_err( SHUTDOWN_RUNSAFE,    1) ; TRACE_CAN_TSK_RS( "\r\nER_TKF" );} // RUN SAFE error (takeover failed)
                else if( cause[2] ){led_usr_op_err( SHUTDOWN_NO_KEY,     1) ; TRACE_CAN_TSK_RS( "\r\nER_KEY" );} // KEY MISSING
                else if( cause[3] ){led_usr_op_err( LED_USR_CUSTOM_ERR5, 1) ; TRACE_CAN_TSK_RS( "\r\nER_SS" );} // SECURITY SHUTDOWN, "DOOR BROKEN, CAR STOLEN"
                else if( cause[4] ){led_usr_op_err( LED_USR_CUSTOM_ERR2, 1) ; TRACE_CAN_TSK_RS( "\r\nER_RS" );} // RS SHUTDOWN

                if(DO_SHUTDOWN())
                {
                	set_door_skip();
                	
                	reg_execute( tsk_rs_reg , (Reg_Id)TSK_RS_DOOR_OPEN,NULL); //open the door
                	
                	os_tsk_wait(500);
                	
                	reg_execute( tsk_rs_reg , (Reg_Id)TSK_RS_DOOR_CLOSE,NULL); //close the door
                	
                	os_tsk_wait(1000);
                	
                	clr_door_skip();
                }	
                
                
                SHUTDOWN_RESET();
            }
        
		
        
            if( tsk_rs_invalid_state() == TRUE )
            {
                tsk_rs_brake_on();
                    
                os_tsk_wait( 1000 );
                    
                tsk_rs_brake_off();
                
                os_tsk_wait( 1000 );
                
                if( do_one_time )
                {
                    do_one_time = FALSE;
                    
                    os_tsk_wait( 3000 );
                    
                    TRACE_CAN_TSK_RS( "\r\nER_SPD" );
                    led_usr_op_err( SHUTDOWN_SPEED_DETECT, 1) ;
                }

            }
            else
            {
                do_one_time = TRUE;
            }
        }
        wake_leave();
    }    
    
    #undef DO_SHUTDOWN
    #undef SHUTDOWN_RESET
}


//--------------------------------------------------------------------------//

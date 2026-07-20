/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: lf_tsk.c 24423 2014-01-27 17:57:47Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "lf_private.h"
#include "trace.h"
#include "wake.h"
#include "status_system.h"
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

void lf_tsk( void )
{
    SInt16  cmd ;      
    
    for( ; ; )
	{		
        cmd = os_pip_remove( lf_pip  , 0 ) ;

        wake_enter();
        {                
            TRACE("[LF_TSK : wake_enter]\n\r");
            
            lf_handle_command :
            {            
                switch( cmd )
                {
                    case LF_CMD_RESET :
                    {
                        TRACE("[LF_LIGHT_OFF - reset]\n\r");
                        
                        //cmd_q_rs_insert( LF_PARKING_LIGHT_OFF , LF_CMD_INSERT_TIMEOUT ) ;                         
						cmd_q_insert( LF_PARKING_LIGHT_OFF , LF_CMD_INSERT_TIMEOUT ) ; // 15_11_2022                         
                    }
                    break ; 
                    
                    case LF_CMD_ON  :
                    {
                        os_tsk_wait( /*LF_WIDTH_TIMEOUT*/500 );
                        
                        TRACE("[LF_LIGHT_ON]\n\r");
                        
                        //cmd_q_rs_insert( LF_PARKING_LIGHT_ON  , LF_CMD_INSERT_TIMEOUT  ) ; 
						cmd_q_insert( LF_PARKING_LIGHT_ON  , LF_CMD_INSERT_TIMEOUT  ) ; // 15_11_2022
                            
                        cmd = os_pip_remove( lf_pip , LF_PULSED_MODE_TIMEOUT ) ;

                        switch( cmd )
                        {
                            case LF_CMD_TIMEOUT :
                            {
                                if( get_rs_gwr() && lf_do_autopulse )
                                {
                                    TRACE("[LF_AUTO_PULSED_MODE]\n\r");
                                
                                    cmd_q_rs_insert( LF_PARKING_LIGHT_OFF  , LF_CMD_INSERT_TIMEOUT ) ;                     
                                
                                    cmd = LF_CMD_ON ;   
                                }
                                else
                                {
                                    cmd = LF_CMD_OFF ; 
                                }
                            }
                            goto lf_handle_command ;

                            case LF_CMD_ON    :
                            case LF_CMD_OFF   :
                            case LF_CMD_RESET :goto lf_handle_command ;
                            
                            default : break ;
                        }
                    }
                    break;
        
                    case LF_CMD_OFF  :
                    {
                        os_tsk_wait( /*LF_WIDTH_TIMEOUT*/300);
                        
                        TRACE("[LF_LIGHT_OFF]\n\r");
                        
                        //cmd_q_rs_insert( LF_PARKING_LIGHT_OFF  , LF_CMD_INSERT_TIMEOUT ) ;                     
						cmd_q_insert( LF_PARKING_LIGHT_OFF  , LF_CMD_INSERT_TIMEOUT ) ;   // 15_11_2022                  
                    }
                    break;
                }        
            }
        }
        TRACE("[LF_TSK : wake_leave]\n\r");
        wake_leave();
	}				
	
}		

//--------------------------------------------------------------------------//


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tsk_rs_cmd.c 26653 2014-08-14 12:06:05Z florin.olariu $
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

void tsk_rs_cmd_gwr_on( void )
{
    if( tsk_rs_gwr == FALSE )
    {
        tsk_rs_invalid_state_reset();
        
        os_pip_insert( tsk_rs_pip , TSK_RS_GWR_ON , TSK_RS_PIP_INSERT_TIMEOUT );
        
        ATOMIC( tsk_rs_gwr = TRUE );
    }
}

/*--------------------------------------------------------------------------*/

void tsk_rs_cmd_gwr_off( void )
{
    if( tsk_rs_gwr == TRUE)
    {
        tsk_rs_cmd_str_off();
        tsk_rs_cmd_ign_off();
                
        os_pip_insert( tsk_rs_pip , TSK_RS_GWR_OFF , TSK_RS_PIP_INSERT_TIMEOUT );
        
        ATOMIC(tsk_rs_gwr = FALSE);
    }
}

/*--------------------------------------------------------------------------*/

void tsk_rs_cmd_acc_on( void )
{
    os_pip_insert( tsk_rs_pip , TSK_RS_ACC_ON , TSK_RS_PIP_INSERT_TIMEOUT );
}

/*--------------------------------------------------------------------------*/

void tsk_rs_cmd_acc_off( void )
{
    os_pip_insert( tsk_rs_pip , TSK_RS_ACC_OFF , TSK_RS_PIP_INSERT_TIMEOUT );
}

/*--------------------------------------------------------------------------*/

void tsk_rs_cmd_ign_on( void )
{
    if((tsk_rs_ign == FALSE) && (tsk_rs_gwr==TRUE))
    {        
        os_pip_insert( tsk_rs_pip , TSK_RS_IGN_ON , TSK_RS_PIP_INSERT_TIMEOUT );
        
        ATOMIC(tsk_rs_ign=TRUE);
    }
}

/*--------------------------------------------------------------------------*/
    
void tsk_rs_cmd_ign_off( void )
{
    if( tsk_rs_ign == TRUE)
    {
        tsk_rs_cmd_str_off();
        
        os_pip_insert( tsk_rs_pip , TSK_RS_IGN_OFF , TSK_RS_PIP_INSERT_TIMEOUT );
        
        ATOMIC(tsk_rs_ign = FALSE);
    }
}

/*--------------------------------------------------------------------------*/

void tsk_rs_cmd_str_on( void )
{
    if((tsk_rs_str == FALSE) && (tsk_rs_gwr==TRUE))
    {
        os_pip_insert( tsk_rs_pip , TSK_RS_STR_ON , TSK_RS_PIP_INSERT_TIMEOUT );
        
        ATOMIC(tsk_rs_str = TRUE);
    }
}

/*--------------------------------------------------------------------------*/

void tsk_rs_cmd_str_off( void )
{
    if(tsk_rs_str == TRUE)
    {
        os_pip_insert( tsk_rs_pip , TSK_RS_STR_OFF , TSK_RS_PIP_INSERT_TIMEOUT );
        
        ATOMIC(tsk_rs_str=FALSE);    
    }
}

/*--------------------------------------------------------------------------*/

Boolean tsk_rs_invalid_state( void )
{
    if(tsk_rs_invalid_state_var == TRUE) return TRUE;
    
	#if STATUS_SPEED_ENABLE == 1
		if(speed.bit.is_detected)
		{   
            if(get_speed()!=0)
            {
                ATOMIC(tsk_rs_invalid_state_var=TRUE);
            }           			
		}
		else
		{
            if(tsk_rs_idle && (get_handbrake_status()==FALSE))
            {
                ATOMIC(tsk_rs_invalid_state_var=TRUE);
            }                            			
		}
	#else
		if(tsk_rs_idle && (get_handbrake_status()==FALSE))
        {
            ATOMIC(tsk_rs_invalid_state_var=TRUE);
        }
	#endif
        
    return tsk_rs_invalid_state_var;
}

/*--------------------------------------------------------------------------*/

void tsk_rs_invalid_state_reset( void )
{            
    tsk_rs_idle = get_ign_status();
	            
    #if STATUS_SPEED_ENABLE == 1
		if(speed.bit.is_detected)
		{   
            ATOMIC(tsk_rs_invalid_state_var=(get_speed()!=0));  //speed == 0 ---> tsk_rs_invalid_state_var = FALSE ( the state is valid )          
		}                                                       //speed != 0 ---> tsk_rs_invalid_state_var = TRUE  ( the state is not valid )          
		else
		{
            //tsk_rs_idle == FALSE  handbrake dosen't matter   ---> tsk_rs_invalid_state_var = FALSE ( the state is valid when we don't running in idle state)
            //tsk_rs_idle == TRUE , handbrake not engagged     ---> tsk_rs_invalid_state_var = TRUE  ( the state is not valid when we running in idle state and handbrake is not engagged)
            //tsk_rs_idle == TRUE , handbrake engagged         ---> tsk_rs_invalid_state_var = FALSE ( the state is     valid when we running in idle state and handbrake is     engagged)
            
            ATOMIC(tsk_rs_invalid_state_var= (tsk_rs_idle && (get_handbrake_status()==FALSE)));               
		}
	#else
        //tsk_rs_idle == FALSE  handbrake dosen't matter   ---> tsk_rs_invalid_state_var = FALSE ( the state is valid when we don't running in idle state)
        //tsk_rs_idle == TRUE , handbrake not engagged     ---> tsk_rs_invalid_state_var = TRUE  ( the state is not valid when we running in idle state and handbrake is not engagged)
        //tsk_rs_idle == TRUE , handbrake engagged         ---> tsk_rs_invalid_state_var = FALSE ( the state is     valid when we running in idle state and handbrake is     engagged)
            
                
		ATOMIC(tsk_rs_invalid_state_var= (tsk_rs_idle && (get_handbrake_status()==FALSE)));               
	#endif            
}

/*--------------------------------------------------------------------------*/

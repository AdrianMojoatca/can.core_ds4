/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_tx_one_shot.c 20087 2013-03-28 16:41:56Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_usr_private.h"
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

Boolean can_usr_tx_one_shot( Can_Dev_Id dev_id , Can_Msg *can_msg , TskTimeout timeout )
{
	Boolean ret         = FALSE; 
	Boolean do_tx       = TRUE ;
	UInt8   retry_count = 0    ;
	
	os_res_request( can_usr[dev_id].tx_res , 0 );
	{			
		can_usr_one_shot_timeout_start( dev_id , timeout );			

       	while( do_tx )
		{			
		  	switch( can_tx_one_shot( can_usr[dev_id].tx_can_slow, can_msg,CAN_USR_TX_ONE_SHOT_TIMEOUT) )
			{
				case CAN_TX_OK :
				{
					do_tx = FALSE ; 
					
					ret = TRUE    ; 
					
					ATOMIC( can_usr[dev_id].err_cnt_slow = 0) ; 
				}
				break ; 
				
				case CAN_TX_BUSY             :
				case CAN_TX_LOST_ARBITRATION :	
				{
					retry_count %= 10 ; 
					os_tsk_wait( retry_count + 1 );//11ms max delay
					
					if( can_usr_one_shot_timeout_test( dev_id ) )
					{
						do_tx = FALSE ; 
						
						TRACE_CAN_USR("[CAN_DEV_%d TX SLOW TIMEOUT ERROR]\n\r" , dev_id );                        
							
						can_usr_err_add( (dev_id == CAN_DEV_0) ? CAN_USR_ERR_DEV0_TX_SLOW_TIMEOUT : CAN_USR_ERR_DEV1_TX_SLOW_TIMEOUT );	
					}						
				}
				break ; 

				case CAN_TX_BUS_ERROR        :
				{
					if( (++can_usr[dev_id].err_cnt_slow ) > CAN_USR_SLOW_ERROR_MAX ) 
					{
						can_usr_controller_reset( dev_id );

						ATOMIC( can_usr[dev_id].err_cnt_slow = 0 ) ; 
						
						#ifdef CAN_TX_USR_WAIT_AFTER_BUS_ERROR
							
							#if CAN_TX_USR_WAIT_AFTER_BUS_ERROR < 1000
								os_tsk_wait(CAN_TX_USR_WAIT_AFTER_BUS_ERROR);
								
								#warning "CAN_TX_USR_WAIT_AFTER_BUS_ERROR used"
							#else
								os_tsk_wait(1000);
								
								#warning "CAN_TX_USR_WAIT_AFTER_BUS_ERROR 1000ms used "
							#endif
						#endif								

						do_tx = FALSE ; 
					}
					else
					{
						can_usr_one_shot_timeout_start( dev_id , timeout );			
					}
															
					if( !can_usr_one_shot_timeout_test( dev_id ) )
					{
						TRACE_CAN_USR("[CAN_DEV_%d TX SLOW BUS ERROR]\n\r" , dev_id );                        
							
						can_usr_err_add( (dev_id == CAN_DEV_0) ? CAN_USR_ERR_DEV0_TX_SLOW_ERROR : CAN_USR_ERR_DEV1_TX_SLOW_ERROR );	
					}
					else
					{
						TRACE_CAN_USR("[CAN_DEV_%d TX SLOW TIMEOUT ERROR]\n\r" , dev_id );                        
							
						can_usr_err_add( (dev_id == CAN_DEV_0) ? CAN_USR_ERR_DEV0_TX_SLOW_TIMEOUT : CAN_USR_ERR_DEV1_TX_SLOW_TIMEOUT );	
					}					
				}
				break;
				
				
				default : break ;                                                                 
           }
		}
           
        can_usr_one_shot_timeout_stop( dev_id );
	}
	os_res_release( can_usr[dev_id].tx_res ) ;    

	return ret ;
}

//--------------------------------------------------------------------------//

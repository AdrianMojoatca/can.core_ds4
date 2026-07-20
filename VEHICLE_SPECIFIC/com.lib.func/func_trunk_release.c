/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: func_trunk_release.c 3491 2011-10-12 07:13:08Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "func_private.h"
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

void func_trunk_release_start( void )
{
	Can_Msg wake_trunk_rel_can_msg;
	Can_Msg trunk_rel_can_msg;
	
	/**** 26_01_2018 **/
	wake_trunk_rel_can_msg.data._8_[0]=0x00;
	wake_trunk_rel_can_msg.data._8_[1]=0x00;
	wake_trunk_rel_can_msg.data._8_[2]=0x00;
	wake_trunk_rel_can_msg.data._8_[3]=0x00;
	wake_trunk_rel_can_msg.data._8_[4]=0x00;
	wake_trunk_rel_can_msg.data._8_[5]=0x00;
	wake_trunk_rel_can_msg.data._8_[6]=0x00;
	wake_trunk_rel_can_msg.data._8_[7]=0x00;
	wake_trunk_rel_can_msg.id = 0x7FF;
	wake_trunk_rel_can_msg.length = 8;
	wake_trunk_rel_can_msg.is_ext = 0;
	wake_trunk_rel_can_msg.is_rtr = 0;
	can_usr_tx( CAN_DEV_0 , &wake_trunk_rel_can_msg , FALSE , 100 );
	os_tsk_wait(100);

	trunk_rel_can_msg.data._8_[0]=0x01;
	trunk_rel_can_msg.data._8_[1]=0x00;
	trunk_rel_can_msg.data._8_[2]=0x00;
	trunk_rel_can_msg.data._8_[3]=0x00;
	trunk_rel_can_msg.data._8_[4]=0x00;
	trunk_rel_can_msg.data._8_[5]=0x00;
	trunk_rel_can_msg.data._8_[6]=0x00;
	trunk_rel_can_msg.data._8_[7]=0x00;
	trunk_rel_can_msg.id = 0x331;
	trunk_rel_can_msg.length = 8;
	trunk_rel_can_msg.is_ext = 0;
	trunk_rel_can_msg.is_rtr = 0;
	can_usr_tx( CAN_DEV_0 , &trunk_rel_can_msg , FALSE , 100 );
	/**** 26_01_2018 **/

	
	    
    //func_can_wake_enter();

    TRACE_FUNC("\n\r[TRUNK_START]\n\r"); 		
}

/*--------------------------------------------------------------------------*/

Boolean func_trunk_release_exec( void )
{
    return get_trunk_status();
}

/*--------------------------------------------------------------------------*/

void func_trunk_release_stop( void )
{
    //func_can_wake_leave();
    
    TRACE_FUNC("\n\r[TRUNK_STOP]\n\r"); 		
}

/*--------------------------------------------------------------------------*/


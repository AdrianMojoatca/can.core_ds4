/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: func_tester_lock_unlock.c ADI M                                     $
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
extern Boolean sense_331_present;
extern Boolean sense_332_present;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
void tester_wake (void )
{
	Can_Msg wake_msg;
	UInt8 i;
	for ( i = 0; i < 8; i++ )
	{
		wake_msg.data._8_[i]	= 0x00;
	}
	wake_msg.id = 0x446;
	wake_msg.length = 8;
	wake_msg.is_ext = 0;
	wake_msg.is_rtr = 0;
	can_usr_tx( CAN_DEV_0 , &wake_msg , FALSE , 100 );	

	os_tsk_wait(50);
}

void tester_lock( void  )
{
	Can_Msg tester_lock_msg;
	UInt8 i, val1, val2;
	
	tester_wake();

	if ( sense_331_present )
	{
		GLOBAL_LOCK_446[6] = GLOBAL_331[2];
		GLOBAL_LOCK_446[7] = GLOBAL_331[3];
	}

	else if ( sense_332_present )
	{		
		GLOBAL_LOCK_446[6] = GLOBAL_332[2];
		GLOBAL_LOCK_446[7] = GLOBAL_332[3];
	}
	
	tester_lock_msg.data._8_[0]=0x0C;
	tester_lock_msg.data._8_[1]= GLOBAL_LOCK_446[1];
	tester_lock_msg.data._8_[2]=GLOBAL_LOCK_446[2];
	tester_lock_msg.data._8_[3]=(GLOBAL_LOCK_446[3] += 0x0D);
	tester_lock_msg.data._8_[4]=GLOBAL_LOCK_446[4];
	tester_lock_msg.data._8_[5]=(GLOBAL_LOCK_446[5] += 0x0D);
	tester_lock_msg.data._8_[6]=GLOBAL_LOCK_446[6];
	tester_lock_msg.data._8_[7]=GLOBAL_LOCK_446[7];
	tester_lock_msg.id = 0x446;
	tester_lock_msg.length = 8;
	tester_lock_msg.is_ext = 0;
	tester_lock_msg.is_rtr = 0;

	can_usr_tx( CAN_DEV_0 , &tester_lock_msg , FALSE , 100 );

	GLOBAL_UNLOCK_446[3] = GLOBAL_LOCK_446[3];
	GLOBAL_UNLOCK_446[5] = GLOBAL_LOCK_446[5];

	
			 
}

void tester_unlock ( void )
{
	Can_Msg tester_unlock_msg;

		tester_wake();

	if ( sense_331_present )
	{
		GLOBAL_UNLOCK_446[6] = GLOBAL_331[2];
		GLOBAL_UNLOCK_446[7] = GLOBAL_331[3];
	}

	else
	{
		GLOBAL_UNLOCK_446[6] = GLOBAL_332[2];
		GLOBAL_UNLOCK_446[7] = GLOBAL_332[3];
	} 
	
	tester_unlock_msg.data._8_[0]=0x04;
	tester_unlock_msg.data._8_[1]= GLOBAL_UNLOCK_446[1];
	tester_unlock_msg.data._8_[2]=GLOBAL_UNLOCK_446[2];
	tester_unlock_msg.data._8_[3]=(GLOBAL_UNLOCK_446[3] += 0x0D);
	tester_unlock_msg.data._8_[4]=GLOBAL_UNLOCK_446[4];
	tester_unlock_msg.data._8_[5]=(GLOBAL_UNLOCK_446[5] += 0x0D);
	tester_unlock_msg.data._8_[6]=GLOBAL_UNLOCK_446[6];
	tester_unlock_msg.data._8_[7]=GLOBAL_UNLOCK_446[7];
	tester_unlock_msg.id = 0x446;
	tester_unlock_msg.length = 8;
	tester_unlock_msg.is_ext = 0;
	tester_unlock_msg.is_rtr = 0;


	can_usr_tx( CAN_DEV_0 , &tester_unlock_msg , FALSE , 100 );

	GLOBAL_LOCK_446[3] = GLOBAL_UNLOCK_446[3];
	GLOBAL_LOCK_446[5] = GLOBAL_UNLOCK_446[5];
}

void tester_trunk_release ( void )
{
	Can_Msg tester_trunk_rel_msg;

	tester_wake();
	if ( sense_331_present )
	{
		GLOBAL_UNLOCK_446[6] = GLOBAL_331[2];
		GLOBAL_UNLOCK_446[7] = GLOBAL_331[3];
	}
	
	else
	{
		GLOBAL_UNLOCK_446[6] = GLOBAL_332[2];
		GLOBAL_UNLOCK_446[7] = GLOBAL_332[3];
	}
	
	tester_trunk_rel_msg.data._8_[0]=0x10;
	tester_trunk_rel_msg.data._8_[1]= GLOBAL_UNLOCK_446[1];
	tester_trunk_rel_msg.data._8_[2]=GLOBAL_UNLOCK_446[2];
	tester_trunk_rel_msg.data._8_[3]=(GLOBAL_UNLOCK_446[3] += 0x0D);
	tester_trunk_rel_msg.data._8_[4]=GLOBAL_UNLOCK_446[4];
	tester_trunk_rel_msg.data._8_[5]=(GLOBAL_UNLOCK_446[5] += 0x0D);
	tester_trunk_rel_msg.data._8_[6]=GLOBAL_UNLOCK_446[6];
	tester_trunk_rel_msg.data._8_[7]=GLOBAL_UNLOCK_446[7];
	tester_trunk_rel_msg.id = 0x446;
	tester_trunk_rel_msg.length = 8;
	tester_trunk_rel_msg.is_ext = 0;
	tester_trunk_rel_msg.is_rtr = 0;

	can_usr_tx( CAN_DEV_0 , &tester_trunk_rel_msg , FALSE , 100 );

	GLOBAL_LOCK_446[3] = GLOBAL_UNLOCK_446[3];
	GLOBAL_LOCK_446[5] = GLOBAL_UNLOCK_446[5];
}

/*--------------------------------------------------------------------------*/


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: func_disarm_p.c -1   $
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

// ADI - 09.01.2017
static	Can_Sync_Flags	disarm_cmd_332(Can_Msg *can_msg);
static  Can_Sync_Flags  disarm_sta_331(Can_Msg *can_msg);

UInt8 dis_counter;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean func_disarm_done = FALSE ;

extern Boolean locked_status;
extern Boolean unlocked_status;

extern UInt8 GLOBAL_RKE[8]; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void func_disarm_start( void  )
{
    
	Can_Msg wake_unlock_can_msg;
	Can_Msg unlock_can_msg;
	Can_Msg mirror_on_can_msg;
	UInt8 i;

	//tester_unlock();
	//ATOMIC( func_disarm_done = TRUE );
	//return;
	

	dis_counter = 0;

	ATOMIC( func_disarm_done = FALSE );	
	
	// ADI - 05.01.2017
	wake_unlock_can_msg.data._8_[0]=0x00;
	wake_unlock_can_msg.data._8_[1]=0x00;
	wake_unlock_can_msg.data._8_[2]=0x00;
	wake_unlock_can_msg.data._8_[3]=0x00;
	wake_unlock_can_msg.data._8_[4]=0x00;
	wake_unlock_can_msg.data._8_[5]=0x00;
	wake_unlock_can_msg.data._8_[6]=0x00;
	wake_unlock_can_msg.data._8_[7]=0x00;
	wake_unlock_can_msg.id = 0x7FF;
	wake_unlock_can_msg.length = 8;
	wake_unlock_can_msg.is_ext = 0;
	wake_unlock_can_msg.is_rtr = 0;
	can_usr_tx( CAN_DEV_0 , &wake_unlock_can_msg , FALSE , 100 );

	os_tsk_wait(50);

	unlock_can_msg.data._8_[0]=0x20;
	//unlock_can_msg.data._8_[1]=0x01;

	// ADI - 22_04_2019
	if ( is_disarm_unlock == FALSE ) 
	{	 
		unlock_can_msg.data._8_[1] = 0x01;
		is_disarm_unlock = TRUE;
	}
	else
	{
		unlock_can_msg.data._8_[1] = 0x02;
		is_disarm_unlock = FALSE;
	}

	unlock_can_msg.data._8_[2]=0x48;
	unlock_can_msg.data._8_[3]=0xAB;
	unlock_can_msg.data._8_[4]=0x00;
	unlock_can_msg.data._8_[5]=0x00;
	unlock_can_msg.data._8_[6]=0x00;
	unlock_can_msg.data._8_[7]=0x00;
	unlock_can_msg.id = 0x332;
	unlock_can_msg.length = 8;
	unlock_can_msg.is_ext = 0;
	unlock_can_msg.is_rtr = 0;
	can_usr_tx( CAN_DEV_0 , &unlock_can_msg , FALSE , 100 );
	os_tsk_wait(50);

	// vers. 1_01 - 08_06_2917 - make here the mirrors
	//331~C0~26~00~00~02~B2~40~24~~
	mirror_on_can_msg.data._8_[0]=GLOBAL_RKE[0];
	mirror_on_can_msg.data._8_[1]=GLOBAL_RKE[1];
	mirror_on_can_msg.data._8_[2]=GLOBAL_RKE[2];
	mirror_on_can_msg.data._8_[3]=GLOBAL_RKE[3];
	mirror_on_can_msg.data._8_[4]=0x04;
	mirror_on_can_msg.data._8_[5]=GLOBAL_RKE[5];
	mirror_on_can_msg.data._8_[6]=0x40;
	mirror_on_can_msg.data._8_[7]=GLOBAL_RKE[7];
	mirror_on_can_msg.id = 0x331;
	mirror_on_can_msg.length = 8;
	mirror_on_can_msg.is_ext = 0;
	mirror_on_can_msg.is_rtr = 0;
	for ( i = 0; i < 3; i++ )
	{
		can_usr_tx( CAN_DEV_0 , &mirror_on_can_msg , FALSE , 100 );
		os_tsk_wait(500);
	}
	// end mirror phase


	can_sync_loop( CAN_DEV_0 , (Can_Sync_Func)disarm_sta_331 ,(Can_Sync_Arg)NULL, 0x331, 1);

	os_tsk_wait(1000);
	    
    //func_can_wake_enter();
    	
    TRACE_FUNC("\n\r[DISARM_START]\n\r"); 		
}

/*--------------------------------------------------------------------------*/

Boolean func_disarm_exec( void  )
{   
	
	// ADI - 09.01.2017
	
	if ( !func_disarm_done )
	{
		// ADI - 11.01.2017
		can_sync_remove( CAN_DEV_0 , (Can_Sync_Func)disarm_cmd_332 ) ;
		can_sync_add ( CAN_DEV_0 , (Can_Sync_Func)disarm_cmd_332 ,(Can_Sync_Arg)NULL, 0x332, 0 , 1 , 1 , 1);
		os_tsk_wait(700);
		dis_counter++;	
	}
	if (dis_counter >=4 ) return TRUE;
	return func_disarm_done ; 	
}

/*--------------------------------------------------------------------------*/

void func_disarm_stop( void  )
{
    //func_can_wake_leave();
	
	can_sync_remove( CAN_DEV_0 , (Can_Sync_Func)disarm_cmd_332 ) ;	
	can_sync_remove( CAN_DEV_0 , (Can_Sync_Func)disarm_sta_331 ) ;

	func_disarm_done = TRUE;	
    
    TRACE_FUNC("\n\r[DISARM_STOP(%s)]\n\r",func_disarm_done ? "ok":"error"); 		
}

/*--------------------------------------------------------------------------*/


// ADI - 09.01.2017
Can_Sync_Flags disarm_cmd_332( Can_Msg *can_msg )
{   
	os_tsk_wait(50);
	can_msg->data._8_[0] = 0x20;

	//can_msg->data._8_[1] = 0x01;
	// ADI - 22_04_2019
	if ( is_disarm_unlock == FALSE ) 
	{	 
		can_msg->data._8_[1] = 0x01;
		is_disarm_unlock = TRUE;
	}
	else
	{
		can_msg->data._8_[1] = 0x02;
		is_disarm_unlock = FALSE;
	}
	can_msg->data._8_[2] = 0x48;
	can_msg->data._8_[3] = 0xAB;
	can_msg->data._8_[4] = 0x00;
	can_msg->data._8_[5] = 0x00;
	can_msg->data._8_[6] = 0x00;
	can_msg->data._8_[7] = 0x00;


    //return  func_disarm_done ? CAN_SYNC_COMPLETE | CAN_SYNC_TX : CAN_SYNC_TX ; 
	//return  (func_disarm_done) ? CAN_SYNC_COMPLETE : CAN_SYNC_TX ; 
	return 	CAN_SYNC_TX;
}

Can_Sync_Flags disarm_sta_331( Can_Msg *can_msg )
{   
	
	if(!func_disarm_done)
	{
		if ((DATA[4] & 0x04) == 0x04 ) 
		{
			ATOMIC( func_disarm_done = TRUE );
		}
		else 
		{
			ATOMIC( func_disarm_done = FALSE );	
		}

	}

	return  CAN_SYNC_NO_ACTION ; 
}




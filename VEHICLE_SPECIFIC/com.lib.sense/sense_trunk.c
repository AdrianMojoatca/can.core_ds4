/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_trunk.c 4227 2012-10-18 13:32:07Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Boolean trunk_st = FALSE;
Boolean tr_to_started = FALSE;
UInt8 tr_to_counter;
Boolean tr_rel_active;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_trunk_3B3( Can_Msg *can_msg )
{
  	
	static UInt8 lastB7 = 0;

	/*  vers. 1_21 - 30_01_2023 */
	//if( ( (!!(DATA1(0) & 0x01)) || (!!(DATA1(7) & 0x04)) ) )
	if (((DATA1(0) & 0x01)== 0x01) ||  ((DATA1(7) & 0x04) == 0x04) )
	{
		trunk_st = TRUE;
	}
	else
	{
		trunk_st = FALSE;
	}
	//if ((DATA1(7) == 0x80 )	&& (lastB7 != 0x80) && (tr_rel_active == FALSE) ) // ...daca avem trunk release OEM
	if ( (((DATA1(6) & 0x40) == 0x40)) && (((DATA1(7) & 0x80) == 0x80)) && (tr_rel_active == FALSE)) //...daca avem trunk release OEM
	{
		tr_rel_active = TRUE;
		tr_to_counter = 0;
		timeout_start(trunk_sense_delayed);  // ....pornim timer-ul 
		tr_to_started = TRUE;
	}
	//if ( tr_to_started == FALSE )  // daca n-avem trunk release OEM ...
	if ( (tr_to_started == FALSE) || (!get_ss_lock()) )  // daca n-avem trunk release OEM ...
	{
		sense_rs_trunk_can(trunk_st); // .... semnalam direct trunk status
	}
	if ( tr_to_started && trunk_st ) // daca am si timer started si trunk open ..
	{
		if ( get_ss_lock() )
		{
			//sense_rs_oem_trunk_can(TRUE); // .... facem trunk release
			d2d_tx(0xDF); // .... facem trunk release
			timeout_stop(trunk_sense_delayed); // ... si oprim timer-ul
			tr_to_started = FALSE;	// .. si permitem semnalarea directa pt. trunk status
			tr_rel_active = FALSE;  // .... putem s-o luam de la capat
		}
	}
	/* vers. 1_21 - 30_01_2023 */

	
	lastB7 = DATA1(7);

}

void trunk_status_handler( void )
{    
    Can_Msg test_msg;
	UInt8 i;
	for ( i = 0; i < 8; i++ )
	{
		test_msg.data._8_[i]	= 0x00;
	}
	test_msg.id = 0x123;
	test_msg.length = 8;
	test_msg.is_ext = 0;
	test_msg.is_rtr = 0;
	
	
	//timeout_stop(trunk_sense_delayed); // ... si oprim timer-ul

	tr_to_counter++;     // .. si incrementam counter-ul

	if ( tr_to_counter < 8 )  // .. verficam daca avem 30 de secunde(tr_to_counter) 
	{
		 timeout_stop(trunk_sense_delayed); // ... si oprim timer-ul
		 timeout_start(trunk_sense_delayed);  // .. si-l mai pornim o data
		 test_msg.data._8_[0] = tr_to_counter;
	
		 can_usr_tx( CAN_DEV_0 , &test_msg , FALSE , 100 );
	}
	else // altfel il oprim de tot si raportam tr_to_started
	{
		timeout_stop(trunk_sense_delayed); //
		tr_to_started = FALSE; 
		tr_to_counter = 0;
		test_msg.data._8_[0] = 0xFF;
		tr_rel_active = FALSE;
		can_usr_tx( CAN_DEV_0 , &test_msg , FALSE , 100 );
	}
}	
//--------------------------------------------------------------------------//

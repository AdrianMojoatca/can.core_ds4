/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_oem.c 1830 2010-10-13 11:43:27Z florin.olariu $
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
Boolean locked_status;
Boolean unlocked_status;

// vers. 1_01 - 08_06_2018
UInt8 GLOBAL_RKE[8];
Boolean trunk_rel_status;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
#define DOORLOCK_IS_RKE               (DATA1(6) &  0x40)
#define DOORLOCK_COUNTER               DATA1(5)
#define DOORLOCK_IS_LOCK           !!(((DATA1(4) & 0x07)  == 2))
#define DOORLOCK_IS_UNLOCK_ALL     !!(((DATA1(4) & 0x04)) == 4)

#define DOORLOCK_IS_KEYPAD            (DATA1(6) &  0x20)
#define DOORLOCK_IS_DOORHANDLE        (DATA1(6) &  0x80)


//--------------------------------------------------------------------------//

void sense_rke_331( Can_Msg *can_msg )
{        
    static UInt8 counter    ;
	UInt8 i;
	// vers. 1_01 - 08_06_2018
	for ( i = 0; i < 8; i++ )
	{
		GLOBAL_RKE[i] = DATA1(i);
	}
	//end vers 1_01 - 08_06_2018

	// vers. 1_03 - 13_06_2018
	

	if(DOORLOCK_COUNTER != counter)
    {
        counter    = DOORLOCK_COUNTER ;
		if(DOORLOCK_IS_LOCK)
	    {
	       	if(DOORLOCK_IS_RKE || DOORLOCK_IS_KEYPAD || DOORLOCK_IS_DOORHANDLE)
	        {
	            sense_rs_oem_lock_can(TRUE);
				locked_status = TRUE;
				unlocked_status = FALSE;
	        }
			else 
			{
				sense_rs_oem_lock_can(FALSE);
				locked_status = FALSE;
			}
		}
	
		if (DOORLOCK_IS_UNLOCK_ALL) 
	    {
	        
			if(DOORLOCK_IS_RKE  || DOORLOCK_IS_KEYPAD || DOORLOCK_IS_DOORHANDLE)
	        {
	            //sense_rs_oem_unlock_can(TRUE) ;
				if ( !unlocked_status ) 
				{
					sense_rs_oem_unlock_can(TRUE); // 25_01_2018
					unlocked_status = TRUE;
				}
				
	        }  
	        else 
			{
				sense_rs_oem_unlock_can(FALSE) ;
				unlocked_status = FALSE;
			}
	    }
		else unlocked_status = FALSE;
	}

    //sense_rs_oem_trunk_can    ( !(LAST_B0&0x04) && !!(DATA[0]&0x04) );
    //sense_rs_oem_autolock_can ( !(LAST_B0&0x08) && !!(DATA[0]&0x08) );
   
}

void sense_rke_3B3( Can_Msg *can_msg )
{
	static UInt8 last_tr_status = 0x00;
	static UInt8 last_tr_status1 = 0x00;

	// first version 07_06_2018 - lets' return here if we are in gwr mode - let 0x3C3 do the rest
	if ( get_rs_gwr() ) return;

	//if ( (DATA1(6) == 0x40) && ((DATA1(7) & 0x84) == 0x84) )
	
	// just for testing without trunk release
	/*
	if ( ((DATA1(6) == 0xC0) || (DATA1(6) == 0x40)) && (((DATA1(7) & 0x84) == 0x84) || ((DATA1(7) & 0x06) == 0x06)) ) // vers. 1_02 - 05_06_2018
	{
		if ( (last_tr_status != 0x84) && (last_tr_status != 0x06) )
		{
			if (get_ss_lock() )
			{
				d2d_tx(0xDF);
				//sense_rs_oem_trunk_can(TRUE);
			}
		}
	}
	else 
	{
		sense_rs_oem_trunk_can(FALSE);
	}
	*/
	last_tr_status = DATA1(7) & 0x84;
	last_tr_status1 = DATA1(7) & 0x06;
}

// 07_06_2018
void sense_rke_3C3( Can_Msg *can_msg )
{
	static UInt8 last_B6 = 0;
	if ( get_rs_gwr() )
	{
		if ( (DATA1(6) == 0x81) && (last_B6 != 0x81 ) )
		{
			if (get_ss_lock() )
			{
				d2d_tx(0xDF);
				//sense_rs_oem_trunk_can(TRUE);
			}
		}
		else 
		{
			sense_rs_oem_trunk_can(FALSE);
		}
		last_B6 = DATA1(6);
	}
}


//--------------------------------------------------------------------------//

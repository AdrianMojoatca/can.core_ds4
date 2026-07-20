/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rpm.c 4227 2012-10-18 13:32:07Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define RPM_HI DATA1(3)&0x0F
#define RPM_LO DATA1(4)
#define RPM_WORD (((UInt16)RPM_HI << 8) | RPM_LO) * 2

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Boolean electric_started;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rpm_204( Can_Msg *can_msg )
{        
    // ADI - 05.01.2017
    UInt16 my_rpm;
	union
	{
		unsigned char sz_rpm[2];
		unsigned short int u_rpm;
	} my_u_rpm;
	my_u_rpm.sz_rpm[1] = DATA1(3)&0x0F;
	my_u_rpm.sz_rpm[0] = DATA1(4);

	my_rpm = 2*my_u_rpm.u_rpm;

	/*
	if (get_ign_status() )	 
	{
		sense_rs_rpm_can(my_rpm);
		//TRACE("RPM: %d\r\n", RPM_WORD );
	}
	else sense_rs_rpm_can(NULL);
	*/

	if ( my_u_rpm.u_rpm != 0x00 )
	{
		if (get_ign_status() ) sense_rs_rpm_can (my_rpm);
		else  sense_rs_rpm_can(NULL);
	}

	else 
	{
		if ( electric_started )
		{
			sense_rs_rpm_can(1000);
		}
		else 
		{
			sense_rs_rpm_can(NULL);
		}
	}
		
	//sense_rs_rpm_can( (DATA[2]*1000)/16   );
}


void sense_rpm_fake_rpm( Can_Msg *can_msg )
{
	if ( (DATA[0] & 0x80) == 0x80 ) // first nibble 0 means electrically started
	{
		electric_started  = TRUE;	
	}
	else
	{
		electric_started = FALSE;
	}	
}	
//--------------------------------------------------------------------------//


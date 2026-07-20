/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_glowplug.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void sense_rs_headlight_status_update(UInt32 *headlight_status, Boolean status, UInt8 from);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static UInt32 sense_rs_headlight_wire_old_sts=0;
static Boolean sense_rs_headlight_wire_old_allsts=FALSE;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/     	

void sense_rs_headlight_detect( Boolean do_init , Boolean status, UInt8 from)
{	
	sense_rs_headlight_status_update(&sense_rs_headlights_sts, status, from);	
	if( do_init )
    {
			 set_headlight_wire_status( sense_rs_headlights_sts ? TRUE : FALSE ) ;
			 sense_rs_headlight_wire_old_sts = sense_rs_headlights_sts;
			 sense_rs_headlight_wire_old_allsts = get_headlight_wire_status();
			
        set_headlight_wire_status( status ) ;	
    }
    else
    {   
			if(sense_rs_headlights_sts != sense_rs_headlight_wire_old_sts)
			{
				set_headlight_wire_status(sense_rs_headlights_sts ? TRUE : FALSE);
				if(get_headlight_wire_status() != sense_rs_headlight_wire_old_allsts)
				{
					cmd_q_rs_insert( get_headlight_wire_status() ? INPUT_HEADLIGHT_STATUS_ON :  INPUT_HEADLIGHT_STATUS_OFF , TIMEOUT_Q_INSERT );
					sense_rs_headlight_wire_old_allsts = get_headlight_wire_status();
				}
			}
			sense_rs_headlight_wire_old_sts = sense_rs_headlights_sts;
    }
}

/*--------------------------------------------------------------------------*/     	
void sense_rs_headlight_status_update(UInt32 *headlight_status, Boolean status, UInt8 from)
{
		if(status)
			*headlight_status |= (1<<from);
		else
			*headlight_status &= ~(1<<from);
}

/*--------------------------------------------------------------------------*/     	


/*==========================================================================*/
// $Id: dei_sc_icon_valet_off.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
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

Boolean dei_sc_icon_combo_valet_off( void )
{
	UInt8 security_status = dei_sc_info_get( DEI_SC_INFO_SECURITY ) ; 
#if 0
    if( (security_status == DEI_SECURITY_DISARMED) || (security_status == DEI_SECURITY_DISARMED_COMBO_VALET_ON) )
    {
        dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_DISARMED ); 
    }
    else
    {
        dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_ARMED ); 
    }        
#endif
		if((security_status & DEI_SECURITY_COMBO_VALET_STATUS_BIT) != DEI_SECURITY_COMBO_VALET_STATUS_BIT )
			security_status &= DEI_SECURITY_ARM_STATUS_BIT;
		else
			security_status &= ~(DEI_SECURITY_COMBO_VALET_STATUS_BIT);
		
	dei_sc_info_set( DEI_SC_INFO_SECURITY  , security_status ); 
		
	return dei_sc_tx( DEI_SC_ACK_VALET_OFF , DEI_SC_PAGGING() ? DEI_SC_PAGGING_TX : DEI_SC_NORMAL_TX, 100 ) ;       	
}

//--------------------------------------------------------------------------//

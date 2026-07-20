/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_gearbox_type.c 33405 2015-08-26 21:21:53Z suchita.yadav $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_gearbox_type( void )
{
#ifdef DEV_LPC_236x
	Boolean read1 , read2 , read3 ; 
	
	read_gearbox_type:
	{
		os_tsk_wait( 100 ); 
		read1 = SENSE_RS_PIN_AUTO_MANUAL ; 
		
		os_tsk_wait( 100 ); 
		read2 = SENSE_RS_PIN_AUTO_MANUAL ; 

		os_tsk_wait( 100 ); 
		read3 = SENSE_RS_PIN_AUTO_MANUAL ; 

		if( (read1 != read2)  || (read2 != read3) || (read1 != read3))  goto read_gearbox_type ;
	}
	set_gearbox_type( read1 ? GEARBOX_MANUAL : GEARBOX_AUTOMATIC);
#else	
	UInt8 gearbox_type;
	if(nvfs_rd_s(nvfs_auto_manual, &gearbox_type, sizeof(gearbox_type)) == NVFS_OK)
	{
		if( gearbox_type == 1 )
			set_gearbox_type( GEARBOX_MANUAL );
		else
			set_gearbox_type( GEARBOX_AUTOMATIC );
	}
	else
		set_gearbox_type( GEARBOX_AUTOMATIC );
#endif

	TRACE_DEI_SENSE_RS("\n\r[GEARBOX_%s]\n\r" , (get_gearbox_type() == GEARBOX_MANUAL)? "MANUAL" : "AUTOMATIC" );	
}

//--------------------------------------------------------------------------//




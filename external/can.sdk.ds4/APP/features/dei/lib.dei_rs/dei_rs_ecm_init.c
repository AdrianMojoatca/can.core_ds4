/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_ecm_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rs_ecm_func( void ); 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Tsk dei_rs_ecm_tsk ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_ecm_init( void )
{	    	
	dei_rs_ecm_tsk = os_tsk_create((TskFunc)dei_rs_ecm_func,TASK_DEI_RS_ECM_PRI,TASK_DEI_RS_ECM_STK,0,TASK_DEI_RS_ECM_NAME);

	
	#define RPM    dei_feature_get( DEI_FEAT_RPM_REF_DIV10 )
	set_rs_rpm_ref( RPM * 10 );	 // Tach reference is divided by 10 (make it one-byte variable)
	#undef RPM

	set_rs_vtacho_ref( dei_feature_get( DEI_FEAT_VTACHO_REF ) ) ;
	
	ecm_vtacho_init( get_rs_vtacho_ref() , dei_feature_get( DEI_FEAT_VIRTUAL_TACH_FINE_TUNE )&0x3F ); 	
}


//--------------------------------------------------------------------------//

void dei_rs_ecm_start( void )
{
	os_tsk_start( dei_rs_ecm_tsk , NULL );
}

//--------------------------------------------------------------------------//

void dei_rs_ecm_func( void )
{
    static const TskFunc ecm_func[] = 
	{
		(TskFunc)dei_rs_ecm_vtacho      ,
		(TskFunc)dei_rs_ecm_voltage     , 
		(TskFunc)dei_rs_ecm_off         , 
		(TskFunc)dei_rs_ecm_tacho         		
	};

    static const UInt8* ecm_type[] = 
	{
		"VIRTUAL TACHO" , 
		"VOLTAGE",
		"NO ENGINE CHECK",
		"TACHOMETER"
	};
        
    TskFunc selected_ecm ; 

    UInt8 ecm_index =  dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )-1;

    if(ecm_index>=sizeof_array(ecm_func))
    {
        ecm_index = 3; // tacho mode if something went wrong with the feature list
    }

    TRACE("[REMOTE STARTED IN %s MODE]\n\r",ecm_type[ecm_index]);

    selected_ecm = ecm_func[ecm_index];

    selected_ecm(NULL);
}

//--------------------------------------------------------------------------//


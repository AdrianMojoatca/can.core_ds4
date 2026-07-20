/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __dei_prkH__
#define __dei_prkH__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef enum
{    
    DEI_PRK_LOCK             	,
    DEI_PRK_LOCK_RS            	,
    DEI_PRK_UNLOCK_NO_EVENT  	,
    DEI_PRK_UNLOCK_NO_EVENT_RS 	,
    DEI_PRK_UNLOCK_LOW_BATT		,
	DEI_PRK_UNLOCK_TRIGGER_ON	,
    DEI_PRK_UNLOCK_NPC_ON    	,	
    DEI_PRK_TRIGGER          	,
    DEI_PRK_WARNING          	,
    DEI_PRK_MTS_ON           	,
    DEI_PRK_MTS_OFF          	,
    DEI_PRK_SMART_ON         	,
    DEI_PRK_SMART_OFF        	,
    DEI_PRK_TMR_ON           	,
    DEI_PRK_TMR_OFF          	,
           
    DEI_PRK_RS_CONFIRM       	,
           
    DEI_PRK_RS_CONSTANT      	,
    DEI_PRK_RS_PULSED        	,    
    
	DEI_PRK_RS_STOP           	,
           
    DEI_PRK_ERROR_START_BRAKE	,
    DEI_PRK_ERROR_START_HOOD 	,
    DEI_PRK_ERROR_START_MTS  	,
    DEI_PRK_ERROR_START_PARKING ,
	DEI_PRK_ERROR_LOW_TACH      ,

	DEI_PRK_FULL_SILENT_ARM     ,
    DEI_PRK_TRIGGER_STOP        ,

    DEI_PRK_CAR_FINDER          ,

	DEI_PRK_SENSOR_BYPASS_OFF   ,
	DEI_PRK_SENSOR_BYPASS_WARN  ,
	DEI_PRK_SENSOR_BYPASS_FULL  ,
           
    DEI_PRK_NO_PATTERN

}Dei_Prk_Pattern;


//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean dei_prk_init( void  ); 

Boolean dei_prk_flash ( Dei_Prk_Pattern pattern );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



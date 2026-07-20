/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk.h 17934 2012-10-29 19:23:36Z martin.bouchard $
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
           
    AS_PRK_RS_OFF							,
	  AS_PRK_RS_MTS_DISABLE			,
	  AS_PRK_ADJUST							,
	  AS_PRK_RUNTIME_RESET		  ,


		DEI_PRK_RS_CONSTANT     	,
    DEI_PRK_RS_PULSED        	,    
    
	DEI_PRK_RS_STOP           	,
           
    DEI_PRK_ERROR_START_BRAKE	,
    DEI_PRK_ERROR_START_HOOD 	,
    DEI_PRK_ERROR_START_TRUNK	,
    DEI_PRK_ERROR_START_MTS  	,
    DEI_PRK_ERROR_START_PARKING ,
	DEI_PRK_ERROR_LOW_TACH      ,

	DEI_PRK_RS_VALET_ON      ,
	DEI_PRK_RS_VALET_OFF      ,

	AS_PRK_ERROR_tACH_BEFORE_ST,

	DEI_PRK_FULL_SILENT_ARM     ,
    DEI_PRK_TRIGGER_STOP        ,

    DEI_PRK_CAR_FINDER          ,

	DEI_PRK_SENSOR_BYPASS_OFF   ,
	DEI_PRK_SENSOR_BYPASS_WARN  ,
	DEI_PRK_SENSOR_BYPASS_FULL  ,
      
    AS_PRK_PRG_OPTION1         ,
    AS_PRK_PRG_OPTION2         ,
    AS_PRK_PRG_OPTION3         ,
    AS_PRK_PRG_OPTION4         ,
    
    AS_PRK_PRG_RESET           ,
    AS_PRK_PRG_VTACH_SAVED     ,
			
    AS_PRK_VALET_ON            ,
    AS_PRK_VALET_OFF           ,
    
    AS_PRK_AUTOMATIC_GEARBOX   ,
    AS_PRK_MANUAL_GEARBOX      ,
    AS_PRK_REMOTE_PROGRAMMED   ,
    AS_PRK_TACH_PROGRAMMED     ,

		DEI_PRK_SENSOR_SILENT_ARM  ,
		
	  DEI_PRK_OFF								 ,

		DEI_PRK_NO_PATTERN

}Dei_Prk_Pattern;

// synonyms:
#define DEI_PRK_FLASH1 DEI_PRK_LOCK
#define DEI_PRK_FLASH2 DEI_PRK_UNLOCK_NO_EVENT
#define DEI_PRK_FLASH3 DEI_PRK_UNLOCK_LOW_BATT
#define DEI_PRK_FLASH4 DEI_PRK_UNLOCK_TRIGGER_ON



//--------------------------------------------------------------------------//


#define AS_PRK_PRG_ENTER        DEI_PRK_RS_CONSTANT
#define AS_PRK_PRG_LEAVE        DEI_PRK_RS_STOP
#define AS_PRK_PRG_VTACH_INC    DEI_PRK_LOCK
#define AS_PRK_PRG_VTACH_DEC    DEI_PRK_UNLOCK_NO_EVENT


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



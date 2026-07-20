/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_feature.h 21550 2013-07-15 13:28:26Z louis-philippe.rispoli $
/*==========================================================================*/

#ifndef __RSR_FEATURE_H__
#define __RSR_FEATURE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// These are the indexes to use when calling rsr_feature_get(), etc.
// Make sure these enums follow the same order as 
// <rsr_feature_default_table> and <rsr_feature_check_list>

typedef enum
{
	RS_FEAT_REMOTE_START_RUNTIME ,
	RS_FEAT_DIESEL_START_DELAY   ,
  	RS_FEAT_START_ATTEMPTS       ,
  	RS_FEAT_3X_LOCK_START        ,
    RS_FEAT_ENGINE_CHECK         ,

	RS_FEAT_COUNT                ,

} Rsr_Feature_Index ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean rsr_feature_init       ( void );
Boolean rsr_feature_set        ( Rsr_Feature_Index index , UInt8 value );
UInt8   rsr_feature_get        ( Rsr_Feature_Index index );
Boolean rsr_feature_enabled    ( Rsr_Feature_Index index );
void    rsr_feature_reset      ( void );
void    rsr_feature_init_reset ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#endif


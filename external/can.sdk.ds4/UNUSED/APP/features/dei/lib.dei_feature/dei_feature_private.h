/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_feature_private.h 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

#ifndef __DEI_FEATURE_PRIVATE_H__
#define __DEI_FEATURE_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_feature.h"

#include "feature.h"
#include "nvfs_usr.h"
#include "wdog_v.h"
#include "led.h"
#include "status_system.h"
#include "config_gpio.h"
#if BUILD_NGMM == 1
#include "config_ngmm_feature.h"
#else
#include "config_dei_feature.h"
#endif

#include "trace.h"
#include "dbg.h"

#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef DEBUG_DEI_FEAT
#define DEBUG_DEI_FEAT			0
#endif

#if DEBUG_DEI_FEAT == 1
#define TRACE_DEI_FEAT			TRACE
#else
#define TRACE_DEI_FEAT(...)		;
#endif
/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

typedef struct s_Dei_Feature_Check
{
    UInt8   min_value ;
    UInt8   max_value ;

    Boolean check     ;

}Dei_Feature_Check;


/*--------------------------------------------------------------------------*/
      
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern UInt8                    dei_feature_list         [FEAT_DEI_SIZE];
extern const Dei_Feature_Check  dei_feature_check_list   [FEAT_DEI_SIZE];
extern const UInt8              dei_feature_default_table[FEAT_DEI_SIZE];
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void dei_feature_save          (void);
void dei_feature_save_sleep    (Wake_Status wake_status);
void dei_feature_check_content (void);     
void dei_feature_check_size    (void);
void dei_feature_error         (void);
/*==========================================================================*/

#endif


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_feature_vars.c 21550 2013-07-15 13:28:26Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_feature_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

//const NVFS_Entry_Name nvfs_feat_rs     = "FEAT_RS"     ;	// Already defined in nvfs_usr_vars
//const NVFS_Entry_Name nvfs_feat_rsr_ver = "FEAT_RS_VER" ;
const NVFS_Entry_Name nvfs_feat_rs_err = "FEAT_RS_ERR" ;

// Actual feature list, loaded from nvfs
Rsr_Feature rsr_feature_list [RS_FEAT_COUNT];

// Default feature list, will be stored in nvfs when list does not exist or is invalid
const Rsr_Feature rsr_feature_default_table [RS_FEAT_COUNT] = 
{
	{ RS_FEAT_REMOTE_START_RUNTIME_ENABLED , RS_FEAT_REMOTE_START_RUNTIME_DEF_VALUE },
	{ RS_FEAT_DIESEL_START_DELAY_ENABLED   , RS_FEAT_DIESEL_START_DELAY_DEF_VALUE   },
	{ RS_FEAT_START_ATTEMPTS_ENABLED       , RS_FEAT_START_ATTEMPTS_DEF_VALUE       },
	{ RS_FEAT_3X_LOCK_START_ENABLED        , RS_FEAT_3X_LOCK_START_DEF_VALUE        },
    { RS_FEAT_ENGINE_CHECK_ENABLED         , RS_FEAT_ENGINE_CHECK_DEF_VALUE         },
};

// Feature check list - values loaded from nvfs are domain-checked against this table
const Rsr_Feature_Check rsr_feature_check_list [RS_FEAT_COUNT] = 
{
	{ RS_FEAT_REMOTE_START_RUNTIME_MIN, RS_FEAT_REMOTE_START_RUNTIME_MAX , RS_FEAT_REMOTE_START_RUNTIME_CHECK },    
    { RS_FEAT_DIESEL_START_DELAY_MIN  , RS_FEAT_DIESEL_START_DELAY_MAX   , RS_FEAT_DIESEL_START_DELAY_CHECK   },     
    { RS_FEAT_START_ATTEMPTS_MIN      , RS_FEAT_START_ATTEMPTS_MAX       , RS_FEAT_START_ATTEMPTS_CHECK       },      
    { RS_FEAT_3X_LOCK_START_MIN       , RS_FEAT_3X_LOCK_START_MAX        , RS_FEAT_3X_LOCK_START_CHECK        }, 
    { RS_FEAT_ENGINE_CHECK_MIN        , RS_FEAT_ENGINE_CHECK_MAX         , RS_FEAT_ENGINE_CHECK_CHECK         },      
};

// Error log, stored in nvfs
Rsr_Feature_Err rsr_feature_err ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


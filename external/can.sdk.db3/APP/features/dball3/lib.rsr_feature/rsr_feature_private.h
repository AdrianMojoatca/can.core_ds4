/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_feature_private.h 36991 2016-02-02 16:29:50Z martin.bouchard $
/*==========================================================================*/

#ifndef __RSR_FEATURE_PRIVATE_H__
#define __RSR_FEATURE_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_feature.h"
#include "core_contract_db3_rsr_feature_config.h"
#include "nvfs_usr.h"
#include "nvram_usr.h"

#if RS_FEATURE_VERSION != 5
  #error RSR FEATURE NOT MATCH, PLEASE UPDATE CONFIG_RSR_FEATURE.h
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// Size of the error log (circular buffer)
#define RS_FEAT_ERR_BUFF_SIZE 5

// This structure configures a single dball feature
typedef struct s_Rsr_Feature
{
    Boolean enabled ;
    UInt8   value   ;

} Rsr_Feature;

// This structure supports rsr_feature_check_contents for a single feature
typedef struct s_Rsr_Feature_Check
{
    UInt8   min_value ;
    UInt8   max_value ;
    Boolean check     ;

} Rsr_Feature_Check;

// enumeration for rsr_feature error codes, stored in the nvfs error log
typedef enum
{
    RS_FEAT_ERR_INVALID_INDEX = 1,
    RS_FEAT_ERR_INVALID_VALUE    ,
    RS_FEAT_ERR_INVALID_SIZE     ,
    RS_FEAT_ERR_INVALID_VERSION  ,

    RS_FEAT_ERR_COUNT 

} Rsr_Feature_Err_Code ;

// This structure contains the error log (stored to nvfs)
typedef struct s_Rsr_Feature_Err
{
    UInt16              count                           ;
    Rsr_Feature_Err_Code code [RS_FEAT_ERR_BUFF_SIZE] ;
  
} Rsr_Feature_Err ;

//--------------------------------------------------------------------------//
// Remote Start Features
//--------------------------------------------------------------------------//

// Remote Start Runtime
#define RS_FEAT_REMOTE_START_RUNTIME_MIN  			1
#define RS_FEAT_REMOTE_START_RUNTIME_MAX  			60
#define RS_FEAT_REMOTE_START_RUNTIME_CHECK			TRUE

#if RS_FEAT_REMOTE_START_RUNTIME_CHECK==TRUE
    #if RS_FEAT_REMOTE_START_RUNTIME_DEF_VALUE < RS_FEAT_REMOTE_START_RUNTIME_MIN
        #error RS_FEAT_REMOTE_START_RUNTIME MIN ERROR
    #endif

    #if RS_FEAT_REMOTE_START_RUNTIME_DEF_VALUE > RS_FEAT_REMOTE_START_RUNTIME_MAX
        #error RS_FEAT_REMOTE_START_RUNTIME MAX ERROR
    #endif
#endif

// Diesel Start Delay
#define RS_FEAT_DIESEL_START_DELAY_MIN  			0
#define RS_FEAT_DIESEL_START_DELAY_MAX  			90
#define RS_FEAT_DIESEL_START_DELAY_CHECK			TRUE

#if RS_FEAT_DIESEL_START_DELAY_CHECK==TRUE
    #if RS_FEAT_DIESEL_START_DELAY_DEF_VALUE < RS_FEAT_DIESEL_START_DELAY_MIN
        #error RS_FEAT_DIESEL_START_DELAY MIN ERROR
    #endif

    #if RS_FEAT_DIESEL_START_DELAY_DEF_VALUE > RS_FEAT_DIESEL_START_DELAY_MAX
        #error RS_FEAT_DIESEL_START_DELAY MAX ERROR
    #endif
#endif

// Start Attempts
#define RS_FEAT_START_ATTEMPTS_MIN  			1
#define RS_FEAT_START_ATTEMPTS_MAX  			3
#define RS_FEAT_START_ATTEMPTS_CHECK			TRUE

#if RS_FEAT_START_ATTEMPTS_CHECK==TRUE
    #if RS_FEAT_START_ATTEMPTS_DEF_VALUE < RS_FEAT_START_ATTEMPTS_MIN
        #error RS_FEAT_START_ATTEMPTS MIN ERROR
    #endif

    #if RS_FEAT_START_ATTEMPTS_DEF_VALUE > RS_FEAT_START_ATTEMPTS_MAX
        #error RS_FEAT_START_ATTEMPTS MAX ERROR
    #endif
#endif

// 3X Lock Activation Timeout
#define RS_FEAT_3X_LOCK_START_MIN  			0
#define RS_FEAT_3X_LOCK_START_MAX  			10
#define RS_FEAT_3X_LOCK_START_CHECK			TRUE

#if RS_FEAT_3X_LOCK_START_CHECK==TRUE
    #if RS_FEAT_3X_LOCK_START_DEF_VALUE < RS_FEAT_3X_LOCK_START_MIN
        #error RS_FEAT_3X_LOCK_START MIN ERROR
    #endif

    #if RS_FEAT_3X_LOCK_START_DEF_VALUE > RS_FEAT_3X_LOCK_START_MAX
        #error RS_FEAT_3X_LOCK_START MAX ERROR
    #endif
#endif

// Engine Checking Mode
#define RS_FEAT_ENGINE_CHECK_MIN  			0
#define RS_FEAT_ENGINE_CHECK_MAX  			18
#define RS_FEAT_ENGINE_CHECK_CHECK			RS_FEAT_ENGINE_CHECK_ENABLED

#if RS_FEAT_ENGINE_CHECK_CHECK==TRUE
    #if RS_FEAT_ENGINE_CHECK_DEF_VALUE < RS_FEAT_ENGINE_CHECK_MIN
        #error RS_FEAT_ENGINE_CHECK MIN ERROR
    #endif

    #if RS_FEAT_ENGINE_CHECK_DEF_VALUE > RS_FEAT_ENGINE_CHECK_MAX
        #error RS_FEAT_ENGINE_CHECK MAX ERROR
    #endif
#endif

//--------------------------------------------------------------------------//
// Prog Features
//--------------------------------------------------------------------------//

//--------------------------------------------------------------------------//
// Other Features
//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean rsr_feature_err_save ( void ) ;
void    rsr_feature_err_add  ( Rsr_Feature_Err_Code code ) ;
void    rsr_feature_save     ( void );


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

//extern const NVFS_Entry_Name     nvfs_feat_dball     ;	// Already defined in nvfs_usr_vars
//extern const NVFS_Entry_Name     nvfs_feat_rsr_ver ;
extern const NVFS_Entry_Name     nvfs_feat_rs_err ;

extern Rsr_Feature                rsr_feature_list         [RS_FEAT_COUNT];
extern const Rsr_Feature   	     rsr_feature_default_table[RS_FEAT_COUNT];
extern const Rsr_Feature_Check    rsr_feature_check_list   [RS_FEAT_COUNT];

extern Rsr_Feature_Err            rsr_feature_err ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#endif


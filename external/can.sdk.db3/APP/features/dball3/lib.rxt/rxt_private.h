/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rxt_private.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __RXT_PRIVATE_H__
#define __RXT_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "rxt.h"
#include "timeout_f.h"
#include "config_rxt.h"
#include "nvfs_usr.h"
#include "status.h"
#include "dbg.h"

#ifdef DEV_LPC_236x
  #include "nvram_usr.h"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if CONFIG_RXT_VERSION != 2  // must be changed when config file is modified
   #error config_rxt does not match
#endif



#define RXT_ERR_BUFF_SIZE               5 

//#define RXT_DISPLAY_ERR_OEM_DIS         4
//#define RXT_DISPLAY_ERR_START_FAIL      5
//#define RXT_DISPLAY_ERR_STOP_FAIL       6
//#define RXT_DISPLAY_ERR_RFTD_NOT_ENA    7

#define RXT_DISPLAY_FUNC_START          7
#define RXT_DISPLAY_FUNC_STOP           8

#define RXT_1MIN_TIMEOUT                60000
#define RXT_EXPIRE_IN_3MIN              3
#define RXT_EXPIRE_IN_1MIN              1

#define RXT_MAX_RUNTIME                 30
#define RXT_MIN_RUNTIME                 1

#define RXT_OVERRUN                     -5      //vehicle has not stopped 5 min after runtime expired

//--------------------------------------------------------------------------//

typedef enum
{
    RXT_STATE_STOP      ,
    RXT_STATE_STARTING  ,
    RXT_STATE_STARTED   ,

    RXT_STATE_COUNT

} Rxt_State ;

//--------------------------------------------------------------------------//
  
typedef union s_Rxt_Dis
{   
  struct
    {
      unsigned extend_time  : 1 ;
      unsigned count        : 1 ;
      unsigned door         : 1 ;
      unsigned dtc          : 1 ;
      unsigned ignition     : 1 ;
      unsigned enable       : 1 ;
      unsigned hood         : 1 ;
      unsigned reserved1    : 1 ;
    } bit ;

  UInt8 byte ;

} Rxt_Rdy ;

//--------------------------------------------------------------------------//  

typedef struct s_Rxt_Entry
{
    Boolean     enable          ;
    Boolean     stopping        ;
    Boolean     oem_start       ;
    UInt8       default_runtime ;
    SInt8       runtime         ;
    UInt8       attempt         ;
    UInt8       count           ;    // rs count
    UInt8       ext_count       ;    //extended time count
    Rxt_Rdy     ready           ;
    Rxt_State   state           ;
    Rxt_Func *  start_func      ;
    Rxt_Func *  stop_func       ;
    Timeout     timeout         ;
    Timeout     lock_start_timeout ;

} Rxt_Entry ;

//--------------------------------------------------------------------------//

typedef enum
{
   NO_RXT_ERR         ,
   ERR_RXT_START_FAIL ,
   ERR_RXT_STOP_FAIL  ,
   ERR_RXT_OVERRUN    ,

   ERR_RXT_COUNT 

}Rxt_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Rxt_Err
{
  UInt16         count ;
  Rxt_Error_Code code [RXT_ERR_BUFF_SIZE] ;
  
}Rxt_Error ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    rxt_toggle              (void * arg) ;
void    rxt_start               (void * arg) ;
void    rxt_stop                (void * arg) ;
void    rxt_extend_runtime      (void * arg) ;
void    rxt_req_runtime         (void * arg) ;

void    rxt_do_start            (void) ;
void    rxt_do_stop             (void) ;

void    rxt_extend_runtime_update   (void) ;
void    rxt_ready_update            (void) ;
void    rxt_rf_update               (Boolean do_time_update) ;
void    rxt_timeout                 (void * arg) ;

Boolean rxt_init_nvfs           (void) ;
void    rxt_error               (Rxt_Error_Code code) ;
void    rxt_reset               (void) ;

void    rxt_gwr_on              (void * arg) ;

void    rxt_oem_lock            (void * arg) ;
void    rxt_oem_timeout_stop    (void * arg) ;



/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const NVFS_Entry_Name nvfs_rxt_err ;
extern       Rxt_Error       rxt_err ;

extern       Rxt_Entry       rxt ;

extern       Boolean         rxt_gwr_is_detect;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


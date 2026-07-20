/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tach_private.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __TACH_PRIVATE_H__
#define __TACH_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "wake.h"
#include "timeout.h"
#include "dbg.h"

#if DBG_TACH == 1
    #warning "DBG_TACH ENABLE"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef struct s_Tach_Vars
{
   Boolean is_enabled  ;
   Boolean flag_ign    ;
   Boolean flag_rs_ign ;
   Timeout timeout     ;
 
} Tach_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/


void tach_enable      (void) ;
void tach_disable     (void) ;
void tach_cmd_lut_ena (void) ;
void tach_cmd_lut_dis (void) ;

void tach_update_timeout    (void * arg) ;
void tach_ign_on      (void * arg) ;
void tach_ign_off     (void * arg) ;
void tach_rs_ign_on   (void * arg) ;
void tach_rs_ign_off  (void * arg) ;

void tach_wake        (Wake_Status wake_status) ;

void tach_dbg_print   (StrConst * text ,  const UInt8 data[] , UInt8 size) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Tach_Vars tach_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



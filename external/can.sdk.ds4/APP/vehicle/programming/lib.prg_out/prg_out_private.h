/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg_out_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __PRG_OUT_PRIVATE_H__
#define __PRG_OUT_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "prg_out.h"
#include "reg.h"
#include "brand.h"
#include "dei_feature.h"
#include "feature.h"
#include "config_gpio.h"

#if AUTOSTART_BRAND_ENABLE == 1
    #include "autostart_system.h"
#endif

#if ASTROSTART_BRAND_ENABLE == 1
    #include "astrostart_system.h"
#endif

#include "cmd.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef DEBUG_PRG_OUT
#define DEBUG_PRG_OUT			1
#endif

#if DEBUG_PRG_OUT == 1
#define TRACE_DEI_PRG_OUT			TRACE
#else
#define TRACE_DEI_PRG_OUT(...)		;
#endif

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void prg_out_hw_o1 	( Boolean status );
void prg_out_hw_o2 	( Boolean status );
void prg_out_hw_o3 	( Boolean status );
void prg_out_hw_o4 	( Boolean status );
void prg_out_hw_o5 	( Boolean status );
void prg_out_hw_o6 	( Boolean status );
void prg_out_hw_o7 	( Boolean status );
void prg_out_hw_o8 	( Boolean status );
void prg_out_hw_o9 	( Boolean status );
void prg_out_hw_o10	( Boolean status );
void prg_out_hw_o11	( Boolean status );
void prg_out_hw_o12	( Boolean status );
void prg_out_hw_o13	( Boolean status );
void prg_out_hw_o14	( Boolean status );
void prg_out_hw_o15	( Boolean status );
void prg_out_hw_o16	( Boolean status );
void prg_out_hw_o17	( Boolean status );
void prg_out_hw_not_configured( Boolean status );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Reg_Table     prg_out_table ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_siren_private.h 20820 2013-05-16 12:38:20Z florin.olariu $
/*==========================================================================*/

#ifndef __DEI_SIREN_PRIVATE_H__
#define __DEI_SIREN_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_siren.h"
#include "cmd.h"
#include "sys.h"
#include "out_q.h"
#include "config_gpio.h"
#include "per_exec.h"
#include "system_type.h"
#include "brand.h"
#include "dei_feature.h"
#include "prg_out.h"

#include "trace.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef DEBUG_DEI_SIREN
#define DEBUG_DEI_SIREN			0
#endif

#if DEBUG_DEI_SIREN == 1
#define TRACE_DEI_SIREN			TRACE
#else
#define TRACE_DEI_SIREN(...)		;
#endif

#define DEI_SIREN_OUT_Q_SIZE        5   //--- the size of out queue
#define DEI_SIREN_TMR_PERIOD        5   //--- the base time used by siren patterns
#define AS_SIREN_TMR_PERIOD        1   //--- the base time used by siren patterns

#ifndef DEI_SIREN_USE_SYSTEM_TIMER
    #define DEI_SIREN_USE_SYSTEM_TIMER  1
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    dei_siren_control( Boolean status );
void 		as_horn_control( Boolean status );
Boolean dei_siren_init   ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Out_Q    dei_siren_out_q ; 
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_private.h 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_PRK_PRIVATE_H__
#define __DEI_PRK_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_prk.h"
#include "sys.h"
#include "wake.h"
#include "brand.h"

#include "trace.h"
#include "dbg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#define TASK_DEI_PRK_NAME  "dei_prk"
#define TASK_DEI_PRK_PRI   6
#define DEI_PRK_STACK	   0x0150		   //Must be a multiple of 4
#define DEI_PRK_PIP_SIZE   8

//--------------------------------------------------------------------------//




#ifndef DEBUG_DEI_PRK
#define DEBUG_DEI_PRK			0
#endif

#if DEBUG_DEI_PRK == 1
#define TRACE_DEI_PRK			TRACE
#else
#define TRACE_DEI_PRK(...)		;
#endif
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean dei_prk_offset( void );

void    dei_prk_tmr   ( void );

void    dei_prk_tsk   ( void );

void    dei_prk_output( Boolean staus );

void 	dei_prk_pattern_trace   ( Dei_Prk_Pattern pattern ); 
void 	dei_prk_pattern_trace_enq   ( Dei_Prk_Pattern pattern ); 
void 	dei_prk_pattern_trace_deq   ( Dei_Prk_Pattern pattern ); 


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Pip 		  dei_prk_pip           ;
extern Irq 		  dei_prk_irq           ;
extern TskTimeout dei_prk_offset_length ;
extern Boolean    dei_prk_dome_active   ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


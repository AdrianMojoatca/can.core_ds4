/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: device.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "config.h"       // !!! Must be first include !!!

#undef    DEV_DEFINED

#if defined (DEV_LPC2364) || defined (DEV_LPC2366) || defined (DEV_LPC2368)
#define   DEV_LPC_236x
#define   DEV_DEFINED
#include "device_lpc.h"
//#warning "DEV_LPC_236x"
#endif

#if defined (DEV_LPC1765) || defined (DEV_LPC1766) 
#define   DEV_LPC_176x
#define   DEV_DEFINED
#include "device_lpc_17xx.h"
//#warning "DEV_LPC_176x"
#endif

#if defined (DEV_LPC11XX)
#define   DEV_LPC
#define   DEV_DEFINED
#include "device_lpc_11xx.h"
#endif

#if defined (DEV_GCC_X86)
#define   DEV_DEFINED
#include "device_gcc_x86.h"
#endif

#if defined (DEV_PIC18F2550)
#define  DEV_DEFINED
#include "device_pic18f2550.h"
#endif

#if defined (DEV_PIC18F26K20)
#define  DEV_DEFINED
#include "device_pic18f26k20.h"
#endif

#ifndef   DEV_DEFINED
#error !!! Target device not specified !!!
#endif

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//-[ Atomic Regions Abstractions ]------------------------------------------//

#define ATOMIC(__zzz__)     { CRITICAL     ; C_ENTER     ; { __zzz__ ; } C_LEAVE     ; }
#define ATOMIC_ALL(__zzz__) { CRITICAL_ALL ; C_ENTER_ALL ; { __zzz__ ; } C_LEAVE_ALL ; }

//-[ Harvard Architecture Abstractions ]------------------------------------//

#ifndef HARVARD_ARCHITECTURE

typedef const char * HVA_Const_Char_P ;

#endif

#define VUL_TYPECAST			(volatile unsigned long *)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#define struct_offs(type,field) ((UInt16)(UInt32)&(((type)0)->field))

#define sizeof_array(array) (sizeof (array) / sizeof (*array))

/*==========================================================================*/

#endif


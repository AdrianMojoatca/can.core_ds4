/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: device_lpc_17xx.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEVICE_LPC_17XX_H__
#define __DEVICE_LPC_17XX_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "LPC17xx.h"
#include "device_lpc_1700_rep.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifdef __cplusplus
  #define EXTERN_C_ENTER extern "C" {
  #define EXTERN_C_LEAVE } ;
#else
  #define EXTERN_C_ENTER 
  #define EXTERN_C_LEAVE
#endif

//-[ Boolean and Pointer Constants ]----------------------------------------//

#ifndef NULL
#define NULL  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//-[ IRQ Support ]----------------------------------------------------------//

EXTERN_C_ENTER

register unsigned PRIMASK     __asm ("primask"    ) ;
register unsigned BASEPRI     __asm ("basepri"    ) ;
register unsigned BASEPRI_MAX __asm ("basepri_max") ;

#define CRITICAL     unsigned int __irq_critvar__
#define CRITICAL_ALL unsigned int __irq_critvar_all__

#define IRQ_ENA_GLOBAL     (__schedule_barrier () , BASEPRI     = 0x00 , __schedule_barrier ())
#define IRQ_DIS_GLOBAL     (__schedule_barrier () , BASEPRI_MAX = 0xF8 , __schedule_barrier ())

#define IRQ_ENA_GLOBAL_ALL (__schedule_barrier () , PRIMASK     = 0x00 , __schedule_barrier ())
#define IRQ_DIS_GLOBAL_ALL (__schedule_barrier () , PRIMASK     = 0x01 , __schedule_barrier ())

#define C_ENTER            (__schedule_barrier () ,           __irq_critvar__     = BASEPRI , BASEPRI_MAX = 0xF8 , __schedule_barrier ()) 
#define C_LEAVE            (__schedule_barrier () , BASEPRI = __irq_critvar__                                    , __schedule_barrier ())

#define C_ENTER_ALL        (__schedule_barrier () ,           __irq_critvar_all__ = PRIMASK , PRIMASK     = 0x01 , __schedule_barrier ()) 
#define C_LEAVE_ALL        (__schedule_barrier () , PRIMASK = __irq_critvar_all__                                , __schedule_barrier ())

EXTERN_C_LEAVE

//-[ Machine Specific Halting ]---------------------------------------------//

EXTERN_C_ENTER

#define HALT_LOOP for (;;)

EXTERN_C_LEAVE

//-[ Machine Specific Data Types ]------------------------------------------//

EXTERN_C_ENTER

typedef signed   char          SInt8   ;
typedef signed   short         SInt16  ;
typedef signed   int           SInt32  ;
typedef signed   long long int SInt64  ;

typedef unsigned char          UInt8   ;
typedef unsigned short         UInt16  ;
typedef unsigned int           UInt32  ;
typedef unsigned long long int UInt64  ;

typedef          UInt8         Boolean ;

typedef          UInt32        VARG    ;

EXTERN_C_LEAVE

//-[ Machine Specific String Constants ]-------------------------------------//

EXTERN_C_ENTER

typedef const char StrConst ;

#define P_STR(z)       z
#define P_STR_RD(z) (*(z))

EXTERN_C_LEAVE

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: trace_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __TRACE_PRIVATE_H__
#define __TRACE_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "trace.h"
#include "q.h"
#include "wake.h"

#ifdef DEV_LPC_236x
	#include "iap.h"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

#ifndef TRACE_Q_SIZE
#define TRACE_Q_SIZE			20
#else
#warning "THE DEFAULT VALUE FOR TRACE_Q_SIZE IS CHANGED"
#endif

#ifndef TRACE_ARG_SIZE
#define TRACE_ARG_SIZE			2
#else
	#if TRACE_ARG_SIZE > 4
		#error "TOO MANY ARGUMENTS IN TRACE" 
	#endif
#warning "THE DEFAULT VALUE FOR TRACE_ARG_SIZE IS CHANGED"
#endif


/*--------------------------------------------------------------------------*/

#define TRACE_TSK_PRI			5
#define TRACE_TSK_STK			0x140
#define TRACE_TSK_NAME			"trace"


/*--------------------------------------------------------------------------*/

#define TRACE_KEY_OS_STATUS     '?'

/*--------------------------------------------------------------------------*/

typedef struct s_Trace_Q_Msg
{
  Q_Msg        q_msg  ;  
  void         *str   ;    
  void         *arg[TRACE_ARG_SIZE];    
}Trace_Q_Msg ;

/*--------------------------------------------------------------------------*/


typedef struct s_Trace_Vars
{
    Q           q_free   ; 
    Q           q_pending;
    Sio         sio      ;
}Trace_Vars ;

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

#if USE_TRACE == 1

void trace_tsk            ( void );
void trace_print_os_status( void );
void trace_print_mem      ( UInt8* addr );
void trace_tx             ( int data );
void trace_print_wdog     ( void );
void trace_mem_error      ( void * address , UInt16 size_requested );


//void t_printf( const char *fmt , ... );
/**** some new printf *****/
void init_printf(void* putp,void (*putf) (void*,char));
void tfp_printf(const char *fmt, ...);
void tfp_sprintf(const char* s,char *fmt, ...);
void tfp_format(void* putp,void (*putf) (void*,char),const char *fmt, va_list va);
#define t_printf tfp_printf 
#define t_sprintf tfp_sprintf 
void putc1 ( void* p, char c);

#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#if USE_TRACE == 1

extern Trace_Vars trace_vars ; 

#endif

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


/*<#======================================================================#>*/
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbgcli_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __DBGCLI_PRIVATE_H__
#define __DBGCLI_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <stdio.h>
#include "os.h"
#include "q.h"
#include "config_dbg.h"
#include <stdarg.h>

#include "dbgcli.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

struct __FILE
{
  /// Standard 'C' library fields ///

  int handle ;

  /// User defined fields ///
 
} ;

#define DBG_TSK_PRI			  5
#define DBG_TSK_STK			  0x100
#define DBG_TSK_NAME			"dbg_tsk"

#define DBG_ARG_SIZE			4

#define DBG_Q_SIZE 	20

/*--------------------------------------------------------------------------*/

typedef struct s_Dbg_Q_Msg
{
  Q_Msg        q_msg  ;  
  void         *str   ;    
  void         *arg[DBG_ARG_SIZE];    
}Dbg_Q_Msg ;

/*--------------------------------------------------------------------------*/


typedef struct s_Dbg_Vars
{
    Q           q_free   ; 
    Q           q_pending;
}Dbg_Vars ;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void dbg_irq_service (void) ;
void dbg_tsk	(void);
void dbgcli_tx           (int data);

void do_help(char *arg);
void dbg_print_os_status( void );
void store_char(char ch);
int dbgprintf(const char *fmt,...);

void dbgcli_tx_request (void) ;
void dbgcli_tx_release (void) ;

void dbg_print_status (void)             ;
void dbg_print_mem    (UInt8 * mem_addr) ;


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern FILE __stdout  ; 

extern Irq dbg_irq_tx ;
extern Irq dbg_irq_rx ;

extern volatile UInt8 dbg_rda_rx ;

extern Res dbg_tx_res ;

extern Res dbg_str_res ;
extern Dbg_Vars dbg_vars ; 

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


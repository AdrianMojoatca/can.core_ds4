/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: cmd_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/
#ifndef __CMD_PRIVATE_H__
#define __CMD_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "cmd.h"
#include "reg_q.h"
#include "exec.h"
#include "q.h"
#include "nvfs.h"
#include <stdarg.h>

#include "dbg.h"

#if DBG_CMD == 1

  #if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800
    #include "trace.h"
  #endif

  #warning "DBG_CMD Is Enable"
#endif


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'cmd' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_CMD_NAME  "cmd"
#define TASK_CMD_PRI   10000//5000
#define CMD_STACK      0x0210         //Must be a multiple of 4

#ifndef DBG_CMD
#define DBG_CMD     0
#endif

#if DBG_CMD== 1
#define TRACE_CMD           TRACE
#else
#define TRACE_CMD(...)      ;
#endif

#define CMD_ERR_BUFF_SIZE       10 

//--------------------------------------------------------------------------//

typedef enum
{
   NO_CMD_ERR                 ,
   ERR_CMD_INSERT_TIMEOUT     ,    // cmd_q_insert timed out

   ERR_CMD_COUNT 

}Cmd_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Cmd_Err
{

  UInt16         count                    ;

  Cmd_Error_Code code [CMD_ERR_BUFF_SIZE] ;
 
  
}Cmd_Error ;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean         cmd_init_nvfs   (void);
void            cmd_task        (void *  arg) ;
void            cmd_reset       (void);
Boolean         cmd_error       (Cmd_Error_Code code);
void            cmd_trace       (Cmd cmd) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Tsk              tsk_cmd        ;
extern Reg_Q_Table      cmd_exec_table ;
extern Q                cmd_q_pending  ;
extern Q                cmd_q_free     ;

extern const NVFS_Entry_Name nvfs_cmd_error       ; // CMD Error Code
extern       Cmd_Error       cmd_err ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


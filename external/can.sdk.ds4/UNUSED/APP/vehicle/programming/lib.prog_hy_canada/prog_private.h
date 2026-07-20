/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id$
/*==========================================================================*/

#ifndef __PROG_PRIVATE_H__
#define __PROG_PRIVATE_H__



/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
#include "ied.h"

#include "prog.h"
#include "timeout.h"
#include "prog_private_defines.h"
#include "dbg.h"
#include "cmd.h"


#if DBG_PROG == 1
  
  #warning "DBG_PROG is Enable"
  
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'prog' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:8>
// <i>  Must be a multiple of 8.
// </h> 

#define TASK_PROG_NAME  "prog"
#define TASK_PROG_PRI   300
#define TASK_PROG_STK   0x100


#if DEBUG_PROG == 1
#define TRACE_DEI_PROG          dbg_tx_text
#else
#define TRACE_DEI_PROG(...)     ;
#endif


#define PROG_VERSION        2   // version of prog for XKEY compatibility

#define PROG_ERR_BUFF_SIZE  5  
#define PROG_FEAT_SIZE      FEATURE_NUM * sizeof (Feature_Entry)   

#define PROG_PTE_PERIOD     5   // ms
#define PROG_PIP_SIZE       4


//--------------------------------------------------------------------------//

typedef enum
{
    DEFAULT                      ,
    ENTER_LEAVE_EVENT            ,
    CHANGE_FEATURE_EVENT         ,
    CHANGE_OPTION_EVENT          ,
    PREPARE_CHANGE_FEATURE_EVENT

} Prog_Event ;

//--------------------------------------------------------------------------//

typedef struct s_Prog_Vars
{
    TskTimeout timeout           ;
    UInt8      feature_current   ;
    Boolean    do_change_feature ;
    Boolean    prog_enabled      ;
    Boolean    prog_entered      ;

    Prog_Func  exit_func         ;

} Prog_Vars ;

//--------------------------------------------------------------------------//

typedef struct s_Feature_Entry
{
    const UInt8   feature        ;
    const UInt8   option_number  ;
          UInt8   option_current ;
		  Boolean user_changeble ;
} Feature_Entry ;

//--------------------------------------------------------------------------//

typedef enum
{
   NO_PROG_ERR        ,
   ERR_PROG_READ      ,
   ERR_PROG_WRITE     ,
   ERR_PROG_CRC       ,

   ERR_PROG_COUNT 

}Prog_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Prog_Err
{

  UInt16         count                    ;

  Prog_Error_Code code [PROG_ERR_BUFF_SIZE] ;

}Prog_Error ;

//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                
void    prog_task                (void * tsk_arg)          ;

void    prog_init_reset          (void) ;
void    prog_reset               (void) ;

void    prog_input_ign_on        (void * arg) ;
void    prog_input_ign_off       (void * arg) ;
void    prog_input_long_push     (void * arg) ;
void    prog_input_push_press    (void * arg) ;
void    prog_input_push_release  (void * arg) ;
void    prog_input_change_option (void * arg) ;
void    prog_input_change_custom1option1(void * arg) ;
void    prog_input_change_custom1option2(void * arg) ;
void    prog_input_change_custom1option3(void * arg) ;

Boolean prog_init_nvfs           (void) ;
Boolean prog_feat_table_read     (void) ;
Boolean prog_feat_table_write    (void) ;

void    prog_err_add             (Prog_Error_Code code) ;
Boolean prog_err_save            (void)                 ; 


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Tsk           tsk_prog           ;
extern Pip           pip_prog           ;
extern Prog_Vars     prog_vars          ;
extern Timeout       prog_ign_timeout   ;
extern Feature_Entry feature_table   [] ;
extern UInt8         xkey_feature    [] ;
extern Prog_Error    prog_err           ;
extern const UInt8   FEATURE_NUM        ;

extern Feature_Entry feature_table_default[] ;

#endif



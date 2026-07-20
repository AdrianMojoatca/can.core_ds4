/****************************************************************************/
/*                                                                          */
/*                         A-Task Real-Time Kernel                          */
/*                                                                          */
/*                    Copyright (C) 2000 - Acacetus Inc                     */
/*                                                                          */
/*                           All rights reserved                            */
/*                                                                          */
/*                             www.acacetus.com                             */
/*                                                                          */
/****************************************************************************/

/*==========================================================================*/
// $Id: os.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#ifndef __OS_H__
#define __OS_H__

/*--------------------------------------------------------------------------*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*-------------------------------------------------------------------------*\
|                         Context Save Optimization                         |
|                                                                           |
| If user hooks for START, STOP, SAVE, and LOAD are not required, then set  |
| OS_USE_HOOK_CONTEXT to zero.  This signals the kernel not to process      |
| these hooks during a context switch and reduces the time required to      |
| switch contexts. It also eliminates the respective hook variables and     |
| reduces RAM usage.                                                        |
|                                                                           |
| When set to one, the kernel will process the hook variables.              |
|                                                                           |
| If this parameter is not defined, hook processing is enabled by default.  |
|                                                                           |
| Since task creation operations require a hook argument, set this argument |
| to NULL when hook processing is disabled.                                 |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef OS_USE_HOOK_CONTEXT
#define OS_USE_HOOK_CONTEXT 0
#endif

/*-------------------------------------------------------------------------*\
|                           Idle Stack Optimization                         |
|                                                                           |
| Certain CPU architectures require a separate IDLE stack for when the CPU  |
| becomes idle.  If this is required by the target application, then it can |
| be enabled by defining 'OS_USE_IDLE_STACK' to a value of 1.               |
|                                                                           |
| By default, no stack is created.  Enable this only if the particular OS   |
| port requires it.                                                         |
\*-------------------------------------------------------------------------*/

#ifndef OS_USE_IDLE_STACK
#define OS_USE_IDLE_STACK 0
#endif

/*-------------------------------------------------------------------------*\
|                          OS Component Optimization                        |
|                                                                           |
| OS components may be excluded by removing the component source file from  |
| the build.  Doing so will reduce code size for those environemtns that do |
| not filter out unreferenced code.                                         |
|                                                                           |
| The PTE and GTE components require additional macro definitions in order  |
| to tell the kernel not to link in their respective timer handlers.        |
|                                                                           |
| By default, PTE and GTE are integrated into the OS central timer logic.   |
|                                                                           |
| Components that can be excluded:                                          |
|                                                                           |
|       MSG , MBX , TFR , RES , PTE , GTE , FLG , PIP , EVT , IRQ           |
\*-------------------------------------------------------------------------*/

#ifndef OS_COMPONENT_PTE
#define OS_COMPONENT_PTE 1
#endif

#ifndef OS_COMPONENT_GTE
#define OS_COMPONENT_GTE 1
#endif

/*--------------------------------------------------------------------------*/

#define OS_PRI_MIN     0U
#define OS_PRI_MAX 65535U

#define CRITICAL_TSK TskPri __tsk_critvar__

#define C_ENTER_TSK do { __tsk_critvar__ = os_tsk_set_pri (  OS_PRI_MAX   ) ; } while (0)
#define C_LEAVE_TSK do {     (void)        os_tsk_set_pri (__tsk_critvar__) ; } while (0)

#define ATOMIC_TSK(__zzz__) { CRITICAL_TSK ; C_ENTER_TSK ; { __zzz__ ; } C_LEAVE_TSK ; }

/*--------------------------------------------------------------------------*/

typedef enum e_TskState
{
  OS_TSK_ABORTED            ,
  OS_TSK_RUNNING            ,
  OS_TSK_STOPPED            ,
  OS_TSK_WAITING            ,
  OS_TSK_ELIGIBLE           ,
  OS_TSK_SUSPENDED          ,

  OS_TSK_WAIT_MBX           ,
  OS_TSK_WAIT_IRQ           ,
  OS_TSK_WAIT_EVT           ,
  OS_TSK_WAIT_PTE           ,
  OS_TSK_WAIT_GTE           ,
  OS_TSK_WAIT_RES           ,

  OS_TSK_WAIT_FLG_SET       ,
  OS_TSK_WAIT_FLG_CLR       ,

  OS_TSK_WAIT_PIP_INS       ,
  OS_TSK_WAIT_PIP_REM       ,

  OS_TSK_WAIT_MSG_SEND      ,
  OS_TSK_WAIT_MSG_RCEV      ,
  OS_TSK_WAIT_MSG_RESP_PASS ,

  OS_TSK_WAIT_TFR_SEND      ,
  OS_TSK_WAIT_TFR_RCEV      ,

  OS_TSK_WAIT_TSK_STOP

} TskState ;

/*--------------------------------------------------------------------------*/

typedef struct s_Tsk * Tsk ; // Task Object       
typedef struct s_Irq * Irq ; // Interrupt Interface Object 
typedef struct s_Evt * Evt ; // Multiple Event Manager Object
typedef struct s_Res * Res ; // Resource Management Object
typedef struct s_Pte * Pte ; // Single Periodic Timer Event Object
typedef struct s_Gte * Gte ; // Grouped Periodic Timer Event Object
typedef struct s_Mbx * Mbx ; // Message Mailbox Object
typedef struct s_Flg * Flg ; // Flag set/clr object
typedef struct s_Pip * Pip ; // Byte Pipe Object
typedef struct s_Tfr * Tfr ; // Message Transfer Object

/*--------------------------------------------------------------------------*/

typedef UInt16 TskPri     ;
typedef UInt32 TskTimeout ;

/*--------------------------------------------------------------------------*/

typedef void (* SwiFunc       )(void *) ;

typedef void (* TskFunc       )(void *) ;
typedef void (* TskStopFunc   )(void *) ;
typedef void (* TskTimeoutFunc)(void *) ;

typedef void (* TskHookFunc   )(Tsk tsk , void *) ;

/*--------------------------------------------------------------------------*/

typedef struct  s_MbxMsg
{
  struct s_MbxMsg * next ;

} MbxMsg ;

/*--------------------------------------------------------------------------*/

typedef struct s_TskInfo
{
  StrConst * tsk_name       ;
  TskFunc    tsk_func       ;
  TskState   tsk_state      ;
  TskPri     tsk_priority   ;
  TskPri     tsk_priority_a ;

} TskInfo ;

/*--------------------------------------------------------------------------*/

typedef struct s_MemInfo
{
  SInt8   addr_inc  ;
  void  * addr_min  ;
  void  * addr_max  ;
  void  * addr_next ;

  UInt32  heap_allocated ; // Total RAM allocated by 'os_mem_request' calls.
  UInt32  heap_remaining ; // Total RAM unallocated
  UInt32  heap_unuseable ; // Total RAM unuseable due to alignment, etc.

} MemInfo ;

/*--------------------------------------------------------------------------*/

typedef struct s_StkInfo
{
  UInt8 * stk_beg      ; // Points to first byte of stack (After  $BB sequence).
  UInt8 * stk_end      ; // Points to last  byte of stack (Before $EE sequence).

  UInt16  free_mem     ;

  Boolean fence_ok_beg ;
  Boolean fence_ok_end ;

} StkInfo ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void       os_execute           (TskFunc tsk_func,TskPri tsk_pri,UInt16 stk_size,void * hook_arg,StrConst * tsk_name,void * tsk_arg,UInt16 sys_stk_size) ;

/*--------------------------------------------------------------------------*/

void     * os_mem_request       (UInt16    mem_size) ;
void       os_mem_info          (MemInfo * mem_info) ;

/*--------------------------------------------------------------------------*/

void       os_stk_info          (Tsk tsk,StkInfo * stk_info) ;
void       os_tsk_info          (Tsk tsk,TskInfo * tsk_info) ;

/*--------------------------------------------------------------------------*/

Tsk        os_tsk_current       (void) ;
                        
TskPri     os_tsk_get_pri       (Tsk    tsk)     ;
TskPri     os_tsk_set_pri       (TskPri tsk_pri) ;

TskFunc    os_tsk_set_func      (Tsk tsk,TskFunc tsk_func) ;
TskFunc    os_tsk_get_func      (Tsk tsk) ;

void     * os_tsk_get_user_ptr  (Tsk tsk) ;
void     * os_tsk_set_user_ptr  (Tsk tsk , void * user_ptr) ;

TskState   os_tsk_state         (Tsk tsk) ;

#define    os_tsk_create(tsk_func,tsk_pri,stk_size,hook_arg,tsk_name) \
           os_tsk_create_v      ((TskFunc)(tsk_func),(VARG)(tsk_pri),(VARG)(stk_size),(void *)(hook_arg),(StrConst *)(tsk_name))
Tsk        os_tsk_create_v      ( TskFunc  tsk_func ,...) ;

#define    os_tsk_start(tsk,tsk_arg) \
           os_tsk_start_v       ((Tsk)(tsk),(void *)(tsk_arg))
Boolean    os_tsk_start_v       ( Tsk  tsk ,...) ;
Boolean    os_tsk_start_i       ( Tsk  tsk , void * tsk_arg) ;

#define    os_tsk_start_s(tsk,tsk_arg,tsk_stop_func,tsk_stop_arg) \
           os_tsk_start_s_v     ((Tsk)(tsk),(void *)(tsk_arg),(TskStopFunc)(tsk_stop_func),(void *)(tsk_stop_arg))
Boolean    os_tsk_start_s_v     ( Tsk  tsk , ...) ;
Boolean    os_tsk_start_s_i     ( Tsk  tsk , void * tsk_arg , TskStopFunc tsk_stop_func , void * tsk_stop_arg) ;

void       os_tsk_wait          (TskTimeout timeout) ;

#define    os_tsk_wait_stop(tsk,timeout) \
           os_tsk_wait_stop_v   ((Tsk)(tsk),(VARG)(timeout))
Boolean    os_tsk_wait_stop_v   ( Tsk  tsk,...) ;

void     * os_tsk_suspend       (TskTimeout timeout) ;

#define    os_tsk_resume(tsk,tsk_resume_arg) \
           os_tsk_resume_v      ((Tsk)(tsk),(void *)(tsk_resume_arg))
Boolean    os_tsk_resume_v      ( Tsk  tsk , ... ) ;
Boolean    os_tsk_resume_i      ( Tsk  tsk , void * tsk_resume_arg) ;

void       os_tsk_stop          (void) ;

#define    os_tsk_stop_s(tsk_stop_func,tsk_stop_arg) \
           os_tsk_stop_s_v      ((TskStopFunc)(tsk_stop_func),(void *)(tsk_stop_arg))
void       os_tsk_stop_s_v      ( TskStopFunc  tsk_stop_func ,...) ;
           
void       os_tsk_release       (void) ;

void       os_tsk_list_init     (Tsk * tsk_list) ;
void       os_tsk_list_next     (Tsk * tsk_list) ;

/*--------------------------------------------------------------------------*/

#define    os_swi(swi_func,swi_arg) \
           os_swi_v             ((SwiFunc)(swi_func),(void *)(swi_arg))
void       os_swi_v             ( SwiFunc  swi_func,...) ;

/*--------------------------------------------------------------------------*/

Irq        os_irq_create        (void) ;
Boolean    os_irq_set           (Irq irq) ;
Boolean    os_irq_set_i         (Irq irq) ;
void       os_irq_clr           (Irq irq) ;
void       os_irq_clr_i         (Irq irq) ;

#define    os_irq_wait(irq,pre_clear,timeout) \
           os_irq_wait_v        ((Irq) irq,(VARG)(pre_clear),(VARG)(timeout))
Boolean    os_irq_wait_v        ( Irq  irq,...) ;

void       os_irq_enter         (void) ;
void       os_irq_leave         (void) ;

/*--------------------------------------------------------------------------*/

Evt        os_evt_create        (void) ;

void       os_evt_clr           (Evt evt,UInt16 evt_flags) ;
           
#define    os_evt_signal(evt,evt_flags) \
           os_evt_signal_v      ((Evt)(evt),(VARG)(evt_flags))
Boolean    os_evt_signal_v      ( Evt evt  ,...) ;

Boolean    os_evt_signal_i      (Evt evt,UInt16 evt_flags) ;

#define    os_evt_wait(evt,evt_flags,clr_flags,timeout) \
           os_evt_wait_v        ((Evt)(evt),(VARG)(evt_flags),(VARG)(clr_flags),(VARG)(timeout))
UInt16     os_evt_wait_v        ( Evt  evt ,...) ;

/*--------------------------------------------------------------------------*/

#define    os_msg_send(tsk_dest, msg, timeout) \
           os_msg_send_v        ((Tsk)(tsk_dest),(void * )(msg),(VARG)(timeout))
Boolean    os_msg_send_v        ( Tsk  tsk_dest ,...) ;

#define    os_msg_send_c(tsk_dest, msg) \
           os_msg_send_c_v      ((Tsk)(tsk_dest),(void *)(msg))
Boolean    os_msg_send_c_v      ( Tsk tsk_dest  ,...) ;

#define    os_msg_rcev(tsk_accept,msg,timeout) \
           os_msg_rcev_v        ((Tsk)(tsk_accept),(void *)(msg),(VARG)(timeout))
Tsk        os_msg_rcev_v        ( Tsk tsk_accept  ,...) ;

#define    os_msg_rcev_c(tsk_accept, msg) \
           os_msg_rcev_c_v      ((Tsk)(tsk_accept),(void *)(msg))
Tsk        os_msg_rcev_c_v      ( Tsk tsk_accept  ,...) ;

#define    os_msg_read(tsk_srce, msg) \
           os_msg_read_v        ((Tsk)(tsk_srce),(void *)(msg))
void       os_msg_read_v        ( Tsk  tsk_srce ,...) ;

void       os_msg_resp          (Tsk tsk_srce) ;

#define    os_msg_pass(tsk_srce, tsk_dest) \
           os_msg_pass_v        ((Tsk)(tsk_srce),(Tsk)(tsk_dest))
void       os_msg_pass_v        ( Tsk tsk_srce  ,...) ;

/*--------------------------------------------------------------------------*/

Tfr        os_tfr_create        (void) ;

#define    os_tfr_send(tfr,object,timeout) \
           os_tfr_send_v        ((Tfr)(tfr),(void *)(object),(VARG)(timeout))
Boolean    os_tfr_send_v        ( Tfr tfr,...) ;

#define    os_tfr_send_c(tfr,object) \
           os_tfr_send_c_v      ((Tfr)(tfr),(void *)(object))
Boolean    os_tfr_send_c_v      ( Tfr tfr,...) ;

Boolean    os_tfr_send_i        (Tfr tfr,void * object) ;

#define    os_tfr_rcev(tfr,object,timeout) \
           os_tfr_rcev_v        ((Tfr)(tfr),(void *)(object),(VARG)(timeout))
Boolean    os_tfr_rcev_v        ( Tfr tfr,...) ;

#define    os_tfr_rcev_c(tfr,object) \
           os_tfr_rcev_c_v      ((Tfr)(tfr),(void *)(object))
Boolean    os_tfr_rcev_c_v      ( Tfr tfr,...) ;

Boolean    os_tfr_rcev_i        (Tfr tfr,void * object) ;

/*--------------------------------------------------------------------------*/

Res        os_res_create        (void) ;

#define    os_res_request(res,timeout) \
           os_res_request_v     ((Res)(res),(VARG)(timeout))
Boolean    os_res_request_v     ( Res res  ,...) ;
Boolean    os_res_request_c     ( Res res ) ;
Boolean    os_res_request_i     ( Res res ) ;

void       os_res_release       (Res res) ;
void       os_res_release_i     (Res res) ;

/*--------------------------------------------------------------------------*/

Mbx        os_mbx_create        (void) ;

#define    os_mbx_send(mbx, msg) \
           os_mbx_send_v        ((Mbx)(mbx),(MbxMsg *)(msg))
void       os_mbx_send_v        ( Mbx  mbx ,...) ;

void       os_mbx_send_i        (Mbx mbx,MbxMsg * msg) ;

#define    os_mbx_rcev(mbx,timeout) \
           os_mbx_rcev_v        ((Mbx)(mbx),(VARG)(timeout))
MbxMsg   * os_mbx_rcev_v        ( Mbx  mbx ,...) ;

MbxMsg   * os_mbx_rcev_c        (Mbx mbx) ;
MbxMsg   * os_mbx_rcev_i        (Mbx mbx) ;

Boolean    os_mbx_test_empty    (Mbx mbx) ;
Boolean    os_mbx_test_empty_i  (Mbx mbx) ;

/*--------------------------------------------------------------------------*/

Flg        os_flg_create        (Boolean flg_ival) ;

Boolean    os_flg_state         (Flg flg) ;

Boolean    os_flg_set           (Flg flg) ;
Boolean    os_flg_set_i         (Flg flg) ;

Boolean    os_flg_clr           (Flg flg) ;
Boolean    os_flg_clr_i         (Flg flg) ;
                              
#define    os_flg_wait_set(flg,timeout) \
           os_flg_wait_set_v    ((Flg)(flg),(VARG)(timeout))
Boolean    os_flg_wait_set_v    ( Flg flg,...) ;

#define    os_flg_wait_clr(flg,timeout) \
           os_flg_wait_clr_v    ((Flg)(flg),(VARG)(timeout))
Boolean    os_flg_wait_clr_v    ( Flg flg,...) ;

/*--------------------------------------------------------------------------*/

Pip        os_pip_create        (UInt16 pip_size) ;

#define    os_pip_insert(pip,data,timeout) \
           os_pip_insert_v      ((Pip)(pip),(VARG)(data),(VARG)(timeout))
Boolean    os_pip_insert_v      ( Pip  pip ,...) ;

#define    os_pip_insert_c(pip,data) \
           os_pip_insert_c_v    ((Pip)(pip),(VARG)(data))
Boolean    os_pip_insert_c_v    ( Pip  pip ,...) ;

Boolean    os_pip_insert_i      (Pip pip,int data) ;

#define    os_pip_remove(pip,timeout) \
           os_pip_remove_v      ((Pip)(pip),(VARG)(timeout))
int        os_pip_remove_v      ( Pip  pip ,...) ;

int        os_pip_remove_c      (Pip pip) ;
int        os_pip_remove_i      (Pip pip) ;

void       os_pip_flush         (Pip pip) ;
void       os_pip_flush_i       (Pip pip) ;

Boolean    os_pip_test_full     (Pip pip) ;
Boolean    os_pip_test_empty    (Pip pip) ;

/*--------------------------------------------------------------------------*/

Pte        os_pte_create        (TskTimeout period) ;
void       os_pte_sync          (Pte pte) ;
Boolean    os_pte_wait          (Pte pte) ;

Boolean    os_pte_wake_set      (Pte pte) ;
Boolean    os_pte_wake_set_i    (Pte pte) ;

Boolean    os_pte_wake_clr      (Pte pte) ;
Boolean    os_pte_wake_clr_i    (Pte pte) ;

TskTimeout os_pte_set_period    (Pte pte, TskTimeout period) ;
TskTimeout os_pte_get_period    (Pte pte) ;

/*--------------------------------------------------------------------------*/

#define    os_gte_create(period,period_cnt) \
           os_gte_create_v      ((TskTimeout *)(period),(VARG)(period_cnt))
Gte        os_gte_create_v      ( TskTimeout *  period ,...) ;

UInt16     os_gte_wait          (Gte gte) ;

/*--------------------------------------------------------------------------*/

void       os_timeout_arm_set   (TskTimeoutFunc tsk_timeout_func , void * tsk_timeout_arg) ;
void       os_timeout_arm_clr   (void) ;

/*--------------------------------------------------------------------------*/

void       os_service_timer     (void) ;

EXTERN_C_LEAVE

/*==========================================================================*/
/*                     I N L I N E   F U N C T I O N S                      */
/*==========================================================================*/

#ifdef __cplusplus

inline void * operator new    (size_t size) { return os_mem_request (size) ; }
inline void * operator new [] (size_t size) { return os_mem_request (size) ; }

#endif

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus

inline void operator delete    (void * mem) { }
inline void operator delete [] (void * mem) { }

#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern void           * os_tsk_local ;             // Task local scope pointer

extern volatile Boolean os_irq_executing ;         // IRQ or TASK executing

extern void          (* os_sys_idle_func )(void) ; // System Idle Hook  
extern void          (* os_sys_init_func )(void) ; // System Init Hook

#if OS_USE_HOOK_CONTEXT == 1

extern TskHookFunc      os_tsk_hook_create ;       // CREATE Task Hook
extern TskHookFunc      os_tsk_hook_start  ;       // START  Task Hook
extern TskHookFunc      os_tsk_hook_stop   ;       // STOP   Task Hook
extern TskHookFunc      os_tsk_hook_save   ;       // SAVE   Task Hook
extern TskHookFunc      os_tsk_hook_load   ;       // LOAD   Task Hook

#endif

/*==========================================================================*/

#endif


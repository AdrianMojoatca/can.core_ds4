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
// $Id: os_local.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __OS_LOCAL_H__
#define __OS_LOCAL_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <stdarg.h>

#include "os.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define OS_MEM_FILL_VAL 0x00

#define OS_STK_FENCE_SIZE 4U

#define OS_STK_FENCE_MARK_BEG 0xBB
#define OS_STK_FENCE_MARK_END 0xEE

/*--------------------------------------------------------------------------*/

typedef void (* K_TmoFunc)(Tsk) ;
typedef void (* K_Func   )(void *,va_list) ;

/*--------------------------------------------------------------------------*/

typedef struct  s_StkLimits
{
  UInt8 * stk_beg ; // Points to first byte of stack (After  $BB sequence).
  UInt8 * stk_end ; // Points to last  byte of stack (Before $EE sequence).

} StkLimits ;

/*--------------------------------------------------------------------------*/

typedef struct s_QueueNode
{
  struct s_QueueNode * rhs ;
  struct s_QueueNode * lhs ;

} QueueNode ;

/*--------------------------------------------------------------------------*/

typedef struct s_PQueueNode // Derived from : QueueNode
{
  QueueNode q_node   ;

  TskPri    priority ;

} PQueueNode ;

/*--------------------------------------------------------------------------*/

typedef struct s_TQueueNode // Derived from : QueueNode
{
  QueueNode  q_node  ;

  TskTimeout timeout ;

} TQueueNode ;

/*--------------------------------------------------------------------------*/

struct s_Tsk // Derived from : PQueueNode , TQueueNode
{
  PQueueNode       pq_node ;
  TQueueNode       tq_node ;

  QueueNode        q_msgs      ;
  QueueNode        q_wait_stop ;

  void           * stk_top    ;
  StkLimits        stk_limits ;
  void           * stk_frame  ;

  K_TmoFunc        k_tmo_func ;

  void           * tsk_local      ;
  StrConst       * tsk_name       ;
  TskFunc          tsk_func       ;
  Tsk              tsk_list       ;
  TskState         tsk_state      ;
  TskPri           tsk_priority_a ;
  void           * tsk_user_ptr   ;

  void           * tsk_stop_arg  ;
  TskStopFunc      tsk_stop_func ;

  TskTimeoutFunc   tsk_timeout_func     ;
  void           * tsk_timeout_func_arg ;

  Boolean          is_timer_used  ;

  #if OS_USE_HOOK_CONTEXT == 1

  void           * tsk_hook_arg      ;
  TskHookFunc      running_hook_load ;

  #endif

  union 
    {
      Boolean  status ;

      void    * mem ;

      Tsk       tsk ;

      Irq       irq ;

      Res       res ;

      Pte       pte ;

      Flg       flg ;

      Tfr       tfr          ;
      void    * tfr_arg_send ;
      void   ** tfr_arg_rcev ;

      Pip       pip      ;
      int       pip_data ;
     
      Gte       gte      ;
      UInt16    gte_args ;

      Mbx       mbx     ;
      MbxMsg  * mbx_msg ;

      Evt       evt      ;
      UInt16    evt_args ;

      TskPri    tsk_pri  ;

      void    * tsk_hs_args    ;

      void    * tsk_resume_arg ;

      struct
        {
          void  * msg ;

        } msg_send   ;

      struct
        {
          void ** msg    ;

          Tsk tsk_accept ;

        } msg_rcev ;

    } arg ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Irq
{
  Boolean state   ;
  
  Tsk tsk_waiting ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Evt
{
  UInt16 evt_bits  ;

  UInt16 evt_flags ;
  UInt16 clr_flags ;

  Tsk tsk_waiting  ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Res
{
  QueueNode  q_waiting ;

  Tsk        tsk_owner ;

  UInt16     req_depth ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Mbx
{
  QueueNode q_waiting ;

  MbxMsg * head ;
  MbxMsg * tail ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Flg
{
  QueueNode q_wait_set ;
  QueueNode q_wait_clr ;

  Boolean   flg_state  ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Pip
{
  UInt8 * buff ;
  UInt8 * head ;
  UInt8 * tail ;

  UInt16  size    ;
  UInt16  entries ;

  QueueNode q_wait_insert ;
  QueueNode q_wait_remove ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Pte
{
  Pte        next ;

  Tsk        tsk_waiting ; 

  TskTimeout time    ;
  TskTimeout period  ;

  Boolean    wake_request ;
} ;

/*--------------------------------------------------------------------------*/

struct s_Gte
{
  Gte          next ;

  Tsk          tsk_waiting ;

  TskTimeout * time        ;
  TskTimeout * period      ;
  UInt8        period_cnt  ;

  UInt16       timeout_flags ;
        
  Boolean      is_active ;
} ;

//--------------------------------------------------------------------------//

struct s_Tfr
{
  QueueNode q_wait_send ;
  QueueNode q_wait_rcev ;
} ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

#define cast_q_to_pq(z)   ((PQueueNode *)((UInt8 *)(z) - struct_offs(PQueueNode *, q_node)))
#define cast_q_to_tq(z)   ((TQueueNode *)((UInt8 *)(z) - struct_offs(TQueueNode *, q_node)))
#define cast_q_to_tsk(z)  cast_pq_to_tsk(cast_q_to_pq(z))

#define cast_pq_to_q(z)   ((QueueNode  *)((UInt8 *)(z) + struct_offs(PQueueNode *, q_node)))
#define cast_pq_to_tsk(z) ((Tsk         )((UInt8 *)(z) - struct_offs(Tsk         ,pq_node)))

#define cast_tq_to_q(z)   ((QueueNode  *)((UInt8 *)(z) + struct_offs(TQueueNode *, q_node)))
#define cast_tq_to_tsk(z) ((Tsk         )((UInt8 *)(z) - struct_offs(Tsk         ,tq_node)))

#define cast_tsk_to_pq(z) ((PQueueNode *)((UInt8 *)(z) + struct_offs(Tsk         ,pq_node)))
#define cast_tsk_to_tq(z) ((TQueueNode *)((UInt8 *)(z) + struct_offs(Tsk         ,tq_node)))
#define cast_tsk_to_q(z)  cast_pq_to_q(cast_tsk_to_pq(z))

/*--------------------------------------------------------------------------*/

void         os_kern_init_hw_1             (void) ;
void         os_kern_init_hw_2             (void) ;

Tsk          os_kern_tsk_create            (TskFunc tsk_func,va_list tsk_info) ;
Tsk          os_kern_tsk_create_v          (TskFunc tsk_func,...) ;
void         os_kern_tsk_idle              (void) ;
void         os_kern_tsk_schedule          (void) ;
void         os_kern_tsk_start             (Tsk tsk,void * tsk_arg) ;
void       * os_kern_tsk_start_hw          (void * stk_top,TskFunc tsk_func,void * tsk_arg) ;
void         os_kern_tsk_execute_hw        (void) ;

void       * os_kern_mem_request_stk       (UInt16 mem_size,StkLimits * stk_limits) ;
UInt16       os_kern_mem_request_stk_extra (void) ;

void         os_kern_process_cmd           (K_Func k_func,void * arg0,va_list args) ;
void         os_kern_process_cmd_hw        (K_Func k_func,void * arg0,va_list args) ;

void         os_q_init                     (QueueNode  * q_node) ;
QueueNode  * os_q_head                     (QueueNode  * q_list) ;
void         os_q_unlink                   (QueueNode  * q_node) ;
void         os_q_link_rhs                 (QueueNode  * q_list,QueueNode * q_node) ;
void         os_q_link_lhs                 (QueueNode  * q_list,QueueNode * q_node) ;

PQueueNode * os_pq_head                    (QueueNode  * pq_list) ;
void         os_pq_insert_head             (QueueNode  * pq_list,PQueueNode * pq_node) ;
void         os_pq_insert_tail             (QueueNode  * pq_list,PQueueNode * pq_node) ;
void         os_pq_remove                  (PQueueNode * pq_node) ;

TQueueNode * os_tq_head                    (TQueueNode * tq_list) ;
void         os_tq_insert                  (TQueueNode * tq_list,TQueueNode * tq_node,TskTimeout timeout) ;
void         os_tq_remove                  (TQueueNode * tq_node) ;

void         os_eligible_insert_head       (Tsk tsk) ;
void         os_eligible_insert_tail       (Tsk tsk) ;

void         os_timer_insert               (TskTimeout timeout) ;
void         os_timer_remove               (Tsk tsk) ;
void         os_timer_remove_z             (Tsk tsk) ;

void         os_k_pte_service              (void) ;
void         os_k_gte_service              (void) ;
void         os_k_tmr_service              (void) ;

void         os_kern_null_idle             (void) ;

#if OS_USE_HOOK_CONTEXT == 1

void         os_kern_null_hook             (Tsk tsk , void * hook_arg) ;

#endif

void         os_kern_null_mem_fail (UInt16 size_requested) ;

void       * os_kern_stk_ptr_adjust        (void * stk_ptr,UInt16 stk_size) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern UInt16      os_stk_size_sys_and_idle ;

extern void      * os_sys_stk               ;
extern StkLimits   os_sys_stk_limits        ;

#if OS_USE_IDLE_STACK == 1

extern void      * os_idle_stk              ;
extern StkLimits   os_idle_stk_limits       ;

#endif
                                    
extern void      * os_stk_frame             ;
extern SInt8       os_stk_dir               ; // +1 = INC from STK_BEG , -1 = DEC from STK_END

extern TQueueNode  os_q_timer               ;
extern QueueNode   os_q_eligible            ;

extern Tsk         os_tsk_list              ; // Linked list of all tasks

extern Tsk         os_tsk_running           ;

extern va_list     os_va_list_void          ; // Used to satisfy compiler

#if OS_USE_HOOK_CONTEXT == 1

extern TskHookFunc os_running_hook_save ;

#endif

/*==========================================================================*/

#endif


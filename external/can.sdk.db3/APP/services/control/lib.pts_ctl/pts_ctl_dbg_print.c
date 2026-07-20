
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: pts_ctl_dbg_print.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "pts_ctl_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define TASK_PTS_CTL_DBG_STK   0x0180
#define TASK_PTS_CTL_DBG_NAME  "Pts_Ctl_Dbg"
#define TASK_PTS_CTL_DBG_PRI   100

#define PTS_CTL_DBG_Q_SIZE     20

//--------------------------------------------------------------------------//

typedef struct s_Pts_Ctl_Dbg_Q_Msg
{ 
  Q_Msg         q_base    ;
    
  StrConst    * text  ;
  
  UInt8 data[50]  ;
  
  UInt8 size    ;

} Pts_Ctl_Dbg_Q_Msg ;

//--------------------------------------------------------------------------//

typedef struct
{
  Q    q_free               ;
  Q    q_pending            ; 

  Tsk  tsk                  ;       
 
}Pts_Ctl_Dbg_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void pts_ctl_dbg_task(void * arg) ;
 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Pts_Ctl_Dbg_Vars pts_ctl_dbg_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void pts_ctl_dbg_init(void)
{
  #if DBG_PTS_CTL == 1

  UInt8 n ;
  
  pts_ctl_dbg_vars.q_free    = q_create () ;
  pts_ctl_dbg_vars.q_pending = q_create () ;

  pts_ctl_dbg_vars.tsk = os_tsk_create (pts_ctl_dbg_task , TASK_PTS_CTL_DBG_PRI , TASK_PTS_CTL_DBG_STK , 0 , TASK_PTS_CTL_DBG_NAME) ;

  for (n = 0 ; n < PTS_CTL_DBG_Q_SIZE ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Pts_Ctl_Dbg_Q_Msg)) ;
      
      q_assign (pts_ctl_dbg_vars.q_free , q_msg) ;
      q_insert (pts_ctl_dbg_vars.q_free , q_msg) ;
    }
  
  os_tsk_start (pts_ctl_dbg_vars.tsk, 0) ;
  
  #endif
}

//--------------------------------------------------------------------------//
static void pts_ctl_dbg_task(void * arg)
{
  #if DBG_PTS_CTL == 1

  pts_ctl_dbg_print ("\nPts_Ctl Dbg Start" , NULL , NULL) ;

  for(;;)
    {
      
      Pts_Ctl_Dbg_Q_Msg * q_msg = (Pts_Ctl_Dbg_Q_Msg *) q_remove (pts_ctl_dbg_vars.q_pending, 10) ;
        
      if (q_msg)
        {
          UInt8  i  ;
          
          dbg_tx_request() ;

          if(q_msg->text != NULL)
            {
              dbg_tx_text(q_msg->text) ;
            }
        
          for (i = 0 ; i < q_msg->size ; i++)
            {
              dbg_tx_h8(q_msg->data[i]) ;
            }
          
          dbg_tx_release() ;

          q_return ((Q_Msg *) q_msg) ;
    
        }
    }
  

  #endif
}
//--------------------------------------------------------------------------//

void pts_ctl_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size)
{  
  #if DBG_PTS_CTL == 1
  
  Pts_Ctl_Dbg_Q_Msg * q_msg = (Pts_Ctl_Dbg_Q_Msg *) q_remove_c (pts_ctl_dbg_vars.q_free);

  if (q_msg)
    {
      q_msg->text = text ;
  
      memcpy(&q_msg->data[0] , &data[0] , size)  ;
  
      q_msg->size = size ;

      q_insert (pts_ctl_dbg_vars.q_pending , (Q_Msg *) q_msg) ;
    }
  #endif

}

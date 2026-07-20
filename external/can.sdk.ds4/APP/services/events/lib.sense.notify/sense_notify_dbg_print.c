
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_dbg_print.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include <string.h>
#include "q.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define TASK_SENSE_NOTIFY_DBG_STK   0x120
#define TASK_SENSE_NOTIFY_DBG_NAME  "Sense_notify_Dbg"
#define TASK_SENSE_NOTIFY_DBG_PRI   5

#define SENSE_NOTIFY_DBG_Q_SIZE     15

#define SENSE_NOTIFY_DBG_TEXT_SIZE  50
#define SENSE_NOTIFY_DBG_DATA_SIZE  16

//--------------------------------------------------------------------------//

typedef struct s_Sense_notify_Dbg_Q_Msg
{ 
  Q_Msg         q_base    ;
    
  char  text[SENSE_NOTIFY_DBG_TEXT_SIZE]  ;
  
  UInt8 data[SENSE_NOTIFY_DBG_DATA_SIZE]  ;
  
  UInt8 size    ;

} Sense_notify_Dbg_Q_Msg ;

//--------------------------------------------------------------------------//

typedef struct
{
  Q    q_free               ;
  Q    q_pending            ; 

  Tsk  tsk                  ;       
 
}Sense_notify_Dbg_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/


static void sense_notify_dbg_task(void * arg) ;
 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Sense_notify_Dbg_Vars sense_notify_dbg_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

static void sense_notify_dbg_init(void)
{
  #if DBG_SENSE == 1

  UInt8 n ;
  
  sense_notify_dbg_vars.q_free    = q_create () ;
  sense_notify_dbg_vars.q_pending = q_create () ;

  sense_notify_dbg_vars.tsk = os_tsk_create (sense_notify_dbg_task , TASK_SENSE_NOTIFY_DBG_PRI , TASK_SENSE_NOTIFY_DBG_STK , 0 , TASK_SENSE_NOTIFY_DBG_NAME) ;

  for (n = 0 ; n < SENSE_NOTIFY_DBG_Q_SIZE ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Sense_notify_Dbg_Q_Msg)) ;
      
      q_assign (sense_notify_dbg_vars.q_free , q_msg) ;
      q_insert (sense_notify_dbg_vars.q_free , q_msg) ;
    }
  
  os_tsk_start (sense_notify_dbg_vars.tsk, 0) ;
  
  #endif
}

//--------------------------------------------------------------------------//
static void sense_notify_dbg_task(void * arg)
{
  #if DBG_SENSE == 1

  for(;;)
    {
      
      Sense_notify_Dbg_Q_Msg * q_msg = (Sense_notify_Dbg_Q_Msg *) q_remove (sense_notify_dbg_vars.q_pending, 10) ;
        
      if (q_msg)
        {
          UInt8  i  ;
          
          dbg_tx_request() ;

          if(q_msg->text != NULL)
            {
        	  dbg_tx_text(&q_msg->text[0]) ;
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

void sense_notify_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size)
{  
  #if DBG_SENSE == 1

  Sense_notify_Dbg_Q_Msg * q_msg = (Sense_notify_Dbg_Q_Msg *) q_remove (sense_notify_dbg_vars.q_free , 100);

  if (q_msg)
    {      
      UInt8 char_count = 0;
      
      while(text[char_count++]) ;
      
      if(size < SENSE_NOTIFY_DBG_DATA_SIZE && (char_count < SENSE_NOTIFY_DBG_TEXT_SIZE))
        {
          memcpy(&q_msg->text[0] , text , SENSE_NOTIFY_DBG_TEXT_SIZE)  ;
          
          memcpy(&q_msg->data[0] , &data[0] , size)  ;
      
          q_msg->size = size ;
        }
      else
        {
          StrConst * err_txt = "\n!!!PRINT SIZE ERROR!!!" ;

          memcpy(&q_msg->text[0] , err_txt , SENSE_NOTIFY_DBG_TEXT_SIZE)  ;

          q_msg->size = 0 ;

        }

      q_insert (sense_notify_dbg_vars.q_pending , (Q_Msg *) q_msg) ;

    }
  #endif

}

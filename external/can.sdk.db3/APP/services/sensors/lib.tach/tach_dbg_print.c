
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tach_dbg_print.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "tach_private.h"
#include <string.h>
#include "q.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define TASK_TACH_DBG_STK   0x0200
#define TASK_TACH_DBG_NAME  "Tach_Dbg"
#define TASK_TACH_DBG_PRI   5

#define TACH_DBG_Q_SIZE     15

#define TACH_DBG_TEXT_SIZE  50
#define TACH_DBG_DATA_SIZE  2

//--------------------------------------------------------------------------//

typedef struct s_Tach_Dbg_Q_Msg
{ 
  Q_Msg         q_base    ;
    
  char  text[TACH_DBG_TEXT_SIZE]  ;
  
  UInt8 data[TACH_DBG_DATA_SIZE]  ;
  
  UInt8 size    ;

} Tach_Dbg_Q_Msg ;

//--------------------------------------------------------------------------//

typedef struct
{
  Q    q_free               ;
  Q    q_pending            ; 

  Tsk  tsk                  ;       
 
}Tach_Dbg_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
#if DBG_TACH == 1
static void tach_dbg_init(void)       ;
static void tach_dbg_task(void * arg) ;
#endif 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#if DBG_TACH == 1
static Tach_Dbg_Vars tach_dbg_vars ;
#endif
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#if DBG_TACH == 1
static void tach_dbg_init(void)
{

  UInt8 n ;
  
  tach_dbg_vars.q_free    = q_create () ;
  tach_dbg_vars.q_pending = q_create () ;

  tach_dbg_vars.tsk = os_tsk_create (tach_dbg_task , TASK_TACH_DBG_PRI , TASK_TACH_DBG_STK , 0 , TASK_TACH_DBG_NAME) ;

  for (n = 0 ; n < TACH_DBG_Q_SIZE ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Tach_Dbg_Q_Msg)) ;
      
      q_assign (tach_dbg_vars.q_free , q_msg) ;
      q_insert (tach_dbg_vars.q_free , q_msg) ;
    }
  
  os_tsk_start (tach_dbg_vars.tsk, 0) ;
  
}

//--------------------------------------------------------------------------//
static void tach_dbg_task(void * arg)
{

  for(;;)
    {
      
      Tach_Dbg_Q_Msg * q_msg = (Tach_Dbg_Q_Msg *) q_remove (tach_dbg_vars.q_pending, 10) ;
        
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
  

}

#endif
//--------------------------------------------------------------------------//

void tach_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size)
{  
  #if DBG_TACH == 1

  Tach_Dbg_Q_Msg * q_msg ;
  
  static Boolean do_init = TRUE ;

  if(do_init)
    {
      do_init=FALSE ;

      tach_dbg_init() ;

    }

  q_msg = (Tach_Dbg_Q_Msg *) q_remove (tach_dbg_vars.q_free , 100);

  if (q_msg)
    {      
      UInt8 char_count = 0;
      
      while(text[char_count++]) ;
      
      if(size <= TACH_DBG_DATA_SIZE && (char_count < TACH_DBG_TEXT_SIZE))
        {
          memcpy(&q_msg->text[0] , text , TACH_DBG_TEXT_SIZE)  ;
          
          memcpy(&q_msg->data[0] , &data[0] , size)  ;
      
          q_msg->size = size ;
        }
      else
        {
          StrConst * err_txt = "\n!!!PRINT SIZE ERROR!!!" ;

          memcpy(&q_msg->text[0] , err_txt , TACH_DBG_TEXT_SIZE)  ;

          q_msg->size = 0 ;

        }

      q_insert (tach_dbg_vars.q_pending , (Q_Msg *) q_msg) ;

    }
  #endif

}

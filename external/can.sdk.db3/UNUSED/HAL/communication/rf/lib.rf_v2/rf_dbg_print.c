
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rf_dbg_print.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rf_private.h"
#include <string.h>
#include "q.h"
#include "dbg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define TASK_RF_DBG_STK   0x0200
#define TASK_RF_DBG_NAME  "Rf_Dbg"
#define TASK_RF_DBG_PRI   5

#define RF_DBG_Q_SIZE     15

#define RF_DBG_TEXT_SIZE  50
#define RF_DBG_DATA_SIZE  1

//--------------------------------------------------------------------------//

typedef struct s_Rf_Dbg_Q_Msg
{ 
  Q_Msg         q_base    ;
    
  char  text[RF_DBG_TEXT_SIZE]  ;
  
  UInt8 data[RF_DBG_DATA_SIZE]  ;
  
  UInt8 size    ;

} Rf_Dbg_Q_Msg ;

//--------------------------------------------------------------------------//

typedef struct
{
  Q    q_free               ;
  Q    q_pending            ; 

  Tsk  tsk                  ;       
 
}Rf_Dbg_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rf_dbg_init(void)       ;
static void rf_dbg_task(void * arg) ;
 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Rf_Dbg_Vars rf_dbg_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

static void rf_dbg_init(void)
{
#if DBG_RF == 1
  UInt8 n ;
  
  rf_dbg_vars.q_free    = q_create () ;
  rf_dbg_vars.q_pending = q_create () ;

  rf_dbg_vars.tsk = os_tsk_create (rf_dbg_task , TASK_RF_DBG_PRI , TASK_RF_DBG_STK , 0 , TASK_RF_DBG_NAME) ;

  for (n = 0 ; n < RF_DBG_Q_SIZE ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Rf_Dbg_Q_Msg)) ;
      
      q_assign (rf_dbg_vars.q_free , q_msg) ;
      q_insert (rf_dbg_vars.q_free , q_msg) ;
    }
  
  os_tsk_start (rf_dbg_vars.tsk, 0) ;
#endif
}

//--------------------------------------------------------------------------//
static void rf_dbg_task(void * arg)
{
#if DBG_RF == 1
  for(;;)
    {
      
      Rf_Dbg_Q_Msg * q_msg = (Rf_Dbg_Q_Msg *) q_remove (rf_dbg_vars.q_pending, 10) ;
        
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

void rf_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size)
{
#if DBG_RF == 1
  Rf_Dbg_Q_Msg * q_msg ;
  
  static Boolean do_init = TRUE ;

  if(do_init)
    {
      do_init=FALSE ;

      rf_dbg_init() ;

    }

  q_msg = (Rf_Dbg_Q_Msg *) q_remove (rf_dbg_vars.q_free , 100);

  if (q_msg)
    {      
      UInt8 char_count = 0;
      
      while(text[char_count++]) ;
      
      if(size < RF_DBG_DATA_SIZE && (char_count < RF_DBG_TEXT_SIZE))
        {
          memcpy(&q_msg->text[0] , text , RF_DBG_TEXT_SIZE)  ;
          
          memcpy(&q_msg->data[0] , &data[0] , size)  ;
      
          q_msg->size = size ;
        }
      else
        {
          StrConst * err_txt = "\n!!!PRINT SIZE ERROR!!!" ;

          memcpy(&q_msg->text[0] , err_txt , RF_DBG_TEXT_SIZE)  ;

          q_msg->size = 0 ;

        }

      q_insert (rf_dbg_vars.q_pending , (Q_Msg *) q_msg) ;

    }
#endif
}

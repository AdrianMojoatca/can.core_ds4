
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

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

static void sense_notify_dbg_init(void)
{

}

//--------------------------------------------------------------------------//
static void sense_notify_dbg_task(void * arg)
{

}
//--------------------------------------------------------------------------//

void sense_notify_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size)
{  
  #if DBG_SENSE == 1
     
  TRACE_ZONE
    (
      TRACE("%s ",text); 
      
      if(size)
        {
          UInt8 n ;
          
          for(n = 0 ; n < size ; n++)
            {
                TRACE("%02X" , data[n]);
            }           
        }
    )      

  
//  Sense_notify_Dbg_Q_Msg * q_msg = (Sense_notify_Dbg_Q_Msg *) q_remove (sense_notify_dbg_vars.q_free , 100);
//
//  if (q_msg)
//    {      
//      UInt8 char_count = 0;
//      
//      while(text[char_count++]) ;
//      
//      if(size < SENSE_NOTIFY_DBG_DATA_SIZE && (char_count < SENSE_NOTIFY_DBG_TEXT_SIZE))
//        {
//          memcpy(&q_msg->text[0] , text , SENSE_NOTIFY_DBG_TEXT_SIZE)  ;
//          
//          memcpy(&q_msg->data[0] , &data[0] , size)  ;
//      
//          q_msg->size = size ;
//        }
//      else
//        {
//          StrConst * err_txt = "\n!!!PRINT SIZE ERROR!!!" ;
//
//          memcpy(&q_msg->text[0] , err_txt , SENSE_NOTIFY_DBG_TEXT_SIZE)  ;
//
//          q_msg->size = 0 ;
//
//        }
//
//      q_insert (sense_notify_dbg_vars.q_pending , (Q_Msg *) q_msg) ;
//
//    }
  #endif

}

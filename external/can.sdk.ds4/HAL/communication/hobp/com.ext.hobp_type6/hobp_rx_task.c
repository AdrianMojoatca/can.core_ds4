/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_rx_task.c 44556 2016-11-29 13:03:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_private.h"
#include "wake.h"
#include <string.h>
#include "bitfield.h"
#include "config_gpio.h"
#include "hobp_bus.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define HOBP_IMO_SKIP_COUNT 5

#define BROADCAST_COUNT     5
#define BROADCAST_ERR_MAX   20

typedef void  ( * Hobp_Imo_Callback) (Hobp_Msg msg) ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void ecm_process    (UInt8 data) ;
static void imo_process    (UInt8 data) ;

static void imo_seq        (Hobp_Msg msg);
//static void imo_status1    (Hobp_Msg msg);
//static void imo_status2    (Hobp_Msg msg);

static void imo_first_resp (Hobp_Msg msg);
static void imo_skip_resp  (Hobp_Msg msg);
static void imo_resp       (Hobp_Msg msg);


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Hobp_Imo_Callback hobp_imo_callback  = NULL ;//imo_status1;

static Hobp_Msg          imo_message_filter = {NULL} ; 

static Hobp_Msg          tx_msg ;

static UInt16            broadcast_counter ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void hobp_rx_task (void * tsk_arg)
{
  #if DBG_HOBP == 1
     hobp_dbg_print("\nHobp Rx Start" , NULL , NULL) ;
  #endif

  for(;;)
    {
			static Boolean serprc = TRUE;
			

      int data  = hobp_rx(NULL);

			if(serprc == TRUE)
			{
				//06 40 xx xx
				int tempdata = 0;
				serprc = FALSE;
				serial_process(tempdata);
			}
			
			TRACE("0x%02X",data);

      wake_enter() ;

      //timeout_stop (hobp_vars.tx_timeout)  ;

      switch(data)
        {
          
          case  0x09 : //hobp_set_flag (HOBP_PROG_FLAG_PTS)    ; //hobp_vars.nvfs_entry.is_pts = TRUE   ;
          case  0x06 : serial_process(data)                 ; break ;
          
          case  0x59 : //hobp_set_flag (HOBP_PROG_FLAG_PTS)    ; //hobp_vars.nvfs_entry.is_pts = TRUE   ; 
          case  0x56 : imo_process   (data)                   ; break ;

          case  0x95 : //hobp_set_flag (HOBP_PROG_FLAG_PTS)    ; //hobp_vars.nvfs_entry.is_pts = TRUE   ; 
          case  0x65 : ecm_process   (data)                   ; break ;
        }

      if(broadcast_counter >= BROADCAST_ERR_MAX)
        {
          static Boolean do_log_err = TRUE ;

          if(do_log_err)
            {
              do_log_err = FALSE ;

              if(!hobp_get_flag(HOBP_PROG_FLAG_TABLE1))
                { 
                  hobp_err_add(HOBP_ERR_GET_TABLE1)  ;
                }
              else if(!hobp_get_flag(HOBP_PROG_FLAG_CHALLENGE))
                {
                  hobp_err_add(HOBP_ERR_GET_CHALLENGE)  ;
                }
    //          hobp_bus_dis(NULL) ;
    
              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Err No Response" , NULL ,NULL) ;
              #endif
            }
          

        }
      wake_leave() ;
    }

}
//--------------------------------------------------------------------------//
 void serial_process (UInt8 data) 
{
  //06 40 xx xx

  Hobp_Msg msg ;

  msg.size    = 4   ;
  msg.data[0] = 0x06;
  msg.data[1] = 0x40;
  msg.data[2] = 0xAB;
  msg.data[3] = 0xCD;
    
          if(hobp_vars.gwr && hobp_get_flag(HOBP_PROG_FLAG_SEQ_2))
            {
              msg = hobp_msg[HOBP_MSG_IMO_STATUS2] ;

              msg.data[0] =  hobp_vars.nvfs_entry.is_pts ? 0x59 : 0x56 ;  // hobp_get_flag(HOBP_PROG_FLAG_PTS)

              memcpy(&msg.data[2] ,&hobp_vars.nvfs_entry.status[0] , 4) ;
              
              msg.data[msg.size -1] = hobp_csum(&msg.data[0] , msg.size -1 , NULL)  ;
              
              hobp_tx_broadcast(&msg , 400) ;
            }

          
          memcpy(&hobp_vars.nvfs_entry.serial[0] , &msg.data[2] , 2) ;
         
          hobp_set_flag(HOBP_PROG_FLAG_SERIAL) ;          

          hobp_imo_callback  = imo_seq ;

          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp Serial:     " , &msg.data[2] , 2) ;
          #endif

          if(!hobp_log_is_done())
            {
              initBypassLib();

              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Bypass Lib Init" , NULL ,NULL) ;
              #endif
            }

}
//--------------------------------------------------------------------------//
static void ecm_process   (UInt8 data) 
{  
  static Hobp_Msg msg  ;

  msg.size    = 7    ;
  msg.data[0] = data ;

  if(!hobp_rx_data(&msg.data[1] , msg.size -1)) // remove the id
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Ecm Rx Fail " , NULL , NULL) ;
      #endif
      
      return ;
    }

    #if DBG_HOBP == 1
      hobp_dbg_print("\nHoBp Ecm:        " , &msg.data[2] , 4) ;
    #endif

  if(hobp_csum(&msg.data[0] , msg.size , NULL) != NULL)
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Ecm Cs FAIL "     , NULL , NULL)        ;// 
      #endif

      return ;
    } 
  
  if(!hobp_log_is_done() && (hobp_msg[HOBP_MSG_ECM].data[2] == 0xFF))
    {
      hobp_msg[HOBP_MSG_ECM] = msg ;

      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Ecm ID:     "     , &hobp_msg[HOBP_MSG_ECM].data[2] , 1)        ;// 
      #endif
    }
 
  if(!hobp_get_flag(HOBP_PROG_FLAG_LOG_1))
    {
      memcpy(&hobp_vars.nvfs_entry.challenge[0][0] , &msg.data[3] , 3) ;
    }
  else if(!hobp_get_flag(HOBP_PROG_FLAG_LOG_2))
    {
      memcpy(&hobp_vars.nvfs_entry.challenge[1][0] , &msg.data[3] , 3) ;
    }
  
  
  if(hobp_vars.gwr && hobp_get_flag(HOBP_PROG_FLAG_SEQ_2))
    {
      Hobp_Msg tx_msg = hobp_msg[HOBP_MSG_IMO_STATUS2] ;

      tx_msg.data[0] = hobp_vars.nvfs_entry.is_pts? 0x59 : 0x56 ; //hobp_get_flag(HOBP_PROG_FLAG_PTS)

      tx_msg.data[2] = hobp_vars.nvfs_entry.status[0][0] ;

//      tx_msg.data[2] = hobp_vars.nvfs_entry.response[0][0] ;
      
      algo_Honda_V2  (&msg.data[3]  , &tx_msg.data[3]) ;

      tx_msg.data[msg.size -1] = hobp_csum(&tx_msg.data[0] , tx_msg.size -1 , NULL)  ;

      os_tsk_wait(HOBP_TX_TIMEOUT) ;

      hobp_tx_broadcast(&tx_msg , HOBP_TX_TIMEOUT) ;//hobp_tx (&tx_msg);
    }

  return ;

}
//--------------------------------------------------------------------------//
static void imo_process   (UInt8 data) 
{
  static Hobp_Msg last_msg ;
  
  Hobp_Msg msg  ;
  UInt8    csum ;

  msg.size    = 7    ;
  msg.data[0] = data ;
  
  if(!hobp_rx_data(&msg.data[1] , msg.size -1)) // remove the id
    {      
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Imo Rx Fail " , NULL , NULL) ;
      #endif
      return ;
    }

  csum = hobp_csum(&msg.data[0] , msg.size , NULL) ;

  if(csum != NULL)
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Imo Cs FAIL "     , &msg.data[0] , msg.size)        ;// 
      #endif

      return ;
    } 

  if(memcmp(&msg.data[0] , &last_msg.data[0] , msg.size))
    {
      broadcast_counter = 0 ; 
    } 
    
  broadcast_counter++ ;

  last_msg = msg ;
 
//  #if DBG_HOBP == 1
//    if(broadcast_counter > BROADCAST_COUNT)
//      {
//        hobp_dbg_print("\nCounter:    " , &broadcast_counter , 1) ;
//      }
//    
//  #endif


  if(hobp_imo_callback)
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Imo:        " , &msg.data[2] , 4) ;
      #endif

      hobp_imo_callback(msg) ;
    }
  else
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Callback err" , NULL , NULL) ;
      #endif
    }
    
  return ;  
  
}

//--------------------------------------------------------------------------//
static void imo_seq    (Hobp_Msg msg)
{    
  if((msg.data[3] ^ msg.data[5]) == NULL)
    {
      if((broadcast_counter == 1))
        {
          if(!hobp_get_flag(HOBP_PROG_FLAG_SEQ_1))
            {
              hobp_set_flag(HOBP_PROG_FLAG_SEQ_1)  ;
    
              memcpy(&hobp_vars.nvfs_entry.status[0] , &msg.data[2] , 4) ;
        
              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Seq 1:      ", &msg.data[2] , 4) ;
              #endif
            }
          else if(!hobp_get_flag(HOBP_PROG_FLAG_SEQ_2))
            {
              memcpy(&hobp_vars.nvfs_entry.status[1] , &msg.data[2] , 4) ;
        
              memcpy(&hobp_msg[HOBP_MSG_IMO_STATUS2].data[0] , &msg.data[0] , msg.size) ;
        
              memset(&imo_message_filter , NULL , sizeof(imo_message_filter)) ;
                      
              hobp_set_flag(HOBP_PROG_FLAG_SEQ_2)  ;
        
              hobp_imo_callback  = imo_resp ;
              
              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Seq 2:      ", &msg.data[2] , 4) ;
              #endif
            }
        }
    }
  else
    {
      hobp_imo_callback  = imo_resp ;
    }  

}
//--------------------------------------------------------------------------//
static void imo_resp       (Hobp_Msg msg)
{
  static Boolean is_ready = FALSE ;
  
  if(/*((msg.data[3] ^ msg.data[5]) != NULL) &&*/ !hobp_log_is_done())
    {
      static UInt8 table_status = PROCESS_INIT ;
      
      if(broadcast_counter == 1)
        {
          if(!hobp_get_flag(HOBP_PROG_FLAG_LOG_1))
            {
              memcpy(&hobp_vars.nvfs_entry.challenge[0][0] , &hobp_msg[HOBP_MSG_ECM].data[3] , 3) ;
              memcpy(&hobp_vars.nvfs_entry.response [0][0] , &msg.data[3]                    , 3) ;
            
              hobp_set_flag(HOBP_PROG_FLAG_LOG_1) ;

              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Log 1:      "  , &hobp_vars.nvfs_entry.challenge[0][0] , 3 ) ;
                hobp_dbg_print(" "             , &hobp_vars.nvfs_entry.response [0][0] , 3 ) ;
              #endif

              return ;
            }
          else if(!hobp_get_flag(HOBP_PROG_FLAG_LOG_2) && tx_msg.size != NULL)
            {
              memcpy(&hobp_vars.nvfs_entry.challenge[1][0] , &tx_msg.data[3] , 3) ;
              memcpy(&hobp_vars.nvfs_entry.response [1][0] , &msg.data[3]                    , 3) ;
            
              hobp_set_flag(HOBP_PROG_FLAG_LOG_2) ;

              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Log 2:      " , &hobp_vars.nvfs_entry.challenge[1][0] , 3 ) ;
                hobp_dbg_print(" "              , &hobp_vars.nvfs_entry.response [1][0] , 3 ) ;
              #endif
            }
          
          if(memcmp(&msg.data[3] , &hobp_vars.nvfs_entry.response [0][0] , 3))
            {
              if(!hobp_get_flag(HOBP_PROG_FLAG_TABLE1))
                {
                  #if DBG_HOBP == 1
                    hobp_dbg_print("\nHoBp Table1:     " , &msg.data[3] , 3) ;
                  #endif
                  
                  ATOMIC(table_status = getTable1(&tx_msg.data[3] , &msg.data[3] , &hobp_vars.nvfs_entry.u8Tab1Col1[0] , hobp_vars.nvfs_entry.u8Tab1)) ;

                  tx_msg.data[5] = hobp_vars.nvfs_entry.challenge[0][0] ;
                  tx_msg.data[3] = hobp_vars.nvfs_entry.challenge[0][2] ;

                  
                }
              else if(!hobp_get_flag(HOBP_PROG_FLAG_POLY))
                {
					table_status = getChallenge(&tx_msg.data[3] , &msg.data[3] , hobp_vars.log.item.u8TabResponse) ;
                  
                  #if DBG_HOBP == 1
                    hobp_dbg_print("\nHoBp Challenge:  " , &msg.data[3] , 3) ;
                  #endif
                }
            }
        }
      else if(broadcast_counter == BROADCAST_COUNT)
        {
          if(!is_ready)
            {
              is_ready = TRUE ;

              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Ready" , NULL , NULL) ;
              #endif

              if(!hobp_get_flag(HOBP_PROG_FLAG_TABLE1))
                {
                  #if DBG_HOBP == 1
                    hobp_dbg_print("\nHoBp Table1:     " , &msg.data[3] , 3) ;
                  #endif

                  ATOMIC(table_status = getTable1(&tx_msg.data[3] , &msg.data[3] , &hobp_vars.nvfs_entry.u8Tab1Col1[0] , hobp_vars.nvfs_entry.u8Tab1)) ;

                  tx_msg.data[5] = hobp_vars.nvfs_entry.challenge[0][0] ;
                  tx_msg.data[3] = hobp_vars.nvfs_entry.challenge[0][2] ;
                }

            }
          
        }
//      else
//        {
//          #if DBG_HOBP == 1
//            hobp_dbg_print(" Skipped" , NULL , NULL) ;
//          #endif
//        }

      
      if(table_status == PROCESS_TABLE1_DONE)
        {
          hobp_set_flag(HOBP_PROG_FLAG_TABLE1) ;
          
          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp PROCESS_TABLE1_DONE" , NULL , NULL ) ;
          #endif
          
          if(hobp_is_prog())
            {
              hobp_bus_dis(NULL) ;

              hobp_vars.log_is_done = hobp_save () ;
            }
          else 
            {
							
			  initBypassLib() ;

              table_status = getChallenge(&tx_msg.data[3] , &msg.data[3] , hobp_vars.log.item.u8TabResponse) ;
            }
        }
      if(table_status == PROCESS_CHALLENGE_DONE)
        {
          hobp_set_flag(HOBP_PROG_FLAG_TABLE1) ;
          
          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp PROCESS_CHALLENGE_DONE" , NULL , NULL ) ;
          #endif
          
          if(nvfs_wr(nvfs_hobp_log , &hobp_vars.log.data[0]) == NVFS_OK)
            {
              hobp_set_flag(HOBP_PROG_FLAG_CHALLENGE) ;
          
//              #if DBG_HOBP == 1
//                hobp_dbg_print("\nLog= " , &hobp_vars.log.data[0]  , HOBP_LOG_SIZE ) ;
//              #endif
              
              hobp_bus_dis(NULL) ;

              hobp_vars.log_is_done = hobp_save () ;

            }
          else
            {
              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Write fail" , NULL , NULL) ;
              #endif

            }
        }      
           
      if(table_status == PROCESS_OK)
        {
          #if DBG_HOBP == 1
            hobp_dbg_print(" HoBp PROCESS_OK" , NULL , NULL) ;
          #endif

          tx_msg.size    = 7 ;
          tx_msg.data[0] = hobp_vars.nvfs_entry.is_pts ? 0x95 : 0x65 ; //hobp_get_flag(HOBP_PROG_FLAG_PTS) hobp_vars.nvfs_entry.is_pts? 0x95 : 0x65 ;
          tx_msg.data[1] = 7 ;
          tx_msg.data[2] = hobp_msg[HOBP_MSG_ECM].data[2];//0x39 ;// we need to check the data here
          
          tx_msg.data[tx_msg.size -1] = hobp_csum(&tx_msg.data[0] , tx_msg.size -1 , NULL)  ;

          table_status = PROCESS_TX ;

        }
   
      if (table_status == PROCESS_TX)
        {
          if(tx_msg.size != NULL)
            {
              if(!hobp_get_flag(HOBP_PROG_FLAG_TABLE1))
                {
                  if(broadcast_counter > 1)
                    {
                      tx_msg.data[3] ^= 0xFF ;
                      tx_msg.data[5] ^= 0xFF ;
    
                      tx_msg.data[tx_msg.size -1] = hobp_csum(&tx_msg.data[0] , tx_msg.size -1 , NULL)  ;
                    }

                }
              
              hobp_tx (&tx_msg);
            }
          else
            {
              #if DBG_HOBP == 1
                hobp_dbg_print(" HoBp Skip Response" , NULL , NULL) ;
              #endif
            }
        }

      if(table_status == PROCESS_ERROR) 
        {
          hobp_err_add(HOBP_ERR_PROCESS_TABLE) ;

          #if DBG_HOBP == 1
            hobp_dbg_print(" HoBp PROCESS_ERROR" , NULL , NULL) ;
          #endif
        } 

    }
}

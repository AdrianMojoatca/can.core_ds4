/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_bus_init.c 34683 2015-10-25 16:42:09Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_bus_private.h"
#include "fiq.h"
#include "irq_user.h"
#include "isr.h"
#include "cmd.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DST_BUS_Q_SIZE          5

#define DST_BUS_BAUDRATE        15625


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void dst_bus_init_reader  (void)  ;
static void dst_bus_init_tag     (void)  ;
static void dst_bus_init_cmd     (void)  ;
                              
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/
int dst_bus_init (void)           
{
  #if DBG_DST_BUS == 1
    dst_bus_dbg_print ("\nDst Bus Init", NULL , NULL) ;
  #endif
  TRACE("\r\nDST BUS INIT\r\n");


  if(dst_bus_init_nvfs())
    {
      UInt8 n  ;
    
      dst_bus_vars.q_free    = q_create () ;
      dst_bus_vars.q_pending = q_create () ;
    
      for (n = 0 ; n < DST_BUS_Q_SIZE ; n++)
        {
          Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Dst_Bus_Q_Msg)) ;

          if(!q_msg)
            {
              #if DBG_DST_BUS == 1
                dst_bus_dbg_print ("\nDst Bus Init FAIL", NULL , NULL) ;
              #endif
			  TRACE("\r\nDST BUS INIT FAIL\r\n");
              
              return FALSE ;
            }
         
          q_assign (dst_bus_vars.q_free , q_msg) ;
          q_insert (dst_bus_vars.q_free , q_msg) ;
        }

      dst_bus_init_reader () ;
      dst_bus_init_tag    () ;
      dst_bus_init_cmd    () ;
    
      dst_bus_vars.tsk = os_tsk_create (dst_bus_task , TASK_DST_BUS_PRI , TASK_DST_BUS_STK , 0 , TASK_DST_BUS_NAME) ;
      os_tsk_start (dst_bus_vars.tsk, 0) ;

      irq_user_register(dst_bus_user_service) ;
      fiq_register     (dst_bus_fiq_service ) ;     

      #if DBG_DST_BUS == 1
        dst_bus_dbg_print ("\nDst Bus Init Done", NULL , NULL) ;
      #endif
	  TRACE("DST BUS INIT DONE\r\n");

      return TRUE ;      

    }
  return FALSE ;
} 
//--------------------------------------------------------------------------//

static void dst_bus_init_reader(void)
{
  dst_bus_vars.tmr_reader_rx_timeout = tmr_create();
  if(dst_bus_vars.tmr_reader_rx_timeout == NULL)
    {
      TRACE("\rERROR: dst_bus_init_reader timer creation failed\r");
      return;
    }
  TRACE("\rdst_bus_init_reader timer created\r");
}  
//--------------------------------------------------------------------------//

static void dst_bus_init_tag(void)
{
  dst_bus_vars.tag_baudrate = (CPU_CLOCK_HZ / DST_BUS_BAUDRATE);
  
  dst_bus_vars.tmr_tag_rx = tmr_create();
  if(dst_bus_vars.tmr_tag_rx == NULL)
    {
      TRACE("\rERROR: tmr_tag_rx creation failed\r");
      return;
    }
  TRACE("\rdst_bus_vars.tmr_tag_rx\r");
  
  dst_bus_vars.tmr_tag_tx = tmr_create();
  if(dst_bus_vars.tmr_tag_tx == NULL)
    {
      TRACE("\rERROR: tmr_tag_tx creation failed\r");
      return;
    }
  TRACE("\rdst_bus_vars.tmr_tag_tx\r");
  
  dst_bus_vars.tmr_tag_rx_timeout = tmr_create();
  if(dst_bus_vars.tmr_tag_rx_timeout == NULL)
    {
      TRACE("\rERROR: tmr_tag_rx_timeout creation failed\r");
      return;
    }
  TRACE("\rtmr_tag_rx_timeout\r");
}
//--------------------------------------------------------------------------//
static void dst_bus_init_cmd(void)
{
  static const Cmd_Array1 cmd_array[] = 
    {
      {DST_BUS_CMD_DIS         , dst_bus_dis     }, 
      {DST_BUS_CMD_ENA         , dst_bus_ena     }, 
    #ifdef DST_BUS_CMD_ON  
      {DST_BUS_CMD_ON          , dst_bus_ign_on  },
    #endif  
    #ifdef DST_BUS_CMD_OFF
      {DST_BUS_CMD_OFF         , dst_bus_ign_off },
	   
    #endif
    #ifdef DST_BUS_CMD_RS_ON
	  {DST_BUS_CMD_RS_ON       , dst_bus_ign_on  },  
    #endif
    #ifdef DST_BUS_CMD_RS_OFF
      {DST_BUS_CMD_RS_OFF      , dst_bus_ign_off },  
    #endif      
    } ;

  CMD_LUT_INSERT1_RAM(cmd_array,EXEC_MODE_NORMAL);
}


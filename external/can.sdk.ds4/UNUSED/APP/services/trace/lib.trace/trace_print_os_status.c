/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: trace_print_os_status.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "trace_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

#if USE_TRACE == 1

static void         trace_print_mem_info  (void)    ;
static void         trace_print_stk_info  (Tsk tsk) ;
static void         trace_print_tsk_info  (Tsk tsk) ;

static void         trace_print_tsk_state (TskState tsk_state) ;

static void         trace_print_crlf      (void)    ;
static void         trace_print_header    (void)    ;


static UInt32       trace_get_heap_size   (void)    ; 

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void trace_print_os_status (void)
{
  Tsk tsk ;
  trace_print_crlf     ()           ;
  trace_print_header   ()           ;
  trace_print_stk_info (0)          ;
  t_printf         ("   (SYS)") ;
  trace_print_crlf     ()           ;

  for (os_tsk_list_init (& tsk) ; tsk ; os_tsk_list_next (& tsk))
    {
      trace_print_stk_info (tsk) ;
      trace_print_tsk_info (tsk) ;      
      trace_print_crlf     ()    ;
    }

  trace_print_mem_info () ;

		#ifdef DEV_LPC_236x

  		t_printf ("\r\n\nDev ID = %08X  Boot Code Ver = %04X\r\n\n" , iap_rd_device_id () , iap_rd_boot_code_ver ()) ;
		
		#endif

}

//--------------------------------------------------------------------------//

static void trace_print_mem_info (void)
{
  MemInfo mem ;

  os_mem_info (&mem) ;

  t_printf
    (
      "\r\n[%08X %08X %08X] %s <%010u> (heap used %02d%%) " ,
      mem.addr_min  ,
      mem.addr_next ,
      mem.addr_max  ,
      mem.addr_inc == +1 ? "++" : "--" ,

      ((UInt32) mem.addr_max  - (UInt32) mem.addr_next) + 1 ,

      100 - ((mem.heap_remaining*100)/trace_get_heap_size()) 
    ) ;
}

//--------------------------------------------------------------------------//

static void trace_print_stk_info (Tsk tsk)
{
  StkInfo stk_info ;

  os_stk_info (tsk , &stk_info) ;

  t_printf
    (
      "%c%08X %c%08X %04X   %02d%% " ,
      stk_info.fence_ok_beg ? '+' : '-' , (UInt32) stk_info.stk_beg     ,
      stk_info.fence_ok_end ? '+' : '-' , (UInt32) stk_info.stk_end     ,
      stk_info.free_mem                                                 ,
      100-((stk_info.free_mem * 100)/(stk_info.stk_end-stk_info.stk_beg))
    ) ;
}

//--------------------------------------------------------------------------//

static void trace_print_tsk_info (Tsk tsk)
{
  TskInfo tsk_info ;

  os_tsk_info (tsk , &tsk_info) ;

  t_printf ("   %05u   " , tsk_info.tsk_priority) ;

  trace_print_tsk_state  (tsk_info.tsk_state)    ;

  t_printf ("%s"   , tsk_info.tsk_name)    ;
}

//--------------------------------------------------------------------------//

static void trace_print_tsk_state (TskState tsk_state)
{
  struct s_Map
    {
      TskState     tsk_state  ;

      const char * state_name ;

	  const char * space;

    } ;

  static const struct s_Map map [] =
    {
      #define ENTRY(z,y) { OS_TSK_##z , "-" #z "-",#y} ,

      ENTRY (ABORTED,---------------)           
      ENTRY (RUNNING,---------------)           
      ENTRY (STOPPED,---------------)           
      ENTRY (WAITING,---------------)           
      ENTRY (ELIGIBLE,--------------)          
      ENTRY (SUSPENDED,-------------)         
            
      ENTRY (WAIT_MBX,--------------)
      ENTRY (WAIT_IRQ,--------------)
      ENTRY (WAIT_EVT,--------------)
      ENTRY (WAIT_PTE,--------------)
      ENTRY (WAIT_GTE,--------------)
      ENTRY (WAIT_RES,--------------)
            
      ENTRY (WAIT_FLG_SET,----------)
      ENTRY (WAIT_FLG_CLR,----------)
            
      ENTRY (WAIT_PIP_INS,----------)
      ENTRY (WAIT_PIP_REM,----------)
            
      ENTRY (WAIT_MSG_SEND,---------)
      ENTRY (WAIT_MSG_RCEV,---------)
      ENTRY (WAIT_MSG_RESP_PASS,----)
            
      ENTRY (WAIT_TFR_SEND,---------)
      ENTRY (WAIT_TFR_RCEV,---------)
            
      ENTRY (WAIT_TSK_STOP,---------)
    
      #undef  ENTRY
    } ;

  UInt16 index = 0 ;

  const char * state = "-undefined-" ;
  const char * space = "   ";

  do
    {
      if (map[index].tsk_state == tsk_state)
        {
          state = map[index].state_name ;
		  space = map[index].space ;

          break ;
        }
    }
  while (++index < sizeof_array (map)) ;

  t_printf ("%s%s" ,state,space) ;  
}

//--------------------------------------------------------------------------//

static void trace_print_crlf (void)
{
  t_printf ("\r\n") ;
}

//--------------------------------------------------------------------------//

static void trace_print_header( void )
{
    const char *head_line = "------------------------------------------------------------------------------\n\r" ;
    const char *head_txt  = "   beg       end    free  used   tsk pri   tsk state              tsk name\n\r" ;
    
    t_printf(head_line);t_printf(head_txt);t_printf(head_line);           
}

//--------------------------------------------------------------------------//

UInt32  trace_get_heap_size( void )
{    
    #define HEAP_BASE_VAR(_region_)       extern unsigned Image$$ER_OS_HEAP_##_region_##$$ZI$$Base
    #define HEAP_BASE(_region_)          ((unsigned)(&    Image$$ER_OS_HEAP_##_region_##$$ZI$$Base))
    
    #define HEAP_LIMIT_VAR(_region_)      extern unsigned Image$$ER_OS_HEAP_##_region_##$$ZI$$Limit
    #define HEAP_LIMIT(_region_)          ((unsigned)(&   Image$$ER_OS_HEAP_##_region_##$$ZI$$Limit))

    
    UInt32 heap_size = 0 ; 
    
    heap_region_0:
    {    
        #if OS_NUM_HEAP_REGIONS >= 1
            HEAP_BASE_VAR(0);
            HEAP_LIMIT_VAR(0);

            heap_size += (HEAP_LIMIT(0)- HEAP_BASE(0));
        #endif
    }

    heap_region_1:
    {    
        #if OS_NUM_HEAP_REGIONS >= 2
            HEAP_BASE_VAR(1);
            HEAP_LIMIT_VAR(1);

            heap_size += (HEAP_LIMIT(1)- HEAP_BASE(1));
        #endif
    }

    heap_region_2:
    {    
        #if OS_NUM_HEAP_REGIONS >= 3
            HEAP_BASE_VAR(2);
            HEAP_LIMIT_VAR(2);

            heap_size += (HEAP_LIMIT(2)- HEAP_BASE(2));
        #endif
    }

    heap_region_3:
    {    
        #if OS_NUM_HEAP_REGIONS >= 4
            HEAP_BASE_VAR(3);
            HEAP_LIMIT_VAR(3);

            heap_size += (HEAP_LIMIT(3)- HEAP_BASE(3));
        #endif
    }

    return heap_size ; 

    #undef HEAP_BASE_VAR
    #undef HEAP_BASE
    
    #undef HEAP_LIMIT_VAR
    #undef HEAP_LIMIT
}

//--------------------------------------------------------------------------//

#endif


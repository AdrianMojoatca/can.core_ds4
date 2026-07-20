/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: core.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/



#include "device.h"
#include "core.h"

#include "dbg.h"
#include "trace.h"
#include "sio_uart.h"
#include "isr.h"

#include "w2w.h"
#include "wake.h"
#include "gpio.h"
#include "d2d.h"

#ifdef DEV_LPC_176x
    #include "d2di.h"
#endif

#include "display.h"

#include "reset.h"
#include "tmr.h"
#include "prog.h"
#include "cmd.h"  
#include "exec.h"
#include "tach.h"
#include "wdog_v.h"
#include "bus_status.h"
#include "status_req.h"
#include "rf.h"
#include "rf_loop.h"
#include "rftd2.h"
#include "ctl_dl.h"
#include "detect.h"
#include "secure_lock.h"
#include "per_exec.h"
#include "gpio_wake.h"
#include "led_usr.h"

#include "main.c            "

#include "task_main.c"

#ifdef DEV_LPC_236x
  #include "xk401_io_init.c   "
  #include "nvram_usr.h"
#endif 

#include "trace.c"
#include "sio.c"
#include "sio_uart.c"


#include "core_contract_db3_firmware_config.h"
#include "core_contract_db3_gpio_config.h"
#include "core_contract_db3_dbg_config.h"

 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef CMD_BUFFER_MAX_SIZE
  #define CMD_BUFFER_MAX_SIZE         8
#endif
#ifndef CMD_EXEC_Q_MSG_MAX_SIZE
  #define CMD_EXEC_Q_MSG_MAX_SIZE     15
#endif 

#ifndef EXEC_TASK_COUNT
  #define EXEC_TASK_COUNT             3
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean initialize_nvfs_fw      (void) ;

#ifdef DEV_LPC_236x
static Boolean initialize_nvfs_fw_23xx (void) ;
#endif

#ifdef DEV_LPC_176x
static Boolean initialize_nvfs_fw_17xx (void) ;
#endif

static void core_init_reset  (void) ;
static void core_debug_init  (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
void core_init( void )
{
	Sio sio = sio_uart_create(
    SIO_UART_3,
    SIO_UART_TX_3_P4_28,
    SIO_UART_RX_3_P4_29,
    ISR_PRI_1,
    SIO_UART_CCLK_DIV_1,
    39
);

    nvfs_usr_init_entry_names_from_contract();

	TRACE_INIT(sio);
	core_contract_db3_dbg_config_init_defaults();  /* �ncarca valorile default */
	set_use_trace(1);      /* activeaza TRACE */


	if (!initialize_nvfs_fw ())
    {
        while (1) ;
    }


    // IMPORTANT: if you want to use the virtual watchdog RESET-ON-SLEEP feature, it must
    //            be initialized *before* any calls to wake_register(), in order to be called
    //            *after* all the registered functions.
    wdog_v_init_reset_on_sleep() ;
    
    per_exec_init() ;
	
	//wake_init    () ;

    core_debug_init();
	wake_init    () ;

	TRACE("\r\nCORE INIT START\r\n");
    
    wake_register (nvram_usr_sleep_commit) ;// create a function call to commit all changes done
                                            // to nvram ram buffer when cpu goes to sleep mode


    //TRACE("\r\nDO THE WAKE ENTER\r\n");
	wake_enter   () ;

	
		#ifdef DEV_LPC_236x
			#if DBG_JTAG == 1
				#warning "!!! Sleep is Disable !!!"
				wake_enter () ;   
			#endif
		#endif


    tmr_init     () ;

	wdog_v_init  () ;


    display_init () ; 


    gpio_wake_init();  
    gpio_init    () ;

    core_init_reset   () ;
    // EVERY INIT FUNCTION REGISTERING A RESET FUNCTION SHOULD BE BEFORE THIS LINE
    reset_process   () ; // process the reset if push is pressed

    cmd_init        (CMD_BUFFER_MAX_SIZE, CMD_EXEC_Q_MSG_MAX_SIZE) ;
    exec_init       (EXEC_TASK_COUNT);

    
    #ifdef DEV_LPC_176x
        if(!d2di_init())       
        {
          d2d_init        () ;
        }
    #else
       d2d_init        () ;
    #endif 

    tach_init       () ;    
    bus_status_init () ;     
    status_req_init () ;
    prog_init       () ; 
    rf_init         () ; 
    rf_loop_init    () ;
    ctl_dl_init     () ;
    secure_lock_init() ;
    
	
    led_usr_init    () ;
    
    prog_dis        () ;  // disable prog during module programming

	TRACE("\r\n START SEARCHING\r\n");

	//return; // test point 1

	/*
	can_usr_init(CAN0_ENABLE|CAN0_RX_BUFFER_ENABLE|CAN0_TX_BUFFER_ENABLE);
	can_per_init (8);    
    can_sync_init(CAN_DEV_0 , 16 );
	sense_init(CAN_DEV_0);
	*/

	/*
	can_usr_init(CAN0_ENABLE | CAN0_RX_BUFFER_ENABLE | CAN0_TX_BUFFER_ENABLE);
	can_per_init(8);
	can_sync_init(CAN_DEV_0, 16);
	*/

	d2d_init        () ;
	 
    //toki01_detect_car_search();
	 //that's detect init
	  
      TRACE("\r\nDETECT INIT FINISHED\r\n");

    w2w_init        () ;  // must be done last 
	
	
	//while(bmw_detect_done () ) ; 

	TRACE("\r\nAFTER DETECT IS DONE\r\n");
    
    status_req_obdcan_check () ; // display smartstart obd can not active led pattern 

    prog_ena        () ;
}

/*--------------------------------------------------------------------------*/

static Boolean initialize_nvfs_fw (void)
{
  #ifdef DEV_LPC_236x
    return initialize_nvfs_fw_23xx() ;
  #endif
  
  #ifdef DEV_LPC_176x
    return initialize_nvfs_fw_17xx() ;
  #endif  
}
/*--------------------------------------------------------------------------*/
#ifdef DEV_LPC_236x
static Boolean initialize_nvfs_fw_23xx (void)
{
  switch (nvfs_init (NVFS_BASE , NVFS_SIZE , FALSE))
    {
      case NVFS_OK :  return (TRUE) ;
    } 

#if DBG_JTAG == 1
  switch (nvfs_init (NVFS_BASE , NVFS_SIZE , TRUE))
    {
      case NVFS_OK :  return (TRUE) ; 
    }
#endif

return FALSE ;  
}
#endif
/*--------------------------------------------------------------------------*/
#ifdef DEV_LPC_176x
static Boolean initialize_nvfs_fw_17xx (void)
{
  switch (nvfs_init (NVFS_MOUNT_DEFAULT))
    {
      case NVFS_OK :  return (TRUE) ;
    } 

#if DBG_JTAG == 1
  switch (nvfs_init (NVFS_MOUNT_DEBUG)) 
    {
      case NVFS_OK :  return (TRUE) ; 
    }
#endif

return FALSE ;  
}
#endif
/*--------------------------------------------------------------------------*/
static void core_init_reset(void)
{
     /* place here the reset functions for digital firmware */
	//detect_init_reset  () ;
	//toki01_detect_reset_init();
	d2d_init_reset     () ;
	w2w_init_reset     () ;  
	prog_init_reset    () ;
	status_req_init_reset () ;
	rftd2_init_reset   () ;

	fw_init_reset      () ;
}
/*--------------------------------------------------------------------------*/
void core_debug_init( void )
{
	#if (USE_TRACE == 1)

    TRACE_ZONE
    (
			Sio sio = sio_uart_create ( SIO_UART_3 , SIO_UART_TX_3_P4_28  , SIO_UART_RX_3_P4_29  , ISR_PRI_1 , SIO_UART_CCLK_DIV_1 , 39) ; // 115200 8N1 
			
			TRACE_INIT( sio ) ;  
    )
    
    //TRACE("\nDebug Enable for %s %s\n",FW_NAME , FW_VERSION);
	TRACE("\nDebug Enable for %s %s\n",get_fw_name() , get_app_version());
	#endif
} 

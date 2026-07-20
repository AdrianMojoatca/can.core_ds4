/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: task_main.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "core.h"
#include "fw.h"
#include "wake.h"
#include "dbg.h"
#include "fiq.h"
#include "irq_user.h"

#include "core_contract_db3_gpio_output.h"
#include "core_contract_db3_misc_gpio.h"
#include "core_contract_db3_boot_handshake.h"

#ifdef OUT_1
#undef OUT_1
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static int get_cmd (void) ;
void gpio_iface_doors_open (void);
void gpio_iface_doors_closed (void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Tsk tsk_main ;

static UInt8 * const RAM_MEM = (UInt8 *) 0x7FE00000 ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void task_main (void * tsk_arg)
{   
	UInt8 i;
	core_db3_boot_handshake_result_t abi_handshake_result;

	#ifdef DEV_LPC_176x
	
	// Timer 1
		PCONP |= (1u << 2) ;		// POWER the TIMER

	PCLKSEL0 &= ~(3u << 4) ;
	PCLKSEL0 |=  (1u << 4) ;    // Divide CPU clock by 1

	
	// Timer 2
	PCONP |= (1u << 22) ;		// POWER the TIMER

	PCLKSEL1 &= ~(3u << 12) ;
	PCLKSEL1 |=  (1u << 12) ;    // Divide CPU clock by 1
	
	
	// Timer 3
		PCONP |= (1u << 23) ;		// POWER the TIMER

	PCLKSEL1 &= ~(3u << 14) ;
	PCLKSEL1 |=  (1u << 14) ;    // Divide CPU clock by 1

	core_contract_db3_dac_power_set(FALSE);

	#endif

	fw_identity_init() ;	// Vehicle-specific contract identity init (must run before core_init)

	core_init 	() ;		// Platform core init
	TRACE("\r\n[FW_TRACE] after core_init\r\n");
	dbg_tx_text("\r\n[FW_TRACE] after core_init\r\n");
	TRACE("\r\n[FW_TRACE] before abi_handshake_validate\r\n");
	dbg_tx_text("\r\n[FW_TRACE] before abi_handshake_validate\r\n");

	abi_handshake_result = core_contract_db3_boot_handshake_validate(
		CORE_DB3_ABI_VERSION_MINOR,
		CORE_DB3_CAPABILITY_DEFAULT_MASK,
		0);
	TRACE("\r\n[FW_TRACE] after abi_handshake_validate\r\n");
	dbg_tx_text("\r\n[FW_TRACE] after abi_handshake_validate\r\n");

	if (abi_handshake_result != CORE_DB3_BOOT_HANDSHAKE_OK)
	{
		TRACE("\r\n[FW_TRACE] abi_handshake FAIL\r\n");
		dbg_tx_text("\r\n[FW_TRACE] abi_handshake FAIL code=");
		dbg_tx_u32((UInt32)abi_handshake_result);
		dbg_tx_crlf();
		core_contract_db3_gpio_led_red_on();
		while (1)
		{
			os_tsk_wait(1000);
		}
	}
	else
	{
		TRACE("\r\n[FW_TRACE] abi_handshake OK\r\n");
		dbg_tx_text("\r\n[FW_TRACE] abi_handshake OK code=");
		dbg_tx_u32((UInt32)abi_handshake_result);
		dbg_tx_crlf();
	}

	/* a small test for the IO's */
	for ( i = 0; i < 3; i++ )
	{
		/* asa nu - GPIO_IFACE_OUTPUT_NEG_OUT1 poate fi inlocuit cu alt OUT de catre programatorul "outside core" */

		/*core_contract_db3_gpio_output_on(GPIO_IFACE_CFG_DOORS_OUTPUT);	 // asa merge
		os_tsk_wait(500);
		core_contract_db3_gpio_output_off(GPIO_IFACE_CFG_DOORS_OUTPUT);
		os_tsk_wait(500);*/
		
		/* asa da - programatorul "outside core" nu poate schimba out-ul pt. doors */
		 
		gpio_iface_doors_open();
		os_tsk_wait(500);
		gpio_iface_doors_closed();
		os_tsk_wait(500);
	}
	/* end small test */

#ifdef DEV_LPC_176x
  	fiq_init();
	irq_user_init();
#endif

	TRACE("\r\n[FW_TRACE] before fw_init\r\n");
	dbg_tx_text("\r\n[FW_TRACE] before fw_init\r\n");
	fw_init 	() ;		// Platform & fw specific init
	TRACE("\r\n[FW_TRACE] after fw_init\r\n");
	dbg_tx_text("\r\n[FW_TRACE] after fw_init\r\n");

	os_tsk_wait (3000) ; 	// wait for LED to turn OFF before going to Sleep
	TRACE("\r\n[FW_TRACE] before debug_or_sleep\r\n");
	dbg_tx_text("\r\n[FW_TRACE] before debug_or_sleep\r\n");
	
    #if (DEBUG_TEXT == 1) || USE_TRACE == 1
    #warning "Fw Debug Enable"
    
    while( 1 )
      {
        TRACE_RX();
      }
    
    #else
      wake_leave () ;
    #endif	   	
}	


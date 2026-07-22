/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: isr_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr_private.h"
#include "wdog.h"
#include "933_gpio.h"   /* DS5 BRING-UP: GPIO_PL_LED_GREEN for the fault-code blinker */
#include "gpio_pl.h"    /* DS5 BRING-UP: gpio_pl_pin_write */
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void vector_default (void) ;
static void reset_vector_init (void);
void vector_default_c (unsigned int *frame);   /* DS5 BRING-UP: fault PC classifier */

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
volatile UInt32 hard_fault_dump[2] = {0};
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void isr_init (void)
{
  /// Initialize all RAM vectors to point to default handler ///
  ATOMIC_ALL
    (
      UInt8 index = 0 ;

      do
        {
          nvic_vector_table [index] = (UInt32)vector_default ;
        }
      while (++index < sizeof_array (nvic_vector_table)) ;
			
			reset_vector_init();

      SCB->VTOR = (UInt32) nvic_vector_table ;
    )
}

//--------------------------------------------------------------------------//

/* DS5 BRING-UP: capture the exception stack frame (MSP/PSP per EXC_RETURN bit2)
   and pass it to the C classifier. REVERT (restore the simple reset handler) before release. */
static __asm void vector_default (void)
{
	IMPORT  vector_default_c
	TST     lr, #4
	ITE     EQ
	MRSEQ   r0, MSP
	MRSNE   r0, PSP
	B       vector_default_c
}

/* Classify the faulting PC (frame[6] = stacked PC = the bad target for INVSTATE)
   and blink a small code on the GREEN LED (count the blinks per burst):
     1 = PC in CORE code (0x8100..0x37000)
     2 = PC in ABI/table/fill region (0x37000..0x38000)  -> jumped into table/0xFF fill
     3 = PC in FW code (0x38000..0x40000)
     4 = PC == 0 (null fn-ptr)
     5 = PC elsewhere (RAM / garbage) */
void vector_default_c (unsigned int *frame)
{
	volatile unsigned int pc = frame[6];
	volatile unsigned int code, i, d;

	hard_fault_dump[0] = pc;
	hard_fault_dump[1] = SCB->CFSR;

	if      (pc == 0u)                                   code = 4u;
	else if (pc >= 0x00008100u && pc <  0x00037000u)     code = 1u;
	else if (pc >= 0x00037000u && pc <  0x00038000u)     code = 2u;
	else if (pc >= 0x00038000u && pc <  0x00040000u)     code = 3u;
	else                                                 code = 5u;

	for(;;)
	{
		for(i = 0u; i < code; i++)
		{
			gpio_pl_pin_write(GPIO_PL_LED_GREEN, 0);   /* on  (active-low) */
			for(d = 0u; d < 10000000u; d++) { }
			gpio_pl_pin_write(GPIO_PL_LED_GREEN, 1);   /* off */
			for(d = 0u; d < 10000000u; d++) { }
		}
		for(d = 0u; d < 60000000u; d++) { }            /* long gap between bursts */
	}
}

//--------------------------------------------------------------------------//

static void reset_vector_init (void)
{
	UInt32 *addr;
	
	addr = (UInt32*) MEMMAP;
	*addr = 0x01;			// User Mode
	
	addr = (UInt32 *)(0x00000000);
	nvic_vector_table[0] = *addr;

	addr = (UInt32 *)(0x00000004);
	nvic_vector_table[1] = *addr;
}

//--------------------------------------------------------------------------//


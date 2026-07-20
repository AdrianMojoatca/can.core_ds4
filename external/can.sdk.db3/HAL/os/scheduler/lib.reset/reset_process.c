/*==========================================================================*/
// $Id: reset_process.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "reset_private.h"
#include "gpio.h"
#include "core_contract_db3_gpio_input.h"
#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif

void gpio_iface_led_red_on(void);
void gpio_iface_led_red_off(void);
void gpio_iface_led_green_on(void);
void gpio_iface_led_green_off(void);

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean check_pushbutton_pressed(UInt8 seconds);
static void complete_reset(void);
static void led_red(void);
static void led_orange(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void reset_process (void)
{
    if (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN))                // push not press
        return ;

    if (!check_pushbutton_pressed(2)) return;
    
    reset_service () ;          // do reset everything

    nvram_usr_commit () ;       // commit in NVRAM

    led_orange();

    if (!check_pushbutton_pressed(12)) 
    {
      complete_reset();
      return;
    }

    reset_long_service () ;     // do hard reset

    nvram_usr_commit () ;       // commit in NVRAM

    while (!core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN))            // while push is press, wait here
    {
      led_orange();
      os_tsk_wait(333);
      if (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN)) break;
      led_red();
      os_tsk_wait(333);
    }

    complete_reset();
}

//--------------------------------------------------------------------------//

static Boolean check_pushbutton_pressed(UInt8 seconds)
{
  UInt16 i ;
  for (i = 0 ; i < seconds * 100 ; i++)
  {
    if (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN)) return FALSE;
    os_tsk_wait (10) ;
  }
  return TRUE;
}

//--------------------------------------------------------------------------//

static void complete_reset(void)
{
  gpio_iface_led_green_off();
  gpio_iface_led_red_off();
  os_tsk_wait (1000) ;
}

//--------------------------------------------------------------------------//

static void led_red(void)
{
  gpio_iface_led_green_off();
  gpio_iface_led_red_on();
}

//--------------------------------------------------------------------------//

static void led_orange(void)
{
  gpio_iface_led_green_on();
  gpio_iface_led_red_on();
}


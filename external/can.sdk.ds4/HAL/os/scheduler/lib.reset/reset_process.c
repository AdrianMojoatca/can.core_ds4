/*==========================================================================*/
// $Id: reset_process.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "reset_private.h"
#include "config_gpio.h"
#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif

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
    if (PUSH_IN)                // push not press
        return ;

    if (!check_pushbutton_pressed(2)) return;
    
    reset_service () ;          // do reset everything

    nvram_usr_commit () ;       // commit in NVRAM

    led_orange();

#ifdef DEV_LPC_236x          //CM800
    if (!check_pushbutton_pressed(12)) 
#else
		if (!check_pushbutton_pressed(4)) 
#endif		
    {
      complete_reset();
      return;
    }

    reset_long_service () ;     // do hard reset

    nvram_usr_commit () ;       // commit in NVRAM

    while (!PUSH_IN)            // while push is press, wait here
    {
      led_orange();
      os_tsk_wait(333);
      if (PUSH_IN) break;
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
    if (PUSH_IN) return FALSE;
    os_tsk_wait (10) ;
  }
  return TRUE;
}

//--------------------------------------------------------------------------//

static void complete_reset(void)
{
  OUT_LED_GREEN( FALSE ) ;
  OUT_LED_RED( FALSE ) ;
  os_tsk_wait (1000) ;
}

//--------------------------------------------------------------------------//

static void led_red(void)
{
  OUT_LED_GREEN( FALSE ) ;
  OUT_LED_RED( TRUE ) ;
}

//--------------------------------------------------------------------------//

static void led_orange(void)
{
  OUT_LED_GREEN( TRUE ) ;
  OUT_LED_RED( TRUE ) ;
}


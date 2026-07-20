/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_task.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <setjmp.h>
#include "display_private.h"
#include "wdog_v.h"

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

static void wait_command          (Display_Cmd * display_cmd_local) ;
static void display_wait          (jmp_buf vec_update , TskTimeout timeout) ;
static void service_display_once  (jmp_buf vec_update , Display_Cmd_Once * display_cmd_once) ;
static void service_display_prog  (jmp_buf vec_update , Display_Cmd_Prog * display_cmd_prog) ;
static void service_display_error (jmp_buf vec_update , Display_Cmd_Error * display_cmd_error) ;
static void service_display_flash (jmp_buf vec_update , Display_Cmd_Flash * display_cmd_flash) ;

static void led_red_on    (void) ;
static void led_red_off   (void) ;
static void led_green_on  (void) ;
static void led_green_off (void) ;
static void led_all_on    (void) ;
static void led_all_off   (void) ;


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void display_task (void * tsk_arg)
{ 
  for (;;)
    {
      jmp_buf vec_update ;

      while (setjmp (vec_update) != 0)
        ;

      for (;;)
        {
          Display_Cmd  display_cmd_current ;
    
          wait_command (& display_cmd_current) ;
    
		  // Do not be tempted to place a VIRTUAL_WATCH here - service_display_flash() can potentially loop for many seconds
		  //VIRTUAL_WATCH
		  //(
	          switch (display_cmd_current.id)
	            {
				  case DISPLAY_ONCE  : service_display_once  (vec_update , & display_cmd_current.arg.cmd_once)  ; break ;
	
	              case DISPLAY_PROG  : service_display_prog  (vec_update , & display_cmd_current.arg.cmd_prog)  ; break ;
	    
	              case DISPLAY_ERROR : service_display_error (vec_update , & display_cmd_current.arg.cmd_error) ; break ;
	
				  case DISPLAY_FLASH : service_display_flash (vec_update , & display_cmd_current.arg.cmd_flash) ; break ;
	            }
		  //);
        }
    }
}

//--------------------------------------------------------------------------//

static void wait_command (Display_Cmd * display_cmd_local)
{
  CRITICAL ;

  led_all_off () ;

  C_ENTER ;
    {
      os_irq_wait (display_irq_update , FALSE , 0) ;

      *display_cmd_local = display_cmd ;
    }
  C_LEAVE ;
}

//--------------------------------------------------------------------------//

static void service_display_once (jmp_buf vec_update , Display_Cmd_Once * display_cmd_once)
{
  switch (display_cmd_once->type)
    {
	  case BOTH_LED  : led_all_on   () ; break ;
	  case GREEN_LED : led_green_on () ; break ;
	  case RED_LED   : led_red_on   () ; break ;
	}

  display_wait (vec_update , display_cmd_once->timeout) ;

}

//--------------------------------------------------------------------------//

static void service_display_prog (jmp_buf vec_update , Display_Cmd_Prog * display_cmd_prog)
{
  #define GREEN_ON_TIME    400
  #define GREEN_OFF_TIME   300
  #define GREEN_BREAK_TIME 400
  #define RED_ON_TIME	   100
  #define RED_OFF_TIME	   300
  #define RED_BREAK_TIME   400

  UInt8 feature_count ;
  UInt8  option_count ;
  
  if (!display_cmd_prog->feature_change)
    goto state_option ;	// option change so start flashing state option
  
  for (;;)
    {
      /*state_feature :*/
        {
		  feature_count = display_cmd_prog->feature ;

		  led_all_off () ;
	      display_wait (vec_update , GREEN_BREAK_TIME) ;
		  do
		    {
			  led_green_on () ;
	          display_wait (vec_update , GREEN_ON_TIME)    ;
			  led_green_off () ;
	          display_wait (vec_update , GREEN_OFF_TIME)   ;  
			}
		  while (--feature_count > 0) ;
        }

      state_option :
        {
		  option_count = display_cmd_prog->option ;

		  led_all_off () ;
	      display_wait (vec_update , RED_BREAK_TIME) ;
          do
		    {	  
			  led_red_on () ;
	          display_wait (vec_update , RED_ON_TIME)    ;
			  led_red_off () ;
	          display_wait (vec_update , RED_OFF_TIME)   ;  
			}
		  while (--option_count > 0) ;
        }
    }
}

//--------------------------------------------------------------------------//

static void service_display_error (jmp_buf vec_update , Display_Cmd_Error * display_cmd_error)
{
#define ERROR_ON_TIME	   200
#define ERROR_OFF_TIME	   400
#define ERROR_PAUSE_TIME   800

  Display_Cmd_Error local_display_cmd_error = *display_cmd_error;

  for( ; local_display_cmd_error.display_count != 0 ; local_display_cmd_error.display_count --)
    {
	  led_all_off () ;
	  display_wait (vec_update , ERROR_PAUSE_TIME)   ;	  	  
	  do
	  {
		 led_red_on () ;
         display_wait (vec_update , ERROR_ON_TIME)    ;
         led_red_off () ;
         display_wait (vec_update , ERROR_OFF_TIME)   ;
	  }while(--local_display_cmd_error.err_code != 0) ;

	  local_display_cmd_error.err_code = display_cmd_error->err_code;

	}

}

//--------------------------------------------------------------------------//

static void service_display_flash (jmp_buf vec_update , Display_Cmd_Flash * display_cmd_flash)
{
  while (display_cmd_flash->count-- > 0)
    {
  	  switch (display_cmd_flash->type)
        {
           case BOTH_LED  : led_all_on   () ; break ;
           case GREEN_LED : led_green_on () ; break ;
           case RED_LED   : led_red_on   () ; break ;
        }
      display_wait (vec_update , display_cmd_flash->time_on) ;

	  switch (display_cmd_flash->type)
        {
           case BOTH_LED  : led_all_off   () ; break ;
           case GREEN_LED : led_green_off () ; break ;
           case RED_LED   : led_red_off   () ; break ;
        }
      display_wait (vec_update , display_cmd_flash->time_off) ;  
    } 
}
//--------------------------------------------------------------------------//

static void display_wait (jmp_buf vec_update , TskTimeout timeout)
{
  if (os_irq_wait (display_irq_update , FALSE , timeout))
    {
	  os_irq_set (display_irq_update) ;	// need this otherwise wait_command is waiting for its irq flag too
      longjmp (vec_update , 1) ;
    }
}

//--------------------------------------------------------------------------//

static void led_red_on (void)
{
  gpio_iface_led_red_on();
}
//--------------------------------------------------------------------------//

static void led_red_off (void)
{
  gpio_iface_led_red_off();
}
//--------------------------------------------------------------------------//

static void led_green_on (void)
{
  gpio_iface_led_green_on();
}
//--------------------------------------------------------------------------//

static void led_green_off (void)
{
  gpio_iface_led_green_off();
}
//--------------------------------------------------------------------------//

static void led_all_on (void)
{
  gpio_iface_led_green_on();
  gpio_iface_led_red_on();
}
//--------------------------------------------------------------------------//

static void led_all_off (void)
{
  gpio_iface_led_green_off();
  gpio_iface_led_red_off();
}



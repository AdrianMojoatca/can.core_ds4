/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_private.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __DISPLAY_PRIVATE_H__
#define __DISPLAY_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "display.h"
#include "timeout_f.h"
#include "gpio.h"
#include "core_contract_db3_gpio_output.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DISPLAY_TIMEOUT    1000 //default timeout

// <h>  Task 'display' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_DISPLAY_NAME  "display"
#define TASK_DISPLAY_PRI   100
#define TASK_DISPLAY_STK   0x0200

//--------------------------------------------------------------------------//

typedef enum  { DISPLAY_ONCE , DISPLAY_PROG , DISPLAY_ERROR , DISPLAY_FLASH } Display_Id ;

typedef struct s_Display_Cmd_Once
{
  Display_Type	type    ;
  UInt32            timeout ;
 
} Display_Cmd_Once ;

//--------------------------------------------------------------------------//

typedef struct s_Display_Cmd_Prog
{
  UInt8   feature        ;
  UInt8   option         ;
  Boolean feature_change ;	// feature change = TRUE, option change = FALSE

} Display_Cmd_Prog ;

//--------------------------------------------------------------------------//

typedef struct s_Display_Cmd_Error
{
  UInt16 err_code      ;
  UInt8  display_count ;

} Display_Cmd_Error    ;

//--------------------------------------------------------------------------//

typedef struct s_Display_Cmd_Flash
{
  Display_Type type ;
  UInt16 count      ;
  UInt16 time_on    ;    // t_on  or first slow
  UInt16 time_off   ;    // t_off or others following first slow
 
} Display_Cmd_Flash ;

//--------------------------------------------------------------------------//

typedef struct s_Display_Cmd
{
  Display_Id id ;

  union
    {
	  Display_Cmd_Once  cmd_once  ;
      Display_Cmd_Prog  cmd_prog  ;
	  Display_Cmd_Error cmd_error ;
	  Display_Cmd_Flash cmd_flash ;
    } arg ;

} Display_Cmd ;

//--------------------------------------------------------------------------//

typedef enum     
{
    EFFECT_OVERLAP      ,         
    EFFECT_FLASH        ,
    EFFECT_FIRST_SLOW   ,

}Display_Effect ; 

//--------------------------------------------------------------------------//

typedef struct s_Display_Pattern
{
  Display_Name   name        ;
  Display_Effect effect      ;
  UInt16         time_on     ;
  UInt16         time_off    ;
 
} Display_Pattern ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void display_task           (void * tsk_arg               ) ;
void display_pass_cmd       (Display_Cmd * display_cmd_new) ;
void display_pattern_update (void * arg) ;

void display_wake_enter     (void) ;
void display_wake_leave     (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern          Tsk             tsk_display        ;

extern          Irq             display_irq_update ;
extern volatile Display_Cmd     display_cmd        ;
extern          Timeout         display_timeout    ;

extern const    Display_Pattern pattern_table[];

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



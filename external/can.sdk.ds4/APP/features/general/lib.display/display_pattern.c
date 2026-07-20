/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_pattern.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "display_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DISPLAY_PTRN_MAX    8

typedef struct s_Display_Pattern_Exec
{
  Display_Type   pattern[DISPLAY_PTRN_MAX] ;
  UInt8          curr     ; 
  UInt8          lenght   ;
  UInt16         count    ;
  Display_Name   name     ;
  
}Display_Pattern_Exec ;

#define EFFECT   pattern_table[display_ptrn.name].effect 
#define TIME_ON  pattern_table[display_ptrn.name].time_on
#define TIME_OFF pattern_table[display_ptrn.name].time_off

#define FIRST_SLOW  TIME_ON  
#define OTHERS      TIME_OFF 
 
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Display_Pattern_Exec display_ptrn ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void display_pattern (Display_Type type_str[] , Display_Name name , UInt8 str_lenght,  UInt16 flash_count)
{	
    if( str_lenght <= DISPLAY_PTRN_MAX)
    {
        memcpy(&display_ptrn.pattern[0] , &type_str[0] , str_lenght) ;

        display_ptrn.lenght   = str_lenght     ;
        display_ptrn.name     = name           ;
        display_ptrn.curr     = 0              ;

        if (flash_count)  // when 0, flash forever
        {
            display_ptrn.count    = flash_count    ;
        }
        else
        {
            display_ptrn.count    = 0xFFFF     ;
        }
    
        display_pattern_update (0) ;
    }
}

//--------------------------------------------------------------------------//

void display_pattern_update (void * arg)
{	
    if(display_ptrn.curr < display_ptrn.lenght)
    {
        if(EFFECT == EFFECT_FIRST_SLOW)
        {
            if(display_ptrn.curr == 0)
            {
                display_once(display_ptrn.pattern[display_ptrn.curr++], FIRST_SLOW) ;
                timeout_update (display_timeout, FIRST_SLOW + (2*OTHERS)) ;
            }
            else
            {
                display_once(display_ptrn.pattern[display_ptrn.curr++], OTHERS) ;
                timeout_update (display_timeout, 2 * OTHERS) ;
            }
        }
        else
        {
            display_once(display_ptrn.pattern[display_ptrn.curr++], TIME_ON) ;
            timeout_update (display_timeout, TIME_ON + TIME_OFF) ;
        }      
    }
    else
    {
        display_ptrn.curr = 0 ;

        if(display_ptrn.count != 0xFFFF)
        {
         display_ptrn.count-- ;    
        }
        timeout_update (display_timeout, 2 * TIME_OFF) ;
    }

    if(display_ptrn.count)
    {    
        timeout_start  (display_timeout) ;	
    }
}


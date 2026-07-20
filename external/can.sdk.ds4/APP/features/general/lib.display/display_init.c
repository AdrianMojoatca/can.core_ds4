/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "display_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/** lib.display will only be a wrapper functionality 
   this library will use lib.led in order to display the LED pattrens requested 
   display_init is an empty api and does nothing
   */
void display_init (void)
{	
    display_timeout = timeout_f_create(DISPLAY_TIMEOUT , display_pattern_update)  ;  

/* display is used only as a wrapper function for lib.led from svn.common */
/* init function is defnied only to support the interface and hence has no actual functionality defnied */

}


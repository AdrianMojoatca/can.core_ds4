/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: display.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
 #include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define PATTERN_MAX_LENGHT      8

typedef enum  { BOTH_LED , GREEN_LED , RED_LED} Display_Type ;

typedef enum      // order must match pattern_table
{
    DISPLAY_100MS_OVERLAP      ,  // 100ms color change without off pause
    DISPLAY_200MS_FIRST_SLOW   ,  // 800ms first flash followed by all other at 200ms    
    DISPLAY_300MS_FLASH        ,  // 300ms flash with a pause at the end of the pattern
    DISPLAY_600MS_FLASH        ,  // 600ms flash with a pause at the end of the pattern
    DISPLAY_600MS_OVERLAP      ,  // 600ms color change without off pause
    DISPLAY_3SEC_FIRST_SLOW    ,  // 3sec first flash followed by all other at 200ms    

}Display_Name ; 


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void display_init    (void) ;
void display_once    (Display_Type type , UInt32 timeout) ;
void display_prog    (UInt8 feature , UInt8 option , Boolean feature_change) ;
void display_error   (UInt16 err_code , UInt8 display_count) ;
void display_flash   (Display_Type type , UInt16 flash_count , UInt16 flash_time_on , UInt16 flash_time_off) ;
void display_pattern (Display_Type type_str[] , Display_Name name , UInt8 str_lenght,  UInt16 flash_count) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



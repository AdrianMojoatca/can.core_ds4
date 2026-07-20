/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __VIN_PRIVATE_H__
#define __VIN_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "vin.h"
#include "nvfs_usr.h"

#include <string.h>  

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define VIN_ERR_BUFF_SIZE  5 
#define VIN_SAVE_SIZE      5 

//--------------------------------------------------------------------------//

typedef struct
{
  struct
    {
      Vin code              ;
      unsigned is_valid : 1 ;

    } vin ;

  struct
    {
      Vin_Car_Type type     ;
      unsigned is_valid : 1 ;

    } car_type ;

} Vin_Info ;

//--------------------------------------------------------------------------//

typedef struct
{
    const char * const vin_def ;
    Vin_Car_Type const vin_car_type ;
 
} Vin_Table_Entry ;

//--------------------------------------------------------------------------//

typedef enum
{
   NO_VIN_ERR          ,

   ERR_VIN_VALUE       ,
   ERR_VIN_CHECK_DIGIT ,


   ERR_VIN_COUNT 

}Vin_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Vin_Err
{

  UInt16         count                    ;

  Vin_Error_Code code [VIN_ERR_BUFF_SIZE] ;
  
}Vin_Error ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern       Vin_Info        vin_info         ;
extern       Vin_Error       vin_err          ;

extern const Vin_Table_Entry vin_table     [] ;
extern const UInt8           VIN_TABLE_SIZE   ;

extern const NVFS_Entry_Name nvfs_vin_error   ;


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean     vin_init_nvfs   (void) ;

void        vin_reset       (void) ;
void        vin_read        (void) ;
void        vin_write       (void) ;
Boolean     vin_err_save    (void) ;

void        vin_err_add     (Vin_Error_Code code)                   ;

SInt8       vin_check_digit (const char vin_digit)                  ;

Vin_Status  vin_match       (const char vin_def [] , Vin const vin) ;

/*==========================================================================*/

#endif



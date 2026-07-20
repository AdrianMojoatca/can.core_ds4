/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_lut_sw_insert_entry.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_lut_sw_private.h"

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

Can_Lut_Sw_Status can_lut_sw_insert_entry (Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry * const entry)
{
  Exception exception ;

  /// Check if the insertion will be valid \\\

  if (setjmp (exception.throw_vec) == 0)
    {
      can_lut_sw_check_if_valid_entry    (& exception , entry) ;
    
      can_lut_sw_check_if_table_conflict (& exception , can_lut_sw , entry) ;
    
      can_lut_sw_check_if_table_full     (& exception , can_lut_sw) ;
    }
  else
    {
      return exception.status ;
    }

  /// Insertion will be valid, so do it \\\

  can_lut_sw_table_insert (can_lut_sw , entry) ;
  
  return CAN_LUT_SW_OK ;
}


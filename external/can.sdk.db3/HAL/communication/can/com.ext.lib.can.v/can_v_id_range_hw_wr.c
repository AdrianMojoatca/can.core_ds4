/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_id_range_hw_wr.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_private.h"

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

HW_Commit_Status can_v_id_range_hw_wr_std_single_c (const Can_Dev_Id can_dev_id , ID_Range * const id_range)
{
  if (id_range->min == id_range->max)
    {
      return can_lut_insert_id_std (can_dev_id , (UInt16) id_range->min) ? HW_LUT_COMMITTED : HW_LUT_OVERFLOW ;
    }

  return HW_LUT_SKIPPED ;
}

//--------------------------------------------------------------------------//

HW_Commit_Status can_v_id_range_hw_wr_std_group_c  (const Can_Dev_Id can_dev_id , ID_Range * const id_range)
{
  if (id_range->min != id_range->max)
    {
      return can_lut_insert_id_std_grp (can_dev_id , (UInt16) id_range->min , (UInt16) id_range->max) ? HW_LUT_COMMITTED : HW_LUT_OVERFLOW ;
    }

  return HW_LUT_SKIPPED ;
}

//--------------------------------------------------------------------------//

HW_Commit_Status can_v_id_range_hw_wr_ext_single_c (const Can_Dev_Id can_dev_id , ID_Range * const id_range)
{
  if (id_range->min == id_range->max)
    {
      return can_lut_insert_id_ext (can_dev_id , (UInt32) id_range->min) ? HW_LUT_COMMITTED : HW_LUT_OVERFLOW ;
    }

  return HW_LUT_SKIPPED ;
}

//--------------------------------------------------------------------------//

HW_Commit_Status can_v_id_range_hw_wr_ext_group_c  (const Can_Dev_Id can_dev_id , ID_Range * const id_range)
{
  if (id_range->min != id_range->max)
    {
      return can_lut_insert_id_ext_grp (can_dev_id , (UInt32) id_range->min , (UInt32) id_range->max) ? HW_LUT_COMMITTED : HW_LUT_OVERFLOW ;
    }

  return HW_LUT_SKIPPED ;
}


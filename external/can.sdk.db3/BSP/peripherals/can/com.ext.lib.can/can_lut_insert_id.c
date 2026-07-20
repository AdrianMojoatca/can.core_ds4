/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_lut_insert_id.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define IS_ODD(z) ((z) & 1U)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean can_lut_insert_id_std (Can_Dev_Id can_dev_id , UInt16 id_std)
{
  if (IS_ODD (can_lut_counters.std) || CAN_EOT < HW_LUT_TABLE_OFFSET_MAX)
    {
      UInt16 index_32 = CAN_SFF_GRP_SA >> 2 ;
    
      if (IS_ODD (can_lut_counters.std))
        {
          index_32 -= 1 ;
    
          can_lut_wr_std   (can_dev_id , index_32 , id_std , 1) ;
        }
      else
        {
          can_lut_wr_std   (can_dev_id , index_32 , id_std , 0) ; 
    
          can_lut_wr_std_x (can_dev_id , index_32) ;
          
          CAN_SFF_GRP_SA += 4 ;
    
          CAN_EFF_SA     = CAN_SFF_GRP_SA ;
          CAN_EFF_GRP_SA = CAN_SFF_GRP_SA ;
          CAN_EOT        = CAN_SFF_GRP_SA ;
        }
    
      can_lut_counters.std ++ ;

      return TRUE ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

Boolean can_lut_insert_id_std_grp (Can_Dev_Id can_dev_id , UInt16 id_std_min , UInt16 id_std_max)
{                
  if (CAN_EOT < HW_LUT_TABLE_OFFSET_MAX)
    {
      UInt16 index_32 = CAN_EFF_SA >> 2 ;
    
      can_lut_wr_std (can_dev_id , index_32 , id_std_min , 0) ;
      can_lut_wr_std (can_dev_id , index_32 , id_std_max , 1) ;
    
      CAN_EFF_SA += 4 ;
    
      CAN_EFF_GRP_SA = CAN_EFF_SA ;
      CAN_EOT        = CAN_EFF_SA ;

      can_lut_counters.std_grp ++ ;

      return TRUE ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

Boolean can_lut_insert_id_ext (Can_Dev_Id can_dev_id , UInt32 id_ext)
{
  if (CAN_EOT < HW_LUT_TABLE_OFFSET_MAX)
    {
      UInt16 index_32 = CAN_EFF_GRP_SA >> 2 ;
    
      can_lut_wr_ext (can_dev_id , index_32 , id_ext) ;
    
      CAN_EFF_GRP_SA += 4 ;
    
      CAN_EOT         = CAN_EFF_GRP_SA ;

      can_lut_counters.ext ++ ;

      return TRUE ;
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

Boolean can_lut_insert_id_ext_grp (Can_Dev_Id can_dev_id , UInt32 id_ext_min , UInt32 id_ext_max)
{
  if (CAN_EOT < HW_LUT_TABLE_OFFSET_MAX - 4U)
    {
      UInt16 index_32 = CAN_EOT >> 2 ;
    
      can_lut_wr_ext (can_dev_id , index_32 + 0 , id_ext_min) ;
      can_lut_wr_ext (can_dev_id , index_32 + 1 , id_ext_max) ;
    
      CAN_EOT += 8 ;

      can_lut_counters.ext_grp ++ ;

      return TRUE ;
    }

  return FALSE ;
}


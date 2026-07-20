/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_rx_lut_map_sw_to_hw.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can.h"
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

Boolean can_v_rx_lut_map_sw_to_hw (void)
{
  typedef struct s_Class_Rule
    {
      ID_Class_Extract id_class_extract ;

      ID_Set_HW_Commit id_range_hw_commit [NUM_RANGE_TYPES] ;

    } Class_Rule ;

  static const Class_Rule class_rule [NUM_CLASS_TYPES] =
    {
      can_v_lut_extract_class_std , { can_v_id_range_hw_wr_std_single_c , can_v_id_range_hw_wr_std_group_c } ,
      can_v_lut_extract_class_ext , { can_v_id_range_hw_wr_ext_single_c , can_v_id_range_hw_wr_ext_group_c } ,
    } ;

  Boolean lut_wr_status ;

		can_dev_ena(CAN_DEV_0);
  can_lut_accept_dis () ;
    {
      UInt8 id_class = 0 ;

      can_lut_reset  () ;
    
      do
        {
          ID_Set * id_set_combined [CAN_DEV_COUNT] ;

          can_v_id_class_combine   (id_set_combined , class_rule[id_class].id_class_extract) ;
      
          can_v_id_class_wr_hw_lut (id_set_combined , class_rule[id_class].id_range_hw_commit , & lut_wr_status) ;
        }
      while (lut_wr_status && ++id_class < NUM_CLASS_TYPES) ;
    }
  can_lut_accept_ena () ;

  return lut_wr_status ;
}


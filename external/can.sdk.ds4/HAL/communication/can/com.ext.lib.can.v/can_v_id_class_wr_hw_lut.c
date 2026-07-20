/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_id_class_wr_hw_lut.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void combined_sets_operator (ID_Set * const id_set_combined [] , void (* id_set_operator)(ID_Set *)) ;

static void combined_sets_write    (ID_Set * const id_set_combined [] , const ID_Set_HW_Commit id_set_hw_commit [] , Boolean * const lut_wr_status) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_v_id_class_wr_hw_lut (ID_Set * id_set_combined [] , const ID_Set_HW_Commit id_set_hw_commit [] , Boolean * const lut_wr_status)
{
  combined_sets_operator (id_set_combined , can_v_id_set_sort               ) ;
  combined_sets_write    (id_set_combined , id_set_hw_commit , lut_wr_status) ;
  combined_sets_operator (id_set_combined , can_v_id_set_delete             ) ;
}

//--------------------------------------------------------------------------//

static void combined_sets_operator (ID_Set * const id_set_combined [] , void (* id_set_operator)(ID_Set *))
{
#ifdef DEV_LPC_176x
  Can_Dev_Id can_dev_id = CAN_DEV_1 ;
#else
  Can_Dev_Id can_dev_id = CAN_DEV_0 ;
#endif

  do
    {
      id_set_operator (id_set_combined [can_dev_id]) ;
    }
  while (++can_dev_id < CAN_DEV_COUNT) ;
}

//--------------------------------------------------------------------------//

static void combined_sets_write (ID_Set * const id_set_combined [] , const ID_Set_HW_Commit id_set_hw_commit [] , Boolean * const lut_wr_status)
{
  UInt8 range_type = 0 ;

  *lut_wr_status = TRUE ;

  do
    {
			#ifdef DEV_LPC_176x
				Can_Dev_Id can_dev_id = CAN_DEV_1 ;
			#else
				Can_Dev_Id can_dev_id = CAN_DEV_0 ;
			#endif

      do
        {
          ID_Range      * curr ;
          ID_Set_Iterator iter ;
          
          can_v_id_set_iterator_init (& iter , & curr , id_set_combined [can_dev_id] , ITER_L2R) ;

          while (curr)
            {
              ID_Range * next ;

              can_v_id_set_iterator_next (& iter , & next) ;

              switch (id_set_hw_commit [range_type] (can_dev_id , curr))
                {
                  case HW_LUT_SKIPPED :
                    {
                      break ;
                    }

                  case HW_LUT_COMMITTED :
                    {
                      can_v_vars.init.can_v_table_size ++ ;
    
                      can_v_id_range_unlink (curr) ;
    
                      can_v_id_range_delete (curr) ;

                      break ;
                    }

                  default :
                    {
                      *lut_wr_status = FALSE ;

                      return ;
                    }
                }

              curr = next ;
            }
        }
      while (++can_dev_id < CAN_DEV_COUNT) ;
    }
  while (++range_type < NUM_RANGE_TYPES) ;
}


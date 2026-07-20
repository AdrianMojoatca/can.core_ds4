/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_id_class_combine.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef ID_Set * (* ID_Set_Operator)(ID_Set * , ID_Set *) ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static ID_Set * id_set_combine        (void) ;

static void     perform_intersections (const UInt32 set_select_mask , ID_Set ** const set_lhs) ;
static void     perform_subtractions  (const UInt32 set_select_mask , ID_Set ** const set_lhs) ;

static void     execute_set_operator  (ID_Set ** const set_lhs , ID_Set * const set_rhs , ID_Set_Operator const id_set_operator) ;

static ID_Set * fetch_set             (UInt8 index)     ;
static UInt32   fetch_max_val         (UInt8 bit_count) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_v_id_class_combine (ID_Set * id_set_combined [] , ID_Class_Extract const id_class_extract)
{
#ifdef DEV_LPC_176x
	Can_Dev_Id can_dev_id = CAN_DEV_1 ;
#else
	Can_Dev_Id can_dev_id = CAN_DEV_0 ;
#endif

  do
    {
      can_v_id_class_create (can_dev_id , id_class_extract) ;

      id_set_combined [can_dev_id] = id_set_combine () ;

      can_v_id_class_delete () ;
    }
  while (++can_dev_id < CAN_DEV_COUNT) ;
}

//--------------------------------------------------------------------------//

static ID_Set * id_set_combine (void)
{
  ID_Set * set_combined = can_v_id_set_create () ;

  if (can_v_vars.init.can_v_rx_count > 0)
    {
      UInt32 set_select_mask = fetch_max_val (can_v_vars.init.can_v_rx_count) ;
                   
      do
        {
          ID_Set * set_acc ;

          perform_intersections   (set_select_mask , & set_acc) ;

          perform_subtractions    (set_select_mask , & set_acc) ;

          can_v_id_set_append_rhs (set_combined    ,   set_acc) ;

          can_v_id_set_delete     (set_acc) ;
        }
      while (--set_select_mask > 0) ;
    }

  return set_combined ;
}

//--------------------------------------------------------------------------//

static void perform_intersections (const UInt32 set_select_mask , ID_Set ** const set_lhs)
{
  SInt8 index ;

  Combo_Iterator iter ;

  *set_lhs = NULL ;

  for (can_v_combo_iterator_init (& iter , & index , COMBO_MODE_1 , can_v_vars.init.can_v_rx_count , set_select_mask) ; index >= 0 ; can_v_combo_iterator_next (& iter , & index))
    {
      ID_Set * set_rhs = fetch_set (index) ;

      if (!*set_lhs)
        {
           *set_lhs = can_v_id_set_copy (set_rhs) ;
        }
      else
        {
          execute_set_operator (set_lhs , set_rhs , can_v_id_set_intersect) ;
        }
    }
}

//--------------------------------------------------------------------------//

static void perform_subtractions (const UInt32 set_select_mask , ID_Set ** const set_lhs)
{
  SInt8 index ;

  Combo_Iterator iter ;

  for (can_v_combo_iterator_init (& iter , & index , COMBO_MODE_0 , can_v_vars.init.can_v_rx_count , set_select_mask) ; index >= 0 ; can_v_combo_iterator_next (& iter , & index))
    {
      ID_Set * set_rhs = fetch_set (index) ;

      execute_set_operator (set_lhs , set_rhs , can_v_id_set_subtract) ;
    }
}

//--------------------------------------------------------------------------//

static void execute_set_operator (ID_Set ** const set_lhs , ID_Set * const set_rhs , ID_Set_Operator const id_set_operator)
{
  ID_Set * set_res = id_set_operator (*set_lhs , set_rhs) ;

  can_v_id_set_delete (*set_lhs) ;

  *set_lhs = set_res ;
}

//--------------------------------------------------------------------------//

static ID_Set * fetch_set (UInt8 index)
{
  Can_V_Rx curr ;

  for (curr = can_v_vars.init.can_v_rx_list ; curr ; curr = curr->obj.init.next)
    {
      if (index-- == 0)
        {
          return curr->obj.init.id_set ;
        }
    }

  return NULL ;
}

//--------------------------------------------------------------------------//

static UInt32 fetch_max_val (UInt8 bit_count)
{
  return bit_count < 32 ? 0xFFFFFFFF >> 32 - bit_count : 0xFFFFFFFF ; 
}


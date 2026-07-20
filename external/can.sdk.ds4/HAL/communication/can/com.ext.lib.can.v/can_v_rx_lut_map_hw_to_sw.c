/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_rx_lut_map_hw_to_sw.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static Lut_Map * build_rx_lut_map_list (const UInt16 index) ;

static SInt16    find_lut_index        (const Can_Lut_Definition * const lut_hw_def , const Can_Lut_Sw * const can_lut_sw) ;

static Boolean   check_if_match        (const Can_Lut_Definition * const lut_def , const Can_Lut_Definition * const can_def) ;

static Boolean   compare_ss            (const Can_Lut_Single * const lut_entry , const Can_Lut_Single * const can_entry) ;
static Boolean   compare_gs            (const Can_Lut_Group  * const lut_entry , const Can_Lut_Single * const can_entry) ;
static Boolean   compare_gg            (const Can_Lut_Group  * const lut_entry , const Can_Lut_Group  * const can_entry) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Lut_Map ** can_v_rx_lut_map_hw_to_sw (void)
{
  Lut_Map ** lut_map_table = NULL ;

  #define TABLE_SIZE can_v_vars.init.can_v_table_size

  if (TABLE_SIZE > 0)
    {
      UInt16 index  = 0 ;

      lut_map_table = (Lut_Map **) os_mem_request (TABLE_SIZE * sizeof (Lut_Map *)) ;

      do
        {
          lut_map_table [index] = build_rx_lut_map_list (index) ;
        }
      while (++index < TABLE_SIZE) ;
    }

  #undef TABLE_SIZE

  return lut_map_table ;
}

//--------------------------------------------------------------------------//

static Lut_Map * build_rx_lut_map_list (const UInt16 index)
{
  Can_V_Rx  can_v_rx ;

  Lut_Map * lut_last = NULL ;
  Lut_Map * lut_list = NULL ;

  Can_Lut_Definition lut_def ;

  can_lut_read (index , & lut_def) ;

  for (can_v_rx = can_v_vars.init.can_v_rx_list ; can_v_rx ; can_v_rx = can_v_rx->obj.init.next)
    {
      SInt16 lut_index = find_lut_index (& lut_def , can_v_rx->obj.init.can_lut_sw) ;

      if (lut_index >= 0)
        {
          Lut_Map * lut_map = can_v_mem_request_lut_map () ;

          lut_map->lut_index = (UInt16) lut_index ;
          lut_map->can_v_rx  =          can_v_rx  ;
          lut_map->next      = NULL               ;

          if (lut_list)
            {
              lut_last->next = lut_map ;
            }
          else
            {
              lut_list = lut_map ;
            }

          lut_last = lut_map ;
        }
    }

  return lut_list ;
}

//--------------------------------------------------------------------------//

static SInt16 find_lut_index (const Can_Lut_Definition * const lut_hw_def , const Can_Lut_Sw * const can_lut_sw)
{
  SInt16 lut_index ;

  const Can_Lut_Sw_Entry * lut_entry ;

  for (lut_index = 0 , can_lut_sw_iterator_init (can_lut_sw , & lut_entry) ; lut_entry ; can_lut_sw_iterator_next (can_lut_sw , & lut_entry) , lut_index++)
    {
      if (check_if_match (& lut_entry->def , lut_hw_def))
        {
          return lut_index ;
        }
    }

  return -1 ;
}

//--------------------------------------------------------------------------//

static Boolean check_if_match (const Can_Lut_Definition * const lut_def , const Can_Lut_Definition * const can_def)
{
  if (lut_def->dev_id == can_def->dev_id)
    {
      #define PAIR(z1,z2) (((UInt16)(z1) << 8) | (UInt16)(z2))

      switch (PAIR (lut_def->type , can_def->type))
        {
          case PAIR (CAN_LUT_STD       , CAN_LUT_STD      ) : 
          case PAIR (CAN_LUT_EXT       , CAN_LUT_EXT      ) : return compare_ss (& lut_def->ident.single , & can_def->ident.single) ;

          case PAIR (CAN_LUT_STD_GROUP , CAN_LUT_STD      ) : 
          case PAIR (CAN_LUT_EXT_GROUP , CAN_LUT_EXT      ) : return compare_gs (& lut_def->ident.group  , & can_def->ident.single) ;  

          case PAIR (CAN_LUT_STD_GROUP , CAN_LUT_STD_GROUP) :
          case PAIR (CAN_LUT_EXT_GROUP , CAN_LUT_EXT_GROUP) : return compare_gg (& lut_def->ident.group  , & can_def->ident.group ) ;
        }

      #undef  PAIR
    }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static Boolean compare_ss (const Can_Lut_Single * const lut_entry , const Can_Lut_Single * const can_entry)
{
  return lut_entry->val == can_entry->val ;
}

//--------------------------------------------------------------------------//

static Boolean compare_gs (const Can_Lut_Group * const lut_entry , const Can_Lut_Single * const can_entry)
{
  return lut_entry->min <= can_entry->val && can_entry->val <= lut_entry->max ;
}

//--------------------------------------------------------------------------//

static Boolean compare_gg (const Can_Lut_Group * const lut_entry , const Can_Lut_Group * const can_entry)
{
  return lut_entry->min <= can_entry->min && can_entry->max <= lut_entry->max ;
}


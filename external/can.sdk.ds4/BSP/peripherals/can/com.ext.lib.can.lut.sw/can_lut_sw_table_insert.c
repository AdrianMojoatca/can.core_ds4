/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_lut_sw_table_insert.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static UInt16  find_insert_point (Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry * const entry) ;

static void    shift_table_right (Can_Lut_Sw * const can_lut_sw , UInt16 const insert_point) ;

static Boolean compare_lt        (const Can_Lut_Sw_Entry * const lhs , const Can_Lut_Sw_Entry * const rhs) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_lut_sw_table_insert (Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry * const entry)
{
  UInt16 insert_point = find_insert_point (can_lut_sw , entry) ;

  can_lut_sw->num_entries ++ ;

  shift_table_right (can_lut_sw , insert_point) ;

  can_lut_sw->table [insert_point] = *entry ;
}

//--------------------------------------------------------------------------//

static UInt16 find_insert_point (Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry * const entry)
{
  UInt16 insert_point ;

  for (insert_point = 0 ; insert_point < can_lut_sw->num_entries ; insert_point++)
    {
      if (compare_lt (entry , can_lut_sw->table + insert_point))
        {
          break ;
        }
    }

  return insert_point ;
}

//--------------------------------------------------------------------------//

static void shift_table_right (Can_Lut_Sw * const can_lut_sw , UInt16 const insert_point)
{
  if (can_lut_sw->num_entries - insert_point >= 2)
    {
      UInt16 index = can_lut_sw->num_entries - 1 ;

      do
        {
          can_lut_sw->table [index] = can_lut_sw->table [index - 1] ;
        }
      while (--index > insert_point) ;
    }
}

//--------------------------------------------------------------------------//

static Boolean compare_lt (const Can_Lut_Sw_Entry * const lhs , const Can_Lut_Sw_Entry * const rhs)
{
  if (lhs->def.type != rhs->def.type)
    {
      return lhs->def.type < rhs->def.type ;
    }

  if (lhs->def.dev_id != rhs->def.dev_id)
    {
      return lhs->def.dev_id < rhs->def.dev_id ;
    }

  switch (lhs->def.type)
    {
      /// Compare single ident case \\\

      case CAN_LUT_STD :
      case CAN_LUT_EXT : return lhs->def.ident.single.val < rhs->def.ident.single.val ;

      /// Compare group  ident case \\\

      default             : return lhs->def.ident.group.min  < rhs->def.ident.group.min  ; 
    }
}


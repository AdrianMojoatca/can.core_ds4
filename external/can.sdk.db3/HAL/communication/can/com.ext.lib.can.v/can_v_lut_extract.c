/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_lut_extract.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static ID_Range * create_group  (const Can_Lut_Group  * const group ) ;
static ID_Range * create_single (const Can_Lut_Single * const single) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

ID_Set * can_v_lut_extract_class (const Can_Dev_Id can_dev_id , const Can_Lut_Sw * const can_lut_sw , const ID_Class_Extract id_class_extract)
{
  const Can_Lut_Sw_Entry * lut_entry ;

  ID_Set * id_set = can_v_id_set_create () ;

  for (can_lut_sw_iterator_init (can_lut_sw , & lut_entry) ; lut_entry ; can_lut_sw_iterator_next (can_lut_sw , & lut_entry))
    {
      ID_Range * id_range = id_class_extract (can_dev_id , lut_entry) ;

      if (id_range)
        {
          can_v_id_set_insert_rhs (id_set , id_range) ;
        }
    }

  return id_set ;
}

//--------------------------------------------------------------------------//

ID_Range * can_v_lut_extract_class_std (const Can_Dev_Id can_dev_id , const Can_Lut_Sw_Entry * const can_lut_sw_entry)
{
  if (can_lut_sw_entry->def.dev_id == can_dev_id)
    {
      switch (can_lut_sw_entry->def.type)
        {
          case CAN_LUT_STD       : return create_single (& can_lut_sw_entry->def.ident.single) ;
          case CAN_LUT_STD_GROUP : return create_group  (& can_lut_sw_entry->def.ident.group ) ;
        }
    }

  return NULL ;
}

//--------------------------------------------------------------------------//

ID_Range * can_v_lut_extract_class_ext (const Can_Dev_Id can_dev_id , const Can_Lut_Sw_Entry * const can_lut_sw_entry)
{
  if (can_lut_sw_entry->def.dev_id == can_dev_id)
    {
      switch (can_lut_sw_entry->def.type)
        {
          case CAN_LUT_EXT       : return create_single (& can_lut_sw_entry->def.ident.single) ;
          case CAN_LUT_EXT_GROUP : return create_group  (& can_lut_sw_entry->def.ident.group ) ;
        }
    }

  return NULL ;
}

//--------------------------------------------------------------------------//

static ID_Range * create_group (const Can_Lut_Group * const group)
{
  return can_v_id_range_create (group->min , group->max) ;
}

//--------------------------------------------------------------------------//

static ID_Range * create_single (const Can_Lut_Single * const single)
{
  return can_v_id_range_create (single->val , single->val) ;
}


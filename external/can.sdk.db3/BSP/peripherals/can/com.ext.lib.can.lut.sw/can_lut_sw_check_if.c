/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_lut_sw_check_if.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static Boolean is_can_id_in_group (const UInt32 can_id , const Can_Lut_Group * const group) ;

static void    throw_exception    (Exception * const exception , const Can_Lut_Sw_Status status) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_lut_sw_check_if_valid_entry (Exception * const exception , const Can_Lut_Sw_Entry * const entry)
{
  static const UInt32 MAX_CAN_ID_STD = 0x000007FF ;
  static const UInt32 MAX_CAN_ID_EXT = 0x1FFFFFFF ;

  switch (entry->def.type)
    {
      UInt32 max_id ;

      case CAN_LUT_STD : max_id = MAX_CAN_ID_STD ; goto check_single ;
      case CAN_LUT_EXT : max_id = MAX_CAN_ID_EXT ; goto check_single ; 

      check_single :
        {
          can_lut_sw_check_if_valid_can_id_single (exception , & entry->def.ident.single , max_id) ;
    
          return ;
        }

      case CAN_LUT_STD_GROUP : max_id = MAX_CAN_ID_STD ; goto check_group ; 
      case CAN_LUT_EXT_GROUP : max_id = MAX_CAN_ID_EXT ; goto check_group ; 

      check_group :
        {
          can_lut_sw_check_if_valid_can_id_group  (exception , & entry->def.ident.group , max_id) ;
    
          return ;
        }

      default : throw_exception (exception , CAN_LUT_SW_ERR_INTERNAL) ;
    }
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_valid_can_id_single (Exception * const exception , const Can_Lut_Single * const can_lut_single , const UInt32 can_id_max)
{
  can_lut_sw_check_if_valid_can_id (exception , can_lut_single->val , can_id_max) ;
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_valid_can_id_group  (Exception * const exception , const Can_Lut_Group  * const can_lut_group  , const UInt32 can_id_max)
{
  can_lut_sw_check_if_valid_can_id (exception , can_lut_group->min , can_id_max) ;
  can_lut_sw_check_if_valid_can_id (exception , can_lut_group->max , can_id_max) ;

  if (can_lut_group->min >= can_lut_group->max)
    {
      throw_exception (exception , CAN_LUT_SW_GROUP_ERR) ;
    }
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_valid_can_id (Exception * const exception , const UInt32 can_id , const UInt32 can_id_max)
{
  if (can_id > can_id_max)
    {
      throw_exception (exception , CAN_LUT_SW_CAN_ID_INVALID) ;
    }
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_table_conflict (Exception * const exception , const Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry * const entry)
{
  const Can_Lut_Sw_Entry * table_entry ;

  for (can_lut_sw_iterator_init (can_lut_sw , & table_entry) ; table_entry ; can_lut_sw_iterator_next (can_lut_sw , & table_entry))
    {                                                                  
      if (table_entry->def.dev_id == entry->def.dev_id)
        {
          #define PAIR(lhs,rhs) (((UInt16) lhs << 8) | ((UInt16) rhs))
    
          switch (PAIR(table_entry->def.type , entry->def.type))
            {
              case PAIR (CAN_LUT_STD       , CAN_LUT_STD      ) :
              case PAIR (CAN_LUT_EXT       , CAN_LUT_EXT      ) : can_lut_sw_check_if_single_equal    (exception , & table_entry->def.ident.single , & entry->def.ident.single     ) ; break ;
    
              case PAIR (CAN_LUT_STD       , CAN_LUT_STD_GROUP) :
              case PAIR (CAN_LUT_EXT       , CAN_LUT_EXT_GROUP) : can_lut_sw_check_if_single_in_group (exception , & table_entry->def.ident.single , & entry->def.ident.group      ) ; break ;
    
              case PAIR (CAN_LUT_STD_GROUP , CAN_LUT_STD      ) :
              case PAIR (CAN_LUT_EXT_GROUP , CAN_LUT_EXT      ) : can_lut_sw_check_if_single_in_group (exception , & entry->def.ident.single       , & table_entry->def.ident.group) ; break ;
    
              case PAIR (CAN_LUT_STD_GROUP , CAN_LUT_STD_GROUP) :
              case PAIR (CAN_LUT_EXT_GROUP , CAN_LUT_EXT_GROUP) : can_lut_sw_check_if_group_overlap   (exception , & entry->def.ident.group        , & table_entry->def.ident.group) ; break ;
            }
    
          #undef  PAIR
        }
    }
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_table_full (Exception * const exception , const Can_Lut_Sw * const can_lut_sw)
{
  if (can_lut_sw->num_entries >= can_lut_sw->max_entries)
    {
      throw_exception (exception , CAN_LUT_SW_FULL) ;
    }
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_single_equal (Exception * const exception , const Can_Lut_Single * const single_lhs , const Can_Lut_Single * const single_rhs)
{
  if (single_lhs->val == single_rhs->val)
    {
      throw_exception (exception , CAN_LUT_SW_DUPLICATE) ;
    }
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_single_in_group (Exception * const exception , const Can_Lut_Single * const single_lhs , const Can_Lut_Group * const group_rhs)
{
  if (is_can_id_in_group (single_lhs->val , group_rhs))
    {
      throw_exception (exception , CAN_LUT_SW_GROUP_OVERLAP) ;
    }
}

//--------------------------------------------------------------------------//

void can_lut_sw_check_if_group_overlap (Exception * const exception , const Can_Lut_Group * const group_lhs , const Can_Lut_Group * const group_rhs)
{
  #define TEST_1 is_can_id_in_group (group_lhs->min , group_rhs)
  #define TEST_2 is_can_id_in_group (group_lhs->max , group_rhs)
  #define TEST_3 is_can_id_in_group (group_rhs->min , group_lhs)
  #define TEST_4 is_can_id_in_group (group_rhs->max , group_lhs)

  if (TEST_1 || TEST_2 || TEST_3 || TEST_4)
    {
      throw_exception (exception , CAN_LUT_SW_GROUP_OVERLAP) ;
    }

  #undef  TEST_4
  #undef  TEST_3
  #undef  TEST_2
  #undef  TEST_1
}

//--------------------------------------------------------------------------//

static Boolean is_can_id_in_group (const UInt32 can_id , const Can_Lut_Group * const group)
{
  return group->min <= can_id && can_id <= group->max ;
}

//--------------------------------------------------------------------------//

static void throw_exception (Exception * const exception , const Can_Lut_Sw_Status status)
{
  exception->status = status ;

  longjmp (exception->throw_vec , 1) ;
}


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_id_set_intersect.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

ID_Set * can_v_id_set_intersect (ID_Set * lhs , ID_Set * rhs)
{
  ID_Set * set = can_v_id_set_create () ;

  ID_Range       * lhs_r , * rhs_r ;
  ID_Set_Iterator  lhs_i ,   rhs_i ;

  for (can_v_id_set_iterator_init (& lhs_i , & lhs_r , lhs , ITER_L2R) ; lhs_r ; can_v_id_set_iterator_next (& lhs_i , & lhs_r))
    {
      for (can_v_id_set_iterator_init (& rhs_i , & rhs_r , rhs , ITER_L2R) ; rhs_r ; can_v_id_set_iterator_next (& rhs_i , & rhs_r))
        {
          #define INTERSECT ! (lhs_r->max < rhs_r->min || lhs_r->min > rhs_r->max)

          if (INTERSECT)
            {
              #define ISEC_MIN (lhs_r->min > rhs_r->min ? lhs_r : rhs_r)->min
              #define ISEC_MAX (lhs_r->max < rhs_r->max ? lhs_r : rhs_r)->max

              can_v_id_set_insert_rhs (set , can_v_id_range_create (ISEC_MIN , ISEC_MAX)) ;

              #undef  ISEC_MAX
              #undef  ISEC_MIN
            }

          #undef  INTERSECT
        }
    }

  return set ;
}


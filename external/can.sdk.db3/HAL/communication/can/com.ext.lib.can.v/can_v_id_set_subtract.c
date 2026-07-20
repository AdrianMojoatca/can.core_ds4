/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_id_set_subtract.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

ID_Set * can_v_id_set_subtract (ID_Set * lhs , ID_Set * rhs)
{
  ID_Range       * lhs_r , * rhs_r ;
  ID_Set_Iterator  lhs_i ,   rhs_i ;

  lhs = can_v_id_set_copy (lhs) ;

  for (can_v_id_set_iterator_init (& rhs_i , & rhs_r , rhs , ITER_L2R) ; rhs_r ; can_v_id_set_iterator_next (& rhs_i , & rhs_r))
    {
      for (can_v_id_set_iterator_init (& lhs_i , & lhs_r , lhs , ITER_L2R) ; lhs_r ; )
        {
          ID_Range * root = lhs_r ;

          can_v_id_set_iterator_next (& lhs_i , & lhs_r) ;

          #define INTERSECT ! (root->max < rhs_r->min || root->min > rhs_r->max)

          if (INTERSECT)
            {
              if (root->min < rhs_r->min)
                {
                  can_v_id_range_link_lhs (root , can_v_id_range_create (root->min     , rhs_r->min - 1)) ;
                }

              if (root->max > rhs_r->max)
                {
                  can_v_id_range_link_rhs (root , can_v_id_range_create (rhs_r->max + 1 , root->max    )) ;
                }

              can_v_id_range_unlink (root) ;
              can_v_id_range_delete (root) ;
            }

          #undef  INTERSECT
        }
    }

  return lhs ;
}


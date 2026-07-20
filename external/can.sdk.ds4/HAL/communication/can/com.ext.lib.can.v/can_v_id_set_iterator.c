/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_id_set_iterator.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void advance_l2r (ID_Set_Iterator * iter) ;
static void advance_r2l (ID_Set_Iterator * iter) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_v_id_set_iterator_init (ID_Set_Iterator * iter , ID_Range ** id_range , ID_Set * id_set , ID_Set_Iterator_Dir iter_dir)
{
  switch (iter_dir)
    {
      case ITER_L2R :
        {
          iter->next    = (Q_Node *) can_v_id_set_peek_lhs (id_set) ;
          iter->advance = advance_l2r                               ;

          break ;
        }

      default : // ITER_R2L
        {
          iter->next    = (Q_Node *) can_v_id_set_peek_rhs (id_set) ;
          iter->advance = advance_r2l                               ;

          break ;
        }
    }

  iter->root = (Q_Node *) id_set ;

  *id_range  = (ID_Range *) iter->next ;
}

//--------------------------------------------------------------------------//

void can_v_id_set_iterator_next (ID_Set_Iterator * iter , ID_Range ** range)
{
  if (iter->next)
    {
      iter->advance (iter) ;
    
      if (iter->next == iter->root)
        {
          iter->next = NULL ;
        }
    }
    
  *range = (ID_Range *) iter->next ;
}

//--------------------------------------------------------------------------//

static void advance_l2r (ID_Set_Iterator * iter)
{
  iter->next = iter->next->lhs ;
}

//--------------------------------------------------------------------------//

static void advance_r2l (ID_Set_Iterator * iter)
{
  iter->next = iter->next->rhs ;
}


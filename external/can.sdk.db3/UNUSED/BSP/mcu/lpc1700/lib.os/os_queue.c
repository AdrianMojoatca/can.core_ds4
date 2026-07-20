/****************************************************************************/
/*                                                                          */
/*                         A-Task Real-Time Kernel                          */
/*                                                                          */
/*                    Copyright (C) 2000 - Acacetus Inc                     */
/*                                                                          */
/*                           All rights reserved                            */
/*                                                                          */
/*                             www.acacetus.com                             */
/*                                                                          */
/****************************************************************************/

/*==========================================================================*/
// $Id: os_queue.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os_private.h"
 
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

//-------------------------------[ QueueNode ]------------------------------//

void os_q_init (QueueNode * q_node)
{
  q_node->rhs = q_node->lhs = q_node ;
}

/*--------------------------------------------------------------------------*/

void os_q_unlink (QueueNode * q_node)
{
  q_node->lhs->rhs = q_node->rhs ;
  q_node->rhs->lhs = q_node->lhs ;
}

/*--------------------------------------------------------------------------*/

QueueNode * os_q_head (QueueNode * q_list)
{
  return q_list->rhs == q_list ? NULL : q_list->rhs ;
}

/*--------------------------------------------------------------------------*/

void os_q_link_rhs (QueueNode * q_list , QueueNode * q_node)
{
  q_node->lhs      = q_list      ;
  q_node->rhs      = q_list->rhs ;
  q_node->rhs->lhs = q_node      ;

  q_list->rhs      = q_node      ;
}

/*--------------------------------------------------------------------------*/

void os_q_link_lhs (QueueNode * q_list , QueueNode * q_node)
{
  q_node->rhs      = q_list      ;
  q_node->lhs      = q_list->lhs ;
  q_node->lhs->rhs = q_node      ;

  q_list->lhs      = q_node      ;
}

//-------------------------------[ PQueueNode ]-----------------------------//

PQueueNode * os_pq_head (QueueNode * pq_list)
{
  QueueNode * next = os_q_head (pq_list) ;

  return next ? cast_q_to_pq (next) : NULL ;
}

/*--------------------------------------------------------------------------*/

void os_pq_insert_head (QueueNode * pq_list , PQueueNode * pq_node)
{
  QueueNode * curr ;

  for (curr = pq_list->rhs ; curr != pq_list ; curr = curr->rhs)
    {
      if (cast_q_to_pq(curr)->priority <= pq_node->priority)
        {
          break ;
        }
    }

  os_q_link_lhs (curr,cast_pq_to_q (pq_node)) ;
}

/*--------------------------------------------------------------------------*/

void os_pq_insert_tail (QueueNode * pq_list , PQueueNode * pq_node)
{
  QueueNode * curr ;

  for (curr = pq_list->rhs ; curr != pq_list ; curr = curr->rhs)
    {
      if (cast_q_to_pq(curr)->priority < pq_node->priority)
        {
          break ;
        }
    }

  os_q_link_lhs (curr , cast_pq_to_q (pq_node)) ;
}

/*--------------------------------------------------------------------------*/

void os_pq_remove (PQueueNode * pq_node)
{
  os_q_unlink (cast_pq_to_q (pq_node)) ;
}

//-------------------------------[ TQueueNode ]-----------------------------//

TQueueNode * os_tq_head (TQueueNode * tq_list)
{
  QueueNode * next = os_q_head (cast_tq_to_q (tq_list)) ;

  return next ? cast_q_to_tq (next) : NULL ;
}

/*--------------------------------------------------------------------------*/

void os_tq_insert (TQueueNode * tq_list , TQueueNode * tq_node , TskTimeout timeout)
{
  QueueNode * curr ;

  #define TQ_LIST cast_tq_to_q (tq_list)
  
  for (curr = TQ_LIST->rhs ; curr != TQ_LIST ; curr = curr->rhs)
    {
      #define CURR cast_q_to_tq(curr)

      if (CURR->timeout > timeout)
        {
          CURR->timeout -= timeout ;

          break ;
        }

      timeout -= CURR->timeout ;

      #undef  CURR
    }

  #undef  TQ_LIST

  tq_node->timeout = timeout ;

  os_q_link_lhs (curr , cast_tq_to_q (tq_node)) ;
}

/*--------------------------------------------------------------------------*/

void os_tq_remove (TQueueNode * tq_node)
{
  cast_q_to_tq (cast_tq_to_q (tq_node)->rhs)->timeout += tq_node->timeout ;

  os_q_unlink  (cast_tq_to_q (tq_node)) ;
}


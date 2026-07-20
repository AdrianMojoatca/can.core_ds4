/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_reg.c 34443 2015-10-19 14:58:23Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_bus_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean hobp_bus_entry_q_init(Hobp_Bus_Entry * entry ,  UInt8 q_size) ;
                                      
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

Q hobp_bus_reg     (UInt8 q_size) 
{
  Boolean status = FALSE ;
   
  Hobp_Bus_Entry * entry  = (Hobp_Bus_Entry *) os_mem_request (sizeof (Hobp_Bus_Entry)) ;

  if(entry)
    {
      status = hobp_bus_entry_q_init (entry , q_size) ;
    }
           

  if(status)
    {
      return (entry->q_pending)  ;
    }
  else
    {
      return NULL ;
    }
}
//--------------------------------------------------------------------------//

Boolean hobp_bus_entry_q_init(Hobp_Bus_Entry * entry , UInt8 q_size)
{
  UInt8 n ;

  entry->q_free    = q_create() ;
  entry->q_pending = q_create() ;

  for(n = 0 ; n < q_size ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Hobp_Bus_Rx_Q_Msg)) ;
      
      if(!q_msg)
        {
          return FALSE ;
        }
      q_assign (entry->q_free , q_msg) ;
      q_insert (entry->q_free , q_msg) ;
    }

  entry -> next = hobp_bus_vars.list ;

  hobp_bus_vars.list = entry         ;

  return TRUE ;
}

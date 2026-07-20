/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_abd_test.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define TOTAL_EVT_NEEDED 10
#define GOOD_MSG_NEEDED 5 
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_abd_test (Can_Abd_Status * can_abd_status , TskTimeout timeout)
{
  UInt8 evt_count = 0 ;

  can_abd_status->speed_match = FALSE ;
  can_abd_status->nb_good_msg = 0 ;

  while(evt_count < TOTAL_EVT_NEEDED)
    {
	  UInt16 status ;
      
	  status = os_evt_wait (can_abd.evt , CAN_IRQ_ABD_RX_OK | CAN_IRQ_ABD_RX_BUS_ERROR , 0 , timeout) ;

	  switch (status)
	    {
		  case CAN_IRQ_ABD_RX_OK :
		    {
			  can_abd_status->nb_good_msg++ ;

		      if (can_abd_status->nb_good_msg >= GOOD_MSG_NEEDED)
              {
	            can_abd_status->speed_match = TRUE ;
              }
			}
			break ;

		  case CAN_IRQ_ABD_RX_BUS_ERROR :
			break ;

		  default :	// timeout
			can_abd_status->speed_match = FALSE ;
		    return ;
		}
	  evt_count ++ ;
    }
}


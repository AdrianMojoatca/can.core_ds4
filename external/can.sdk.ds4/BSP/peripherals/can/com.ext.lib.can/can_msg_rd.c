/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_msg_rd.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_private.h"

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

void can_msg_rd (Can_IO_Base can_io_base , Can_Msg * can_msg)
{
  #define BIT(z) ((UInt32) 1 << (z))
  
  //Can_Dev_Id dev = CAN_BASE_1 == can_io_base ? CAN_DEV_1 : CAN_DEV_0;

  can_msg->id = can_io_base [CANRID] ;
  
  can_msg->is_ext = !! (can_io_base [CANRFS] & (1ul << 31)) ;
  can_msg->is_rtr = !! (can_io_base [CANRFS] & (1ul << 30)) ;
  
  can_msg->length = (can_io_base [CANRFS] >> 16) & 0x0F  ;

  can_msg->data._32_ [0] = can_io_base [CANRDA] ;
  can_msg->data._32_ [1] = can_io_base [CANRDB] ;

  can_msg->lut_index_rx  = can_io_base [CANRFS] & 0x03FF ;

  /*--------------------------------------------------------------------------*\
  | If the number of STD definitions is odd, there is a hole in the LUT table  |
  | and we have to correct the physical index returned by the CAN hardware to  |
  | match the logical index used when reading the table via 'can_lut_read'.    |
  \*--------------------------------------------------------------------------*/

  if (can_lut_counters.std & 1 && can_msg->lut_index_rx >= can_lut_counters.std)
    {
      can_msg->lut_index_rx -- ;
    }

  can_io_base [CANCMR] = BIT(CAN_RRB) ; // Release CAN RX buffer
 	
  #undef  BIT 
}


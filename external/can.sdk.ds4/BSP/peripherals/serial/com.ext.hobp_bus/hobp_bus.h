/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus.h 34443 2015-10-19 14:58:23Z martin.bouchard $
/*==========================================================================*/

#ifndef __HOBP_BUS_H__
#define __HOBP_BUS_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "q.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define HOBP_BUS_DATA_SIZE       16

typedef void (* volatile Hobp_Bus_Func)(int data) ;

typedef struct
{
  UInt8 size   ;
  UInt8 data[HOBP_BUS_DATA_SIZE];

}Hobp_Bus_Msg ;

//--------------------------------------------------------------------------//
typedef struct s_Hobp_Bus_Rx_Q_Msg
{ 
  Q_Msg q_base    ;
    
  int   data      ;

} Hobp_Bus_Rx_Q_Msg ;

//--------------------------------------------------------------------------//

enum
{
  HOBP_BUS_DATA_OK     =  0  ,
  HOBP_BUS_TIMEOUT     = -1  ,
  HOBP_BUS_SYNCBREAK   = -2  ,
} ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void    hobp_bus_ena         (void   * arg)      ;
void    hobp_bus_dis         (void   * arg)      ;

void    hobp_bus_set_baudrate(UInt32   baudrate) ;

int     hobp_bus_init        (void)              ; 

Boolean hobp_bus_tx          (UInt8  * data , UInt8 size)    ;

Boolean hobp_bus_is_connected(void)                          ;

Q       hobp_bus_reg         (UInt8 q_size)                  ;

EXTERN_C_LEAVE  

#endif


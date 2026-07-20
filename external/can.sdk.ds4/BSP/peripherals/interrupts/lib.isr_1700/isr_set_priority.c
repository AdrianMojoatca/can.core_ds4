/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: isr_set_priority.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void set_priority_processor  (Isr_Number irq_num , UInt8 irq_priority_shifted) ;
static void set_priority_peripheral (Isr_Number irq_num , UInt8 irq_priority_shifted) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void nvic_set_priority (Isr_Number irq_num , UInt8 irq_priority)
{
  ATOMIC_ALL
    (
      (irq_num < 0 ? set_priority_processor : set_priority_peripheral) (irq_num , (irq_priority & 0x1F) << 3) ;
    )
}

//--------------------------------------------------------------------------//

void set_priority_processor (Isr_Number irq_num , UInt8 irq_priority_shifted)
{
  typedef struct
    {
      Isr_Number  irq_num ;
      volatile UInt8 * shpr    ;

    } Pri_Map ;

  static const Pri_Map pri_map [] =
    {
      { ISR_ID_SVCALL      , & 11 [SCB->SHP - 4] } ,
      { ISR_ID_PENDSV      , & 14 [SCB->SHP - 4] } ,
      { ISR_ID_SYS_TICK    , & 15 [SCB->SHP - 4] } ,
      { ISR_ID_BUS_FAULT   , &  5 [SCB->SHP - 4] } ,
      { ISR_ID_MEM_MANAGE  , &  4 [SCB->SHP - 4] } ,
      { ISR_ID_USAGE_FAULT , &  6 [SCB->SHP - 4] } , 
    } ;

  UInt8 index = 0 ;

  do
    {
      if (pri_map [index].irq_num == irq_num)
        {
          *pri_map [index].shpr = irq_priority_shifted ;

          return ;
        }
    }
  while (++index < sizeof_array (pri_map)) ;
}

//--------------------------------------------------------------------------//

void set_priority_peripheral ( Isr_Number irq_num , UInt8 irq_priority_shifted )
{
  NVIC->IP [irq_num] = irq_priority_shifted ;
}


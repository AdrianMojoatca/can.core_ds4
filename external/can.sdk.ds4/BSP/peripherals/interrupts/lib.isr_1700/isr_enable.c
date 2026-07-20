/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: isr_enable.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void irq_enable_processor  (Isr_Number irq_num , Boolean enable) ;
static void irq_enable_peripheral (Isr_Number irq_num , Boolean enable) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void nvic_irq_enable (Isr_Number irq_num , Boolean enable)
{
  ATOMIC_ALL
    (
      (irq_num < 0 ? irq_enable_processor : irq_enable_peripheral) (irq_num , enable) ;
    )
}

//--------------------------------------------------------------------------//

void irq_enable_processor (Isr_Number irq_num , Boolean enable)
{
  typedef struct
    {
      Isr_Number   irq_num ;
      volatile UInt32 * addr    ;
      UInt8             bit     ;

    } Ena_Map ;

  static const Ena_Map ena_map [] =
    {
      { ISR_ID_SYS_TICK    , & SysTick->CTRL ,  1 } ,
      { ISR_ID_BUS_FAULT   , & SCB->SHCSR    , 17 } ,
      { ISR_ID_MEM_MANAGE  , & SCB->SHCSR    , 16 } ,
      { ISR_ID_USAGE_FAULT , & SCB->SHCSR    , 18 } ,
    } ;

  UInt8 index = 0 ;

  do
    {
      if (ena_map [index].irq_num == irq_num)
        {
          UInt32 mask = ((UInt32) 1 << ena_map [index].bit) ;

          if (enable)
            {
              *ena_map [index].addr |=   mask ;
            }
          else
            {
              *ena_map [index].addr |= ~ mask ;
            }

          return ;
        }
    }
  while (++index < sizeof_array (ena_map)) ;
}

//--------------------------------------------------------------------------//

void irq_enable_peripheral (Isr_Number irq_num , Boolean enable)
{
  if (enable)
    {
      NVIC->ISER [irq_num / 32] |=   ((UInt32) 1 << (irq_num % 32)) ;
    }
  else
    {
      NVIC->ICER [irq_num / 32] &= ~ ((UInt32) 1 << (irq_num % 32)) ;
    }
}


//--------------------------------------------------------------------------//

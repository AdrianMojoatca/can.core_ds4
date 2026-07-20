/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: gpio_pl.c 33398 2015-08-26 16:43:57Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "isr.h"
#include "gpio.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
//--------------------------------------------------------------------------//

void gpio_pl_pin_write( Gpio_Pl_T pl, Boolean val)
{
	if(val)
		gpio_pl_set(pl);
	else
		gpio_pl_clr(pl);
}

//--------------------------------------------------------------------------//

void gpio_pl_init(Gpio_Pl_T pl)
{
	if( pl )
	{
		gpio_pl_direction(pl);
		gpio_pl_select(pl);
		gpio_pl_mode(pl);
	}
}

//--------------------------------------------------------------------------//

// initialize data direction bit
void gpio_pl_direction(Gpio_Pl_T pl)
{
	UInt32 * addr;
	UInt32 addr_ofs;
	addr = (UInt32 *) & FIO0DIR;
	
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 8 ;
	
	if( (pl>>GPIO_PL_POS_DIR) & GPIO_PL_MASK_DIR )
		*(addr + addr_ofs) |= (1u<<GPIO_PL_PIN_NUM(pl));
	else
		*(addr + addr_ofs) &= ~(1u<<GPIO_PL_PIN_NUM(pl));

}

//--------------------------------------------------------------------------//

// Set port pin
void gpio_pl_set(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	if( ((GPIO_PL_FUNC_NUM(pl) == 0) & (GPIO_PL_CHAN_NUM(pl) == 1))) // for some Input GPIO - Software inversion
	{
	addr = (UInt32 *) & FIO0CLR;
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 8 ;
	}
	else
	{
	addr = (UInt32 *) & FIO0SET;
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 8 ;
	}

	*(addr + addr_ofs) |= (1u<<GPIO_PL_PIN_NUM(pl));
}


//--------------------------------------------------------------------------//

// Clear port pin
void gpio_pl_clr(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	if( ((GPIO_PL_FUNC_NUM(pl) == 0) & (GPIO_PL_CHAN_NUM(pl) == 1))) // for some Input GPIO - Software inversion
	{
	addr = (UInt32 *) & FIO0SET;
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 8 ;
	}
	else
	{
	addr = (UInt32 *) & FIO0CLR;
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 8 ;
	}

	*(addr + addr_ofs) |= (1u<<GPIO_PL_PIN_NUM(pl));
}

//--------------------------------------------------------------------------//

// toggle port pin
void gpio_pl_tog(Gpio_Pl_T pl)
{
	if(gpio_pl_pin_read(pl))
	{
		gpio_pl_clr(pl);
	}
	else
	{
		gpio_pl_set(pl);
	}
}


//--------------------------------------------------------------------------//
// Pin Selection for specific pin function
void gpio_pl_select(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	// skip pin function selection for 
	// port2 28-31
	// port3 0-17 and 22-31
	// port4 0-23 and 28-31
	
    addr = (UInt32 * )((pl & (16<<GPIO_PL_POS_PIN)) ? &PINSEL1 : &PINSEL0); // 
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 2 ;
	
	*(addr + addr_ofs) &= (~(3<<((GPIO_PL_PIN_NUM(pl) & 0xF)* 2)));
	*(addr + addr_ofs) |= (GPIO_PL_FUNC_NUM(pl)<<((GPIO_PL_PIN_NUM(pl) & 0xF)* 2));

}


//--------------------------------------------------------------------------//

// Pin Mode 
//00 Pin has an on-chip pull-up resistor enabled. 00
//01 Repeater mode (see text below).
//10 Pin has neither pull-up nor pull-down resistor enabled.
//11 Pin has an on-chip pull-down resistor enabled.

void gpio_pl_mode(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

    addr = (UInt32 * )((pl & (16<<GPIO_PL_POS_PIN)) ? &PINMODE1 : &PINMODE0); // 
	
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 2 ;

	*(addr + addr_ofs) &= (~(3<<((GPIO_PL_PIN_NUM(pl) & 0xF)* 2)));
	*(addr + addr_ofs) |= (GPIO_PL_PIN_MODE(pl)<<((GPIO_PL_PIN_NUM(pl) & 0xF)* 2));
}


//--------------------------------------------------------------------------//

// FIOPIN read 
Boolean gpio_pl_pin_read(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) & FIO0PIN;
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 8 ;

	if( ((GPIO_PL_FUNC_NUM(pl) == 0) & (GPIO_PL_CHAN_NUM(pl) == 1))) // for some Input GPIO - Software inversion
	{
		if( !!((*( addr + addr_ofs)) & (1u << GPIO_PL_PIN_NUM(pl))) )
			return FALSE;
		else
			return TRUE;
	}
	else
	{
		if( !!((*( addr + addr_ofs)) & (1u << GPIO_PL_PIN_NUM(pl))) )
			return TRUE;
		else
			return FALSE;
	}
}


//--------------------------------------------------------------------------//

// PIN function configuration as gpio

void gpio_pl_func_cfg_gpio(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

    addr = (UInt32 *)((pl & (16<<GPIO_PL_POS_PIN)) ? &PINSEL1 : &PINSEL0); // 
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 2 ;
	
	*(addr + addr_ofs) &= (~(3<<(GPIO_PL_PIN_NUM(pl) & 0xF)* 2));
	*(addr + addr_ofs) |= ( GPIO_PL_GPIO << ((GPIO_PL_PIN_NUM(pl) & 0xF)* 2));
}


//--------------------------------------------------------------------------//

// PIN Rising edge IRQ enable

void gpio_pl_rising_irq_ena(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	if( pl )
	{
		addr = (UInt32 *) & IO0_INT_EN_R;
		addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8 ;
		
		*(addr + addr_ofs) |= (1u << GPIO_PL_PIN_NUM(pl)); 
	}
	
}

//--------------------------------------------------------------------------//

// PIN Rising edge IRQ disable

void gpio_pl_rising_irq_dis(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) &IO0_INT_EN_R;
	addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8 ;
	
	*(addr + addr_ofs) &= ~(1u << GPIO_PL_PIN_NUM(pl)); 
	
}



//--------------------------------------------------------------------------//

// PIN Rising edge IRQ enabled?

Boolean gpio_pl_check_irq_ena_R(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;
	
	addr = (UInt32 *) & IO0_INT_EN_R;
	addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8 ;
	
	if( !!((*( addr + addr_ofs)) & (1u << GPIO_PL_PIN_NUM(pl))) )
		return TRUE;
	else
		return FALSE;

	//*(addr + addr_ofs) |= (1u << GPIO_PL_PIN_NUM(pl)); 
	
}

//--------------------------------------------------------------------------//

// PIN falling edge IRQ enable

void gpio_pl_falling_irq_ena(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	if( pl )
	{
		addr = (UInt32 *) &IO0_INT_EN_F;
		addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8;
		
		*(addr + addr_ofs) |= (1u << GPIO_PL_PIN_NUM(pl));
	}
	
}

//--------------------------------------------------------------------------//

// PIN falling edge IRQ disable

void gpio_pl_falling_irq_dis(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) &IO0_INT_EN_F;
	addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8 ;
	
	*(addr + addr_ofs) &= ~(1u << GPIO_PL_PIN_NUM(pl));
	
}

//--------------------------------------------------------------------------//

// PIN falling edge IRQ enabled?

Boolean gpio_pl_check_irq_ena_F(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) &IO0_INT_EN_F;
	addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8;
	
	if( !!((*( addr + addr_ofs)) & (1u << GPIO_PL_PIN_NUM(pl))) )
		return TRUE;
	else
		return FALSE;

//	*(addr + addr_ofs) |= (1u << GPIO_PL_PIN_NUM(pl));
	
}


//--------------------------------------------------------------------------//

// EXT interrupt clear

void gpio_pl_int_clr(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) &IO0_INT_CLR;
	addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8 ;
	
	*(addr + addr_ofs) |= (1u << GPIO_PL_PIN_NUM(pl));
	
}

//--------------------------------------------------------------------------//

// External rising edge interrupt status read 

Boolean gpio_pl_int_stat_r(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) &IO0_INT_STAT_R;
	addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8 ;
	
	if( !!((*(addr + addr_ofs)) & (1u << (pl & GPIO_PL_MASK_PIN))) )
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------//

// External falling edge interrupt status read 

Boolean gpio_pl_int_stat_f(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) &IO0_INT_STAT_F;
	addr_ofs = (GPIO_PL_PORT_NUM(pl)/2) * 8 ;
	
	if( !!((*(addr + addr_ofs)) & (1u << (pl & GPIO_PL_MASK_PIN))) )
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------//

// UART Channel cofiguration

void gpio_pl_uart_cfg(Gpio_Pl_T pl, UInt8 bdrate_type)
{
	UInt32 chan_num;

	chan_num = GPIO_PL_CHAN_NUM(pl);
	switch(chan_num)
	{
		case GPIO_PL_CHAN_0:
      U0LCR    =  0x83 ;           // N-parity 8-bits 1-stop enable-div-latches
      if(bdrate_type == 1)
      { 
        U0DLL = 39 ;              // LSB of 115200 into latches
        U0DLM = 0  ;              // MSB of 115200 into latches
      }
      else
      {
        U0DLL    = 166 ;             // LSB of 1200 into latches
        U0DLM    = 14  ;             // MSB of 1200 into latches
      }

      U0LCR   &= 0x7F ;           // Disable div-latches

      U0FCR    = 0x07 ;           // Enable FIFO (TX/RX) , RX threshold at 1-bytes

			break;
		case GPIO_PL_CHAN_1:
      U1LCR =  0x83 ;           // N-parity 8-bits 1-stop enable-div-latches

      if(bdrate_type == 1)
      {
        U1DLL = 39 ;              // LSB of 115200 into latches
        U1DLM = 0  ;              // MSB of 115200 into latches
      }
      else
      {
        U1DLL    = 166 ;             // LSB of 1200 into latches
        U1DLM    = 14  ;             // MSB of 1200 into latches
      }

      U1LCR &= 0x7F ;           // Disable div-latches

      U1FCR = 0x07 ;           // Enable FIFO (TX/RX) , RX threshold at 1-byte
			break;
		case GPIO_PL_CHAN_2:
			U2LCR =  0x83 ;           // N-parity 8-bits 1-stop enable-div-latches

      U2DLL = 39 ;              // LSB of 115200 into latches
      U2DLM = 0  ;              // MSB of 115200 into latches

      U2LCR &= 0x7F ;           // Disable div-latches

      U2FCR  = 0xC7 ;           // Enable FIFO (TX/RX) , RX threshold at 14-bytes
			break;
		case GPIO_PL_CHAN_3:
			U3LCR =  0x83 ;           // N-parity 8-bits 1-stop enable-div-latches

      if(bdrate_type == 1)
      {
          U3DLL = 39 ;              // LSB of 115200 into latches
          U3DLM = 0  ;              // MSB of 115200 into latches
      }
      else
      {
          U3DLL = 166 ;
          U3DLM = 14;
      }

      U3LCR &= 0x7F ;           // Disable div-latches

      U3FCR  = 0x07 ;           // Enable FIFO (TX/RX) , RX threshold at 1-byte
			
			break;
	}
}
//--------------------------------------------------------------------------//

// Pin channel 

void gpio_pl_chan(Gpio_Pl_T pl)
{
	UInt32 *addr;
	UInt32 addr_ofs;

	addr = (UInt32 *) &FIO0CLR;
	addr_ofs = GPIO_PL_PORT_NUM(pl) * 8 ;
	
	*(addr + addr_ofs) |= (1u<<GPIO_PL_PIN_NUM(pl));

}


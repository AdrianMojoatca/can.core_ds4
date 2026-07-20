/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d1_irq_service.c $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_valet_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void d2d_valet_butn_service        (UInt8 port_no) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void d2d_valet_butn_p4_service(void)
{
	d2d_valet_butn_service( 4 );
}

/*

void d2d_valet_butn_p1_service(void)
{
	d2d_valet_butn_service( UART_PORT1);
}
*/
	D2d_Valet_Port *local_port;
	struct s_Smsg_Port *port_new;

static void d2d_valet_butn_service( UInt8 port_no)
{
	port_new = smsg_port[port_no];
	local_port = (D2d_Valet_Port *)port_new;
		// Check id interrupt is because of D2D0 port button press
	if( gpio_pl_int_stat_f( local_port->butn_pin_label) 
      || gpio_pl_int_stat_r( local_port->butn_pin_label) )
	{
    gpio_pl_int_clr(local_port->butn_pin_label);   //clears both rising/falling
		// indebounce state
		d2d_valet_debounce_state = (gpio_pl_pin_read(local_port->butn_pin_label) ? DEBOUNCE_RELEASE : DEBOUNCE_PRESS);
    port_timer_rewind( port_new , PORT_TIMER_MS(100));
	}
	// clear the interrupt flag
	//gpio_pl_int_clr(local_port->butn_pin_label);
	// start 50ms debounce timer
	//port_timer_rewind( port_new , PORT_TIMER_MS(50));
}

/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "d2d_code.h"
#include "d2di_uart.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
// request protocol version: (0C FF 03 06 00 08 0D)
const UInt8 d2di_protocol_ver_req_packet[D2DI_MIN_PACKET_SIZE] = {D2DI_SOF, D2DI_ADDRESS_BROADCAST, D2DI_ADDRESS_RS, D2DI_CODE_REQ_PROTO_VERSION, D2DI_NO_DATA, 0x08, D2DI_EOF};

// request manufacturer id: (0C FF 03 08 00 0A 0D)
const UInt8 d2di_manufactuer_id_req_packet[D2DI_MIN_PACKET_SIZE] = {D2DI_SOF, D2DI_ADDRESS_BROADCAST, D2DI_ADDRESS_RS, D2DI_CODE_REQ_MANUFACTURER_ID, D2DI_NO_DATA, 0x0A, D2DI_EOF};

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
/* Given a D2D code (not D2DI code) map it to the supported D2DI command if it
 * exists in order to build and send the packet. */
void d2di_tx_code(UInt8 code)
{
  if(d2di_mask_is_enabled(code))
    {
        d2di_tx(code);
    }        
}
//--------------------------------------------------------------------------//
void d2di_tx(UInt8 code)
{
    UInt8 d2di_tx_packet[D2DI_MIN_PACKET_SIZE] = {D2DI_SOF, D2DI_ADDRESS_DL, D2DI_ADDRESS_BROADCAST, D2DI_NO_DATA, D2DI_NO_DATA, D2DI_DEFAULT_CHECKSUM, D2DI_EOF};
    Boolean flag_supported = TRUE;
    Boolean mask_enabled = FALSE;
    
    switch(code)
    {
        case SET_IGN_ON:
            if(d2di_mask_is_enabled(SET_IGN_ON))
            {
                mask_enabled = TRUE;
                d2di_tx_packet[D2DI_CODE_POS] = D2DI_CODE_IGN_ON;
            }
            break;
        
        case SET_IGN_OFF:
            if(d2di_mask_is_enabled(SET_IGN_OFF))
            {
                mask_enabled = TRUE;
                d2di_tx_packet[D2DI_CODE_POS] = D2DI_CODE_IGN_OFF;
            }
            break;
            
        case SET_KEYLESS_ARM:
            if(d2di_mask_is_enabled(SET_KEYLESS_ARM))
            {
                mask_enabled = TRUE;
                d2di_tx_packet[D2DI_DEST_ADDRESS_POS] = D2DI_ADDRESS_ALARM;
                d2di_tx_packet[D2DI_CODE_POS] = D2DI_CODE_ARM;
            }
            break;
            
        case SET_KEYLESS_DISARM:
            if(d2di_mask_is_enabled(SET_KEYLESS_DISARM))
            {
                mask_enabled = TRUE;
                d2di_tx_packet[D2DI_DEST_ADDRESS_POS] = D2DI_ADDRESS_ALARM;
                d2di_tx_packet[D2DI_CODE_POS] = D2DI_CODE_DISARM;
            }
            break;
        
        default:
            flag_supported = FALSE;
            break;
    }
    
    // transmit only if lib supports msg and mask is enabled
    if(flag_supported && mask_enabled)
    {
        d2di_insert_checksum(d2di_tx_packet);
        d2di_uart_tx_q_insert_frame(d2di_tx_packet, D2DI_MIN_PACKET_SIZE);
    }
}

//--------------------------------------------------------------------------//
void d2di_request_rs_info(void)
{
    // request manufacturer id and protocol version
    d2di_uart_tx_q_insert_frame(d2di_manufactuer_id_req_packet, D2DI_MIN_PACKET_SIZE);
    os_tsk_wait(15);
    d2di_uart_tx_q_insert_frame(d2di_protocol_ver_req_packet, D2DI_MIN_PACKET_SIZE);
}

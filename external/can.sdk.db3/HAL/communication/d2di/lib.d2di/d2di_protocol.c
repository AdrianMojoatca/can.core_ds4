/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "d2di_uart.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define D2DI_DBG_PROTOCOL 0
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static UInt8 byte_index, d2di_last_index;
static UInt8 d2di_frame[D2DI_MAX_PACKET_SIZE];
static UInt8 decode_from_memory;
static Boolean decode_done;
static D2Di_State decode_state;

#if D2DI_DBG_PROTOCOL == 1
    // debugging
    static const char * state_table[] = 
    {
      {"\n[D2DI_PROTOCOL]D2DI_STATE_SOF"     } ,
      {"\n[D2DI_PROTOCOL]D2DI_STATE_SRC"     } ,
      {"\n[D2DI_PROTOCOL]D2DI_STATE_DST"     } ,
      {"\n[D2DI_PROTOCOL]D2DI_STATE_CODE"    } ,
      {"\n[D2DI_PROTOCOL]D2DI_STATE_CNT"     } ,
      {"\n[D2DI_PROTOCOL]D2DI_STATE_DATA"    } ,
      {"\n[D2DI_PROTOCOL]D2DI_STATE_CHECKSUM"} ,
      {"\n[D2DI_PROTOCOL]D2DI_STATE_EOF"     }
    } ;
#endif

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
static void d2di_append_byte_to_frame(UInt8 value);
static void d2di_restart_decode(void);
void d2di_timeout_expired(void *arg);

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
/*
 * The state machine decoding the packets can work in two modes: decode byte
 * per byte as they are removed from rx queue or decode bytes from memory (already
 * removed from rx q). The second case represents the situation where we started
 * decoding a packet, realized it was corrupt so we try to backtrack and see if
 * the corrupt packet actually contains a valid packet. In that case we don't want to 
 * pop bytes from rx queue so we use the decode_from_memory var to keep track of how
 * many bytes we decode from what was already received before popping from UART again.
 */
Boolean d2di_decode_frame(UInt8 **d2di_packet)
{
    UInt8 data_cnt, checksum, byte_read ;
    Boolean valid_decode = FALSE;
    
    // init values
    decode_state = D2DI_STATE_SOF;
    decode_done = FALSE;
    data_cnt = 0;
    checksum = 0;
    decode_from_memory = 0;
    d2di_last_index = 0;
    
    #if D2DI_DBG_PROTOCOL == 1
        dbg_tx_text("\n[D2DI_PROTOCOL]Starting decode routine");
    #endif
    
    timeout_start(d2di_vars.d2di_interframe_timeout);
    
    while(!decode_done) // loop until packet decoded or dropped
    {
        if(decode_from_memory)
        {
            byte_read = d2di_frame[byte_index];
        }
        else
        {
            // check decode_done again because the timeout callback may have set it
            while(!decode_done && d2di_uart_rx_q_remove(&byte_read) == D2DI_UART_Q_EMPTY)
            {
                if(timeout_test(d2di_vars.d2di_interframe_timeout))
                {
                    #if DBG_D2D == 1
                        dbg_tx_text("\n[D2DI_PROTOCOL]Interframe timeout error! Packet dropped");
                    #endif
                    
                    decode_done = TRUE;
                    
                    // first packet failed to decode
                    if(d2di_wake_ready_bytes < D2DI_MIN_PACKET_SIZE)
                    {
                        d2di_err_add(D2DI_ERR_WAKE_ERROR);
                    }
                    
                    // interframe timeout error
                    else
                    {
                        d2di_err_add(D2DI_ERR_INTERFRAME_TIMEOUT);
                    }
                    
                    return FALSE;
                }
            }
            // reset timer on byte reception
            timeout_start(d2di_vars.d2di_interframe_timeout);
        }
            
        #if D2DI_DBG_PROTOCOL == 1
            dbg_tx_text("\n[D2DI_PROTOCOL]Got byte: ");
            dbg_tx_h8(byte_read);
        #endif
            
        switch(decode_state)
        {
            case D2DI_STATE_SOF:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(byte_read == D2DI_SOF)
                {
                    byte_index = 0;
                    d2di_append_byte_to_frame(byte_read);
                    decode_state = D2DI_STATE_SRC;
                }
                break;
                
            case D2DI_STATE_SRC:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(decode_from_memory)
                {
                    decode_from_memory--;
                    byte_index++;
                }
                else
                    d2di_append_byte_to_frame(byte_read);
                
                decode_state = D2DI_STATE_DST;
                break;
            
            case D2DI_STATE_DST:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(decode_from_memory)
                {
                    decode_from_memory--;
                    byte_index++;
                }
                else
                    d2di_append_byte_to_frame(byte_read);
                
                decode_state = D2DI_STATE_CODE;
                break;
            
            case D2DI_STATE_CODE:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(decode_from_memory)
                {
                    decode_from_memory--;
                    byte_index++;
                }
                else
                    d2di_append_byte_to_frame(byte_read);
                
                decode_state = D2DI_STATE_CNT;
                break;
            
            case D2DI_STATE_CNT:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(decode_from_memory)
                {
                    decode_from_memory--;
                    byte_index++;
                }
                else
                    d2di_append_byte_to_frame(byte_read);
                
                data_cnt = byte_read;
                if(data_cnt == 0)
                {
                    decode_state = D2DI_STATE_CHKSUM;
                }
                else if((data_cnt > 0) && (data_cnt <= D2DI_MAX_DATA_CNT))
                {
                    decode_state = D2DI_STATE_DATA;
                }
                else
                    d2di_restart_decode();
                break;
                
            case D2DI_STATE_DATA:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(data_cnt > 0)
                {
                    if(decode_from_memory)
                    {
                        decode_from_memory--;
                        byte_index++;
                    }
                    else
                        d2di_append_byte_to_frame(byte_read);
                    
                    data_cnt--;
                    break;
                }
                decode_state = D2DI_STATE_CHKSUM;
                
            case D2DI_STATE_CHKSUM:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(decode_from_memory)
                {
                    decode_from_memory--;
                    byte_index++;
                }
                else
                    d2di_append_byte_to_frame(byte_read);
                
                checksum = d2di_calculate_checksum(d2di_frame);
                if(byte_read != checksum)
                {
                    #if DBG_D2D == 1
                        dbg_tx_text("\n[D2DI_PROTOCOL]wrong checksum!");
                    #endif
                    d2di_restart_decode();
                }
                else
                    decode_state = D2DI_STATE_EOF;
                break;
                
            case D2DI_STATE_EOF:
                #if D2DI_DBG_PROTOCOL == 1
                    dbg_tx_text(state_table[decode_state]);
                #endif
                if(decode_from_memory)
                {
                    decode_from_memory--;
                    byte_index++;
                }
                else
                {
                    d2di_append_byte_to_frame(byte_read);
                }
                
                if(byte_read == D2DI_EOF)
                {
                    decode_done = TRUE;
                    valid_decode = TRUE;
                    
                    #if DBG_D2D == 1
                        dbg_tx_text("\n[D2DI_PROTOCOL]Decode done! Packet = ");
                        for(i = 0; i < (D2DI_MIN_PACKET_SIZE + d2di_frame[D2DI_DATA_COUNT_POS]); i++)
                        {
                            dbg_tx_h8(d2di_frame[i]);
                        }
                    #endif
                    
                }
                else // not a lot of chances that this happens (correct checksum but missing EOF)
                {
                    d2di_restart_decode();
                }
                break;
        }
    }

    timeout_stop(d2di_vars.d2di_interframe_timeout);
    *d2di_packet = d2di_frame;
    
    // to avoid race condition where multiple packets may have been received while we decoded one packet
    if(!d2di_uart_rx_q_is_empty())
    {
        os_irq_set_i(d2di_uart_rx_irq);
    }
    
    return valid_decode; // TRUE if packet is valid, otherwise FALSE
}

//--------------------------------------------------------------------------//
static void d2di_append_byte_to_frame(UInt8 value)
{
    d2di_frame[byte_index] = value;
    byte_index++;
}

//--------------------------------------------------------------------------//
/* Check if buffer already contains a SOF if yes shift the data back and attempt
 * to decode the packet again. */
static void d2di_restart_decode(void)
{
    UInt8 i, j;
    UInt8 tmp_d2di_frame[D2DI_MAX_PACKET_SIZE];
    Boolean contains_sof;
    j = 0;
    contains_sof = FALSE;
    
    //we only set it once because we need to keep the position of the last index.
    if(d2di_last_index == 0)
    {
        
        d2di_last_index = byte_index;
    }
    
    for(i = 1; i < d2di_last_index; i++)
    {
        if(d2di_frame[i] == D2DI_SOF)
        {
            contains_sof = TRUE;
            while(i <= d2di_last_index)
            {
                tmp_d2di_frame[j] = d2di_frame[i];
                j++;
                i++;
            }
            d2di_last_index = d2di_last_index + 1 - i;
            decode_from_memory =  j - 2; // decode only bytes in between the new 0C and last index
            byte_index = 1; // restarting state machine at D2DI_STATE_SRC (index = 1)
            break;
        }
    }
    
    if(contains_sof)
    {
        #if DBG_D2D == 1
            dbg_tx_text("\n[D2DI_PROTOCOL]Found another SOF, attempt decoding from there");
        #endif
        memcpy(&d2di_frame[0], &tmp_d2di_frame[0], j);
        decode_state = D2DI_STATE_SRC;
    }
    else
    {
        #if DBG_D2D == 1
            dbg_tx_text("\n[D2DI_PROTOCOL]No other SOF found. Scrap packet and restart decoding");
        #endif
        decode_state = D2DI_STATE_SOF;
        
        // first packet failed to decode
        if(d2di_wake_ready_bytes < D2DI_MIN_PACKET_SIZE)
        {
            d2di_err_add(D2DI_ERR_WAKE_ERROR);
        }
        
        // general packet decoding error
        else
        {
            d2di_err_add(D2DI_ERR_CORRUPT_PACKET);
        }
        
    }
}

//--------------------------------------------------------------------------//
UInt8 d2di_calculate_checksum(UInt8 *d2di_packet)
{
    UInt8 i, checksum;
    UInt8 data_cnt = d2di_packet[D2DI_DATA_COUNT_POS];
    
    i = 0;
    checksum = 0;
    
    for(i = 1; i <= (D2DI_DATA_COUNT_POS + data_cnt); i++)
    {
        checksum += d2di_packet[i];
    }
    
    return checksum;
}

//--------------------------------------------------------------------------//
void d2di_insert_checksum(UInt8 *d2di_packet)
{
    UInt8 checksum = d2di_calculate_checksum(d2di_packet);
    d2di_packet[d2di_packet[D2DI_DATA_COUNT_POS] + D2DI_FIRST_DATA_BYTE] = checksum;
}

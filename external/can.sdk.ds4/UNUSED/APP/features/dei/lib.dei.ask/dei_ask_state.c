/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_state.c 31193 2015-05-06 17:14:56Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ask_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static __inline Dei_Ask_Bit dei_ask_bit_time_check(Boolean is_rising_edge,Dei_Ask_Bit_Time_Checker *checker);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ask_state_header( Boolean is_rising_edge )
{
    Dei_Ask_Bit bit = dei_ask_bit_time_check(is_rising_edge,&dei_ask.bit_checker_header);
    
    switch(bit)
    {
        case DEI_ASK_BIT_HEADER:
        {
            dei_ask.state = dei_ask_state_data_enc_portion;
            
            dei_ask.bit_index      = 0;
            dei_ask.data_enc.data  = 0;    

            //change the bit checker to verify both times            
            dei_ask.bit_checker_high.check_what = DEI_ASK_BIT_CHECK_BOTH;
            dei_ask.bit_checker_low .check_what = DEI_ASK_BIT_CHECK_BOTH;
        }
        break;
        
        default:break;                    
    }
}

//--------------------------------------------------------------------------//

void dei_ask_state_data_enc_portion( Boolean is_rising_edge )
{
    Dei_Ask_Bit bit = dei_ask_bit_time_check(is_rising_edge,&dei_ask.bit_checker_low);
    
    if(bit == DEI_ASK_BIT_ERROR)
    {
        bit = dei_ask_bit_time_check(is_rising_edge,&dei_ask.bit_checker_high);
    }
    
    switch(bit)
    {
        case DEI_ASK_BIT_HIGH:
        case DEI_ASK_BIT_LOW :
        {
            dei_ask.bit_index ++;    
            
            if(bit == DEI_ASK_BIT_HIGH)
            {
                dei_ask.data_enc.data |= (((UInt64)1) << DEI_ASK_ENC_PORTION_DATA_SIZE);
            }
            
            dei_ask.data_enc.data = dei_ask.data_enc.data >> 1;
            
            if(dei_ask.bit_index == DEI_ASK_ENC_PORTION_DATA_SIZE)
            {
                dei_ask.bit_index      = 0;
                dei_ask.data_fix.data  = 0;
                
                dei_ask.state = dei_ask_state_data_fix_portion;
            }            
        }
        break;
        
        case DEI_ASK_BIT_HEADER:
        {
            dei_ask.state = dei_ask_state_data_enc_portion;
            
            dei_ask.bit_index     = 0;
            dei_ask.data_enc.data = 0;
        }
        break;
        
        
        case DEI_ASK_BIT_ERROR:
        {
             dei_ask.state = dei_ask_state_header;
        }
        break;
        
        default:break;                    
    }
}

//--------------------------------------------------------------------------//

void dei_ask_state_data_fix_portion(Boolean is_rising_edge)
{
    Dei_Ask_Bit bit = dei_ask_bit_time_check(is_rising_edge,&dei_ask.bit_checker_low);
    
    if(bit == DEI_ASK_BIT_ERROR)
    {
        bit = dei_ask_bit_time_check(is_rising_edge,&dei_ask.bit_checker_high);
    }
    
    switch(bit)
    {
        case DEI_ASK_BIT_HIGH:
        case DEI_ASK_BIT_LOW :
        {
            dei_ask.bit_index ++;
            
            if(bit == DEI_ASK_BIT_HIGH)
            {
                dei_ask.data_fix.data |= (((UInt64)1) << DEI_ASK_FIX_PORTION_DATA_SIZE);
            }
            
            dei_ask.data_fix.data = dei_ask.data_fix.data >> 1;
            
            if(dei_ask.bit_index == (DEI_ASK_FIX_PORTION_DATA_SIZE-1))
            {
                //change the bit checker to verify only the high time for tha last bit           
                dei_ask.bit_checker_high.check_what = DEI_ASK_BIT_CHECK_HIGH_TIME;
                dei_ask.bit_checker_low .check_what = DEI_ASK_BIT_CHECK_HIGH_TIME;
                
                dei_ask.state = dei_ask_state_data_fix_portion_last_bit;                                                
            }                                                
        }
        break;
        
        case DEI_ASK_BIT_HEADER:
        {
            dei_ask.state = dei_ask_state_data_enc_portion;
            
            dei_ask.bit_index      = 0;
            dei_ask.data_enc.data  = 0;
        }
        break;
        
        
        case DEI_ASK_BIT_ERROR:
        {
             dei_ask.state = dei_ask_state_header;
        }
        break;
        
        default:break;                    
    }
}

//--------------------------------------------------------------------------//

void dei_ask_state_data_fix_portion_last_bit(Boolean is_rising_edge)
{
    Dei_Ask_Bit bit = dei_ask_bit_time_check(is_rising_edge,&dei_ask.bit_checker_low);
    
    if(bit == DEI_ASK_BIT_ERROR)
    {
        bit = dei_ask_bit_time_check(is_rising_edge,&dei_ask.bit_checker_high);
    }
    
    switch(bit)
    {
        case DEI_ASK_BIT_HIGH:
        case DEI_ASK_BIT_LOW :
        {
            dei_ask.bit_index ++;
            
            if(bit == DEI_ASK_BIT_HIGH)
            {
                dei_ask.data_fix.data |= (((UInt64)1) << DEI_ASK_FIX_PORTION_DATA_SIZE);
            }
            
            dei_ask.data_fix.data = dei_ask.data_fix.data >> 1;
            
            if(dei_ask.bit_index == DEI_ASK_FIX_PORTION_DATA_SIZE)
            {
                UInt8 i ; 
                
                for(i=0;i<DEI_ASK_MAX_REMOTES;i++)
                {
                    if(dei_ask.programming_on || (dei_ask.database.remote[i].serial_number == dei_ask.data_fix.bits.sn) )
                    {
                        // disable external interrupts
                        // I'm done with the decoding
                                               
                        dei_ask.rx_complete = TRUE;
                                    
                        dei_ask.state = dei_ask_state_header;
                                
                        VICSoftInt = 2 ;
                    
                        DEI_ASK_EXT_DIS(); //disable the external interrupts
                    
                        wake_notify_wake();
                    }
                }                                    
            }                                                
        }
        break;
        
        case DEI_ASK_BIT_HEADER:
        {
            dei_ask.state = dei_ask_state_data_enc_portion;
            
            dei_ask.bit_index      = 0;
            dei_ask.data_enc.data  = 0;
        }
        break;
        
        
        case DEI_ASK_BIT_ERROR:
        {
             dei_ask.state = dei_ask_state_header;
        }
        break;
        
        default:break;                    
    }
}

//--------------------------------------------------------------------------//

__inline Dei_Ask_Bit dei_ask_bit_time_check(Boolean is_rising_edge,Dei_Ask_Bit_Time_Checker *checker)
{
    if(is_rising_edge == FALSE) return DEI_ASK_BIT_NOT_DECODED; //the bit is full decoded only on rising edge
    
    //check the bit time
    {                   
        switch(checker->check_what)
        {
            case DEI_ASK_BIT_CHECK_LOW_TIME:
            {
                Boolean low_ok  = (dei_ask.t_low  < checker->t_low_max ) && (dei_ask.t_low  > checker->t_low_min );
                
                if(low_ok ) return checker->bit;
            }
            break;
            
            case DEI_ASK_BIT_CHECK_HIGH_TIME:
            {
                Boolean high_ok = (dei_ask.t_high < checker->t_high_max) && (dei_ask.t_high > checker->t_high_min);
                
                if(high_ok) return checker->bit;
            }
            break;
            
            case DEI_ASK_BIT_CHECK_BOTH:
            {
                UInt32 per = dei_ask.t_low + dei_ask.t_high;
                               
                Boolean per_ok  = (per < (checker->t_low_max + checker->t_high_max)) && 
                                  (per > (checker->t_low_min + checker->t_high_min));
                
                
                if(per_ok && (dei_ask.t_high > dei_ask.t_low)) return DEI_ASK_BIT_LOW ;
                if(per_ok && (dei_ask.t_high < dei_ask.t_low)) return DEI_ASK_BIT_HIGH;                                                
            }
            break;
            
            default:break;
        }
    }
        
    return DEI_ASK_BIT_ERROR;
}
    
//----------------------------------------------------------------------------//



/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_bit_time_set.c 31193 2015-05-06 17:14:56Z martin.bouchard $
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
static void dei_ask_bit_time_set_72MHZ(void);
static void dei_ask_bit_time_set_12MHZ(void);
static void dei_ask_bit_time_set_6MHZ (void);
static void dei_ask_bit_time_set_4MHZ (void);
static void dei_ask_bit_time_set_3MHZ (void);
static void dei_ask_bit_time_set_2MHZ (void);
static void dei_ask_bit_time_set_1MHZ (void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//----------------------------------------------------------------------------//

void dei_ask_bit_time_set( void )
{
    switch(wake_get_current_cpu_freq())
    {
        case CPU_FREQ_72MHZ : dei_ask_bit_time_set_72MHZ();break;               
        case CPU_FREQ_12MHZ : dei_ask_bit_time_set_12MHZ();break;
        case CPU_FREQ_6MHZ  : dei_ask_bit_time_set_6MHZ ();break;
        case CPU_FREQ_4MHZ  : dei_ask_bit_time_set_4MHZ ();break;
        case CPU_FREQ_3MHZ  : dei_ask_bit_time_set_3MHZ ();break;
        case CPU_FREQ_2MHZ  : dei_ask_bit_time_set_2MHZ ();break;
        case CPU_FREQ_1MHZ  : dei_ask_bit_time_set_1MHZ ();break;
            
        default:break;
    }
}

//----------------------------------------------------------------------------//

void dei_ask_bit_time_set_72MHZ( void )
{
    static const Dei_Ask_Bit_Time_Checker bit_checker_low [] = 
    {
        DEI_ASK_LOW_BIT_LOW_MIN_72MHZ  , 
        DEI_ASK_LOW_BIT_LOW_MAX_72MHZ  , 
        
        DEI_ASK_LOW_BIT_HIGH_MIN_72MHZ , 
        DEI_ASK_LOW_BIT_HIGH_MAX_72MHZ , 
        
        DEI_ASK_BIT_LOW                ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME    ,                
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_high[] = 
    {
        DEI_ASK_HIGH_BIT_LOW_MIN_72MHZ  , 
        DEI_ASK_HIGH_BIT_LOW_MAX_72MHZ  , 
        
        DEI_ASK_HIGH_BIT_HIGH_MIN_72MHZ , 
        DEI_ASK_HIGH_BIT_HIGH_MAX_72MHZ , 
        
        DEI_ASK_BIT_HIGH                ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME     ,
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_header[] = 
    {
        DEI_ASK_HEADER_BIT_LOW_MIN_72MHZ  , 
        DEI_ASK_HEADER_BIT_LOW_MAX_72MHZ  , 
        
        DEI_ASK_HEADER_BIT_HIGH_MIN_72MHZ , 
        DEI_ASK_HEADER_BIT_HIGH_MAX_72MHZ , 
        
        DEI_ASK_BIT_HEADER                ,        
        DEI_ASK_BIT_CHECK_LOW_TIME        ,
    };
    
    *(&dei_ask.bit_checker_header)= *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_header));
    *(&dei_ask.bit_checker_low)   = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_low));
    *(&dei_ask.bit_checker_high)  = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_high));        
}
    
//----------------------------------------------------------------------------//  

void dei_ask_bit_time_set_12MHZ( void )
{
    static const Dei_Ask_Bit_Time_Checker bit_checker_low [] = 
    {
        DEI_ASK_LOW_BIT_LOW_MIN_12MHZ  , 
        DEI_ASK_LOW_BIT_LOW_MAX_12MHZ  , 
        
        DEI_ASK_LOW_BIT_HIGH_MIN_12MHZ , 
        DEI_ASK_LOW_BIT_HIGH_MAX_12MHZ , 
        
        DEI_ASK_BIT_LOW                ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME    ,                
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_high[] = 
    {
        DEI_ASK_HIGH_BIT_LOW_MIN_12MHZ  , 
        DEI_ASK_HIGH_BIT_LOW_MAX_12MHZ  , 
        
        DEI_ASK_HIGH_BIT_HIGH_MIN_12MHZ , 
        DEI_ASK_HIGH_BIT_HIGH_MAX_12MHZ , 
        
        DEI_ASK_BIT_HIGH                ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME     ,
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_header[] = 
    {
        DEI_ASK_HEADER_BIT_LOW_MIN_12MHZ  , 
        DEI_ASK_HEADER_BIT_LOW_MAX_12MHZ  , 
        
        DEI_ASK_HEADER_BIT_HIGH_MIN_12MHZ , 
        DEI_ASK_HEADER_BIT_HIGH_MAX_12MHZ , 
        
        DEI_ASK_BIT_HEADER                ,        
        DEI_ASK_BIT_CHECK_LOW_TIME        ,
    };
    
    
    *(&dei_ask.bit_checker_header)= *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_header));
    *(&dei_ask.bit_checker_low)   = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_low));
    *(&dei_ask.bit_checker_high)  = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_high));        
}
    
//----------------------------------------------------------------------------//  

void dei_ask_bit_time_set_6MHZ( void )
{
    static const Dei_Ask_Bit_Time_Checker bit_checker_low [] = 
    {
        DEI_ASK_LOW_BIT_LOW_MIN_6MHZ  , 
        DEI_ASK_LOW_BIT_LOW_MAX_6MHZ  , 
        
        DEI_ASK_LOW_BIT_HIGH_MIN_6MHZ , 
        DEI_ASK_LOW_BIT_HIGH_MAX_6MHZ , 
        
        DEI_ASK_BIT_LOW               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME   ,                
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_high[] = 
    {
        DEI_ASK_HIGH_BIT_LOW_MIN_6MHZ  , 
        DEI_ASK_HIGH_BIT_LOW_MAX_6MHZ  , 
        
        DEI_ASK_HIGH_BIT_HIGH_MIN_6MHZ , 
        DEI_ASK_HIGH_BIT_HIGH_MAX_6MHZ , 
        
        DEI_ASK_BIT_HIGH               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME    ,
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_header[] = 
    {
        DEI_ASK_HEADER_BIT_LOW_MIN_6MHZ  , 
        DEI_ASK_HEADER_BIT_LOW_MAX_6MHZ  , 
        
        DEI_ASK_HEADER_BIT_HIGH_MIN_6MHZ , 
        DEI_ASK_HEADER_BIT_HIGH_MAX_6MHZ , 
        
        DEI_ASK_BIT_HEADER               ,        
        DEI_ASK_BIT_CHECK_LOW_TIME       ,
    };
    
    
    *(&dei_ask.bit_checker_header)= *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_header));
    *(&dei_ask.bit_checker_low)   = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_low));
    *(&dei_ask.bit_checker_high)  = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_high));        
}

//----------------------------------------------------------------------------//  

void dei_ask_bit_time_set_4MHZ( void )
{
    static const Dei_Ask_Bit_Time_Checker bit_checker_low [] = 
    {
        DEI_ASK_LOW_BIT_LOW_MIN_4MHZ  , 
        DEI_ASK_LOW_BIT_LOW_MAX_4MHZ  , 
        
        DEI_ASK_LOW_BIT_HIGH_MIN_4MHZ , 
        DEI_ASK_LOW_BIT_HIGH_MAX_4MHZ , 
        
        DEI_ASK_BIT_LOW               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME   ,                
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_high[] = 
    {
        DEI_ASK_HIGH_BIT_LOW_MIN_4MHZ  , 
        DEI_ASK_HIGH_BIT_LOW_MAX_4MHZ  , 
        
        DEI_ASK_HIGH_BIT_HIGH_MIN_4MHZ , 
        DEI_ASK_HIGH_BIT_HIGH_MAX_4MHZ , 
        
        DEI_ASK_BIT_HIGH               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME    ,
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_header[] = 
    {
        DEI_ASK_HEADER_BIT_LOW_MIN_4MHZ  , 
        DEI_ASK_HEADER_BIT_LOW_MAX_4MHZ  , 
        
        DEI_ASK_HEADER_BIT_HIGH_MIN_4MHZ , 
        DEI_ASK_HEADER_BIT_HIGH_MAX_4MHZ , 
        
        DEI_ASK_BIT_HEADER               ,        
        DEI_ASK_BIT_CHECK_LOW_TIME       ,
    };
    
    
    *(&dei_ask.bit_checker_header)= *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_header));
    *(&dei_ask.bit_checker_low)   = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_low));
    *(&dei_ask.bit_checker_high)  = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_high));        
}

//----------------------------------------------------------------------------//  

void dei_ask_bit_time_set_3MHZ( void )
{
    static const Dei_Ask_Bit_Time_Checker bit_checker_low [] = 
    {
        DEI_ASK_LOW_BIT_LOW_MIN_3MHZ  , 
        DEI_ASK_LOW_BIT_LOW_MAX_3MHZ  , 
        
        DEI_ASK_LOW_BIT_HIGH_MIN_3MHZ , 
        DEI_ASK_LOW_BIT_HIGH_MAX_3MHZ , 
        
        DEI_ASK_BIT_LOW               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME   ,                
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_high[] = 
    {
        DEI_ASK_HIGH_BIT_LOW_MIN_3MHZ  , 
        DEI_ASK_HIGH_BIT_LOW_MAX_3MHZ  , 
        
        DEI_ASK_HIGH_BIT_HIGH_MIN_3MHZ , 
        DEI_ASK_HIGH_BIT_HIGH_MAX_3MHZ , 
        
        DEI_ASK_BIT_HIGH               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME    ,
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_header[] = 
    {
        DEI_ASK_HEADER_BIT_LOW_MIN_3MHZ  , 
        DEI_ASK_HEADER_BIT_LOW_MAX_3MHZ  , 
        
        DEI_ASK_HEADER_BIT_HIGH_MIN_3MHZ , 
        DEI_ASK_HEADER_BIT_HIGH_MAX_3MHZ , 
        
        DEI_ASK_BIT_HEADER               ,        
        DEI_ASK_BIT_CHECK_LOW_TIME       ,
    };
    
    
    *(&dei_ask.bit_checker_header)= *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_header));
    *(&dei_ask.bit_checker_low)   = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_low));
    *(&dei_ask.bit_checker_high)  = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_high));        
}

//----------------------------------------------------------------------------//  

void dei_ask_bit_time_set_2MHZ( void )
{
    static const Dei_Ask_Bit_Time_Checker bit_checker_low [] = 
    {
        DEI_ASK_LOW_BIT_LOW_MIN_2MHZ  , 
        DEI_ASK_LOW_BIT_LOW_MAX_2MHZ  , 
        
        DEI_ASK_LOW_BIT_HIGH_MIN_2MHZ , 
        DEI_ASK_LOW_BIT_HIGH_MAX_2MHZ , 
        
        DEI_ASK_BIT_LOW               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME   ,                
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_high[] = 
    {
        DEI_ASK_HIGH_BIT_LOW_MIN_2MHZ  , 
        DEI_ASK_HIGH_BIT_LOW_MAX_2MHZ  , 
        
        DEI_ASK_HIGH_BIT_HIGH_MIN_2MHZ , 
        DEI_ASK_HIGH_BIT_HIGH_MAX_2MHZ , 
        
        DEI_ASK_BIT_HIGH               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME    ,
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_header[] = 
    {
        DEI_ASK_HEADER_BIT_LOW_MIN_2MHZ  , 
        DEI_ASK_HEADER_BIT_LOW_MAX_2MHZ  , 
        
        DEI_ASK_HEADER_BIT_HIGH_MIN_2MHZ , 
        DEI_ASK_HEADER_BIT_HIGH_MAX_2MHZ , 
        
        DEI_ASK_BIT_HEADER               ,        
        DEI_ASK_BIT_CHECK_LOW_TIME       ,
    };
    
    
    *(&dei_ask.bit_checker_header)= *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_header));
    *(&dei_ask.bit_checker_low)   = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_low));
    *(&dei_ask.bit_checker_high)  = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_high));        
}

//----------------------------------------------------------------------------//  

void dei_ask_bit_time_set_1MHZ( void )
{
    static const Dei_Ask_Bit_Time_Checker bit_checker_low [] = 
    {
        DEI_ASK_LOW_BIT_LOW_MIN_1MHZ  , 
        DEI_ASK_LOW_BIT_LOW_MAX_1MHZ  , 
        
        DEI_ASK_LOW_BIT_HIGH_MIN_1MHZ , 
        DEI_ASK_LOW_BIT_HIGH_MAX_1MHZ , 
        
        DEI_ASK_BIT_LOW               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME   ,                
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_high[] = 
    {
        DEI_ASK_HIGH_BIT_LOW_MIN_1MHZ  , 
        DEI_ASK_HIGH_BIT_LOW_MAX_1MHZ  , 
        
        DEI_ASK_HIGH_BIT_HIGH_MIN_1MHZ , 
        DEI_ASK_HIGH_BIT_HIGH_MAX_1MHZ , 
        
        DEI_ASK_BIT_HIGH               ,        
        DEI_ASK_BIT_CHECK_HIGH_TIME    ,
    };
    
    static const Dei_Ask_Bit_Time_Checker bit_checker_header[] = 
    {
        DEI_ASK_HEADER_BIT_LOW_MIN_1MHZ  , 
        DEI_ASK_HEADER_BIT_LOW_MAX_1MHZ  , 
        
        DEI_ASK_HEADER_BIT_HIGH_MIN_1MHZ , 
        DEI_ASK_HEADER_BIT_HIGH_MAX_1MHZ , 
        
        DEI_ASK_BIT_HEADER               ,        
        DEI_ASK_BIT_CHECK_LOW_TIME       ,
    };
    
    
    *(&dei_ask.bit_checker_header)= *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_header));
    *(&dei_ask.bit_checker_low)   = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_low));
    *(&dei_ask.bit_checker_high)  = *((Dei_Ask_Bit_Time_Checker*)(&bit_checker_high));        
}

//----------------------------------------------------------------------------//  

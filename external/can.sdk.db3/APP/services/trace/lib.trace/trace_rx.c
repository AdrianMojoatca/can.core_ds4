/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: trace_rx.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "trace_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void trace_key_check( SInt16 key ) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#define RAM(_region_)         extern unsigned Image$$ER_OS_HEAP_##_region_##$$Base
#define RAM_ADDR(_region_)    ((unsigned) (& Image$$ER_OS_HEAP_##_region_##$$Base ))

#if OS_NUM_HEAP_REGIONS >= 1
RAM(0) ;
#endif

#if OS_NUM_HEAP_REGIONS >= 2
RAM (1) ;
#endif

#if OS_NUM_HEAP_REGIONS >= 3
RAM (2) ;
#endif

#if OS_NUM_HEAP_REGIONS >= 4
RAM (3) ;
#endif


extern  unsigned Image$$ER_NVRAM_IMAGE_FLASH_0$$Base ;
#define NVRAM_ADDR0 ((unsigned) (& Image$$ER_NVRAM_IMAGE_FLASH_0$$Base))  

extern  unsigned Image$$ER_NVRAM_IMAGE_FLASH_1$$Base ;
#define NVRAM_ADDR1 ((unsigned) (& Image$$ER_NVRAM_IMAGE_FLASH_1$$Base))  

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#if USE_TRACE == 1

UInt8 trace_rx ( void )
{
    UInt8 key  ; 

    trace_print( ">");

    key = sio_rx( trace_vars.sio );
    
    trace_print( "\n\r");
    
    trace_key_check( key );

    return key ; 
}

/*--------------------------------------------------------------------------*/

SInt16 trace_rx_t ( TskTimeout timeout )
{
    SInt16 key  ; 

    key = sio_rx_t( trace_vars.sio , timeout );

    if( key != 0 )
    {
        trace_print( ">");
        
        trace_key_check( key ) ; 

        trace_print( "\n\r");
    }
                    
    return key ; 
} 
 
/*--------------------------------------------------------------------------*/

void trace_key_check( SInt16 key )
{
    switch( key )
    {
        case TRACE_KEY_OS_STATUS :
        {
          trace_print( NULL ,(void*)trace_print_os_status ) ; 
          break;
        }         
        default : 
        {
          if(trace_vars.func)
          {
            trace_vars.func(key) ;
          } 
          break;
        }
        
    }
}    

/*--------------------------------------------------------------------------*/

#endif

#undef RAM 
#undef RAM_ADDR
#undef NVRAM_ADDR0
#undef NVRAM_ADDR1

/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio.h 32991 2015-07-28 15:42:19Z martin.bouchard $
/*==========================================================================*/

#ifndef __SIO_H__
#define __SIO_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <stdarg.h>
#include "device.h"

#ifndef  Sio_Timeout
#include "os.h"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef Sio_Timeout
#define Sio_Timeout TskTimeout
#endif

//--------------------------------------------------------------------------//

typedef enum
{
  SIO_OK             =  0 ,
  SIO_TIMEOUT        = -1 ,
  SIO_DOMAIN         = -2 ,
  SIO_RX_APPEND_FAIL = -3 ,

  /// !!! This Must Be Last !!! ///

  SIO_STATUS_USER_BASE = -4

} Sio_Status ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order Critical !!!
{
  SIO_CTRL_TX_DRAIN  ,  // Transmit all buffered TX data

  SIO_CTRL_RX_FLUSH  ,  // Discard  all buffered RX data
  SIO_CTRL_RX_APPEND ,  // Append data to RX buffer

  /// !!! This Must Be Last !!! ///

  SIO_CTRL_USER_BASE   // Starting point for derived-type commands

} Sio_Ctrl_Cmd ;

//--------------------------------------------------------------------------//

typedef struct s_Sio * Sio ;

//--------------------------------------------------------------------------//

struct s_Sio
{
  SInt16 (* rx_t)(Sio ,               Sio_Timeout ) ;
  SInt16 (* tx_t)(Sio , SInt16 data , Sio_Timeout ) ;
  SInt16 (* ctrl)(Sio , int    cmd  , va_list args) ;
} ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

EXTERN_C_ENTER

//-[ TX Functions ]---------------------------------------------------------//

SInt16 sio_tx            (Sio sio , SInt16 data                      ) ;                                // Returns 'Sio_Status' value 
SInt16 sio_tx_t          (Sio sio , SInt16 data , Sio_Timeout timeout) ;                                // Returns 'Sio_Status' value

SInt16 sio_tx_drain      (Sio sio                      ) ;                                              // Returns 'Sio_Status' value  
SInt16 sio_tx_drain_t    (Sio sio , Sio_Timeout timeout) ;                                              // Returns 'Sio_Status' value  

SInt16 sio_tx_u8         (Sio sio , UInt8  unum                       ) ;                               // Returns 'Sio_Status' value    
SInt16 sio_tx_u8_t       (Sio sio , UInt8  unum ,  Sio_Timeout timeout) ;                               // Returns 'Sio_Status' value    

SInt16 sio_tx_u16        (Sio sio , UInt16 unum                       ) ;                               // Returns 'Sio_Status' value    
SInt16 sio_tx_u16_t      (Sio sio , UInt16 unum ,  Sio_Timeout timeout) ;                               // Returns 'Sio_Status' value    

SInt16 sio_tx_u32        (Sio sio , UInt32 unum                       ) ;                               // Returns 'Sio_Status' value    
SInt16 sio_tx_u32_t      (Sio sio , UInt32 unum ,  Sio_Timeout timeout) ;                               // Returns 'Sio_Status' value    

SInt16 sio_tx_h8         (Sio sio , UInt8  unum                       ) ;                               // Returns 'Sio_Status' value    
SInt16 sio_tx_h8_t       (Sio sio , UInt8  unum ,  Sio_Timeout timeout) ;                               // Returns 'Sio_Status' value    

SInt16 sio_tx_h16        (Sio sio , UInt16 unum                       ) ;                               // Returns 'Sio_Status' value    
SInt16 sio_tx_h16_t      (Sio sio , UInt16 unum ,  Sio_Timeout timeout) ;                               // Returns 'Sio_Status' value    

SInt16 sio_tx_h32        (Sio sio , UInt32 unum                       ) ;                               // Returns 'Sio_Status' value    
SInt16 sio_tx_h32_t      (Sio sio , UInt32 unum ,  Sio_Timeout timeout) ;                               // Returns 'Sio_Status' value    

SInt16 sio_tx_unsigned   (Sio sio , UInt32 unum , UInt8 base , UInt8 digits                      ) ;    // Returns 'Sio_Status' value    
SInt16 sio_tx_unsigned_t (Sio sio , UInt32 unum , UInt8 base , UInt8 digits , Sio_Timeout timeout) ;    // Returns 'Sio_Status' value    

SInt16 sio_tx_text       (Sio sio , HVA_Const_Char_P text                      ) ;                      // Returns 'Sio_Status' value   
SInt16 sio_tx_text_t     (Sio sio , HVA_Const_Char_P text , Sio_Timeout timeout) ;                      // Returns 'Sio_Status' value   

SInt16 sio_tx_crlf       (Sio sio                      ) ;                                              // Returns 'Sio_Status' value   
SInt16 sio_tx_crlf_t     (Sio sio , Sio_Timeout timeout) ;                                              // Returns 'Sio_Status' value   

//-[ RX Functions ]---------------------------------------------------------//

SInt16 sio_rx            (Sio sio                      ) ;                                              // Returns data value (> 0) or 'Sio_Status' value
SInt16 sio_rx_t          (Sio sio , Sio_Timeout timeout) ;                                              // Returns data value (> 0) or 'Sio_Status' value

SInt16 sio_rx_flush      (Sio sio                      ) ;                                              // Returns 'Sio_Status' value   
SInt16 sio_rx_flush_t    (Sio sio , Sio_Timeout timeout) ;                                              // Returns 'Sio_Status' value   

SInt16 sio_rx_append     (Sio sio , SInt16 data                      ) ;                                // Returns 'Sio_Status' value   
SInt16 sio_rx_append_t   (Sio sio , SInt16 data , Sio_Timeout timeout) ;                                // Returns 'Sio_Status' value   

//-[ CTRL Functions ]-------------------------------------------------------//

SInt16 sio_ctrl          (Sio sio , int cmd  , ...) ;                                                   // Device specific command , argument(s) , and return value

EXTERN_C_LEAVE

/*==========================================================================*/

#endif


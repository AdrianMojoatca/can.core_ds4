/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: key_dbg_print.c 16273 2012-07-31 13:01:18Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "key_private.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void key_dbg_print (void)           
{ 
  UInt8 i ;
  
  dbg_tx_crlf() ;
  dbg_tx_text("Pg Data") ;
  
  #define PRINT_PAGE(z)   dbg_tx_crlf() ; dbg_tx_u(z,2) ;dbg_tx_space() ;
//  #define PRINT_STATUS(z) dbg_tx_text(((key_memory.lock_status >> (z - 1)) & 1)? "   LOCK   ": "   UNLOCK ") 
   
  
  PRINT_PAGE(1) ; 
  dbg_tx_h8(key_memory.sel_addr) ;
 
  PRINT_PAGE(2) ; 
  dbg_tx_h8(key_memory.usr_data1) ;
 
  PRINT_PAGE(3) ; 
  for(i = 0  ; i < sizeof_array(key_memory.uid.byte) ; i++)
    {
      dbg_tx_h8(key_memory.uid.byte[i]) ;
    }

  PRINT_PAGE(4) ; 
  for(i = 0 ; i < sizeof_array(key_memory.isk) ; i++)
    {
      dbg_tx_h8(key_memory.isk[i]) ;
      
    }    

  PRINT_PAGE(16) ; 
  for(i = 0 ; i < sizeof_array(key_memory.tic) ; i++)
    {
      dbg_tx_h8(key_memory.tic[i]) ;
      
    }    
      
  PRINT_PAGE(30) ; 
  dbg_tx_h8(key_memory.config.byte[0]) ;
  dbg_tx_h8(key_memory.config.byte[1]) ;
 


  // UInt8 sel_addr        ; /* Select Adress */
  // UInt8 usr_data1       ; /* User Data 1 */
  // UInt8 uid        [4]  ; /* Unique Identification (3 byte serial number +  1 byte manuf. code) */
  // UInt8 isk        [10] ; /* Encryption key 40 bits dst40 and 80bit dst 80 */
  // UInt8 mak        [10] ; /* Mutual authent. key */
  // UInt8 usr_data2  [5] ; /* User Data 2 */
  // UInt8 usr_data3  [5] ; /* User Data 3 */
  // UInt8 usr_data4  [5] ; /* User Data 4 */
  // UInt8 usr_data5  [5] ; /* User Data 5 */
  // UInt8 usr_data6  [5] ; /* User Data 6 */
  // UInt8 usr_data7  [5] ; /* User Data 7 */
  // UInt8 usr_data8  [5] ; /* User Data 8 */
  // UInt8 usr_data9  [5] ; /* User Data 9 */
  // UInt8 push_mask      ; /* counter byte */
  
  // Dst_Func_Def config ; /* configuration 1 byte for counter + 1 byte for function */

  // UInt64 lock_status  ; /* Select Adress */  // 1 bit = 1 page (page1 = bit 0...)


 
}





/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dstcrypt.c 16986 2012-09-02 18:08:42Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dstcrypt_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// Encryption time  without optimisation = 330 ms
// First optimisation                    = 230 ms


#define vars_4(x)   dst_table4  [x]
#define vars_32(x)  dst_table32 [x]
#define vars_60(x)  dst_table60 [x]
#define vars_88(x)  dst_table88 [x]
#define vars_116(x) dst_table116[x]
#define vars_144(x) dst_table144[x]
#define vars_172(x) dst_table172[x]

//--------------------------------------------------------------------------//

#define PROC_0C(a) (a = ((a & 0xA5) | ((a & 0x50)>> 3) | ((a & 0x0A)<< 3)))
#define PROC_94(result , a, b) (result = (a & 0xF0)       |  (b >> 4) & 0x0F)
#define PROC_5C(result , a, b) (result =((a << 4) & 0xF0) | ((b >> 4) & 0x0F))
#define PROC_04(a)(a ^= 255)
#define PROC_7C(a)(a >>= 1)

#define TEST(a, b)           ((a >> b) & 1)            /* convert boolean to 00h/01h */

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

UInt8 vars[55] ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void dstcrypt(UInt8 * addr , Dst_Device_Type device_type)
{  
  UInt8 var_C0 ;
  UInt8 var_C4 ;

  dstcrypt_nvfs_entry.device_type = device_type ;

  vars[0] = addr[0] ;
  vars[1] = addr[1] ;
  vars[2] = addr[2] ;
  vars[3] = addr[3] ;
  vars[4] = addr[4] ;
        
  vars[5] = dstcrypt_nvfs_entry.isk[0] ;
  vars[6] = dstcrypt_nvfs_entry.isk[1] ;
  vars[7] = dstcrypt_nvfs_entry.isk[2] ;
  vars[8] = dstcrypt_nvfs_entry.isk[3] ;
  vars[9] = dstcrypt_nvfs_entry.isk[4] ;

  vars[20] = dstcrypt_nvfs_entry.isk[DSTCRYPT_KEY40_SIZE + 0] ;
  vars[21] = dstcrypt_nvfs_entry.isk[DSTCRYPT_KEY40_SIZE + 1] ;
  vars[22] = dstcrypt_nvfs_entry.isk[DSTCRYPT_KEY40_SIZE + 2] ;
  vars[23] = dstcrypt_nvfs_entry.isk[DSTCRYPT_KEY40_SIZE + 3] ;
  vars[24] = dstcrypt_nvfs_entry.isk[DSTCRYPT_KEY40_SIZE + 4] ;


  vars[10] = vars[5] ;
  vars[11] = vars[6] ;
  vars[12] = vars[7] ;
  vars[13] = vars[8] ;
  vars[14] = vars[9] ;
  vars[15] = vars[5] ;
  vars[16] = vars[6] ;
  vars[17] = vars[7] ;
  vars[18] = vars[8] ;
  vars[19] = vars[9] ;
  vars[25] = vars[20] ;
  vars[26] = vars[21] ;
  vars[27] = vars[22] ;
  vars[28] = vars[23] ;
  vars[29] = vars[24] ;

  vars[51] = 2 ;

  for(var_C0 = 1 ;  var_C0 <= 200 ; var_C0++)
    {
      if(device_type == DST_80)
        {
          if (vars[19] & 128)
          {
            PROC_04(vars[19]) ;
            PROC_04(vars[18]) ;
            PROC_04(vars[17]) ;
            PROC_04(vars[16]) ;
            PROC_04(vars[15]) ;
            vars[19] = (vars[19] | 128)          ;
          }
          
          PROC_0C(vars[19])   ;
          PROC_0C(vars[18])   ;
          PROC_0C(vars[17])   ;
          PROC_0C(vars[16])   ;
          PROC_0C(vars[15])   ;
          
          if (vars[29] & 128)
          {
            PROC_04(vars[29]) ;
            PROC_04(vars[28]) ;
            PROC_04(vars[27]) ;
            PROC_04(vars[26]) ;
            PROC_04(vars[25]) ;
            vars[29] = (vars[29] | 128)          ;
          }
         
          PROC_0C(vars[29])   ;
          PROC_0C(vars[28])   ;
          PROC_0C(vars[27])   ;
          PROC_0C(vars[26])   ;
          PROC_0C(vars[25])   ;
          
          PROC_5C(vars[15],vars[18], vars[17]) ;
          PROC_5C(vars[16],vars[19], vars[18]) ;
          PROC_94(vars[17],vars[27], vars[19]) ;
          vars[18] = vars[28]                              ;
          vars[19] = vars[29]                              ;
        }

      vars[5] = vars[15] ;
      vars[6] = vars[16] ;
      vars[7] = vars[17] ;
      vars[8] = vars[18] ;
      vars[9] = vars[19] ;
//      vars[30] = 0         ;
//      vars[31] = 0         ;
//      vars[32] = 0         ;
//      vars[33] = 0         ;
//      vars[34] = 0         ;
//      vars[35] = 0         ;
//      vars[36] = 0         ;
//      vars[37] = 0         ;
//      vars[38] = 0         ;
//      vars[39] = 0         ;
//      vars[40] = 0         ;
//      vars[41] = 0         ;
//      vars[42] = 0         ;
//      vars[43] = 0         ;
//      vars[44] = 0         ;
//      vars[45] = 0         ;
//      vars[46] = 0         ;
//      vars[47] = 0         ;
//      vars[48] = 0         ;
//      vars[49] = 0         ;
//      vars[50] = 0         ;

      vars[45]  = (TEST(vars[4] , 0) << 4) ; //if (vars[4] & 1)           {vars[45] = (vars[45] | 0x10) ; }  
      vars[45] |= (TEST(vars[9] , 0) << 3) ; //if (vars[9] & 1)           {vars[45] = (vars[45] | 8)    ; }  
      vars[45] |= (TEST(vars[3] , 0) << 2) ; //if (vars[3] & 1)           {vars[45] = (vars[45] | 4)    ; }  
      vars[45] |= (TEST(vars[8] , 0) << 1) ; //if (vars[8] & 1)           {vars[45] = (vars[45] | 2)    ; }  
      vars[45] |= (TEST(vars[2] , 0) << 0) ; //if (vars[2] & 1)           {vars[45] = (vars[45] | 1)    ; }  

      vars[44]  = (TEST(vars[7] , 0) << 3) ; //if (vars[7] & 1)           {vars[44] = (vars[44] | 8)    ; }   
      vars[44] |= (TEST(vars[1] , 0) << 2) ; //if (vars[1] & 1)           {vars[44] = (vars[44] | 4)    ; }   
      vars[44] |= (TEST(vars[6] , 0) << 1) ; //if (vars[6] & 1)           {vars[44] = (vars[44] | 2)    ; }   
      vars[44] |= (TEST(vars[5] , 0) << 0) ; //if (vars[5] & 1)           {vars[44] = (vars[44] | 1)    ; }  

      vars[43]  = (TEST(vars[4] , 1) << 4) ; //if (vars[4] & 2)           {vars[43] = (vars[43] | 0x10) ; } 
      vars[43] |= (TEST(vars[9] , 1) << 3) ; //if (vars[9] & 2)           {vars[43] = (vars[43] | 8)    ; } 
      vars[43] |= (TEST(vars[3] , 1) << 2) ; //if (vars[3] & 2)           {vars[43] = (vars[43] | 4)    ; } 
      vars[43] |= (TEST(vars[8] , 1) << 1) ; //if (vars[8] & 2)           {vars[43] = (vars[43] | 2)    ; } 
      vars[43] |= (TEST(vars[2] , 1) << 0) ; //if (vars[2] & 2)           {vars[43] = (vars[43] | 1)    ; } 
              
      vars[42]  = (TEST(vars[7] , 1) << 3) ; //if (vars[7] & 2)           {vars[42] = (vars[42] | 8)    ; }   
      vars[42] |= (TEST(vars[1] , 1) << 2) ; //if (vars[1] & 2)           {vars[42] = (vars[42] | 4)    ; }   
      vars[42] |= (TEST(vars[6] , 1) << 1) ; //if (vars[6] & 2)           {vars[42] = (vars[42] | 2)    ; }   
      vars[42] |= (TEST(vars[5] , 1) << 0) ; //if (vars[5] & 2)           {vars[42] = (vars[42] | 1)    ; }  
       
      vars[41]  = (TEST(vars[4] , 2) << 4) ; //if (vars[4] & 4)           {vars[41] = (vars[41] | 0x10) ; }  
      vars[41] |= (TEST(vars[9] , 2) << 3) ; //if (vars[9] & 4)           {vars[41] = (vars[41] | 8)    ; }  
      vars[41] |= (TEST(vars[3] , 2) << 2) ; //if (vars[3] & 4)           {vars[41] = (vars[41] | 4)    ; }  
      vars[41] |= (TEST(vars[8] , 2) << 1) ; //if (vars[8] & 4)           {vars[41] = (vars[41] | 2)    ; }  
      vars[41] |= (TEST(vars[2] , 2) << 0) ; //if (vars[2] & 4)           {vars[41] = (vars[41] | 1)    ; }  
             
      vars[40]  = (TEST(vars[7] , 2) << 4) ; //if (vars[7] & 4)           {vars[40] = (vars[40] | 0x10) ; } 
      vars[40] |= (TEST(vars[1] , 2) << 3) ; //if (vars[1] & 4)           {vars[40] = (vars[40] | 8)    ; }   
      vars[40] |= (TEST(vars[6] , 2) << 2) ; //if (vars[6] & 4)           {vars[40] = (vars[40] | 4)    ; }   
      vars[40] |= (TEST(vars[0] , 2) << 1) ; //if (vars[0] & 4)           {vars[40] = (vars[40] | 2)    ; }   
      vars[40] |= (TEST(vars[5] , 2) << 0) ; //if (vars[5] & 4)           {vars[40] = (vars[40] | 1)    ; }   

      vars[39]  = (TEST(vars[4] , 3) << 4) ; //if (vars[4] & 8)           {vars[39] = (vars[39] | 0x10) ; }   
      vars[39] |= (TEST(vars[9] , 3) << 3) ; //if (vars[9] & 8)           {vars[39] = (vars[39] | 8)    ; }   
      vars[39] |= (TEST(vars[3] , 3) << 2) ; //if (vars[3] & 8)           {vars[39] = (vars[39] | 4)    ; }   
      vars[39] |= (TEST(vars[8] , 3) << 1) ; //if (vars[8] & 8)           {vars[39] = (vars[39] | 2)    ; }   
      vars[39] |= (TEST(vars[2] , 3) << 0) ; //if (vars[2] & 8)           {vars[39] = (vars[39] | 1)    ; }   

      vars[38]  = (TEST(vars[7] , 3) << 4) ; //if (vars[7] & 8)           {vars[38] = (vars[38] | 0x10) ; }      
      vars[38] |= (TEST(vars[1] , 3) << 3) ; //if (vars[1] & 8)           {vars[38] = (vars[38] | 8)    ; }   
      vars[38] |= (TEST(vars[6] , 3) << 2) ; //if (vars[6] & 8)           {vars[38] = (vars[38] | 4)    ; }   
      vars[38] |= (TEST(vars[0] , 3) << 1) ; //if (vars[0] & 8)           {vars[38] = (vars[38] | 2)    ; }   
      vars[38] |= (TEST(vars[5] , 3) << 0) ; //if (vars[5] & 8)           {vars[38] = (vars[38] | 1)    ; }   

      vars[37]  = (TEST(vars[4] , 4) << 4) ; //if (vars[4] & 0x10)        {vars[37] = (vars[37] | 0x10) ; }   
      vars[37] |= (TEST(vars[9] , 4) << 3) ; //if (vars[9] & 0x10)        {vars[37] = (vars[37] | 8)    ; }   
      vars[37] |= (TEST(vars[3] , 4) << 2) ; //if (vars[3] & 0x10)        {vars[37] = (vars[37] | 4)    ; }   
      vars[37] |= (TEST(vars[8] , 4) << 1) ; //if (vars[8] & 0x10)        {vars[37] = (vars[37] | 2)    ; }   
      vars[37] |= (TEST(vars[2] , 4) << 0) ; //if (vars[2] & 0x10)        {vars[37] = (vars[37] | 1)    ; }   

      vars[36]  = (TEST(vars[7] , 4) << 4) ; //f (vars[7] & 0x10)        {vars[36] = (vars[36] | 0x10) ; }      
      vars[36] |= (TEST(vars[1] , 4) << 3) ; //f (vars[1] & 0x10)        {vars[36] = (vars[36] | 8)    ; }   
      vars[36] |= (TEST(vars[6] , 4) << 2) ; //f (vars[6] & 0x10)        {vars[36] = (vars[36] | 4)    ; }   
      vars[36] |= (TEST(vars[0] , 4) << 1) ; //f (vars[0] & 0x10)        {vars[36] = (vars[36] | 2)    ; }   
      vars[36] |= (TEST(vars[5] , 4) << 0) ; //f (vars[5] & 0x10)        {vars[36] = (vars[36] | 1)    ; } 
        
      vars[35]  = (TEST(vars[4] , 5) << 4) ; //if (vars[4] & 0x20)        {vars[35] = (vars[35] | 0x10) ; }      
      vars[35] |= (TEST(vars[9] , 5) << 3) ; //if (vars[9] & 0x20)        {vars[35] = (vars[35] | 8)    ; }   
      vars[35] |= (TEST(vars[3] , 5) << 2) ; //if (vars[3] & 0x20)        {vars[35] = (vars[35] | 4)    ; }   
      vars[35] |= (TEST(vars[8] , 5) << 1) ; //if (vars[8] & 0x20)        {vars[35] = (vars[35] | 2)    ; }   
      vars[35] |= (TEST(vars[2] , 5) << 0) ; //if (vars[2] & 0x20)        {vars[35] = (vars[35] | 1)    ; } 
        
      vars[34]  = (TEST(vars[7] , 5) << 4) ; //if (vars[7] & 0x20)        {vars[34] = (vars[34] | 0x10) ; }      
      vars[34] |= (TEST(vars[1] , 5) << 3) ; //if (vars[1] & 0x20)        {vars[34] = (vars[34] | 8)    ; }   
      vars[34] |= (TEST(vars[6] , 5) << 2) ; //if (vars[6] & 0x20)        {vars[34] = (vars[34] | 4)    ; }   
      vars[34] |= (TEST(vars[0] , 5) << 1) ; //if (vars[0] & 0x20)        {vars[34] = (vars[34] | 2)    ; }   
      vars[34] |= (TEST(vars[5] , 5) << 0) ; //if (vars[5] & 0x20)        {vars[34] = (vars[34] | 1)    ; } 
        
      vars[33]  = (TEST(vars[4] , 6) << 4) ; //if (vars[4] & 0x40)        {vars[33] = (vars[33] | 0x10) ; }      
      vars[33] |= (TEST(vars[9] , 6) << 3) ; //if (vars[9] & 0x40)        {vars[33] = (vars[33] | 8)    ; }   
      vars[33] |= (TEST(vars[3] , 6) << 2) ; //if (vars[3] & 0x40)        {vars[33] = (vars[33] | 4)    ; }   
      vars[33] |= (TEST(vars[8] , 6) << 1) ; //if (vars[8] & 0x40)        {vars[33] = (vars[33] | 2)    ; }   
      vars[33] |= (TEST(vars[2] , 6) << 0) ; //if (vars[2] & 0x40)        {vars[33] = (vars[33] | 1)    ; } 
        
      vars[32]  = (TEST(vars[7] , 6) << 4) ; //if (vars[7] & 0x40)        {vars[32] = (vars[32] | 0x10) ; }      
      vars[32] |= (TEST(vars[1] , 6) << 3) ; //if (vars[1] & 0x40)        {vars[32] = (vars[32] | 8)    ; }   
      vars[32] |= (TEST(vars[6] , 6) << 2) ; //if (vars[6] & 0x40)        {vars[32] = (vars[32] | 4)    ; }   
      vars[32] |= (TEST(vars[0] , 6) << 1) ; //if (vars[0] & 0x40)        {vars[32] = (vars[32] | 2)    ; }   
      vars[32] |= (TEST(vars[5] , 6) << 0) ; //if (vars[5] & 0x40)        {vars[32] = (vars[32] | 1)    ; } 
        
      vars[31]  = (TEST(vars[4] , 7) << 4) ; //if (vars[4] & 128)         {vars[31] = (vars[31] | 0x10) ; }      
      vars[31] |= (TEST(vars[9] , 7) << 3) ; //if (vars[9] & 128)         {vars[31] = (vars[31] | 8)    ; }   
      vars[31] |= (TEST(vars[3] , 7) << 2) ; //if (vars[3] & 128)         {vars[31] = (vars[31] | 4)    ; }   
      vars[31] |= (TEST(vars[8] , 7) << 1) ; //if (vars[8] & 128)         {vars[31] = (vars[31] | 2)    ; }   
      vars[31] |= (TEST(vars[2] , 7) << 0) ; //if (vars[2] & 128)         {vars[31] = (vars[31] | 1)    ; } 
        
      vars[30]  = (TEST(vars[7] , 7) << 4) ; //if (vars[7] & 128)         {vars[30] = (vars[30] | 0x10) ; }      
      vars[30] |= (TEST(vars[1] , 7) << 3) ; //if (vars[1] & 128)         {vars[30] = (vars[30] | 8)    ; }   
      vars[30] |= (TEST(vars[6] , 7) << 2) ; //if (vars[6] & 128)         {vars[30] = (vars[30] | 4)    ; }   
      vars[30] |= (TEST(vars[0] , 7) << 1) ; //if (vars[0] & 128)         {vars[30] = (vars[30] | 2)    ; }   
      vars[30] |= (TEST(vars[5] , 7) << 0) ; //if (vars[5] & 128)         {vars[30] = (vars[30] | 1)    ; }   

      vars[46]  = ((vars_4  (vars[30]) & 1) << 3) ; //if (vars_4(vars[30]) )       {vars[46] = (vars[46] | 8)    ; }      
      vars[46] |= ((vars_32 (vars[31]) & 1) << 2) ; //if (vars_32(vars[31]))       {vars[46] = (vars[46] | 4)    ; }      
      vars[46] |= ((vars_60 (vars[32]) & 1) << 1) ; //if (vars_60(vars[32]))       {vars[46] = (vars[46] | 2)    ; }      
      vars[46] |= ((vars_88 (vars[33]) & 1) << 0) ; //if (vars_88(vars[33]))       {vars[46] = (vars[46] | 1)    ; }
                            
      vars[47]  = ((vars_4  (vars[34]) & 1) << 3) ; //if (vars_4(vars[34]) )       {vars[47] = (vars[47] | 8)    ; }      
      vars[47] |= ((vars_32 (vars[35]) & 1) << 2) ; //if (vars_32(vars[35]))       {vars[47] = (vars[47] | 4)    ; }      
      vars[47] |= ((vars_60 (vars[36]) & 1) << 1) ; //if (vars_60(vars[36]))       {vars[47] = (vars[47] | 2)    ; }      
      vars[47] |= ((vars_88 (vars[37]) & 1) << 0) ; //if (vars_88(vars[37]))       {vars[47] = (vars[47] | 1)    ; } 
                                                                                          
      vars[48]  = ((vars_4  (vars[38]) & 1) << 3) ; //if (vars_4(vars[38]) )       {vars[48] = (vars[48] | 8)    ; }      
      vars[48] |= ((vars_32 (vars[39]) & 1) << 2) ; //if (vars_32(vars[39]))       {vars[48] = (vars[48] | 4)    ; }      
      vars[48] |= ((vars_60 (vars[40]) & 1) << 1) ; //if (vars_60(vars[40]))       {vars[48] = (vars[48] | 2)    ; }      
      vars[48] |= ((vars_88 (vars[41]) & 1) << 0) ; //if (vars_88(vars[41]))       {vars[48] = (vars[48] | 1)    ; }   
                                                                       
      vars[49]  = ((vars_116(vars[42]) & 1) << 3) ; //if (vars_116(vars[42]))      {vars[49] = (vars[49] | 8)    ; }      
      vars[49] |= ((vars_32 (vars[43]) & 1) << 2) ; //if (vars_32(vars[43]))       {vars[49] = (vars[49] | 4)    ; }      
      vars[49] |= ((vars_116(vars[44]) & 1) << 1) ; //if (vars_116(vars[44]))      {vars[49] = (vars[49] | 2)    ; }      
      vars[49] |= ((vars_88 (vars[45]) & 1) << 0) ; //if (vars_88(vars[45]))       {vars[49] = (vars[49] | 1)    ; }   
         
      // Warning the shift is reverse here
      vars[50]  = ((vars_144(vars[46]) & 1) << 0) ; //if (vars_144(vars[46]))      {vars[50] = (vars[50] | 1)    ; }      
      vars[50] |= ((vars_144(vars[47]) & 1) << 1) ; //if (vars_144(vars[47]))      {vars[50] = (vars[50] | 2)    ; }      
      vars[50] |= ((vars_144(vars[48]) & 1) << 2) ; //if (vars_144(vars[48]))      {vars[50] = (vars[50] | 4)    ; }      
      vars[50] |= ((vars_144(vars[49]) & 1) << 3) ; //if (vars_144(vars[49]))      {vars[50] = (vars[50] | 8)    ; }      
      
      vars[52] = vars_172(vars[50])       ;
      vars[52] = vars[52] ^ (vars[0] & 3) ;
      
      for (var_C4 = 0 ; var_C4 <= 1 ; var_C4++) 
      {
        PROC_7C(vars[0]) ;

        vars[0] |= (TEST(vars[1] , 0) << 7) ; //if (vars[1] & 1)          {vars[0] = (vars[0] | 128) ; }
                                                                   
        PROC_7C(vars[1]) ;                  
                                                                   
        vars[1] |= (TEST(vars[2] , 0) << 7) ; //if (vars[2] & 1)          {vars[1] = (vars[1] | 128) ; }
                                                                   
        PROC_7C(vars[2]) ;                  
                                                                   
        vars[2] |= (TEST(vars[3] , 0) << 7) ; //if (vars[3] & 1)          {vars[2] = (vars[2] | 128) ; }
                                                                   
        PROC_7C(vars[3]) ;                  
                                                                   
        vars[3] |= (TEST(vars[4] , 0) << 7) ; //if (vars[4] & 1)          {vars[3] = (vars[3] | 128) ; }
                                                                   
        PROC_7C(vars[4]) ;                  
                                                                   
        vars[4] |= (TEST(vars[52], 0) << 7) ; //if (vars[52] & 1)          {vars[4] = (vars[4] | 128) ; }
        
        PROC_7C(vars[52]) ;
      }//Next var_C4 'Integer
      
      
      vars[51]-- ;//(vars[51] - 1) ;

      if(device_type == DST_80)
        {
          vars[51] = 0 ;
        }

      if (vars[51] == 0)
      {
        vars[53] = 0 ;

        vars[53] ^= TEST(vars[10], 0) ; // if (vars[10] & 1)          {vars[53] = vars[53] ^ 1 ; }  
        vars[53] ^= TEST(vars[10], 2) ; // if (vars[10] & 4)          {vars[53] = vars[53] ^ 1 ; }        
        vars[53] ^= TEST(vars[12], 3) ; // if (vars[12] & 8)          {vars[53] = vars[53] ^ 1 ; }  
        vars[53] ^= TEST(vars[12], 5) ; // if (vars[12] & 0x20)       {vars[53] = vars[53] ^ 1 ; }  
             
        PROC_7C(vars[10])                   ; 

        vars[10] |= (TEST(vars[11] , 0) << 7) ; //if (vars[11] & 1)          {vars[10] = (vars[10] | 128); } 
             
        PROC_7C(vars[11])                   ;

        vars[11] |= (TEST(vars[12] , 0) << 7) ; //if (vars[12] & 1)          {vars[11] = (vars[11] | 128); } 
             
        PROC_7C(vars[12])                   ;

        vars[12] |= (TEST(vars[13] , 0) << 7) ; //if (vars[13] & 1)          {vars[12] = (vars[12] | 128); } 
             
        PROC_7C(vars[13])                   ;

        vars[13] |= (TEST(vars[14] , 0) << 7) ; //if (vars[14] & 1)          {vars[13] = (vars[13] | 128); }  
            
        PROC_7C(vars[14])                   ;

        vars[14] |= (TEST(vars[53] , 0) << 7) ; //if (vars[53])                {vars[14] = (vars[14] | 128); }
        
        vars[15] = vars[10] ;
        vars[16] = vars[11] ;
        vars[17] = vars[12] ;
        vars[18] = vars[13] ;
        vars[19] = vars[14] ;
        
//'************************* 80 bits Start ****************************************
        vars[54] = 0         ;

        vars[54] ^= TEST(vars[20], 0) ; // if (vars[20] & 1)           {vars[54] = vars[54] ^ 1 ;}         
        vars[54] ^= TEST(vars[20], 2) ; // if (vars[20] & 4)           {vars[54] = vars[54] ^ 1 ;}         
        vars[54] ^= TEST(vars[22], 3) ; // if (vars[22] & 8)           {vars[54] = vars[54] ^ 1 ;}         
        vars[54] ^= TEST(vars[22], 5) ; // if (vars[22] & 0x20)        {vars[54] = vars[54] ^ 1 ;} 
        
        PROC_7C(vars[20])                     ;

        vars[20] |= (TEST(vars[21] , 0) << 7) ; //if (vars[21] & 1)           {vars[20] = (vars[20] | 128) ;}
                                                                    
        PROC_7C(vars[21])                     ;

        vars[21] |= (TEST(vars[22] , 0) << 7) ; //if (vars[22] & 1)           {vars[21] = (vars[21] | 128) ;}
                                                                    
        PROC_7C(vars[22])                     ;

        vars[22] |= (TEST(vars[23] , 0) << 7) ; //if (vars[23] & 1)           {vars[22] = (vars[22] | 128) ;}
                                                                    
        PROC_7C(vars[23])                     ;

        vars[23] |= (TEST(vars[24] , 0) << 7) ; //if (vars[24] & 1)           {vars[23] = (vars[23] | 128) ;}
                                                                    
        PROC_7C(vars[24])                     ;

        vars[24] |= (TEST(vars[54] , 0) << 7) ; //if (vars[54])                 {vars[24] = (vars[24] | 128) ;}
        
        vars[25] = vars[20] ;
        vars[26] = vars[21] ;
        vars[27] = vars[22] ;
        vars[28] = vars[23] ;
        vars[29] = vars[24] ;

//'************************* 80 bits End ****************************************

        vars[51] = 3 ;
      }//End If
    } // for loop var_C0


  addr[0] = vars[0]   ;
  addr[1] = vars[1]   ;
  addr[2] = vars[2]   ;
  addr[3] = vars[3]   ;
  addr[4] = vars[4]   ;


}

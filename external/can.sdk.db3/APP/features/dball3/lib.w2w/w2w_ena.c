/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: w2w_dis.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "w2w_private.h"
#include "bitfield.h"
#include "nvfs_usr.h"
#include "d2d_code.h"

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
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void w2w_ena (UInt8 d2d_code)
{
    
 W2W_Ena_Bit input ;

 switch(d2d_code)
   {
     case FN_GWR_OFF            : input = W2W_ENA_GWR    ; break ;
     case FN_GWR_ON             : input = W2W_ENA_GWR    ; break ;
                              
     case FN_LOCK_REM1_ON       : input = W2W_ENA_LOCK   ; break ;
     case FN_LOCK_REM2_ON       : input = W2W_ENA_LOCK   ; break ;
     case FN_LOCK_REM3_ON       : input = W2W_ENA_LOCK   ; break ;
                          
     case FN_UNLOCK_DR_REM1_ON  : input = W2W_ENA_UNLOCK ; break ;
     case FN_UNLOCK_DR_REM2_ON  : input = W2W_ENA_UNLOCK ; break ;
     case FN_UNLOCK_DR_REM3_ON  : input = W2W_ENA_UNLOCK ; break ;
     case FN_UNLOCK_ALL_REM1_ON : input = W2W_ENA_UNLOCK ; break ;
     case FN_UNLOCK_ALL_REM2_ON : input = W2W_ENA_UNLOCK ; break ;
     case FN_UNLOCK_ALL_REM3_ON : input = W2W_ENA_UNLOCK ; break ;
                             
     case FN_TRUNK_REM1_ON      : input = W2W_ENA_TRUNK  ; break ;
     case FN_TRUNK_REM2_ON      : input = W2W_ENA_TRUNK  ; break ;
     case FN_TRUNK_REM3_ON      : input = W2W_ENA_TRUNK  ; break ;
                              
     case FN_AUX1_REM1_ON       : input = W2W_ENA_AUX1   ; break ;
     case FN_AUX1_REM2_ON       : input = W2W_ENA_AUX1   ; break ;
     case FN_AUX1_REM3_ON       : input = W2W_ENA_AUX1   ; break ;
                                
     case FN_AUX2_REM1_ON       : input = W2W_ENA_AUX2   ; break ;
     case FN_AUX2_REM2_ON       : input = W2W_ENA_AUX2   ; break ;
     case FN_AUX2_REM3_ON       : input = W2W_ENA_AUX2   ; break ;
                                
     case FN_AUX3_REM1_ON       : input = W2W_ENA_AUX3   ; break ;
     case FN_AUX3_REM2_ON       : input = W2W_ENA_AUX3   ; break ;
     case FN_AUX3_REM3_ON       : input = W2W_ENA_AUX3   ; break ;
                                
     case FN_AUX4_REM1_ON       : input = W2W_ENA_AUX4   ; break ;      
     case FN_AUX4_REM2_ON       : input = W2W_ENA_AUX4   ; break ;      
     case FN_AUX4_REM3_ON       : input = W2W_ENA_AUX4   ; break ;  

   default : return ;    

   }
 
 
  bitfield_wr_u(&w2w_ena_table  , input , input , W2W_ON) ;

  nvfs_wr(nvfs_w2w_ena, &w2w_ena_table) ;
  
  #if DBG_W2W == 1
    TRACE("\nW2W Disable Code: %X" , d2d_code) ;
  #endif  

}

//--------------------------------------------------------------------------//






/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_save.c 44556 2016-11-29 13:03:49Z martin.bouchard $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

 #include "hobp_private.h"
 #include "nvram_usr.h"

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
Boolean hobp_save (void)
{  
  NVFS_Status status  ;

  #if DBG_HOBP == 1
    hobp_dbg_print("\nHoBp SAVING DATA... " , NULL , NULL) ;

  #endif

  if((nvfs_wr(nvfs_hobp, &hobp_vars.nvfs_entry)== NVFS_OK) && hobp_err_save())
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Save Done", NULL, NULL) ;
//        hobp_dbg_print("\nserial    :" , &hobp_vars.nvfs_entry.serial        [0]    ,sizeof(hobp_vars.nvfs_entry.serial    )) ;
//        hobp_dbg_print("\nstatus    :" , (UInt8 *)hobp_vars.nvfs_entry.status[0]    ,sizeof(hobp_vars.nvfs_entry.status    )) ;
//        hobp_dbg_print("\nchallenge :" , &hobp_vars.nvfs_entry.challenge     [0][0] ,sizeof(hobp_vars.nvfs_entry.challenge )) ;
//        hobp_dbg_print("\nresponse  :" , &hobp_vars.nvfs_entry.response      [0][0] ,sizeof(hobp_vars.nvfs_entry.response  )) ;
//        hobp_dbg_print("\nu8Tab1Col1:" , &hobp_vars.nvfs_entry.u8Tab1Col1    [0]    ,sizeof(hobp_vars.nvfs_entry.u8Tab1Col1)) ;
//        hobp_dbg_print("\nu8Tab1    :" , &hobp_vars.nvfs_entry.u8Tab1        [0][0] ,sizeof(hobp_vars.nvfs_entry.u8Tab1    )) ;
//        hobp_dbg_print("\nprog_flags:" , (UInt8 *)&hobp_vars.nvfs_entry.prog_flags  ,sizeof(hobp_vars.nvfs_entry.prog_flags)) ;
      #endif

      return TRUE ;
    }
  else
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Save FAIL", NULL, NULL) ;
      #endif

      return FALSE ;
    }
}


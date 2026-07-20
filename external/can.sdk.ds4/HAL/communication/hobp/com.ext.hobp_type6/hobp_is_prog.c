/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_is_prog.c 26504 2014-08-01 17:24:42Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define CHALLENGE(x) hobp_vars.nvfs_entry.challenge[x][0]
#define REPSONSE(x)  hobp_vars.nvfs_entry.response [x][0]
#define IMO_SEQ(x)   hobp_vars.nvfs_entry.status   [x][0]

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                                      
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

Boolean hobp_is_prog(void)
{
  if(!hobp_vars.is_prog) //  && hobp_log_is_done()
    {      
      if(nvfs_size_is_valid(nvfs_hobp_isk , HOBP_ISK_SIZE) == NVFS_OK)
        {
          if(nvfs_rd(nvfs_hobp_isk , &hobp_vars.isk) == NVFS_OK)
            {
              convert_isk_to_table(&IMO_SEQ(0) , &hobp_vars.isk[0]) ;
 
              #if DBG_HOBP == 1
                hobp_dbg_print ("\nIsk Load Ok " , &hobp_vars.isk[0] , HOBP_ISK_SIZE) ;
              #endif
            }
        }

      
      hobp_vars.is_prog = (getPolynome(&CHALLENGE(0) , &REPSONSE(0) , &CHALLENGE(1) , &REPSONSE(1) , &IMO_SEQ(0)) == OK) ;
      
      if (hobp_vars.is_prog)//(getPolynome(&CHALLENGE(0) , &REPSONSE(0) , &CHALLENGE(1) , &REPSONSE(1) , &IMO_SEQ(0)) == OK)
        {          
          hobp_set_flag(HOBP_PROG_FLAG_POLY ) ;
          
          #if DBG_HOBP == 1
            hobp_dbg_print("\nPoly Found" , NULL , NULL) ;
          #endif
        }
      else
        {
          #if DBG_HOBP == 1
            hobp_dbg_print("\nPoly Not Found" , NULL , NULL) ;
          #endif
        }
   }  

  return(hobp_vars.is_prog)  ;
}

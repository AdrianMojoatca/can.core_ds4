/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_init_nvfs.c 44556 2016-11-29 13:03:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_private.h"
#include "nvfs_usr.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean init_ver (void) ;
static Boolean init_data (void) ;
static Boolean init_log  (void) ;
static Boolean init_error(void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean hobp_init_nvfs (void)
{
  if(init_ver() && init_data() && init_error() && init_log())
    {
      return TRUE ;
    }
  
  return FALSE ;


}

//--------------------------------------------------------------------------//
static Boolean init_ver()
{
  UInt8 ver ;

  NVFS_Status status = nvfs_rd(nvfs_hobp_ver , &ver) ; //nvfs_check(nvfs_detect_ver , &detect_vars.version , sizeof(Detect_Nvfs_Version) , DETECT_VERSION) ;

  if(status == NVFS_OK)
    {
      if(ver == HOBP_T5_VERSION)
        {

          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp Version OK " , NULL , NULL) ;
          #endif    

          return TRUE ;
        }
      else
        {
          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp Version Mistmatch \nHoBp Isk Delete" , NULL , NULL) ;
          #endif    

        }
    }
  else if(status == NVFS_NAME_NOT_FOUND)
    {
      ver = HOBP_T5_VERSION ;

      if(!nvfs_wr_create(nvfs_hobp_ver , &ver , 1) == NVFS_OK)
        {
          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp Version Create FAIL " , NULL , NULL) ;
          #endif    

          return FALSE ;
        }
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Version Create " , &ver , 1) ;
      #endif    
    }



  nvfs_delete(nvfs_hobp_isk)  ;
  hobp_reset () ;

  return TRUE ;


}

//--------------------------------------------------------------------------//

static Boolean init_log (void)
{
  NVFS_Status status ;

  status = nvfs_size_is_valid(nvfs_hobp_log , sizeof(Hobp_Log)) ;
  
  if(status == NVFS_OK)
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Log Ok" , NULL , NULL) ;
      #endif
      
      return TRUE ;
    }
  
  nvfs_delete(nvfs_hobp_log) ;
 
  memset(&hobp_vars.log , NULL , sizeof(Hobp_Log)) ;

  #if DBG_HOBP == 1
    hobp_dbg_print("\nHoBp Log Not Valid " , NULL , NULL) ;
  #endif
  
  return (nvfs_wr_create(nvfs_hobp_log , &hobp_vars.log.data[0] , sizeof(Hobp_Log)) == NVFS_OK) ;
}

//--------------------------------------------------------------------------//

static Boolean init_data (void)
{
  hobp_vars.log_is_done = FALSE ;

  if(nvfs_size_is_valid(nvfs_hobp , sizeof(Hobp_Nvfs_Entry)) == NVFS_OK)
    {
      if(nvfs_rd(nvfs_hobp , &hobp_vars.nvfs_entry) == NVFS_OK)
        {
          UInt8 n ;

          UInt8 * ptr =  &hobp_vars.nvfs_entry.u8Tab1Col1[0] ;

          #if DBG_HOBP == 1
            hobp_dbg_print ("\nHoBp Data Tab1Col1  = " , ptr , sizeof_array(hobp_vars.nvfs_entry.u8Tab1Col1)) ;
          #endif
                
          for(n = 0 ; n < sizeof_array(hobp_vars.nvfs_entry.u8Tab1Col1) ; n++)
            {
              hobp_vars.log_is_done |=  (!!(*ptr != NULL)) ;
            }

          ptr =  &hobp_vars.nvfs_entry.u8Tab1[0][0] ;

          #if DBG_HOBP == 1
            hobp_dbg_print ("\nHoBp Data Tab1 Part1= " , ptr , sizeof(hobp_vars.nvfs_entry.u8Tab1)) ;
          #endif

          for(n = 0 ; n < sizeof(hobp_vars.nvfs_entry.u8Tab1) ; n++)
            {
              hobp_vars.log_is_done |= (!!(*ptr != NULL)) ;
              
              *ptr++ ;
            }
          
          if(hobp_vars.log_is_done)
            {
              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Log Is Done" , NULL , NULL) ;
              #endif
            }
          else
            {
              #if DBG_HOBP == 1
                hobp_dbg_print("\nHoBp Log Not Done" , NULL , NULL) ;
              #endif
            }
          return TRUE ;
        }
      else
        {
          #if DBG_HOBP == 1
            hobp_dbg_print ("\nHoBp Data Load Fail" , NULL , NULL) ;
          #endif
        }
    }
 
  nvfs_delete(nvfs_hobp) ;

  memset(&hobp_vars.nvfs_entry , NULL , sizeof(Hobp_Nvfs_Entry)) ;

  nvfs_wr_create (nvfs_hobp , &hobp_vars.nvfs_entry , sizeof(Hobp_Nvfs_Entry)) ;

  #if DBG_HOBP == 1
    hobp_dbg_print ("\nHoBp Data Not Valid" , NULL , NULL) ;
  #endif  



  return TRUE ;

}

//--------------------------------------------------------------------------//

static Boolean init_error (void)
{
  if(nvfs_size_is_valid(nvfs_hobp_error , sizeof(Hobp_Error)) == NVFS_OK)
    {
      if(nvfs_rd(nvfs_hobp_error , &hobp_vars.err) == NVFS_OK)
        {
           #if DBG_HOBP == 1
            hobp_dbg_print ("\nHoBp Err Ok " , NULL , NULL) ;
          #endif
          
          return TRUE ;
        }
      else
        {
          #if DBG_HOBP == 1
            hobp_dbg_print ("\nHoBp Err Load Fail" , NULL , NULL) ;
          #endif
        }
    }

  nvfs_delete(nvfs_hobp_error) ;

  memset(&hobp_vars.err , NULL , sizeof(Hobp_Error)) ;

  nvfs_wr_create (nvfs_hobp_error , &hobp_vars.err , sizeof(Hobp_Error)) ;

  #if DBG_HOBP == 1
    hobp_dbg_print ("\nHoBp Err Not Valid" , NULL , NULL) ;
  #endif  

  return TRUE ;  
}



/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CORE_VERSION_H__
#define __CORE_VERSION_H__


#ifdef DEV_LPC1765
    #ifdef CM800_DEI_BRAND
    #define CORE_VER "198.10.1" // update if the change is from common or ngmm
  #endif
#else
    #ifdef  CM800_DEI_BRAND
      #define CORE_VER "435d"  // update if the change is from common or dei
  #endif
  #ifdef  CM800_AUTOSTART_BRAND
          #define CORE_VER "202a" // update if the change is from common or autostart
  #endif
#endif

//--------------------------------------------------------------------------//


#define FW_VERSION      APP_VERSION"."CORE_VER
//--------------------------------------------------------------------------//

#endif

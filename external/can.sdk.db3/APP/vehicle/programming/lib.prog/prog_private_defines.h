/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_private_defines.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __PROG_PRIVATE_DEFINES_H__
#define __PROG_PRIVATE_DEFINES_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define PROG_DEFAULT_TO_AUX_OPTION(default_index) ((UInt8)((default_index) + 2))

#define DEFAULT_TRUNK_OPTION PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_TRUNK_DEFAULT)
#define DEFAULT_AUX1_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX1_DEFAULT)
#define DEFAULT_AUX2_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX2_DEFAULT)
#define DEFAULT_AUX3_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX3_DEFAULT)
#define DEFAULT_AUX4_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX4_DEFAULT)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
					
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



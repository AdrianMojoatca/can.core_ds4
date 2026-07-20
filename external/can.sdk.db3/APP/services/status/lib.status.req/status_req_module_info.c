/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_req_module_info.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_req_private.h"
#include "about.h"
#include "nvfs_usr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void status_req_platform_id 	(void) ;
static void status_req_hw_version	(void) ;
static void status_req_fw_id		(void) ;
static void status_req_fw_version	(void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void status_req_module_info(void * arg)
{
	status_req_platform_id () ;
	status_req_hw_version () ;
	status_req_fw_id () ;
	status_req_fw_version () ;
}

//--------------------------------------------------------------------------//

static void status_req_platform_id (void)
{
	UInt8 data[3]  = {SET_PLATFORM_ID, };

	data[1] = 0xFF ;          
	data[2] = 0xFF ;  

	d2d_tx_frame (&data[0] , sizeof_array(data)) ;
}

//--------------------------------------------------------------------------//

static void status_req_hw_version(void)
{
	UInt8 data[3]  = {SET_HW_VERSION, };

	data[1] = 0xFF ;          
	data[2] = 0xFF ;

	d2d_tx_frame (&data[0] , sizeof_array(data)) ;
} 
      
//--------------------------------------------------------------------------//

static void status_req_fw_id(void)
{	   
	UInt8 data[3]  = {SET_FW_ID, };

	data[1] = 0xFF ;          
	data[2] = 0xFF ;  
        
	d2d_tx_frame (&data[0] , sizeof_array(data)) ;			
}
//--------------------------------------------------------------------------//
static void status_req_fw_version(void)
{
	UInt8 data[3]  = {SET_FW_VERSION, };

    data[1] = 0xFF ;          
	data[2] = 0xFF ;      

	d2d_tx_frame (&data[0] , sizeof_array(data)) ;
}


/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_req_vin.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_req_private.h"

#if STATUS_VIN_ENABLE == 1
#include "vin.h"
#endif

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

void status_req_vin(void * arg)
{
	status_req_vin_1 (0) ; 
  	status_req_vin_2 (0) ;
	status_req_vin_3 (0) ;
	status_req_vin_4 (0) ;
	status_req_vin_5 (0) ;
	status_req_vin_6 (0) ;
}

//--------------------------------------------------------------------------//

void status_req_vin_1(void * arg)
{
	UInt8 data[3]  = {SET_VIN_1, 0xFF,0xFF};
  
#if STATUS_VIN_ENABLE == 1
    
    Vin vin ;
  
  	if(vin_get(vin))
    {
      
          data[1] = vin[0] ;
          data[2] = vin[1] ;
        
    } 
     
#endif

  	d2d_tx_frame (&data[0] , sizeof_array(data)) ;  
}

//--------------------------------------------------------------------------//

void status_req_vin_2(void * arg)
{
	UInt8 data[4]  = {SET_VIN_2,0xFF,0xFF,0xFF};

#if STATUS_VIN_ENABLE == 1

  	Vin vin ;
  
  	if(vin_get(vin))
    {
      
          data[1] = vin[2] ;
          data[2] = vin[3] ;
          data[3] = vin[4] ;
        
    }  

#endif

  	d2d_tx_frame (&data[0] , sizeof_array(data)) ;  
}

//--------------------------------------------------------------------------//

void status_req_vin_3(void * arg)
{
	UInt8 data[4]  = {SET_VIN_3,0xFF,0xFF,0xFF};

#if STATUS_VIN_ENABLE == 1

  	Vin vin ;
  
  	if(vin_get(vin))
    {
      
          data[1] = vin[5] ;
          data[2] = vin[6] ;
          data[3] = vin[7] ;
        
    }  

#endif

  	d2d_tx_frame (&data[0] , sizeof_array(data)) ;  
}

//--------------------------------------------------------------------------//

void status_req_vin_4(void * arg)
{
	UInt8 data[4]  = {SET_VIN_4,0xFF,0xFF,0xFF};


#if STATUS_VIN_ENABLE == 1

  	Vin vin ;
  
  	if(vin_get(vin))
    {
      
          data[1] = vin[8]  ;
          data[2] = vin[9]  ;
          data[3] = vin[10] ;
        
    }  

#endif

  	d2d_tx_frame (&data[0] , sizeof_array(data)) ;  
}

//--------------------------------------------------------------------------//

void status_req_vin_5(void * arg)
{
	UInt8 data[4]  = {SET_VIN_5,0xFF,0xFF,0xFF};

#if STATUS_VIN_ENABLE == 1

  	Vin vin ;
  
  	if(vin_get(vin))
    {
      
          data[1] = vin[11] ;
          data[2] = vin[12] ;
          data[3] = vin[13] ;
        
    }  

#endif

  	d2d_tx_frame (&data[0] , sizeof_array(data)) ;  
}

//--------------------------------------------------------------------------//

void status_req_vin_6(void * arg)
{
	UInt8 data[4]  = {SET_VIN_6,0xFF,0xFF,0xFF};

#if STATUS_VIN_ENABLE == 1

  	Vin vin ;
  
  	if(vin_get(vin))
    {
      
          data[1] = vin[14] ;
          data[2] = vin[15] ;
          data[3] = vin[16] ;
        
    }  

#endif

  	d2d_tx_frame (&data[0] , sizeof_array(data)) ; 
}

//--------------------------------------------------------------------------//

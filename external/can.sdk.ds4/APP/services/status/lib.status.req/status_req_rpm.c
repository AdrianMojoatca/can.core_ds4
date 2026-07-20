/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_req_rpm.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_req_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean req_rpm_blocked ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void status_req_rpm(void * arg)
{
  	UInt8 data[3]  = {SET_RPM, 0, 0};

#if STATUS_RPM_ENABLE == 1
  	
      	if(status_get_rpm (STATUS_RPM_WORD) != -1)
		{
#if defined PLATFORM_CM800 || defined PLATFORM_933
//			#warning "abcd"
			data[1] = status_get_rpm (STATUS_RPM_HIGH) ;
      		data[2] = status_get_rpm (STATUS_RPM_LOW) ;	
#else
			{
//			#warning "efgh"
				Rpm rpm ;
				rpm.byte[1] = status_get_rpm (STATUS_RPM_HIGH) ;
				rpm.byte[0] = status_get_rpm (STATUS_RPM_LOW) ;

				if(rpm.word >= 3000)
					rpm.word = 3000;
				else if(rpm.word > 300)
					rpm.word = 1000;
				if( (rpm.byte[0] == SET_HOOD_OPEN) || (rpm.byte[0] == SET_BRAKE_ON) || (rpm.byte[0] == SET_GLOWPLUG_ON))
					rpm.byte[0] = 0;
				data[1] = rpm.byte[1] ;
      	data[2] = rpm.byte[0] ;	
			}
#endif
		}     	  
   
#endif

    if((timeout_test(status_req_vars.rpm_timeout)) ||          // answer rpm request if internval is larger than 250ms
	   (status_get_rpm (STATUS_RPM_WORD) > RPM_RELEASE_BLOCK)) // OR current rpm is bigger than "RPM_RELEASE_BLOCK" 
    {
  	    req_rpm_blocked = FALSE ;
        d2d_tx_frame (&data[0] , sizeof_array(data)) ;
        timeout_start (status_req_vars.rpm_timeout) ;
    }
    else                                      // block rpm if request interval is smaller than 250 ms
    {
        req_rpm_blocked = TRUE ;    
    }
}

void status_req_rpm_blocked(void * arg) 
{
    if(req_rpm_blocked)     // if rpm was blocked, respond after minimal interval
    {
        req_rpm_blocked = FALSE ;
        status_req_rpm(0) ;
    }
}


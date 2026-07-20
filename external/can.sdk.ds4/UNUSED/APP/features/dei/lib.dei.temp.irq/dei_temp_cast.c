/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_temp_cast.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_temp_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DEI_TEMP_REF                             156

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

UInt8 dei_temp_cast ( UInt8 sensor_data )
{    
	static Boolean overflow   = FALSE; 
    static Boolean req_update = TRUE ;  
    static SInt16  last_temp_sensor  ;
    
    SInt16    temp_computed;
    SInt16    temp_sensor   = !(sensor_data&0x80) ?   sensor_data&0x7F : (-1)*(sensor_data&0x7F);    
    
    if(dei_temp_vars.tempHandshakeStatus < DEI_TEMP_HANDSHAKE_PASS)
	{
        if(sensor_data == DEI_TEMP_RESPONSE_HANDSHAKE)
		{
	        temp_sensor = 0x0;
		    dei_temp_vars.tempHandshakeStatus = DEI_TEMP_HANDSHAKE_PASS;
			TRACE_DEI_TEMP("Temp Handshake Pass");
		}
		else
		{
    	    dei_temp_vars.tempHandshakeStatus = DEI_TEMP_HANDSHAKE_FAIL;
	    }	    
	}

    TRACE_DEI_TEMP("[Temp reported by sensor %d ]\n\r" , temp_sensor );

    if(dei_temp_vars.tempHandshakeStatus == DEI_TEMP_HANDSHAKE_PASS)
	{
		 temp_computed = temp_sensor;
	}
	else
	{
		if( req_update )
		{
		    last_temp_sensor  = temp_sensor ;
		    req_update = FALSE ;
		}
		
		
		if( (SInt16)(temp_sensor - last_temp_sensor)  < (0-43) )
		{
		    //--- any decrease of temp with more then 43 degree is caused by an overflow
		    
		    TRACE_DEI_TEMP("[Temp Overflow Enter %d ]\n\r" , temp_sensor-last_temp_sensor );
		    
		    overflow = TRUE ;
		}
		
		 
		last_temp_sensor = temp_sensor ;
		
		if( (temp_sensor < 0) || (temp_sensor > 43) )
		{
		    //--- the overflow cannot generate negative values 
		    //--- the overflow cannot generate values over 43 ( > 100 Celsius in overflow mode )
		    
		    if( overflow == TRUE )
		    {
		        TRACE_DEI_TEMP("[Temp Overflow Leave]\n\r" );    
		    }
		    
		    overflow = FALSE ;
		}
		
		if( overflow == TRUE )
		{                
		    //--- overflow correction
		    temp_sensor   = 128 + sensor_data ;
		}
		
		//--- compute the temp using transfer function for 3.3V
		temp_computed = (SInt16)((((temp_sensor-25)*2+DEI_TEMP_REF)*0.6666)- DEI_TEMP_REF)/2 + 25;       
    }



    TRACE_DEI_TEMP("[Temp Computed %d ]\n\r" , temp_computed ) ;

    //--- saturate the result to -39 ( Viper remote cannot show less then -39 )
    if( temp_computed < -39 ) temp_computed = -39 ;
    
    //--- saturate the result to 
    if( temp_computed >  85 ) temp_computed = 85  ;

    return temp_computed + 40 ;
} 

/*--------------------------------------------------------------------------*/

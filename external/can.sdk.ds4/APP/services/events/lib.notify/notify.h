/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: notify.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/
#ifndef __NOTIFY_H__
#define __NOTIFY_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
#include "os.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum
{
/*---------------------------------------------------------------------------*/    	      
	NOTIFY_LOCK                           ,  		/* Notify_Pld_Zone_Report*/
/*---------------------------------------------------------------------------*/    	      
	NOTIFY_UNLOCK                         ,  		/*  Notify_Pld_Trigger   */
/*---------------------------------------------------------------------------*/
    NOTIFY_ACTIVE_ZONE                    ,    	    /* Notify_Pld_Zone_Report*/
/*---------------------------------------------------------------------------*/      
	NOTIFY_VALET_ON                       ,			/*  no payload           */ 
/*---------------------------------------------------------------------------*/    	  
	NOTIFY_VALET_OFF                      ,			/*  no payload           */ 
/*---------------------------------------------------------------------------*/    	  
    NOTIFY_TRUNK_RELEASE_OPEN             ,  	    /*  no payload           */ 
/*---------------------------------------------------------------------------*/    	
	NOTIFY_TRUNK_RELEASE_CLOSE            ,  		/*  no payload           */ 
/*---------------------------------------------------------------------------*/    
	NOTIFY_AUX_CH3_ON                     ,			/*  no payload           */ 
/*---------------------------------------------------------------------------*/
	NOTIFY_AUX_CH3_OFF                    ,   		/*  no payload           */
/*---------------------------------------------------------------------------*/
    NOTIFY_AUX_CH4_ON                     ,			/*  no payload           */
/*---------------------------------------------------------------------------*/
	NOTIFY_AUX_CH4_OFF                    ,	    	/*  no payload           */
/*---------------------------------------------------------------------------*/
    NOTIFY_AUX_CH5_ON                     ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/
    NOTIFY_AUX_CH5_OFF                    ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/
	NOTIFY_AUX_CH6_ON                     ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/
	NOTIFY_AUX_CH6_OFF                    ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/
    NOTIFY_AUX_CH2_TYPE                   ,  		/*  Notify_Pld_Aux_Type  */
/*---------------------------------------------------------------------------*/
	NOTIFY_AUX_CH3_TYPE                   ,  		/*  Notify_Pld_Aux_Type  */
/*---------------------------------------------------------------------------*/
    NOTIFY_AUX_CH4_TYPE                   ,  		/*  Notify_Pld_Aux_Type  */
/*---------------------------------------------------------------------------*/
    NOTIFY_AUX_CH5_TYPE                   ,  		/*  Notify_Pld_Aux_Type  */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR1_TYPE                   ,  		/*Notify_Pld_Sensor_Type */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR2_TYPE                   ,  		/*Notify_Pld_Sensor_Type */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR3_TYPE                   ,  		/*Notify_Pld_Sensor_Type */
/*---------------------------------------------------------------------------*/
    NOTIFY_PANIC                          ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/
    NOTIFY_RANGE_CHECK                    ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/
    NOTIFY_CAR_FINDER                     ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR1_ADJUST                 ,  		/*Notify_Pld_Sensor_Level*/
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR_BYPASS_WARN             ,  		/*  no paylod            */
/*---------------------------------------------------------------------------*/	 
    NOTIFY_SENSOR_BYPASS_SHOCK            ,  		/*  no paylod            */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR_BYPASS_OPT              ,  		/*  no paylod            */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR_BYPASS_FULL             ,  		/*  no paylod            */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR_BYPASS_OFF              ,  		/*  no paylod            */
/*---------------------------------------------------------------------------*/
    NOTIFY_SENSOR_SILENT_ARM              ,  		/*  no paylod            */
/*---------------------------------------------------------------------------*/
    NOTIFY_FULL_SILENT_ARM                ,  		/*  no paylod            */
/*---------------------------------------------------------------------------*/
    NOTIFY_LAST_TRIGGER                   ,  		/*  Notify_Pld_Trigger   */
/*---------------------------------------------------------------------------*/
    NOTIFY_ERROR                          ,  		/*  Notify_Pld_Error     */
/*---------------------------------------------------------------------------*/
    NOTIFY_TRIGGER                        ,  		/*  Notify_Pld_Trigger   */
/*---------------------------------------------------------------------------*/
    NOTIFY_WARNING                        ,			/* Notify_Pld_Sensor_Warn*/
/*---------------------------------------------------------------------------*/	          
    NOTIFY_RS_START                       ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/	          
    NOTIFY_RS_STOP_ACK                    ,  		/*  no paylod          	 */		
/*---------------------------------------------------------------------------*/	
    NOTIFY_RS_STOP                        ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/
	NOTIFY_RS_SUCCESSFULLY_STARTED        ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/
	NOTIFY_RS_FAILED_TO_START             ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/
    NOTIFY_RS_CRANKING_DONE               ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/
    NOTIFY_RS_RETRY                       ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/
    NOTIFY_TIMER_MODE_ON              	  ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/	          
    NOTIFY_TIMER_MODE_OFF             	  ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/	          
	NOTIFY_SMART_MODE_ON              	  ,  		/*  no paylod          	 */
/*---------------------------------------------------------------------------*/	          
    NOTIFY_SMART_MODE_OFF             	  ,  		/*  no paylod         	 */
/*---------------------------------------------------------------------------*/	          
    NOTIFY_TIMER_ADJUST_ENTER         	  ,  		/*  no paylod         	 */
/*---------------------------------------------------------------------------*/	          
	NOTIFY_TIMER_SET_START_COUNT		  ,		    /* Notify_Pld_Start_Count*/
/*---------------------------------------------------------------------------*/	         
	NOTIFY_TIMER_SET_TIME_START           ,         /*  Notify_Pld_Time_Start*/
/*---------------------------------------------------------------------------*/	                  
    NOTIFY_SHOW_TEMPERATURE               ,         /*  Notify_Pld_Temp      */
/*---------------------------------------------------------------------------*/	                  
	NOTIFY_TEMP_ADJUST_ENTER			  ,         /*  no payload           */
/*---------------------------------------------------------------------------*/	                  
    NOTIFY_LOW_TEMP_UPDATE                ,  		/*  Notify_Pld_Low_Temp  */
/*---------------------------------------------------------------------------*/	                  
    NOTIFY_HIGH_TEMP_UPDATE               ,  		/*  Notify_Pld_High_Temp */
/*---------------------------------------------------------------------------*/	                  
   	NOTIFY_RUNTIME_PROGRAMMED             ,  		/*  Notify_Pld_Runtime   */
/*---------------------------------------------------------------------------*/	                  
    NOTIFY_RUNTIME_REMAINING              ,  		/*  Notify_Pld_Runtime   */
/*---------------------------------------------------------------------------*/	                  
    NOTIFY_RUNTIME_RESET                  ,  		/*  Notify_Pld_Runtime   */
/*---------------------------------------------------------------------------*/	                  
    NOTIFY_RUNTIME_LOW                    ,  		/*  Notify_Pld_Runtime   */
/*---------------------------------------------------------------------------*/	                  
    NOTIFY_RUNTIME_CHECK                  ,  		/*  Notify_Pld_Runtime   */
/*---------------------------------------------------------------------------*/
#ifdef PLATFORM_933
    NOTIFY_RUNTIME_UPDATE                 ,  		/*  Notify_Pld_Runtime   */
#endif		
/*---------------------------------------------------------------------------*/	                      
    NOTIFY_DEFOGGER_ON                    ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/	                      
    NOTIFY_DEFOGGER_OFF                   ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/	                      	
	NOTIFY_KEYBOARD_UNLOCK                ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/	                      	
	NOTIFY_PAGE_UPDATE	                  ,  	    /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	
    NOTIFY_MTS_ENABLE                     ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/	                      	
	NOTIFY_MTS_DISABLE                    ,  		/*  no payload           */
/*---------------------------------------------------------------------------*/	                      	        
	NOTIFY_PROGRAMMING_ENTER              ,		    /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	            
	NOTIFY_REMOTE_PROGRAMMED_ACK 	      ,	        /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	            
  NOTIFY_PROGRAMMING_LEAVE              ,		    /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	            
	NOTIFY_REMOTE_DELETE				  ,	        /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	            
	NOTIFY_REMOTE_DELETE_ENTER			  ,	        /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	            
	NOTIFY_REMOTE_ZAP_ENTER			  ,	        /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	            
	NOTIFY_REMOTE_TACHO_DELETE			  ,	        /*  no payload           */
/*---------------------------------------------------------------------------*/	                      	            
	NOTIFY_LED_FLASH			  ,	        /*  Notify_Pld_Led           */
/*---------------------------------------------------------------------------*/	  
	NOTIFY_SENSOR1_LEVEL_UPDATE			  ,	        /*  Notify_Pld_Sensor_Level          */
/*---------------------------------------------------------------------------*/	  
	NOTIFY_SENSOR_PROG_MODE_EXIT			,					/*  no payload           */
/*---------------------------------------------------------------------------*/	  
	NOTIFY_SENSOR1_LEVEL_INCREASE			,					/*  Notify_Pld_Sensor_Level increase*/
/*---------------------------------------------------------------------------*/	  
	NOTIFY_SENSOR1_LEVEL_DECREASE			,					/*  Notify_Pld_Sensor_Level decrease */
/*---------------------------------------------------------------------------*/	  
	NOTIFY_RS_VALET_ON                       ,			/*  no payload           */ 
/*---------------------------------------------------------------------------*/    	  
	NOTIFY_RS_VALET_OFF                      ,			/*  no payload           */ 
/*---------------------------------------------------------------------------*/	  
	NOTIFY_COMBO_VALET_ON                       ,			/*  no payload           */ 
/*---------------------------------------------------------------------------*/    	  
	NOTIFY_COMBO_VALET_OFF                      ,			/*  no payload           */ 
/*---------------------------------------------------------------------------*/	  
	NOTIFY_CONVENIENCE_ALERT									,	/*  Notify_Pld_Convenience_Alert */
/*---------------------------------------------------------------------------*/	
	NOTIFY_STANDBY									,	/*  no payload */
 
	NOTIFY_COUNT

}Notify ;

/*---------------------------------------------------------------------------*/

typedef enum 
{
	NOTIFY_PLD_ZONE_OK              = 0		  ,
	NOTIFY_PLD_ZONE_DOOR			= 1u << 0 , 	
	NOTIFY_PLD_ZONE_TRUNK			= 1u << 1 ,	
	NOTIFY_PLD_ZONE_HOOD        	= 1u << 2 ,	
	NOTIFY_PLD_ZONE_IGN		    	= 1u << 3 , 	
	NOTIFY_PLD_ZONE_S1_NPC          = 1u << 4 , 
	NOTIFY_PLD_ZONE_S2_NPC          = 1u << 5 , 
	NOTIFY_PLD_ZONE_S3_NPC          = 1u << 6 , 
	NOTIFY_PLD_ZONE_S3              = 1u << 7 , 

}Notify_Pld_Zone_Report;

/*---------------------------------------------------------------------------*/

typedef enum
{
	NOTIFY_PLD_NO_TRIG              = 0       ,
	NOTIFY_PLD_TRIG_DOOR			= 1u << 0 , 	
	NOTIFY_PLD_TRIG_TRUNK			= 1u << 1 ,	
	NOTIFY_PLD_TRIG_HOOD        	= 1u << 2 ,	
	NOTIFY_PLD_TRIG_IGN		    	= 1u << 3 , 	
	NOTIFY_PLD_TRIG_S1		        = 1u << 4 , 
	NOTIFY_PLD_TRIG_S2              = 1u << 5 , 
	NOTIFY_PLD_TRIG_S3              = 1u << 6 , 
	NOTIFY_PLD_TRIG_S1_NPC          = 1u << 7 , 
	NOTIFY_PLD_TRIG_S2_NPC          = 1u << 8 , 
	NOTIFY_PLD_TRIG_S3_NPC          = 1u << 9 ,
	NOTIFY_PLD_TRIG_CUSTOM					= 1u << 10

}Notify_Pld_Trigger ;  

/*-------------------------------------------------------------------------*/


typedef enum
{
    NOTIFY_PLD_AUX_TRUNK      , 
    NOTIFY_PLD_AUX_WINDOW     , 
    NOTIFY_PLD_AUX_SUNRROF    , 
    NOTIFY_PLD_AUX_AUDIO      ,
    NOTIFY_PLD_AUX_LIGHTS     ,
    NOTIFY_PLD_AUX_LEFT_DOOR  ,
    NOTIFY_PLD_AUX_RIGHT_DOOR ,
    NOTIFY_PLD_AUX_REAR_HATCH ,
    NOTIFY_PLD_AUX_TIMED      ,
    NOTIFY_PLD_AUX_PULSED     ,
    NOTIFY_PLD_AUX_GARAGE_DOOR

}Notify_Pld_Aux_Type ;

/*-------------------------------------------------------------------------*/

typedef enum
{
    NOTIFY_PLD_SENSOR_NONE      , 
    NOTIFY_PLD_SENSOR_SHOCK     , 
    NOTIFY_PLD_SENSOR_FIELD     , 
    NOTIFY_PLD_SENSOR_TILT      ,
    NOTIFY_PLD_SENSOR_GLASS     ,
    NOTIFY_PLD_SENSOR_ULTRA 

}Notify_Pld_Sensor_Type ;  

/*-------------------------------------------------------------------------*/

typedef enum
{
    NOTIFY_PLD_ERROR_FEATURE_NA      , //0
    NOTIFY_PLD_ERROR_RUNTIME_EXPIRED , //1
    NOTIFY_PLD_ERROR_OVERREV         , //2
    NOTIFY_PLD_ERROR_LOW_TACH        , //3
    NOTIFY_PLD_ERROR_TRANSMITTER     , //4
    NOTIFY_PLD_ERROR_BRAKE           , //5
    NOTIFY_PLD_ERROR_HOOD            , //6
    NOTIFY_PLD_ERROR_RS_PROGRAMMING  , //7
    NOTIFY_PLD_ERROR_NEUTRAL         , //8
    NOTIFY_PLD_ERROR_LOW_BATT        , //9
    NOTIFY_PLD_ERROR_ALARM_TRIGGER   , //10
    NOTIFY_PLD_ERROR_WAIT_TO_START   , //11
	
		NOTIFY_PLD_ERROR_NO_TACH				, //12
		NOTIFY_PLD_ERROR_MANUAL_INTERFACE_STOPPED	,	//13
		NOTIFY_PLD_ERROR_IGN_DETECTED		, //14
		NOTIFY_PLD_ERROR_RS_FAILED_CRANK_RETRIES , //15
		NOTIFY_PLD_ERROR_TACH_AT_START	, //16
		NOTIFY_PLD_ERROR_READY_MODE_BROKEN_HBRAKE , //17
		NOTIFY_PLD_ERROR_READY_MODE_BROKEN_DOOR , //18
		NOTIFY_PLD_ERROR_MANUAL_INTERFACE_NOT_CONNECTED , //19

		NOTIFY_PLD_ERROR_RS_VALET , //20

}Notify_Pld_Error ;

/*-------------------------------------------------------------------------*/

typedef enum
{
	NOTIFY_PLD_SENSOR1_WARN , 
	NOTIFY_PLD_SENSOR2_WARN ,
	NOTIFY_PLD_SENSOR3_WARN 

}Notify_Pld_Sensor_Warn ; 

/*-------------------------------------------------------------------------*/

typedef enum
{
    NOTIFY_PLD_START_COUNT_01   = 1    , 
    NOTIFY_PLD_START_COUNT_02   = 2    ,
    NOTIFY_PLD_START_COUNT_03   = 3    ,
    NOTIFY_PLD_START_COUNT_04   = 4    ,
    NOTIFY_PLD_START_COUNT_06   = 5    ,
    NOTIFY_PLD_START_COUNT_08   = 6    ,
    NOTIFY_PLD_START_COUNT_10   = 7    ,
    NOTIFY_PLD_START_COUNT_12   = 8    ,
    NOTIFY_PLD_START_COUNT_14   = 9    ,
    NOTIFY_PLD_START_COUNT_16   = 10   ,
    NOTIFY_PLD_START_COUNT_18   = 11   ,
    NOTIFY_PLD_START_COUNT_20   = 12   ,
    NOTIFY_PLD_START_COUNT_22   = 13   ,
    NOTIFY_PLD_START_COUNT_24   = 14   

}Notify_Pld_Start_Count ; 

/*-------------------------------------------------------------------------*/

typedef enum
{
    NOTIFY_PLD_START_TIME_01   = 1    , 
    NOTIFY_PLD_START_TIME_02   = 2    ,
    NOTIFY_PLD_START_TIME_03   = 3    ,
    NOTIFY_PLD_START_TIME_04   = 4    ,
    NOTIFY_PLD_START_TIME_06   = 5    ,
    NOTIFY_PLD_START_TIME_08   = 6    ,
    NOTIFY_PLD_START_TIME_10   = 7    ,
    NOTIFY_PLD_START_TIME_12   = 8    ,
    NOTIFY_PLD_START_TIME_14   = 9    ,
    NOTIFY_PLD_START_TIME_16   = 10   ,
    NOTIFY_PLD_START_TIME_18   = 11   ,
    NOTIFY_PLD_START_TIME_20   = 12   ,
    NOTIFY_PLD_START_TIME_22   = 13   ,
    NOTIFY_PLD_START_TIME_24   = 14   

}Notify_Pld_Start_Time ; 

/*-------------------------------------------------------------------------*/

typedef UInt8		Notify_Pld_Runtime;

/*-------------------------------------------------------------------------*/

typedef UInt8		Notify_Pld_Low_Temp;

/*-------------------------------------------------------------------------*/

typedef UInt8		Notify_Pld_High_Temp;

/*-------------------------------------------------------------------------*/

typedef UInt8		Notify_Pld_Sensor_Level	;

/*-------------------------------------------------------------------------*/

typedef UInt16		Notify_Pld_Temp	;

/*-------------------------------------------------------------------------*/
#ifdef DEV_LPC_176x
typedef UInt32		Notify_Pld_Led	;
#else
typedef UInt8		Notify_Pld_Led	;
#endif

/*-------------------------------------------------------------------------*/
	 
typedef void (*Notify_Func)( Notify , ... );

/*-------------------------------------------------------------------------*/

typedef struct s_Notify_Cfg
{
	UInt16 tsk_pri 	   ; 
	UInt16 tsk_stk 	   ;
	UInt8* tsk_name	   ;

	UInt8  q_msg_size  ;	

}Notify_Cfg ;

/*-------------------------------------------------------------------------*/

typedef struct s_Notify_Array
{
    Notify      nf   ;
    Notify_Func func ;

}Notify_Array ;

#define NOTIFY_ADD(x) do{\
                            UInt8 i ;\
                            for(i=0;i<sizeof_array(x);i++)\
                                notify_add(x[i].nf,x[i].func);\
                        }while(0)

/*-------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void 	notify_init		 ( void  );
void 	notify_add 		 ( Notify nf , Notify_Func func );
void	notify_rem 		 ( Notify nf , Notify_Func func );
Boolean	notify_send 	 ( Notify nf , ... );
UInt8*  notify_txt       ( Notify nf );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif



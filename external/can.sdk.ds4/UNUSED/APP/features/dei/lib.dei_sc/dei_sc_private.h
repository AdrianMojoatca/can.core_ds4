/*==========================================================================*/
// $Id: dei_sc_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_SC_PRIVATE_H_
#define __DEI_SC_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf.h"
#include "keeloq.h"
#include "sync_count.h"
#include "bitfield.h"
#include "wake.h"
#include "timeout.h"
#include "q.h"
#include "nvram_usr.h"
#include "nvfs_usr.h"
#include "nvfs.h"  
#include "trace.h"
#include "cmd.h"
#include "notify.h"
#include "stdarg.h"
#include "vrtc_f.h"
#include "trace.h"
#include "wdog_v.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef DEBUG_DEI_SC
#define DEBUG_DEI_SC			0
#endif

#if DEBUG_DEI_SC == 1
#define TRACE_DEI_SC			TRACE
#else
#define TRACE_DEI_SC(...)		;
#endif

//--------------------------------------------------------------------------//

#define TASK_DEI_RF_RX_NAME  "dei_remote_rx"
#define TASK_DEI_RF_RX_PRI   10000
#define TASK_DEI_RF_RX_STK   0x0130

//--------------------------------------------------------------------------//

#define TASK_DEI_RF_TX_NAME  "dei_remote_tx"
#define TASK_DEI_RF_TX_PRI   10000
#define TASK_DEI_RF_TX_STK   0x0130
#define TASK_DEI_RF_TX_Q     8

//--------------------------------------------------------------------------//


#define DEI_SC_TIMEOUT_PAGGING		1000 // --> for 1 second
#define DEI_SC_TIMEOUT_LOCK			1000 // --> for 1 second

#define DEI_SC_PAGGING()			timeout_test( dei_sc_timeout_pagging ) 

#define DEI_SC_LOCK()               timeout_test( dei_sc_timeout_lock )  



//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SC_RF_1WAY  = 0,     
    DEI_SC_RF_2WAY  = 1

}Dei_Sc_Notify_Type ; 

//--------------------------------------------------------------------------//


typedef enum
{    									   
    DEI_SC_NORMAL_TX      ,    
    DEI_SC_PROGRAMMING_TX ,  
    DEI_SC_PAGGING_TX     ,       

}Dei_Sc_Tx_Mode ;

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SC_MODE_NORMAL       ,  
    DEI_SC_MODE_PROGRAMMING  ,
    DEI_SC_MODE_ALARM_REPORT ,

}Dei_Sc_Mode ; 

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SECURITY_ARMED             =0x00, 
    DEI_SECURITY_DISARMED          =0x01,
    DEI_SECURITY_STEALTH1          =0x02, 
    DEI_SECURITY_STEALTH2          =0x03,      
    DEI_SECURITY_ARMED_VALET_ON    =0x04,
    DEI_SECURITY_DISARMED_VALET_ON =0x05

}Dei_Icon_Security;

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_ICON_TRIGGER_NO_TRIGGER     , 
    DEI_ICON_TRIGGER_ACTIVE ,
    DEI_ICON_TRIGGER_NPC_ON     

}Dei_Icon_Trigger ; 

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_BYPASS_OFF   , 
    DEI_BYPASS_WARN  , 
    DEI_BYPASS_FULL  ,     
    DEI_BYPASS_SHOCK ,
    DEI_BYPASS_OPT

}Dei_Icon_Bypass ; 

//--------------------------------------------------------------------------//

typedef UInt32      Dei_Sc_Payload ;

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SC_TYPE_2B_SN  =  0xD6 ,     
    DEI_SC_TYPE_3B_SN  =  0xD8 

}Dei_Sc_Sn_Type;

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SC_PLD001 ,
    DEI_SC_PLD002 ,
    DEI_SC_PLD003 ,
    DEI_SC_PLD004 ,
    DEI_SC_PLD005 ,
    DEI_SC_PLD006 ,
    DEI_SC_PLD007 ,
    DEI_SC_PLD008 ,
    DEI_SC_PLD009 ,
    DEI_SC_PLD010 ,
    DEI_SC_PLD011 ,
    DEI_SC_PLD012 ,
    DEI_SC_PLD013 ,
    DEI_SC_PLD015 ,
    DEI_SC_PLDNA

}DEI_SC_PAYLOAD ; 

//--------------------------------------------------------------------------//


typedef enum 
{
    DEI_SC_ACK_LOCK	                        	,
    DEI_SC_ACK_LOCKED_VALET_ON	                ,
    DEI_SC_ACK_SILENT_LOCKED	                ,    
    DEI_SC_ACK_LOCK_WITH_ACTIVE_ZONE_REPORT		,
    DEI_SC_ACK_UNLOCK	                        ,
    DEI_SC_ACK_UNLOCKED_VALET_ON	            ,
    DEI_SC_ACK_SILENT_UNLOCKED	                ,
    DEI_SC_ACK_TRUNK_RELEASE	                ,
    DEI_SC_ACK_PANIC_ON	                    	,
    DEI_SC_ACK_NOT_USED_A9	                    ,
    DEI_SC_ACK_SENSOR_BYPASS_CONFIRM	        ,
    DEI_SC_ACK_FULL_BYPASS_CONFIRM	            ,
    DEI_SC_ACK_SENSOR_BYPASS_OFF	            ,
    DEI_SC_ACK_UPDATE_PAGE	                    ,
    DEI_SC_ACK_AUX1_ON	                        ,
    DEI_SC_ACK_AUX1_OFF	                    	,
    DEI_SC_ACK_AUX2_ON	                        ,
    DEI_SC_ACK_AUX2_OFF	                    	,
    DEI_SC_ACK_AUX3_ON	                        ,
    DEI_SC_ACK_AUX3_OFF	                    	,
    DEI_SC_ACK_AUX4_ON	                        ,
    DEI_SC_ACK_AUX4_OFF	                    	,
    DEI_SC_ACK_VALET_ON	                    	,
    DEI_SC_ACK_VALET_OFF	                    ,
    DEI_SC_ACK_CAR_FINDER_ON	                ,
    DEI_SC_ACK_ADJUST_SENSOR_ENTER	            ,
    DEI_SC_ACK_ADJUST_TEMP_ENTER	            ,
    DEI_SC_ACK_ADJUST_START_COUNT_ENTER	    	,
    DEI_SC_ACK_SENSOR1_LEVEL_INCREASE	        ,
    DEI_SC_ACK_SENSOR1_LEVEL_DECREASE	        ,
    DEI_SC_ACK_SENSOR_LEVEL_RESET	            ,
    DEI_SC_ACK_TEMP_LEVEL_RESET	            	,
    DEI_SC_ACK_START_COUNT_LEVEL_RESET	        ,
    DEI_SC_ACK_ALARM_WARNING_EVENT	            ,
    DEI_SC_ACK_ALARM_TRIGGER_EVENT	            ,
    DEI_SC_ACK_PROGRAMMING_MODE_EXIT	        ,
    DEI_SC_ACK_REMOTE_START_ON	                ,
    DEI_SC_ACK_REMOTE_START_OFF	                ,
    DEI_SC_ACK_ERROR_MSG	                    ,
    DEI_SC_ACK_TIMER_MODE_ON	                ,
    DEI_SC_ACK_TIMER_MODE_OFF	                ,
    DEI_SC_ACK_SMART_START_ON	                ,
    DEI_SC_ACK_SMART_START_OFF	                ,
    DEI_SC_ACK_RUNTIME_CONFIRM	                ,
    DEI_SC_ACK_REAR_DEFOGGER_ON	                ,
    DEI_SC_ACK_REAR_DEFOGGER_OFF               	,
    DEI_SC_ACK_NOT_USED_CA	                    ,
    DEI_SC_ACK_TRIGGER_ZONE_REPORT	            ,
    DEI_SC_ACK_SENSOR_SILENT_ARM	            ,
    DEI_SC_ACK_VECHICLE_TEMP_REPORT	        	,
    DEI_SC_ACK_FULL_SILENT_ARM	                ,
    DEI_SC_ACK_RUNTIME_LOW_ALERT	            ,
    DEI_SC_ACK_RANGE_CHECK	                    ,
    DEI_SC_ACK_IGNITION_OFF	                	,
    DEI_SC_ACK_GARAGE_DOOR_OPEN	            	,
    DEI_SC_ACK_GARAGE_DOOR_CLOSE	            ,
    DEI_SC_ACK_MODE_CONFIGURATION               ,
	DEI_SC_ACK_COUNT                               

}DEI_SC_ACK ; 

//--------------------------------------------------------------------------//

typedef enum 
{
//                                         bit index msb  bit index lsb
    DEI_SC_INFO_RUNTIME_PROGRAMMED        = (5u  << 8) | ( 0  ),
    DEI_SC_INFO_RUNTIME_NOTIFYAINING      = (11u << 8) | ( 6  ),
    DEI_SC_INFO_REMOTE_START              = (12u << 8) | ( 12 ),
    DEI_SC_INFO_TIMER_MODE                = (13u << 8) | ( 13 ),
    DEI_SC_INFO_SMART_MODE                = (14u << 8) | ( 14 ),
    DEI_SC_INFO_MTS                       = (15u << 8) | ( 15 ),    
    DEI_SC_INFO_TRIGGER                   = (17u << 8) | ( 16 ),
    DEI_SC_INFO_SECURITY                  = (20u << 8) | ( 18 ),       
    DEI_SC_INFO_DOOR_STATUS               = (21u << 8) | ( 21 ),
    DEI_SC_INFO_DOOR_TRIGGER_BYPASS       = (22u << 8) | ( 22 ),
    DEI_SC_INFO_DOOR_TRIGGERED            = (23u << 8) | ( 23 ),        
    DEI_SC_INFO_HOOD_STATUS               = (24u << 8) | ( 24 ),
    DEI_SC_INFO_HOOD_TRIGGER_BYPASS       = (25u << 8) | ( 25 ),
    DEI_SC_INFO_HOOD_TRIGGERED            = (26u << 8) | ( 26 ),
    DEI_SC_INFO_TRUNK_STATUS              = (27u << 8) | ( 27 ),
    DEI_SC_INFO_TRUNK_TRIGGER_BYPASS      = (28u << 8) | ( 28 ),
    DEI_SC_INFO_TRUNK_TRIGGERED           = (29u << 8) | ( 29 ),
    DEI_SC_INFO_IGNITION_STATUS           = (30u << 8) | ( 30 ),
    DEI_SC_INFO_IGNITION_TRIGGER_BYPASS   = (31u << 8) | ( 31 ),
    DEI_SC_INFO_IGNITION_TRIGGERED        = (32u << 8) | ( 32 ),        
    DEI_SC_INFO_SENSOR1_STATUS            = (33u << 8) | ( 33 ),
    DEI_SC_INFO_SENSOR1_TRIGGER_BYPASS    = (34u << 8) | ( 34 ),
    DEI_SC_INFO_SENSOR1_WARNING_BYPASS    = (35u << 8) | ( 35 ),
    DEI_SC_INFO_SENSOR1_NPC               = (36u << 8) | ( 36 ),
    DEI_SC_INFO_SENSOR1_TRIGGERED         = (37u << 8) | ( 37 ),
    DEI_SC_INFO_SENSOR1_ICON              = (40u << 8) | ( 38 ),
    DEI_SC_INFO_SENSOR2_STATUS            = (41u << 8) | ( 41 ),
    DEI_SC_INFO_SENSOR2_TRIGGER_BYPASS    = (42u << 8) | ( 42 ),
    DEI_SC_INFO_SENSOR2_WARNING_BYPASS    = (43u << 8) | ( 43 ),
    DEI_SC_INFO_SENSOR2_NPC               = (44u << 8) | ( 44 ),
    DEI_SC_INFO_SENSOR2_TRIGGERED         = (45u << 8) | ( 45 ),
    DEI_SC_INFO_SENSOR2_ICON              = (48u << 8) | ( 46 ),
    DEI_SC_INFO_SENSOR3_STATUS            = (49u << 8) | ( 49 ),
    DEI_SC_INFO_SENSOR3_TRIGGER_BYPASS    = (50u << 8) | ( 50 ),
    DEI_SC_INFO_SENSOR3_WARNING_BYPASS    = (51u << 8) | ( 51 ),
    DEI_SC_INFO_SENSOR3_NPC               = (52u << 8) | ( 52 ),
    DEI_SC_INFO_SENSOR3_TRIGGERED         = (53u << 8) | ( 53 ),
    DEI_SC_INFO_SENSOR3_ICON              = (56u << 8) | ( 54 ),
    DEI_SC_INFO_SENSOR_BYPASS_LEVEL       = (60u << 8) | ( 57 ),
    DEI_SC_INFO_SENSOR1_LEVEL             = (64u << 8) | ( 61 ),    
    DEI_SC_INFO_SMART_START_LOW_TEMP      = (68u << 8) | ( 65 ),
    DEI_SC_INFO_SMART_START_HIGH_TEMP     = (72u << 8) | ( 69 ),
    DEI_SC_INFO_TIMER_MODE_STARTS_COUNT   = (76u << 8) | ( 73 ),
    DEI_SC_INFO_TIMER_MODE_STARTS_PERIOD  = (80u << 8) | ( 77 ),    
    DEI_SC_INFO_AUXCH2_STATUS             = (81u << 8) | ( 81 ),
    DEI_SC_INFO_AUXCH2_ICON               = (85u << 8) | ( 82 ),    
    DEI_SC_INFO_AUXCH3_STATUS             = (86u << 8) | ( 86 ),
    DEI_SC_INFO_AUXCH3_ICON               = (90u << 8) | ( 87 ),
    DEI_SC_INFO_AUXCH4_STATUS             = (91u << 8) | ( 91 ),
    DEI_SC_INFO_AUXCH4_ICON               = (95u << 8) | ( 92 ),
    DEI_SC_INFO_AUXCH5_STATUS             = (96u << 8) | ( 96 ),
    DEI_SC_INFO_AUXCH5_ICON               = (100u<< 8) | ( 97 ),    
    DEI_SC_INFO_AUXCH6_STATUS             = (101u << 8)| ( 101),    
    DEI_SC_INFO_ERROR                     = (106u<< 8) | ( 102),
    DEI_SC_INFO_VECHICLE_TEMP             = (114u<< 8) | ( 107),
    DEI_SC_INFO_BIT_COUNT                 = 115

}DEI_SC_INFO ; 
        
//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SC_BUTTON_LOCK	                    =	0x00A0-0x00A0 , 
    DEI_SC_BUTTON_UNLOCK	                =	0x00A1-0x00A0 , 
    DEI_SC_BUTTON_START	                    =	0x00A2-0x00A0 ,  
    DEI_SC_BUTTON_STOP	                    =	0x00A3-0x00A0 ,  
    DEI_SC_BUTTON_START_STOP_TOOLGE         =	0x00A4-0x00A0 ,  
    DEI_SC_BUTTON_TRUNK	                    =	0x00A5-0x00A0 ,  
    DEI_SC_BUTTON_PANIC	                    =	0x00A6-0x00A0 ,  
    DEI_SC_BUTTON_STATUS	                =	0x00A7-0x00A0 ,  
    DEI_SC_BUTTON_STATUS_ALARM	            =	0x00A8-0x00A0 ,  
    DEI_SC_BUTTON_STATUS_RS	                =	0x00A9-0x00A0 ,  
    DEI_SC_BUTTON_AUX1	                    =	0x00AA-0x00A0 ,  
    DEI_SC_BUTTON_AUX2	                    =	0x00AB-0x00A0 ,  
    DEI_SC_BUTTON_AUX3	                    =	0x00AC-0x00A0 ,  
    DEI_SC_BUTTON_AUX4	                    =	0x00AD-0x00A0 ,  
    DEI_SC_BUTTON_AUX5	                    =	0x00AE-0x00A0 ,  
    DEI_SC_BUTTON_AUX6     	                =	0x00AF-0x00A0 ,  
    DEI_SC_BUTTON_AUX7	                    =	0x00B0-0x00A0 ,  
    DEI_SC_BUTTON_AUX8	                    =	0x00B1-0x00A0 ,  
    DEI_SC_BUTTON_REAR_DEFOGER	            =	0x00B2-0x00A0 ,  
    DEI_SC_BUTTON_SENSOR1_ADJUST	        =	0x00B3-0x00A0 ,  
    DEI_SC_BUTTON_SENSOR2_ADJUST	        =	0x00B4-0x00A0 ,  
    DEI_SC_BUTTON_SENSOR3_ADJUST	        =	0x00B5-0x00A0 ,  
    DEI_SC_BUTTON_SENSOR4_ADJUST	        =	0x00B6-0x00A0 ,  
    DEI_SC_BUTTON_SENSOR_BYPASS_LEVEL	    =	0x00B7-0x00A0 ,  
    DEI_SC_BUTTON_COLD_START_TOOGLE	        =	0x00B8-0x00A0 ,  
    DEI_SC_BUTTON_TEMP_ADJUST	            =	0x00B9-0x00A0 ,  
    DEI_SC_BUTTON_TIME_START_ADJUST	        =	0x00BA-0x00A0 ,  
    DEI_SC_BUTTON_ALARM_CODE_REQ	        =	0x00BB-0x00A0 ,  
    DEI_SC_BUTTON_NOT_USED_BC	            =	0x00BC-0x00A0 ,  
    DEI_SC_BUTTON_ERROR_START_CODE_REQ	    =	0x00BD-0x00A0 ,  
    DEI_SC_BUTTON_VALET_TOOGLE	            =	0x00BE-0x00A0 ,  
    DEI_SC_BUTTON_CAR_FINDER	            =	0x00BF-0x00A0 ,  
    DEI_SC_BUTTON_TEMP_REPORT_REQ	        =	0x00C0-0x00A0 ,  
    DEI_SC_BUTTON_RUNTIME_CHECK	            =	0x00C1-0x00A0 ,  
    DEI_SC_BUTTON_NOT_USED_C2	            =	0x00C2-0x00A0 ,  
    DEI_SC_BUTTON_TIMER_MODE_TOOGLE	        =	0x00C3-0x00A0 ,  
    DEI_SC_BUTTON_SMART_START_TOOGLE	    =	0x00C4-0x00A0 ,  
    DEI_SC_BUTTON_RUNTIME_RESET	            =	0x00C5-0x00A0 ,  
    DEI_SC_BUTTON_SILENT_LOCK	            =	0x00C6-0x00A0 ,  
    DEI_SC_BUTTON_SILENT_UNLOCK	            =	0x00C7-0x00A0 ,  
    DEI_SC_BUTTON_WINDOWS_ROLL_UP	        =	0x00C8-0x00A0 ,  
    DEI_SC_BUTTON_WINDOWS_ROLL_DOWN	        =	0x00C9-0x00A0 ,  
    DEI_SC_BUTTON_GARAGE_DOOR_TOOGLE	    =	0x00CA-0x00A0 ,  
    DEI_SC_BUTTON_GARAGE_DOOR_OPEN	        =	0x00CB-0x00A0 ,  
    DEI_SC_BUTTON_GARAGE_DOOR_CLOSE	        =	0x00CC-0x00A0 ,  
    DEI_SC_BUTTON_GARAGE_DOOR_STATUS	    =	0x00CD-0x00A0 ,  
    DEI_SC_BUTTON_AUX_ARM	                =	0x00CE-0x00A0 ,  
    DEI_SC_BUTTON_AUX_DISARM	            =	0x00CF-0x00A0 ,  
    DEI_SC_BUTTON_NOT_USED_D0	            =	0x00D0-0x00A0 ,  
    DEI_SC_BUTTON_AUX_LOCK	                =	0x00D1-0x00A0 ,  
    DEI_SC_BUTTON_AUX_UNLOCK	            =	0x00D2-0x00A0 ,  
    DEI_SC_BUTTON_AUX_ARM_LOCK	            =	0x00D3-0x00A0 ,  
    DEI_SC_BUTTON_AUX_DISARM_UNLOCK	        =	0x00D4-0x00A0 ,  
    DEI_SC_BUTTON_AUX_STATUS	            =	0x00D5-0x00A0 ,  
    DEI_SC_BUTTON_LEARNING_BEGIN	        =	0x00D6-0x00A0 ,  
    DEI_SC_BUTTON_EXIT_PROGRAMMING_MODE	    =	0x00D7-0x00A0 ,  
    DEI_SC_BUTTON_ENTER_PROGRAMMING_MODE	=	0x00D8-0x00A0 ,  
    DEI_SC_BUTTON_ADJUST_PLUS	            =	0x00D9-0x00A0 ,  
    DEI_SC_BUTTON_ADJUST_MINUS	            =	0x00DA-0x00A0 ,  
    DEI_SC_BUTTON_SENSOR_SILENT_ARM	        =	0x00DB-0x00A0 ,  
    DEI_SC_BUTTON_NOT_USED_DC	            =	0x00DC-0x00A0 ,  
    DEI_SC_BUTTON_NOT_USED_DD	            =	0x00DD-0x00A0 ,  
    DEI_SC_BUTTON_FULL_SILENT_ARM	        =	0x00DE-0x00A0 ,  
    DEI_SC_BUTTON_TRIGGER_HISTORY_REQUEST	=	0x00DF-0x00A0 ,  
    DEI_SC_BUTTON_PUT_NEW_SENSOR1_VALUE	    =	0x00E0-0x00A0 ,  
    DEI_SC_BUTTON_TEMP_START_ADJUST	        =	0x00E1-0x00A0 ,  
    DEI_SC_BUTTON_PUT_NEW_TEMP 	            =	0x00E2-0x00A0 ,  
    DEI_SC_BUTTON_START_TIMES_ADJUST	    =	0x00E3-0x00A0 ,  
    DEI_SC_BUTTON_PUT_NEW_START_TIMES	    =	0x00E4-0x00A0 ,  
    DEI_SC_BUTTON_ONE_BUTTON_FUNCTION	    =	0x00E5-0x00A0 ,  
    DEI_SC_BUTTON_PROGRAMMING_DONE	        =	0x00E6-0x00A0 , 	    
    DEI_SC_BUTTON_COUNT                     =   0x00E7-0x00A0 ,
	    
}DEI_SC_BUTTON ;

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SC_SENSOR_BYPASS_OFF   = 0x00 ,
    DEI_SC_SENSOR_BYPASS_WARN  = 0x01 , 
    DEI_SC_SENSOR_BYPASS_SHOCK = 0x03 , 
    DEI_SC_SENSOR_BYPASS_OPT   = 0x08 ,
    DEI_SC_SENSOR_BYPASS_FULL  = 0x02 ,

}DEI_SC_SENSOR_BYPASS ; 

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_SC_NGRF_MODE_NORMAL       ,  
    DEI_SC_NGRF_MODE_PROGRAMMING_ENTER  ,
    DEI_SC_NGRF_MODE_PROGRAMMING_LEAVE  ,
    DEI_SC_NGRF_MODE_ERASE  ,

}Dei_Sc_NGRF_Mode ;  

//--------------------------------------------------------------------------//
    
typedef struct s_Dei_Sc_Info
{
    UInt8 bits[ (DEI_SC_INFO_BIT_COUNT / 8)  + 1] ; 

}Dei_Sc_Info ; 

//--------------------------------------------------------------------------//


typedef struct _s_Dei_Sc
{
    Sync_Count      count   ; 
    
    UInt32          sn      ;    
    
    UInt64          key     ;
    
    Dei_Sc_Sn_Type sn_type ;

    Dei_Sc_Notify_Type rf_type ;

}_Dei_Sc ;           

//--------------------------------------------------------------------------//

typedef struct	s_Dei_Sc
{
	_Dei_Sc		remote[	DEI_RF_ID_COUNT ]    ;

	UInt8           index                        ;

	UInt16          car_sn                       ;

}Dei_Sc ;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Tx_Frame
{
    Q_Msg               base      ;
    
    Dei_Sc_Tx_Mode  mode      ; 
    
    Dei_Sc_Payload  payload   ;     

}Dei_Sc_Tx_Frame ; 

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Button
{
    unsigned button     :16 ; 
    unsigned pl         :8  ;
    unsigned id         :2  ;    
    unsigned press_type :1  ;
	unsigned rf_type    :1  ;
    unsigned batt_low   :1  ;
	
}Dei_Sc_Button ; 

//--------------------------------------------------------------------------//

#define	DEI_SC_VRTC_TIMEOUT_FIRST_RF_UPDATE		VRTC_1SEC*30		//30 seconds for first update
#define	DEI_SC_VRTC_TIMEOUT_RF_UPDATE			VRTC_1SEC*60*60		//1h for next updates
#define DEI_SC_VRTC_TIMEOUT_LOCK_ZONE_ACTIVE 	VRTC_1SEC*2			//2 seconds

#define DEI_SC_COUNT_RF_UPDATE					4					//update the remote for maximum 4 times

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void        		dei_sc_init	 							( void );
void     		    dei_sc_notify_init                      ( void );

void                dei_sc_delete                           ( void );
void                dei_sc_delete_enter                     ( void );

Boolean             dei_sc_init_nvfs                        ( void );
void                dei_sc_save_nvfs                        ( Wake_Status wake_status );

void 				dei_sc_automatic_page_update_init		( void );
void 				dei_sc_automatic_page_update			( void );

void	 			dei_sc_icon_lock_active_zone_report_init( void );
Boolean 			dei_sc_icon_lock_active_zone_report	    ( void );

void                dei_sc_tx_flush               		  	( void );
void                dei_sc_tx_tsk                 		  	( void );
void                dei_sc_rx_tsk                 	  	  	( void );

Dei_Sc_Button*  	dei_sc_check_frame_normal     		  	( Dei_Rf_Frame *frame );
Dei_Sc_Button*  	dei_sc_check_frame_programming		  	( Dei_Rf_Frame *frame );

Dei_Sc_Payload  	dei_sc_payload_build          		  	( DEI_SC_PAYLOAD pld , UInt8 cmd );

void                dei_sc_info_set               		  	( DEI_SC_INFO info , UInt8 status );
UInt8               dei_sc_info_get               		  	( DEI_SC_INFO info );
void                dei_sc_info_clr               		  	( void  );

Boolean             dei_sc_tx                     		  	( DEI_SC_ACK ack , Dei_Sc_Tx_Mode mode , TskTimeout timeout);
void                dei_sc_set_mode               		  	( Dei_Sc_Mode mode );

Boolean 			dei_sc_icon_lock			  		  	( Notify_Pld_Zone_Report zone );
Boolean 			dei_sc_icon_unlock			  		  	( Notify_Pld_Trigger     trig );

Boolean 			dei_sc_icon_oem_lock			  		( Notify_Pld_Zone_Report zone );
Boolean 			dei_sc_icon_oem_unlock			  		( Notify_Pld_Trigger     trig );

Boolean 			dei_sc_icon_valet_on		  		  	( void );
Boolean 			dei_sc_icon_valet_off		  		  	( void );
Boolean 			dei_sc_icon_unlock_trigger_zone_report	( void );
Boolean 			dei_sc_icon_trunk_release_on		  	( void );
Boolean 			dei_sc_icon_trunk_release_off   	  	( void );

Boolean 			dei_sc_icon_aux2_type				  	( Notify_Pld_Aux_Type    type );
Boolean 			dei_sc_icon_aux3_type				  	( Notify_Pld_Aux_Type    type );
Boolean 			dei_sc_icon_aux4_type				  	( Notify_Pld_Aux_Type    type );
Boolean 			dei_sc_icon_aux5_type				  	( Notify_Pld_Aux_Type    type );

Boolean 			dei_sc_icon_aux_ch3_on				    ( void );
Boolean 			dei_sc_icon_aux_ch3_off				    ( void );
Boolean 			dei_sc_icon_aux_ch4_on				    ( void );
Boolean 			dei_sc_icon_aux_ch4_off				    ( void );
Boolean 			dei_sc_icon_aux_ch5_on				    ( void );
Boolean 			dei_sc_icon_aux_ch5_off				    ( void );
Boolean 			dei_sc_icon_aux_ch6_on				    ( void );
Boolean 			dei_sc_icon_aux_ch6_off				    ( void );
Boolean 			dei_sc_icon_car_finder				    ( void );
Boolean             dei_sc_icon_defogger_on                 ( void );
Boolean             dei_sc_icon_defogger_off                ( void );

Boolean 			dei_sc_icon_error					    ( Notify_Pld_Error error_type );

Boolean 			dei_sc_icon_full_silent_arm			    ( void );
Boolean 			dei_sc_icon_keyboard_unlock			    ( void );
Boolean             dei_sc_icon_mts_enable                  ( void );
Boolean             dei_sc_icon_mts_disable                 ( void );
Boolean             dei_sc_icon_panic                       ( void );
Boolean             dei_sc_icon_programming_enter           ( void );
Boolean             dei_sc_icon_programming_leave           ( void );
Boolean             dei_sc_icon_range_check                 ( void );

Boolean             dei_sc_icon_runtime_check               ( Notify_Pld_Runtime runtime );
Boolean             dei_sc_icon_runtime_low_alert           ( Notify_Pld_Runtime runtime );
Boolean             dei_sc_icon_runtime_programmed          ( Notify_Pld_Runtime runtime );
Boolean             dei_sc_icon_runtime_remaining           ( Notify_Pld_Runtime runtime );
Boolean             dei_sc_icon_runtime_reset               ( Notify_Pld_Runtime runtime );

Boolean 			dei_sc_icon_sensor1_adjust			    ( Notify_Pld_Sensor_Level level );
Boolean 			dei_sc_icon_sensor1_type			    ( Notify_Pld_Sensor_Type  type  );
Boolean 			dei_sc_icon_sensor2_type			    ( Notify_Pld_Sensor_Type  type  );
Boolean 			dei_sc_icon_sensor3_type			    ( Notify_Pld_Sensor_Type  type  );

Boolean 			dei_sc_icon_sensor_bypass_full		    ( void );
Boolean 			dei_sc_icon_sensor_bypass_off		    ( void );
Boolean 			dei_sc_icon_sensor_bypass_opt   	    ( void );
Boolean 			dei_sc_icon_sensor_bypass_shock		    ( void );
Boolean 			dei_sc_icon_sensor_bypass_warning	    ( void );
Boolean 			dei_sc_icon_sensor_silent_arm		    ( void );

Boolean 			dei_sc_icon_smart_mode_off			    ( void );
Boolean 			dei_sc_icon_smart_mode_on			    ( void );
Boolean 			dei_sc_icon_timer_mode_on				( void );
Boolean 			dei_sc_icon_timer_mode_off				( void );


Boolean 			dei_sc_icon_smart_mode_temp_adjust_enter( void );
Boolean 			dei_sc_icon_smart_mode_temp_adjust_low	( Notify_Pld_Low_Temp  low_temp  );
Boolean 			dei_sc_icon_smart_mode_temp_adjust_high	( Notify_Pld_High_Temp high_temp ); 
Boolean 			dei_sc_icon_timer_mode_adjust_count		( Notify_Pld_Start_Count count );
Boolean 			dei_sc_icon_timer_mode_adjust_enter		( void ); 
Boolean 			dei_sc_icon_timer_mode_adjust_time 		( Notify_Pld_Start_Time time );  

Boolean 			dei_sc_icon_temp						( Notify_Pld_Temp temp );
Boolean 			dei_sc_icon_start						( void );
Boolean 			dei_sc_icon_stop						( void );

Boolean 			dei_sc_icon_trigger						( Notify_Pld_Trigger     trig );
Boolean 			dei_sc_icon_warning					  	( Notify_Pld_Sensor_Warn warn );
Boolean 			dei_sc_icon_trigger_zone_report			( Notify_Pld_Trigger     trig );
Boolean             dei_sc_icon_active_zone                 ( Notify_Pld_Zone_Report zone );
Boolean 			dei_sc_icon_update_page					( void );
 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Dei_Sc               	dei_sc                             ; 
extern Sync_Count_Obj           dei_sc_sync_count[ DEI_RF_ID_COUNT];
extern UInt32                   dei_sc_current_sn                  ;
extern Dei_Sc_Info          	dei_sc_info                        ;
extern Dei_Sc_Mode          	dei_sc_mode                        ;
extern Dei_Sc_NGRF_Mode       	dei_sc_ngrf_mode                   ;
extern Q                        dei_sc_tx_q_pending                ;
extern Q                        dei_sc_tx_q_free                   ;
extern Boolean                  dei_sc_is_silent                   ;
extern Timeout                  dei_sc_timeout_pagging             ;
extern Timeout                  dei_sc_timeout_lock                ;
extern Boolean                  dei_sc_rf_type                     ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#endif





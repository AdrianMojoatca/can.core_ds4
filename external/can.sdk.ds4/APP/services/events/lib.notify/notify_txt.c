/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: notify_txt.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "notify_private.h"
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

/*--------------------------------------------------------------------------*/

UInt8* notify_txt( Notify nf )
{
	switch ( nf )
	{
		case NOTIFY_LOCK                           :  return "N01";                    	
		case NOTIFY_UNLOCK                         :  return "N02";                  		
		case NOTIFY_VALET_ON                       :  return "N03";                		    	  
		case NOTIFY_VALET_OFF                      :  return "N04";               		    	  
    case NOTIFY_TRUNK_RELEASE_OPEN             :  return "N05";      	        	
		case NOTIFY_TRUNK_RELEASE_CLOSE            :  return "N06";     		    
		case NOTIFY_AUX_CH3_ON                     :  return "N07";              		
		case NOTIFY_AUX_CH3_OFF                    :  return "N08";              		
	  case NOTIFY_AUX_CH4_ON                     :  return "N09";              		
		case NOTIFY_AUX_CH4_OFF                    :  return "N11";                 	
    case NOTIFY_AUX_CH5_ON                     :  return "N12";              		
	  case NOTIFY_AUX_CH5_OFF                    :  return "N13";             		
		case NOTIFY_AUX_CH6_ON                     :  return "N14";              				
		case NOTIFY_AUX_CH6_OFF                    :  return "N15";             		
    case NOTIFY_AUX_CH2_TYPE                   :  return "N16";            		
		case NOTIFY_AUX_CH3_TYPE                   :  return "N17";            		
	  case NOTIFY_AUX_CH4_TYPE                   :  return "N18";            		
	  case NOTIFY_AUX_CH5_TYPE                   :  return "N19";            		
	  case NOTIFY_SENSOR1_TYPE                   :  return "N20";            		
		case NOTIFY_SENSOR2_TYPE                   :  return "N21";            		
		case NOTIFY_SENSOR3_TYPE                   :  return "N22";            			
		case NOTIFY_PANIC                          :  return "N23";                   		
		case NOTIFY_RANGE_CHECK                    :  return "N24";             		
		case NOTIFY_CAR_FINDER                     :  return "N25";              		
		case NOTIFY_SENSOR1_ADJUST                 :  return "N26";          		
		case NOTIFY_SENSOR_BYPASS_WARN             :  return "N27";      			 
		case NOTIFY_SENSOR_BYPASS_SHOCK            :  return "N28";     		
		case NOTIFY_SENSOR_BYPASS_OPT              :  return "N28";       		
		case NOTIFY_SENSOR_BYPASS_FULL             :  return "N29";      		
		case NOTIFY_SENSOR_BYPASS_OFF              :  return "N30";       		
		case NOTIFY_SENSOR_SILENT_ARM              :  return "N31";       		
		case NOTIFY_FULL_SILENT_ARM                :  return "N32";         		
		case NOTIFY_LAST_TRIGGER                   :  return "N33";            		
		case NOTIFY_ERROR                          :  return "N34";                   		
		case NOTIFY_TRIGGER                        :  return "N35";                 		
		case NOTIFY_WARNING                        :  return "N36";                 			          
		case NOTIFY_RS_START                       :  return "N37";                			
		case NOTIFY_RS_STOP                        :  return "N38";                 			          
		case NOTIFY_TIMER_MODE_ON                  :  return "N39";           			          
		case NOTIFY_TIMER_MODE_OFF                 :  return "N40";          			          
		case NOTIFY_SMART_MODE_ON                  :  return "N41";           			          
		case NOTIFY_SMART_MODE_OFF                 :  return "N42";          			          
		case NOTIFY_TIMER_ADJUST_ENTER             :  return "N43";      			          
		case NOTIFY_TIMER_SET_START_COUNT					 :  return "N44";		    	          
		case NOTIFY_TIMER_SET_TIME_START           :  return "N45";           	                  
		case NOTIFY_SHOW_TEMPERATURE               :  return "N46";               	                  
		case NOTIFY_TEMP_ADJUST_ENTER	       	   	 :  return "N47";	       	                  
		case NOTIFY_LOW_TEMP_UPDATE                :  return "N48";         			                  
		case NOTIFY_HIGH_TEMP_UPDATE               :  return "N49";        			                  
		case NOTIFY_RUNTIME_PROGRAMMED             :  return "N50";      			                  
		case NOTIFY_RUNTIME_REMAINING              :  return "N51";       			                  
		case NOTIFY_RUNTIME_RESET                  :  return "N52";           			                  
		case NOTIFY_RUNTIME_LOW                    :  return "N53";             			                  
		case NOTIFY_RUNTIME_CHECK                  :  return "N54";           			                      
		case NOTIFY_DEFOGGER_ON                    :  return "N55";             			                      
		case NOTIFY_DEFOGGER_OFF                   :  return "N56";            			                      	
		case NOTIFY_KEYBOARD_UNLOCK                :  return "N57";         			                      	
		case NOTIFY_PAGE_UPDATE	               	   :  return "N58";	        	    	                      	
		case NOTIFY_MTS_ENABLE                     :  return "N59";                  			                      	
		case NOTIFY_MTS_DISABLE                    :  return "N60";                 			                      	        
		case NOTIFY_PROGRAMMING_ENTER              :  return "N61";       	    	                      	            
		case NOTIFY_PROGRAMMING_LEAVE              :  return "N62";       	    	                      	            
		case NOTIFY_REMOTE_DELETE		       	   		 :  return "N63";		        	                      	            
		case NOTIFY_REMOTE_DELETE_ENTER	       	   :  return "N64";
		case NOTIFY_RS_STOP_ACK		               	 :  return "N65";
		case NOTIFY_RS_VALET_ON					   				 :  return "N66";
		case NOTIFY_RS_VALET_OFF				   				 :  return "N67";
		case NOTIFY_CONVENIENCE_ALERT			   			 :  return "N68";
		default : break;
	}
	return "N69" ;
}

/*--------------------------------------------------------------------------*/


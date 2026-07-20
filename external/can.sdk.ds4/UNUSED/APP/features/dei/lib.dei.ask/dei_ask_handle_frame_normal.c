/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_handle_frame_normal.c 31490 2015-05-13 15:36:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ask_private.h"
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

//--------------------------------------------------------------------------//

void dei_ask_handle_frame_normal_mode( void )
{    
    decode_normal_frame:
    {    
        UInt8 database_index = dei_ask_check_frame_normal_mode();                                
                    
        if(database_index < DEI_ASK_MAX_REMOTES)
        {                                                                        
            if(dei_ask.data_fix.bits.button != DEI_ASK_BUTTON_START_STOP)
            {
                if(dei_ask.database.remote[database_index].is_1button)
                {
                    dei_ask.database.remote[database_index].is_1button = FALSE;
                    
                    dei_ask_database_save();
                }
            }
                        
            if(dei_ask.last_button != dei_ask.data_fix.bits.button)
            {
                dei_ask.frame_count = 0 ;
                            
                if(dei_ask.last_button != DEI_ASK_BUTTON_UNKNOWN)
                {
                    // send the release of next button
                    TRACE_DEI_ASK("\n\r[LIB.DEI.ASK]   %02x - button release  \n\r" , dei_ask.last_button);
                    
                    dei_ask_handle_button_event(dei_ask.last_button , DEI_ASK_RELEASE , dei_ask.last_is_1button);
                }
                else
                {
                    // send the first button press                    
                    TRACE_DEI_ASK("\n\r[LIB.DEI.ASK]   %02x - button first press    \n\r" , dei_ask.data_fix.bits.button);
                    
//                    if((dei_ask.last_data_enc == dei_ask.data_enc.data) && (dei_ask.last_data_fix == dei_ask.data_fix.data))
//                      {
//                        TRACE_DEI_ASK("\n\n[LIB.DEI.ASK] ***** FRAME Was Skipped *****\n\n");
//                      } 
//                    else
                      {
                        dei_ask_handle_button_event(dei_ask.data_fix.bits.button , DEI_ASK_FIRST_PRESS,dei_ask.database.remote[database_index].is_1button);
                      }                     
                }
            }
            
            dei_ask.last_button     = dei_ask.data_fix.bits.button;
            dei_ask.last_is_1button = dei_ask.database.remote[database_index].is_1button;
            
            dei_ask.last_data_enc = dei_ask.data_enc.data;
            dei_ask.last_data_fix = dei_ask.data_fix.data;

                        
            if(dei_ask_receive(300) == FALSE)
            {
                if(dei_ask.last_button != DEI_ASK_BUTTON_UNKNOWN)
                {
                    // send the release of next button
                    TRACE_DEI_ASK("\n\r[LIB.DEI.ASK]   %02x - button release1  \n\r" , dei_ask.last_button);
                    
                    dei_ask_handle_button_event(dei_ask.last_button , DEI_ASK_RELEASE , dei_ask.last_is_1button);
                }
                
                dei_ask.last_button = DEI_ASK_BUTTON_UNKNOWN;
            }
            else
            {
                database_index = dei_ask_check_frame_normal_mode(); 
                
                if(database_index < DEI_ASK_MAX_REMOTES)
                {   
                    if(dei_ask.data_fix.bits.button != DEI_ASK_BUTTON_START_STOP)
                    {
                        if(dei_ask.database.remote[database_index].is_1button)
                        {
                            dei_ask.database.remote[database_index].is_1button = FALSE;
                    
                            dei_ask_database_save();
                        }
                    }
                                
                    dei_ask.last_data_enc = dei_ask.data_enc.data;
                    dei_ask.last_data_fix = dei_ask.data_fix.data;  
                                        
                    if(dei_ask.last_button != dei_ask.data_fix.bits.button)
                    {
                         dei_ask.frame_count = 0 ;
                            
                         TRACE_DEI_ASK("\n\r[LIB.DEI.ASK]   %02x - button release3      \n\r" , dei_ask.last_button);
                         
                         dei_ask_handle_button_event(dei_ask.last_button          , DEI_ASK_RELEASE     , FALSE);                        
                         dei_ask_handle_button_event(dei_ask.data_fix.bits.button , DEI_ASK_FIRST_PRESS , dei_ask.database.remote[database_index].is_1button );
                        
                         TRACE_DEI_ASK("\n\r[LIB.DEI.ASK]   %02x - button first press2  \n\r" , dei_ask.data_fix.bits.button);                        
                    }
                    else
                    {
                        if(dei_ask.frame_count == 20 )
                        {
                            // send the extended press
                            TRACE_DEI_ASK("\n\r[LIB.DEI.ASK]   %02x - button extended press  \n\r" , dei_ask.last_button);       

                            dei_ask_handle_button_event(dei_ask.last_button , DEI_ASK_EXTENDED_PRESS , dei_ask.database.remote[database_index].is_1button);                        
                        }
                    
                        if(dei_ask.frame_count < 21)
                        {
                            dei_ask.frame_count ++;
                        }
                    }   
                    
                    dei_ask.last_button     = dei_ask.data_fix.bits.button;
                    dei_ask.last_is_1button = dei_ask.database.remote[database_index].is_1button;
                                    
                    goto  decode_normal_frame;   
                }
                else
                {
                    if(dei_ask.last_button != DEI_ASK_BUTTON_UNKNOWN)
                    {
                        // send the release of next button
                        TRACE_DEI_ASK("\n\r[LIB.DEI.ASK]   %02x - button release2  \n\r" , dei_ask.last_button);
                        
                        dei_ask_handle_button_event(dei_ask.last_button,DEI_ASK_RELEASE,dei_ask.last_is_1button);                        
                    }   
                
                    dei_ask.last_button = DEI_ASK_BUTTON_UNKNOWN;
                }
            }            
        }                
    }
}

//--------------------------------------------------------------------------//    

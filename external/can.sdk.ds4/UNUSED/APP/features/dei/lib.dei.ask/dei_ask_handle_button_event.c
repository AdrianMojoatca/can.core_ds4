/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_handle_button_event.c 31490 2015-05-13 15:36:35Z martin.bouchard $
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

void dei_ask_handle_button_event( Dei_Ask_Button button , Dei_Ask_Press_Type press_type , Boolean is_one_button )
{    
    Cmd_Rf_Pld  btn_pld;
    
    btn_pld.bits.pld 	  = 0;
    btn_pld.bits.id  	  = dei_ask.data_fix.bits.sn   ;
	btn_pld.bits.batt_low = 0 ; //dei_ask.data_fix.bits.v_low; - for the moment ignore the low batt info from remote because seems that is wrong 
    
    switch(button)
    {
        case DEI_ASK_BUTTON_LOCK:            
        {
            switch(press_type)
            {
                case DEI_ASK_FIRST_PRESS    :cmd_q_rs_insert( INPUT_RF_LOCK                    , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_EXTENDED_PRESS :cmd_q_rs_insert( INPUT_RF_LOCK | INPUT_RF_EXT     , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_RELEASE        :cmd_q_rs_insert( INPUT_RF_LOCK | INPUT_RF_RELEASE , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    
                default:break;
            }
        }
        break;
        
        case DEI_ASK_BUTTON_UNLOCK:            
        {
            switch(press_type)
            {
                case DEI_ASK_FIRST_PRESS    :cmd_q_rs_insert( INPUT_RF_UNLOCK                    , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_EXTENDED_PRESS :cmd_q_rs_insert( INPUT_RF_UNLOCK | INPUT_RF_EXT     , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_RELEASE        :cmd_q_rs_insert( INPUT_RF_UNLOCK | INPUT_RF_RELEASE , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    
                default:break;
            }
        }
        break;
        
        case DEI_ASK_BUTTON_TRUNK:            
        {
            switch(press_type)
            {
                case DEI_ASK_FIRST_PRESS    :cmd_q_rs_insert( INPUT_RF_TRUNK                    , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_EXTENDED_PRESS :cmd_q_rs_insert( INPUT_RF_TRUNK | INPUT_RF_EXT     , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_RELEASE        :cmd_q_rs_insert( INPUT_RF_TRUNK | INPUT_RF_RELEASE , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    
                default:break;
            }
        }
        break;
                        
        case DEI_ASK_BUTTON_AUX1:
        {
            switch(press_type)
            {
                case DEI_ASK_FIRST_PRESS    :cmd_q_rs_insert( INPUT_RF_AUX1                    , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_EXTENDED_PRESS :cmd_q_rs_insert( INPUT_RF_AUX1 | INPUT_RF_EXT     , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_RELEASE        :cmd_q_rs_insert( INPUT_RF_AUX1 | INPUT_RF_RELEASE , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    
                default:break;
            }
        }
        break;
        
        case DEI_ASK_BUTTON_AUX2:
        {
            switch(press_type)
            {
                case DEI_ASK_FIRST_PRESS    :cmd_q_rs_insert( INPUT_RF_AUX2                    , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_EXTENDED_PRESS :cmd_q_rs_insert( INPUT_RF_AUX2 | INPUT_RF_EXT     , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_RELEASE        :cmd_q_rs_insert( INPUT_RF_AUX2 | INPUT_RF_RELEASE , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    
                default:break;
            }
        }
        break;
		
		case DEI_ASK_BUTTON_CAR_FINDER:
        {
            switch(press_type)
            {
                case DEI_ASK_FIRST_PRESS    :cmd_q_rs_insert( INPUT_RF_CAR_FINDER                    , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_EXTENDED_PRESS :cmd_q_rs_insert( INPUT_RF_CAR_FINDER | INPUT_RF_EXT     , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                case DEI_ASK_RELEASE        :cmd_q_rs_insert( INPUT_RF_CAR_FINDER | INPUT_RF_RELEASE , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    
                default:break;
            }
        }
        break;
        
        
        case DEI_ASK_BUTTON_START_STOP:            
        {
            if(is_one_button)                
            {            
                static Boolean     first_press = FALSE;
                static Cmd_Rf_Pld  btn_pld_copy;

                switch(press_type)
                {
                    case DEI_ASK_FIRST_PRESS    :
                    {
                        first_press  = TRUE;
                        btn_pld_copy = btn_pld;
                    }
                    break; 

                        
                    case DEI_ASK_EXTENDED_PRESS :
                    {
                        first_press = FALSE;
                        
                        cmd_q_rs_insert( INPUT_RF_STOP , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );
                    }
                    break;
                        
                    case DEI_ASK_RELEASE:
                    {
                        if(first_press)
                        {
                            first_press = FALSE;

                            cmd_q_rs_insert( INPUT_RF_ONE_BUTTON_FUNCTION , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld_copy.pld );
                        }
                    }
                    break;

                    default:break;
                }
            }
            else
            {
                switch(press_type)
                {
                    case DEI_ASK_FIRST_PRESS    :cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE                    , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    case DEI_ASK_EXTENDED_PRESS :cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE | INPUT_RF_EXT     , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                    case DEI_ASK_RELEASE        :cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE | INPUT_RF_RELEASE , DEI_ASK_CMD_Q_INSERT_TIMEOUT , btn_pld.pld );break;
                        
                    default:break;
                }
            }
        }
        break;
        
        
        
        default:break;
    }                                    
}

//--------------------------------------------------------------------------//    

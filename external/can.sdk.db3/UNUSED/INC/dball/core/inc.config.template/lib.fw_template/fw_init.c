/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fw_init.c 24872 2014-03-24 15:02:13Z martin.bouchard $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "fw.h"
#include "trace.h"
#include "detect.h"
#include "cmd.h"
#include "sense_notify.h"
//#include "rxt.h"
//#include "rsr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
#if USE_TRACE == 1
static void fw_dbg(char key) ;
#endif
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void fw_init( void )
{
/* place here the init functions for digital firmware */  
  #if USE_TRACE == 1
    trace_func_update(fw_dbg) ;
  #endif

  if(detect_bp_is_skip())
    {
      //#warning "init RXT here"
      //rxt_init(function_rxt_start, function_rxt_stop);    
    }
  else
    {
      //#warning "init RSR here"      
      //rsr_init(0) ; 
    }   
}
/*--------------------------------------------------------------------------*/
static void fw_dbg(char key)
{
  #if USE_TRACE == 1

    /* place here the debug functions for digital firmware */
    /* The CMD is UART cmd received with TRACE_RX function */
         
    switch( key )
      {
        case 'l' : cmd_q_insert (INPUT_LOCK1            | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        case 'u' : cmd_q_insert (INPUT_UNLOCK_DRIVER1   | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        case 't' : cmd_q_insert (INPUT_TRUNK            | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        case '1' : cmd_q_insert (INPUT_AUX1             | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        case '2' : cmd_q_insert (INPUT_AUX2             | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        case '3' : cmd_q_insert (INPUT_AUX3             | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        
        case 'g' : 
          {
            static Boolean g_state = FALSE ;

            g_state ^= 1 ;
            if(g_state)
              cmd_q_insert (INPUT_GWR | INPUT_ON  , COMMAND_INSERT_TIMEOUT); 
            else
              cmd_q_insert (INPUT_GWR             , COMMAND_INSERT_TIMEOUT); 
            
            break ; 
          }
          
        case 'i' : 
          {
            static Boolean i_state = FALSE ;

            i_state ^= 1 ;
            
            if(i_state)
              cmd_q_insert (INPUT_IGNITION | INPUT_ON  , COMMAND_INSERT_TIMEOUT); 
            else
              cmd_q_insert (INPUT_IGNITION             , COMMAND_INSERT_TIMEOUT); 

            break ; 
          }  
        case 's' : 
          {
            static Boolean s_state = FALSE ;

            s_state ^= 1 ;
            if(s_state)
              cmd_q_insert (INPUT_STARTER | INPUT_ON  , COMMAND_INSERT_TIMEOUT); 
            else
              cmd_q_insert (INPUT_STARTER             , COMMAND_INSERT_TIMEOUT); 
            
            break ;
          }
        
        case 'a' : cmd_q_insert (INPUT_ARM              | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        case 'd' : cmd_q_insert (INPUT_DISARM           | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
        
        case 'S' : cmd_q_insert (INPUT_START            | INPUT_ON  , COMMAND_INSERT_TIMEOUT); break ; 
     
        case 'p' : 
          {
            static Boolean p_state = FALSE ;

            p_state ^= 1 ;

            cmd_q_insert(p_state? INPUT_PARKING_ON : INPUT_PARKING_OFF ,COMMAND_INSERT_TIMEOUT);
          }break;

          default  :                                          break ; 
      }

  #endif  
}


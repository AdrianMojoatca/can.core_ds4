/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_rpm.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "tach.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#if DBG_SENSE == 1

static UInt16 rpm_last_value ;

#endif

static UInt16 fake_rpm_value = 0;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void sense_notify_rpm(UInt16 value)
{
  static Boolean state = FALSE;
 

  if(fake_rpm_value == FAKE_RPM_300 && value < FAKE_RPM_300)
    {
      value = FAKE_RPM_300;
    }
    
  rpm.word = value ;
 
  tach_update (rpm.word) ;
            
  if( !!rpm.word != state && rpm.word > FAKE_RPM_300)
    {
      state = !!rpm.word;
        
      cmd_q_insert(INPUT_RPM_SET, COMMAND_INSERT_TIMEOUT );
    }
  else if (!!rpm.word != state && rpm.word == NULL)
    {
      state = !!rpm.word;
        
      cmd_q_insert(INPUT_RPM_CLR, COMMAND_INSERT_TIMEOUT );      
    }

  #if DBG_SENSE == 1
    {
      if(value != rpm_last_value)
        {
          if((value > (rpm_last_value +500)) || ((rpm_last_value > (value + 500))))          
            {
              sense_notify_dbg_print("\nSense Rpm Upd +/- 500 RPm" , NULL , NULL);
            }
         
          if(value == 0)
            {
              sense_notify_dbg_print("\nSense Rpm Off" , NULL , NULL);
            } 
          
          rpm_last_value = value ;
        }
    } 
  #endif
   

}

//--------------------------------------------------------------------------//
void sense_notify_gwr_on (void * arg)
{
  sense_notify_gwr_flag = TRUE ;    
}
//--------------------------------------------------------------------------//
void sense_notify_gwr_off (void * arg)
{
  sense_notify_gwr_flag = FALSE ;  
}
//--------------------------------------------------------------------------//
void sense_notify_fake_rpm_on (void * arg)
{
  if(sense_notify_gwr_flag)
  {
    fake_rpm_value = FAKE_RPM_300;
    
    sense_notify_rpm(FAKE_RPM_300); 
    
    #if DBG_SENSE == 1
      {
        sense_notify_dbg_print("\nFake rpm set to 300" , NULL , NULL); 
      }
    #endif  
  }      

}
//--------------------------------------------------------------------------//
void sense_notify_fake_rpm_off (void * arg)
{
   fake_rpm_value = NULL;
   
   if(rpm.word == FAKE_RPM_300)
     {  
        sense_notify_rpm(NULL);
         
        #if DBG_SENSE == 1
          {        
            sense_notify_dbg_print("\nFake rpm clr" , NULL , NULL);
          } 
          #endif
     }       
}


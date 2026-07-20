/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_cutom.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
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

void sense_rs_custom_neg1( Boolean do_init , Boolean status, UInt8 from)
{
    //The active level is LOW for negative inputs
    //We will insert CMD_NEG1_ON  if the level is changed from INACTIVE to ACTIVE
    //               CMD_NEG1_OFF if the level is changed from ACTIVE to INACTIVE
        
    #ifndef CMD_NEG1_ON
        #define CMD_NEG1_ON     INPUT_INVALID        
    #endif

    #ifndef CMD_NEG1_OFF
        #define CMD_NEG1_OFF    INPUT_INVALID
    #endif
    
    UInt8 cmd_on = CMD_NEG1_ON , cmd_off = CMD_NEG1_OFF;        
    
    cmd_q_rs_insert(status ? CMD_NEG1_OFF : CMD_NEG1_ON  , TIMEOUT_Q_INSERT  );
                  
    TRACE_DEI_SENSE_RS("\n\r[CMD_NEG1_%s]\n\r" , status ? "OFF":"ON" );	
        
}

/*--------------------------------------------------------------------------*/     	

void sense_rs_custom_neg2( Boolean do_init , Boolean status )
{
    //The active level is LOW for negative inputs
    //We will insert CMD_NEG2_ON  if the level is changed from INACTIVE to ACTIVE
    //               CMD_NEG2_OFF if the level is changed from ACTIVE to INACTIVE
    
    #ifndef CMD_NEG2_ON
        #define CMD_NEG2_ON     INPUT_INVALID        
    #endif

    #ifndef CMD_NEG2_OFF
        #define CMD_NEG2_OFF    INPUT_INVALID
    #endif
    
    
    cmd_q_rs_insert(status ? CMD_NEG2_OFF : CMD_NEG2_ON  , TIMEOUT_Q_INSERT  );

    TRACE_DEI_SENSE_RS("\n\r[CMD_NEG2_%s]\n\r" , status ? "OFF":"ON" );	

    #undef INSERT_ON
    #undef INSERT_OFF
        
}

/*--------------------------------------------------------------------------*/     	

void sense_rs_custom_neg3( Boolean do_init , Boolean status )
{
    //The active level is LOW for negative inputs
    //We will insert CMD_NEG2_ON  if the level is changed from INACTIVE to ACTIVE
    //               CMD_NEG2_OFF if the level is changed from ACTIVE to INACTIVE
    
    #ifndef CMD_NEG3_ON
        #define CMD_NEG3_ON     INPUT_INVALID        
    #endif

    #ifndef CMD_NEG3_OFF
        #define CMD_NEG3_OFF    INPUT_INVALID
    #endif
    
    
    cmd_q_rs_insert(status ? CMD_NEG3_OFF : CMD_NEG3_ON  , TIMEOUT_Q_INSERT  );

    TRACE_DEI_SENSE_RS("\n\r[CMD_NEG3_%s]\n\r" , status ? "OFF":"ON" );	
        
}

/*--------------------------------------------------------------------------*/     	

void sense_rs_custom_pos1( Boolean do_init , Boolean status, UInt8 from)
{
    //The active level is LOW for positive inputs
    //We will insert CMD_POS1_ON  if the level is changed from INACTIVE to ACTIVE
    //               CMD_POS1_OFF if the level is changed from ACTIVE to INACTIVE
    
    #ifndef CMD_POS1_ON
        #define CMD_POS1_ON     INPUT_INVALID        
    #endif

    #ifndef CMD_POS1_OFF
        #define CMD_POS1_OFF    INPUT_INVALID
    #endif
    
    
    cmd_q_rs_insert(status ? CMD_POS1_OFF : CMD_POS1_ON  , TIMEOUT_Q_INSERT  );

   
    TRACE_DEI_SENSE_RS("\n\r[CMD_POS1_%s]\n\r" , status ? "OFF":"ON" );	
        
}

/*--------------------------------------------------------------------------*/     	

void sense_rs_custom_pos2( Boolean do_init , Boolean status )
{
    //The active level is LOW for positive inputs
    //We will insert CMD_POS2_ON  if the level is changed from INACTIVE to ACTIVE
    //               CMD_POS2_OFF if the level is changed from ACTIVE to INACTIVE
    
    #ifndef CMD_POS2_ON
        #define CMD_POS2_ON     INPUT_INVALID        
    #endif

    #ifndef CMD_POS2_OFF
        #define CMD_POS2_OFF    INPUT_INVALID
    #endif
    
    
    cmd_q_rs_insert(status ? CMD_POS2_OFF : CMD_POS2_ON  , TIMEOUT_Q_INSERT  );


    TRACE_DEI_SENSE_RS("\n\r[CMD_POS2_%s]\n\r" , status ? "OFF":"ON" );	
        
}

/*--------------------------------------------------------------------------*/     	

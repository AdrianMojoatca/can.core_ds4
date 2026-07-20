/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_windows_pos.c 2939 2011-07-05 09:52:31Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean left_front_closed   = FALSE ; 
static Boolean right_front_closed  = FALSE ; 
static Boolean left_rear_closed    = FALSE ; 
static Boolean right_rear_closed   = FALSE ;

static UInt8 left_front_pos        = 0 ; 
static UInt8 right_front_pos       = 0 ; 
static UInt8 left_rear_pos         = 0 ; 
static UInt8 right_rear_pos        = 0 ; 

static UInt8 saved_left_front_pos  = 0 ; 
static UInt8 saved_right_front_pos = 0 ; 
static UInt8 saved_left_rear_pos   = 0 ; 
static UInt8 saved_right_rear_pos  = 0 ; 

static Boolean with_4_windows      = FALSE ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_left_front_window_xxx( Can_Msg *can_msg )
{
    left_front_closed  = CAST_BOOL( DATA[3]<0x04);    
    left_front_pos     = DATA[3] ; 
}

//--------------------------------------------------------------------------//

void sense_right_front_window_xxx( Can_Msg *can_msg )
{
    right_front_closed  = CAST_BOOL( DATA[3]<0x04);    
    right_front_pos     = DATA[3] ; 
}

//--------------------------------------------------------------------------//

void sense_left_rear_window_xxx( Can_Msg *can_msg )
{
    with_4_windows = TRUE ;
    
    left_rear_closed  = CAST_BOOL( DATA[3]<0x04);    
    left_rear_pos     = DATA[3] ; 
}

//--------------------------------------------------------------------------//

void sense_right_rear_window_xxx( Can_Msg *can_msg )
{
    with_4_windows = TRUE ;
    
    right_rear_closed  = CAST_BOOL( DATA[3]<0x04);    
    right_rear_pos     = DATA[3] ;     
}

//--------------------------------------------------------------------------//

void sense_save_windows_pos( void )
{
    saved_right_rear_pos  = right_rear_pos  ; 
    saved_left_rear_pos   = left_rear_pos   ; 
    saved_right_front_pos = right_front_pos ; 
    saved_left_front_pos  = left_front_pos  ;
} 

//--------------------------------------------------------------------------//

Boolean sense_get_windows_closed( void )
{
    #define FRONT_CLOSED (left_front_closed  && right_front_closed) 
    #define REAR_CLOSED  (left_rear_closed   && right_rear_closed )
    
    return with_4_windows ? FRONT_CLOSED && REAR_CLOSED : FRONT_CLOSED ;    

    #undef FRONT_CLOSED
    #undef REAR_CLOSED

}

//--------------------------------------------------------------------------//

Boolean sense_get_windows_vent( void )
{
    #define LEFT_FRONT_VENT  ((saved_left_front_pos   > left_front_pos       )                        ?\
                             ((saved_left_front_pos   - left_front_pos       )>SENSE_WINDOW_VENT):\
                             ((left_front_pos         - saved_left_front_pos )>SENSE_WINDOW_VENT))


    #define RIGHT_FRONT_VENT ((saved_right_front_pos  > right_front_pos      )                        ?\
                             ((saved_right_front_pos  - right_front_pos      )>SENSE_WINDOW_VENT):\
                             ((right_front_pos        - saved_right_front_pos)>SENSE_WINDOW_VENT))


    #define LEFT_REAR_VENT   ((saved_left_rear_pos    > left_rear_pos        )                        ?\
                             ((saved_left_rear_pos    - left_rear_pos        )>SENSE_WINDOW_VENT):\
                             ((left_rear_pos          - saved_left_rear_pos  )>SENSE_WINDOW_VENT))


    #define RIGHT_REAR_VENT  ((saved_right_rear_pos   > right_rear_pos       )                        ?\
                             ((saved_right_rear_pos   - right_rear_pos       )>SENSE_WINDOW_VENT):\
                             ((right_rear_pos         - saved_right_rear_pos )>SENSE_WINDOW_VENT))



    return with_4_windows ? (LEFT_FRONT_VENT || RIGHT_FRONT_VENT || LEFT_REAR_VENT || RIGHT_REAR_VENT) : (LEFT_FRONT_VENT || RIGHT_FRONT_VENT) ;

    
    #undef LEFT_FRONT_VENT
    #undef RIGHT_FRONT_VENT
    #undef LEFT_REAR_VENT
    #undef RIGHT_REAR_VENT
}

//--------------------------------------------------------------------------//


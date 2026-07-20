/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: w2w_execute.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "w2w_private.h"
#include "wake.h"
#include "bitfield.h"
#include "core_contract_db3_gpio_input.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void w2w_send_cmd      (Cmd input_cmd) ;

static void w2w_check_gwr     (void) ;
static void w2w_check_ign     (void) ;
static void w2w_check_start   (void) ;
static void w2w_check_pos_in1 (void) ;
static void w2w_check_lock    (void) ;
static void w2w_check_unlock  (void) ;
static void w2w_check_trunk   (void) ;
static void w2w_check_aux1    (void) ;
static void w2w_check_aux2    (void) ;
static void w2w_check_aux3    (void) ;
static void w2w_check_push    (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Per_Exec_Wake wake_status ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


Per_Exec_Wake w2w_execute (void)
{ 
  wake_status = PER_EXEC_SLEEP ;

  w2w_check_ign      () ;
  w2w_check_pos_in1  () ;

  w2w_check_start  () ;

  if (bitfield_rd_u(w2w_ena_table  , W2W_ENA_GWR    , W2W_ENA_GWR   )) w2w_check_gwr    () ;
  if (bitfield_rd_u(w2w_ena_table  , W2W_ENA_LOCK   , W2W_ENA_LOCK  )) w2w_check_lock   () ;
  if (bitfield_rd_u(w2w_ena_table  , W2W_ENA_UNLOCK , W2W_ENA_UNLOCK)) w2w_check_unlock () ;
  if (bitfield_rd_u(w2w_ena_table  , W2W_ENA_TRUNK  , W2W_ENA_TRUNK )) w2w_check_trunk  () ;
  if (bitfield_rd_u(w2w_ena_table  , W2W_ENA_AUX1   , W2W_ENA_AUX1  )) w2w_check_aux1   () ;
  if (bitfield_rd_u(w2w_ena_table  , W2W_ENA_AUX2   , W2W_ENA_AUX2  )) w2w_check_aux2   () ;
  if (bitfield_rd_u(w2w_ena_table  , W2W_ENA_AUX3   , W2W_ENA_AUX3  )) w2w_check_aux3   () ;

  w2w_check_push   () ;

  return wake_status ;//PER_EXEC_SLEEP;
}

//---------------------------------------------------- //
void w2w_set_level (void)
{
  ied_set_level (ied_gwr      , FALSE ) ;
  ied_set_level (ied_ign      , FALSE ) ;
  ied_set_level (ied_start    , FALSE ) ;
  ied_set_level (ied_pos_in1  , FALSE ) ;
  ied_set_level (ied_lock     , TRUE  ) ;
  ied_set_level (ied_unlock   , TRUE  ) ;
  ied_set_level (ied_trunk    , TRUE  ) ;
  ied_set_level (ied_aux1     , TRUE  ) ;
  ied_set_level (ied_aux2     , TRUE  ) ;
  ied_set_level (ied_aux3     , TRUE  ) ;
}

//---------------------------------------------------- //

static void w2w_send_cmd (Cmd input_cmd)
{ 
    cmd_q_insert (input_cmd, COMMAND_INSERT_TIMEOUT , NULL) ;

    w2w_dbg_print(input_cmd) ;

    wake_status = PER_EXEC_WAKE ;
}

//---------------------------------------------------- //
static void w2w_check_gwr (void)
{
	
    switch (ied_detect (ied_gwr, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_7)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_GWR | INPUT_ON)) ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_GWR))            ; break ;
    }
	

  /* ADI M added - 14_05_2026 */
	/*
  	switch (ied_detect (ied_gwr, negative input 7))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_GWR | INPUT_ON)) ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_GWR))            ; break ;
    }
	*/
	/*
  switch (ied_detect (ied_gwr, negative input 7))
    {
      case +1 : w2w_send_cmd  ((Cmd_List)(INPUT_GWR_ON))  ; TRACE("\r W2W GWR ON");  break ;
      case -1 : w2w_send_cmd  ((Cmd_List)(INPUT_GWR_OFF)) ; TRACE("\r W2W GWR_OFF"); break ;
    }
	*/
}

//---------------------------------------------------- //
static void w2w_check_ign (void)
{
// no lockout get because the wire ign is not the same as the d2d ign
// so we always need to check the wire ign AND d2d ign

  switch (ied_detect (ied_ign, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_POSITIVE_2)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_IGNITION | INPUT_ON)) ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_IGNITION))            ; break ;
    }
}

//---------------------------------------------------- //

static void w2w_check_start (void)
{
// no lockout get because the wire start is not the same as the d2d start
// so we always need to check the wire start AND d2d start

  switch (ied_detect (ied_start, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_POSITIVE_3)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_STARTER | INPUT_ON)) ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_STARTER))             ; break ;
    }
}    

//---------------------------------------------------- //
static void w2w_check_pos_in1 (void)
{
  /*
  switch (ied_detect (ied_pos_in1, positive input 1))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(W2W_POS1_CMD | INPUT_ON));  ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(W2W_POS1_CMD));             ; break ;
    }
	*/
  	/**** ADI M - 08_02_2025 ****/
  switch (ied_detect (ied_pos_in1, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_POSITIVE_1)))
    {
      case +1 : {w2w_send_cmd ((Cmd_List)(INPUT_PARKING_ON )); /*TRACE("\r\nCHECK POS IN 1: ON\r\n" );*/}  ; break ;
      case -1 : {w2w_send_cmd ((Cmd_List)(INPUT_PARKING_OFF)); /*TRACE("\r\nCHECK POS IN 1: OFF\r\n" );*/} ; break ;
    }

}

//---------------------------------------------------- //
static void w2w_check_lock (void)
{
  switch (ied_detect (ied_lock, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_1)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_LOCK1))            ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_LOCK1 | INPUT_ON)) ; break ;
    }
}

//---------------------------------------------------- //
static void w2w_check_unlock (void)
{
  switch (ied_detect (ied_unlock, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_2)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_UNLOCK_DRIVER1))            ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_UNLOCK_DRIVER1 | INPUT_ON)) ; break ;
    }
}

//---------------------------------------------------- //
static void w2w_check_trunk (void)
{
  switch (ied_detect (ied_trunk, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_3)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_TRUNK))            ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_TRUNK | INPUT_ON)) ; break ;
    }
}

//---------------------------------------------------- //
static void w2w_check_aux1 (void)
{
  switch (ied_detect (ied_aux1, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_4)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_AUX1))            ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_AUX1 | INPUT_ON)) ; break ;
    }
}

//---------------------------------------------------- //
static void w2w_check_aux2 (void)
{
  switch (ied_detect (ied_aux2, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_5)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_AUX2))            ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_AUX2 | INPUT_ON)) ; break ;
    }
}

//---------------------------------------------------- //
static void w2w_check_aux3 (void)
{
  switch (ied_detect (ied_aux3, core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_6)))
    {
      case +1 : w2w_send_cmd ((Cmd_List)(INPUT_AUX3))            ; break ;
      case -1 : w2w_send_cmd ((Cmd_List)(INPUT_AUX3 | INPUT_ON)) ; break ;
    }
}
//---------------------------------------------------- //

static void w2w_check_push (void)
{
    static Boolean is_pressed  = FALSE ;
    static Boolean is_extended = FALSE ;
    static UInt32   time       = 0 ;

    switch( ied_detect( ied_push , !core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN) ) )
    {
        case +1 :
        {
            w2w_send_cmd( (Cmd)INPUT_PUSH_PRESS ) ; 
            
            is_pressed  = TRUE ;
            is_extended = FALSE; 

            //TRACE_WW("\n\r[W2W_PUSH_PRESS]\n\r");
            
            time = 0 ;          
        }
        break ;
        
        case -1 :
        {
            w2w_send_cmd( (Cmd)(is_extended ? INPUT_EXT_PUSH_RELEASE : INPUT_PUSH_RELEASE )) ; 

            //TRACE_WW(is_extended ? "\n\r[W2W_PUSH_EXT_RELEASE]\n\r" : "\n\r[W2W_PUSH_RELEASE]\n\r");
            
            is_extended = FALSE ; 
            
            is_pressed = FALSE ; 

            time = 0 ;          
        }
        break ;
        

        default :
        {
            if( is_pressed ) 
            {
                if( ++time == 10*10*3 )//3 SECONDS
                {
                    w2w_send_cmd( (Cmd)INPUT_LONG_PUSH ) ; 

                    is_extended = TRUE ;

                    //TRACE_WW("\n\r[W2W_PUSH_EXT_PRESS]\n\r");
                }                
            }
        }
        break;
    }   
}
//---------------------------------------------------- //



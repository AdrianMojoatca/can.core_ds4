/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_init.c 33395 2015-08-26 15:17:38Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d_private.h"
#include "d2d_code.h"
#include "irq_user.h"
#include "isr.h"
#include "fiq.h"
#include "gpio.h"
#include "w2w.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void insert_d2d_exception (void) ;
static void clear_d2d_false_code (void) ;
static void d2d_enable_w2w       (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void d2d_init (void)
{	
  if(d2d_init_nvfs ())
    {
      CRITICAL ;
      
      d2d_tx_set_func      (d2d_tx_code)       ;
      d2d_tx_frame_set_func(d2d_tx_frame_data) ;
      d2d_set_get_mask_func(d2d_get_mask_func) ;
      d2d_set_get_ena_func (d2d_get_ena_func)  ; 
      
      
      d2d_irq_tx = os_irq_create () ;
      d2d_irq_rx = os_irq_create () ;

      d2d_tx_res = os_res_create () ;

      tsk_d2d    = os_tsk_create (d2d_task , TASK_D2D_PRI , TASK_D2D_STK , 0 , TASK_D2D_NAME) ;
      /// Configure UART-0  ///

     C_ENTER ;
        {
          /// NOTE : Hardcoded UART-0 at 75 Mhz clock source - 1200 N 8 1 ///
            
          PCONP   |= (1u << 3) ;     // Turn on UART0 clock
          gpio_pl_uart_cfg(GPIO_PL_D2D_TX, 0);
          PCLKSEL0 = (PCLKSEL0 & ~(3U << 6)) | (1u << 6) ;       // CPU_CLK / 1 = 72 MHz			
            
          gpio_pl_select(GPIO_PL_D2D_TX);
          gpio_pl_select(GPIO_PL_D2D_RX);
        
          isr_set_vector   (ISR_ID_UART_0 , ISR_PRI_14 , ISR_TYPE_IRQ , d2d_irq_service) ;

          wake_register      (d2d_wake);
          wake_register_ready(d2d_wake_ready);


          #ifdef DEV_LPC_176x
            gpio_pl_set(GPIO_PL_RF_D2D_EN);         //enable first to have bus available
          #endif

          insert_d2d_exception () ;
        }

      C_LEAVE ;

      
      d2d_enable_w2w       () ;
      
      clear_d2d_false_code () ;   
      
      os_tsk_start (tsk_d2d , 0) ;     
    }
  else
    {
      #if DBG_D2D == 1
        TRACE("\n[D2D NVFS ERROR]");
      #endif      
    }

}

//--------------------------------------------------------------------------//

static void insert_d2d_exception (void) 
{
    d2d_lut_insert (FN_61          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_62          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_63          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_64          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_65          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_66          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_67          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_68          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_69          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_6A          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_6B          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_6C          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_6D          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_6E          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_6F          , d2d_exception_gwr_enc) ;
    d2d_lut_insert (FN_READ_EEPROM , d2d_exception_read_eeprom) ;
    d2d_lut_insert (FN_WRITE_EEPROM, d2d_exception_write_eeprom) ;
}

//--------------------------------------------------------------------------//

static void clear_d2d_false_code (void)
{
	d2d_tx (SET_IGN_OFF) ;
	d2d_tx (SET_KEY_OUT) ;
	d2d_tx (SET_GLOWPLUG_OFF) ;
	d2d_tx (SET_HOOD_CLOSE) ;
	d2d_tx (SET_TRUNK_CLOSE) ;
	d2d_tx (SET_OTHER_CLOSE) ;
	d2d_tx (SET_DRIVER_CLOSE) ;
	d2d_tx (SET_BRAKE_OFF) ;
	d2d_tx (SET_PRK_NTRL_ACTIVE) ;
	d2d_tx (SET_HANDBRAKE_OFF) ;
}
//--------------------------------------------------------------------------//

static void d2d_enable_w2w(void)
{
  #ifndef PLATFORM_CM800
    {
      static const UInt8 d2d_code_table[] = 
      {
        FN_GWR_OFF           , FN_GWR_ON            , FN_LOCK_REM1_ON      , FN_LOCK_REM2_ON      , FN_LOCK_REM3_ON      ,    
        FN_UNLOCK_DR_REM1_ON , FN_UNLOCK_DR_REM2_ON , FN_UNLOCK_DR_REM3_ON , FN_UNLOCK_ALL_REM1_ON, FN_UNLOCK_ALL_REM2_ON,    
        FN_UNLOCK_ALL_REM3_ON, FN_TRUNK_REM1_ON     , FN_TRUNK_REM2_ON     , FN_TRUNK_REM3_ON     , FN_AUX1_REM1_ON      ,    
        FN_AUX1_REM2_ON      , FN_AUX1_REM3_ON      , FN_AUX2_REM1_ON      , FN_AUX2_REM2_ON      , FN_AUX2_REM3_ON      ,    
        FN_AUX3_REM1_ON      , FN_AUX3_REM2_ON      , FN_AUX3_REM3_ON      , FN_AUX4_REM1_ON      , FN_AUX4_REM2_ON      ,    
        FN_AUX4_REM3_ON      , 
      } ;
      
     
      UInt8 n ;
           
      for(n = 0 ; n < sizeof_array(d2d_code_table) ; n++)
        {
          if(!d2d_get_ena(d2d_code_table[n]))
            {
              w2w_ena(d2d_code_table[n]) ;
              
              #if DBG_D2D == 1
                TRACE("\nD2D Enable Wire For D2D Code: %X" , d2d_code_table[n]) ;
              #endif           
            }
//          else
//            {
//              #if DBG_D2D == 1
//                TRACE("\nD2D Skip Code: %X" , d2d_code_table[n]) ;
//              #endif        
//            }        
        }   
   }
  #endif
}

/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


#define FIO0SET			LPC_GPIO0->FIOSET
#define FIO0CLR			LPC_GPIO0->FIOCLR
#define FIO0PIN			LPC_GPIO0->FIOPIN
#define FIO0DIR			LPC_GPIO0->FIODIR

#define FIO1SET			LPC_GPIO1->FIOSET
#define FIO1CLR			LPC_GPIO1->FIOCLR
#define FIO1PIN			LPC_GPIO1->FIOPIN
#define FIO1DIR			LPC_GPIO1->FIODIR

#define FIO2SET			LPC_GPIO2->FIOSET
#define FIO2CLR			LPC_GPIO2->FIOCLR
#define FIO2PIN			LPC_GPIO2->FIOPIN
#define FIO2DIR			LPC_GPIO2->FIODIR

#define FIO3SET			LPC_GPIO3->FIOSET
#define FIO3CLR			LPC_GPIO3->FIOCLR
#define FIO3PIN			LPC_GPIO3->FIOPIN
#define FIO3DIR			LPC_GPIO3->FIODIR

#define FIO4SET			LPC_GPIO4->FIOSET
#define FIO4CLR			LPC_GPIO4->FIOCLR
#define FIO4PIN			LPC_GPIO4->FIOPIN
#define FIO4DIR			LPC_GPIO4->FIODIR

#define PINMODE0 		LPC_PINCON->PINMODE0
#define PINMODE1 		LPC_PINCON->PINMODE1
#define PINMODE2 		LPC_PINCON->PINMODE2
#define PINMODE3 		LPC_PINCON->PINMODE3
#define PINMODE4 		LPC_PINCON->PINMODE4
#define PINMODE5 		LPC_PINCON->PINMODE5
#define PINMODE6 		LPC_PINCON->PINMODE6
#define PINMODE7 		LPC_PINCON->PINMODE7
#define PINMODE8 		LPC_PINCON->PINMODE8
#define PINMODE9 		LPC_PINCON->PINMODE9

#define PINSEL0			LPC_PINCON->PINSEL0
#define PINSEL1			LPC_PINCON->PINSEL1
#define PINSEL2			LPC_PINCON->PINSEL2
#define PINSEL3			LPC_PINCON->PINSEL3
#define PINSEL4			LPC_PINCON->PINSEL4
#define PINSEL5			LPC_PINCON->PINSEL5
#define PINSEL6			LPC_PINCON->PINSEL6
#define PINSEL7			LPC_PINCON->PINSEL7
#define PINSEL8			LPC_PINCON->PINSEL8
#define PINSEL9			LPC_PINCON->PINSEL9
#define PINSEL10		LPC_PINCON->PINSEL10

#define PCLKSEL0		LPC_SC->PCLKSEL0
#define PCLKSEL1		LPC_SC->PCLKSEL1
#define PCONP				LPC_SC->PCONP
#define PCON				LPC_SC->PCON
#define PLLCON			LPC_SC->PLL0CON
#define PLLFEED			LPC_SC->PLL0FEED
#define PLLSTAT			LPC_SC->PLL0STAT
#define PLLCFG			LPC_SC->PLL0CFG
#define CCLKCFG			LPC_SC->CCLKCFG
#define CLKSRCSEL		LPC_SC->CLKSRCSEL
#define SCS					LPC_SC->SCS
#define RSIR        LPC_SC->RSID
#define USBCLKCFG		LPC_SC->USBCLKCFG
#define EXTINT			LPC_SC->EXTINT
#define EXTMODE			LPC_SC->EXTMODE
#define EXTPOLAR		LPC_SC->EXTPOLAR

#define IO0_INT_CLR			LPC_GPIOINT->IO0IntClr
#define IO0_INT_EN_F		LPC_GPIOINT->IO0IntEnF
#define IO0_INT_EN_R		LPC_GPIOINT->IO0IntEnR
#define IO0_INT_STAT_F	LPC_GPIOINT->IO0IntStatF
#define IO0_INT_STAT_R	LPC_GPIOINT->IO0IntStatR

#define IO2_INT_CLR			LPC_GPIOINT->IO2IntClr
#define IO2_INT_EN_F		LPC_GPIOINT->IO2IntEnF
#define IO2_INT_EN_R		LPC_GPIOINT->IO2IntEnR
#define IO2_INT_STAT_F	LPC_GPIOINT->IO2IntStatF
#define IO2_INT_STAT_R	LPC_GPIOINT->IO2IntStatR

#define PWM1MR0			LPC_PWM1->MR0
#define PWM1MR2			LPC_PWM1->MR2
#define PWM1MR3			LPC_PWM1->MR3
#define PWM1LER			LPC_PWM1->LER
#define PWM1PR			LPC_PWM1->PR
#define PWM1MCR			LPC_PWM1->MCR
#define PWM1PCR			LPC_PWM1->PCR
#define PWM1TCR			LPC_PWM1->TCR


#define TMR0_BASE_ADDR	LPC_TIM0_BASE
#define T0TC						LPC_TIM0->TC
#define T0MR0						LPC_TIM0->MR0
#define T0MCR						LPC_TIM0->MCR
#define T0TCR						LPC_TIM0->TCR
#define T0IR						LPC_TIM0->IR
#define T0PR						LPC_TIM0->PR
#define T0PC						LPC_TIM0->PC

#define TMR1_BASE_ADDR	LPC_TIM1_BASE
#define T1TC						LPC_TIM1->TC
#define T1MR0						LPC_TIM1->MR0
#define T1MCR						LPC_TIM1->MCR
#define T1TCR						LPC_TIM1->TCR
#define T1IR						LPC_TIM1->IR
#define T1PR						LPC_TIM1->PR
#define T1PC						LPC_TIM1->PC

#define TMR2_BASE_ADDR	LPC_TIM2_BASE
#define T2TC						LPC_TIM2->TC
#define T2MR0						LPC_TIM2->MR0
#define T2MCR						LPC_TIM2->MCR
#define T2TCR						LPC_TIM2->TCR
#define T2IR						LPC_TIM2->IR
#define T2PR						LPC_TIM2->PR
#define T2PC						LPC_TIM2->PC

#define TMR3_BASE_ADDR	LPC_TIM3_BASE
#define T3TC						LPC_TIM3->TC
#define T3MR0						LPC_TIM3->MR0
#define T3MCR						LPC_TIM3->MCR
#define T3TCR						LPC_TIM3->TCR
#define T3IR						LPC_TIM3->IR
#define T3PR						LPC_TIM3->PR
#define T3PR						LPC_TIM3->PR

#define WDMOD				LPC_WDT->WDMOD
#define WDFEED			LPC_WDT->WDFEED
#define WDCLKSEL		LPC_WDT->WDCLKSEL
#define WDTC				LPC_WDT->WDTC

#define AD0CR				LPC_ADC->ADCR
#define AD0STAT			LPC_ADC->ADSTAT
#define AD0DR0			LPC_ADC->ADDR0
#define AD0DR1			LPC_ADC->ADDR1
#define AD0DR2			LPC_ADC->ADDR2
#define AD0INTEN		LPC_ADC->ADINTEN
#define AD0CR				LPC_ADC->ADCR
#define AD0GDR			LPC_ADC->ADGDR

#define DACR			LPC_DAC->DACR

#define U0LCR			LPC_UART0->LCR
#define U0DLL			LPC_UART0->DLL
#define U0DLM			LPC_UART0->DLM
#define U0FCR			LPC_UART0->FCR
#define U0IIR			LPC_UART0->IIR
#define U0IER			LPC_UART0->IER
#define U0RBR			LPC_UART0->RBR
#define U0LSR			LPC_UART0->LSR
#define U0THR			LPC_UART0->THR
#define U0SCR			LPC_UART0->SCR
#define U0ACR			LPC_UART0->ACR
#define U0ICR			LPC_UART0->ICR
#define U0FDR			LPC_UART0->FDR
#define U0TER			LPC_UART0->TER

#define U1LCR			LPC_UART1->LCR
#define U1DLL			LPC_UART1->DLL
#define U1DLM			LPC_UART1->DLM
#define U1FCR			LPC_UART1->FCR
#define U1IIR			LPC_UART1->IIR
#define U1IER			LPC_UART1->IER
#define U1RBR			LPC_UART1->RBR
#define U1LSR			LPC_UART1->LSR
#define U1THR			LPC_UART1->THR
#define U1MCR			LPC_UART1->MCR
#define U1MSR			LPC_UART1->MSR
#define U1SCR			LPC_UART1->SCR
#define U1ACR			LPC_UART1->ACR
#define U1FDR			LPC_UART1->FDR
#define U1TER			LPC_UART1->TER
#define U1RS485CTRL		LPC_UART1->RS485CTRL
#define U1ADRMATCH		LPC_UART1->ADRMATCH
#define U1RS485DLY		LPC_UART1->RS485DLY


#define U2LCR			LPC_UART2->LCR
#define U2DLL			LPC_UART2->DLL
#define U2DLM			LPC_UART2->DLM
#define U2FCR			LPC_UART2->FCR
#define U2IIR			LPC_UART2->IIR
#define U2IER			LPC_UART2->IER
#define U2RBR			LPC_UART2->RBR
#define U2LSR			LPC_UART2->LSR
#define U2THR			LPC_UART2->THR
#define U2SCR			LPC_UART2->SCR
#define U2ACR			LPC_UART2->ACR
#define U2ICR			LPC_UART2->ICR
#define U2FDR			LPC_UART2->FDR
#define U2TER			LPC_UART2->TER


#define U3LCR			LPC_UART3->LCR
#define U3DLL			LPC_UART3->DLL
#define U3DLM			LPC_UART3->DLM
#define U3FCR			LPC_UART3->FCR
#define U3IIR			LPC_UART3->IIR
#define U3IER			LPC_UART3->IER
#define U3RBR			LPC_UART3->RBR
#define U3LSR			LPC_UART3->LSR
#define U3THR			LPC_UART3->THR
#define U3SCR			LPC_UART3->SCR
#define U3ACR			LPC_UART3->ACR
#define U3ICR			LPC_UART3->ICR
#define U3FDR			LPC_UART3->FDR
#define U3TER			LPC_UART3->TER


#define UART0_BASE_ADDR			LPC_UART0_BASE
#define UART1_BASE_ADDR			LPC_UART1_BASE
#define UART2_BASE_ADDR			LPC_UART2_BASE
#define UART3_BASE_ADDR			LPC_UART3_BASE

#define CAN_AFMR					LPC_CANAF->AFMR
#define CAN_EOT						LPC_CANAF->ENDofTable
#define CAN_SFF_SA				LPC_CANAF->SFF_sa
#define CAN_SFF_GRP_SA		LPC_CANAF->SFF_GRP_sa
#define CAN_EFF_SA				LPC_CANAF->EFF_sa
#define CAN_EFF_GRP_SA		LPC_CANAF->EFF_GRP_sa
#define CAN1_BASE_ADDR		LPC_CAN1_BASE
#define CAN2_BASE_ADDR		LPC_CAN2_BASE

#define CPSR							LPC_SSP0->CPSR

#define RTC_CCR 					LPC_RTC->CCR
#define RTC_ILR 					LPC_RTC->ILR
#define RTC_CIIR					LPC_RTC->CIIR


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/



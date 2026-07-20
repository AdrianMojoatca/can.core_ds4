#define OUT_LED_BLUE(x)
#define OUT_STB_HS(x)                   (x ? (gpio_pl_set(GPIO_PL_STB_HSCAN)) : (gpio_pl_clr(GPIO_PL_STB_HSCAN)))		
#define OUT_EN_FT(x)                    (x ? (gpio_pl_set(GPIO_PL_EN_FTCAN)) : (gpio_pl_clr(GPIO_PL_EN_FTCAN)))		
#define OUT_STB_FT(x)                   (x ? (gpio_pl_set(GPIO_PL_STB_FTCAN)) : (gpio_pl_clr(GPIO_PL_STB_FTCAN)))		

#define OUT_MODE0_SW(x)                 (x ? (gpio_pl_set(GPIO_PL_MODE_0)) : (gpio_pl_clr(GPIO_PL_MODE_0)))		
#define OUT_MODE1_SW(x)                 (x ? (gpio_pl_set(GPIO_PL_MODE_1)) : (gpio_pl_clr(GPIO_PL_MODE_1)))		

#define OUT_1(x)       			(x ? (gpio_pl_set(GPIO_PL_NEG_OUT1)) : (gpio_pl_clr(GPIO_PL_NEG_OUT1)))
#define OUT_2(x)       			(x ? (gpio_pl_set(GPIO_PL_NEG_OUT2)) : (gpio_pl_clr(GPIO_PL_NEG_OUT2)))
#define OUT_3(x)       			(x ? (gpio_pl_set(GPIO_PL_NEG_OUT3)) : (gpio_pl_clr(GPIO_PL_NEG_OUT3)))
#define OUT_4(x)       			(x ? (gpio_pl_set(GPIO_PL_NEG_OUT4)) : (gpio_pl_clr(GPIO_PL_NEG_OUT4)))
#define OUT_5(x)       			(x ? (gpio_pl_set(GPIO_PL_NEG_OUT5)) : (gpio_pl_clr(GPIO_PL_NEG_OUT5)))
#define OUT_1_PLUS(x)                   (x ? (gpio_pl_set(GPIO_PL_POS_OUT1)) : (gpio_pl_clr(GPIO_PL_POS_OUT1)))
#define OUT_2_PLUS(x)                   (x ? (gpio_pl_set(GPIO_PL_POS_OUT2)) : (gpio_pl_clr(GPIO_PL_POS_OUT2)))
#define OUT_RELAY1(x)  			(x ? (gpio_pl_set(GPIO_PL_RLY1_CTL)) : (gpio_pl_clr(GPIO_PL_RLY1_CTL)))
#define OUT_RELAY2(x)  			(x ? (gpio_pl_set(GPIO_PL_RLY2_CTL)) : (gpio_pl_clr(GPIO_PL_RLY2_CTL)))

#define OUT_DAC_RELAY(x)                (x ? (gpio_pl_set(GPIO_PL_DAC_PWR_CTL)) : (gpio_pl_clr(GPIO_PL_DAC_PWR_CTL)))	// ADI M - vers. 1_00	

//PHAL1---------------------------------------------------------------------//
#define IN_PHAL1_RX                     gpio_pl_pin_read(GPIO_PL_ISO1_RX)		
#define OUT_PHAL1_TX(x)                 (x ? (gpio_pl_set(GPIO_PL_ISO1_TX)) : (gpio_pl_clr(GPIO_PL_ISO1_TX)))		
#define OUT_PHAL1_12(x)                 (x ? (gpio_pl_set(GPIO_PL_PULL_12V_ISO1)) : (gpio_pl_clr(GPIO_PL_PULL_12V_ISO1)))		
#define OUT_PHAL1_5(x)                  (x ? (gpio_pl_set(GPIO_PL_PULL_5V_ISO1)) : (gpio_pl_clr(GPIO_PL_PULL_5V_ISO1)))		
//--------------------------------------------------------------------------//


//PHAL1---------------------------------------------------------------------//
#define IN_PHAL2_RX                     gpio_pl_pin_read(GPIO_PL_ISO2_RX)		
#define OUT_PHAL2_TX(x)                 (x ? (gpio_pl_set(GPIO_PL_ISO2_TX)) : (gpio_pl_clr(GPIO_PL_ISO1_TX)))		
#define OUT_PHAL2_12(x)                 (x ? (gpio_pl_set(GPIO_PL_PULL_12V_ISO2)) : (gpio_pl_clr(GPIO_PL_PULL_12V_ISO2)))		
#define OUT_PHAL2_5(x)                  (x ? (gpio_pl_set(GPIO_PL_PULL_5V_ISO2)) : (gpio_pl_clr(GPIO_PL_PULL_5V_ISO2)))		
//--------------------------------------------------------------------------//

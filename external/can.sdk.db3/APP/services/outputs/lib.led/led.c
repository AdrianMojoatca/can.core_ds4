// .\core\lib.led\led.c

// header files

// source files
#include "led_atomic_release.c"
#include "led_atomic_request.c"
#include "led_ctrl_green.c"
#include "led_ctrl_red.c"
#include "led_ctrl_yellow.c"
#include "led_execute.c"
#include "led_flash.c"
#include "led_init.c"
#include "led_reset.c"
#include "led_vars.c"

#ifdef PLATFORM_933
	#include "led_ctrl_blue.c"
	#include "led_ctrl_all.c"
#endif



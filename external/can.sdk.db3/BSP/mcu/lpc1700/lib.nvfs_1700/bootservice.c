// bootservice.c

#include "device.h"
#include "bootservice.h"


typedef UInt16 (*pbootcall_t)(UInt32 *,UInt32 *);

#define BOOTSVC_VECTOR ((pbootcall_t *)0x20)

volatile UInt32 never;
UInt16 BootService(UInt32 *inptr,UInt32 *outptr)
{
	UInt16 sts;
	//CRITICAL_ALL;
	//C_ENTER_ALL;
	sts = (**BOOTSVC_VECTOR)(inptr,outptr);
  //return (**BOOTSVC_VECTOR)(inptr,outptr);
	never = sts;
	//C_LEAVE_ALL;
	
	never =  __get_PRIMASK ();
	never =  __get_BASEPRI ();
	return sts;
}

void BootReset(UInt8 port_no)
{
UInt32 arg[1];
  (void)port_no;
  arg[0] = BootService_XKL_0 | (0 << 16);  //ignore port number

  BootService(arg,NULL);
}




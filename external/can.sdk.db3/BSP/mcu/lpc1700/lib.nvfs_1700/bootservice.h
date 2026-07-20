
#ifndef BOOTSERVICE_H
#define BOOTSERVICE_H


// list of available boot services
enum BootServiceID
{		
	BootService_XKL_0, // xkloader @ 38400 on uart0 (below dash port)
	BootService_XKL_1, // xkloader @ 38400 on uart1 (above dash port)
	BootService_Boot0, // nxp bootloader
	
	BootService_bfs_init,
	BootService_bfs_commit,
	
	BootService_bfs_read_at,
	BootService_bfs_getinfo,
	BootService_bfs_getnext,
	
	BootService_bfs_write_at,
	BootService_bfs_putattr,
	BootService_bfs_delete,
	BootService_bfs_create

};

// input:
// bottom 16 bits of inptr[0] contains which BootServiceID
// upper 16 bits of inptr[0] is the number of additional U32 arguments passed in inptr[1],inptr[2],...
// if outptr is not null:
// upper 16 bits of outptr[0] will be number of additional U32 output results expected

// output:
// bottom 16 bits of outptr[0] is identical to returned result
// uppder 16 bits of outptr[0] is number of additional U32 output results returned or available to return
// (if this is larger than results expected then extra values were dropped)

UInt16 BootService( UInt32 *inptr, UInt32 *outptr);
#define BOOTSERVICE_ERROR ((UInt16)0xFFFF) // mismatch on input argument count, or unrecognized function
void BootReset(UInt8 port_no);

#endif // BOOTSERVICE_H



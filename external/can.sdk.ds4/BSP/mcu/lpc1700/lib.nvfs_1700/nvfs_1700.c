// nvfs_1700.c 

#include "device.h"
#include "nvfs_usr.h"
#include "bfs.h"
#include "nvfs.h"
#include "bootservice.h"
#include "nvram.h      "
#include "os.h"
#include "wdog_v.h"
#include "wake.h"


Res	nvfs_res_access;


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

//type: 0=reset; 1=maint
void BootReset(UInt8 type)
{
UInt32 arg[1];
  if(type)
  {
    arg[0] = BootService_XKL_0 | (0 << 16);  //ignore port number
    BootService(arg,NULL);
  }
  else
    wdog_v_force_reset(WDOG_V_RESET_FIRMWARE,(char*)__func__,__LINE__);
}





////////////////////// bootloader APIs which do not change the filesystem ///////////////////////////////////


BFS_Status bfs_getinfo(const char *name, BFS_Info *pinfo)
{
UInt32 arg[3];

  arg[0] = BootService_bfs_getinfo | (2 << 16);
  arg[1] = (UInt32)name;
  arg[2] = (UInt32)pinfo;

  return (BFS_Status)BootService(arg,NULL);

}


UInt16 bfs_read_at(const char *name, UInt16 ofs, void *buf, UInt16 bufsize)
{
UInt32 arg[5];

  arg[0] = BootService_bfs_read_at | (4 << 16);
  arg[1] = (UInt32)name;
  arg[2] = ofs;
  arg[3] = (UInt32)buf;
  arg[4] = bufsize;


  return (UInt16)BootService(arg,NULL);
}


// returns BFS_ERROR when no more files
BFS_Status bfs_getnext(BFS_Iter *it)
{
UInt32 arg[2];

  arg[0] = BootService_bfs_getnext| (1 << 16);
  arg[1] = (UInt32)it;


  return (BFS_Status)BootService(arg,NULL);
}




//////////////////////// bootloader APIs which make changes to the filesystem /////////////////

UInt16 bfs_write_at(const char *name, UInt16 ofs, const void *buf, UInt16 bufsize)
{
UInt32 arg[5];
UInt16 result;
	
  arg[0] = BootService_bfs_write_at | (4 << 16);
  arg[1] = (UInt32)name;
  arg[2] = ofs;
  arg[3] = (UInt32)buf;
  arg[4] = bufsize;


  result = (UInt16)BootService(arg,NULL);
	wake_sleep_soon();
	return result;
}


// change 4-bit attribute nibble
BFS_Status bfs_putattr(const char *name, UInt8 attr)
{
UInt32 arg[3];

  arg[0] = BootService_bfs_putattr | (2 << 16);
  arg[1] = (UInt32)name;
  arg[2] = attr;

  return (BFS_Status)BootService(arg,NULL);
}



BFS_Status bfs_delete(const char *name)
{
UInt32 arg[2];

  arg[0] = BootService_bfs_delete | (1 << 16);
  arg[1] = (UInt32)name;

  return (BFS_Status)BootService(arg,NULL);
}

// create a new entry (must not exist)  file attributes are initially zero (not write-protected)
BFS_Status bfs_create(const char *name, const void *buf, UInt16 size)
{
UInt32 arg[4];

  arg[0] = BootService_bfs_create | (3 << 16);
  arg[1] = (UInt32)name;
  arg[2] = (UInt32)buf;
  arg[3] = size;


  return (BFS_Status)BootService(arg,NULL);
}

BFS_Status bfs_commit(void)
{
UInt32 arg[1];

  arg[0] = BootService_bfs_commit | (0 << 16);

  return (BFS_Status)BootService(arg,NULL);
}


BFS_Status bfs_init(UInt8 flags)
{
UInt32 arg[2];

  arg[0] = BootService_bfs_init | (1 << 16);
  arg[1] = flags;

  return (BFS_Status)BootService(arg,NULL);
}






//////////////////////// nvfs legacy APIs in application under RTOS //////////////////////////////

void nvfs_res_init(void)
{
	  nvfs_res_access = os_res_create () ;
}

void nvfs_enter(void)
{
	while(os_res_request(nvfs_res_access , 0)==0)
	{
		;// do somthing like play chdexx
	}
}
static void nvfs_leave(void)
{
	os_res_release(nvfs_res_access) ;
}


NVFS_Status nvfs_init(UInt8 flags)
{
NVFS_Status sts;
	nvfs_res_init();
	
	nvfs_enter();
#if 0	
	sts = bfs_init(flags); // normal production read/write mount, see mount flags
#endif
  sts = NVFS_OK;	
	nvfs_leave();
	return sts;
}

NVFS_Status nvfs_delete(const char *name)
{
NVFS_Status sts;

	nvfs_enter();
	sts = bfs_delete(name);
	nvfs_leave();
	return sts;
}

NVFS_Status nvfs_create(const char *name,UInt16 size)
{
NVFS_Status sts;

	nvfs_enter();
	sts = bfs_create(name,NULL,size);
	nvfs_leave();
	return sts;
}



NVFS_Status nvfs_rd_size(const char *name, UInt16 *psize)
{
BFS_Info info;
NVFS_Status sts;

	nvfs_enter();
	sts = bfs_getinfo(name, &info);
	*psize = info.size;
	nvfs_leave();
	return sts;
}

// warning: assumed size for buf!
NVFS_Status nvfs_rd(const char *name, void *buf) // deprecated API. use nvfs_rd_s() instead.
{
NVFS_Status sts;
BFS_Info info;

	nvfs_enter();
	sts = bfs_getinfo(name, &info);
	if (sts == NVFS_OK)
	{
		if (bfs_read_at(name, 0, buf, info.size) != info.size)
			sts = NVFS_ERROR;
	}
	nvfs_leave();
	return sts;
}

NVFS_Status nvfs_rd_s(const char *name, void *buf,UInt16 size) // new API to fix issue NCC-DIRE002-019
{
NVFS_Status sts;
BFS_Info info;

	nvfs_enter();
	sts = bfs_getinfo(name, &info);
	if (sts == NVFS_OK)
	{
		if(size > info.size)
      size = info.size; // truncate
		
		if (bfs_read_at(name, 0, buf, size) != size)
			sts = NVFS_ERROR;
	}
	nvfs_leave();
	return sts;
}


UInt16 nvfs_rd_at(const char *name, UInt16 pos, void *buf, UInt16 size)
{
UInt16 result;
	
	nvfs_enter();
	result = bfs_read_at(name, pos, buf, size);
	nvfs_leave();
	return result;
}

UInt16 nvfs_wr_at(const char *name, UInt16 pos, void *buf, UInt16 size)
{
UInt16 result;
	
	nvfs_enter();
	result = bfs_write_at(name, pos, buf, size);
	nvfs_leave();
	return result;
}

NVFS_Status nvfs_wr(const char *name, void *buf)
{
NVFS_Status sts;
BFS_Info info;

	nvfs_enter();
	sts = bfs_getinfo(name, &info);
	if (sts == NVFS_OK)
	{
		if (info.attr & NVFS_ATTRIB_WR_PROTECTED)
			sts = NVFS_WR_PROTECTED;
		else
		{
			if (bfs_write_at(name, 0, buf, info.size) != info.size)
				sts = NVFS_ERROR;
		}
	}
	nvfs_leave();
	return sts;
}

NVFS_Status nvfs_wr_create(const char *name, const void *buf, UInt16 size)
{
NVFS_Status sts;
BFS_Info info;
	
	nvfs_enter();
	sts = bfs_getinfo(name, &info);
	if (sts == NVFS_NAME_NOT_FOUND)
	{
		sts = bfs_create(name, buf, size);
	}
	else
	{
		sts = NVFS_ERROR; // assume failure
		if (info.attr & NVFS_ATTRIB_WR_PROTECTED)
			sts = NVFS_WR_PROTECTED;
		else if (size == info.size && bfs_write_at(name, 0, buf, size) == size)
			sts = NVFS_OK;
	}
	nvfs_leave();
	return sts;
}

NVFS_Status nvfs_wr_delete(const char *name, void *buf, UInt16 size)
{
NVFS_Status sts;
BFS_Info info;

	nvfs_enter();

	// try to save time by overwriting existing data
	if  (  bfs_getinfo(name, &info) == NVFS_OK
		&& info.size == size
		&& !(info.attr & NVFS_ATTRIB_WR_PROTECTED)
		&& bfs_write_at(name, 0, buf, size) == size
		)
	{
		sts = NVFS_OK;
	}
	else
	{// incompatible.  just delete and start over.
		sts = bfs_delete(name);

		if (sts != NVFS_WR_PROTECTED)
			sts = bfs_create(name, buf, size);
	}
	nvfs_leave();
	return sts;
}


// bfs doesn't have a 'get_attributes' api, so we create our own
static NVFS_Status _get_attr(const char *name, UInt8 *pattributes)
{
NVFS_Status sts;
BFS_Info info;
	
	sts = bfs_getinfo(name,&info);	
	*pattributes = (sts == BFS_OK) ? info.attr : 0;
	return sts;
}

// get all attributes
NVFS_Status nvfs_get_attributes(const char *name,UInt8 *pattributes)
{
NVFS_Status sts;

	nvfs_enter();
	sts = _get_attr(name,pattributes);
	nvfs_leave();

	return sts;
}

// replace all attributes with this new value (are you really sure?) see nvfs_bit_set_attributes below
NVFS_Status nvfs_set_attributes(const char *name,UInt8 attributes)
{
NVFS_Status sts;
	
	nvfs_enter();
	sts = bfs_putattr(name,attributes);
	nvfs_leave();
	return sts;
}

// set these attributes
NVFS_Status nvfs_bit_set_attributes(const char *name,UInt8 attribute_bits)
{
NVFS_Status sts;
UInt8 attr;
	
	nvfs_enter();
	sts = _get_attr(name,&attr);
	if(sts == NVFS_OK)
	{
	  attr |= attribute_bits;
		sts = bfs_putattr(name,attr);
	}
	nvfs_leave();
	return sts;
}

// clear these attributes
NVFS_Status nvfs_bit_clr_attributes(const char *name,UInt8 attribute_bits)
{
NVFS_Status sts;
UInt8 attr;
	
	nvfs_enter();
	sts = _get_attr(name,&attr);
	if(sts == NVFS_OK)
	{
	  attr &= ~attribute_bits;
		sts = bfs_putattr(name,attr);
	}
	nvfs_leave();
	return sts;
}


Boolean nvram_cache_commit(void)
{
	nvfs_enter();
	bfs_commit();
	nvfs_leave();
	return (1==1); // hard-coded true value
}




Boolean nvram_usr_commit(void)
{
return (nvram_cache_commit()); 
}



void nvram_usr_sleep_commit(Wake_Status wake_status)
{
	if (wake_status == s_SLEEP)
	{
		nvram_cache_commit(); 
	}
}


extern volatile UInt32 never;
volatile UInt32 never1;
volatile UInt32 stat_test;
volatile NVFS_Entry_Name entry_name_test;
NVFS_Status nvfs_size_is_valid(NVFS_Entry_Name const entry_name , UInt16 entry_size)
{
  UInt16 rd_size ;

  NVFS_Status status = NVFS_OK;

	entry_name_test = entry_name;
  status = nvfs_rd_size (entry_name , &rd_size) ;

		never = rd_size;
		never1 = entry_size;
	stat_test = status;
  if(status == NVFS_OK)
    {
      if(rd_size != entry_size)
        {
          status  = NVFS_ERROR ;
        } 
    }
		
	stat_test = status;

  return status ;
}


NVFS_Status nvfs_iter_next(NVFS_Iterator *iter)
{
	UInt32 status ;
  nvfs_enter();
	
  status = bfs_getnext(&bfsiter);
	if(status == NVFS_OK)
	{
		iter->is_valid = 1;
		strcpy(iter->name_buff,bfsiter.name);
	}
	else
	  iter->is_valid = 0;
	
  nvfs_leave();	
return (NVFS_Status)status;
}



NVFS_Status nvfs_iter_init(NVFS_Iterator *iter,char *name, unsigned size)
{
	iter->name_buff = name;
	iter->name_buff_size = size;
	iter->is_truncated = 0;
  memset(&bfsiter,0xFF,sizeof(bfsiter));
	return nvfs_iter_next(iter);
}


Nvram_Status nvram_init (Boolean do_format)
{
	return ((Nvram_Status) TRUE);
}

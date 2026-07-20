
#ifndef NVFS_H
#define NVFS_H

#include "device.h"
//#include "types.h"
#include "string.h"
#include "bfs.h"
#include "wake.h"

#if 0
typedef enum
{
	NVFS_OK = 0,  // !!! MUST BE ZERO !!!

	NVFS_ERROR,   // must be one!

	NVFS_WR_PROTECTED,

	NVFS_NAME_NOT_FOUND,

	NVFS_NAME_DUPLICATE,

	NVFS_FORMAT_UNKNOWN,

} NVFS_Status;
#else

typedef enum
{
  NVRAM_OK                ,
  NVRAM_OK_RECOVERED      ,

  NVRAM_ERR_RANGE         ,

  NVRAM_ERR_WR_FAIL       ,

  NVRAM_ERR_CORRUPTED     ,

  NVRAM_ERR_INTERNAL      ,

  NVRAM_ERR_UNINITIALIZED ,

} Nvram_Status ;


#define NVFS_OK             BFS_OK
#define NVFS_ERROR          BFS_ERROR
#define NVFS_WR_PROTECTED   BFS_WR_PROTECTED
#define NVFS_NAME_NOT_FOUND BFS_NAME_NOT_FOUND
#define NVFS_NAME_DUPLICATE BFS_NAME_DUPLICATE
#define NVFS_FORMAT_UNKNOWN BFS_FORMAT_UNKNOWN

typedef BFS_Status NVFS_Status;

#endif

// attribute bits (there can only be 4 of them!)
#define	NVFS_ATTRIB_WR_PROTECTED (1U << 0)
#define	NVFS_ATTRIB_HIDDEN (1U << 3)


// legacy typedefs
typedef char NVFS_Char;
typedef char *NVFS_Entry_Name;
typedef UInt8 NVFS_Attributes;

static BFS_Iter bfsiter;


typedef struct
{
 // public  :

    Boolean is_valid     ;
    Boolean is_truncated ;

 // private :

    NVFS_Char * name_buff       ;
    UInt16      name_buff_size  ;


} NVFS_Iterator ;

//////////////////////// nvfs legacy APIs in application under RTOS //////////////////////////////

// mount flags (same as bfs.h)
#define NVFS_MOUNT_RDONLY   (1<<0)
#define NVFS_MOUNT_DEBUG    (1<<1)
#define NVFS_MOUNT_NOCOMMIT (1<<2)
#define NVFS_MOUNT_DEFAULT		0

NVFS_Status nvfs_init(UInt8 flags);

NVFS_Status nvfs_delete(const char *name);

NVFS_Status nvfs_create(const char *name,UInt16 size);
NVFS_Status nvfs_rd_size(const char *name, UInt16 *psize);

UInt16 nvfs_rd_at(const char *name,UInt16 pos,void *buf,UInt16 size);
UInt16 nvfs_wr_at(const char *name,UInt16 pos,void *buf,UInt16 size);

// deprecated API.  use nvfs_rd_s() instead!
NVFS_Status nvfs_rd(const char *name , void *ram_dest);
// replacement API for nvfs_rd() to fix issue NCC-DIRE002-019
NVFS_Status nvfs_rd_s(const char *name , void *ram_dest, UInt16 ram_size);


NVFS_Status nvfs_wr(const char *name, void *buf);

NVFS_Status nvfs_wr_create(const char *name, const void *buf, UInt16 size);

NVFS_Status nvfs_wr_delete(const char *name, void *buf, UInt16 size);

NVFS_Status nvfs_get_attributes(const char *name,UInt8 *attributes);
NVFS_Status nvfs_set_attributes(const char *name,UInt8 attributes);

NVFS_Status nvfs_bit_set_attributes(const char *name,UInt8 attribute_bits);
NVFS_Status nvfs_bit_clr_attributes(const char *name,UInt8 attribute_bits);

NVFS_Status nvfs_iter_init(NVFS_Iterator *iter,char *name, unsigned size);
NVFS_Status nvfs_iter_next(NVFS_Iterator *iter);

Boolean nvram_cache_commit(void);
Boolean nvram_cache_commit_sync(void);
Boolean nvram_usr_commit(void);
void nvram_usr_sleep_commit(Wake_Status wake_status);
NVFS_Status nvfs_size_is_valid(NVFS_Entry_Name const entry_name , UInt16 entry_size);
Nvram_Status nvram_init (Boolean do_format);


#endif

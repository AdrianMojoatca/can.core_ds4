
#ifndef BFS_H
#define BFS_H

//#include "types.h"

typedef UInt16 BF_OFS;


// same return codes as nvfs.h
typedef enum
{
	BFS_OK = 0,  // !!! MUST BE ZERO !!!

	BFS_ERROR,   // must be one!

	BFS_WR_PROTECTED,

	BFS_NAME_NOT_FOUND,

	BFS_NAME_DUPLICATE,

	BFS_FORMAT_UNKNOWN,

} BFS_Status;

// attribute bits (there can only be 4 of them!)
#define	BFS_ATTRIB_WR_PROTECTED (1U << 0)
// unassigned                   (1U << 1)
// unassigned                   (1U << 2)
#define BFS_ATTRIB_HIDDEN       (1U << 3)



/////////////////// bfs variables and structures ////////////

typedef struct
{
	BF_OFS rec;  // location offset (for reference, does not persist)
	UInt8 attr;   // U4 attribute
	UInt16 size;  // user data size
	UInt16 block; // total nvram occupied including padding 

} BFS_Info;


#define BFS_NAMESIZE 31 // how much of name is significant

typedef struct // initialize this struct with FFs before use.
{
	BFS_Info info;
	UInt8 files;
	UInt8 errors;
	UInt16 data;
	UInt16 used;
	char name[BFS_NAMESIZE + 1];
} BFS_Iter;

typedef struct
{
	UInt16 sequencenum;
	UInt16 size;
	UInt8 grain;
} BFS_VolInfo;



////////////////////// bootloader APIs which do not change the filesystem ///////////////////////////////////


UInt16 bfs_read_at(const char *name, UInt16 ofs, void *buf, UInt16 bufsize);
BFS_Status bfs_getinfo(const char *name, BFS_Info *pinfo);
BFS_Status bfs_getnext(BFS_Iter *it); // returns BFS_ERROR when no more files



//////////////////////// bootloader APIs which make changes to the filesystem /////////////////

UInt16 bfs_write_at(const char *name, UInt16 ofs, const void *buf, UInt16 bufsize);
BFS_Status bfs_putattr(const char *name, UInt8 attr); // change 4-bit attribute nibble
BFS_Status bfs_delete(const char *name);

// create a new entry (must not exist)  file attributes are initially zero (not write-protected)
BFS_Status bfs_create(const char *name, const void *buf, UInt16 size);
BFS_Status bfs_commit(void);




BFS_Status bfs_init(UInt8 flags);

// mount_flags
#define BFS_MOUNT_RDONLY   (1<<0)
#define BFS_MOUNT_DEBUG    (1<<1)
#define BFS_MOUNT_NOCOMMIT (1<<2)


#endif


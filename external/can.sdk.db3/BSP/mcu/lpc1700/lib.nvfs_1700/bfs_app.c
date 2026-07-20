// bfs.c : Defines the entry point for the console application.
//
#include "device.h"

#include "bfs.h"
#include "bootservice.h"


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

  arg[0] = BootService_bfs_write_at | (4 << 16);
  arg[1] = (UInt32)name;
  arg[2] = ofs;
  arg[3] = (UInt32)buf;
  arg[4] = bufsize;


  return (UInt16)BootService(arg,NULL);
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


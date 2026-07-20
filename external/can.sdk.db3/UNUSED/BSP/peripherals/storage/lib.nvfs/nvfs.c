/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs.h"
#include "nvfs.atomic_init.c        "
#include "nvfs.atomic_release.c     "
#include "nvfs.atomic_request.c     "
#include "nvfs.bit_clr_attributes.c "
#include "nvfs.bit_set_attributes.c "
#include "nvfs.crc_calc.c           "
#include "nvfs.crc_calc_entry.c     "
#include "nvfs.crc_calc_entry_link.c"
#include "nvfs.crc_calc_header.c    "
#include "nvfs.crc_test_entry.c     "
#include "nvfs.crc_test_entry_link.c"
#include "nvfs.crc_test_header.c    "
#include "nvfs.create.c             "
#include "nvfs.delete.c             "
#include "nvfs.find_entry.c         "
#include "nvfs.get_attributes.c     "
#include "nvfs.init.c               "
#include "nvfs.init_existing.c      "
#include "nvfs.init_format.c        "
#include "nvfs.iter_init.c          "
#include "nvfs.iter_next.c          "
#include "nvfs.range_check.c        "
#include "nvfs.rd.c                 "
#include "nvfs.rd_entry.c           "
#include "nvfs.rd_header.c          "
#include "nvfs.rd_nvram.c           "
#include "nvfs.rd_size.c            "
#include "nvfs.set_attributes.c     "
#include "nvfs.throw.c              "
#include "nvfs.vars.c               "
#include "nvfs.wr.c                 "
#include "nvfs.wr_create.c          "
#include "nvfs.wr_delete.c          "
#include "nvfs.wr_entry.c           "
#include "nvfs.wr_header.c          "
#include "nvfs.wr_nvram.c           "
#include "nvfs_size_is_valid.c      "

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


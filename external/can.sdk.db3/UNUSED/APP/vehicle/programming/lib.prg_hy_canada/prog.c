/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog.c 32230 2015-06-11 19:53:02Z simon.demers $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h "  

#ifdef PLATFORM_CM800

    #include "prog.h                     "                   
    #include "prog_init.c                "                   
    #include "prog_init_reset.c          "                   
    #include "prog_reset.c               "                   
    #include "prog_retrieve_option.c     "                   
    #include "prog_feat_table_read.c     "                   
    #include "prog_feat_table_write.c    "                   
    #include "prog_set_option.c          "   
    
    #include "prog_vars.c                "                  
    #include "prog_init_nvfs.c           " 
#else

    #include "prog.h                     "                   
    #include "prog_dis.c                 "                   
    #include "prog_ena.c                 "                   
    #include "prog_err_add.c             "                   
    #include "prog_err_save.c            "                   
    #include "prog_feat_table_read.c     "                   
    #include "prog_feat_table_write.c    "                   
    #include "prog_init.c                "                   
    #include "prog_init_nvfs.c           "                   
    #include "prog_init_reset.c          "                   
    #include "prog_input_process.c       "
    #include "prog_reset.c               "                   
    #include "prog_retrieve_option.c     "                   
    #include "prog_set_exit_callback.c   "                   
    #include "prog_set_option.c          "                   
    #include "prog_task.c                "                   
    #include "prog_vars.c                "                   
#endif

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

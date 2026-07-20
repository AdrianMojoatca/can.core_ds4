#ifndef __BUILD_IDENTITY_H__
#define __BUILD_IDENTITY_H__

#if defined(CORE_DS4_BUILD)
#define BUILD_FW_NAME       "403.CORE.DS4"
#define BUILD_FW_VERSION    "1.00_00"
#define BUILD_FW_INFO       "CORE split image"
#else
#define BUILD_FW_NAME       "403.FW.DS4"
#define BUILD_FW_VERSION    "1.00_00"
#define BUILD_FW_INFO       "FW split image"
#endif

#endif

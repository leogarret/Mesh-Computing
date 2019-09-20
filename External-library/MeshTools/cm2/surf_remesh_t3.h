#ifndef __CM2_SURF_REMESH_T3_H__
#define __CM2_SURF_REMESH_T3_H__

/**
   \file       surf_remesh_t3.h
   \brief      File to include the cm2::surf_remesh_t3 classes & routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2SURFREMESHT3IMPORT, CM2SURFREMESHT3EXPORT, CM2SURFREMESHT3LOCAL and CM2SURFREMESHT3PUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2SURFREMESHT3EXPORT __declspec(dllexport)
   #else
      #define CM2SURFREMESHT3EXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2SURFREMESHT3IMPORT
   #else
      #define CM2SURFREMESHT3IMPORT __declspec(dllimport)
   #endif
   #define CM2SURFREMESHT3LOCAL
   #define CM2SURFREMESHT3PUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2SURFREMESHT3IMPORT
   #define CM2SURFREMESHT3EXPORT __attribute__ ((visibility("default")))
   #define CM2SURFREMESHT3LOCAL __attribute__ ((visibility("hidden")))
   #define CM2SURFREMESHT3PUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2SURFREMESHT3IMPORT
   #define CM2SURFREMESHT3EXPORT
   #define CM2SURFREMESHT3LOCAL
   #define CM2SURFREMESHT3PUBLIC
#endif

// Define CM2SURFREMESHT3_API for DLL builds
#ifdef CM2SURFREMESHT3_EXPORTS
   #define CM2SURFREMESHT3_API CM2SURFREMESHT3EXPORT
#else
   #define CM2SURFREMESHT3_API CM2SURFREMESHT3IMPORT
#endif


// SURF_REMESH_T3 API
#include "surf_remesher_t3.h"


/**
   \namespace cm2::surf_remesh_t3
   \brief Namespace for the 3d-surface triangle remesher.
   */
namespace cm2 {
namespace surf_remesh_t3 {

/// Versioning.
CM2SURFREMESHT3_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2SURFREMESHT3_API void
registration (const char* agreement, const char* user_key);

}  // namespace surf_remesh_t3
}  // namespace cm2

#endif


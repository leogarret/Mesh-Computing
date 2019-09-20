#ifndef __CM2_SURF_REMESH_Q4_H__
#define __CM2_SURF_REMESH_Q4_H__

/**
   \file       surf_remesh_q4.h
   \brief      File to include the cm2::surf_remesh_q4 classes & routines.
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


// Define CM2SURFREMESHQ4IMPORT, CM2SURFREMESHQ4EXPORT, CM2SURFREMESHQ4LOCAL and CM2SURFREMESHQ4PUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2SURFREMESHQ4EXPORT __declspec(dllexport)
   #else
      #define CM2SURFREMESHQ4EXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2SURFREMESHQ4IMPORT
   #else
      #define CM2SURFREMESHQ4IMPORT __declspec(dllimport)
   #endif
   #define CM2SURFREMESHQ4LOCAL
   #define CM2SURFREMESHQ4PUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2SURFREMESHQ4IMPORT
   #define CM2SURFREMESHQ4EXPORT __attribute__ ((visibility("default")))
   #define CM2SURFREMESHQ4LOCAL __attribute__ ((visibility("hidden")))
   #define CM2SURFREMESHQ4PUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2SURFREMESHQ4IMPORT
   #define CM2SURFREMESHQ4EXPORT
   #define CM2SURFREMESHQ4LOCAL
   #define CM2SURFREMESHQ4PUBLIC
#endif

// Define CM2SURFREMESHQ4_API for DLL builds
#ifdef CM2SURFREMESHQ4_EXPORTS
   #define CM2SURFREMESHQ4_API CM2SURFREMESHQ4EXPORT
#else
   #define CM2SURFREMESHQ4_API CM2SURFREMESHQ4IMPORT
#endif


// SURF_REMESH_Q4 API
#include "surf_remesher_q4.h"


/**
   \namespace cm2::surf_remesh_q4
   \brief Namespace for the 3-D surface quadrangle remesher.
   */
namespace cm2 {
namespace surf_remesh_q4 {

/// Versioning.
CM2SURFREMESHQ4_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2SURFREMESHQ4_API void
registration (const char* agreement, const char* user_key);

}  // namespace surf_remesh_q4
}  // namespace cm2

#endif


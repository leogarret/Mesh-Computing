/**
   \file       misc.h
   \brief      Definition of the miscellaneous classes & functions of the MeshTools suite.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MISC_H__
#define __CM2_MISC_H__


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2MISCIMPORT, CM2MISCEXPORT, CM2MISCLOCAL and CM2MISCPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2MISCEXPORT __declspec(dllexport)
   #else
      #define CM2MISCEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2MISCIMPORT
   #else
      #define CM2MISCIMPORT __declspec(dllimport)
   #endif
   #define CM2MISCLOCAL
   #define CM2MISCPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2MISCIMPORT
   #define CM2MISCEXPORT __attribute__ ((visibility("default")))
   #define CM2MISCLOCAL __attribute__ ((visibility("hidden")))
   #define CM2MISCPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2MISCIMPORT
   #define CM2MISCEXPORT
   #define CM2MISCLOCAL
   #define CM2MISCPUBLIC
#endif

// Define CM2MISC_API for DLL builds
#ifdef CM2MISC_EXPORTS
   #define CM2MISC_API CM2MISCEXPORT
#else
   #define CM2MISC_API CM2MISCIMPORT
#endif


// CM2
#include "math1.h"

// CM2MISC API
#include "histogram.h"


/**
   \namespace cm2::misc
   \brief Namespace for miscellaneous classes & functions.
   */
namespace cm2 {
namespace misc {

/// Versioning.
CM2MISC_API const char*
version();

}  // namespace misc
}  // namespace cm2

#endif


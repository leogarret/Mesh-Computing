/**
   \file       meshtools3d.h
   \brief      File to include the cm2::meshtools3d functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS_3D_H__
#define __CM2_MESHTOOLS_3D_H__


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2MESHTOOLS3DIMPORT, CM2MESHTOOLS3DEXPORT, CM2MESHTOOLS3DLOCAL and CM2MESHTOOLS3DPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2MESHTOOLS3DEXPORT __declspec(dllexport)
   #else
      #define CM2MESHTOOLS3DEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2MESHTOOLS3DIMPORT
   #else
      #define CM2MESHTOOLS3DIMPORT __declspec(dllimport)
   #endif
   #define CM2MESHTOOLS3DLOCAL
   #define CM2MESHTOOLS3DPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2MESHTOOLS3DIMPORT
   #define CM2MESHTOOLS3DEXPORT __attribute__ ((visibility("default")))
   #define CM2MESHTOOLS3DLOCAL __attribute__ ((visibility("hidden")))
   #define CM2MESHTOOLS3DPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2MESHTOOLS3DIMPORT
   #define CM2MESHTOOLS3DEXPORT
   #define CM2MESHTOOLS3DLOCAL
   #define CM2MESHTOOLS3DPUBLIC
#endif

// Define CM2MESHTOOLS3D_API for DLL builds
#ifdef CM2MESHTOOLS3D_EXPORTS
   #define CM2MESHTOOLS3D_API CM2MESHTOOLS3DEXPORT
#else
   #define CM2MESHTOOLS3D_API CM2MESHTOOLS3DIMPORT
#endif



// CM2
#include "math1.h"
#include "fe_defs.h"

// API
#include "converters3d.h"
#include "extrude3d.h"
#include "interpolate3d.h"
#include "mesh_transform3d.h"
#include "meshtools3d_misc.h"
#include "metrics3d.h"


/**
   \namespace cm2::meshtools3d
   \brief Namespace for 3-D mesh creation and manipulation functions.
   */

namespace cm2 {
namespace meshtools3d {

/// Versioning.
CM2MESHTOOLS3D_API const char* 
version();

}  // namespace meshtools3d

namespace meshtools3D = meshtools3d;      // Alias for backward compatibility.

}  // namespace cm2

#endif


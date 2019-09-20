/**
   \file       meshtools2d.h
   \brief      File to include the cm2::meshtools2d functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS2D_H__
#define __CM2_MESHTOOLS2D_H__


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2MESHTOOLS2DIMPORT, CM2MESHTOOLS2DEXPORT, CM2MESHTOOLS2DLOCAL and CM2MESHTOOLS2DPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2MESHTOOLS2DEXPORT __declspec(dllexport)
   #else
      #define CM2MESHTOOLS2DEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2MESHTOOLS2DIMPORT
   #else
      #define CM2MESHTOOLS2DIMPORT __declspec(dllimport)
   #endif
   #define CM2MESHTOOLS2DLOCAL
   #define CM2MESHTOOLS2DPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2MESHTOOLS2DIMPORT
   #define CM2MESHTOOLS2DEXPORT __attribute__ ((visibility("default")))
   #define CM2MESHTOOLS2DLOCAL __attribute__ ((visibility("hidden")))
   #define CM2MESHTOOLS2DPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2MESHTOOLS2DIMPORT
   #define CM2MESHTOOLS2DEXPORT
   #define CM2MESHTOOLS2DLOCAL
   #define CM2MESHTOOLS2DPUBLIC
#endif

// Define CM2MESHTOOLS2D_API for DLL builds
#ifdef CM2MESHTOOLS2D_EXPORTS
   #define CM2MESHTOOLS2D_API CM2MESHTOOLS2DEXPORT
#else
   #define CM2MESHTOOLS2D_API CM2MESHTOOLS2DIMPORT
#endif



// CM2
#include "math1.h"
#include "fe_defs.h"

// API
#include "converters2d.h"
#include "extrude2d.h"
#include "interpolate2d.h"
#include "mesh_struct2d.h"
#include "mesh_surface_chord.h"
#include "mesh_transform2d.h"
#include "meshtools2d_misc.h"
#include "metrics2d.h"



/**
   \namespace cm2::meshtools2d
   \brief Namespace for 2-D mesh creation and manipulation functions.
*/

namespace cm2 {
namespace meshtools2d {

/// Versioning.
CM2MESHTOOLS2D_API const char* 
version();

}  // namespace meshtools2d

namespace meshtools2D = meshtools2d;      // Alias for backward compatibility.

}  // namespace cm2

#endif


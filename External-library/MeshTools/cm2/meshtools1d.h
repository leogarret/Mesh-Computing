/**
   \file       meshtools1d.h
   \brief      File to include the cm2::meshtools1d functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS1D_H__
#define __CM2_MESHTOOLS1D_H__


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2MESHTOOLS1DIMPORT, CM2MESHTOOLS1DEXPORT, CM2MESHTOOLS1DLOCAL and CM2MESHTOOLS1DPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2MESHTOOLS1DEXPORT __declspec(dllexport)
   #else
      #define CM2MESHTOOLS1DEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2MESHTOOLS1DIMPORT
   #else
      #define CM2MESHTOOLS1DIMPORT __declspec(dllimport)
   #endif
   #define CM2MESHTOOLS1DLOCAL
   #define CM2MESHTOOLS1DPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2MESHTOOLS1DIMPORT
   #define CM2MESHTOOLS1DEXPORT __attribute__ ((visibility("default")))
   #define CM2MESHTOOLS1DLOCAL __attribute__ ((visibility("hidden")))
   #define CM2MESHTOOLS1DPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2MESHTOOLS1DIMPORT
   #define CM2MESHTOOLS1DEXPORT
   #define CM2MESHTOOLS1DLOCAL
   #define CM2MESHTOOLS1DPUBLIC
#endif

// Define CM2MESHTOOLS1D_API for DLL builds
#ifdef CM2MESHTOOLS1D_EXPORTS
   #define CM2MESHTOOLS1D_API CM2MESHTOOLS1DEXPORT
#else
   #define CM2MESHTOOLS1D_API CM2MESHTOOLS1DIMPORT
#endif



// CM2
#include "math1.h"
#include "fe_defs.h"

// MESHTOOLS1D API
#include "converters1d.h"
#include "extrude1d.h"
#include "mesh_curve_chord.h"
#include "mesh_line.h"
#include "mesh_spline.h"
#include "mesh_straight.h"
#include "mesh_transform1d.h"
#include "meshtools1d_misc.h"
#include "metrics1d.h"


/**
   \namespace cm2::meshtools1d
   \brief Namespace for 1-D mesh creation and manipulation functions.
   */

namespace cm2 {
namespace meshtools1d {

/// Versioning.
CM2MESHTOOLS1D_API const char*
version();

}  // namespace meshtools1d

namespace meshtools1D = meshtools1d;      // Alias for backward compatibility.

}  // namespace cm2

#endif


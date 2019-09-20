/**
   \file       meshtools.h
   \brief      File to include the cm2::meshtools routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS_H__
#define __CM2_MESHTOOLS_H__


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2MESHTOOLSIMPORT, CM2MESHTOOLSEXPORT, CM2MESHTOOLSLOCAL and CM2MESHTOOLSPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2MESHTOOLSEXPORT __declspec(dllexport)
   #else
      #define CM2MESHTOOLSEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2MESHTOOLSIMPORT
   #else
      #define CM2MESHTOOLSIMPORT __declspec(dllimport)
   #endif
   #define CM2MESHTOOLSLOCAL
   #define CM2MESHTOOLSPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2MESHTOOLSIMPORT
   #define CM2MESHTOOLSEXPORT __attribute__ ((visibility("default")))
   #define CM2MESHTOOLSLOCAL __attribute__ ((visibility("hidden")))
   #define CM2MESHTOOLSPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2MESHTOOLSIMPORT
   #define CM2MESHTOOLSEXPORT
   #define CM2MESHTOOLSLOCAL
   #define CM2MESHTOOLSPUBLIC
#endif

// Define CM2MESHTOOLS_API for DLL builds
#ifdef CM2MESHTOOLS_EXPORTS
   #define CM2MESHTOOLS_API CM2MESHTOOLSEXPORT
#else
   #define CM2MESHTOOLS_API CM2MESHTOOLSIMPORT
#endif


// CM2 BASE
#include "fe_defs.h"
#include "math1.h"
#include "misc.h"

// MESHTOOLS API
#include "edge_qualities.h"
#include "mesh_transform.h"
#include "meshtools_misc.h"
#include "metrics.h"
#include "neighbors.h"
#include "node_detector.h"
#include "translator_ensight.h"
#include "translator_femap.h"
#include "translator_medit.h"
#include "translator_nastran.h"
#include "translator_stl.h"
#include "translator_vtk.h"
#include "translator_wfrt.h"
#include "shape_qualities.h"
#include "subdomains.h"



/**
   \namespace cm2::meshtools
   \brief Namespace for general mesh manipulation functions.

   This namespace gathers general mesh and points manipulation functions for 1-D, 2-D or 3-D.
   */
namespace cm2 {
namespace meshtools {

/// Versioning.
CM2MESHTOOLS_API const char*
version();

}  // namespace meshtools
}  // namespace cm2


#endif


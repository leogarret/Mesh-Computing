#ifndef __CM2_QUADMESH_H__
#define __CM2_QUADMESH_H__

/**
   \file       quadmesh.h
   \brief      File to include the cm2::quadmesh::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::quadmesh namespace. 
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


// Define CM2QUADMESHIMPORT, CM2QUADMESHEXPORT, CM2QUADMESHLOCAL and CM2QUADMESHPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2QUADMESHEXPORT __declspec(dllexport)
   #else
      #define CM2QUADMESHEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2QUADMESHIMPORT
   #else
      #define CM2QUADMESHIMPORT __declspec(dllimport)
   #endif
   #define CM2QUADMESHLOCAL
   #define CM2QUADMESHPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2QUADMESHIMPORT
   #define CM2QUADMESHEXPORT __attribute__ ((visibility("default")))
   #define CM2QUADMESHLOCAL __attribute__ ((visibility("hidden")))
   #define CM2QUADMESHPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2QUADMESHIMPORT
   #define CM2QUADMESHEXPORT
   #define CM2QUADMESHLOCAL
   #define CM2QUADMESHPUBLIC
#endif

// Define CM2QUADMESH_API for DLL builds
#if !defined(CM2QUADMESH_API)
   #ifdef CM2QUADMESH_EXPORTS
      #define CM2QUADMESH_API CM2QUADMESHEXPORT
   #else
      #define CM2QUADMESH_API CM2QUADMESHIMPORT
   #endif
#endif


// QUADMESH API
#include "mesher_q4.h"


/**
   \namespace  cm2::quadmesh
   \brief      Namespace for the quadrangle isotropic %mesher.
   */
namespace cm2 {
namespace quadmesh { 


/// Versioning.
CM2QUADMESH_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement   A short user agreement in human readable form.
   \param[in]   user_key    A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2QUADMESH_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace quadmesh
}  //  end namespace cm2


#endif

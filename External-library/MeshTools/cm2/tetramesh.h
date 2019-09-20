#ifndef __CM2_TETRAMESH_H__
#define __CM2_TETRAMESH_H__

/**
   \file       tetramesh.h
   \brief      File to include the Delaunay cm2::tetramesh::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::tetramesh namespace. 
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


// Define CM2TETRAMESHIMPORT, CM2TETRAMESHEXPORT, CM2TETRAMESHLOCAL and CM2TETRAMESHPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2TETRAMESHEXPORT __declspec(dllexport)
   #else
      #define CM2TETRAMESHEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2TETRAMESHIMPORT
   #else
      #define CM2TETRAMESHIMPORT __declspec(dllimport)
   #endif
   #define CM2TETRAMESHLOCAL
   #define CM2TETRAMESHPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2TETRAMESHIMPORT
   #define CM2TETRAMESHEXPORT __attribute__ ((visibility("default")))
   #define CM2TETRAMESHLOCAL __attribute__ ((visibility("hidden")))
   #define CM2TETRAMESHPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2TETRAMESHIMPORT
   #define CM2TETRAMESHEXPORT
   #define CM2TETRAMESHLOCAL
   #define CM2TETRAMESHPUBLIC
#endif

// Define CM2TETRAMESH_API for DLL builds
#if !defined(CM2TETRAMESH_API)
   #ifdef CM2TETRAMESH_EXPORTS
      #define CM2TETRAMESH_API CM2TETRAMESHEXPORT
   #else
      #define CM2TETRAMESH_API CM2TETRAMESHIMPORT
   #endif
#endif


// TETRAMESH API
#include "mesher_th4.h"


/**
   \namespace  cm2::tetramesh
   \brief      Namespace for tetrahedron isotropic %mesher.
   */

namespace cm2 {
namespace tetramesh { 

/// Versioning.
CM2TETRAMESH_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2TETRAMESH_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace tetramesh
}  //  end namespace cm2


#endif

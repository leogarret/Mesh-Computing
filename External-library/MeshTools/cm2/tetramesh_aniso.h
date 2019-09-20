#ifndef __CM2_TETRAMESH_ANISO_H__
#define __CM2_TETRAMESH_ANISO_H__

/**
   \file       tetramesh_aniso.h
   \brief      File to include the Delaunay cm2::tetramesh_aniso::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::tetramesh_aniso namespace. 
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


// Define CM2TETRAMESHANISOIMPORT, CM2TETRAMESHANISOEXPORT, CM2TETRAMESHANISOLOCAL and CM2TETRAMESHANISOPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2TETRAMESHANISOEXPORT __declspec(dllexport)
   #else
      #define CM2TETRAMESHANISOEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2TETRAMESHANISOIMPORT
   #else
      #define CM2TETRAMESHANISOIMPORT __declspec(dllimport)
   #endif
   #define CM2TETRAMESHANISOLOCAL
   #define CM2TETRAMESHANISOPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2TETRAMESHANISOIMPORT
   #define CM2TETRAMESHANISOEXPORT __attribute__ ((visibility("default")))
   #define CM2TETRAMESHANISOLOCAL __attribute__ ((visibility("hidden")))
   #define CM2TETRAMESHANISOPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2TETRAMESHANISOIMPORT
   #define CM2TETRAMESHANISOEXPORT
   #define CM2TETRAMESHANISOLOCAL
   #define CM2TETRAMESHANISOPUBLIC
#endif

// Define CM2TETRAMESH_ANISO_API for DLL builds
#if !defined(CM2TETRAMESH_ANISO_API)
   #ifdef CM2TETRAMESH_ANISO_EXPORTS
      #define CM2TETRAMESH_ANISO_API CM2TETRAMESHANISOEXPORT
   #else
      #define CM2TETRAMESH_ANISO_API CM2TETRAMESHANISOIMPORT
   #endif
#endif


// TETRAMESH_ANISO API
#include "mesher_th4_aniso.h"


/**
   \namespace  cm2::tetramesh_aniso
   \brief      Namespace for tetrahedron anisotropic %mesher.
   */

namespace cm2 {
namespace tetramesh_aniso { 

/// Versioning.
CM2TETRAMESH_ANISO_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement   A short user agreement in human readable form.
   \param[in]   user_key    A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2TETRAMESH_ANISO_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace tetramesh_aniso
}  //  end namespace cm2


#endif

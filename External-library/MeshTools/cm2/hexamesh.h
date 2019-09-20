#ifndef __CM2_HEXAMESH_H__
#define __CM2_HEXAMESH_H__

/**
   \file       hexamesh.h
   \brief      File to include the Delaunay cm2::hexamesh::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::hexamesh namespace. 
   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2HEXAMESHIMPORT, CM2HEXAMESHEXPORT, CM2HEXAMESHLOCAL and CM2HEXAMESHPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2HEXAMESHEXPORT __declspec(dllexport)
   #else
      #define CM2HEXAMESHEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2HEXAMESHIMPORT
   #else
      #define CM2HEXAMESHIMPORT __declspec(dllimport)
   #endif
   #define CM2HEXAMESHLOCAL
   #define CM2HEXAMESHPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2HEXAMESHIMPORT
   #define CM2HEXAMESHEXPORT __attribute__ ((visibility("default")))
   #define CM2HEXAMESHLOCAL __attribute__ ((visibility("hidden")))
   #define CM2HEXAMESHPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2HEXAMESHIMPORT
   #define CM2HEXAMESHEXPORT
   #define CM2HEXAMESHLOCAL
   #define CM2HEXAMESHPUBLIC
#endif

// Define CM2HEXAMESH_API for DLL builds
#ifndef CM2HEXAMESH_API
   #ifdef CM2HEXAMESH_EXPORTS
      #define CM2HEXAMESH_API CM2HEXAMESHEXPORT
   #else
      #define CM2HEXAMESH_API CM2HEXAMESHIMPORT
   #endif
#endif


// HEXAMESH API
#include "mesher_h8.h"


/**
   \namespace  cm2::hexamesh
   \brief      Namespace for the hex-dominant %mesher.
   */

namespace cm2 {
namespace hexamesh { 

/// Versioning.
CM2HEXAMESH_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param   agreement   A short user agreement in human readable form.
   \param   user_key    A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2HEXAMESH_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace hexamesh
}  //  end namespace cm2


#endif

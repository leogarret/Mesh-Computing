#ifndef __CM2_TRIAMESH_H__
#define __CM2_TRIAMESH_H__

/**
   \file       triamesh.h
   \brief      File to include the cm2::triamesh::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::triamesh namespace. 
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


// Define CM2TRIAMESHIMPORT, CM2TRIAMESHEXPORT, CM2TRIAMESHLOCAL and CM2TRIAMESHPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2TRIAMESHEXPORT __declspec(dllexport)
   #else
      #define CM2TRIAMESHEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2TRIAMESHIMPORT
   #else
      #define CM2TRIAMESHIMPORT __declspec(dllimport)
   #endif
   #define CM2TRIAMESHLOCAL
   #define CM2TRIAMESHPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2TRIAMESHIMPORT
   #define CM2TRIAMESHEXPORT __attribute__ ((visibility("default")))
   #define CM2TRIAMESHLOCAL __attribute__ ((visibility("hidden")))
   #define CM2TRIAMESHPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2TRIAMESHIMPORT
   #define CM2TRIAMESHEXPORT
   #define CM2TRIAMESHLOCAL
   #define CM2TRIAMESHPUBLIC
#endif

// Define CM2TRIAMESH_API for DLL builds
#if !defined(CM2TRIAMESH_API)
   #ifdef CM2TRIAMESH_EXPORTS
      #define CM2TRIAMESH_API CM2TRIAMESHEXPORT
   #else
      #define CM2TRIAMESH_API CM2TRIAMESHIMPORT
   #endif
#endif


// TRIAMESH API
#include "mesher_t3.h"


/**
   \namespace  cm2::triamesh
   \brief      Namespace for the triangle isotropic %mesher.
   */
namespace cm2 {
namespace triamesh { 


/// Versioning.
CM2TRIAMESH_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2TRIAMESH_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace triamesh
}  //  end namespace cm2


#endif

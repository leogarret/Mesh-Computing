#ifndef __CM2_TRIAMESH_ANISO_H__
#define __CM2_TRIAMESH_ANISO_H__

/**
   \file       triamesh_aniso.h
   \brief      File to include the cm2::triamesh_aniso::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::triamesh_aniso namespace. 
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


// Define CM2TRIAMESHANISOIMPORT, CM2TRIAMESHANISOEXPORT, CM2TRIAMESHANISOLOCAL and CM2TRIAMESHANISOPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2TRIAMESHANISOEXPORT __declspec(dllexport)
   #else
      #define CM2TRIAMESHANISOEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2TRIAMESHANISOIMPORT
   #else
      #define CM2TRIAMESHANISOIMPORT __declspec(dllimport)
   #endif
   #define CM2TRIAMESHANISOLOCAL
   #define CM2TRIAMESHANISOPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2TRIAMESHANISOIMPORT
   #define CM2TRIAMESHANISOEXPORT __attribute__ ((visibility("default")))
   #define CM2TRIAMESHANISOLOCAL __attribute__ ((visibility("hidden")))
   #define CM2TRIAMESHANISOPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2TRIAMESHANISOIMPORT
   #define CM2TRIAMESHANISOEXPORT
   #define CM2TRIAMESHANISOLOCAL
   #define CM2TRIAMESHANISOPUBLIC
#endif

// Define CM2TRIAMESH_ANISO_API for DLL builds
#if !defined(CM2TRIAMESH_ANISO_API)
   #ifdef CM2TRIAMESH_ANISO_EXPORTS
      #define CM2TRIAMESH_ANISO_API CM2TRIAMESHANISOEXPORT
   #else
      #define CM2TRIAMESH_ANISO_API CM2TRIAMESHANISOIMPORT
   #endif
#endif


// TRIAMESH_ANISO API
#include "mesher_t3_aniso.h"


/**
   \namespace  cm2::triamesh_aniso
   \brief      Namespace for the triangle anisotropic %mesher.
   */
namespace cm2 {
namespace triamesh_aniso { 

/// Versioning.
CM2TRIAMESH_ANISO_API const char*
version();


/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2TRIAMESH_ANISO_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace triamesh_aniso
}  //  end namespace cm2


#endif

#ifndef __CM2_QUADMESH_ANISO_H__
#define __CM2_QUADMESH_ANISO_H__

/**
   \file       quadmesh_aniso.h
   \brief      File to include the cm2::quadmesh_aniso::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::quadmesh_aniso namespace. 
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


// Define CM2QUADMESHANISOIMPORT, CM2QUADMESHANISOEXPORT, CM2QUADMESHANISOLOCAL and CM2QUADMESHANISOPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2QUADMESHANISOEXPORT __declspec(dllexport)
   #else
      #define CM2QUADMESHANISOEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2QUADMESHANISOIMPORT
   #else
      #define CM2QUADMESHANISOIMPORT __declspec(dllimport)
   #endif
   #define CM2QUADMESHANISOLOCAL
   #define CM2QUADMESHANISOPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2QUADMESHANISOIMPORT
   #define CM2QUADMESHANISOEXPORT __attribute__ ((visibility("default")))
   #define CM2QUADMESHANISOLOCAL __attribute__ ((visibility("hidden")))
   #define CM2QUADMESHANISOPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2QUADMESHANISOIMPORT
   #define CM2QUADMESHANISOEXPORT
   #define CM2QUADMESHANISOLOCAL
   #define CM2QUADMESHANISOPUBLIC
#endif

// Define CM2QUADMESH_ANISO_API for DLL builds
#ifdef CM2QUADMESH_ANISO_EXPORTS
   #define CM2QUADMESH_ANISO_API CM2QUADMESHANISOEXPORT
#else
   #define CM2QUADMESH_ANISO_API CM2QUADMESHANISOIMPORT
#endif


// QUADMESH_ANISO API
#include "mesher_q4_aniso.h"


/**
   \namespace  cm2::quadmesh_aniso
   \brief      Namespace for the quadrangle anisotropic %mesher.
   */
namespace cm2 {
namespace quadmesh_aniso { 


/// Versioning.
CM2QUADMESH_ANISO_API const char*
version();

/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2QUADMESH_ANISO_API void
registration (const char* agreement, const char* user_key);


}  //  end namespace quadmesh_aniso
}  //  end namespace cm2


#endif

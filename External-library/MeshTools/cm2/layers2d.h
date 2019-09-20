#ifndef __CM2_LAYERS2D_H__
#define __CM2_LAYERS2D_H__

/**
   \file       layers2d.h
   \brief      File to include the cm2::layers2d::mesher and related structs.
   \details    This file provides access to the classes and functions of the cm2::layers2d namespace. 
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


// Define CM2LAYERS2DIMPORT, CM2LAYERS2DEXPORT, CM2LAYERS2DLOCAL and CM2LAYERS2DPUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2LAYERS2DEXPORT __declspec(dllexport)
   #else
      #define CM2LAYERS2DEXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2LAYERS2DIMPORT
   #else
      #define CM2LAYERS2DIMPORT __declspec(dllimport)
   #endif
   #define CM2LAYERS2DLOCAL
   #define CM2LAYERS2DPUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2LAYERS2DIMPORT
   #define CM2LAYERS2DEXPORT __attribute__ ((visibility("default")))
   #define CM2LAYERS2DLOCAL  __attribute__ ((visibility("hidden")))
   #define CM2LAYERS2DPUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2LAYERS2DIMPORT
   #define CM2LAYERS2DEXPORT
   #define CM2LAYERS2DLOCAL
   #define CM2LAYERS2DPUBLIC
#endif

// Define CM2LAYERS2D_API for DLL builds
#if !defined(CM2LAYERS2D_API)
   #ifdef CM2LAYERS2D_EXPORTS
      #define CM2LAYERS2D_API CM2LAYERS2DEXPORT
   #else
      #define CM2LAYERS2D_API CM2LAYERS2DIMPORT
   #endif
#endif


// LAYERS 2D API
#include "mesher_layers_2d.h"


/**
   \namespace  cm2::layers2d
   \brief      Namespace for the 2-D boundary layer %mesher.
   */
namespace cm2 {
namespace layers2d { 


/// Versioning.
CM2LAYERS2D_API const char*
version();


/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2LAYERS2D_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace layers2d
}  //  end namespace cm2


#endif

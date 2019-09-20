/**
   \file       intersect_t3.h
   \brief      File to include the cm2::intersect_t3::mesher and related structs.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   $Author: Damien LUCAS $   
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_INTERSECT_T3_H__
#define __CM2_INTERSECT_T3_H__

// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2INTERSECT_T3IMPORT, CM2INTERSECT_T3EXPORT, CM2INTERSECT_T3LOCAL and CM2INTERSECT_T3PUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2INTERSECT_T3EXPORT __declspec(dllexport)
   #else
      #define CM2INTERSECT_T3EXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2INTERSECT_T3IMPORT
   #else
      #define CM2INTERSECT_T3IMPORT __declspec(dllimport)
   #endif
   #define CM2INTERSECT_T3LOCAL
   #define CM2INTERSECT_T3PUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2INTERSECT_T3IMPORT
   #define CM2INTERSECT_T3EXPORT __attribute__ ((visibility("default")))
   #define CM2INTERSECT_T3LOCAL  __attribute__ ((visibility("hidden")))
   #define CM2INTERSECT_T3PUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2INTERSECT_T3IMPORT
   #define CM2INTERSECT_T3EXPORT
   #define CM2INTERSECT_T3LOCAL
   #define CM2INTERSECT_T3PUBLIC
#endif

// Define CM2INTERSECT_T3_API for DLL builds
#if !defined(CM2INTERSECT_T3_API)
   #ifdef CM2INTERSECT_T3_EXPORTS
      #define CM2INTERSECT_T3_API CM2INTERSECT_T3EXPORT
   #else
      #define CM2INTERSECT_T3_API CM2INTERSECT_T3IMPORT
   #endif
#endif


// INTERSECT_T3 API
#include "intersector_t3.h"


/**
   \namespace  cm2::intersect_t3
   \brief      Namespace for the surface triangle mesh intersector.
   */
namespace cm2 {
namespace intersect_t3 { 


/// Versioning.
CM2INTERSECT_T3_API const char*
version();


/**
   Function to unlock the DLL.
   The user must provide two strings in order to use this DLL.

   \param[in]   agreement     A short user agreement in human readable form.
   \param[in]   user_key      A secure encoded string.

   \note    To get or update a license, please contact license@computing-objects.com
   */
CM2INTERSECT_T3_API void
registration (const char* agreement, const char* user_key);

}  //  end namespace intersect_t3
}  //  end namespace cm2


#endif

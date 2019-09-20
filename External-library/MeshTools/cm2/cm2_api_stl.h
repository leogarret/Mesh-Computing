#ifndef __CM2_API_STL_H__
#define __CM2_API_STL_H__

/**
   \file       cm2_api_stl.h
   \brief      STL include file for the CM2 API.
               Users should normally not change this file.
   \copyright  (C)1999-2019, Computing Objects, France. info@computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (_MSC_VER < 1800)
#   pragma warning (push)
#   pragma warning (disable: 4996)           // Disable deprecated STL warnings.
#endif

#if defined(CM2_STL_SGI)

   #define __STL_NO_EXCEPTIONS 
   #define __STL_NO_DRAND48 

   #ifndef CM2_NO_CLASS_PARTIAL_SPECIALIZATION
   #  define  __STL_CLASS_PARTIAL_SPECIALIZATION
   #endif

   #ifdef _DEBUG  
   #  define   __STL_DEBUG
   #  define   __STL_DEBUG_ALLOC
   #  define   __STL_ASSERTIONS
   #endif

   #define _STL_NO_CONCEPT_CHECKS

#else
   // MS VISUAL 6 lacks std::min and std::max.
   #if defined(_MSC_VER) && (_MSC_VER <= 1200) 
      namespace std 
      {
         template <class T>
         inline const T& min(const T& a, const T& b) { return b < a ? b : a; }

         template <class T>
         inline const T& max(const T& a, const Tp& b) { return a < b ? b : a; }
      }
   #endif
#endif


/* *********************************** */
/*          BASIC STL NEEDS            */
/* *********************************** */

#include <algorithm>
#include <complex>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <vector>



#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (_MSC_VER < 1800)
#   pragma warning (pop)
#endif


#endif      // __CM2_API_STL_H__

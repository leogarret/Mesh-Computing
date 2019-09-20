#ifndef __CM2_API_CONFIG_H__
#define __CM2_API_CONFIG_H__

/**
   \file       cm2_api_config.h
   \brief      CM2 API configuration file. 
               Users should normally not change this file.
   \copyright  (C)1999-2019, Computing Objects, France. info@computing-objects.com

   $Rev: 3441 $
   $Date: 2019-01-08 14:15:26 +0100 (mar., 08 janv. 2019) $   
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#include <stddef.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#ifdef sun
#  include <ieeefp.h>
#endif


/***********************
   MICROSOFT VISUAL C++ 
 ***********************/
#ifdef _MSC_VER      
#include <windows.h>

// Undef MS macros min and max.
#  ifdef min 
#     undef min
#  endif
#  ifdef max 
#     undef max
#  endif

// Define if the compiler needs to instantiate explicitly the template classes exported by a DLL.
#  define CM2_EXPLICIT_INSTANCE_EXPORT

// MS VISUAL 6.0
#  if (_MSC_VER < 1300) 
#     pragma message ("Warning: MS Visual 6.0 is deprecated and will not be supported in future versions.")

//    Define if the compiler supports template partial specialization.
#     define CM2_NO_CLASS_PARTIAL_SPECIALIZATION

//    Define if the compiler rejects template function declaration.
#     define CM2_NO_TMPL_FUNC_DECLARATION

//    Define if the compiler rejects template specialization.
#     define CM2_FUNCTION_TMPL_SPECIALIZATION_BUG

// MS VISUAL .NET 2002
#  elif (_MSC_VER == 1300) 
#     pragma message ("Warning: MS Visual .NET 2002 is deprecated and will not be supported in future versions.")

// MS VISUAL .NET 2003
#  elif (_MSC_VER == 1310)
#     define CM2_NOINLINE  __declspec(noinline) 
#     define CM2_ALIGN(N)  __declspec(align(N)) 
#     define CM2_RESTRICT

// MS VISUAL .NET 2005, 2008, 2010
#  elif (_MSC_VER >= 1400) 
#     define CM2_NOINLINE  __declspec(noinline) 
#     define CM2_ALIGN(N)  __declspec(align(N)) 
#     define CM2_RESTRICT  __restrict
#  endif


/***********************
           GCC 
 ***********************/
#elif defined(__GNUC__)

// Define if the compiler has C++ visibility support for DSO (Dynamic Shared Object).
#  if (__GNUC__ >= 4)
#     define CM2_USE_GCC_VISIBILITY_ATTRIBS
#  endif

// Define if the compiler support builtin atomics.
#  if (__GNUC__ * 100 + __GNUC_MINOR__ * 10 >= 420)
#     define CM2_GCC_BUILTIN_ATOMICS
//#     include <cstdatomic>
#  endif

#  define CM2_NOINLINE  __attribute__((noinline)) 
#  if (__GNUC__ * 100 + __GNUC_MINOR__ * 10 >= 420)
#     define CM2_ALIGN(N)  __attribute__(aligned(N))
#     define CM2_RESTRICT  __restrict
#  endif


/*********************************************
       INTEL ICC (NOT PLUGGED INTO MSVC)
 *********************************************/
#elif defined(__INTEL_COMPILER)

// Define if the compiler needs to instantiate explicitly the template classes exported by a DLL.
#  define CM2_EXPLICIT_INSTANCE_EXPORT

#  if (__INTEL_COMPILER > 1200) 
#     ifndef CM2_USE_GCC_VISIBILITY_ATTRIBS
#        define CM2_USE_GCC_VISIBILITY_ATTRIBS
#     endif

#     ifndef CM2_GCC_BUILTIN_ATOMICS
#        define CM2_GCC_BUILTIN_ATOMICS
#     endif

#     define CM2_NOINLINE  __attribute__((noinline)) 
#     define CM2_ALIGN(N)  __attribute__(aligned(N))
#     define CM2_RESTRICT  __restrict
#  endif


/***********************
     SGI MIPS PRO CC 
 ***********************/
#elif defined(__sgi)
#  pragma message ("Warning: SGI MIPS PRO CC is not supported. Hacking this file may be required.")

// Define if the compiler supports template partial specialization.
#  define CM2_NO_CLASS_PARTIAL_SPECIALIZATION
#  define CM2_NOINLINE 
#  define CM2_ALIGN(N) 
#  define CM2_RESTRICT  __restrict


/***********************
       SUN PRO CC 
 ***********************/
#elif defined(__SUNPRO_CC)
#  pragma message ("Warning: SUN PRO CC is not supported. Hacking this file may be required.")

// Define if the compiler supports template partial specialization.
//#  define CM2_NO_CLASS_PARTIAL_SPECIALIZATION

#  define CM2_NOINLINE 
#  define CM2_ALIGN(N) 
#  define CM2_RESTRICT  __restrict


/***********************
     OTHER COMPILERS
 ***********************/
#else                
#  pragma message ("Warning: Unknown compiler. Might not be supported. Hacking this file may be required.")

// Define if the compiler needs to instantiate explicitly the template classes exported by a DLL.
#  define CM2_EXPLICIT_INSTANCE_EXPORT

// Define if the compiler supports template partial specialization.
//#  define CM2_NO_CLASS_PARTIAL_SPECIALIZATION

// Define if the compiler rejects template function declaration.
//#  define CM2_NO_TMPL_FUNC_DECLARATION

#  define CM2_NOINLINE 
#  define CM2_ALIGN(N) 
#  define CM2_RESTRICT  __restrict

#endif



/***************************
      FALLBACK DEFINES          
 ***************************/ 
#ifndef INLINE
#  define INLINE  inline
#endif

#ifndef CM2_NOINLINE
#  define CM2_NOINLINE
#endif

#ifndef CM2_ALIGN
#  define CM2_ALIGN(N) 
#endif

#ifndef CM2_RESTRICT
#  define CM2_RESTRICT
#endif



/********************************************************
 *         CACHE AND ALIGNMENT SPECIFIC DEFINES         *
 ********************************************************/ 
// Currently, CM2 can find the last-level cache (LLC) size at run-time only on x86 and x86-64 platforms
// and in a build by a recent compiler (MSVC >= 2005, GCC >= 4.2).
// In all other cases, the following default last-level cache (LLC) size will be used:
// "CM2_LLC" MB * number of logical processors.
#ifdef CM2_LLC
#  define CM2_DEFAULT_LLC_SIZE_PER_PROC  (CM2_LLC * 1024 * 1024)
#else
#  define CM2_DEFAULT_LLC_SIZE_PER_PROC  (1024 * 1024)
#endif


// Minimum alignment in CM2 dynamic arrays (8-byte alignment).
// Alignment must be power of 2 and less than max<unsigned short>.
// The actual alignment is the highest of CM2_ALIGNMENT and sizeof(T).
#define  CM2_ALIGNMENT     8


/********************************************************
 *                VECTORIZATION FLAGS                   *
 ********************************************************/ 
// Vectorizers prefer when loops are not unrolled.
// Without vectorization, performance is better when loops are unrolled.
#ifndef CM2_NO_UNROLL
#  if defined(__AVX__) || defined(__AVX2__)
#     define CM2_NO_UNROLL
#  endif
#endif


/********************************************************
 *                OMP SPECIFIC FLAGS                    *
 ********************************************************/ 
#ifdef _OPENMP
   #include <omp.h>
#endif

// Empirical minimum length of simple loops (such as vector dot product) for OMP benefit.
#if !defined(CM2_OMP_MIN_LOOP)
   #define CM2_OMP_MIN_LOOP     (16384)
#endif


/*****************************
       SOME OTHER DEFINES          
 *****************************/ 
#define CM2_NONE           (unsigned(-1))
#define CM2_ALL            (unsigned(-2))
#define CM2_EPSILON        (1.E-12)
#define CM2_EPSILON2       (CM2_EPSILON * CM2_EPSILON)

#ifdef NDEBUG
#  define DUM(param) 
#else
#  define DUM(param)        param
#endif

#ifndef _OPENMP
#  define DUM_OMP(param) 
#else
#  define DUM_OMP(param)    param
#endif

// Macros for bit manipulation (on unsigned).
#define CM2_TESTBIT(bits, i)      (((bits >> i) & 1u) == 1u)
#define CM2_SETBIT(bits, i)       (bits |= (1u << i))
#define CM2_CLEARBIT(bits, i)     (bits &= ~(1u << i))
#define CM2_MASKBITS(bits, i, j)  (bits & ~(~0u << (i+1)) & (~0u << j))


#endif      //  __CM2_API_CONFIG_H__

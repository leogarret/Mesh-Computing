 /**
   \file       vecvec.h
   \brief      Definition of vector-vector template functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_VECTOR_VECTOR_OPS__
#define __CM2_VECTOR_VECTOR_OPS__


#include "matvec_tags.h"
#include "math1_aux.h"
#include "vecscal.h"

#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (push)
#pragma warning (disable : 4068)             // Disable unrecognized OMP pragma.
#endif


/**
   \namespace  cm2::vecvec
   \brief      Namespace for Vector-Vector template functions.

   Namespace \ref vecvec is made of two categories of template functions:
      -  Basic functions on iterators (can be used with raw pointers)
         \ref VecVecBasicGroup "Vector-Vector Basic Template Functions"
      -  Functions on vector classes (concepts of vector with members `begin()`, `end()` and `size()`)
         \ref VecVecGroup "Vector-Vector Template Functions"
   */

namespace cm2 { 
namespace vecvec { 

/** \defgroup VecVecBasicGroup Vector-Vector Basic Template Functions */
//@{

/**
   Element-wise copy. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] = x[i], for i = 0, 1 
   \endverbatim

   \return     The iterator \p y + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy2 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] = x[0];
   y[1] = x[1];
   return y+2;
}

/**
   Element-wise copy. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] = x[i], for i = 0, 1, 2 
   \endverbatim

   \return     The iterator \p y + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy3 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] = x[0];
   y[1] = x[1];
   y[2] = x[2];
   return y+3;
}

/**
   Element-wise copy. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] = x[i], for i = 0 ... n-1 
   \endverbatim

   \return     The iterator \p y + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE T*
copy_n (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] = x[i];
   return y + n;
}

/**
   Element-wise copy. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] = x[i], for i = 0 ... n-1 
   \endverbatim

   \return     The iterator \p y + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class IterX, class IterY>
INLINE IterY
copy_n (IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] = x[i];
   return y + n;
}



/**
   Element-wise swap. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      swap(x[i], y[i]), for i = 0, 1. 
   \endverbatim

   \return     The iterator \p y + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
swap2 (T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   std::swap(x[0], y[0]);
   std::swap(x[1], y[1]);
   return y+2;
}

/**
   Element-wise swap. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      swap(x[i], y[i]), for i = 0, 1, 2. 
   \endverbatim

   \return     The iterator \p y + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
swap3 (T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   std::swap(x[0], y[0]);
   std::swap(x[1], y[1]);
   std::swap(x[2], y[2]);
   return y+3;
}

/**
   Element-wise swap. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      swap(x[i], y[i]), for i = 0 ... n-1 
   \endverbatim

   \return     The iterator \p y + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE T*
swap_n (T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         std::swap(x[i], y[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         std::swap(x[0], y[0]);
         std::swap(x[1], y[1]);
         std::swap(x[2], y[2]);
         std::swap(x[3], y[3]);
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] = x[0];
   else if (n == 2)
      vecvec::swap2(x, y);
   else if (n == 3)
      vecvec::swap3(x, y);

   return y + n;
}

/**
   Element-wise swap. 
   Serial version with iterators (may alias).

   \verbatim    
      swap(x[i], y[i]), for i = 0 ... n-1 
   \endverbatim

   \return     The iterator \p y + \p n.
   */
template <class IterX, class IterY>
INLINE IterY
swap_n (IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      std::swap(x[i], y[i]);
   return y + n;
}


/**
   Element-wise copy with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] = a * x[i], for i = 0, 1 
   \endverbatim

   \return     The iterator \p y + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_scale2 (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] = a * x[0];
   y[1] = a * x[1];
   return y + 2;
}

/**
   Element-wise copy with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] = a * x[i], for i = 0, 1, 2 
   \endverbatim

   \return     The iterator \p y + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_scale3 (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] = a * x[0];
   y[1] = a * x[1];
   y[2] = a * x[2];
   return y + 3;
}

/**
   Element-wise copy with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] = a * x[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p y + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE T*
copy_scale_n (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         y[i] = a * (x[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         y[0] = a * x[0];
         y[1] = a * x[1];
         y[2] = a * x[2];
         y[3] = a * x[3];
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] = a * x[0];
   else if (n == 2)
      vecvec::copy_scale2 (a, x, y);
   else if (n == 3)
      vecvec::copy_scale3 (a, x, y);

   return y + n;
}

/**
   Element-wise sum with scaling. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] = a * x[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p y + \p n.
   */
template <class T, class IterX, class IterY>
INLINE IterY
copy_scale_n (T a, IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] = a * x[i];
   return y + n;
}

/**
   Element-wise sum with scaling. 
   Parallel version with pointers (no aliasing allowed).

   \verbatim    
      y[i] = a * x[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline T*
copy_scale_n_par (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         y[i] = a * (x[i]);
   }
   else
#endif
      vecvec::copy_scale_n(a, x, y, n);

   return y + n;
}



/**
   Element-wise sum. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + y[i], for i = 0, 1  
   \endverbatim

   \return     The iterator \p z + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_add2 (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = x[0] + y[0];
   z[1] = x[1] + y[1];
   return z + 2;
}

/**
   Element-wise sum. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + y[i], for i = 0, 1, 2  
   \endverbatim

   \return     The iterator \p z + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_add3 (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = x[0] + y[0];
   z[1] = x[1] + y[1];
   z[2] = x[2] + y[2];
   return z + 3;
}

/**
   Element-wise sum. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + y[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE T*
copy_add_n (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         z[i] = x[i] + y[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         z[0] = x[0] + y[0];
         z[1] = x[1] + y[1];
         z[2] = x[2] + y[2];
         z[3] = x[3] + y[3];
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      z[0] = x[0] + y[0];
   else if (n == 2)
      vecvec::copy_add2(x, y, z);
   else if (n == 3)
      vecvec::copy_add3(x, y, z);

   return z + n;
}

/**
   Element-wise sum. 
   Serial version with iterators (may alias).

   \verbatim    
      z[i] = x[i] + y[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   */
template <class IterX, class IterY, class IterZ>
INLINE IterZ
copy_add_n (IterX x, IterY y, IterZ z, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      z[i] = x[i] + y[i];
   return z + n;
}

/**
   Element-wise sum. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + y[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline T*
copy_add_n_par (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         z[i] = x[i] + y[i];
   }
   else
#endif
      vecvec::copy_add_n(x, y, z, n);

   return z + n;
}



/**
   Element-wise sum with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] + y[i]), for i = 0, 1  
   \endverbatim

   \return     The iterator \p z + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_add2 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = a * (x[0] + y[0]);
   z[1] = a * (x[1] + y[1]);
   return z + 2;
}

/**
   Element-wise sum with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] + y[i]), for i = 0, 1, 2  
   \endverbatim

   \return     The iterator \p z + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_add3 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = a * (x[0] + y[0]);
   z[1] = a * (x[1] + y[1]);
   z[2] = a * (x[2] + y[2]);
   return z + 3;
}

/**
   Element-wise sum with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] + y[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE T*
copy_add_n (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         z[i] = a * (x[i] + y[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         z[0] = a * (x[0] + y[0]);
         z[1] = a * (x[1] + y[1]);
         z[2] = a * (x[2] + y[2]);
         z[3] = a * (x[3] + y[3]);
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      z[0] = a * (x[0] + y[0]);
   else if (n == 2)
      vecvec::copy_add2(a, x, y, z);
   else if (n == 3)
      vecvec::copy_add3(a, x, y, z);

   return z + n;
}

/**
   Element-wise sum with scaling. 
   Serial version with iterators (may alias).

   \verbatim    
      z[i] = a * (x[i] + y[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   */
template <class T, class IterX, class IterY, class IterZ>
INLINE IterZ
copy_add_n (T a, IterX x, IterY y, IterZ z, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      z[i] = a * (x[i] + y[i]);

   return z + n;
}

/**
   Element-wise sum with scaling. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] + y[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline T*
copy_add_n_par (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n, int DUM_OMP(nthreads))
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         z[i] = a * (x[i] + y[i]);
   }
   else
#endif
      vecvec::copy_add_n(a, x, y, z, n);

   return z + n;
}



/**
   Element-wise sum with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      w[i] = a * (x[i] + y[i] + z[i]), for i = 0, 1  
   \endverbatim

   \return     The iterator \p w + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_add2 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, const T* CM2_RESTRICT z, T* CM2_RESTRICT w) 
{
   w[0] = a * ((x[0] + y[0]) + z[0]);
   w[1] = a * ((x[1] + y[1]) + z[1]);
   return w + 2;
}

/**
   Element-wise sum with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      w[i] = a * (x[i] + y[i] + z[i]), for i = 0, 1, 2  
   \endverbatim

   \return     The iterator \p w + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_add3 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, const T* CM2_RESTRICT z, T* CM2_RESTRICT w) 
{
   w[0] = a * ((x[0] + y[0]) + z[0]);
   w[1] = a * ((x[1] + y[1]) + z[1]);
   w[2] = a * ((x[2] + y[2]) + z[2]);
   return w + 3;
}

/**
   Element-wise sum with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      w[i] = a * (x[i] + y[i] + z[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p w + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T, class IterX, class IterY, class IterZ, class IterW>
INLINE IterW
copy_add_n (T a, IterX x, IterY y, IterZ z, IterW w, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         w[i] = a * ((x[i] + y[i]) + z[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         w[0] = a * ((x[0] + y[0]) + z[0]);
         w[1] = a * ((x[1] + y[1]) + z[1]);
         w[2] = a * ((x[2] + y[2]) + z[2]);
         w[3] = a * ((x[3] + y[3]) + z[3]);
         x += 4;
         y += 4;
         z += 4;
         w += 4;
      }
#endif
   }

   if (n == 1)
      w[0] = a * ((x[0] + y[0]) + z[0]);
   else if (n == 2)
      vecvec::copy_add2(a, x, y, z, w);
   else if (n == 3)
      vecvec::copy_add3(a, x, y, z, w);

   return w + n;
}

/**
   Element-wise sum with scaling. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      w[i] = a * (x[i] + y[i] + z[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p w + \p n.
   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T, class IterX, class IterY, class IterZ, class IterW>
inline T*
copy_add_n_par (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, const T* CM2_RESTRICT z, T* CM2_RESTRICT w, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         w[i] = a * ((x[i] + y[i]) + z[i]);
   }
   else
#endif
      vecvec::copy_add_n(a, x, y, z, w, n);

   return w + n;
}



/**
   Element-wise difference. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] - y[i], for i = 0, 1 
   \endverbatim

   \return     The iterator \p z + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_diff2 (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = x[0] - y[0];
   z[1] = x[1] - y[1];
   return z + 2;
}

/**
   Element-wise difference. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] - y[i], for i = 0, 1, 2 
   \endverbatim

   \return     The iterator \p z + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_diff3 (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = x[0] - y[0];
   z[1] = x[1] - y[1];
   z[2] = x[2] - y[2];
   return z + 3;
}

/**
   Element-wise difference. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] - y[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE T*
copy_diff_n (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         z[i] = x[i] - y[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         z[0] = x[0] - y[0];
         z[1] = x[1] - y[1];
         z[2] = x[2] - y[2];
         z[3] = x[3] - y[3];
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      z[0] = x[0] - y[0];
   else if (n == 2)
      vecvec::copy_diff2(x, y, z);
   else if (n == 3)
      vecvec::copy_diff3(x, y, z);

   return z + n;
}

/**
   Element-wise difference. 
   Serial version with iterators (may alias).

   \verbatim    
      z[i] = x[i] - y[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   */
template <class IterX, class IterY, class IterZ>
INLINE IterZ
copy_diff_n (IterX x, IterY y, IterZ z, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      z[i] = x[i] - y[i];
   return z + n;
}

/**
   Element-wise difference. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] - y[i], for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline T*
copy_diff_n_par (const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         z[i] = x[i] - y[i];
   }
   else
#endif
      vecvec::copy_diff_n(x, y, z, n);

   return z + n;
}



/**
   Element-wise difference with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] - y[i]), for i = 0, 1  
   \endverbatim

   \return     The iterator \p z + 2.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_diff2 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = a * (x[0] - y[0]);
   z[1] = a * (x[1] - y[1]);
   return z + 2;
}

/**
   Element-wise difference with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] - y[i]), for i = 0, 1, 2  
   \endverbatim

   \return     The iterator \p z + 3.
   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE T*
copy_diff3 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = a * (x[0] - y[0]);
   z[1] = a * (x[1] - y[1]);
   z[2] = a * (x[2] - y[2]);
   return z + 3;
}

/**
   Element-wise difference with scaling. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] - y[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE T*
copy_diff_n (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         z[i] = a * (x[i] - y[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         z[0] = a * (x[0] - y[0]);
         z[1] = a * (x[1] - y[1]);
         z[2] = a * (x[2] - y[2]);
         z[3] = a * (x[3] - y[3]);
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      z[0] = a * (x[0] - y[0]);
   else if (n == 2)
      vecvec::copy_diff2(a, x, y, z);
   else if (n == 3)
      vecvec::copy_diff3(a, x, y, z);

   return z + n;
}

/**
   Element-wise difference with scaling. 
   Serial version with iterators (may alias).

   \verbatim    
      z[i] = a * (x[i] - y[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   */
template <class T, class IterX, class IterY, class IterZ>
INLINE IterZ
copy_diff_n (T a, IterX x, IterY y, IterZ z, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      z[i] = a * (x[i] - y[i]);
   return z + n;
}

/**
   Element-wise difference with scaling. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = a * (x[i] - y[i]), for i = 0...n-1  
   \endverbatim

   \return     The iterator \p z + \p n.
   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline T*
copy_diff_n_par (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n, int DUM_OMP(nthreads))
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         z[i] = a * (x[i] - y[i]);
   }
   else
#endif
      vecvec::copy_diff_n(a, x, y, z, n);

   return z + n;
}



/**
   Element-wise add. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += x[i], for i = 0, 1 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
add2 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] += x[0];
   y[1] += x[1];
}

/**
   Element-wise add. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += x[i], for i = 0, 1, 2 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
add3 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] += x[0];
   y[1] += x[1];
   y[2] += x[2];
}

/**
   Element-wise add. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
add_n (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         y[i] += x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         y[0] += x[0];
         y[1] += x[1];
         y[2] += x[2];
         y[3] += x[3];
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] += x[0];
   else if (n == 2)
      vecvec::add2(x, y);
   else if (n == 3)
      vecvec::add3(x, y);
}

/**
   Element-wise add. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] += x[i], for i = 0..n-1
   \endverbatim
   */
template <class IterX, class IterY>
INLINE void
add_n (IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] += x[i];
}

/**
   Element-wise add. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
add_n_par (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads))
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         y[i] += x[i];
   }
   else
#endif
      vecvec::add_n(x, y, n);
}



/**
   Element-wise subtract. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] -= x[i], for i = 0, 1 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
subtract2 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] -= x[0];
   y[1] -= x[1];
}

/**
   Element-wise subtract. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] -= x[i], for i = 0, 1, 2 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
subtract3 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] -= x[0];
   y[1] -= x[1];
   y[2] -= x[2];
}

/**
   Element-wise subtract. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] -= x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
subtract_n (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         y[i] -= x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         y[0] -= x[0];
         y[1] -= x[1];
         y[2] -= x[2];
         y[3] -= x[3];
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] -= x[0];
   else if (n == 2)
      vecvec::subtract2(x, y);
   else if (n == 3)
      vecvec::subtract3(x, y);
}

/**
   Element-wise subtract. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] -= x[i], for i = 0..n-1
   \endverbatim
   */
template <class IterX, class IterY>
INLINE void
subtract_n (IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] -= x[i];
}

/**
   Element-wise subtract. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      y[i] -= x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
subtract_n_par (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads))
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         y[i] -= x[i];
   }
   else
#endif
      vecvec::subtract_n(x, y, n);
}



/**
   Element-wise mult. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] *= x[i], for i = 0, 1 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
mult2 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] *= x[0];
   y[1] *= x[1];
}

/**
   Element-wise mult. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] *= x[i], for i = 0, 1, 2 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
mult3 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] *= x[0];
   y[1] *= x[1];
   y[2] *= x[2];
}

/**
   Element-wise mult. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] *= x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
mult_n (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         y[i] *= x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         y[0] *= x[0];
         y[1] *= x[1];
         y[2] *= x[2];
         y[3] *= x[3];
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] *= x[0];
   else if (n == 2)
      vecvec::mult2(x, y);
   else if (n == 3)
      vecvec::mult3(x, y);
}

/**
   Element-wise mult. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] *= x[i], for i = 0..n-1
   \endverbatim
   */
template <class IterX, class IterY>
INLINE void
mult_n (IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] *= x[i];
}

/**
   Element-wise mult. 
   Parallel version with pointers (no aliasing allowed).

   \verbatim    
      y[i] *= x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
mult_n_par (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads))
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         y[i] *= x[i];
   }
   else
#endif
      vecvec::mult_n(x, y, n);
}



/**
   Element-wise div. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] /= x[i], for i = 0, 1 
   \endverbatim

   \pre        x[i] is not null for i = 0, 1
   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
div2 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] /= x[0];
   y[1] /= x[1];
}

/**
   Element-wise div. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] /= x[i], for i = 0, 1, 2 
   \endverbatim

   \pre        x[i] is not null for i = 0, 1, 2
   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
div3 (const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] /= x[0];
   y[1] /= x[1];
   y[2] /= x[2];
}

/**
   Element-wise div. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] /= x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
div_n (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         y[i] /= x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         y[0] /= x[0];
         y[1] /= x[1];
         y[2] /= x[2];
         y[3] /= x[3];
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] /= x[0];
   else if (n == 2)
      vecvec::div2(x, y);
   else if (n == 3)
      vecvec::div3(x, y);
}

/**
   Element-wise div. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] /= x[i], for i = 0..n-1
   \endverbatim
   */
template <class IterX, class IterY>
INLINE void
div_n (IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] /= x[i];
}

/**
   Element-wise div. 
   Parallel version with pointers (no aliasing allowed).

   \verbatim    
      y[i] /= x[i], for i = 0..n-1
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
div_n_par (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads))
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         y[i] /= x[i];
   }
   else
#endif
      vecvec::div_n(x, y, n);
}



/**
   Element-wise AXPY. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * x[i], for i = 0, 1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE void
axpy2 (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] += a * x[0];
   y[1] += a * x[1];
}

/**
   Element-wise AXPY. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * x[i], for i = 0, 1, 2  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE void
axpy3 (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] += a * x[0];
   y[1] += a * x[1];
   y[2] += a * x[2];
}

/**
   Element-wise AXPY. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * x[i], for i = 0...n-1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
axpy_n (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         y[i] += a * x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         y[0] += a * x[0];
         y[1] += a * x[1];
         y[2] += a * x[2];
         y[3] += a * x[3];
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] += a * x[0];
   else if (n == 2)
      vecvec::axpy2(a, x, y);
   else if (n == 3)
      vecvec::axpy3(a, x, y);
}

/**
   Element-wise AXPY. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] += a * x[i], for i = 0...n-1  
   \endverbatim
   */
template <class T, class IterX, class IterY>
INLINE void
axpy_n (T a, IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] += a * x[i];
}

/**
   Element-wise AXPY. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * x[i], for i = 0...n-1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
axpy_n_par (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         y[i] += a * x[i];
   }
   else
#endif
      vecvec::axpy_n(a, x, y, n);
}



/**
   Element-wise AXYPZ. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] += a * (x[i] * y[i]), for i = 0, 1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE void
axypz2 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] += a * (x[0] * y[0]);
   z[1] += a * (x[1] * y[1]);
}

/**
   Element-wise AXYPZ. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] += a * (x[i] * y[i]), for i = 0, 1, 2  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE void
axypz3 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] += a * (x[0] * y[0]);
   z[1] += a * (x[1] * y[1]);
   z[2] += a * (x[2] * y[2]);
}

/**
   Element-wise AXYPZ. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] += a * (x[i] * y[i]), for i = 0...n-1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
axypz_n (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         z[i] += a * (x[i] * y[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         z[0] += a * (x[0] * y[0]);
         z[1] += a * (x[1] * y[1]);
         z[2] += a * (x[2] * y[2]);
         z[3] += a * (x[3] * y[3]);
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      z[0] += a * (x[0] * y[0]);
   else if (n == 2)
      vecvec::axypz2(a, x, y, z);
   else if (n == 3)
      vecvec::axypz3(a, x, y, z);
}

/**
   Element-wise AXYPZ. 
   Serial version with iterators (may alias).

   \verbatim    
      z[i] += a * (x[i] * y[i]), for i = 0...n-1  
   \endverbatim
   */
template <class T, class IterX, class IterY, class IterZ>
INLINE void
axypz_n (T a, IterX x, IterY y, IterZ z, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      z[i] += a * (x[i] * y[i]);
}

/**
   Element-wise AXYPZ. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      z[i] += a * (x[i] * y[i]), for i = 0...n-1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
axypz_n_par (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         z[i] += a * (x[i] * y[i]);
   }
   else
#endif
      vecvec::axypz_n(a, x, y, z, n);
}



/**
   Element-wise interpolation. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + a * (y[i]-x[i]), for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
interpol2 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = x[0] + a * (y[0] - x[0]);
   z[1] = x[1] + a * (y[1] - x[1]);
}

/**
   Element-wise interpolation. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + a * (y[i]-x[i]), for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
interpol3 (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z) 
{
   z[0] = x[0] + a * (y[0] - x[0]);
   z[1] = x[1] + a * (y[1] - x[1]);
   z[2] = x[2] + a * (y[2] - x[2]);
}

/**
   Element-wise interpolation. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + a * (y[i]-x[i]), for i = 0...n-1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
interpol_n (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         z[i] = x[i] + a * (y[i] - x[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         z[0] = x[0] + a * (y[0] - x[0]);
         z[1] = x[1] + a * (y[1] - x[1]);
         z[2] = x[2] + a * (y[2] - x[2]);
         z[3] = x[3] + a * (y[3] - x[3]);
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      z[0] = x[0] + a * (y[0] - x[0]);
   else if (n == 2)
      vecvec::interpol2(a, x, y, z);
   else if (n == 3)
      vecvec::interpol3(a, x, y, z);
}

/**
   Element-wise interpolation. 
   Serial version with iterators (may alias).

   \verbatim    
      z[i] = x[i] + a * (y[i]-x[i]), for i = 0...n-1  
   \endverbatim
   */
template <class T, class IterX, class IterY, class IterZ>
INLINE void
interpol_n (T a, IterX x, IterY y, IterZ z, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      z[i] = x[i] + a * (y[i] - x[i]);
}

/**
   Element-wise interpolation. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      z[i] = x[i] + a * (y[i]-x[i]), for i = 0...n-1  
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
interpol_n_par (T a, const T* CM2_RESTRICT x, const T* CM2_RESTRICT y, T* CM2_RESTRICT z, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         z[i] = x[i] + a * (y[i] - x[i]);
   }
   else
#endif
      vecvec::interpol_n(a, x, y, z, n);
}



/**
   Dot product. 
   Serial version.

   \verbatim    
      s = sum (x[i] * y[i]), for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
dot2 (IterX x, IterY y) 
{
   return x[0]*y[0] + x[1]*y[1]; 
}

/**
   Dot product. 
   Serial version.

   \verbatim    
      s = sum (x[i] * y[i]), for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
dot3 (IterX x, IterY y) 
{
   return (x[0]*y[0] + x[1]*y[1]) + x[2]*y[2]; 
}

/**
   Dot product. 
   Serial version.

   \verbatim    
      s = sum (x[i] * y[i]), for i = 0,..n-1  
   \endverbatim
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
dot_n (IterX x, IterY y, size_t n) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += x[i] * y[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s += (x[0]*y[0] + x[1]*y[1]) + (x[2]*y[2] + x[3]*y[3]); 
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      s += x[0]*y[0];
   else if (n == 2)
      s += vecvec::dot2(x, y);
   else if (n == 3)
      s += vecvec::dot3(x, y);

   return s;
}

/**
   Dot product. Parallel version.

   \verbatim    
      s = sum (x[i] * y[i]), for i = 0,..n-1  
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
dot_n_par (IterX x, IterY y, size_t n, int DUM_OMP(nthreads)) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += x[i] * y[i];
   }
   else
#endif
      s = vecvec::dot_n(x, y, n);

   return s;
}


/**
   Dot product between three vectors. 
   Serial version.

   \verbatim    
      s = sum (x[i] * y[i] * z[i]), for i = 0, 1.
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
dot2 (IterX x, IterY y, IterZ z) 
{
   return x[0]*y[0]*z[0] + x[1]*y[1]*z[1]; 
}

/**
   Dot product between three vectors. 
   Serial version.

   \verbatim    
      s = sum (x[i] * y[i] * z[i]), for i = 0, 1, 2.
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
dot3 (IterX x, IterY y, IterZ z) 
{
   return (x[0]*y[0]*z[0] + x[1]*y[1]*z[1]) + x[2]*y[2]*z[2]; 
}

/**
   Dot product between three vectors. 
   Serial version.

   \verbatim    
      s = sum (x[i] * y[i] * z[i]), for i = 0,..n-1  
   \endverbatim
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
dot_n (IterX x, IterY y, IterZ z, size_t n) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += x[i] * y[i] * z[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s += (x[0]*y[0]*z[0] + x[1]*y[1]*z[1]) + (x[2]*y[2]*z[2] + x[3]*y[3]*z[3]); 
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      s += x[0]*y[0]*z[0];
   else if (n == 2)
      s += vecvec::dot2(x, y, z);
   else if (n == 3)
      s += vecvec::dot3(x, y, z);

   return s;
}

/**
   Dot product between three vectors. Parallel version.

   \verbatim    
      s = sum (x[i] * y[i] * z[i]), for i = 0,..n-1  
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
dot_n_par (IterX x, IterY y, IterZ z, size_t n, int DUM_OMP(nthreads)) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename std::iterator_traits<IterZ>::value_type   TZ;
   typedef typename cm2::biggest3_type<TX, TY, TZ>::type      T;

   T     s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += x[i] * y[i] * z[i];
   }
   else
#endif
      s = vecvec::dot_n(x, y, z, n);

   return s;
}


/**
   Hermitian inner (dot) product. 
   Serial version.

   \verbatim    
      s = sum(conj(x[i]) * y[i]), for i = 0, 1.
      x is complex, s is complex.
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
Herm_dot2 (IterX x, IterY y) 
{
   return std::conj(x[0])*y[0] + std::conj(x[1])*y[1]; 
}

/**
   Hermitian inner (dot) product. 
   Serial version.

   \verbatim    
      s = sum(conj(x[i]) * y[i]), for i = 0, 1, 2. 
      x is complex, s is complex.
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
Herm_dot3 (IterX x, IterY y) 
{
   return (std::conj(x[0])*y[0] + std::conj(x[1])*y[1]) + std::conj(x[2])*y[2];
}

/**
   Hermitian inner (dot) product. 
   Serial version.

   \verbatim    
      s = sum(conj(x[i]) * y[i]), for i = 0,..n-1.
      x is complex, s is complex.
   \endverbatim
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
Herm_dot_n (IterX x, IterY y, size_t n) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += std::conj(x[i]) * y[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s += std::conj(x[0])*y[0];
         s += std::conj(x[1])*y[1];
         s += std::conj(x[2])*y[2];
         s += std::conj(x[3])*y[3];
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      s += std::conj(x[0])*y[0];
   else if (n == 2)
      s += vecvec::Herm_dot2(x, y);
   else if (n == 3)
      s += vecvec::Herm_dot3(x, y);

   return s;
}

/**
   Hermitian inner (dot) product. Parallel version.

   \verbatim    
      s = sum(conj(x[i]) * y[i]), for i = 0,..n-1  
      x is complex, s is complex.
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
Herm_dot_n_par (IterX x, IterY y, size_t n, int DUM_OMP(nthreads)) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += std::conj(x[i])*y[i];
   }
   else
#endif
      s = vecvec::Herm_dot_n(x, y, n);

   return s;
}


/**
   Hermitian AXPY. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * conj(x[i]), for i = 0, 1.
      x and y are complexes.
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE void
Herm_axpy2 (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] += a * std::conj(x[0]);
   y[1] += a * std::conj(x[1]);
}

/**
   Hermitian AXPY. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * conj(x[i]), for i = 0, 1, 2.
      x and y are complexes.
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       This specialization makes the optimizer's job easier.
   */
template <class T>
INLINE void
Herm_axpy3 (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y) 
{
   y[0] += a * std::conj(x[0]);
   y[1] += a * std::conj(x[1]);
   y[2] += a * std::conj(x[2]);
}

/**
   Hermitian AXPY. 
   Serial version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * conj(x[i]), for i = 0...n-1.
      x and y are complexes.
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   */
template <class T>
INLINE void
Herm_axpy_n (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         y[i] += a * std::conj(x[i]);
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         y[0] += a * std::conj(x[0]);
         y[1] += a * std::conj(x[1]);
         y[2] += a * std::conj(x[2]);
         y[3] += a * std::conj(x[3]);
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
      y[0] += a * std::conj(x[0]);
   else if (n == 2)
      vecvec::Herm_axpy2(a, x, y);
   else if (n == 3)
      vecvec::Herm_axpy3(a, x, y);
}

/**
   Hermitian AXPY. 
   Serial version with iterators (may alias).

   \verbatim    
      y[i] += a * conj(x[i]), for i = 0...n-1. 
      x and y are complexes.
   \endverbatim
   */
template <class T, class IterX, class IterY>
INLINE void
Herm_axpy_n (T a, IterX x, IterY y, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      y[i] += a * std::conj(x[i]);
}

/**
   Hermitian AXPY. Parallel version with pointers (no aliasing allowed).

   \verbatim    
      y[i] += a * conj(x[i]), for i = 0...n-1.  
      x and y are complexes.
   \endverbatim

   \warning    No aliasing/overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline void
Herm_axpy_n_par (T a, const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         y[i] += a * std::conj(x[i]);
   }
   else
#endif
      vecvec::Herm_axpy_n(a, x, y, n);
}



/**
   Dot product between the difference between two vectors and a third vector. 
   Serial version.

   \verbatim    
      d = sum ((x[i]-y[i])*z[i]), for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
diff_dot2 (IterX x, IterY y, IterZ z) 
{ 
   typedef typename std::iterator_traits<IterX>::value_type    TX;
   typedef typename std::iterator_traits<IterY>::value_type    TY;
   typedef typename std::iterator_traits<IterZ>::value_type    TZ;
   typedef typename cm2::biggest3_type<TX,TY,TZ>::type         T;
  
   T     t[2];

   t[0] = x[0] - y[0];
   t[1] = x[1] - y[1];

   return t[0]*z[0] + t[1]*z[1];
}

/**
   Dot product between a vector and the difference between two vectors. 
   Serial version.

   \verbatim    
      d = sum ((x[i]-y[i])*z[i]), for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
diff_dot3 (IterX x, IterY y, IterZ z) 
{
   typedef typename std::iterator_traits<IterX>::value_type    TX;
   typedef typename std::iterator_traits<IterY>::value_type    TY;
   typedef typename std::iterator_traits<IterZ>::value_type    TZ;
   typedef typename cm2::biggest3_type<TX,TY,TZ>::type         T;
  
   T     t[3];

   t[0] = x[0] - y[0];
   t[1] = x[1] - y[1];
   t[2] = x[2] - y[2];

   return (t[0]*z[0] + t[1]*z[1]) + t[2]*z[2];
}

/**
   Dot product between a vector and the difference between two vectors. 
   Serial version.

   \verbatim    
      d = sum ((x[i]-y[i])*z[i]), for i = 0..n-1 
   \endverbatim
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
diff_dot_n (IterX x, IterY y, IterZ z, size_t n) 
{
   typedef typename std::iterator_traits<IterX>::value_type    TX;
   typedef typename std::iterator_traits<IterY>::value_type    TY;
   typedef typename std::iterator_traits<IterZ>::value_type    TZ;
   typedef typename cm2::biggest3_type<TX,TY,TZ>::type         T;

   T     s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += (x[i] - y[i]) * z[i]; 
#else
      T              t[4];
      const size_t   n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         t[0] = x[0] - y[0];
         t[1] = x[1] - y[1];
         t[2] = x[2] - y[2]; 
         t[3] = x[3] - y[3]; 
         s += (t[0]*z[0] + t[1]*z[1]) + (t[2]*z[2] + t[3]*z[3]);
         x += 4;
         y += 4;
         z += 4;
      }
#endif
   }

   if (n == 1)
      s += (x[0] - y[0]) * z[0];
   else if (n == 2)
      s += vecvec::diff_dot2(x, y, z);
   else if (n == 3)
      s += vecvec::diff_dot3(x, y, z);

   return s;
}

/**
   Dot product between a vector and the difference between two vectors. Parallel version.

   \verbatim    
      d = sum ((x[i]-y[i])*z[i]), for i = 0..n-1 
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
diff_dot_n_par (IterX x, IterY y, IterZ z, size_t n, int DUM_OMP(nthreads)) 
{
   typedef typename std::iterator_traits<IterX>::value_type    TX;
   typedef typename std::iterator_traits<IterY>::value_type    TY;
   typedef typename std::iterator_traits<IterZ>::value_type    TZ;
   typedef typename cm2::biggest3_type<TX,TY,TZ>::type         T;

   T     s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += (x[i] - y[i]) * z[i]; 
   }
   else
#endif
      s = vecvec::diff_dot_n(x, y, z, n);
 
   return s;
}



/**
   Square of two-norm of difference between two vectors. 
   Serial version.

   \verbatim     
      s = sum((x[i]-y[i])*(x[i]-y[i])), for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
diff_sqr_two_norm2 (IterX x, IterY y) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     t[2];

   t[0] = x[0] - y[0];
   t[1] = x[1] - y[1];

   return t[0]*t[0] + t[1]*t[1];
}

/**
   Square of two-norm of difference between two vectors. 
   Serial version.

   \verbatim     
      s = sum((x[i]-y[i])*(x[i]-y[i])), for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
diff_sqr_two_norm3 (IterX x, IterY y) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     t[3];

   t[0] = x[0] - y[0];     
   t[1] = x[1] - y[1];
   t[2] = x[2] - y[2];

   return (t[0]*t[0] + t[1]*t[1]) + t[2]*t[2];
}

/**
   Square of two-norm of difference between two vectors. 
   Serial version.

   \verbatim    
      s = sum((x[i]-y[i])*(x[i]-y[i])), for i = 0..n-1
   \endverbatim
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
diff_sqr_two_norm_n (IterX x, IterY y, size_t n) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      T              t;
      for (size_t i = 0; i < n; ++i)
      {
         t = x[i] - y[i];    
         s += t * t;
      }
#else
      const size_t   n4 = n / 4;
      T              t[4];
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         t[0] = x[0] - y[0];     
         t[1] = x[1] - y[1];
         t[2] = x[2] - y[2];
         t[3] = x[3] - y[3];
         s += (t[0]*t[0] + t[1]*t[1]) + (t[2]*t[2] + t[3]*t[3]);
         x += 4;
         y += 4;
      }
#endif
   }

   if (n == 1)
   {
      const T  t = x[0] - y[0];     
      s += t*t;
   }
   else if (n == 2)
      s += vecvec::diff_sqr_two_norm2(x, y);
   else if (n == 3)
      s += vecvec::diff_sqr_two_norm3(x, y);

   return s;
}

/**
   Square of two-norm of difference between two vectors. Parallel version.

   \verbatim    
      s = sum ((x[i]-y[i])*(x[i]-y[i])), for i = 0..n-1
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
diff_sqr_two_norm_n_par (IterX x, IterY y, size_t n, int DUM_OMP(nthreads))
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
      {
         const T  t = x[i] - y[i];     
         s += t * t;
      }
   }
   else
#endif
      s = vecvec::diff_sqr_two_norm_n(x, y, n);

   return s;
}



/**
   Returns the two-norm of the difference between arrays [\p x, \p x + 2[ and [\p y, \p y + 2[.

   \verbatim    
      sqrt (sum ((x[i]-y[i])*(x[i]-y[i])), for i = 0, 1  
   \endverbatim

   \pre        For `double` arrays only.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
diff_two_norm2 (IterX x, IterY y) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s;

   s = vecvec::diff_sqr_two_norm2(x, y);
   s = ::sqrt(s);

   return s;
}

/**
   Returns the two-norm of the difference between arrays [\p x, \p x + 3[ and [\p y, \p y + 3[.

   \verbatim    
      sqrt (sum ((x[i]-y[i])*(x[i]-y[i])), for i = 0, 1, 2  
   \endverbatim

   \pre        For `double` arrays only.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
diff_two_norm3 (IterX x, IterY y) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s;

   s = vecvec::diff_sqr_two_norm3(x, y);
   s = ::sqrt(s);

   return s;
}


/**
   Two-norm of the difference between two vectors.

   \verbatim    
      sqrt (sum ((x[i]-y[i])*(x[i]-y[i]))), for i = 0...n-1  
   \endverbatim

   \pre     For `double` arrays only.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
diff_two_norm_n (IterX x, IterY y, size_t n) 
{
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T     s;

   s = vecvec::diff_sqr_two_norm_n(x, y, n);
   s = ::sqrt(s);

   return s;
}



/// Tests the equality between arrays [\p x, \p x + 2[ and [\p y, \p y + 2[.
template <class IterX, class IterY>
INLINE bool
equal2 (IterX x, IterY y) 
{
   return (x[0] == y[0]) && (x[1] == y[1]);
}

/// Tests the equality between arrays [\p x, \p x + 3[ and [\p y, \p y + 3[.
template <class IterX, class IterY>
INLINE bool
equal3 (IterX x, IterY y) 
{
   return (x[0] == y[0]) && (x[1] == y[1]) && (x[2] == y[2]);
}

/// Tests the equality between arrays [\p x, \p x + \p n[ and [\p y, \p y + \p n[.
template <class IterX, class IterY>
INLINE bool
equal_n (IterX x, IterY y, size_t n) 
{
   bool     result(true);

   for (size_t i = 0; (i < n) && result; ++i) 
      result = (x[i] == y[i]);

   return result;
}



/**
   Mult Sum. 
   Serial version.

   \verbatim
      s += y[i] * (y[i] * x[i])
      y[i] *= x[i]
   \endverbatim

   \note    No overlapping allowed.
   \note    When vectors are long enough (size > CM2_OMP_MIN_LOOP), the computation
            is done in parallel (if OMP is enabled).
   */
template <class T>
INLINE T
mult_sum_n (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n) 
{
   T     s(0);

   for (size_t i = 0; i < n; ++i) 
   {
      const T  t = y[i];
      y[i] *= x[i];
      s += t * y[i];
   }

   return s;
}


/**
   Mult Sum. Parallel version.

   \verbatim
      s += y[i] * (y[i] * x[i])
      y[i] *= x[i]
   \endverbatim

   \note       No overlapping allowed.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class T>
inline T
mult_sum_n_par (const T* CM2_RESTRICT x, T* CM2_RESTRICT y, size_t n, int DUM_OMP(nthreads)) 
{
   T     s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
      {
         const T  t = y[i];
         y[i] *= x[i];
         s += t * y[i];
      }
   }
   else
#endif
      s = vecvec::mult_sum_n(x, y, n);

   return s;
}

//@}






/** \defgroup VecVecGroup Vector-Vector Template Functions */
//@{

/**
   Appends a vector to another vector.

   \param[in]     V1    The source vector to be appended to V2.
   \param[in,out] V2    The target vector.
   \return        Returns true if the operation succeed. 
                  False in case of memory allocation error.
   \warning       The vectors should not overlap.
   */
template <class Vec1, class Vec2>
bool 
push_back (const Vec1& V1, Vec2& V2)  
{ 
   const size_t                  S1(V1.size());
   const size_t                  S2(V2.size());
   const typename Vec1::const_iterator p1(V1.begin());
   typename Vec2::iterator       p2;
   bool                          ok(true);

   // Fast return.
   if (S1 == 0)
      return true;

   if (S1 + S2 > V2.capacity())
      ok = V2.reserve(S2 + std::max(S1,S2));          // Double the capacity of the vector.

   ok &= V2.resize(S1 + S2);

   if (ok)
   {
      p2 = V2.begin() + S2;
      vecvec::copy_n(p1, p2, S1);
   }

   return ok;
}   


/**
   Appends a specific batch of elements to a vector.

   \param[in]     V1          The source vector.
   \param[in]     V1_indices  The indices of the values in \p V1 to be copied.
                              Must all refer to valid IDs in vector \p V1.
   \param[in,out] V2          The target vector.
   \return        Returns true if the operation succeed. 
                  False in case of memory allocation error.

   \pre           All the values in \p V1_indices are smaller than \p V1.size(). 
   \warning       Multiple indices cause multiple copies.
   */
template <class Vec1, class Vec2, class Vector>
bool 
push_back (const Vec1& V1, const Vector& V1_indices, Vec2& V2)  
{
   const size_t                        N1  = V1_indices.size();
   const size_t                        N2  = V2.size();
   typename Vec1::const_iterator       p1;
   typename Vec2::iterator             p2;
   typename Vector::const_iterator     pj;
   bool                                ok;

   if (N2 + N1 > V2.capacity())
      V2.reserve(N2 + std::max(N1,N2));           // At least double the capacity of V2.

   ok = V2.resize(N2 + N1);

   if (ok)
   {
      p1 = V1.begin();
      p2 = V2.begin() + N2;   
      pj = V1_indices.begin();
      for (size_t j = 0; j < N1; ++j, ++pj, ++p2)
      {
         assert (*pj < V1.size());
         *p2 = *(p1 + *pj);
      }
   }

   return ok;
}



/**
   Copies a vector to another.

   \verbatim     
      y[i] = x[i], i = 0..n-1  
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE void
copy (const VecX& X, VecY& Y)                      
{ 
   assert (X.size() == Y.size());
   vecvec::copy_n(X.begin(), Y.begin(), X.size()); 
}


/**
   Swap elements between two vectors.

   \verbatim     
      swap(x[i], y[i]), for i = 0 ... n-1 
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE void
swap (VecX& X, VecY& Y)                      
{ 
   assert (X.size() == Y.size());
   vecvec::swap_n(X.begin(), Y.begin(), X.size()); 
}


/**
   Copies the scaling of a vector to another.

   \verbatim     
      y[i] = a * x[i], i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class T, class VecX, class VecY>
INLINE void
copy_scale (T a, const VecX& X, VecY& Y)    
{ 
   assert (X.size() == Y.size());
   vecvec::copy_scale_n(a, X.begin(), Y.begin(), X.size()); 
}


/**
   Adds a vector to another.

   \verbatim     
      y[i] += x[i], i = 0..n-1 
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE void
add (const VecX& X, VecY& Y)                       
{ 
   assert (X.size() == Y.size());
   vecvec::add_n(X.begin(), Y.begin(), X.size()); 
}


/**
   Subtracts a vector from another.

   \verbatim     
      y[i] -= x[i], i = 0..n-1
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE void
subtract (const VecX& X, VecY& Y)                 
{ 
   assert (X.size() == Y.size());
   vecvec::subtract_n(X.begin(), Y.begin(), X.size()); 
}


/**
   Multiplies a vector by another (element by element).

   \verbatim     
      y[i] *= x[i], i = 0..n-1
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE void
mult (const VecX& X, VecY& Y)                      
{ 
   assert (X.size() == Y.size());
   vecvec::mult_n(X.begin(), Y.begin(), X.size()); 
}


/**
   Divides a vector by another (element by element).

   \verbatim     
      y[i] /= x[i], i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   \pre  x[i] is not null for i = 0...n-1
   */
template <class VecX, class VecY>
INLINE void
div (const VecX& X, VecY& Y)                       
{ 
   assert (X.size() == Y.size());
   vecvec::div_n(X.begin(), Y.begin(), X.size()); 
}


/**
   Multiplies two vectors and adds into a third (element by element).

   \verbatim     
      z[i] += x[i] * y[i], i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY, class VecZ>
INLINE void
mult (const VecX& X, const VecY& Y, VecZ& Z) 
{
   typedef typename VecX::value_type      T;

   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   vecvec::axypz_n(T(1), X.begin(), Y.begin(), Z.begin(), X.size());
}



/**
   Dot product between two vectors.

   \verbatim     
      sum (x[i] * y[i]), i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE typename cm2::biggest2_type<typename VecX::value_type,
                                   typename VecY::value_type>::type
dot (const VecX& X, const VecY& Y) 
{
   assert (X.size() == Y.size());
   return vecvec::dot_n(X.begin(), Y.begin(), X.size()); 
}


/**
   AXPY between two vectors.

   \verbatim     
      y[i] += a * x[i], i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class T, class VecX, class VecY>
INLINE void
axpy (T a, const VecX& X, VecY& Y) 
{
   assert (X.size() == Y.size());
   vecvec::axpy_n(a, X.begin(), Y.begin(), X.size()); 
}


/**
   Element-wise multiplication between three vectors.

   \verbatim     
      z[i] += a * x[i] * y[i], i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class T, class VecX, class VecY, class VecZ>
INLINE void
axypz (T a, const VecX& X, const VecY& Y, VecZ& Z) 
{
   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   vecvec::axypz_n(a, X.begin(), Y.begin(), Z.begin(), X.size());
}


/**
   Interpolation between two vectors.

   \verbatim     
      z[i] = x[i] + a * (y[i] - x[i]), i = 0..n-1   
   \endverbatim

   \pre  Vectors must have same size.
   */
template <class T, class VecX, class VecY, class VecZ>
INLINE void
interpol (T a, const VecX& X, const VecY& Y, VecZ& Z) 
{
   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   vecvec::interpol_n(a, X.begin(), Y.begin(), Z.begin(), X.size());
}


/**
   Sum of two vectors (element by element).

   \verbatim     
      z[i] = x[i] + y[i], i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY, class VecZ>
INLINE void
copy_add (const VecX& X, const VecY& Y, VecZ& Z) 
{
   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   vecvec::copy_add_n(X.begin(), Y.begin(), Z.begin(), X.size());
}


/**
   Sum with scaling of two vectors (element by element).

   \verbatim     
      z[i] = a * (x[i] + y[i]), i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class T, class VecX, class VecY, class VecZ>
INLINE void
copy_add (T a, const VecX& X, const VecY& Y, VecZ& Z) 
{
   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   vecvec::copy_add_n(a, X.begin(), Y.begin(), Z.begin(), X.size());
}


/**
   Difference between two vectors.

   \verbatim     
      z[i] = x[i] - y[i], i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY, class VecZ>
INLINE void
copy_diff (const VecX& X, const VecY& Y, VecZ& Z) 
{
   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   vecvec::copy_diff_n(X.begin(), Y.begin(), Z.begin(), X.size());
}


/**
   Difference with scaling between two vectors.

   \verbatim     
      z[i] = a * (x[i] - y[i]), i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class T, class VecX, class VecY, class VecZ>
INLINE void
copy_diff (T a, const VecX& X, const VecY& Y, VecZ& Z) 
{
   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   vecvec::copy_diff_n(a, X.begin(), Y.begin(), Z.begin(), X.size());
}


/**
   Returns the square of the two norm of the difference between two vectors.

   \verbatim     
      sum (x[i]-y[i], x[i]-y[i]), i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE typename cm2::biggest2_type<typename VecX::value_type,
                                   typename VecY::value_type>::type
diff_sqr_two_norm (const VecX& X, const VecY& Y) 
{ 
   assert (X.size() == Y.size());
   return vecvec::diff_sqr_two_norm_n(X.begin(), Y.begin(), X.size());
}


/**
   Returns the two norm of the difference between two vectors.
   Only the first 3 components of each vector are used.

   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
INLINE typename cm2::biggest2_type<typename VecX::value_type,
                                   typename VecY::value_type>::type
diff_two_norm (const VecX& X, const VecY& Y) 
{ 
   return sqrt(vecvec::diff_sqr_two_norm(X, Y));
}


/**
   Returns the dot product of the difference between two vectors and a third vector.

   \verbatim     
      sum (x[i]-y[i], z[i]), i = 0..n-1   
   \endverbatim
   \pre  Vectors must have same size.
   */
template <class VecX, class VecY, class VecZ>
INLINE typename cm2::biggest3_type<typename VecX::value_type,
                                   typename VecY::value_type,
                                   typename VecZ::value_type>::type
diff_dot (const VecX& X, const VecY& Y, const VecZ& Z) 
{ 
   assert (X.size() == Y.size());
   assert (X.size() == Z.size());
   return vecvec::diff_dot_n(X.begin(), Y.begin(), Z.begin(), X.size());
}


/**
   Tests the equality between two vectors.

   \note    If vectors \p x and \p y have different size, they are not equal.
   */
template <class VecX, class VecY>
INLINE bool
equal (const VecX& X, const VecY& Y) 
{
   return (X.size() == Y.size()) && vecvec::equal_n(X.begin(), Y.begin(), X.size()); 
}


/**
   Cross product between two vectors.

   \verbatim     
      z += x ^ y  
   \endverbatim
   \pre  Vectors must have size greater than or equal to 3.
   */
template <class VecX, class VecY, class VecZ>
INLINE void
cross (const VecX& x, const VecY& y, VecZ& z) 
{
   z[0] += x[1]*y[2] - x[2]*y[1];
   z[1] += x[2]*y[0] - x[0]*y[2];
   z[2] += x[0]*y[1] - x[1]*y[0];
}

/**
   Cross product between two vectors, with a scaling factor.
   Only the first 3 components of each vector are used.

   \verbatim    
      z += a * (x ^ y)  
   \endverbatim
   \pre  Vectors must have size greater than or equal to 3.
   */
template <class T, class VecX, class VecY, class VecZ>
INLINE void
cross (T a, const VecX& x, const VecY& y, VecZ& z) 
{
   z[0] += a * (x[1]*y[2] - x[2]*y[1]);
   z[1] += a * (x[2]*y[0] - x[0]*y[2]);
   z[2] += a * (x[0]*y[1] - x[1]*y[0]);
}

/**
   Cross product between two vectors.

   \verbatim    
      z = x ^ y  
   \endverbatim
   \pre  Vectors must have size greater than or equal to 3.
   */
template <class VecX, class VecY, class VecZ>
INLINE void
copy_cross (const VecX& x, const VecY& y, VecZ& z) 
{
   z[0] = x[1]*y[2] - x[2]*y[1];
   z[1] = x[2]*y[0] - x[0]*y[2];
   z[2] = x[0]*y[1] - x[1]*y[0];
}

/**
   Cross product between two vectors, with a scaling factor.

   \verbatim    
      z = a * (x ^ y)  
   \endverbatim
   \pre  Vectors must have size greater than or equal to 3.
   */
template <class T, class VecX, class VecY, class VecZ>
INLINE void
copy_cross (T a, const VecX& x, const VecY& y, VecZ& z) 
{
   z[0] = a * (x[1]*y[2] - x[2]*y[1]);
   z[1] = a * (x[2]*y[0] - x[0]*y[2]);
   z[2] = a * (x[0]*y[1] - x[1]*y[0]);
}

/**
   The cross product between two vectors (2-D version).

   Only the first 2 components of each vector are used.

   Computes the 3rd component of the cross product using the first two components.
   \pre  Vectors must have size greater than or equal to 2.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
cross2 (IterX x, IterY y) 
{
   return x[0]*y[1] - x[1]*y[0];
}

/**
   The square of the two-norm of the cross product between two vectors.

   Only the first 3 components of each vector are used.

   \pre  Vectors must have size greater than or equal to 3.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
cross_sqr_two_norm3 (IterX x, IterY y) 
{ 
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T  t[3];

   vecvec::copy_cross(x, y, t);
   return (t[0]*t[0] + t[1]*t[1]) + t[2]*t[2];
}

/**
   The two-norm of the cross product between two vectors.

   Only the first 3 components of each vector are used.

   \pre  Vectors must have size greater than or equal to 3.
   */
template <class IterX, class IterY>
INLINE typename cm2::biggest2_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type>::type
cross_two_norm3 (IterX x, IterY y) 
{ 
   typedef typename std::iterator_traits<IterX>::value_type   TX;
   typedef typename std::iterator_traits<IterY>::value_type   TY;
   typedef typename cm2::biggest2_type<TX,TY>::type           T;

   T   s;

   s = vecvec::cross_sqr_two_norm3(x, y);
   return sqrt(s);
}

/**
   The square of the two-norm of the cross product between two vectors.

   \pre  Vectors must have size equal to 2 or 3.
   */
template <class VecX, class VecY>
INLINE typename cm2::biggest2_type<typename VecX::value_type,
                                   typename VecY::value_type>::type
cross_sqr_two_norm (const VecX& x, const VecY& y) 
{ 
   assert ((x.size() == 2) || (x.size() == 3));
   assert (x.size() == y.size());

   typedef typename VecX::value_type                  TX;
   typedef typename VecY::value_type                  TY;
   typedef typename cm2::biggest2_type<TX,TY>::type   T;

   T     s(0);

   if (x.size() == 2)
   {
      s = vecvec::cross2(x.begin(), y.begin());  
      s *= s;
   }
   else if (x.size() == 3)
      s = vecvec::cross_sqr_two_norm3(x.begin(), y.begin());

   return s;
}

/**
   The two-norm of the cross product between two vectors.

   \pre  Vectors must have size equal to 2 or 3.
   */
template <class VecX, class VecY>
INLINE typename cm2::biggest2_type<typename VecX::value_type,
                                   typename VecY::value_type>::type
cross_two_norm (const VecX& x, const VecY& y) 
{ 
   assert ((x.size() == 2) || (x.size() == 3));
   assert (x.size() == y.size());

   typedef typename VecX::value_type                  TX;
   typedef typename VecY::value_type                  TY;
   typedef typename cm2::biggest2_type<TX,TY>::type   T;

   T     s(0);

   if (x.size() == 2)
   {
      s = x[0]*y[1] - x[1]*y[0];  
      s = Tabs(s);
   }
   else if (x.size() == 3)
      s = vecvec::cross_two_norm3(x.begin(), y.begin());

   return s;
}


/**
   The mixed product of two vectors.

   Only the first 3 components of each vector are used.

   Mixed product = determinant of 3 vectors.
   \pre  Vectors must have size greater than or equal to 3.
   */
template <class IterX, class IterY, class IterZ>
INLINE typename cm2::biggest3_type<typename std::iterator_traits<IterX>::value_type,
                                   typename std::iterator_traits<IterY>::value_type,
                                   typename std::iterator_traits<IterZ>::value_type>::type
mixed_product (IterX x, IterY y, IterZ z) 
{ 
   return (x[0] * (y[1]*z[2] - y[2]*z[1]) - x[1] * (y[0]*z[2] - y[2]*z[0]))
         + x[2] * (y[0]*z[1] - y[1]*z[0]);
}


/**
   The reciprocal vector.

   \verbatim 
      det = two_norm (x) 
      invT_x = x / (det*det)
   \endverbatim
   \pre  Vectors must have same size.
   \post Dot product \p x % \p invT_x == 1
   */
template <class Vec>
typename Vec::value_type
reciprocal (const Vec& x, Vec& invT_x) 
{
   typename Vec::value_type   det = vecscal::sqr_two_norm(x);

   vecvec::copy_scale(1./det, x, invT_x);
   return sqrt(det);
}

/**
   Tests the collinearity between two vectors.

   \pre  Vectors must have same size.
   \see  are_independent.
   */
template <class VecX, class VecY>
bool 
are_collinear (const VecX& x, const VecY& y, double tol = CM2_EPSILON) 
{
   typedef typename VecX::value_type                  TX;
   typedef typename VecY::value_type                  TY;
   typedef typename cm2::biggest2_type<TX,TY>::type   T;

   const   T   x2y2 = vecscal::sqr_two_norm(x) * vecscal::sqr_two_norm(y);
   const   T   xy   = vecvec::dot(x, y);

   return (Tabs(xy*xy - x2y2) < tol * x2y2);
}

/**
   Tests the independence (non collinearity) between two vectors.

   \pre  Vectors must have same size.
   \see  are_collinear.
   */
template <class VecX, class VecY>
INLINE bool 
are_independent (const VecX& x, const VecY& y, double tol = CM2_EPSILON) 
{
   return !vecvec::are_collinear(x, y, tol);
}

/**
   Tests the orthogonality between two vectors.

   \pre  Vectors must have same size.
   */
template <class VecX, class VecY>
bool 
are_orthogonal (const VecX& x, const VecY& y, double tol = CM2_EPSILON) 
{
   typedef typename VecX::value_type                  TX;
   typedef typename VecY::value_type                  TY;
   typedef typename cm2::biggest2_type<TX,TY>::type   T;

   const   T   x2y2 = vecscal::sqr_two_norm(x) * vecscal::sqr_two_norm(y);
   const   T   xy   = vecvec::dot(x, y);

   return (xy*xy < tol * x2y2);
}
//@}

} // end namespace vecvec
} // end namespace cm2


#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (pop)
#endif

#endif

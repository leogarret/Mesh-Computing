/**
   \file       vecscal.h
   \brief      Definition of vector-scalar template functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3626 $
   $Date: 2019-05-20 16:18:52 +0200 (lun., 20 mai 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_VECTOR_SCALAR_OPS__
#define __CM2_VECTOR_SCALAR_OPS__


#include "matvec_tags.h"
#include "math1_aux.h"


#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (push)
#pragma warning (disable : 4068)             // Disable unrecognized OMP pragma.
#endif


/**
   \namespace  cm2::vecscal
   \brief      Namespace for Vector-Scalar template functions.

   Namespace \ref vecscal is made of two categories of template functions:
      - Basic functions on iterators (can be used with raw pointers)
        \ref VecScalBasicGroup "Vector-Scalar Basic Template Functions"
      - Functions on vector classes (concepts of vector with members `begin()`, `end()` and `size()`)
        \ref VecScalGroup "Vector-Scalar Template Functions"

   */

namespace cm2 { 
namespace vecscal { 


/** \defgroup VecScalBasicGroup Vector-Scalar Basic Template Functions */
//@{

/**
   Scalar copy.

   \verbatim    
      x[i] = v, for i = 0, 1, 2  
   \endverbatim

   \return     The iterator \p x + 2.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE IterX
copy2 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] = x[1] = v; 
   return x+2;
}

/**
   Scalar copy.

   \verbatim    
      x[i] = v, for i = 0, 1, 2  
   \endverbatim

   \return     The iterator \p x + 3.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE IterX
copy3 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] = x[1] = x[2] = v; 
   return x+3;
}

/**
   Scalar copy.

   \verbatim    
      x[i] = v, for i = 0...n-1  
   \endverbatim

   \return     The iterator \p x + \p n.
   */
template <class IterX>
INLINE IterX
copy_n (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n) 
{
   for (size_t i = 0; i < n; ++i)
      x[i] = v;

   return x + n;
}



/**
   Negates (serial version).

   \verbatim    
      x[i] = -x[i], for i = 0, 1
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
negate2 (IterX x) 
{
   x[0] = -x[0]; 
   x[1] = -x[1]; 
}

/**
   Negates (serial version).

   \verbatim    
      x[i] = -x[i], for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
negate3 (IterX x) 
{
   x[0] = -x[0]; 
   x[1] = -x[1]; 
   x[2] = -x[2]; 
}

/**
   Negates (serial version).

   \verbatim    
      x[i] = -x[i], for i = 0...n-1  
   \endverbatim
   */
template <class IterX>
INLINE void
negate_n (IterX x, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         x[i] = - x[i];
#else
      const size_t   n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         x[0] = -x[0]; 
         x[1] = -x[1]; 
         x[2] = -x[2]; 
         x[3] = -x[3]; 
         x += 4;
      }
#endif
   }

   if (n == 1)
      x[0] = -x[0];
   else if (n == 2)
      vecscal::negate2(x);
   else if (n == 3)
      vecscal::negate3(x);
}

/**
   Negates (parallel version).

   \verbatim    
      x[i] = -x[i], for i = 0...n-1  
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline void
negate_n_par (IterX x, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         x[i] = - x[i];
   }
   else
#endif
      vecscal::negate_n(x, n);
}



/**
   Scalar add.

   \verbatim    
      x[i] += v, for i = 0, 1 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
add2 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] += v; 
   x[1] += v; 
}

/**
   Scalar add.

   \verbatim    
      x[i] += v, for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
add3 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] += v; 
   x[1] += v; 
   x[2] += v; 
}

/**
   Scalar add (serial version).

   \verbatim    
      x[i] += v, for i = 0..n-1 
   \endverbatim
   */
template <class IterX>
INLINE void
add_n (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         x[i] += v;
#else
      const size_t   n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         x[0] += v;
         x[1] += v;
         x[2] += v;
         x[3] += v;
         x += 4;
      }
#endif
   }

   if (n == 1)
      x[0] += v;
   else if (n == 2)
      vecscal::add2(v, x);
   else if (n == 3)
      vecscal::add3(v, x);
}

/**
   Scalar add (parallel version).

   \verbatim    
      x[i] += v, for i = 0..n-1 
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline void
add_n_par (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         x[i] += v;
   }
   else
#endif
      vecscal::add_n(v, x, n);
}



/**
   Scalar subtract.

   \verbatim    
      x[i] -= v, for i = 0, 1 
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
subtract2 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] -= v; 
   x[1] -= v; 
}

/**
   Scalar subtract.

   \verbatim    
      x[i] -= v, for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
subtract3 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] -= v; 
   x[1] -= v; 
   x[2] -= v; 
}

/**
   Scalar subtract (serial version).

   \verbatim    
      x[i] -= v, for i = 0..n-1 
   \endverbatim
   */
template <class IterX>
INLINE void
subtract_n (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         x[i] -= v;
#else
      const size_t   n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         x[0] -= v;
         x[1] -= v;
         x[2] -= v;
         x[3] -= v;
         x += 4;
      }
#endif
   }

   if (n == 1)
      x[0] -= v;
   else if (n == 2)
      vecscal::subtract2(v, x);
   else if (n == 3)
      vecscal::subtract3(v, x);
}

/**
   Scalar subtract (parallel version).

   \verbatim    
      x[i] -= v, for i = 0..n-1 
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline void
subtract_n_par (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         x[i] -= v;
   }
   else
#endif
      vecscal::subtract_n(v, x, n);
}



/**
   Scalar mult.

   \verbatim    
      x[i] *= v, for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
mult2 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] *= v; 
   x[1] *= v; 
}

/**
   Scalar mult.

   \verbatim    
      x[i] *= v, for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
mult3 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] *= v; 
   x[1] *= v; 
   x[2] *= v; 
}

/**
   Scalar mult (serial version).

   \verbatim    
      x[i] *= v, for i = 0..n-1 
   \endverbatim
   */
template <class IterX>
INLINE void
mult_n (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         x[i] *= v;
#else
      const size_t   n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         x[0] *= v;
         x[1] *= v;
         x[2] *= v;
         x[3] *= v;
         x += 4;
      }
#endif
   }

   if (n == 1)
      x[0] *= v;
   else if (n == 2)
      vecscal::mult2(v, x);
   else if (n == 3)
      vecscal::mult3(v, x);
}

/**
   Scalar mult (parallel version).

   \verbatim    
      x[i] *= v, for i = 0..n-1 
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline void
mult_n_par (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         x[i] *= v;
   }
   else
#endif
      vecscal::mult_n(v, x, n);
}



/**
   Scalar div.

   \verbatim    
      x[i] /= v, for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
div2 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] /= v; 
   x[1] /= v; 
}

/**
   Scalar div.

   \verbatim    
      x[i] /= v, for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
div3 (typename std::iterator_traits<IterX>::value_type v, IterX x) 
{
   x[0] /= v; 
   x[1] /= v; 
   x[2] /= v; 
}

/**
   Scalar div (serial version).

   \verbatim    
      x[i] /= v, for i = 0..n-1 
   \endverbatim
   */
template <class IterX>
INLINE void
div_n (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         x[i] /= v;
#else
      const size_t   n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         x[0] /= v;
         x[1] /= v;
         x[2] /= v;
         x[3] /= v;
         x += 4;
      }
#endif
   }

   if (n == 1)
      x[0] /= v;
   else if (n == 2)
      vecscal::div2(v, x);
   else if (n == 3)
      vecscal::div3(v, x);
}

/**
   Scalar div (parallel version).

   \verbatim    
      x[i] /= v, for i = 0..n-1 
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline void
div_n_par (typename std::iterator_traits<IterX>::value_type v, IterX x, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         x[i] /= v;
   }
   else
#endif
      vecscal::div_n(v, x, n);
}



/**
   Reciprocal (serial version).

   \verbatim    
      x[i] = 1 / x[i], for i = 0, 1  
   \endverbatim

   \pre        x must not contain any null value.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
reciprocal2 (IterX x) 
{
   x[0] = 1. / x[0]; 
   x[1] = 1. / x[1]; 
}

/**
   Reciprocal (serial version).

   \verbatim    
      x[i] = 1 / x[i], for i = 0, 1, 2  
   \endverbatim

   \pre        x must not contain any null value.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE void
reciprocal3 (IterX x) 
{
   x[0] = 1. / x[0]; 
   x[1] = 1. / x[1]; 
   x[2] = 1. / x[2]; 
}

/**
   Reciprocal (serial version).

   \verbatim    
      x[i] = 1 / x[i], for i = 0..n-1 
   \endverbatim

   \pre        x must not contain any null value.
   */
template <class IterX>
INLINE void
reciprocal_n (IterX x, size_t n) 
{
   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         x[i] = 1. / x[i];
#else
      const size_t   n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         x[0] = 1. / x[0];
         x[1] = 1. / x[1];
         x[2] = 1. / x[2];
         x[3] = 1. / x[3];
         x += 4;
      }
#endif
   }

   if (n == 1)
      x[0]  = 1. / x[0];
   else if (n == 2)
      vecscal::reciprocal2(x);
   else if (n == 3)
      vecscal::reciprocal3(x);
}

/**
   Reciprocal (parallel version).

   \verbatim    
      x[i] = 1 / x[i], for i = 0..n-1 
   \endverbatim

   \pre        x must not contain any null value.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline void
reciprocal_n_par (IterX x, size_t n, int DUM_OMP(nthreads)) 
{
#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
        x[i] = 1. / x[i];
   }
   else
#endif
      vecscal::reciprocal_n(x, n);
}



/**
   Sum (serial version).

   \verbatim    
      s += x[i], for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
sum2 (IterX x) 
{
   return x[0] + x[1]; 
}

/**
   Sum (serial version).

   \verbatim    
      s += x[i], for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
sum3 (IterX x) 
{
   return (x[0] + x[1]) + x[2]; 
}

/**
   Sum (serial version).

   \verbatim    
      s += x[i], for i = 0..n-1
   \endverbatim
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
sum_n (IterX x, size_t n) 
{
   typename std::iterator_traits<IterX>::value_type      s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s += ((x[0] + x[1]) + (x[2] + (x[3])));
         x += 4;
      }
#endif
   }

   if (n == 1)
      s += x[0];
   else if (n == 2)
      s += vecscal::sum2(x);
   else if (n == 3)
      s += vecscal::sum3(x);

   return s;
}

/**
   Sum (parallel version).

   \verbatim    
      s += x[i], for i = 0..n-1
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline typename std::iterator_traits<IterX>::value_type
sum_n_par (IterX x, size_t n, int DUM_OMP(nthreads)) 
{
   typename std::iterator_traits<IterX>::value_type      s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += x[i];
   }
   else
#endif
      s = vecscal::sum_n(x, n);

   return s;
}



/**
   Product (serial version).

   \verbatim    
      s *= x[i], for i = 0, 1
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
prod2 (IterX x) 
{
   return x[0] * x[1]; 
}

/**
   Product (serial version).

   \verbatim    
      s *= x[i], for i = 0, 1, 2
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
prod3 (IterX x) 
{
  return (x[0] * x[1]) * x[2]; 
}

/**
   Product (serial version).

   \verbatim    
      s *= x[i], for i = 0..n-1
   \endverbatim
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
prod_n (IterX x, size_t n) 
{
   typename std::iterator_traits<IterX>::value_type      s(1);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s *= x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s *= ((x[0] * x[1]) * (x[2] * x[3]));
         x += 4;
      }
#endif
   }

   if (n == 1)
      s *= x[0];
   else if (n == 2)
      s *= vecscal::prod2(x);
   else if (n == 3)
      s *= vecscal::prod3(x);

   return s;
}

/**
   Product (parallel version).

   \verbatim    
      s *= x[i], for i = 0..n-1
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline typename std::iterator_traits<IterX>::value_type
prod_n_par (IterX x, size_t n, int DUM_OMP(nthreads)) 
{
   typename std::iterator_traits<IterX>::value_type      s(1);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(*:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s *= x[i];
   }
   else
#endif
      s = vecscal::prod_n(x, n);

   return s;
}



/**
   One norm (serial version).

   \verbatim    
      s += |x[i]|, for i = 0, 1  
   \endverbatim

   \pre        Operator `Tabs` must be defined for the element type.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
one_norm2 (IterX x) 
{
   return Tabs(x[0]) + Tabs(x[1]); 
}

/**
   One norm (serial version).

   \verbatim    
      s += |x[i]|, for i = 0, 1, 2  
   \endverbatim

   \pre        Operator `Tabs` must be defined for the element type.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
one_norm3 (IterX x) 
{
   return (Tabs(x[0]) + Tabs(x[1])) + Tabs(x[2]); 
}

/**
   One norm (serial version).

   \verbatim    
      s += |x[i]|, for i = 0..n-1  
   \endverbatim

   \pre        Operator `Tabs` must be defined for the element type.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
one_norm_n (IterX x, size_t n) 
{
   typename std::iterator_traits<IterX>::value_type      s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += Tabs(x[i]); 
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s += (Tabs(x[0]) + Tabs(x[1]));
         s += (Tabs(x[2]) + Tabs(x[3]));
         x += 4;
      }
#endif
   }

   if (n == 1)
      s += Tabs(x[0]);
   else if (n == 2)
      s += vecscal::one_norm2(x);
   else if (n == 3)
      s += vecscal::one_norm3(x);

   return s;
}

/**
   One norm (parallel version).

   \verbatim    
      s += |x[i]|, for i = 0..n-1  
   \endverbatim

   \pre        Operator `Tabs` must be defined for the element type.
   \note       The computation is done in parallel only when vectors are 
               long enough (\p n > `CM2_OMP_MIN_LOOP`), \p nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline typename std::iterator_traits<IterX>::value_type
one_norm_n_par (IterX x, size_t n, int DUM_OMP(nthreads)) 
{
   typename std::iterator_traits<IterX>::value_type      s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += Tabs(x[i]);
   }
   else
#endif
      s = vecscal::one_norm_n(x, n);

   return s;
}



/**
   Maximum norm (infinite norm).

   \verbatim    
      s = max (|x[i]|), for i = 0...n-1  
   \endverbatim

   \pre     Operator `Tabs` must be defined for the element type.
   \pre     Operator `<` must be defined for the element type.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
max_norm_n (IterX x, size_t n) 
{
   typename std::iterator_traits<IterX>::value_type   s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s = std::max(s, Tabs(x[i]));
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s = std::max(s, Tabs(x[0]));
         s = std::max(s, Tabs(x[1]));
         s = std::max(s, Tabs(x[2]));
         s = std::max(s, Tabs(x[3]));
         x += 4;
      }
#endif
   }

   if (n == 1)
      s = std::max(s, Tabs(x[0]));
   else if (n == 2)
   {
      s = std::max(s, Tabs(x[0]));
      s = std::max(s, Tabs(x[1]));
   }
   else if (n == 3)
   {
      s = std::max(s, Tabs(x[0]));
      s = std::max(s, Tabs(x[1]));
      s = std::max(s, Tabs(x[2]));
   }

   return s;
}



/**
   Square of two-norm (serial version).

   \verbatim    
      s = x[i] * x[i], for i = 0, 1  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
sqr_two_norm2 (IterX x) 
{
   return x[0]*x[0] + x[1]*x[1]; 
}

/**
   Square of two-norm (serial version).

   \verbatim    
      s = x[i] * x[i], for i = 0, 1, 2  
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
sqr_two_norm3 (IterX x) 
{
   return (x[0]*x[0] + x[1]*x[1]) + x[2]*x[2]; 
}

/**
   Square of two-norm (serial version).

   \verbatim    
      s = x[i] * x[i], for i = 0..n-1
   \endverbatim
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
sqr_two_norm_n (IterX x, size_t n) 
{
   typename std::iterator_traits<IterX>::value_type   s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += x[i] * x[i];
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s += (x[0]*x[0] + x[1]*x[1]);
         s += (x[2]*x[2] + x[3]*x[3]);
         x += 4;
      }
#endif
   }

   if (n == 1)
      s += x[0] * x[0];
   else if (n == 2)
      s += vecscal::sqr_two_norm2(x);
   else if (n == 3)
      s += vecscal::sqr_two_norm3(x);

   return s;
}

/**
   Square of two-norm (parallel version).

   \verbatim    
      s = x[i] * x[i], for i = 0..n-1
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline typename std::iterator_traits<IterX>::value_type
sqr_two_norm_n_par (IterX x, size_t n, int DUM_OMP(nthreads)) 
{
   typename std::iterator_traits<IterX>::value_type      s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += x[i] * x[i];
   }
   else
#endif
      s = vecscal::sqr_two_norm_n(x, n);

   return s;
}



/**
   Square of Hermitian norm (serial version).

   \verbatim    
      s = conj(x[i]) * x[i], for i = 0, 1.
      x is complex, s is real.
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type::value_type
sqr_Herm_norm2 (IterX x) 
{
   return (x[0].real()*x[0].real() + x[0].imag()*x[0].imag()) +
          (x[1].real()*x[1].real() + x[1].imag()*x[1].imag()); 
}

/**
   Square of Hermitian norm (serial version).

   \verbatim    
      s = conj(x[i]) * x[i], for i = 0, 1, 2.
      x is complex, s is real.
   \endverbatim

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type::value_type
sqr_Herm_norm3 (IterX x) 
{
   return (x[0].real()*x[0].real() + x[0].imag()*x[0].imag()) +
          (x[1].real()*x[1].real() + x[1].imag()*x[1].imag()) + 
          (x[2].real()*x[2].real() + x[2].imag()*x[2].imag()); 
}

/**
   Square of Hermitian norm (serial version).

   \verbatim    
      s = conj(x[i]) * x[i], for i = 0..n-1.
      x is complex, s is real.
   \endverbatim
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type::value_type
sqr_Herm_norm_n (IterX x, size_t n) 
{
   typename std::iterator_traits<IterX>::value_type::value_type   s(0);

   if (n >= 4)
   {
#ifdef CM2_NO_UNROLL
      for (size_t i = 0; i < n; ++i)
         s += x[i].real()*x[i].real() + x[i].imag()*x[i].imag();
#else
      const size_t      n4 = n / 4;
      n = n % 4;
      for (size_t i = 0; i < n4; ++i)
      {
         s += x[0].real()*x[0].real() + x[0].imag()*x[0].imag();
         s += x[1].real()*x[1].real() + x[1].imag()*x[1].imag();
         s += x[2].real()*x[2].real() + x[2].imag()*x[2].imag();
         s += x[3].real()*x[3].real() + x[3].imag()*x[3].imag();
         x += 4;
      }
#endif
   }

   if (n == 1)
      s += x[0].real()*x[0].real() + x[0].imag()*x[0].imag();
   else if (n == 2)
      s += vecscal::sqr_Herm_norm2(x);
   else if (n == 3)
      s += vecscal::sqr_Herm_norm3(x);

   return s;
}

/**
   Square of Hermitian norm (parallel version).

   \verbatim    
      s = conj(x[i]) * x[i], for i = 0..n-1.
      x is complex, s is real.
   \endverbatim

   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline typename std::iterator_traits<IterX>::value_type::value_type
sqr_Herm_norm_n_par (IterX x, size_t n, int DUM_OMP(nthreads)) 
{
   typename std::iterator_traits<IterX>::value_type::value_type      s(0);

#ifdef _OPENMP
   if ((n > CM2_OMP_MIN_LOOP) && (nthreads > 1))
   {
      assert (n <= size_t(INT_MAX));
      const int n_ = int(n);
      #pragma omp parallel for reduction(+:s) schedule(static) num_threads(nthreads)
      for (int i = 0; i < n_; ++i) 
         s += x[i].real()*x[i].real() + x[i].imag()*x[i].imag();
   }
   else
#endif
      s = vecscal::sqr_Herm_norm_n(x, n);

   return s;
}



/**
   Normalization (serial version).

   \return     The two-norm of the vector.
   \pre        Operator `sqrt` must be defined for the element type.
   \note       The null vector (two-norm = 0) is left null.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
normalize2 (IterX x) 
{
   typedef typename std::iterator_traits<IterX>::value_type   T;

   T     s  = vecscal::sqr_two_norm2(x); 

   if ((s != T(0)) && (s != T(1)))
   {
      s = sqrt(s);
      vecscal::mult2(T(1) / s, x);
   }

   return s;
}

/**
   Normalization (serial version).

   \return     The two-norm of the vector.
   \pre        Operator `sqrt` must be defined for the element type.
   \note       The null vector (two-norm = 0) is left null.
   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
normalize3 (IterX x) 
{
   typedef typename std::iterator_traits<IterX>::value_type   T;

   T     s  = vecscal::sqr_two_norm3(x); 

   if ((s != T(0)) && (s != T(1)))
   {
      s = sqrt(s);
      vecscal::mult3(T(1) / s, x);
   }

   return s;
}

/**
   Normalization (serial version).

   \return     The two-norm of the vector.
   \pre        Operator `sqrt` must be defined for the element type.
   \note       The null vector (two-norm = 0) is left null.
   */
template <class IterX>
INLINE typename std::iterator_traits<IterX>::value_type
normalize_n (IterX x, size_t n) 
{
   typedef typename std::iterator_traits<IterX>::value_type   T;

   T        s  = vecscal::sqr_two_norm_n(x, n);
   
   if ((s != T(0)) && (s != T(1)))
   {
      s = sqrt(s); 
      vecscal::mult_n(T(1) / s, x, n);
   }

   return s;
}

/**
   Normalization (parallel version).

   \return     The two-norm of the vector.
   \pre        Operator `sqrt` must be defined for the element type.
   \note       The null vector (two-norm = 0) is left null.
   \note       The computation is done in parallel only when vectors are 
               long enough (size > CM2_OMP_MIN_LOOP), nthreads > 1 and if OMP is enabled.
   */
template <class IterX>
inline typename std::iterator_traits<IterX>::value_type
normalize_n_par (IterX x, size_t n, int nthreads) 
{
   typedef typename std::iterator_traits<IterX>::value_type   T;

   T        s = vecscal::sqr_two_norm_n_par(x, n, nthreads);
   
   if ((s != T(0)) && (s != T(1)))
   {
      s = sqrt(s);
      vecscal::mult_n_par(T(1) / s, x, n, nthreads);
   }

   return s;
}



/// Returns the number of values in array [\p x, \p x + \p n[ that match a predicate.
template <class IterX, class Predicate>
inline size_t
count_if_n (IterX x, size_t n, Predicate pred) 
{
   size_t       count(0);

   for (size_t i = 0; i < n; ++i)
      if (pred(x[i])) 
         ++count; 

   return count;
}



/**
   Tests all elements in array [\p x, \p x + 2[ against a scalar.

   Returns true if all elements equal to the scalar, 
   false if at least one element does not equal to the scalar.

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE bool
equal2 (IterX x, typename std::iterator_traits<IterX>::value_type v) 
{
   return (x[0] == v) && (x[1] == v);
}

/**
   Tests all elements in array [\a x, \p x + 3[ against a scalar.

   Returns true if all elements equal to the scalar, 
   false if at least one element does not equal to the scalar.

   \note       This specialization makes the optimizer's job easier.
   */
template <class IterX>
INLINE bool
equal3 (IterX x, typename std::iterator_traits<IterX>::value_type v) 
{
   return (x[0] == v) && (x[1] == v) && (x[2] == v);
}

/**
   Tests all elements in array [\p x, \p x + \p n[ against a scalar.

   Returns true if all elements equal to the scalar, 
   false if at least one element does not equal to the scalar, or if \p n==0.
   */
template <class IterX>
inline bool
equal_n (IterX x, size_t n, typename std::iterator_traits<IterX>::value_type v) 
{
   bool        result(n > 0);

   for (size_t i = 0; (i < n) && result; ++i)
      result = (x[i] == v);

   return result;
}



/**
   Computes the mean and the variance of the array [\p x, \p x + \p n[.

   \verbatim     
      mean = sum(x[i]) / n 
      variance = sum((x[i]-mean)*(x[i]-mean)) / n
   \endverbatim

   \note    The standard deviation equal to sqrt(variance).
   */
template <class IterX, class T>
void
statistics_n (IterX x, size_t n, T& mean, T& variance) 
{
   const T     inv_n    = 1. / T(n);

   mean = variance = T(0);

   for (size_t i = 0; i < n; ++i)
   {
      mean += x[i];
      variance += x[i] * x[i]; 
   } 

   mean *= inv_n;
   variance = (variance * inv_n) - (mean * mean);
}



/**
   Increases min and max in \p R for the array [\a x, \p x+n[.

   \pre     The range \p R must be a valid range (`R.first <= R.second`).
   \pre     Operator `<` must be defined for the element type.
   */
template <class IterX, class Range>
inline void
inc_range_n (IterX x, size_t n, Range& R) 
{
   typedef typename std::iterator_traits<IterX>::value_type  T;

   for (size_t i = 0; i < n; ++i)
   {
      const T  v  = x[i];
      if (v < R.first) R.first = v;
      if (R.second < v) R.second = v;
   }
}

//@}




/** \defgroup VecScalGroup Vector-Scalar Template Functions */
//@{
/**
   Copies a scalar value into a vector.

   \verbatim    
      x[i] = v, i = 0..n-1.
   \endverbatim
   */
template <class Vec> 
INLINE void
copy (typename Vec::value_type v, Vec& X)             
{ 
   vecscal::copy_n(v, X.begin(), X.size()); 
}

/**
   Adds a scalar to a vector.

   \verbatim    
      x[i] += v, i = 0..n-1.
   \endverbatim
   */
template <class Vec>
INLINE void
add (typename Vec::value_type v, Vec& X)             
{ 
   vecscal::add_n(v, X.begin(), X.size()); 
}

/**
   Subtract a scalar to a vector.

   \verbatim    
      x[i] -= v, i = 0..n-1.
   \endverbatim
   */
template <class Vec>
INLINE void
subtract (typename Vec::value_type v, Vec& X)             
{ 
   vecscal::subtract_n(v, X.begin(), X.size()); 
}

/**
   Multiplies a vector by a scalar.

   \verbatim    
      x[i] *= v, i = 0..n-1
   \endverbatim
   */
template <class Vec>
INLINE void
mult (typename Vec::value_type v, Vec& X)             
{ 
   vecscal::mult_n(v, X.begin(), X.size()); 
}

/**
   Divides a vector by a scalar.

   \verbatim    
      x[i] /= v, i = 0..n-1
   \endverbatim

   \note    Prefer multiply by 1/v for non integer types.
   */
template <class Vec>
INLINE void
div (typename Vec::value_type v, Vec& X)             
{ 
   vecscal::div_n(v, X.begin(), X.size()); 
}

/**
   Negates a vector.

   \verbatim    
      x[i] = -x[i], i = 0..n-1.
   \endverbatim
   */
template <class Vec>
INLINE void
negate (Vec& X)                                      
{ 
   vecscal::negate_n(X.begin(), X.size()); 
}

/**
   Sets a vector to its reciprocal (element by element).

   \verbatim    
      x[i] = 1/x[i], i = 0..n-1.  
   \endverbatim
   \pre x must not contain any null component.
   */
template <class Vec>
INLINE void
reciprocal (Vec& X)                                  
{ 
   vecscal::reciprocal_n(X.begin(), X.size()); 
}

/**
   Returns the index of the max in a vector. 

   \pre     The vector must not be empty.
   \pre     Operator `<` must be defined for the element type.
   */
template <class Vec>
INLINE size_t
max_index (const Vec& x) 
{
   assert (!x.empty());
   const typename Vec::const_iterator     p = std::max_element(x.begin(), x.end());

   return size_t(p - x.begin());
}

/**
   Returns the index of the min in a vector.

   \pre     The vector must not be empty.
   \pre     Operator `<` must be defined for the element type.
   */
template <class Vec>
INLINE size_t
min_index (const Vec& x) 
{
   assert (!x.empty());
   const typename Vec::const_iterator     p = std::min_element(x.begin(), x.end());

   return size_t(p - x.begin());
}

/**
   Returns the index of the max of absolute (max(|x[i]|) in a vector.

   \pre     The vector must not be empty.
   \pre     Operator `Tabs` must be defined for the element type T = Vec::value_type (in namespace cm2).
            \verbatim
            namespace cm2 
            {
               T Tabs(T a);
            }
            \endverbatim
   \pre     Operator `<` must be defined for the element type.
   */
template <class Vec>
INLINE size_t
max_norm_index (const Vec& x) 
{
   assert (!x.empty());
   const typename Vec::const_iterator     p = std::max_element(x.begin(), x.end(), abs_cmp());

   return size_t(p - x.begin());
}

/**
   Returns the index of the min of absolute (min(|x[i]|) in a vector.

   \pre     The vector must not be empty.
   \pre     Operator `Tabs` must be defined for the element type T = Vec::value_type (in namespace cm2).
            \verbatim
            namespace cm2 
            {
               T Tabs (T a);
            }
            \endverbatim
   \pre     Operator `<` must be defined for the element type.
   */
template <class Vec>
INLINE size_t
min_norm_index (const Vec& x) 
{
   assert (!x.empty());
   const typename Vec::const_iterator     p = std::min_element(x.begin(), x.end(), abs_cmp());

   return size_t(p - x.begin());
}

/**
   Returns the maximum value in a vector.
   \verbatim    
      max (x[i]), i = 0..n-1    
   \endverbatim

   \pre     The vector must not be empty.
   \pre     Operator `<` must be defined for the element type.
   */
template <class Vec>
INLINE typename Vec::value_type
max_value (const Vec& x) 
{
   assert (!x.empty());
   return *std::max_element(x.begin(), x.end());
}

/**
   Returns the minimum value in a vector.
   \verbatim    
      min (x[i]), i = 0..n-1    
   \endverbatim

   \pre     The vector must not be empty.
   \pre     Operator `<` must be defined for the element type.
   */ 
template <class Vec>
INLINE typename Vec::value_type
min_value (const Vec& x) 
{
   assert (!x.empty());
   return *std::min_element(x.begin(), x.end());
}

/**
   Increases range \p R with the range of a vector.

   \verbatim  
      R.first = min (R.first, x[i]), i = 0..n-1
      R.second = max (R.second, x[i]), i = 0..n-1
   \endverbatim
   \pre  The range \p R must be a valid range (`R.first <= R.second`).
   \pre  Operator `<` must be defined for the element type.
   */
template <class Vec, class Range> 
INLINE void
inc_range (const Vec& X, Range& R) 
{
   vecscal::inc_range_n(X.begin(), X.size(), R);
}


/**
   Computes the range of a vector (first = min, second = max)

   \pre  The vector must not be empty.
   \pre  Operator `<` must be defined for the element type.
   */
template <class Vec> 
INLINE std::pair<typename Vec::value_type, typename Vec::value_type>
range (const Vec& X) 
{
   typedef typename Vec::value_type    T;

   typename Vec::const_iterator  x  = X.begin();
   const size_t                  N  = X.size();
   std::pair<T,T>                R (*x, *x);

   vecscal::inc_range_n(++x, N-1, R);

   return R;
}


/**
   Returns the sum of the elements in a vector.
   \verbatim    
      x[0] + x[1] + x[2] ... 
   \endverbatim
   */
template <class Vec> 
INLINE typename Vec::value_type
sum (const Vec& X) 
{
   return vecscal::sum_n(X.begin(), X.size()); 
}


/**
   Returns the product of the elements in a vector.
   \verbatim    
      x[0] * x[1] * x[2] ... 
   \endverbatim
   */
template <class Vec> 
INLINE typename Vec::value_type
prod (const Vec& X) 
{
   return vecscal::prod_n(X.begin(), X.size()); 
}


/**
   Returns the one norm of a vector (the sum of absolute elements).
   \verbatim    
      |x[0]| + |x[1]| + |x[2]| ... 
   \endverbatim

   \pre     Operator `Tabs` must be defined for the element type T = Vec::value_type (in namespace cm2).
            \verbatim
            namespace cm2 
            {
               T Tabs(T a);
            }
            \endverbatim
   */
template <class Vec>
INLINE typename Vec::value_type
one_norm (const Vec& X) 
{
   return vecscal::one_norm_n(X.begin(), X.size()); 
}


/**
   Returns the square of the two norm of a vector.
   \verbatim    
      x[0]*x[0] + x[1]*x[1] + x[2]*x[2] ... 
   \endverbatim
   */
template <class Vec>
INLINE typename Vec::value_type
sqr_two_norm (const Vec& X) 
{
   return vecscal::sqr_two_norm_n(X.begin(), X.size()); 
}


/**
   Returns the two norm of a vector (usual Euclidian norm).
   \verbatim    
      sqrt (x[0]*x[0] + x[1]*x[1] + x[2]*x[2] ... )
   \endverbatim

   \pre     Operator `sqrt` must be defined for the element type.
   */
template <class Vec>
INLINE typename Vec::value_type
two_norm (const Vec& X) 
{
   return sqrt(vecscal::sqr_two_norm(X));
}

/**
   Returns the maximum norm (infinite norm).
   \verbatim    
      max (|x[0]|, |x[1]|, |x[2]| ... )
   \endverbatim

   \pre     Operator `Tabs` must be defined for the element type T = Vec::value_type (in namespace cm2).
            \verbatim
            namespace cm2 
            {
               T Tabs (T a);
            }
            \endverbatim
   \pre     Operator `<` must be defined for the element type.
   */
template <class Vec>
INLINE typename Vec::value_type
max_norm (const Vec& X) 
{
   return vecscal::max_norm_n (X.begin(), X.size());
}

/**
   Returns the mean of a vector.
   \verbatim    
      mean = (x[0] + x[1] + x[2] ... + x[n-1]) / n
   \endverbatim

   \pre     The vector must not be empty.
   \pre     Operator `Tabs` must be defined for the element type.
   \pre     Operator `<` must be defined for the element type.
   */
template <class Vec>
INLINE typename Vec::value_type
mean (const Vec& X) 
{
   assert (!X.empty());
   return vecscal::sum(X) / X.size();
}

/**
   Statistics on a vector (mean and variance).

   \verbatim     
      mean = sum(x[i]) / x.size() 
      variance = sum((x[i]-mean)*(x[i]-mean)) / x.size()
   \endverbatim

   \pre     The vector must not be empty.
   \note    The standard deviation equal to sqrt(variance).
   */
template <class Vec, class T> 
INLINE void
statistics (const Vec& X, T& mean, T& variance) 
{
   vecscal::statistics_n (X.begin(), X.size(), mean, variance);
}

/**
   Returns the number of elements in a vector that match a predicate.

   The predicate must be a valid STL-compatible predicate, with:
   \verbatim
      bool  Predicate::operator (T v) const;
   \endverbatim
   */
template <class Vec, class Predicate>
INLINE size_t
count_if (const Vec& X, Predicate pred) 
{
   return vecscal::count_if_n (X.begin(), X.size(), pred);
}

/**
   Normalizes a vector (if not null).

   \pre     The vector must not be empty.
   \note    The null vector (null two-norm) is not changed.
   \return  The two-norm of the vector.
   */ 
template <class Vec>
INLINE typename Vec::value_type
normalize (Vec& X) 
{ 
   return vecscal::normalize_n(X.begin(), X.size());
}


/**
   Tests all elements in a vector against a scalar.

   Returns true if all elements equal to the scalar, false if at least one element does not equal to the scalar.
   */
template <class Vec>
INLINE bool
equal (const Vec& X, typename Vec::value_type v) 
{
   return vecscal::equal_n(X.begin(), X.size(), v); 
}

/**
   Adds random values between 0 and \p v to all elements of a vector.
   \verbatim    
      x[i] += v * rand(0,1), i = 0..n-1
   \endverbatim
   */
template <class Vec>
void
randomize (Vec& X, typename Vec::value_type v) 
{
   typedef typename Vec::value_type       T;

   const double            DELTA = 1. / double(RAND_MAX);
   typename Vec::iterator  x     = X.begin();
   const size_t            n     = X.size();
   const T                 dv    = v * DELTA;

   for (size_t i = 0; i < n; ++i, ++x)
   {
      const T	r = dv * ::rand();                           // 0 <= r <= v
      *x += r;
   } 
}

/**
   Adds random values between \p v1 and \p v2 to all elements of a vector.
   \verbatim    
      x[i] += v1 + (v2 - v1) * rand(0,1), i = 0..n-1   
   \endverbatim
   */
template <class Vec>
void
randomize_range (Vec& X, typename Vec::value_type v1, typename Vec::value_type v2) 
{
   typedef typename Vec::value_type       T;

   const double            DELTA = 1. / double(RAND_MAX);
   typename Vec::iterator  x     = X.begin();
   const size_t            n     = X.size();
   const T                 dv    = (v2 - v1) * DELTA;

   for (size_t i = 0; i < n; ++i, ++x)
   {
      const T	r = v1 + dv * ::rand();                      // v1 <= r <= v2.
      *x += r;
   } 
}

/**
   Multiplies the elements of a vector by random values between \p 1-v and \p 1+v . 
   \verbatim    
      x[i] *= 1 - v + 2 * v * rand(0,1), i = 0..n-1   
   \endverbatim
   */
template <class Vec> 
void
randomize_mult (Vec& X, typename Vec::value_type v) 
{
   typedef typename Vec::value_type       T;

   const double            DELTA = 2. / double(RAND_MAX);
   const T                 dv    = v  * DELTA;
   const T                 v1    = 1 - v;
   typename Vec::iterator  x     = X.begin();
   const size_t            n     = X.size();

   for (size_t i = 0; i < n; ++i, ++x)
   {
      const T	r = v1 + dv * ::rand();                      // 1-v <= r <= 1+v.
      *x *= r;
   }
}

/**
   Adds random values between 0 and \p v to all elements of a vector.
   \verbatim    
      x[i] += v * rand(0,1), i = 0..n-1
   \endverbatim
   */
template <class Vec, class RandGen>
void
randomize (Vec& X, typename Vec::value_type v, RandGen& randg) 
{
   typedef typename Vec::value_type       T;

   const double            DELTA = 1. / double(RandGen::rand_max());
   const T                 dv    = v * DELTA;
   typename Vec::iterator  x     = X.begin();
   const size_t            n     = X.size();

   for (size_t i = 0; i < n; ++i, ++x)
   {
      const T	r = dv * randg();                         // 0 <= r <= v.
      *x += r;
   } 
}

/**
   Adds random values between \p v1 and \p v2 to all elements of a vector.
   \verbatim    
      x[i] += v1 + (v2 - v1) * rand(0,1), i = 0..n-1   
   \endverbatim
   */
template <class Vec, class RandGen>
void
randomize_range (Vec& X, typename Vec::value_type v1, typename Vec::value_type v2, RandGen& randg) 
{
   typedef typename Vec::value_type       T;

   const double            DELTA = 1. / double(RandGen::rand_max());
   const T                 dv    = (v2 - v1) * DELTA;
   typename Vec::iterator  x     = X.begin();
   const size_t            n     = X.size();

   for (size_t i = 0; i < n; ++i, ++x)
   {
      const T	r = v1 + dv * randg();                    // v1 <= r <= v2.
      *x += r;
   } 
}

/**
   Multiplies the elements of a vector by random values between \p 1-v and \p 1+v . 
   \verbatim    
      x[i] *= 1 - v + 2 * v * rand(0,1), i = 0..n-1   
   \endverbatim
   */
template <class Vec, class RandGen> 
void
randomize_mult (Vec& X, typename Vec::value_type v, RandGen& randg) 
{
   typedef typename Vec::value_type       T;

   const double            DELTA = 2. / double(RandGen::rand_max());
   const T                 dv    = v  * DELTA;
   const T                 v1    = 1 - v;
   typename Vec::iterator  x     = X.begin();
   const size_t            n     = X.size();

   for (size_t i = 0; i < n; ++i, ++x)
   {
      const T	r = v1 + dv * randg();                 // 1-v <= r <= 1+v.
      *x *= r;
   }
}
//@}

} // namespace vecscal
} // end namespace cm2


#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (pop)
#endif

#endif

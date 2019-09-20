/**
   \file       math1_aux.h
   \brief      Small auxiliary functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATH1_AUX_H__
#define __CM2_MATH1_AUX_H__


namespace cm2 {

// From Boost...
template <bool>
struct bool_type                       { };

template <typename Cond, typename T1, typename T2>
struct if_                             { typedef T1 type; };

template <typename T1, typename T2>
struct if_<bool_type<false>, T1, T2>   { typedef T2 type; };

/// Biggest type between two (used to test conversions such as short->long, float->double, double->std::complex<double>...)
template <typename T1, typename T2>
struct biggest2_type
{ 
   typedef bool_type<(sizeof(T1) > sizeof(T2))>       T1_is_bigger; 
   typedef typename if_<T1_is_bigger,T1,T2>::type     type;
};

/// Biggest type between three (used to test conversions such as short->long, float->double, double->std::complex<double>...)
template <typename T1, typename T2, typename T3>
struct biggest3_type
{ 
   typedef bool_type<(sizeof(T1) > sizeof(T2))>       T1_is_bigger; 
   typedef typename if_<T1_is_bigger,T1,T2>::type     T12;
   typedef bool_type<(sizeof(T12) > sizeof(T3))>      T12_is_bigger; 
   typedef typename if_<T12_is_bigger,T12,T3>::type   type;
};


/**  
   General template abs function.
   The default behaviour is to call abs.
   */
template <class T>
INLINE T 
Tabs (T a)                               { return abs(a); }

/**
   Specialization for double.
   Wrapper for ::fabs.
   */
#ifdef CM2_FUNCTION_TMPL_SPECIALIZATION_BUG
INLINE double 
Tabs (double a)                          { return ::fabs(a); }
#else
template<>
INLINE double 
Tabs<double> (double a)                  { return ::fabs(a); }
#endif   // CM2_FUNCTION_TMPL_SPECIALIZATION_BUG

/**
   Specialization for float.
   Wrapper for ::fabs.
   */
#ifdef CM2_FUNCTION_TMPL_SPECIALIZATION_BUG
INLINE float 
Tabs (float a)                           { return ::fabs(a); }
#else
template<>
INLINE float 
Tabs<float> (float a)                    { return ::fabs(a); }
#endif   // CM2_FUNCTION_TMPL_SPECIALIZATION_BUG


/**
   Functor to compare the sum of "absolute" values of \ref b and \ref a.

   The "absolute" value is defined by the template function cm2::Tabs.
   */
struct abs_cmp 
{ 
   template <class T>
   INLINE bool operator()(const T& a, const T& b) { return cm2::Tabs(a) < cm2::Tabs(b); }
};


template <int N> struct __int_to_type { };

/**
   Functor to give the absolute min value of a type (signed or not).
   */
template <class T>
struct abs_min
{ 
   static INLINE T __sgn_min (__int_to_type<true>)  { return -std::numeric_limits<T>::max(); }
   static INLINE T __sgn_min (__int_to_type<false>) { return +std::numeric_limits<T>::min(); }
   static INLINE T value()  
   { 
      return __sgn_min (__int_to_type<std::numeric_limits<T>::is_signed>()); 
   }
};


}  // namespace cm2

#endif

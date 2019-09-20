/**
   \file       math1_ref_counted.h
   \brief      Reference-counter base class in cm2::math1 namespace with CM2MATH1_API declspec.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATH1_REF_COUNTED_H__
#define __CM2_MATH1_REF_COUNTED_H__


namespace cm2 {
namespace math1 {

/**
   Class to implement reference counting (thread-safe).

   All classes subjected to be used as smart pointer (i.e. through the Ptr<T> class)
   must inherit from this base class.
   */
class CM2MATH1_API ref_counted 
{
public:
///
ref_counted() : _refs(0)   { }
/// Special copy constructor.
ref_counted (const ref_counted&) : _refs(0)   { }
///
virtual ~ref_counted()     { assert(_refs == 0); }
///
INLINE void acquire()               
{ 
#if (defined(WIN32) | defined(WIN64)) && defined(_MT)
   InterlockedIncrement(&_refs);                // MS thread-safe inc.
#elif defined(CM2_GCC_BUILTIN_ATOMICS)
   __sync_add_and_fetch(&_refs, 1);             // GCC thread-safe inc.
#else
   ++_refs;                                     // Not thread-safe inc!
#endif
}

///
INLINE void release()             
{ 
#if (defined(WIN32) | defined(WIN64)) && defined(_MT)
   if (InterlockedDecrement(&_refs) == 0)       // MS thread-safe dec.
#elif defined(CM2_GCC_BUILTIN_ATOMICS)
   if (__sync_sub_and_fetch(&_refs, 1) == 0)    // GCC thread-safe dec.
#else
   if (--_refs == 0)                            // Not thread-safe dec!
#endif
      this->__destroy(); 
}

/// Special copy operator.
INLINE ref_counted&
operator= (const ref_counted&)   
{ 
   assert(_refs == 0);
   return *this; 
}

//////////////////////
private:

#if (defined(WIN32) | defined(WIN64)) && defined(_MT)
typedef volatile LONG   counter_type;
#else
typedef int             counter_type;
#endif

///
CM2_NOINLINE void __destroy();

///
counter_type   _refs;

};

}  // end namespace math1 
}  // end namespace cm2 

#endif

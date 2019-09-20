#ifndef __CM2_SMART_PTR_H__
#define __CM2_SMART_PTR_H__

/**
   \file       smart_ptr.h
   \brief      Smart pointer (template class).
   \copyright  (C)1999-2019, Computing Objects, France. info@computing-objects.com

   $Rev: 3541 $
   $Date: 2019-03-15 12:37:45 +0100 (ven., 15 mars 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

namespace cm2 {

/**
   Template smart pointer.
   
   Intrusive reference-counting smart pointer
   (the counter is stored within the object).

   Class X must be a ref_counted (or a concept of).
   */
template <typename X> 
class Ptr 
{
public:

typedef X         pointer_type;

/**
   Ptr needs to be its own friend so Ptr<X> and Ptr<Y> can access
   each other's private data members. 
   */ 
template <class Y> friend class Ptr; 


/// Default constructor.
Ptr() : _p(NULL)          
{ }

/// Construct from a raw pointer.
Ptr (X *x) : _p(x)
{ 
   if (x) 
      x->acquire(); 
}

/// Copy constructor.
Ptr (const Ptr<X>& x) : _p(x._p)
{ 
   if (&x == this)         // Can happen in static data initialization.
      return;           
   if (x._p) 
      x._p->acquire(); 
}

/// Copy constructor with cast.
template <typename Y>
explicit Ptr (const Ptr<Y>& y)
{
   if (y._p) 
      y._p->acquire(); 
   _p = dynamic_cast<X*>(y._p); 
}

///
~Ptr()  
{ 
   if (_p) _p->release(); 
}

/// Assignment operator from a raw pointer.
Ptr<X>& operator= (X* x)
{
   if (x) 
      x->acquire(); 
   if (_p) 
      _p->release(); 
   _p = x;
   return *this; 
}

/// Assignment operator.
Ptr<X>& operator= (const Ptr<X>& x)    { return this->operator=(x._p); }

/// Assignment operator with cast.
template <typename Y>
Ptr<X>& operator= (const Ptr<Y>& y)    { return this->operator=(dynamic_cast<X*>(y._p)); }
 

/// Assignment to int to allow Ptr<X> = NULL.
Ptr<X>& operator= (int num)
{
   assert(num == 0);
   return this->operator=(static_cast<X*>(0)); 
} 

/// Assignment to long to allow Ptr<X> = NULL.
Ptr<X>& operator= (long num)           
{
   assert(num == 0);
   return this->operator=(static_cast<X*>(0)); 
} 

/// Member Access.
INLINE X* operator->() const           { return _p; }

/// Dereference the pointer.
INLINE X& operator* () const           { return *_p; }

/// Conversion/casting operator.
INLINE operator const X*() const       { return _p; }

/// Explicit access to raw pointer.
INLINE X* ptr() const                  { return _p; }

/// Cast conversion.
template <typename Y>
INLINE operator Ptr<Y>()               { return Ptr<Y>(dynamic_cast<Y*>(_p)); }

///
INLINE bool is_valid() const           { return (_p != NULL); }

///
INLINE operator bool() const           { return this->is_valid(); }

private:
/// The raw pointer.
X     *_p;

};


/* Equality comparison operators. */
template <typename X, typename Y>
bool operator== (const Ptr<X>& x, const Ptr<Y>& y)    { return x.ptr() == y.ptr(); }

template <typename X, typename Y>
bool operator== (const Ptr<X>& x, const Y* y)         { return x.ptr() == y; }

template <typename X, typename Y>
bool operator== (const X* x, const Ptr<Y>& y)         { return x == y.ptr(); }


/* Inequality comparison operators. */
template <typename X, typename Y>
bool operator!= (const Ptr<X>& x, const Ptr<Y>& y)    { return (!operator==(x, y)); }

template <typename X, typename Y>
bool operator!= (const Ptr<X>& x, const Y* y)         { return (!operator==(x, y)); }

template <typename X, typename Y>
bool operator!= (const X* x, const Ptr<Y>& y)         { return (!operator==(x, y)); }


/* Boolean operators. */
template <typename X>
bool operator! (const Ptr<X>& p)                      { return !p.is_valid(); }


/* Less than operators. */
template <typename X, typename Y>
bool operator< (const Ptr<X>& x, const Ptr<Y>& y)     { return x.ptr() < y.ptr(); }

template <typename X, typename Y>
bool operator< (const Ptr<X>& x, const Y* y)          { return x.ptr() < y; }

template <typename X, typename Y>
bool operator< (const X* x, const Ptr<Y>& y)          { return x < y.ptr(); }

}  // end namespace cm2 

#endif

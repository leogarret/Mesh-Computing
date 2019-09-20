/**
   \file       vector_num.h
   \brief      Definition of dynamic-sized template array (low level).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_VECTOR_NUM_H__
#define __CM2_VECTOR_NUM_H__


#include "math1_ref_counted.h"
#include "smart_ptr.h"
#include "strided_iterator.h"


namespace cm2 { 

/**
   STL-style vector class modified for high performance. 

   Internal use only.

   \tparam     T     Any type with default constructor, copy constructor and assignment operator.

   \warning    VEC_ALIGNMENT should be a power of 2. \n
               The actual alignment is `max(VEC_ALIGNMENT, alignof(T))`
               (i.e. at least aligned on `alignof(T)` boundaries).
   */
template <class T, 
          int VEC_ALIGNMENT = CM2_ALIGNMENT>
class vector_num : public cm2::math1::ref_counted
{

public:

///
typedef vector_num<T,VEC_ALIGNMENT>             self_type;
///
typedef T                                       value_type; 
///
typedef T&                                      reference;
///
typedef const T&                                const_reference;
///
typedef T*                                      pointer;
///
typedef const T*                                const_pointer;

// MS VISUAL .NET 2002
#if (_MSC_VER == 1300)
///
typedef std::_Ptrit<T, difference_type, pointer, reference,
                    pointer, reference>         iterator;
///
typedef std::_Ptrit<T, difference_type, const_pointer, const_reference,
                    pointer, reference>         const_iterator;
#else
///
typedef T*                                      iterator;
///
typedef const T*                                const_iterator;
#endif


#ifndef CM2_NO_CLASS_PARTIAL_SPECIALIZATION
/// 
typedef std::reverse_iterator<iterator>         reverse_iterator;     
///
typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
#else
/// 
typedef std::reverse_iterator<iterator, value_type, reference, difference_type>  
                                                reverse_iterator;     
///
typedef std::reverse_iterator<const_iterator, value_type, const_reference, difference_type>  
                                                const_reverse_iterator;
#endif   // CM2_NO_CLASS_PARTIAL_SPECIALIZATION


///
typedef cm2::strided<iterator,T>                strided_iterator;
///
typedef cm2::strided<const_iterator,T>          const_strided_iterator;
///
typedef cm2::strided<reverse_iterator,T>        reverse_strided_iterator;
///
typedef cm2::strided<const_reverse_iterator,T>  const_reverse_strided_iterator;
///
typedef ::size_t                                size_type;
///  
typedef ::ptrdiff_t                             difference_type;


public:

/// Default constructor.
vector_num() 
   : _p(NULL), _sz(0), _offset(0)
{ }

/// Non-initializing constructor.
explicit vector_num (size_type n)
   : _p(NULL), _sz(0), _offset(0)
{ 
   if (n > 0) 
      this->__grow(n, 0u, 1u);
}

/// Initializing constructor.
vector_num (size_type n, value_type init)
   : _p(NULL), _sz(0), _offset(0)
{ 
   if (n > 0) 
      this->__grow(n, 0u, 1u);

   if (_sz >= n)
      std::uninitialized_fill_n(_p, n, init);
}

/**
   Preallocated memory constructor.

   The user remains responsible for memory management. \n
   The vector is not allowed to delete the pointer even in case of destruction or 
   reallocation (through \ref reserve or \ref push_back).

   \param   n        The number of elements to consider starting from \p data.
   \param   data     The data array as a continuous vector.
                     Must have at least n elements.
                     Needs not to be aligned on a VEC_ALIGNMENT boundary.
   */
vector_num (size_type n, T* data)
   : _p(data), _sz(n), _offset(0xFFFF)
{ }

/// Destructor.
virtual ~vector_num()                          
{ 
   this->clear(); 
}


/**@name Iterator Access Methods */
//@{
///
INLINE pointer 
data()                                 { return _p; }
/// 
INLINE const_pointer 
data() const                           { return _p; }
/// 
INLINE const_pointer 
cdata() const                          { return _p; }

///
INLINE iterator 
begin()                                { return _p; }
///
INLINE const_iterator 
begin() const                          { return _p; }
///
INLINE const_iterator 
cbegin() const                         { return _p; }

///
INLINE iterator 
end()                                  { return _p + _sz; }
///
INLINE const_iterator 
end() const                            { return _p + _sz; }
///
INLINE const_iterator 
cend() const                           { return _p + _sz; }

///
INLINE reverse_iterator 
rbegin()                               { return reverse_iterator(this->end()); }
///
INLINE const_reverse_iterator
rbegin() const                         { return const_reverse_iterator(this->cend()); }
///
INLINE const_reverse_iterator
crbegin() const                        { return const_reverse_iterator(this->cend()); }

///
INLINE reverse_iterator 
rend()                                 { return reverse_iterator(this->begin()); }
///
INLINE const_reverse_iterator
rend() const                           { return const_reverse_iterator(this->cbegin()); }
///
INLINE const_reverse_iterator
crend() const                          { return const_reverse_iterator(this->cbegin()); }
//@}


/**@name Element Access Methods */
//@{
///
INLINE reference
operator[] (size_type i)               { return _p[i]; }
///
INLINE value_type 
operator[] (size_type i) const         { return _p[i]; }
//@}



/// Capacity.
INLINE size_type 
capacity() const                       { return _sz; }
/// Capacity.
INLINE size_type 
size() const                           { return _sz; }

/// Returns true when data overlap.
bool
overlaps (const self_type& x) const
{
   return (this->cbegin() < x.cend()) && (x.cbegin() < this->cend());
}

/// Returns true when data overlap.
bool
overlaps (const_iterator p0, const_iterator p1) const
{
   assert (p0 <= p1);
   return (this->cbegin() < p1) && (p0 < this->cend());
}

/// Returns true when data[i..i+sz( overlaps with [p..p+sz(.
bool
overlaps (size_t i, const_iterator p, size_t sz) const
{
   return (i+sz < _sz) && (_p+i < p+sz) && (p < _p+i+sz);
}

/**
   Reserve memory.

   \return     The actual size of the array (greater or equal to the requested value).
   \warning    A returned value lesser than the requested size indicates a memory allocation error.
   */
size_type 
reserve (size_type n, size_type recopy_upto, size_t growth_rate)                  
{ 
   if (_sz < n) 
      this->__grow(n, recopy_upto, growth_rate); 

   return _sz;
}

/**
   Clear. Deallocate the memory.
   */
INLINE void 
clear()        { this->__clear_array(); }

/// Assign a single val.
INLINE bool 
assign (size_type i, value_type v)  
{
   bool     ok = (_sz > i);

   assert (_sz+1 >= i);
   if (!ok)
      ok = this->__grow(i+1, /*recopy_upto=>*/ i, /*growth_rate=>*/ 2); 
   if (ok)
      *(_p+i) = v;

   return ok;
}  

/// Assign two vals.
INLINE bool 
assign (size_type i, value_type v0, value_type v1)  
{
   bool     ok = (_sz > i+1);

   assert (_sz+1 >= i);
   if (!ok)
      ok = this->__grow(i+2, /*recopy_upto=>*/ i, /*growth_rate=>*/ 2); 
   if (ok)
   {
      *(_p+i)   = v0;
      *(_p+i+1) = v1;
   }

   return ok;
}

/// Assign three vals.
INLINE bool 
assign (size_type i, value_type v0, value_type v1, value_type v2)  
{
   bool     ok = (_sz > i+2);

   assert (_sz+1 >= i);
   if (!ok)
      ok = this->__grow(i+3, /*recopy_upto=>*/ i, /*growth_rate=>*/ 2); 
   {
      *(_p+i)   = v0;
      *(_p+i+1) = v1;
      *(_p+i+2) = v2;
   }

   return ok;
}

/// Assign four vals.
INLINE bool 
assign (size_type i, value_type v0, value_type v1, value_type v2, value_type v3)  
{
   bool     ok = (_sz > i+3);

   assert (_sz+1 >= i);
   if (!ok)
      ok = this->__grow(i+4, /*recopy_upto=>*/ i, /*growth_rate=>*/ 2);  
   if (ok)
   {
      *(_p+i)   = v0;
      *(_p+i+1) = v1;
      *(_p+i+2) = v2;
      *(_p+i+3) = v3;
   }

   return ok;
}


/*------------------------ Protected members -----------------------*/
private:

// Macro related to buffer alignment.
#define  __CM2_ALIGN(p,align)                ((p+align-1) & ~(align-1))


/// Disable copy constructor.
vector_num (const self_type&);

/// Disable copy operator.
self_type& operator= (const self_type&);

///
bool __grow (size_type n, size_type recopy_upto, size_type growth_rate);

///
void __clear_array();



/// Pointer to buffer (aligned).
T*             _p;

/// Effective size (starting from _p).
size_type      _sz;

/** 
   Distance in bytes from unaligned pointer to _p (aligned).
   If _offset = 0xFFFF, the array is considered as protected (not to be freed).
   */
unsigned short _offset;

};



/// Aligned malloc.
//CM2MATH1_API CM2_NOINLINE void*
//__aligned_malloc (size_t size, size_t alignment);

/// Aligned free.
//CM2MATH1_API CM2_NOINLINE void
//__aligned_free (void* p);

/// Raw memory allocation.
CM2MATH1_API CM2_NOINLINE void*
__vector_num_alloc (size_t sz);

/// Raw memory deallocation.
CM2MATH1_API CM2_NOINLINE void
__vector_num_free (void* p);



// Template construction function.
template <class T>
bool __vec_construction (T* first, T* last)
{
   bool     ok(true);
   T*       p;

   try
   {  for (p = first ; p != last; ++p) new(p) T(); }        // T default constructor called.
   catch (...)
   { 
      for (; first != p; ++first) first->~T();              // T default destructor called.
      ok = false; 
   };

   return ok;
}
// Specializations for PODs.
template<> INLINE bool __vec_construction<int> (int*, int*)                                      { return true; }
template<> INLINE bool __vec_construction<unsigned> (unsigned*, unsigned*)                       { return true; }
template<> INLINE bool __vec_construction<char> (char*, char*)                                   { return true; }
template<> INLINE bool __vec_construction<unsigned char> (unsigned char*, unsigned char*)        { return true; }
template<> INLINE bool __vec_construction<short> (short*, short*)                                { return true; }
template<> INLINE bool __vec_construction<unsigned short> (unsigned short*, unsigned short*)     { return true; }
template<> INLINE bool __vec_construction<long> (long*, long*)                                   { return true; }
template<> INLINE bool __vec_construction<unsigned long> (unsigned long*, unsigned long*)        { return true; }
template<> INLINE bool __vec_construction<double> (double*, double*)                             { return true; }
template<> INLINE bool __vec_construction<float> (float*, float*)                                { return true; }
template<> INLINE bool __vec_construction<std::complex<double> > (std::complex<double>*, std::complex<double>*)   { return true; }
#ifdef WIN64
template<> INLINE bool __vec_construction<__int64> (__int64*, __int64*)                          { return true; }
template<> INLINE bool __vec_construction<unsigned __int64> (unsigned __int64*, unsigned __int64*)  { return true; }
#endif

// Template destruction function.
template <class T>
void __vec_destruction (T* first, T* last)
{
   for ( ; first != last; ++first) first->~T();             // T destructor called.
}
// Specializations for PODs.
template<> INLINE void __vec_destruction<int> (int*, int*)                                       { }
template<> INLINE void __vec_destruction<unsigned> (unsigned*, unsigned*)                        { }
template<> INLINE void __vec_destruction<char> (char*, char*)                                    { }
template<> INLINE void __vec_destruction<unsigned char> (unsigned char*, unsigned char*)         { }
template<> INLINE void __vec_destruction<short> (short*, short*)                                 { }
template<> INLINE void __vec_destruction<unsigned short> (unsigned short*, unsigned short*)      { }
template<> INLINE void __vec_destruction<long> (long*, long*)                                    { }
template<> INLINE void __vec_destruction<unsigned long> (unsigned long*, unsigned long*)         { }
template<> INLINE void __vec_destruction<double> (double*, double*)                              { }
template<> INLINE void __vec_destruction<float> (float*, float*)                                 { }
template<> INLINE void __vec_destruction<std::complex<double> > (std::complex<double>*, std::complex<double>*)    { }
#ifdef WIN64
template<> INLINE void __vec_destruction<__int64> (__int64*, __int64*)                           { }
template<> INLINE void __vec_destruction<unsigned __int64> (unsigned __int64*, unsigned __int64*)  { }
#endif



// DEFINITION OF NON-INLINE MEMBERS //

///
template <class T, int VEC_ALIGNMENT>
bool
vector_num<T,VEC_ALIGNMENT>::__grow (size_type n, size_type recopy_upto, size_type growth_rate)
{
   const size_type         N0    = std::max(size_t(2u), growth_rate * _sz);
   const size_type         N     = std::max(N0, n);
   const size_type         ALIGN = std::max(size_type(VEC_ALIGNMENT), sizeof(T));
   const size_type         SZ    = N * sizeof(T) + (ALIGN - 1);
   size_type               sz, offset;    
   void*                   pu;
   T*                      p;
   bool                    ok;

   assert (n > _sz);
   assert (growth_rate >= 1);
   assert (recopy_upto <= _sz);
   assert ((recopy_upto == 0) || (_p != NULL));

   // Basic checks.
   if (_sz > size_t(-1) / growth_rate)
      return false;
   if (N > size_t(-1) / sizeof(T))
      return false;

   // Raw allocation.
   pu = cm2::__vector_num_alloc(SZ);                              // Unaligned pointer.
//   assert(pu != NULL);                                            // Debug only.
   if (pu == NULL)                                                // Alloc failed.
      return false;

   p = reinterpret_cast<T*>(__CM2_ALIGN(reinterpret_cast<size_t>(pu), ALIGN));     // Aligned pointer.
   assert (reinterpret_cast<char*>(p) - reinterpret_cast<char*>(pu) < 0xFFFF);
   offset = reinterpret_cast<char*>(p) - reinterpret_cast<char*>(pu);
   sz = (SZ - offset) / sizeof(T);
   assert (sz >= n);

   // Construction of the elements.
   if (_p != NULL)
      std::uninitialized_copy(_p, _p + recopy_upto, p);           // T copy constructor called.
   ok = cm2::__vec_construction(p + recopy_upto, p + sz);         // T default constructor called.

   // If construction failed, release memory.
   if (!ok)    
   {
      cm2::__vector_num_free(pu);
      return false;
   }

   // Deallocation of previous buffer.
   this->__clear_array();
   _p = p;
   _offset = static_cast<unsigned char>(offset);
   _sz = p ? sz : 0;

   return true;
}

///
template <class T, int VEC_ALIGNMENT>
void
vector_num<T,VEC_ALIGNMENT>::__clear_array() 
{
   if ((_p != NULL) && (_offset != 0xFFFF))
   {
      // Destruction.
      cm2::__vec_destruction(_p, _p + _sz);                       // T destructor called.

      // Deallocation.
      void*    pu = reinterpret_cast<void*>(reinterpret_cast<char*>(_p) - _offset);
      cm2::__vector_num_free(pu);
   }
   _p = NULL;
   _sz = 0;
   _offset = 0;
}


} // end namespace cm2

#endif      // __CM2_VECTOR_NUM_H__

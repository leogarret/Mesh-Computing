/**
   \file       vector_fixed.h
   \brief      Definition of fixed-sized template vector.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_VECTOR_FIXED_H__
#define __CM2_VECTOR_FIXED_H__


#include "matvec_tags.h"
#include "strided_iterator.h"
#include "vecscal.h"
#include "vecvec.h"


namespace cm2 { 

/**
   Template vector with compile-time fixed size.

   Template type `T` can be a POD (`unsigned`, `double`, `char`...) or a user-defined class
   (with visible default constructor, copy constructor, destructor and assignment operator).

   This class is simpler and more efficient than dense1D (no smart pointer overhead)
   but the size must be known at compile time and cannot be changed. \n
   Another major difference with dense1D concerns the copy constructors and operators. \n
   The copies here are _hard_ copies, not _shallow_ copies.

   \tparam     T     Any type with default constructor, copy constructor, equal operator and assignment operator.
   \tparam     N     The size of the vector (fixed unsigned value).
   \note       Similar to STL `std::array` containers (STL C++ 11).
   \see        dense1D, matrix_fixed, symmetric_fixed.
   */
template <class T, size_t N>
class vector_fixed 
{

// The data.
//CM2_ALIGN(CM2_ALIGNMENT)   T  d[N];
T     d[N];

public:

/// The compile-time fixed size = N.
enum { SIZE = N };

/// self_type.
typedef vector_fixed<T,N>                             self_type;
/// value_type.
typedef T                                             value_type; 
/// reference
typedef T&                                            reference;
/// const_reference
typedef const T&                                      const_reference;
/// pointer
typedef T*                                            pointer;
/// const_pointer
typedef const T*                                      const_pointer;
/// size_type
typedef ::size_t                                      size_type;
/// difference_type
typedef ::ptrdiff_t                                   difference_type;

// MS VISUAL .NET 2002
#if (_MSC_VER == 1300)
///
typedef std::_Ptrit<T, difference_type, pointer, reference,
                    pointer, reference>               iterator;
///
typedef std::_Ptrit<T, difference_type, const_pointer, const_reference,
                    pointer, reference>               const_iterator;
#else
///
typedef T*                                            iterator;
///
typedef const T*                                      const_iterator;
#endif


#ifndef CM2_NO_CLASS_PARTIAL_SPECIALIZATION
/// 
typedef std::reverse_iterator<iterator>               reverse_iterator;     
///
typedef std::reverse_iterator<const_iterator>         const_reverse_iterator;

#else
/// 
typedef std::reverse_iterator<iterator, value_type, reference, difference_type>  
                                                      reverse_iterator;     
///
typedef std::reverse_iterator<const_iterator, value_type, const_reference, difference_type>  
                                                      const_reverse_iterator;
#endif   // CM2_NO_CLASS_PARTIAL_SPECIALIZATION

/// strided_iterator
typedef cm2::strided<iterator,T>                      strided_iterator;
/// const_strided_iterator
typedef cm2::strided<const_iterator,T>                const_strided_iterator;
/// reverse_strided_iterator
typedef cm2::strided<reverse_iterator,T>              reverse_strided_iterator;
/// const_reverse_strided_iterator
typedef cm2::strided<const_reverse_iterator,T>        const_reverse_strided_iterator;

/// dimension_tag
typedef cm2::oneD_tag                                 dimension_tag;
/// size_tag
typedef cm2::fixed_size_tag<N>                        size_tag;
/// fullness_tag
typedef cm2::full_tag                                 fullness_tag;


/**
   Default constructor.
   Elements are uninitialized.
   */
INLINE vector_fixed()                        
{ }

/**
   Constructor with a common initializing value.
   All the `N` elements are initialized to \p v.
   */
INLINE explicit vector_fixed (value_type v)           
{ 
   vecscal::copy_n(v, this->begin(), N);
}

/**
   Constructor with 2 initializing values.
   Range check performed in DEBUG mode only.

   \pre N = 2.
   */
template <class T0_, class T1_>
vector_fixed (T0_ v0, T1_ v1)               
{ 
   assert (N == 2);
   d[0] = v0; 
   d[1] = v1; 
}

/**
   Constructor with 3 initializing values.
   Range check performed in DEBUG mode only.

   \pre N = 3.
   */
template <class T0_, class T1_, class T2_>
vector_fixed (T0_ v0, T1_ v1, T2_ v2)          
{ 
   assert (N == 3);
   d[0] = v0; 
   d[1] = v1; 
   d[2] = v2; 
}

/**
   Constructor with 4 initializing values.
   Range check performed in DEBUG mode only.

   \pre N = 4.
   */
template <class T0_, class T1_, class T2_, class T3_>
vector_fixed (T0_ v0, T1_ v1, T2_ v2, T3_ v3)     
{ 
   assert (N == 4);
   d[0] = v0; 
   d[1] = v1; 
   d[2] = v2; 
   d[3] = v3;
}

/**
   Constructor with 5 initializing values.
   Range check performed in DEBUG mode only.

   \pre N = 5.
   */
template <class T0_, class T1_, class T2_, class T3_, class T4_>
vector_fixed (T0_ v0, T1_ v1, T2_ v2, T3_ v3, T4_ v4)     
{ 
   assert (N == 5);
   d[0] = v0; 
   d[1] = v1; 
   d[2] = v2; 
   d[3] = v3;
   d[4] = v4;
}

/**
   Constructor with 6 initializing values.
   Range check performed in DEBUG mode only.

   \pre N = 6.
   */
template <class T0_, class T1_, class T2_, class T3_, class T4_, class T5_>
vector_fixed (T0_ v0, T1_ v1, T2_ v2, T3_ v3, T4_ v4, T5_ v5)     
{ 
   assert (N == 6);
   d[0] = v0; 
   d[1] = v1; 
   d[2] = v2; 
   d[3] = v3;
   d[4] = v4;
   d[5] = v5;
}

/**
   Constructor with 8 initializing values.
   Range check performed in DEBUG mode only.

   \pre N = 8.
   */
template <class T0_, class T1_, class T2_, class T3_, class T4_, class T5_, class T6_, class T7_>
vector_fixed (T0_ v0, T1_ v1, T2_ v2, T3_ v3, T4_ v4, T5_ v5, T6_ v6, T7_ v7)     
{ 
   assert (N == 8);
   d[0] = v0; 
   d[1] = v1; 
   d[2] = v2; 
   d[3] = v3;
   d[4] = v4;
   d[5] = v5;
   d[6] = v6;
   d[7] = v7;
}

/**
   Copy constructor.  Hard copy.
   */
INLINE vector_fixed (const self_type& V) 
{ 
   vecvec::copy_n(V.begin(), this->begin(), N);
}

/**
   Constructor from pre-allocated memory (deep copy).

   \p N elements starting from pointer \p p are copied to the vector.

   \pre  The N elements from \p p to \p p+N-1 must be valid.
   */
INLINE vector_fixed (const T* p)   
{ 
   vecvec::copy_n(p, this->begin(), N);
}



/**@name Iterator access members */
//@{
/// Pointer to the first element. Same as begin().
INLINE pointer         
data()                              { return &d[0]; }
/// Pointer to the first element (const). Same as cbegin().
INLINE const_pointer 
data() const                        { return &d[0]; }
/// Pointer to the first element (const). Same as cbegin().
INLINE const_pointer 
cdata() const                       { return &d[0]; }

/// STL-compatible begin. Pointer to the first element.
INLINE iterator         
begin()                             { return &d[0]; }
/// STL-compatible begin. Pointer to the first element.
INLINE const_iterator   
begin() const                       { return &d[0]; }
/// STL-compatible cbegin. Pointer to the first element.
INLINE const_iterator   
cbegin() const                      { return &d[0]; }

/// STL-compatible end. Pointer to the past-the-end element.
INLINE iterator         
end()                               { return this->begin() + N; }
/// STL-compatible end. Pointer to the past-the-end element.
INLINE const_iterator   
end() const                         { return this->begin() + N; }
/// STL-compatible cend. Pointer to the past-the-end element.
INLINE const_iterator   
cend() const                        { return this->begin() + N; }
//@}



/**@name Value and reference access operators */
//@{
/**
   Returns the i-th element (by reference).
   Range check performed in DEBUG mode.

   \pre \p i < size(). 
   */
template <class Index_>
reference               
operator[] (Index_ i) 
{
   assert (size_type(i) < N);
   return d[i]; 
}

/**
   Returns the i-th element (by value).
   Range check performed in DEBUG mode.

   \pre \p i < size(). 
   */
template <class Index_>
value_type         
operator[] (Index_ i) const 
{ 
   assert (size_type(i) < N);
   return d[i]; 
}

/**
   Gets the i-th element. Same as operator[] const.

   \pre  i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
value_type 
at (Index_ i) const
{
   assert (size_type(i) < N);
   return d[i]; 
}

/**
   Gets the i-th element. Same as operator[] const.

   \pre  i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
void 
get_val (Index_ i, reference v) const
{
   assert (size_type(i) < N);
   v = d[i];
}

/**
   Sets the i-th element. Same as operator[].

   \pre  i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
void 
set_val (Index_ i, value_type v)
{
   assert (size_type(i) < N);
   d[i] = v;
}

/// Returns the front element. 
INLINE value_type 
front() const     { return d[0]; }
/// Returns the front element (by reference). 
INLINE reference 
front()           { return d[0]; }
/// Returns the last element. 
INLINE value_type 
back() const      { return d[N-1]; }
/// Returns the last element (by reference). 
INLINE reference 
back()            { return d[N-1]; }
//@}



/**@name Operators */
//@{
/// Scalar assignment.
INLINE value_type    
operator= (value_type v) 
{ 
   vecscal::copy_n(v, this->begin(), N);
   return v;
}

/// Copy operator (copy data).
INLINE self_type& 
operator= (const self_type& x) 
{ 
   vecvec::copy_n(x.begin(), this->begin(), N);
   return *this; 
}

/// Equality operator.
INLINE bool    
operator== (const self_type& x) const
{ 
   return vecvec::equal_n(x.begin(), this->begin(), N);
}

/// Inequality operator.
INLINE bool    
operator!= (const self_type& x) const
{ 
   return !vecvec::equal_n(x.begin(), this->begin(), N);
}
//@}



/**@name Size members */
//@{  
/**
   The number of elements stored in the vector (same as size).
   This is a constant static value (= N).
   */
INLINE static size_type 
entries()                                 { return N; }
/**
   The number of elements in the vector.
   This is a constant static value (= N).
   */
INLINE static size_type 
size()                                    { return N; }
/// The leading dimension of the vector. Same as size().
INLINE size_type 
ld() const                                { return N; }
/**
   The size in bytes of the content of the matrix.
   This is a constant static value (= N * sizeof(T)).
   */
INLINE static size_t
size_of()                                 { return N * sizeof(T); }
/// True when the vector is empty.
INLINE static bool 
empty()                                   { return N == 0; }
//@}


/// Deep copy.
bool
copy (const self_type& x)
{
   vecvec::copy_n(x.begin(), this->begin(), N);
   return true;
}


/**@name I/O members (binary) */
//@{  
/**
   Reads data from a binary input stream.

   \tparam        IStream  Input stream class.
   \param[in,out] s        The input stream (template parameter).
   \return        0 (good) or -1 (IO error).
  */
template <class IStream>
int
load (IStream& s)
{  
   s.read(static_cast<char*>(this->data()), this->size_of());     // read data

   return s.good() ? 0 : -1;
}

/**
   Prints all elements of a vector to a binary output stream.

   \tparam        OStream  Output stream class.
   \param[in,out] s        The output stream (template parameter).
   */
template <class OStream>
void
save (OStream& s) const
{
   s.write(static_cast<const char*>(this->data()), this->size_of());     // write data
}
//@}

};

} // end namespace cm2

#endif

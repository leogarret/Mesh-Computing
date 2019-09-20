/**
   \file       symmetric_fixed.h
   \brief      Definition of fixed-sized symmetric template matrix.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_SYMMETRIC_FIXED_H__
#define __CM2_SYMMETRIC_FIXED_H__


#include "vector_fixed.h"


namespace cm2 { 

/**
   Template symmetric matrix with compile-time fixed size.

   Template type `T` can be a POD (`unsigned`, `double`, `char`...) or a user-defined class
   (with visible default constructor, copy constructor, destructor and assignment operator).

   This class is simpler and more efficient than symmetric_full (no smart pointer overhead)
   but the size must be known at compile time and cannot be changed. \n
   Another big difference with symmetric_full concerns the copy constructors and operators. \n
   The copies here are _hard_ copies, not _shallow_ copies.

   The data are stored contiguously in memory and are column oriented (column-major order).
   \verbatim
   [0] [1] [3] [6]
       [2] [4] [7]
           [5] [8]
               [9]
   \endverbatim

   \tparam     T     Any type with default constructor, copy constructor, equal operator and assignment operator.
   \tparam     N     The dimension of the matrix, i.e. number of rows and columns (fixed unsigned value).
   \see        symmetric_full.
   */
template <class T, size_t N>
class symmetric_fixed : protected vector_fixed<T, (N*(N+1))/2> 
{

public:

/// self_type
typedef symmetric_fixed<T, N>                            self_type;
/// self_type
typedef symmetric_fixed<T, N>                            matrix_type;
/// parent_type
typedef vector_fixed<T, (N*(N+1))/2>                     parent_type;

/// value_type
typedef T                                                value_type;
/// reference
typedef typename parent_type::reference                  reference;
/// const_reference
typedef typename parent_type::const_reference            const_reference;
/// pointer.
typedef typename parent_type::pointer                    pointer;
/// const_pointer.
typedef typename parent_type::const_pointer              const_pointer;
/// iterator
typedef typename parent_type::iterator                   iterator;
/// const_iterator
typedef typename parent_type::const_iterator             const_iterator;
/// size_type
typedef typename parent_type::size_type                  size_type;
/// difference_type
typedef typename parent_type::difference_type            difference_type;

/// transpose_type
typedef symmetric_fixed<T,N>                             transpose_type;

/// dimension_tag
typedef cm2::twoD_tag                                    dimension_tag;
/// shape_tag
typedef cm2::symmetric_tag                               shape_tag;
/// orientation_tag
typedef cm2::col_oriented_tag                            orientation_tag;
/// storage_tag
typedef cm2::upper_right_storage_tag                     storage_tag;
/// blocking_tag
typedef cm2::non_blocked_tag                             blocking_tag;
/// diagonal_storage_tag
typedef cm2::non_separate_diagonal_tag                   diagonal_storage_tag;
/// fullness_tag
typedef cm2::full_tag                                    fullness_tag;
/// size_tag
typedef cm2::fixed_size_tag<(N*(N+1))/2>                 size_tag;
/// OOC_management_tag
typedef cm2::non_OOC_tag                                 OOC_management_tag;



/// The compile-time fixed dimensions: ROWS = M, COLS = N.
enum
{ 
   ROWS = N, 
   COLS = N, 
   DIM  = N,
   SIZE = parent_type::SIZE
};


/**
   A segment_type is a view to a column of a matrix. \n
   It is equipped with members such as begin(), end() and size() which make it a candidate
   for most of the STL algorithms and cm2::vecscal, cm2::vecvec functions.

   A segment_type is also an iterator through the segments (i.e. columns) of a matrix. \n
   A segment_type can be incremented and decremented to view the next or the previous segment
   in a matrix.
   */
class segment_type 
{
   T*                                  _d;
   typename matrix_type::size_type     _j;

   typedef segment_type                            self_type;
public:
   typedef T                                       value_type; 
   typedef T&                                      reference;
   typedef const T&                                const_reference;
   typedef T*                                      iterator;
   typedef const T*                                const_iterator;
   typedef typename matrix_type::size_type         size_type;
   typedef typename matrix_type::difference_type   difference_type;
   typedef cm2::oneD_tag                           dimension_tag;
   typedef cm2::full_tag                           fullness_tag;

   typedef T*                                      pointer;
   typedef const T*                                const_pointer;
   typedef std::random_access_iterator_tag         iterator_category;

   INLINE segment_type() : _d(NULL), _j(0) { }
   INLINE segment_type (matrix_type& A, size_type j) 
      : _d(A.data()+(j*(j+1))/2), _j(j)                     { assert(j <= N); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += (++_j); }
   INLINE void next (size_type s)                           { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE iterator data()                                   { assert(_j < N); return _d; }
   INLINE const_iterator data() const                       { assert(_j < N); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < N); return _d; }
   INLINE iterator begin()                                  { assert(_j < N); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < N); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < N); return _d; }
   INLINE iterator end()                                    { return _d + _j+1; }
   INLINE const_iterator end() const                        { return _d + _j+1; }
   INLINE const_iterator cend() const                       { return _d + _j+1; }
   INLINE size_type size() const                            { return _j+1; }
   INLINE static bool empty()                               { return (N == 0); }
   INLINE value_type front() const                          { return *_d; }
   INLINE reference front()                                 { return *_d; }
   INLINE value_type back() const                           { return *(_d+_j); }
   INLINE reference back()                                  { return *(_d+_j); }
   INLINE value_type operator[] (size_type i) const         { assert(i <= _j); return *(_d+i); }
   INLINE reference operator[] (size_type i)                { assert(i <= _j); return *(_d+i); }

   INLINE self_type* operator->()                           { return this; }
   INLINE self_type& operator*()                            { return *this; }
   INLINE self_type& operator++()                           { next(); return *this; }
   INLINE self_type& operator--()                           { next(difference_type(-1)); return *this; }
   INLINE self_type  operator++(int)                        { const self_type cpy(*this); next(); return cpy; }
   INLINE self_type  operator--(int)                        { const self_type cpy(*this); next(difference_type(-1)); return cpy; }
   INLINE self_type& operator+=(size_type s)                { next(s); return *this; }
   INLINE self_type& operator-=(size_type s)                { next(-difference_type(s)); return *this; }
   INLINE self_type  operator+ (size_type s) const          { self_type cpy(*this); cpy.next(s); return cpy; }
   INLINE self_type  operator- (size_type s) const          { self_type cpy(*this); cpy.next(-difference_type(s)); return cpy; }
   INLINE difference_type operator- (const self_type& x) const { return static_cast<difference_type>(_j - x._j); }
   INLINE bool operator== (const self_type& x) const        { return _d == x._d; }
   INLINE bool operator!= (const self_type& x) const        { return _d != x._d; }
   INLINE bool operator< (const self_type& x) const         { return _d < x._d; }
};

typedef segment_type       segment_iterator;          // Alias.


/**
   A const_segment_type is a const-view to a column of a matrix. \n
   It is equipped with members such as begin(), end() and size() which make it a candidate
   for most of the STL algorithms and cm2::vecscal, cm2::vecvec functions.

   A const_segment_type is also an iterator through the segments (i.e. columns) of a matrix. \n
   A const_segment_type can be incremented and decremented to view the next or the previous segment
   in a matrix.
   */
class const_segment_type 
{
   const T*                            _d;
   typename matrix_type::size_type     _j;

   typedef const_segment_type                      self_type;
public:
   typedef T                                       value_type; 
   typedef const T&                                reference;
   typedef const T&                                const_reference;
   typedef const T*                                iterator;
   typedef const T*                                const_iterator;
   typedef typename matrix_type::size_type         size_type;
   typedef typename matrix_type::difference_type   difference_type;
   typedef cm2::oneD_tag                           dimension_tag;
   typedef cm2::full_tag                           fullness_tag;

   typedef const T*                                pointer;
   typedef const T*                                const_pointer;
   typedef std::random_access_iterator_tag         iterator_category;

   INLINE const_segment_type() : _d(NULL), _j(0) { }
   INLINE const_segment_type (const matrix_type& A, size_type j) 
      : _d(A.data()+(j*(j+1))/2), _j(j)                     { assert(j <= N); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += (++_j); }
   INLINE void next (size_type s)                           { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE const_iterator data() const                       { assert(_j < N); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < N); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < N); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < N); return _d; }
   INLINE const_iterator end() const                        { return _d + _j+1; }
   INLINE const_iterator cend() const                       { return _d + _j+1; }
   INLINE size_type size() const                            { return _j+1; }
   INLINE static bool empty()                               { return (N == 0); }
   INLINE value_type front() const                          { return *_d; }
   INLINE value_type back() const                           { return *(_d+_j); }
   INLINE value_type operator[] (size_type i) const         { assert(i <= _j); return *(_d+i); }

   INLINE const self_type* operator->() const               { return this; }
   INLINE const self_type& operator*() const                { return *this; }
   INLINE self_type& operator++()                           { next(); return *this; }
   INLINE self_type& operator--()                           { next(difference_type(-1)); return *this; }
   INLINE self_type  operator++(int)                        { const self_type cpy(*this); next(); return cpy; }
   INLINE self_type  operator--(int)                        { const self_type cpy(*this); next(difference_type(-1)); return cpy; }
   INLINE self_type& operator+=(size_type s)                { next(s); return *this; }
   INLINE self_type& operator-=(size_type s)                { next(-difference_type(s)); return *this; }
   INLINE self_type  operator+ (size_type s) const          { self_type cpy(*this); cpy.next(s); return cpy; }
   INLINE self_type  operator- (size_type s) const          { self_type cpy(*this); cpy.next(-difference_type(s)); return cpy; }
   INLINE difference_type operator- (const self_type& x) const { return static_cast<difference_type>(_j - x._j); }
   INLINE bool operator== (const self_type& x) const        { return _d == x._d; }
   INLINE bool operator!= (const self_type& x) const        { return _d != x._d; }
   INLINE bool operator< (const self_type& x) const         { return _d < x._d; }
};

typedef const_segment_type       const_segment_iterator;       // Alias.



/**
   Default constructor.

   Elements are uninitialized.
   */
INLINE symmetric_fixed() { }

/**
   Constructor with a common initializing value.

   All elements are initialized to \p x.
   */
INLINE explicit symmetric_fixed (value_type v) 
   : parent_type(v) 
{ }

/**
   Copy constructor.

   Hard copy.
   */
INLINE symmetric_fixed (const self_type& A) 
   : parent_type(A) 
{ }

/**
   Constructor with 3 initializing values.

   First three elements are initialized to \p v00, \p v01, \p v11 in that order.

   \pre DIM = 2.
   */
INLINE symmetric_fixed (value_type v00, value_type v01, value_type v11)
   : parent_type(v00, v01, v11) 
{ 
   assert(N == 2);
}

/**
   Constructor with 6 initializing values.

   First six elements are initialized to \p v00, \p v01, \p v11, \p v02, \p v12, \p v22 in that order.

   \pre DIM = 3.
   */
INLINE symmetric_fixed (value_type v00, value_type v01, value_type v11, 
                        value_type v02, value_type v12, value_type v22)
   : parent_type(v00, v01, v11, v02, v12, v22) 
{ 
   assert(N == 3);
}

/**
   Constructor from pre-allocated memory (deep copy).

   (N*(N+1))/2 elements starting from pointer \p d are copied to the vector.

   \pre  The (N*(N+1))/2 elements from pointer \p d must be valid.
   */
INLINE symmetric_fixed (const T* d) 
   : parent_type(d) 
{ }



/**@name Operators */
//@{
/// Scalar assignment.
INLINE value_type
operator= (value_type v) 
{ 
   return parent_type::operator= (v);
}

/// Copy operator (copy data).
self_type& 
operator= (const self_type& x) 
{ 
   vecvec::copy_n(x.cdata(), this->data(), size_t(SIZE));
   return *this; 
}

/// Equality operator.
INLINE bool    
operator== (const self_type& V) const
{ 
   return parent_type::operator== (V);
}

/// Inequality operator.
INLINE bool    
operator!= (const self_type& V) const
{ 
   return parent_type::operator!= (V);
}
//@}



/**@name Size members */
//@{  

/// First index.
INLINE static size_t 
first_index()                       { return 0; }

/// Past-the-end index.
INLINE static size_t 
past_index()                        { return N; }

/// Dimension of the matrix (constant static value = N).
INLINE static size_type 
dim()                               { return N; }

/// Number of rows (i.e. the dimension) of the matrix (constant static value = N).
INLINE static size_type 
rows()                              { return N; }

/// Number of columns (i.e. the dimension) of the matrix (constant static value = N).
INLINE static size_type 
cols()                              { return N; }

/// Returns true if the matrix is empty.
INLINE static bool 
empty()                             { return parent_type::empty(); }

/// Number of elements in the matrix (constant static value = (N*(N+1))/2).
INLINE static size_type 
entries()                           { return parent_type::entries(); }

/// Same as entries.
INLINE size_t
segments_entries() const            { return parent_type::entries(); }

/**
   Number of elements stored in the matrix (same as entries).
   This is a constant static value (= (N*(N+1))/2).
   */
INLINE static size_type 
size()                              { return parent_type::size(); }

/**
   Size in bytes of the content of the matrix.
   This is a constant static value (= (N*(N+1))/2 * sizeof(T)).
   */
INLINE static size_t
size_of()                           { return parent_type::size_of(); }
//@}


/// Returns true if the j-th column can be accessed.
INLINE static bool
is_valid_col (size_type j)          { return j < N; }



/**@name Segment iterators
   The segment_iterator and const_segment_iterator can be seen as views
   to the column data of a matrix. \n
   They are equipped with members such as begin(), end() and size() which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/// Number of segments (constant static value = N).
INLINE size_type 
segments() const                    { return N; }
/// Iterator to the first segment.
INLINE segment_iterator 
seg_begin()                         { return segment_iterator(*this, 0); }
/// Iterator to the first segment (const).
INLINE const_segment_iterator 
seg_begin() const                   { return const_segment_iterator(*this, 0); }
/// Iterator to the first segment (const).
INLINE const_segment_iterator 
cseg_begin() const                  { return const_segment_iterator(*this, 0); }
/// Iterator to the past-the-end segment.
INLINE segment_iterator 
seg_end()                           { return segment_iterator(*this, this->cols()); }
/// Iterator to the past-the-end segment (const).
INLINE const_segment_iterator 
seg_end() const                     { return const_segment_iterator(*this, this->cols()); }
/// Iterator to the past-the-end segment (const).
INLINE const_segment_iterator 
cseg_end() const                    { return const_segment_iterator(*this, this->cols()); }

/**
   Returns a view to the column \p j of the matrix. \n
   The data are not copied (shared).

   Range check performed in DEBUG mode only.

   \pre j < N. 
   */
template <class Index_>
segment_type 
seg (Index_ j)               
{ 
   assert(size_type(j) < N);
   return segment_type(*this, j); 
}

/**
   Returns a const-view to the column \p j of the matrix. \n
   The data are not copied (shared).

   Range check performed in DEBUG mode only.

   \pre j < N. 
   */
template <class Index_>
const_segment_type 
seg (Index_ j) const         
{ 
   assert(size_type(j) < N);
   return const_segment_type(*this, j); 
}

/**
   Returns a const-view to the column \p j of the matrix. \n
   The data are not copied (shared).

   Range check performed in DEBUG mode only.

   \pre j < N. 
   */
template <class Index_>
const_segment_type 
cseg (Index_ j) const         
{ 
   assert(size_type(j) < N);
   return const_segment_type(*this, j); 
}

/// Same as \ref seg.
segment_type
operator[] (size_type j)            { return this->seg(j); }

/// Same as \ref seg.
const_segment_type
operator[] (size_type j) const      { return this->seg(j); }
//@}


/**@name Data iterators */
//@{

/**
   Pointer to the first element.

   Equivalent to segment(0).begin().
   */
INLINE typename self_type::pointer         
data()                              { return parent_type::data(); }

/**
   Pointer to the first element (const).

   Equivalent to segment(0).cbegin().
   */
INLINE typename self_type::const_pointer        
data() const                        { return parent_type::cdata(); }

/**
   Pointer to the first element (const).

   Equivalent to segment(0).cbegin().
   */
INLINE typename self_type::const_pointer        
cdata() const                       { return parent_type::cdata(); }

/**
   Pointer to the first element.

   Equivalent to segment(0).begin().
   */
INLINE typename self_type::iterator         
begin()                             { return parent_type::data(); }

/**
   Pointer to the first element (const).
   Equivalent to segment(0).cbegin().
   */
INLINE typename self_type::const_iterator         
begin() const                       { return parent_type::cdata(); }

/**
   Pointer to the first element (const).
   Equivalent to segment(0).cbegin().
   */
INLINE typename self_type::const_iterator         
cbegin() const                      { return parent_type::cdata(); }

/**
   Pointer to the past-the-end element.
   Equivalent to segment(cols()-1).end().
   */
INLINE typename self_type::iterator         
end()                               { return parent_type::end(); }

/**
   Pointer to the past-the-end element (const).
   Equivalent to segment(cols()-1).end().
   */
INLINE typename self_type::const_iterator         
end() const                         { return parent_type::cend(); }

/**
   Pointer to the past-the-end element (const).
   Equivalent to segment(cols()-1).end().
   */
INLINE typename self_type::const_iterator         
cend() const                        { return parent_type::cend(); }

/**
   Pointer to the first element of column \p j.
   Equivalent to segment(j).begin().
   \pre  j < N
   */
template <class Index_>
typename self_type::iterator         
data_col (Index_ j)               
{ 
   assert(size_type(j) < N);
   return this->begin() + j*(j+1)/2; 
}

/**
   Pointer to the first element of column \p j (const).
   Equivalent to segment(j).begin().
   \pre  j < N
   */
template <class Index_>
typename self_type::const_iterator
cdata_col (Index_ j) const         
{ 
   assert(size_type(j) < N);
   return this->cbegin() + j*(j+1)/2; 
}

/**
   Pointer to the first element of column \p j (const).
   Equivalent to segment(j).begin().
   \pre  j < N
   */
template <class Index_>
typename self_type::const_iterator
data_col (Index_ j) const         
{ 
   assert(size_type(j) < N);
   return this->cbegin() + j*(j+1)/2; 
}
//@}



/**@name Access operators */
//@{
/**
   Returns the element (i,j) (reference).
   Element (i,j) and (j,i) are the same element.
   Range check performed in DEBUG mode.

   \pre \p i < N. 
   \pre \p j < N. 
   */
template <class Index_>
INLINE reference   
operator() (Index_ i, Index_ j) 
{
   if (i > j) std::swap(i, j);
   assert(size_type(j) < N);
   return parent_type::operator[](i + (j*(j+1))/2); 
}

/**
   Returns the element (i,j).
   Element (i,j) and (j,i) are the same element.
   Range check performed in DEBUG mode.
   \pre \p i < N. 
   \pre \p j < N. 
   */
template <class Index_>
INLINE value_type    
operator() (Index_ i, Index_ j) const 
{ 
   if (i > j) std::swap(i, j);
   assert(size_type(j) < N);
   return parent_type::operator[](i + (j*(j+1))/2); 
}
//@}


/**@name Views as vector */
//@{  
/**
   Returns the matrix as a vector (const). 
   Data are copied.
   */
parent_type
as_vector() const                   { return parent_type(*this); }
//@}


/**@name I/O members (binary) */
//@{  
/**
   Reads data from a binary input stream.

   \tparam        IStream  Input stream class.
   \param[in,out] s        The input stream (template parameter).
   \return                 0 (good) or -1 (IO error).
  */
template <class IStream>
int
load (IStream& s)                   { return parent_type::load(s); }

/**
   Writes data to a binary output stream.

   \tparam        OStream  Output stream class.
   \param[in,out] s        The output stream (template parameter).
   */
template <class OStream>
void
save (OStream& s) const             { parent_type::save(s); }
//@}


/**@name Miscellaneous */
//@{  
/// Diagonal is not unity.
INLINE static bool   
is_unit_diagonal()                  { return false; }
/// Is the data stored contiguously?
INLINE static bool   
has_contiguous_storage()            { return true; }
/// Matrix is fully in-core.
bool
is_all_incore() const               { return true; }
/// Matrix is fully in-core.
inline bool 
is_on_disk() const                  { return false; }
/// Matrix is fully in-core.
bool
force_incore()                      { return true; }
/// Matrix is fully in-core.
INLINE static size_t
incore_cols()                       { return N; }
/// Matrix is fully in-core.
INLINE static size_t
last_incore_col()                   { return N; }

/**
   The number of segments fitting in a specified amount of memory.

   \param[in]  sz       The size of memory (in byte).
   \param[in]  seg0     The start segment ID.
   \param[out] se       The number of entries in these segments.
   */
size_type
segments_fitting_in (size_type sz, size_type seg0, size_type& se) const 
{ 
   assert(seg0 <= N);

   size_type         dseg  = N - seg0;
   const size_t      c     = 2*dseg + 1;

   sz /= sizeof(T);

   if (4*sz < c*c)
   {
      const float   df = static_cast<float>(c*c - 4*sz);
      dseg = (c - static_cast<size_t>(::sqrt(df))) / 2;
   }

   se = (dseg*(c-dseg)) / 2;
   return dseg;
}

//@}

};

} // end namespace cm2

#endif

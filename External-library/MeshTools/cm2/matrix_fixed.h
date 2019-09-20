/**
   \file       matrix_fixed.h
   \brief      Definition of fixed-sized template matrix.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATRIX_FIXED_H__
#define __CM2_MATRIX_FIXED_H__


#include "vector_fixed.h"
#include "strided_iterator.h"



namespace cm2 { 

/**
   Template matrix with compile-time fixed size.

   Template type `T` can be a POD (`unsigned`, `double`, `char`...) or a user-defined class
   (with visible default constructor, copy constructor, destructor and assignment operator).

   This class is simpler and more efficient than dense2D (no smart pointer overhead)
   but the sizes must be known at compile time and cannot be changed. \n
   Another big difference with dense2D concerns the copy constructors and operators. \n
   The copies here are _hard_ copies, not _shallow_ copies.

   The data are stored contiguously in memory and are column oriented (column-major order).
   \verbatim
      [0] [3] [6] [9]
      [1] [4] [7] [10]
      [2] [5] [8] [11]
   \endverbatim

   \tparam     T     Any type with default constructor, copy constructor, equal operator and assignment operator.
   \tparam     M     The number of rows of the matrix (fixed unsigned value).
   \tparam     N     The number of columns of the matrix (fixed unsigned value).
   \see        dense2D, vector_fixed.
   */
template <class T, size_t M, size_t N>
class matrix_fixed : protected vector_fixed<T, M*N> 
{

public:

/// self_type
typedef matrix_fixed<T,M,N>                              self_type;
/// self_type
typedef matrix_fixed<T,M,N>                              matrix_type;
/// parent_type
typedef vector_fixed<T,M*N>                              parent_type;

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
typedef matrix_fixed<T,N,M>                              transpose_type;
/// col_type
typedef vector_fixed<T,M>                                col_type;
/// diagonal_iterator
typedef cm2::strided<iterator,T>                         diagonal_iterator; 
/// const_diagonal_iterator
typedef cm2::strided<const_iterator,T>                   const_diagonal_iterator;  
/// row_iterator
typedef cm2::strided<iterator,T>                         row_iterator;    
/// const_row_iterator  
typedef cm2::strided<const_iterator,T>                   const_row_iterator;   

/// dimension_tag
typedef cm2::twoD_tag                                    dimension_tag;
/// shape_tag
typedef cm2::non_symmetric_tag                           shape_tag;
/// orientation_tag
typedef cm2::col_oriented_tag                            orientation_tag;
/// blocking_tag
typedef cm2::non_blocked_tag                             blocking_tag;
/// diagonal_storage_tag
typedef cm2::non_separate_diagonal_tag                   diagonal_storage_tag;
/// fullness_tag
typedef cm2::full_tag                                    fullness_tag;
/// col_size_tag
typedef cm2::fixed_size_tag<M>                           col_size_tag;
/// row_size_tag
typedef cm2::fixed_size_tag<N>                           row_size_tag;
/// size_tag
typedef cm2::fixed_size_tag<M*N>                         size_tag;
/// OOC_management_tag
typedef cm2::non_OOC_tag                                 OOC_management_tag;


/// The compile-time fixed dimensions: ROWS = M, COLS = N.
enum
{ 
   ROWS = M, 
   COLS = N,
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
   typedef cm2::fixed_size_tag<M>                  size_tag;
   typedef cm2::full_tag                           fullness_tag;

   typedef T*                                      pointer;
   typedef const T*                                const_pointer;
   typedef std::random_access_iterator_tag         iterator_category;

   INLINE segment_type() : _d(NULL), _j(0) { }
   INLINE segment_type (matrix_type& A, size_type j) 
      : _d(A.data()+j*M), _j(j)                             { assert(j <= N); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += M; ++_j; }
   INLINE void next (size_t s)                              { _d += s*M; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*M; _j += s; }
   INLINE iterator data()                                   { assert(_j < N); return _d; }
   INLINE const_iterator data() const                       { assert(_j < N); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < N); return _d; }
   INLINE iterator begin()                                  { assert(_j < N); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < N); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < N); return _d; }
   INLINE iterator end()                                    { return _d + M; }
   INLINE const_iterator end() const                        { return _d + M; }
   INLINE const_iterator cend() const                       { return _d + M; }
   INLINE static size_type size()                           { return M; }
   INLINE static bool empty()                               { return (M * N == 0); }
   INLINE value_type front() const                          { assert(M > 0); return *_d; }
   INLINE reference front()                                 { assert(M > 0); return *_d; }
   INLINE value_type back() const                           { assert(M > 0); return *(_d+M-1); }
   INLINE reference back()                                  { assert(M > 0); return *(_d+M-1); }
   INLINE value_type operator[] (size_type i) const         { assert(M > i); return *(_d+i); }
   INLINE reference operator[] (size_type i)                { assert(M > i); return *(_d+i); }

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
   INLINE difference_type operator- (const self_type& x) const { return static_cast<difference_type>(this->index() - x.index()); }
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
   typedef cm2::fixed_size_tag<M>                  size_tag;
   typedef cm2::full_tag                           fullness_tag;

   typedef const T*                                pointer;
   typedef const T*                                const_pointer;
   typedef std::random_access_iterator_tag         iterator_category;

   INLINE const_segment_type() : _d(NULL), _j(0) { }
   INLINE const_segment_type (const matrix_type& A, size_type j) 
      : _d(A.data()+j*M), _j(j)                             { assert(j <= N); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += M; ++_j; }
   INLINE void next (size_t s)                              { _d += s*M; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*M; _j += s; }
   INLINE const_iterator data() const                       { assert(_j < N); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < N); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < N); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < N); return _d; }
   INLINE const_iterator end() const                        { return _d + M; }
   INLINE const_iterator cend() const                       { return _d + M; }
   INLINE static size_type size()                           { return M; }
   INLINE static bool empty()                               { return (M * N == 0); }
   INLINE value_type front() const                          { assert(M > 0); return *_d; }
   INLINE value_type back() const                           { assert(M > 0); return *(_d+M-1); }
   INLINE value_type operator[] (size_type i) const         { assert(M > i); return *(_d+i); }

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
   INLINE difference_type operator- (const self_type& x) const { return static_cast<difference_type>(this->index() - x.index()); }
   INLINE bool operator== (const self_type& x) const        { return _d == x._d; }
   INLINE bool operator!= (const self_type& x) const        { return _d != x._d; }
   INLINE bool operator< (const self_type& x) const         { return _d < x._d; }
};

typedef const_segment_type       const_segment_iterator;       // Alias.


/**
   Default constructor.

   Elements are uninitialized.
   */
INLINE matrix_fixed()   { }

/**
   Constructor an initializing value.

   All elements are initialized to \p x.
   */
INLINE explicit matrix_fixed (value_type x) 
   : parent_type(x) { }

/**
   Constructor without any initializing value.

   Elements are uninitialized.

   \pre  rows == ROWS
   \pre  cols == COLS
   */
INLINE matrix_fixed (size_type DUM(rows), size_type DUM(cols)) 
{ 
   assert (rows == M);
   assert (cols == N);
}

/**
   Constructor an initializing value.

   All elements are initialized to \p x.

   \pre  rows == ROWS
   \pre  cols == COLS
   */
INLINE matrix_fixed (size_type DUM(rows), size_type DUM(cols), T x) 
   : parent_type(x) 
{ 
   assert (rows == M);
   assert (cols == N);
}

/**
   Copy constructor.
   Hard copy.
   */
INLINE matrix_fixed (const self_type& A) 
   : parent_type(A) 
{ }

/**
   Constructor from pre-allocated memory (deep copy).

   N * M elements starting from pointer \p d are copied to the vector.

   \pre  The N * M elements from pointer \p d must be valid.
   */
INLINE matrix_fixed (const T* d) 
   : parent_type(d) { }



/**@name Operators */
//@{
/// Scalar assignment.
INLINE value_type
operator= (value_type v) 
{ 
   return parent_type::operator=(v);
}

/// Copy operator (copy data).
self_type& 
operator= (const self_type& x) 
{ 
   vecvec::copy_n(x.cdata(), this->data(), M * N);
   return *this; 
}

/// Equality operator.
INLINE bool    
operator== (const self_type& V) const
{ 
   return parent_type::operator==(V);
}

/// Inequality operator.
INLINE bool    
operator!= (const self_type& V) const
{ 
   return parent_type::operator!=(V);
}
//@}


/**@name Size members */
//@{  

/// First index.
inline static size_t 
first_index()                       { return 0; }

/// Past-the-end index of columns.
inline static size_t 
past_index()                        { return N; }

/// Number of rows (constant static value = M).
INLINE static size_type 
rows()                              { return M; }

/// Number of columns (constant static value = N).
INLINE static size_type 
cols()                              { return N; }

/// Leading dimension = stride from one column to the next one (constant static value = M).
INLINE static size_type 
ld()                                { return M; }

/**
   Dimension of the matrix.

   When the matrix is not square the dimension is defined by min(rows(), cols()).
   */
INLINE static size_type
dim()                               { return M < N ? M : N; }

/**
   Number of elements in the matrix (i.e. rows() * cols()).

   This is a constant static value (= M * N).
   */
INLINE static size_type 
entries()                           { return parent_type::entries(); }

/// Same as entries.
INLINE size_t
segments_entries() const            { return parent_type::entries(); }

/// Returns true if the matrix is empty.
INLINE static bool 
empty()                             { return parent_type::empty(); }

/**
   Number of elements stored in the matrix (same as entries).

   This is a constant static value (= M * N).
   */
INLINE static size_type 
size()                              { return parent_type::size(); }

/**
   Size in bytes of the content of the matrix.

   This is a constant static value (= M * N * sizeof(T)).
   */
INLINE static size_t
size_of()                           { return parent_type::size_of(); }
//@}


/**@name Columns 

   The \ref col members return column copies (_hard_ copies).
   Use the segment members for shallow copies (views).
   */
//@{

/// Returns true if the j-th column can be accessed.
INLINE static bool
is_valid_col (size_type j)          { return j < N; }

/**
   Returns a _hard_ copy of the column \p j of the matrix. \n
   To get a shallow copy of a column use member \ref seg instead.
   \pre j < N. 
   */
col_type
col (size_type j) const             { return col_type(this->data_col(j)); }               

/**
   Gets the j-th column. _Hard_ copy. Same as \ref col. \n
   To get a shallow copy of a column use member \ref seg instead.
   \pre  j < cols() & v.size() == rows(). Range check performed in DEBUG mode only.
   */
template <class Vector>
void 
get_col (size_t j, Vector& v) const
{
   assert(v.size() == M);
   vecvec::copy_n(this->data_col(j),v.data(),  M);
}

/**
   Sets the j-th column. _Hard_ copy.
   \pre j < N. 
   */
template <class Vector>
void 
set_col (size_t j, const Vector& v)
{
   assert(v.size() == M);
   vecvec::copy_n(v.data(), this->data_col(j), M);
}
//@}


/**@name Segment iterators
   The segment_iterator and const_segment_iterator can be seen as views
   to the column data of a matrix. \n
   They are equipped with members such as begin(), end() and size() which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/// Number of segments (constant static value = N).
INLINE static size_type 
segments()                          { return N; }
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
   return this->begin() + j*M; 
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
   return this->cbegin() + j*M; 
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
   return this->cbegin() + j*M; 
}
//@}



/**@name Diagonal iterators
   The diagonal_iterator and const_diagonal_iterator can be seen as views
   to the data of a matrix with a stride equal to `M + 1`. This makes them
   iterate only on the diagonal terms of the matrix. \n
   They are equipped with members such as `begin()`, `end()` and `size()` which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/// Diagonal iterator to the first element of the diagonal.
INLINE diagonal_iterator 
diagonal_begin()                    { return diagonal_iterator(this->begin(), M+1); }
/// Diagonal iterator to the first element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_begin() const              { return const_diagonal_iterator(this->begin(), M+1); }
/// Diagonal iterator to the first element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_cbegin() const             { return const_diagonal_iterator(this->cbegin(), M+1); }

/// Diagonal iterator to the past-the-end element of the diagonal.
INLINE diagonal_iterator 
diagonal_end()                      { return diagonal_iterator(this->end(), M+1); }
/// Diagonal iterator to the past-the-end element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_end() const                { return const_diagonal_iterator(this->cend(), M+1); }
/// Diagonal iterator to the past-the-end element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_cend() const               { return const_diagonal_iterator(this->cend(), M+1); }
//@}


/**@name Row iterators
   The row_iterator and const_row_iterator can be seen as views
   to the data of a matrix with a stride equal to `M.` This makes them
   iterate only on the terms of a row of the matrix. \n
   They are equipped with members such as `begin()`, `end()` and `size()` which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/**
   Row iterator to the first element of row \p i.
   Range check performed in DEBUG mode only.
   */
template <class Index_>
row_iterator 
row_begin (Index_ i)             
{ 
   assert(size_type(i) < M);
   return row_iterator(this->begin()+i, M); 
}
/**
   Row iterator to the first element of row \p i (const).
   Range check performed in DEBUG mode only.
   */
template <class Index_>
const_row_iterator
row_begin (Index_ i) const       
{ 
   assert(size_type(i) < M);
   return const_row_iterator(this->cbegin()+i, M); 
}
/**
   Row iterator to the first element of row \p i (const).
   Range check performed in DEBUG mode only.
   */
template <class Index_>
const_row_iterator
row_cbegin (Index_ i) const       
{ 
   assert(size_type(i) < M);
   return const_row_iterator(this->cbegin()+i, M); 
}

/**
   Row iterator to the past-the-end element of row \p i.
   Range check performed in DEBUG mode only.
   */
template <class Index_>
row_iterator 
row_end (Index_ i)               
{ 
   assert(size_type(i) < M);
   return row_iterator(this->begin()+i + M*N, M); 
}
/**
   Row iterator to the past-the-end element of row \p i (const).
   Range check performed in DEBUG mode only.
   */
template <class Index_>
const_row_iterator
row_end (Index_ i) const         
{ 
   assert(size_type(i) < M);
   return const_row_iterator(this->cbegin()+i+ M*N, M); 
}
/**
   Row iterator to the past-the-end element of row \p i (const).
   Range check performed in DEBUG mode only.
   */
template <class Index_>
const_row_iterator
row_cend (Index_ i) const         
{ 
   assert(size_type(i) < M);
   return const_row_iterator(this->cbegin()+i+ M*N, M); 
}
//@}


/**@name Value and reference access operators */
//@{
/**
   Returns the element (i,j).
   Range check performed in DEBUG mode.

   \pre \p i < M. 
   \pre \p j < N. 
   */
template <class Index0_, class Index1_>
value_type    
operator() (Index0_ i, Index1_ j) const 
{ 
   assert((size_type(i) < M) && (size_type(j) < N));
   return parent_type::operator[](i + j*M); 
}

/**
   Returns the element (i,j) by reference.
   Range check performed in DEBUG mode.

   \pre \p i < M. 
   \pre \p j < N. 
   */
template <class Index0_, class Index1_>
reference         
operator() (Index0_ i, Index1_ j) 
{
   assert((size_type(i) < M) && (size_type(j) < N));
   return parent_type::operator[](i + j*M); 
}
//@}


/// Deep copy.
bool
copy (const self_type& x)
{
   vecvec::copy_n(x.begin(), this->begin(), M*N);
   return true;
}


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
INLINE size_type
segments_fitting_in (size_type sz, size_type seg0, size_type& se) const 
{ 
   assert(seg0 <= N);
   const size_type      segs = std::min(sz / (M * sizeof(T)), N - seg0); 
   se = segs * M;
   return segs;
}
//@}

};

} // end namespace cm2

#endif

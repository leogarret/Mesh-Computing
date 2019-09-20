/**
   \file       dense2d.h
   \brief      Definition of dynamic-sized template matrices.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3586 $
   $Date: 2019-04-09 10:13:28 +0200 (mar., 09 avr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_DENSE2D_H__
#define __CM2_DENSE2D_H__


#include "dense1d.h"
#include "vecvec.h"
#include "matscal.h"
#include "matvec.h"
#include "matmat.h"


namespace cm2 { 

/**
   Template matrix, resizable, with ref-counting.

   Template type `T` can be a POD (`unsigned`, `double`, `char`...) or a user-defined class
   (with visible default constructor, copy constructor, destructor and assignment operator).

   Just like dense1D from which it inherits many features, dense2D
   uses a "smart pointer" scheme to enable different matrices to share the same data and
   to manage proper memory deallocation. \n
   Copy constructors and operators do not actually copy the data but copy the _view_ to the data. \n
   This is called _shallow_ copy (vs _hard_ copy). \n
   When a dense2D is copied to another one, the copy is inexpensive and they both 
   share the same data. \n
   A change of an element in one matrix is viewed from the other one.

   The data are stored contiguously in memory, in a column oriented way. \n
   However, there may be a jump between the end of a column and the beginning of the next one
   (the jump remains constant between each successive pair of columns). \n
   This is because a matrix can view only a group of rows of another matrix. \n
   So, the stride between columns (called the _leading_ dimension) can be greater than 
   the actual number of rows.

   The data are stored contiguously in memory and are column oriented (column-major order).
   \verbatim
      [0] [3] [6] [9]
      [1] [4] [7] [10]
      [2] [5] [8] [11]
   \endverbatim

   \warning    dense2D, as all variable-size containers, are not fully thread-safe 
               because some resources (heap memory) can be shared between several threads. \n
               Copies, assignments and reads are thread-safe, but not writes!

   \note       The data are stored contiguously in memory except when the leading dimension exceeds
               the row dimension. In these cases, some extra elements (rows() - ld() elements) are 
               present between columns. \n
               The stride between two consecutive columns is always equal to the leading 
               dimension ld().
   \note       dense2D uses a _smart_ pointer with reference counting to manage
               memory deallocation.

   \tparam     T     Any type with default constructor, copy constructor and assignment operator.
   \see        DoubleMat, FloatMat, IntMat, UIntMat, matrix_fixed, dense1D.
   */
template <class T>
class dense2D : protected dense1D<T> 
{
public:

/// self_type
typedef dense2D<T>                                       self_type;
/// self_type
typedef dense2D<T>                                       matrix_type;
/// parent_type
typedef dense1D<T>                                       parent_type;

/// 
typedef dense2D<T>                                       block_type;
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
/// strided_iterator
typedef typename parent_type::strided_iterator           strided_iterator;
/// const_strided_iterator
typedef typename parent_type::const_strided_iterator     const_strided_iterator;
/// size_type
typedef typename parent_type::size_type                  size_type;
/// difference_type
typedef typename parent_type::difference_type            difference_type;

/// transpose_type
typedef dense2D<T>                                       transpose_type;
/// col_type
typedef dense1D<T>                                       col_type;
/// diagonal_iterator
typedef strided_iterator                                 diagonal_iterator;
/// const_diagonal_iterator
typedef const_strided_iterator                           const_diagonal_iterator;
/// row_iterator  
typedef strided_iterator                                 row_iterator;
/// const_row_iterator
typedef const_strided_iterator                           const_row_iterator;
/// shape_tag
typedef cm2::non_symmetric_tag                           shape_tag;
/// orientation_tag
typedef cm2::col_oriented_tag                            orientation_tag;
/// blocking_tag
typedef cm2::non_blocked_tag                             blocking_tag;
/// diagonal_storage_tag
typedef cm2::non_separate_diagonal_tag                   diagonal_storage_tag;

/// dimension_tag
typedef cm2::twoD_tag                                    dimension_tag;
/// fullness_tag
typedef cm2::full_tag                                    fullness_tag;
/// col_size_tag
typedef cm2::dynamic_size_tag                            col_size_tag;
/// row_size_tag
typedef cm2::dynamic_size_tag                            row_size_tag;
/// size_tag
typedef cm2::dynamic_size_tag                            size_tag;
/// OOC_management_tag
typedef cm2::non_OOC_tag                                 OOC_management_tag;


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
   typename matrix_type::size_type     _j, _rows, _ld, _cols;

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
   typedef cm2::dynamic_size_tag                   size_tag;
   typedef cm2::full_tag                           fullness_tag;

   typedef T*                                      pointer;
   typedef const T*                                const_pointer;
   typedef std::random_access_iterator_tag         iterator_category;

   INLINE segment_type() : _d(NULL), _j(0), _rows(0), _ld(0), _cols(0)
   { }
   INLINE segment_type (matrix_type& A, size_type j)
      : _d(A.data()+j*A.ld()), _j(j), _rows(A.rows()), _ld(A.ld()), _cols(A.cols())   
   { assert(j <= _cols); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += _ld; ++_j; }
   INLINE void next (size_t s)                              { _d += s*_ld; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*_ld; _j += s; }
   INLINE iterator data()                                   { assert(_j < _cols); return _d; }
   INLINE const_iterator data() const                       { assert(_j < _cols); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < _cols); return _d; }
   INLINE iterator begin()                                  { assert(_j < _cols); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < _cols); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < _cols); return _d; }
   INLINE iterator end()                                    { assert(_j < _cols); return _d + _rows; }
   INLINE const_iterator end() const                        { assert(_j < _cols); return _d + _rows; }
   INLINE const_iterator cend() const                       { assert(_j < _cols); return _d + _rows; }
   INLINE size_type size() const                            { return _rows; }
   INLINE bool empty() const                                { return (_rows == 0); }
   INLINE value_type front() const                          { assert((_j < _cols) && (_rows > 0)); return *_d; }
   INLINE reference front()                                 { assert((_j < _cols) && (_rows > 0)); return *_d; }
   INLINE value_type back() const                           { assert((_j < _cols) && (_rows > 0)); return *(_d+_rows-1); }
   INLINE reference back()                                  { assert((_j < _cols) && (_rows > 0)); return *(_d+_rows-1); }
   INLINE value_type operator[] (size_type i) const         { assert((_j < _cols) && (_rows > i)); return *(_d+i); }
   INLINE reference operator[] (size_type i)                { assert((_j < _cols) && (_rows > i)); return *(_d+i); }

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
   typename matrix_type::size_type     _j, _rows, _ld, _cols;

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
   typedef cm2::dynamic_size_tag                   size_tag;
   typedef cm2::full_tag                           fullness_tag;

   typedef const T*                                pointer;
   typedef const T*                                const_pointer;
   typedef std::random_access_iterator_tag         iterator_category;

   INLINE const_segment_type() : _d(NULL), _j(0), _rows(0), _ld(0), _cols(0)
   { }
   INLINE const_segment_type (const matrix_type& A, size_type j) 
      : _d(A.data()+j*A.ld()), _j(j), _rows(A.rows()), _ld(A.ld()), _cols(A.cols())   
   { assert(j <= _cols); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += _ld; ++_j; }
   INLINE void next (size_t s)                              { _d += s*_ld; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*_ld; _j += s; }
   INLINE const_iterator data() const                       { assert(_j < _cols); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < _cols); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < _cols); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < _cols); return _d; }
   INLINE const_iterator end() const                        { assert(_j < _cols); return _d + _rows; }
   INLINE const_iterator cend() const                       { assert(_j < _cols); return _d + _rows; }
   INLINE size_type size() const                            { return _rows; }
   INLINE bool empty() const                                { return (_rows == 0); }
   INLINE value_type front() const                          { assert((_j < _cols) && (_rows > 0)); return *_d; }
   INLINE value_type back() const                           { assert((_j < _cols) && (_rows > 0)); return *(_d+_rows-1); }
   INLINE value_type operator[] (size_type i) const         { assert((_j < _cols) && (_rows > i)); return *(_d+i); }

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

   Dimensions are null. Matrix is empty.
   */
dense2D() 
   : _rows(0), _cols(0), _ld(0) { }

/**
   Constructor with dimensions.

   Elements are uninitialized.

   \param   m  The number of rows.
   \param   n  The number of columns.

   \post    ld() == m, i.e. there is no jump between columns.
   \note    In case of allocation error, the size of the matrix is null 
            (rows() = 0, cols() = 0, ld() = 0).
   */
dense2D (size_type m, size_type n)
   : parent_type(m*n), _rows(m), _cols(n), _ld(m) 
{ 
   if (parent_type::empty()) 
      _rows = _cols = _ld = 0;
}

/**
   Constructor with a common initializing value.

   All elements are initialized to \p x.

   \param   m  The number of rows.
   \param   n  The number of cols.
   \param   x  The initializing value.

   \post    ld() == m, i.e. there is no jump between columns.
   \note    In case of allocation error, the size of the matrix is null 
            (rows() = 0, cols() = 0, ld() = 0).
   */
dense2D (size_type m, size_type n, value_type x)
   : parent_type(m*n, x), _rows(m), _cols(n), _ld(m) 
{ 
   if (parent_type::empty()) 
      _rows = _cols = _ld = 0;
}

/**
   Copy constructor (shallow copy).

   The matrices share the same data.
   */
dense2D (const self_type& M)
   : parent_type(M), _rows(M._rows), _cols(M._cols), _ld(M._ld) { }

/**
   Constructor from a vector (one column).

   \param   V0     The vector.

   \note    The data are shared (shallow copy).
   */
dense2D (const col_type& V0)
   : parent_type(V0), _rows(V0.size()), _cols(1), _ld(V0.size()) 
{ 
   if (parent_type::empty()) 
      _cols = 0;
}

/**
   Partial copy constructor (shallow copy).

   The matrices share the same data but the new matrix view only a part of the other one.

   \param   M        The input matrix.
   \param   m0       The row's index in \p M that will be the first row in the new matrix.
   \param   n0       The column's index in \p M that will be the first column in the new matrix.
   \param   m        The number of rows in the new matrix, starting from m0.
   \param   n        The number of cols in the new matrix, starting from n0.

   \pre     m0 + m <= M.rows()
   \pre     n0 + n <= M.cols()
   */
dense2D (const self_type& M, size_type m0, size_type n0, size_type m, size_type n) 
   : parent_type(M, m0 + n0*M._ld, (m*n > 0) ? m + (n-1)*M._ld : 0), 
     _rows(m), _cols(n), _ld(M._ld) 
{
   if (parent_type::empty()) 
      _rows = _cols = _ld = 0;
   assert((m==0) || (n==0) || (m0 + m <= M.rows()));
   assert((m==0) || (n==0) || (n0 + n <= M.cols()));
}

/**
   Constructor from a vector.

   \param   m        The number of rows in the matrix.
   \param   n        The number of cols in the matrix.
   \param   ld       The leading size (stride from one column to the next).
   \param   vals     The vector of data.

   \note    The data are shared (shallow copy).

   \pre     ld >= m
   \pre     md * n <= V.size()
   */
dense2D (size_type m, size_type n, size_type ld, const parent_type& vals)
   : parent_type(vals.sub_vector(0,ld*n)), _rows(m), _cols(n), _ld(ld) 
{ 
   if (parent_type::empty()) 
      _rows = _cols = _ld = 0;
   assert(ld >= m);
}

/**
   Constructor from a preallocated array (shallow copy).

   The user remains responsible for memory management.
   The matrix is not allowed to delete the pointer even in case of destruction or 
   reallocation (through \ref reserve or \ref push_back).

   \param   m        The number of rows in the new matrix.
   \param   n        The number of cols in the new matrix.
   \param   ld       The leading size (stride from one column to the next).
   \param   data     The data array as a continuous vector.
                     Must have at least ld*n elements.
   */
dense2D (size_type m, size_type n, size_type ld, T* data)
   : parent_type(ld*n, data), _rows(m), _cols(n), _ld(ld) 
{ 
   if (parent_type::empty()) 
      _rows = _cols = _ld = 0;
   assert(ld >= m);
}



/**@name Copy and assignment operators */
//@{
/// Scalar Assignment.  
value_type    
operator= (value_type v)  
{ 
   matscal::copy(v, *this);
   return v; 
}

/// Shallow copy (data are shared).
self_type& 
operator= (const self_type& x) 
{ 
   this->shallow_copy(x);
   return *this; 
}

/// Shallow copy (data are shared).
void 
shallow_copy (const self_type& x) 
{ 
   _rows = x._rows; 
   _cols = x._cols;
   _ld = x._ld; 
   parent_type::shallow_copy(x);
}

/// Deep copy (data are copied).
bool
copy (const self_type& x) 
{ 
   bool     ok(true);
   
   if (this == &x) 
      return ok;

   if (this->__rep() == x.__rep())
      this->shallow_copy(self_type());      // New buffer.

   this->clear();
   ok = this->resize(x.rows(), x.cols());
   if (ok)
      matmat::copy(x, *this);
   return ok;
}

/// Change allocation (ensures no longer shared).
bool
reallocate()
{
   self_type      tmp;
   const bool     ok = tmp.copy(*this);

   if (ok)
      this->shallow_copy(tmp);
   return ok;
}
//@}


/**@name Equality operators */
//@{

/// Shallow equality.
bool operator== (const self_type& x) const   
{ 
   return parent_type::operator==(x) && (_rows == x._rows) && (_cols == x._cols) && (_ld == x._ld); 
}
/// Shallow equality.
bool operator!= (const self_type& x) const 
{ 
   return parent_type::operator!=(x) || (_rows != x._rows) || (_cols != x._cols) || (_ld != x._ld); 
}

//@}


/**@name Columns 
   The \ref col members return column copies (shallow copies).
   */
//@{

/// Returns true if the j-th column can be accessed.
INLINE bool
is_valid_col (size_type j) const    { return j < _cols; }

/**
   Returns a shallow copy of the column \p j of the matrix. 
   \pre j < N. 
   */
col_type
col (size_type j)                   { return col_type(_rows, this->data_col(j)); }               

/**
   Returns a shallow copy of the column \p j of the matrix. 

   \note    The data in the returned vector are not const. 
   \pre     j < N. 
   */
col_type
col (size_type j) const             { return col_type(_rows, const_cast<pointer>(this->data_col(j))); }               

/**
   Gets the j-th column. _Hard_ copy.

   Prefer the  \ref seg members for fast access to columns (shallow copies).

   \pre  j < cols() & v.size() == rows(). Range check performed in DEBUG mode only.
   */
template <class Vector>
void 
get_col (size_t j, Vector& v) const
{
   assert(v.size() == _rows);
   vecvec::copy_n(this->data_col(j),v.data(),  _rows);
}

/**
   Sets the j-th column. _Hard_ copy.
   \pre  j < cols() & v.size() == rows(). Range check performed in DEBUG mode only.
   */
template <class Vector>
void 
set_col (size_t j, const Vector& v)
{
   assert(v.size() == _rows);
   vecvec::copy_n(v.data(), this->data_col(j), _rows);
}
//@}


/**@name Segment iterators
   The segment_iterator and const_segment_iterator can be seen as views
   to the columns of the matrix. \n
   They are equipped with members such as begin(), end() and size() which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/// Nbr of segments (columns).
INLINE size_t 
segments() const                    { return _cols; }
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

   \pre j < _cols. 
   */
template <class Index_>
segment_type 
seg (Index_ j)               
{ 
   assert(size_type(j) < _cols);
   return segment_type(*this, j); 
}

/**
   Returns a const-view to the column \p j of the matrix. \n
   The data are not copied (shared).

   Range check performed in DEBUG mode only.

   \pre j < _cols. 
   */
template <class Index_>
const_segment_type 
seg (Index_ j) const         
{ 
   assert(size_type(j) < _cols);
   return const_segment_type(*this, j); 
}

/**
   Returns a const-view to the column \p j of the matrix. \n
   The data are not copied (shared).

   Range check performed in DEBUG mode only.

   \pre j < _cols. 
   */
template <class Index_>
const_segment_type 
cseg (Index_ j) const         
{ 
   assert(size_type(j) < _cols);
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
   assert(size_type(j) < _cols);
   return this->begin() + j*_ld; 
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
   assert(size_type(j) < _cols);
   return this->cbegin() + j*_ld; 
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
   assert(size_type(j) < _cols);
   return this->cbegin() + j*_ld; 
}
//@}


/**@name Diagonal iterators
   The diagonal_iterator and const_diagonal_iterator can be seen as views
   to the data of a matrix with a stride equal to `ld() + 1`. This makes them
   iterate only on the diagonal terms of the matrix. \n
   They are equipped with members such as `begin()`, `end()` and `size()` which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/// Diagonal iterator to the first element of the diagonal.
INLINE diagonal_iterator 
diagonal_begin()                    { return diagonal_iterator(this->begin(), static_cast<int>(_ld+1)); }
/// Diagonal iterator to the first element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_begin() const              { return const_diagonal_iterator(this->cbegin(), static_cast<int>(_ld+1)); }
/// Const diagonal iterator to the first element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_cbegin() const             { return const_diagonal_iterator(this->cbegin(), static_cast<int>(_ld+1)); }

/// Diagonal iterator to the past-the-end element of the diagonal.
INLINE diagonal_iterator 
diagonal_end()                      { return diagonal_iterator(this->end(), static_cast<int>(_ld+1)); }
/// Diagonal iterator to the past-the-end element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_end() const                { return const_diagonal_iterator(this->cend(), static_cast<int>(_ld+1)); }
/// Const diagonal iterator to the past-the-end element of the diagonal (const).
INLINE const_diagonal_iterator
diagonal_cend() const               { return const_diagonal_iterator(this->cend(), static_cast<int>(_ld+1)); }
//@}


/**@name Row iterators
   The row_iterator and const_row_iterator can be seen as views
   to the data of a matrix with a stride equal to `ld()`. This makes them
   iterate only on the terms of a row of the matrix. \n
   They are equipped with members such as `begin()`, `end()` and `size()` which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/**
   Returns true if the i-th row can be accessed.
   */
INLINE bool
is_valid_row (size_type i) const    { return i < _rows; }

/// Row iterator to the first element of row \p i.
template <class Index_>
row_iterator 
row_begin (Index_ i)              
{ 
   assert (size_type(i) < _rows);
   return row_iterator(this->begin()+i, int(_ld)); 
}

/// Const row iterator to the first element of row \p i (const).
template <class Index_>
const_row_iterator
row_begin (Index_ i) const        
{ 
   assert (size_type(i) < _rows);
   return const_row_iterator(this->cbegin()+i, int(_ld)); 
}

/// Const row iterator to the first element of row \p i (const).
template <class Index_>
const_row_iterator
row_cbegin (Index_ i) const        
{ 
   assert (size_type(i) < _rows);
   return const_row_iterator(this->cbegin()+i, int(_ld)); 
}

/// Row iterator to the past-the-end element of row \p i.
template <class Index_>
row_iterator 
row_end (Index_ i)                
{ 
   assert (size_type(i) < _rows);
   return row_iterator(this->begin()+i + this->cols()*_ld, int(_ld)); 
}

/// Const row iterator to the past-the-end element of row \p i (const).
template <class Index_>
const_row_iterator
row_end (Index_ i) const          
{ 
   assert (size_type(i) < _rows);
   return const_row_iterator(this->cbegin()+i+ this->cols()*_ld, int(_ld)); 
}

/// Const row iterator to the past-the-end element of row \p i (const).
template <class Index_>
const_row_iterator
row_cend (Index_ i) const          
{ 
   assert (size_type(i) < _rows);
   return const_row_iterator(this->cbegin()+i+ this->cols()*_ld, int(_ld)); 
}
//@}



/**@name Value and reference access operators */
//@{
/**
   Returns the element (i,j) by reference.

   Range check performed in DEBUG mode.

   \pre \p i < rows(). 
   \pre \p j < cols(). 
   */
template <class Index0_, class Index1_>
reference
operator() (Index0_ i, Index1_ j) 
{
   assert (size_type(i) < _rows);
   assert (size_type(j) < _cols);

   return parent_type::operator[](i + j*_ld); 
}

/**
   Returns the element (i,j) by value.

   Range check performed in DEBUG mode.

   \pre \p i < rows(). 
   \pre \p j < cols(). 
   */
template <class Index0_, class Index1_>
value_type
operator() (Index0_ i, Index1_ j) const 
{ 
   assert (size_type(i) < _rows);
   assert (size_type(j) < _cols);

   return parent_type::operator[](i + j*_ld); 
}
//@}


/**@name Size members */
//@{  
/// First index.
inline size_t 
first_index() const                 { return 0; }

/// Past-the-end index of columns.
inline size_t 
past_index() const                  { return _cols; }

/// Number of rows.
INLINE size_type 
rows() const                        { return _rows; }

/// Number of columns.
INLINE size_type 
cols() const                        { return _cols; }

/** 
   Returns the leading dimension.

   Leading dimension = stride from one column to the next.
   */
INLINE size_type 
ld() const                          { return _ld; }

/// Returns the number of elements in the matrix.
INLINE size_type 
entries() const                     { return _rows * _cols; }

/// Same as entries.
INLINE size_t
segments_entries() const            { return _rows * _cols; }

/**
   Returns the dimension of the matrix.

   When the matrix is not square the dimension is defined by `min(rows(), cols())`.
   */
INLINE size_type
dim() const                         { return _rows < _cols ? _rows : _cols; }

/// True when the matrix is empty.
INLINE bool
empty() const                       { return _cols * _rows == 0; }

/// Returns the number of elements in the underlying vector (= `entries()` when `ld()=rows()`).
INLINE size_t
size() const                        { return parent_type::size(); }

/// The size in bytes of the content of the matrix.
INLINE size_t
size_of() const                     { return parent_type::size_of(); }

/**
   Returns the raw capacity of the matrix (always greater than or equal to `entries()`).

   This is the limit of entries to which the matrix can grow without performing any reallocation.
   */
INLINE size_type 
capacity() const                    { return parent_type::capacity(); }

/**
   Returns the column capacity of the matrix (always greater than or equal to `cols()`).

   This is the max number of columns, with number of rows constant, 
   to which the matrix can grow without performing any reallocation.

   If the matrix is empty, this function returns the raw capacity.
   */
INLINE size_type 
cols_capacity() const 
{ 
   const size_type    cap(parent_type::capacity()); 

   return (_ld > 0) ? cap / _ld : cap;
}

/**
   Appends a scalar to a 1xN matrix. 

   The matrix should be empty upon entry or have only one row. 
   
   \return  True for success.
   \post    The number of rows equals to one.
   \post    The number of columns is increased by one.
   */
INLINE bool 
push_back (value_type v)  
{ 
   return matscal::push_back(v, *this); 
}   


/**
   Appends another matrix (hard copy). 

   \return     True for success.
   \post       The number of columns is increased by `A.cols()`.
   \pre        If none of the two matrices are empty upon entry, 
               they must have the same number of rows.
   \warning    The matrices should not overlap (checked in _DEBUG mode).
   */
INLINE bool 
push_back (const self_type& M)  
{ 
   assert(!this->push_back_overlaps(M));
   return matmat::push_back(M, *this);
}

/**
   Appends a column of another matrix (hard copy). 

   \return     True for success.
   \post       The number of columns is increased by 1.
   \pre        If none of the two matrices are empty upon entry, 
               they must have the same number of rows.
   \warning    The matrices should not overlap (checked in _DEBUG mode).
   */
INLINE bool 
push_back (const self_type& M, size_t j)  
{ 
   assert(!this->push_back_overlaps(M));
   return matmat::push_back(M, j, *this);
}

/**
   Appends a vector (hard copy). 

   \return     True for success.
   \post       The number of columns is increased by one.
   \warning    The matrix and the vector should not overlap (checked in _DEBUG mode).
   */
INLINE bool 
push_back (const col_type& V)  
{ 
   assert(!this->push_back_overlaps(V));
   return matvec::push_back(V, *this);
}

template <class Vector>
INLINE bool __push_back (const Vector& A, cm2::oneD_tag)  { return matvec::push_back(A, *this); }
template <class Matrix>
INLINE bool __push_back (const Matrix& A, cm2::twoD_tag)  { return matmat::push_back(A, *this); }

/**
   Appends a matrix or a vector (hard copy). 

   \return     True for success.
   \post       The number of columns is increased by A.cols() if \p A is matrix
               or one in \p A is a vector.
   \pre        In case of a matrix, if none of the two matrices are empty upon entry, 
               they must have the same number of rows.
   \warning    The data should not overlap (not checked even in _DEBUG mode).
   */
template <class MatOrVec>
INLINE bool 
push_back (const MatOrVec& A)  
{ 
   typename MatOrVec::dimension_tag  dtag;
   return this->__push_back(A, dtag);
}

/// Removes the last column (if any).
INLINE void
pop_back()                                   
{ 
   if (_cols > 0) 
   {
      --_cols; 
      parent_type::pop_back(_ld);
   }
}

/// Removes the last columns (if any).
INLINE void
pop_back (size_type n)                                   
{ 
   n = std::min(n, _cols);
   _cols -= n; 
   parent_type::pop_back(_ld * n);
}


/**
   Manual memory reservation without dimension change.

   The main reason for using reserve() is efficiency: 
   if you know the capacity to which your matrix must eventually grow, then it is  
   more efficient to allocate that memory all at once rather than resizing it several times. \n
   The other reason for using reserve() is that you can control the invalidation of iterators.
   */
bool 
reserve (size_type m, size_type n, size_t growth_rate = 1) 
{ 
   const size_type   m0(_rows), n0(_cols);

   // Reserve new columns with growth_rate.
   if ((m == m0) && (m == _ld) && (n > n0) && (growth_rate > 1))
      return parent_type::reserve(m * n, growth_rate);

   else if ((m > m0) || (n > n0))
      return this->resize(m, n) && this->resize(m0, n0);

   return true;
}

/**
   Changes the dimensions of the matrix (uninitialized).

   Elements under min(rows(),m) and min(cols(),n) are left unchanged. \n
   If m > rows(), new rows between rows() and \p m are uninitialized. \n
   If n > cols(), new columns between cols() and \p n are uninitialized. \n
   A reallocation occurs and iterators are invalided when n*ld() > capacity() or m > ld(), 

   \return  True for success.
   \note    When it is not necessary to keep the data, it is a good practice to clear the matrix before any resizing.
   */
bool 
resize (size_type m, size_type n) 
{ 
   // If one dimension is null, nullify the two.
   if (m * n == 0)
      m = n = 0;

   // Try to avoid a reallocation.
   const size_type    ld(this->empty() ? m : _ld);
   if ((m <= ld) && (ld * n <= parent_type::capacity()))
   {
      parent_type::resize(ld * n);
      _rows = m;
      _cols = n;
      _ld = ((m != 0) ? ld : 0);
      return true;
   }
   
   // Reallocation compulsory.
   self_type         tmp(m, n);
   pointer           p1(this->data()), p2(tmp.data());
   const size_type   m1(std::min(_rows, m));
   const size_type   n1(std::min(_cols, n));
   const bool        ok((tmp.rows() == m) && (tmp.cols() == n));

   if (ok)
   {
      assert(tmp.ld() == m);
      if ((_ld == m1) && (m == m1))
         vecvec::copy_n(p1, p2, m1*n1);
      else
      {
         for (size_type j = 0; j < n1; ++j, p1 += _ld, p2 += m) 
            vecvec::copy_n(p1, p2, m1);
      }
      (*this) = tmp;
   }

   return ok;
}

/**
   Changes the dimensions of the matrix (initialized).
    
   Elements under min(rows(),m) and min(cols(),n) are left unchanged. \n
   If m > rows(), new rows between rows() and \p m are initialized to \p v. \n
   If n > cols(), new columns between cols() and \p n are initialized to \p v. \n
   A reallocation occurs and iterators are invalided when n*ld() > capacity() or m > ld(), 

   \return  True for success.
   \note    When it is not necessary to keep the data, it is a good practice to clear the matrix before any resizing.
   */
bool 
resize (size_type m, size_type n, value_type v) 
{ 
   const size_type   m0(_rows), n0(_cols);
   const bool        ok(this->resize(m, n));
   typename self_type::iterator  p;

   if (ok && !this->empty())
   {
      if (m > m0) 
      {
         p = this->begin() + m0;
         for (size_type j = 0; j < n0; ++j, p += _ld)
            vecscal::copy_n(v, p, m-m0);
      }
      if (n > n0) 
      {
         p = this->begin() + n0*_ld;
         if (_ld == m)
            vecscal::copy_n(v, p, m*(n-n0));
         else
         {
            for (size_type j = n0; j < n; ++j, p += _ld)
               vecscal::copy_n(v, p, m);
         }
      }
   }

   return ok;
}

/**
   Clears without deallocation. 

   The dimensions are set to zero. \n
   No deallocation is performed.
   */
void 
clear() 
{ 
   parent_type::clear(); 
   _rows = _cols = _ld = 0;
}

/**
   Clears with deallocation. 

   The dimensions are set to zero. \n
   Deallocation is performed (when there is no other matrix or vector pointing to the data).
   */
void 
clear_hard() 
{ 
   this->shallow_copy(self_type()); 
}
//@}



/**@name Sub-matrices */
//@{  
/**
   Returns a view on a rectangular part of the matrix. 
   Data are shared.

   \param   m0 The row's index that will be the first row in the new matrix.
   \param   n0 The column's index that will be the first column in the new matrix.
   \param   m  The number of rows in the new matrix, starting from m0.
   \param   n  The number of columns in the new matrix, starting from n0.

   \pre     m0 + m <= rows()
   \pre     n0 + n <= cols()
   */
self_type 
sub_matrix (size_type m0, size_type n0, size_type m, size_type n) 
{
   return self_type(*this, m0, n0, m, n);
}

/**
   Returns a view on a rectangular part of the matrix (const). 
   Data are shared.

   \param   m0 The row's index that will be the first row in the new matrix.
   \param   n0 The column's index that will be the first column in the new matrix.
   \param   m  The number of rows in the new matrix, starting from m0.
   \param   n  The number of columns in the new matrix, starting from n0.

   \pre     m0 + m <= rows()
   \pre     n0 + n <= cols()
   */
const self_type 
sub_matrix (size_type m0, size_type n0, size_type m, size_type n) const 
{
   return self_type(*this, m0, n0, m, n);
}

/**
   Returns the matrix as a vector. 
   Data are shared.
   */
parent_type
as_vector()                                     
{ 
   assert(this->_rows == this->_ld);
   return parent_type(*this); 
}

/**
   Returns the matrix as a vector (const). 
   Data are shared.
   */
const parent_type
as_vector() const
{ 
   assert(this->_rows == this->_ld);
   return parent_type(*this); 
}
//@}


/**@name I/O members (binary) */
//@{  
/**
   Reads data from a binary input stream.

   \tparam        IStream  Input stream class.
   \param[in,out] s        The input stream (template parameter).
   \return        0 (good), -1 (IO error) or -2(memory error).
  */
template <class IStream>
int
load (IStream& s)                   
{ 
   int   ret(0);

   this->clear();
   s >> _rows >> _cols;        // Read dimensions.
   ret = parent_type::load(s); 

   if ((ret == 0) & (_rows * _cols != parent_type::size()))
   {
      this->clear();
      ret = -2;
   }

   return ret;
}

/**
   Writes data to a binary output stream.

   \tparam        OStream  Output stream class.
   \param[in,out] s        The output stream (template parameter).
   */
template <class OStream>
void
save (OStream& s) const             
{ 
   s << _rows << _cols;       // Write dimensions.

   if (_ld == _rows)          // contiguous block.
      s.write(static_cast<const char*>(this->data()), sizeof(value_type) * _rows * _cols);
   else                       // need to skip rows from _rows to _ld.
   {
      for (size_t j = 0; j < _cols; ++j)
         s.write(static_cast<const char*>(this->data_col(j)), sizeof(value_type) * _rows);
   }
}
//@}


/**@name Miscellaneous */
//@{  
/// Diagonal is not unity.
INLINE static bool   
is_unit_diagonal()                  { return false; }
/// Is the data stored contiguously?
INLINE bool   
has_contiguous_storage() const      { return _ld == _rows; }
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
INLINE size_t
incore_cols() const                 { return _cols; }
/// Matrix is fully in-core.
INLINE size_t
last_incore_col() const             { return _cols; }


/**
   The number of segments fitting in a specified amount of memory.

   \param[in]  sz       The size of memory (in byte).
   \param[in]  seg0     The start segment ID.
   \param[out] se       The number of entries in these segments.
   */
INLINE size_type
segments_fitting_in (size_type sz, size_type seg0, size_type& se) const 
{ 
   assert(seg0 <= this->_cols);
   const size_type      segs = std::min(size_type(sz/(this->_ld * sizeof(T))), this->_cols - seg0); 
   se = segs * this->_rows;
   return segs;
}

/// Returns true when data overlap.
bool
overlaps (const self_type& x) const             { return parent_type::overlaps(x); }
/// Returns true when a vector overlaps with reserved data.
bool
reserved_overlaps (const col_type& x) const     { return parent_type::reserved_overlaps(x); }
/// Returns true when a matrix overlaps with reserved data.
bool
reserved_overlaps (const self_type& x) const    { return parent_type::reserved_overlaps(x); }
/// Returns true when a vector overlaps with reserved data when pushed-back.
bool
push_back_overlaps (const col_type& x) const    { return parent_type::push_back_overlaps(x); }
/// Returns true when a matrix overlaps with reserved data when pushed-back.
bool
push_back_overlaps (const self_type& x) const   { return parent_type::push_back_overlaps(x); }
//@}


/*------------------------ Protected members -----------------------*/
private:
///
size_type       _rows;
///
size_type       _cols;
///
size_type       _ld;
};

} // end namespace cm2

#endif

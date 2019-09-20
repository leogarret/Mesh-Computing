/**
   \file       symmetric_full.h
   \brief      Definition of dynamic-sized symmetric template matrices.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_SYMMETRIC_FULL_H__
#define __CM2_SYMMETRIC_FULL_H__


#include "dense1d.h"
#include "matmat.h"


namespace cm2 { 

/**
   Template symmetric full matrix, resizable, with ref-counting.

   Template type `T` can be a POD (`unsigned`, `double`, `char`...) or a user-defined class
   (with visible default constructor, copy constructor, destructor and assignment operator).

   Just like dense1D from which it inherits many features, symmetric_full
   uses a "smart pointer" scheme to enable different matrices to share the same data and
   to manage proper memory deallocation. \n
   Copy constructors and operators do not actually copy the data but copy the _view_ to the data. \n
   This is called _shallow_ copy (vs _hard_ copy). \n
   When a symmetric_full is copied to another one, the copy is inexpensive and they both 
   share the same data. \n
   A change of an element in one matrix is viewed from the other one.

   The data are stored contiguously in memory, in a column oriented way (column-major order). 

   \verbatim
      [0] [1] [3] [6]
          [2] [4] [7]
              [5] [8]
                  [9]
   \endverbatim

   \warning    symmetric_full, as all variable-size containers, are not fully thread-safe 
               because some resources (heap memory) can be shared between several threads. \n
               Copies, assignments and reads are thread-safe, but not writes!

   \note       symmetric_full uses a _smart_ pointer with reference counting to manage 
               memory deallocation.

   \tparam     T     Any type with default constructor, copy constructor and assignment operator.
   \see        DoubleSym, IntSym, UIntSym, dense1D.
   */
template <class T>
class symmetric_full : protected dense1D<T> 
{
public:

/// self_type
typedef symmetric_full<T>                                self_type;
/// self_type
typedef symmetric_full<T>                                matrix_type;
/// parent_type
typedef dense1D<T>                                       parent_type;

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
typedef symmetric_full<T>                                transpose_type;

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
   typename matrix_type::size_type     _j, _dim;

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

   INLINE segment_type() : _d(NULL), _j(0), _dim(0)
   { }
   INLINE segment_type (matrix_type& A, size_type j) 
      : _d(A.data()+(j*(j+1))/2), _j(j), _dim(A.dim())                     
   { assert(j <= _dim); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += (++_j); }
   INLINE void next (size_type s)                           { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE iterator data()                                   { assert(_j < _dim); return _d; }
   INLINE const_iterator data() const                       { assert(_j < _dim); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < _dim); return _d; }
   INLINE iterator begin()                                  { assert(_j < _dim); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < _dim); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < _dim); return _d; }
   INLINE iterator end()                                    { assert(_j < _dim); return _d + _j+1; }
   INLINE const_iterator end() const                        { assert(_j < _dim); return _d + _j+1; }
   INLINE const_iterator cend() const                       { assert(_j < _dim); return _d + _j+1; }
   INLINE size_type size() const                            { return _j+1; }
   INLINE static bool empty()                               { return false; }
   INLINE value_type front() const                          { assert(_j < _dim); return *_d; }
   INLINE reference front()                                 { assert(_j < _dim); return *_d; }
   INLINE value_type back() const                           { assert(_j < _dim); return *(_d+_j); }
   INLINE reference back()                                  { assert(_j < _dim); return *(_d+_j); }
   INLINE value_type operator[] (size_type i) const         { assert((_j < _dim) && (i <= _j)); return *(_d+i); }
   INLINE reference operator[] (size_type i)                { assert((_j < _dim) && (i <= _j)); return *(_d+i); }

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
   typename matrix_type::size_type     _j, _dim;

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

   INLINE const_segment_type() : _d(NULL), _j(0), _dim(0)
   { }
   INLINE const_segment_type (const matrix_type& A, size_type j) 
      : _d(A.data()+(j*(j+1))/2), _j(j), _dim(A.dim())                     
   { assert(j <= _dim); }

   INLINE size_type index() const                           { return _j; }
   INLINE static size_type index_begin()                    { return 0; }
   INLINE void next()                                       { _d += (++_j); }
   INLINE void next (size_type s)                           { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE void next (difference_type s)                     { _d += s*_j + (s*(s+1))/2; _j += s; }
   INLINE const_iterator data() const                       { assert(_j < _dim); return _d; }
   INLINE const_iterator cdata() const                      { assert(_j < _dim); return _d; }
   INLINE const_iterator begin() const                      { assert(_j < _dim); return _d; }
   INLINE const_iterator cbegin() const                     { assert(_j < _dim); return _d; }
   INLINE const_iterator end() const                        { assert(_j < _dim); return _d + _j+1; }
   INLINE const_iterator cend() const                       { assert(_j < _dim); return _d + _j+1; }
   INLINE size_type size() const                            { return _j+1; }
   INLINE static bool empty()                               { return false; }
   INLINE value_type front() const                          { assert(_j < _dim); return *_d; }
   INLINE value_type back() const                           { assert(_j < _dim); return *(_d+_j); }
   INLINE value_type operator[] (size_type i) const         { assert((_j < _dim) && (i <= _j)); return *(_d+i); }

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

   Dimensions are null. Matrix is empty.
   */
symmetric_full() 
   : _dim(0) { }

/**
   Constructor with dimension.

   Elements are uninitialized.

   \param   n  The number of columns = The number of rows.
   */
explicit symmetric_full (size_type n) 
   : parent_type((n*(n+1))/2), _dim(n)
{ 
   if (parent_type::empty()) 
      _dim = 0;
}

/**
   Constructor with a common initializing value.

   All elements are initialized to \p x.

   \param   n  The number of columns = The number of rows.
   \param   x  The initializing value.
   */
symmetric_full (size_type n, value_type x) 
   : parent_type((n*(n+1))/2, x), _dim(n)
{ 
   if (parent_type::empty()) 
      _dim = 0;
}

/**
   Copy constructor (shallow copy).

   The matrices share the same data.
   */
symmetric_full (const self_type& M) 
   : parent_type(M), _dim(M._dim) { }

/**
   Partial copy constructor (shallow copy).

   The matrices share the same data but the new matrix view only a leading part of the other one.

   \param   M     The input matrix.
   \param   n     The dimension of the new matrix (leading part).

   \pre     n <= M.rows()
   */
symmetric_full (const self_type& M, size_type n) 
   : parent_type(M, 0, (n*(n+1))/2), _dim(n) 
{
   assert (n <= M.dim());
}

/**
   Constructor from a vector.

   \param   n        The dimension of the matrix.
   \param   vals     The vector of data.

   \note    The data are shared (shallow copy).

   \pre     n*(n+1)/2 <= V.size()
   */
symmetric_full (size_type n, const parent_type& vals)
   : parent_type(vals.sub_vector(0,(n*(n+1))/2)), _dim(n)
{ }

/**
   Constructor from a preallocated array (shallow copy).

   The user remains responsible for memory management. \n
   The matrix is not allowed to delete the pointer even in case of destruction or 
   reallocation (through \ref reserve or \ref push_back).

   \param   n        The dimension of the matrix.
   \param   data     The data array as a continuous vector.
                     Must have at least n*(n+1)/2 elements.
   */
symmetric_full (size_type n, T* data) 
   : parent_type((n*(n+1))/2, data), _dim(n) 
{ }



/**@name Copy and assignment operators */
//@{
/// Scalar Assignment.  
INLINE value_type    
operator= (value_type v)  
{ 
   return parent_type::operator= (v);
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
   _dim = x._dim;
   parent_type::shallow_copy (x);
}

/// Deep copy (data are copied).
bool
copy (const self_type& x) 
{ 
   _dim = x._dim;
   return parent_type::copy(x);
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


/// Returns true if the j-th column can be accessed.
INLINE bool
is_valid_col (size_type j) const    { return j < _dim; }



/**@name Segment iterators
   The segment_iterator and const_segment_iterator can be seen as views
   to the column data of a matrix. \n
   They are equipped with members such as begin(), end() and size() which make them candidates
   for most of the cm2::vecscal and cm2::vecvec template functions.
   */
//@{
/// Number of segments (dim).
INLINE size_type 
segments() const                    { return _dim; }
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
   assert(size_type(j) < _dim);
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
   assert(size_type(j) < _dim);
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
   assert(size_type(j) < _dim);
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
   assert(size_type(j) < _dim);
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
   assert(size_type(j) < _dim);
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
   assert(size_type(j) < _dim);
   return this->cbegin() + j*(j+1)/2; 
}
//@}



/**@name Value and reference access operators */
//@{

/**
   Returns the element (i,j) by reference.
   Elements (i,j) and (j,i) are the same element.
   Range check performed in DEBUG mode.

   \pre \p i < dim(). 
   \pre \p j < dim(). 
   */
template <class Index_>
INLINE reference          
operator() (Index_ i, Index_ j)
{
   if (i > j) std::swap(i, j);
   assert(size_type(j) < _dim);
   return parent_type::operator[](i + (j*(j+1))/2); 
}

/**
   Returns the element (i,j) by value.
   Elements (i,j) and (j,i) are the same element.
   Range check performed in DEBUG mode.

   \pre \p i < dim(). 
   \pre \p j < dim(). 
   */
template <class Index_>
INLINE value_type
operator() (Index_ i, Index_ j) const 
{ 
   if (i > j) std::swap(i, j);
   assert(size_type(j) < _dim);
   return parent_type::operator[](i + (j*(j+1))/2); 
}
//@}


/**@name Size members */
//@{  
/// First index.
inline size_t 
first_index() const                 { return 0; }

/// Past-the-end index.
inline size_t 
past_index() const                  { return _dim; }

/// Dimension of the matrix.
INLINE size_type         
dim() const                         { return _dim; }

/// Number of rows = dimension of the matrix.
INLINE size_type         
rows() const                        { return _dim; }

/// Number of columns (i.e. the dimension) of the matrix.
INLINE size_type         
cols() const                        { return _dim; }

/// True when the matrix is empty.
INLINE bool
empty() const                       { return parent_type::empty(); }

/// Number of elements in the matrix.
INLINE size_type 
entries() const                     { return parent_type::entries(); }

/// Same as entries.
INLINE size_t
segments_entries() const            { return parent_type::entries(); }

/// Number of elements in the matrix (same as entries).
INLINE size_type 
size() const                        { return parent_type::size(); }

/// Size in bytes of the content of the matrix.
INLINE size_t
size_of() const                     { return parent_type::size_of(); }

/**
   Clears without deallocation. 

   The dimension is set to zero.  \n
   No deallocation is performed.
   */
void 
clear() 
{ 
   parent_type::clear(); 
   _dim = 0;
}

/**
   Clears with deallocation. 

   The dimension is set to zero.  \n
   Deallocation is performed (when there is no other matrix or vector pointing to the data).
   */
void 
clear_hard() 
{ 
   parent_type::clear_hard(); 
   _dim = 0;
}

/**
   Manual memory reservation without dimension change.

   reserve() causes a reallocation manually. The main reason for using reserve() is efficiency: 
   if you know the capacity to which your matrix must eventually grow, then it is usually 
   more efficient to allocate that memory all at once rather than resizing it several times.  \n
   The other reason for using reserve() is that you can control the invalidation of iterators.
   */
void 
reserve (size_type n) 
{ 
   parent_type::reserve((n*(n+1))/2); 
}

/**
   Changes the dimension of the matrix (uninitialized).

   Elements under min(dim(),n) are left unchanged. \n
   If m > dim(), new rows and columns between dim() and \p n are uninitialized. \n
   A reallocation occurs and iterators are invalided when n*(n+1)/2 > capacity().

   \return  True for success.
   \note    When it is not necessary to keep the data, it is a good practice to clear the matrix before any resizing.
   */
bool
resize (size_type n) 
{ 
   const bool ok  = parent_type::resize((n*(n+1))/2); 
   
   if (ok) _dim = n;

   return ok;
}

/**
   Changes the dimension of the matrix (initialized).

   Elements under min(dim(),n) are left unchanged. \n
   If m > dim(), new rows and columns between dim() and \p n are initialized to value \p v. \n
   A reallocation occurs and iterators are invalided when n*(n+1)/2 > capacity(). 

   \return  True for success.
   \note    When it is not necessary to keep the data, it is a good practice to clear the matrix before any resizing.
   */
bool
resize (size_type n, value_type v) 
{ 
   const bool ok  = parent_type::resize((n*(n+1))/2, v); 

   if (ok) _dim = n;

   return ok;
}
//@}


/**@name Sub-matrices */
//@{  
/**
   Returns a view on a leading part of the matrix. 
   Data are shared.

   \param   m  The number of rows (cols) in the sub-matrix.

   \pre     m <= dim()
   */
self_type 
sub_matrix (size_type m) 
{
   return self_type(*this, m);
}

/**
   Returns a view on a leading part of the matrix (const). 
   Data are shared.

   \param   m  The number of rows (cols) in the sub-matrix.

   \pre     m <= dim()
   */
const self_type 
sub_matrix (size_type m) const 
{
   return self_type(*this, m);
}
//@}



/**@name Views as vector */
//@{  
/**
   Returns the matrix as a vector. 
   Data are shared.
   */
parent_type
as_vector()                            { return parent_type(*this); }

/**
   Returns the matrix as a vector (const). 
   Data are shared.
   */
const parent_type
as_vector() const                      { return parent_type(*this); }
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
   s >> _dim;           // Read dimension.
   ret = parent_type::load(s); 

   if ((ret == 0) && (_dim*(_dim+1) != 2 * parent_type::size()))
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
save (OStream& s) const                { parent_type::save(s); }
//@}


/**@name Miscellaneous */
//@{  
/// Diagonal is not unity.
INLINE static bool   
is_unit_diagonal()                     { return false; }
/// Is the data stored contiguously?
INLINE static bool   
has_contiguous_storage()               { return true; }
/// Matrix is fully in-core.
bool
is_all_incore() const                  { return true; }
/// Matrix is fully in-core.
inline bool          
is_on_disk() const                     { return false; }
/// Matrix is fully in-core.
bool
force_incore()                         { return true; }
/// Matrix is fully in-core.
INLINE size_t
incore_cols() const                    { return _dim; }
/// Matrix is fully in-core.
INLINE size_t
last_incore_col() const                { return _dim; }

/**
   The number of segments fitting in a specified amount of memory.

   \param[in]  sz       The size of memory (in byte).
   \param[in]  seg0     The start segment ID.
   \param[out] se       The number of entries in these segments.
   */
size_type
segments_fitting_in (size_type sz, size_type seg0, size_type& se) const 
{ 
   assert(seg0 <= this->_dim);

   size_type         dseg  = this->_dim - seg0;
   const size_t      c     = 2*dseg + 1;

   sz /= sizeof(T);

   if (4*sz < c*c)
   {
      const float   d = static_cast<float>(c*c - 4*sz);
      dseg = (c - static_cast<size_t>(::sqrt(d))) / 2;
   }

   se = (dseg * (c - dseg)) / 2;
   return dseg;
}

/// Returns true when data overlap.
bool
overlaps (const self_type& x) const             { return parent_type::overlaps(x); }
/// Returns true when a matrix overlaps with reserved data.
bool
reserved_overlaps (const self_type& x) const    { return parent_type::reserved_overlaps(x); }
//@}


/*------------------------ Protected members -----------------------*/
private:
///
size_type    _dim;

};

} // end namespace cm2

#endif

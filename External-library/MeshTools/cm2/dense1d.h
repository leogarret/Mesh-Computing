/** 
   \file       dense1d.h
   \brief      Definition of dynamic-sized template vectors.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_DENSE1D_H__
#define __CM2_DENSE1D_H__


#include "matvec_tags.h"
#include "vector_num.h"
#include "smart_ptr.h"
#include "vecvec.h"


namespace cm2 { 

/**
   Template vector, resizable, with ref-counting.

   Template type `T` can be a POD (`unsigned`, `double`, `char`...) or a user-defined class
   (with visible default constructor, copy constructor, destructor and assignment operator).

   dense1D<T> is almost equivalent to the STL class `std::vector<T>`. \n
   The main differences concern the copy constructors and the assignment operator. \n
   dense1D uses a "smart pointer" scheme to enable different vectors to share the same data and
   to manage proper memory deallocation. \n
   Copy constructors and operators do not actually copy the data but copy the _view_ to the data. \n
   This is called _shallow_ copy (vs _hard_ copy). \n
   When a dense1D is copied to another one, the copy is inexpensive and they both 
   share the same data. \n
   A change of an element in one vector is viewed from the other one.

   \warning    dense1D, as all variable-size containers, are not fully thread-safe 
               because some resources (heap memory) can be shared between several threads. \n
               Copies, assignments and reads are thread-safe, but not writes!

   \note       The data are stored contiguously in memory.
   \note       dense1D uses a _smart_ pointer with reference counting to manage
               memory deallocation.

   \tparam     T     Any type with default constructor, copy constructor and assignment operator.
   \see        DoubleVec, FloatVec, UIntVec, IntVec, vector_fixed.
   */
template <class T,
          int VEC_ALIGNMENT = CM2_ALIGNMENT>
class dense1D 
{
public:

/// self_type
typedef dense1D<T>                                          self_type;
/// value_type
typedef T                                                   value_type;
/// rep_type (internal use only)
typedef cm2::vector_num<T,VEC_ALIGNMENT>                    rep_type;
/// reference
typedef typename rep_type::reference                        reference;
/// const_reference
typedef typename rep_type::const_reference                  const_reference;
/// pointer
typedef typename rep_type::pointer                          pointer;
/// const pointer
typedef typename rep_type::const_pointer                    const_pointer;
/// iterator
typedef typename rep_type::iterator                         iterator;
/// const_iterator
typedef typename rep_type::const_iterator                   const_iterator;
/// reverse_iterator
typedef typename rep_type::reverse_iterator                 reverse_iterator;
/// const_reverse_iterator
typedef typename rep_type::const_reverse_iterator           const_reverse_iterator;
/// strided_iterator
typedef typename rep_type::strided_iterator                 strided_iterator;
/// const_strided_iterator
typedef typename rep_type::const_strided_iterator           const_strided_iterator;
/// reverse_strided_iterator
typedef typename rep_type::reverse_strided_iterator         reverse_strided_iterator;
/// const_reverse_strided_iterator
typedef typename rep_type::const_reverse_strided_iterator   const_reverse_strided_iterator;
/// size_type
typedef typename rep_type::size_type                        size_type;
/// difference_type
typedef typename rep_type::difference_type                  difference_type;
/// iterator_category
typedef std::random_access_iterator_tag                     iterator_category;

/// dimension_tag
typedef cm2::oneD_tag                                       dimension_tag;
/// size_tag
typedef cm2::dynamic_size_tag                               size_tag;
/// fullness_tag
typedef cm2::full_tag                                       fullness_tag;



/**
   Default constructor.

   Constructs an empty vector. \n
   The size and capacity are zero.
   */
dense1D();

/**
   Non-initializing constructor.

   Constructs an uninitialized vector of size \p n.

   \note    In case of allocation error, the size of the vector is null.
   */
explicit dense1D (size_type n);

/**
   Initializing constructor.

   Constructs a vector of size \p n initialized to value \p v.

   \note    In case of allocation error, the size of the vector is null.
   */
dense1D (size_type n, value_type v);

/**
   Copy constructor (shallow copy).

   Shallow copy = share the same data. \n
   Data are not copied, only the smart pointer is copied.
   */
dense1D (const self_type& x)
   : _rep(x._rep), _size(x._size), _shift(x._shift) 
{ }

/**
   Partial copy constructor (shallow copy).

   Shallow copy = share the same data. \n
   Constructs a view on a vector with a shift of the first element.

   \pre The value \p shift + \p size must be less than the underlying vector's size.
   */
dense1D (const self_type& x, size_type start, size_type size)
   : _rep(x._rep), _size(size), _shift(x._shift+start) 
{ 
   assert (this->end() <= x.end());
}

/**
   Preallocated memory constructor (shallow copy).

   The user remains responsible for memory management. \n
   The vector is not allowed to delete the pointer even in case of destruction or 
   reallocation (through reserve() or push_back()).

   \param   n        The number of elements to consider starting from \p data.
   \param   data     The data array as a continuous vector. \n
                     Must have at least \p n elements. \n
                     Can be unaligned on a VEC_ALIGNMENT boundary.
   */
dense1D (size_type n, T* data);


/**@name Iterator access members */
//@{
/// Pointer to the first element. Same as begin().
INLINE pointer 
data()                                 { return _rep->data() + _shift; }
/// Const pointer to the first element. Same as cbegin().
INLINE const_pointer 
data() const                           { return _rep->cdata() + _shift; }
/// Const pointer to the first element. Same as cbegin().
INLINE const_pointer 
cdata() const                          { return this->data(); }

/// STL-compatible begin (pointer to the first element).
INLINE iterator 
begin()                                { return _rep->begin() + _shift; }
/// STL-compatible begin (const pointer to the first element).
INLINE const_iterator 
begin() const                          { return _rep->cbegin() + _shift; }
/// STL-compatible cbegin (const pointer to the first element).
INLINE const_iterator 
cbegin() const                         { return this->begin(); }

/// STL-compatible end (pointer to the past-the-end element).
INLINE iterator 
end()                                  { return _rep->begin() + _shift + _size; }
/// STL-compatible end (pointer to the past-the-end element).
INLINE const_iterator 
end() const                            { return _rep->cbegin() + _shift + _size; }
/// STL-compatible cend (pointer to the past-the-end element).
INLINE const_iterator 
cend() const                           { return this->end(); }

/// STL-compatible rbegin (pointer to the reversed first element, i.e. the last).
INLINE reverse_iterator 
rbegin()                               { return reverse_iterator(this->end()); }
/// STL-compatible rbegin (const pointer to the reversed first element, i.e. the last).
INLINE const_reverse_iterator
rbegin() const                         { return const_reverse_iterator(this->cend()); }
/// STL-compatible crbegin (const pointer to the reversed first element, i.e. the last).
INLINE const_reverse_iterator
crbegin() const                        { return this->rbegin(); }

/// STL-compatible rend (pointer to the reversed past-the-end element, i.e. before the first).
INLINE reverse_iterator 
rend()                                 { return reverse_iterator(this->begin()); }
/// STL-compatible rend. (const pointer to the reversed past-the-end element, i.e. before the first).
INLINE const_reverse_iterator
rend() const                           { return const_reverse_iterator(this->cbegin()); }
/// STL-compatible crend. (const pointer to the reversed past-the-end element, i.e. before the first).
INLINE const_reverse_iterator
crend() const                          { return this->rend(); }
//@}


/**@name Copy and assignment operators */
//@{
/// Scalar Assignment.  
value_type    
operator= (value_type v)  
{ 
   if (!this->empty())
      vecscal::copy_n(v, this->begin(), this->size());
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
   _rep = x._rep; 
   _shift = x._shift; 
   _size = x._size; 
}

/// Deep copy (data are copied).
bool
copy (const self_type& x) 
{ 
   bool     ok(true);
   
   if (this == &x) 
      return ok;

   if (_rep == x._rep)
      this->shallow_copy(self_type());      // New buffer.

   this->clear();

   if (!x.empty())
   {
      ok = this->resize(x.size());
      if (ok)
         vecvec::copy_n(x.begin(), this->begin(), x.size());
   }

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

/**
   Shallow equality.
   \return     True when objects are same (i.e. refer to same data).
   */
bool operator== (const self_type& x) const   
{ 
   return (_rep == x._rep) && (_size == x._size) && (_shift == x._shift); 
}
/**
   Shallow inequality.
   \return     True when objects are not same (i.e. refer to different data).
   */
bool operator!= (const self_type& x) const 
{ 
   return (_rep != x._rep) || (_size != x._size) || (_shift != x._shift); 
}

//@}


/**@name Value and reference access operators */
//@{
/// Checks accessibility of element at position \p i.
INLINE bool
is_valid (size_type i) const           { return i < _size; }

/**
   Returns the i-th element. 
   \pre  \p i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
reference 
operator[] (Index_ i) 
{
   assert (size_type(i) < _size);
   return (*_rep)[_shift+i]; 
}

/**
   Returns the i-th element by value. 
   \pre  \p i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
value_type 
operator[] (Index_ i) const 
{
   assert (size_type(i) < _size);
   return (*_rep)[_shift+i]; 
}

/**
   Returns the i-th element by reference. 
   \pre  \p i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
reference 
operator() (Index_ i) 
{
   assert (size_type(i) < _size);
   return (*_rep)[_shift+i]; 
}

/**
   Returns the i-th element by value. 
   \pre  \p i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
value_type 
operator() (Index_ i) const 
{
   assert (size_type(i) < _size);
   return (*_rep)[_shift+i]; 
}

/**
   Gets the i-th element. Same as operator[] const.
   \pre  \p i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
value_type 
at (Index_ i) const
{
   assert (size_type(i) < _size);
   return (*_rep)[_shift+i]; 
}

/**
   Gets the i-th element. Same as operator[] const.
   \pre  \p i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
void 
get_val (Index_ i, reference v) const
{
   assert (size_type(i) < _size);
   v = (*_rep)[_shift+i]; 
}

/**
   Sets the i-th element. Same as operator[].
   \pre  \p i must be < size(). Range check performed in DEBUG mode only.
   */
template <class Index_>
void 
set_val (Index_ i, value_type v)
{
   assert (size_type(i) < _size);
   (*_rep)[_shift+i] = v; 
}

/**
   Returns the front element by value. 
   \pre Vector must not be empty.
   */
INLINE value_type 
front() const     
{ 
   assert (!this->empty());
   return *(this->begin()); 
}
/**
   Returns the front element by reference. 
   \pre Vector must not be empty.
   */
INLINE reference
front()           
{ 
   assert (!this->empty());
   return *(this->begin()); 
}
/**
   Returns the last element by value. 
   \pre Vector must not be empty.
   */
INLINE value_type 
back() const      
{ 
   assert (!this->empty());
   return *(this->end()-1); 
}
/**
   Returns the front element by reference. 
   \pre Vector must not be empty.
   */
INLINE reference 
back()            
{ 
   assert (!this->empty());
   return *(this->end()-1); 
}
//@}



/**@name Size members */
//@{  
/// The number of elements in the vector (same as size()).
INLINE size_type 
entries() const                           { return _size; }
/// The number of elements in the vector (same as entries()).
INLINE size_type 
size() const                              { return _size; }
/// The size in bytes of the content of the vector.
INLINE size_t
size_of() const                           { return _size * sizeof(T); }
/// The size of the vector. Same as size().
INLINE size_type 
dim() const                               { return _size; }
/// The leading dimension of the vector. Same as size().
INLINE size_type 
ld() const                                { return _size; }
/// True when the vector is empty.
INLINE bool
empty() const                             { return _size == 0; }

/**
   Returns the capacity of the vector (always greater than or equal to size()).

   Automatic reallocation occurs when size() reaches capacity(). \n
   It is then usually increased by a factor of two. \n
   \warning    Just like the STL vector, this invalidates the iterators.
   */
INLINE size_type 
capacity() const                          { return _rep->capacity() - _shift; }

/**
   Appends an element. 
   
   \return  True for success.
   \post    The size is increased by one.
   */
INLINE bool 
push_back (value_type v)  
{ 
   const bool  ok = _rep->assign(_shift + _size, v);
   if (ok) ++_size;
   return ok;
}   

/**
   Appends two elements. 
   
   \return  True for success.
   \post    The size is increased by two.
   */
INLINE bool 
push_back (value_type v0, value_type v1)  
{ 
   const bool  ok = _rep->assign(_shift + _size, v0, v1);
   if (ok) _size += 2;
   return ok;
}   

/**
   Appends three elements. 
   
   \return  True for success.
   \post    The size is increased by three.
   */
INLINE bool 
push_back (value_type v0, value_type v1, value_type v2)  
{ 
   const bool  ok = _rep->assign(_shift + _size, v0, v1, v2);
   if (ok) _size += 3;
   return ok;
}   

/**
   Appends four elements. 
   
   \return  True for success.
   \post    The size is increased by four.
   */
INLINE bool 
push_back (value_type v0, value_type v1, value_type v2, value_type v3)  
{ 
   const bool  ok = _rep->assign(_shift + _size, v0, v1, v2, v3);
   if (ok) _size += 4;
   return ok;
}   

/**
   Appends an element n times. 
   
   \return  True for success.
   \post    The size is increased by \p n.
   */
bool 
push_back_n (value_type v, size_t n)  
{ 
   const bool  ok =  this->reserve(this->size() + n, /*growth_rate=>*/ 2); 
   
   if (ok)
      this->resize(this->size() + n, v);

   return ok;
}   

/**
   Appends another vector (hard copy). 

   \return     True for success.
   \post       The size is increased by x.size().
   \warning    The vectors should not overlap (checked in _DEBUG mode).
   */
INLINE bool 
push_back (const self_type& V)  
{ 
   assert (!this->push_back_overlaps(V));
   return vecvec::push_back(V, *this);
}   

/// Removes the last element, if any. 
INLINE void
pop_back()                                { if (_size > 0) --_size; }

/// Removes the last n elements, if any. 
INLINE void
pop_back (size_type n)                    { if (_size > n) _size -= n; else _size = size_type(0); }

/**
   Manual memory reservation without size change.

   Like the STL vector reserve() causes a reallocation manually. \n
   The main reason for using reserve() is efficiency. \n
   If you know the capacity to which your vector must eventually grow, then it is usually 
   more efficient to allocate that memory all at once rather than relying on the automatic 
   reallocation scheme. \n
   The other reason for using reserve() is that you can control the invalidation of iterators.
   reserve() has no effect for a value lesser than the current size.

   \return  True for success.
   */
bool 
reserve (size_type n, size_t growth_rate = 1)                     
{ 
   if (n > this->capacity())
      _rep->reserve(_shift + n, 
                    /*recopy_up_to=>*/ _shift + _size, 
                    growth_rate); 

   return (this->capacity() >= n);
}

/**
   Changes the size of the vector (uninitialized).

   Elements under min(size(),n) are left unchanged. \n
   If \p n > size(), new elements between size() and \p n are uninitialized. \n
   If \p n > capacity(), a reallocation occurs and iterators are invalid. \n

   \note    When the data are not to be kept, it is a good practice to clear the vector before any resizing.
   \return  True for success.
   */
bool 
resize (size_type n)                      
{ 
   if (n > this->capacity())
      this->reserve(n); 

   _size = std::min(n, this->capacity());

   return (_size == n);
}

/**
   Changes the size of the vector (initialized).

   Elements under min(size(),n) are left unchanged. \n
   If \p n > size(), new elements between size() and \p n are initialized to \p v. \n
   If \p n > capacity(), a reallocation occurs and iterators are invalid. \n

   \note    When the data are not to be kept, it is a good practice to clear the vector before any resizing.
   \return  True for success.
   */
bool 
resize (size_type n, value_type v) 
{ 
   const size_type   old_size    = this->size(); 
   const bool        ok          = this->resize(n); 

   if (ok && (n > old_size)) 
      vecscal::copy_n(v, this->begin() + old_size, n - old_size);

   return ok;
}

/**
   Clears without deallocation. 

   The size is set to zero but capacity is left unchanged. \n
   No deallocation is performed.
   */
void 
clear()                                   { _size = 0; }

/**
   Clears with deallocation (if possible). 

   The size and the capacity are set to zero. \n
   Deallocation is performed (when there is no other vector pointing to the data).
   */
void 
clear_hard()                              { this->shallow_copy(self_type()); }
//@}  


/**@name Sub-vectors */
//@{  
/// Returns a view on a portion of the vector. Data are still shared.
self_type 
sub_vector (size_type start, size_type size) 
{
   return self_type(*this, start, size);
}

/// Returns a view on a portion of the vector. Data are still shared.
const self_type 
sub_vector (size_type start, size_type size) const 
{
   return self_type(*this, start, size);
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
   size_t            sz(0);
  
   this->clear();

   s >> sz;                                                                // read size
   if (!this->resize(sz))
      return -2;
   s.read(static_cast<char*>(this->begin()), sizeof(value_type) * sz);     // read data

   return s.good() ? 0 : -1;
}

/**
   Saves the data to a binary output stream.

   \tparam        OStream  Output stream class.
   \param[in,out] s        The output stream (template parameter).
   */
template <class OStream>
void
save (OStream& s) const
{
   const size_t   sz    = this->size();

   s << sz;
   s.write(static_cast<const char*>(this->begin()), sizeof(value_type) * sz);
}
//@}


/**@name Miscellaneous */
//@{  
/// Returns true when data overlap.
bool
overlaps (const self_type& x) const
{
   return (this->cbegin() < x.cend()) && (x.cbegin() < this->cend());
}

/// Returns true when a vector overlaps with reserved data.
bool
reserved_overlaps (const self_type& x) const    
{ 
   return _rep->overlaps(x.cbegin(), x.cend()); 
}

/// Returns true when a vector overlaps with reserved data when pushed-back.
bool
push_back_overlaps (const self_type& x) const    
{ 
   return _rep->overlaps(_shift+_size, x.cbegin(), x.size()); 
}
//@}


/*------------------------ Protected members -----------------------*/
protected:
/// rep_type (internal use only)
typedef cm2::Ptr<rep_type>             SPtr;

///
INLINE const SPtr&     __rep() const     { return _rep; }

private:
///
SPtr              _rep;
///
size_type         _size;
///
size_type         _shift;

};



// DEFINITION OF NON-INLINE MEMBERS //

///
template <class T, int VEC_ALIGNMENT>
dense1D<T,VEC_ALIGNMENT>::dense1D()
   : _rep(new rep_type()), _size(0), _shift(0) 
{ }

///
template <class T, int VEC_ALIGNMENT>
dense1D<T,VEC_ALIGNMENT>::dense1D (size_type n)
   : _rep(new rep_type(n)), _size(n), _shift(0) 
{ 
   if (_rep->size() < n) 
      _size = _rep->size();
}

///
template <class T, int VEC_ALIGNMENT>
dense1D<T,VEC_ALIGNMENT>::dense1D (size_type n, value_type v)
   : _rep(new rep_type(n,v)), _size(n), _shift(0) 
{ 
   if (_rep->size() < n) 
      _size = _rep->size();
}

///
template <class T, int VEC_ALIGNMENT>
dense1D<T,VEC_ALIGNMENT>::dense1D (size_type n, T* data)
   : _rep(new rep_type(n, data)), _size(n), _shift(0) 
{ }


} // end namespace cm2

#endif

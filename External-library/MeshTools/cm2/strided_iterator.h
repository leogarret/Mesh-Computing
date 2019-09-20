/**
   \file       strided_iterator.h
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_STRIDED_ITERATOR_H__
#define __CM2_STRIDED_ITERATOR_H__


#  ifndef INLINE
#     define INLINE  inline
#  endif


namespace cm2 { 

/**
   strided iterator (const stride).
   */
template <class Iter, class T>
class strided 
{
  typedef strided<Iter, T>    self_type;

public:

#ifndef CM2_NO_CLASS_PARTIAL_SPECIALIZATION
  typedef typename std::iterator_traits<Iter>::difference_type       difference_type;
  typedef typename std::iterator_traits<Iter>::value_type            value_type;
  typedef typename std::iterator_traits<Iter>::reference             reference;
//  typedef typename std::iterator_traits<Iter>::const_reference       const_reference;
  typedef typename std::iterator_traits<Iter>::iterator_category     iterator_category;
  typedef typename std::iterator_traits<Iter>::pointer               pointer;
#else
  typedef ::ptrdiff_t                                                difference_type;
  typedef T                                                          value_type;
  typedef T&                                                         reference;
//  typedef const T&                                                   const_reference;
  typedef std::random_access_iterator_tag                            iterator_category;
  typedef T*                                                         pointer;
#endif      // CM2_NO_CLASS_PARTIAL_SPECIALIZATION

  INLINE strided() : _stride(0) { }

  INLINE strided(const Iter& x, difference_type s) 
    : _iter(x), _stride(s) { }

  INLINE strided(const self_type& x)
    : _iter(x._iter), _stride(x._stride) { }

  INLINE int index() const                                     { return _iter.index() / _stride; }
  
  // convert to original iterator
//  INLINE operator Iter () const              { return _iter; }

  INLINE Iter base() const                                     { return _iter; }

  INLINE reference operator*() const                           { return reference(*_iter); }

  INLINE self_type& operator++ ()                              { _iter += _stride; return *this; }
  INLINE self_type operator++ (int)                            { self_type tmp = *this; _iter += _stride; return tmp; }
  INLINE self_type& operator-- ()                              { _iter -= _stride; return *this; }
  INLINE self_type operator-- (int)                            { self_type tmp = *this; _iter -= _stride; return tmp; }
  
  INLINE self_type operator+ (difference_type n) const         { return self_type (_iter + n*_stride, _stride); }
  INLINE self_type& operator+= (difference_type n)             { _iter += n*_stride; return *this; }
  INLINE self_type operator- (difference_type n) const         { return self_type (_iter - n*_stride, _stride); }
  INLINE self_type& operator-= (difference_type n)             { _iter -= n*_stride; return *this; }

  INLINE difference_type operator- (const self_type& x) const  { return (_iter - x._iter) / _stride; }
  
  INLINE reference operator[] (difference_type n) const        { return *(_iter + n*_stride); }

  INLINE bool operator==(const self_type& x) const             { return _iter == x._iter; }
  INLINE bool operator!=(const self_type& x) const             { return _iter != x._iter; }
  INLINE bool operator<(const self_type& x) const              { return _iter < x._iter; }  
  INLINE bool operator>(const self_type& x) const              { return _iter > x._iter; }  

protected:
  Iter               _iter;
  difference_type    _stride;
};

}  // end namespace cm2



#ifndef __CLASS_PARTIAL_SPECIALIZATION

namespace std {

template <class Iter, class T>
INLINE random_access_iterator_tag 
iterator_category (const cm2::strided<Iter,T>&)    { return random_access_iterator_tag(); }

template <class Iter, class T>
INLINE ::ptrdiff_t* 
distance_type (const cm2::strided<Iter,T>&)        { return (::ptrdiff_t*)(0); }

template <class Iter, class T>
INLINE T* 
value_type (const cm2::strided<Iter,T>&)           { return (T*)(0); }

}  // namespace std

#endif


#endif








/**
   \file       triangular_lower.h
   \brief      Definition of lower part of a symmetric template matrix.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_TRIANGULAR_LOWER_H__
#define __CM2_TRIANGULAR_LOWER_H__


namespace cm2 { 

/**
   Lower triangular part of a symmetric matrix.

   This class is almost like its template parameter symmetric matrix class.  \n
   It has the same storage. \n
   The main difference is that triangular_lower is not considered as a symmetric matrix. \n
   Its mathematical shape is the same as its storage shape, i.e. a triangular shape.

   \tparam     SymMatrix     Any symmetric matrix with default constructor, copy constructor and assignment operator.
   \see        symmetric_fixed, symmetric_full, triangular_upper.
   */
template <class SymMatrix>
class triangular_lower : public SymMatrix 
{

public:

/// self_type.
typedef triangular_lower<SymMatrix>                      self_type;
/// parent_type.
typedef SymMatrix                                        parent_type;

/// value_type
typedef typename parent_type::value_type                 value_type;
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

/// shape_tag.
typedef cm2::non_symmetric_tag                           shape_tag;
/// orientation_tag
typedef typename cm2::orientation_as_lower<typename parent_type::storage_tag,
                                           typename parent_type::orientation_tag>::tag  orientation_tag;
/// storage_tag
typedef cm2::lower_left_storage_tag                      storage_tag;
/// fullness_tag
typedef cm2::non_full_tag                                fullness_tag;



/**
   Default constructor.
   Elements are uninitialized.
   */
triangular_lower() { }

/**
   Constructor with a common initializing value.
   All the elements are initialized to \p v.
   */
INLINE explicit triangular_lower (value_type v)           
   : parent_type(v) 
{ }

/**
   Constructor from a symmetric matrix.
   The upper part excluding the diagonal (considered null).
   */
triangular_lower (const parent_type& A) 
   : parent_type(A) 
{ }

/**
   Copy constructor.
   */
triangular_lower (const self_type& A) 
   : parent_type(A) 
{ }



/**@name Copy and assignment operators */
//@{
/// Scalar assignment.
INLINE value_type 
operator= (value_type v) 
{ 
   return parent_type::operator=(v);
}
//@}


/**@name Value and reference access operators */
//@{
/**
   Returns the element (\p i, \p j) by reference.
   Range check performed in DEBUG mode.

   \pre \p j >= i. 
   */
template <class Index_>
reference      
operator() (Index_ i, Index_ j) 
{
   assert (j <= i);
   return parent_type::operator()(i, j); 
}

/**
   Returns the element (\p i, \p j) by value.
   Range check performed in DEBUG mode.
   */
value_type 
operator() (size_type i, size_type j) const 
{ 
   return (j <= i) ? parent_type::operator()(i, j) : value_type(0); 
}
//@}

};

} // end namespace cm2

#endif

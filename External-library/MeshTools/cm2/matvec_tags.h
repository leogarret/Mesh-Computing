/**
   \file       matvec_tags.h
   \brief      Definition of tags for vectors and matrices.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATVEC_TAGS__
#define __CM2_MATVEC_TAGS__

namespace cm2 { 

   
/**@name Tags for vector and matrix types */
/**
   These tags serve as branching parameters for template math function instantiations
   in libraries vecscal, vecvec, matscal, matvec and matmat.
   Thanks to them, you can call the same function,
   for instance matvec::mult(const Matrix& A, const VecX& x, VecY& y),
   for very different kinds of matrix.
   */

//@{  

/// Dimension 0 tag, for scalars.
struct zeroD_tag  { enum { val = 0 }; };
/// Dimension 1 tag, for vectors.
struct oneD_tag   { enum { val = 1 }; };
/// Dimension 2 tag, for matrices.
struct twoD_tag   { enum { val = 2 }; };
/// Dimension 3 tag...
struct threeD_tag { enum { val = 3 }; };

/// Fixed size tag, for vectors.
template <unsigned N> 
struct fixed_size_tag { enum { val = N }; };
/// Dynamic size tag, for vectors.
struct dynamic_size_tag { };

/// Symmetric shape tag, for matrices.
struct symmetric_tag { };
/// Non-symmetric shape tag, for matrices.
struct non_symmetric_tag { };
/// Diagonal shape tag, for matrices.
struct diagonal_tag { };

/// Column oriented tag, for matrices.
struct col_oriented_tag { };
/// Row oriented tag, for matrices.
struct row_oriented_tag { };
/// Storage tag, for symmetric matrices.
struct upper_right_storage_tag { };
/// Storage tag, for symmetric matrices.
struct lower_left_storage_tag { };


/// Blocked tag, for matrices.
struct blocked_tag { };
/// Non blocked tag, for matrices.
struct non_blocked_tag { };

/// Separated diagonal tag, for matrices.
struct separate_diagonal_tag { };
/// Non-separated diagonal tag, for matrices.
struct non_separate_diagonal_tag { };
/// Unit diagonal tag, for matrices.
struct unit_diagonal_tag { };
/// Non-unit diagonal tag, for matrices.
struct non_unit_diagonal_tag { };

/// Fullness tag, for vectors and matrices.
struct full_tag { };
/// Non-fullness tag, for vectors and matrices.
struct non_full_tag { };

/// Non Out-Of-Core management, for matrices.
struct non_OOC_tag { };
/// Out-Of-Core management, for matrices.
struct OOC_tag { };

/// Bug tag.
struct bogus_tag { };


/// Orientation tag of a transposed matrix.
template <class Orientation>
struct transpose_orientation { };
///
template<>
struct transpose_orientation<col_oriented_tag> 
{ 
   typedef row_oriented_tag      tag;
};
///
template<>
struct transpose_orientation<row_oriented_tag> 
{ 
   typedef col_oriented_tag      tag;
};


/// Storage tag of a transposed matrix.
template <class Storage>
struct transpose_storage { };
///
template<>
struct transpose_storage<upper_right_storage_tag> 
{ 
   typedef lower_left_storage_tag      tag;
};
///
template<>
struct transpose_storage<lower_left_storage_tag> 
{ 
   typedef upper_right_storage_tag      tag;
};


/// Orientation tag of a symmetric matrix seen as upper-right.
template <class Storage, class Orient>
struct orientation_as_upper  
{ };
///
template <>
struct orientation_as_upper<upper_right_storage_tag, col_oriented_tag>
{ 
   typedef col_oriented_tag       tag;
};
///
template <>
struct orientation_as_upper<upper_right_storage_tag, row_oriented_tag>
{ 
   typedef row_oriented_tag       tag;
};
///
template <>
struct orientation_as_upper<lower_left_storage_tag, col_oriented_tag>
{ 
   typedef row_oriented_tag       tag;
};
///
template <>
struct orientation_as_upper<lower_left_storage_tag, row_oriented_tag>
{ 
   typedef col_oriented_tag       tag;
};


/// Orientation tag of a symmetric matrix seen as lower-left.
template <class Storage, class Orient>
struct orientation_as_lower 
{ };
///
template <>
struct orientation_as_lower<upper_right_storage_tag, col_oriented_tag>
{ 
   typedef row_oriented_tag       tag;
};
///
template <>
struct orientation_as_lower<upper_right_storage_tag, row_oriented_tag>
{ 
   typedef col_oriented_tag       tag;
};
///
template <>
struct orientation_as_lower<lower_left_storage_tag, col_oriented_tag>
{ 
   typedef col_oriented_tag       tag;
};
///
template <>
struct orientation_as_lower<lower_left_storage_tag, row_oriented_tag>
{ 
   typedef row_oriented_tag       tag;
};
//@}

} // end namespace cm2

#endif


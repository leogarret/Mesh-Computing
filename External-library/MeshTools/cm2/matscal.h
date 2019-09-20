/**
   \file       matscal.h
   \brief      Definition of matrix-scalar template functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3498 $
   $Date: 2019-02-14 14:04:35 +0100 (jeu., 14 févr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATRIX_SCALAR_OPS__
#define __CM2_MATRIX_SCALAR_OPS__


#include "matvec_tags.h"
#include "math1_aux.h"
#include "vecscal.h"
#include "vecvec.h"


/**
   \namespace  cm2::matscal
   \brief      Namespace for Matrix-Scalar template functions.
   */

namespace cm2 { 
namespace matscal { 

/** \defgroup MatScalGroup Matrix-Scalar Template Functions */
//@{

/**
   Appends a scalar to a matrix.

   \return  Success flag.
   \pre     The matrix should be empty upon entry or have only one row. 
   */
template <class Matrix>
bool 
push_back (typename Matrix::value_type v, Matrix& A)  
{ 
   const size_t      C  = A.cols();
   bool              ok (A.empty() || (A.rows() == 1));

   if (ok)
   {
      if (A.empty() || (C == A.cols_capacity()))
         A.reserve(1, std::max(size_t(4),2*C));         // Double the capacity of the matrix.

      ok = A.resize(1, C+1, v);
   }

   return ok;
}   

/**
   Returns the determinant of a 2x2 matrix.

   \pre  A.rows() >= 2
   \pre  A.cols() >= 2
   */
template <class Matrix>
inline typename Matrix::value_type  
det2 (const Matrix& A) 
{
   return A(0,0)*A(1,1) - A(1,0)*A(0,1);
}    

/**
   Returns the determinant of a 3x3 matrix.

   \pre  A.rows() >= 3
   \pre  A.cols() >= 3
   */
template <class Matrix>
inline typename Matrix::value_type  
det3 (const Matrix& A) 
{
   return (A(0,0)*(A(1,1)*A(2,2) - A(2,1)*A(1,2)) -
           A(1,0)*(A(0,1)*A(2,2) - A(2,1)*A(0,2))) +
           A(2,0)*(A(0,1)*A(1,2) - A(1,1)*A(0,2));
}    

/**
   Returns the determinant of a 1x1, 2x2 or 3x3 matrix.

   \pre  A.rows() <= 3
   \pre  A.cols() <= 3
   */
template <class Matrix>
typename Matrix::value_type  
det (const Matrix& A) 
{
   const size_t                  DIM (A.dim());
   typename Matrix::value_type   d(0); 

   assert (DIM <= 3);

   if (DIM == 1)
      d = A(0,0);
   else if (DIM == 2)
      d = matscal::det2(A);
   else if (DIM == 3)
      d = matscal::det3(A);

   return d;
}    

/**
   Returns a cofactor of a 2x2 matrix.
   */
template <class Matrix>
inline typename Matrix::value_type  
cofactor2 (size_t i, size_t j, const Matrix& A) 
{
   return A((i+1)%2,(j+1)%2);
}    

/**
   Returns a cofactor of a 3x3 matrix.
   */
template <class Matrix>
inline typename Matrix::value_type  
cofactor3 (size_t i, size_t j, const Matrix& A) 
{
   return A((i+1)%3,(j+1)%3) * A((i+2)%3,(j+2)%3) - A((i+2)%3,(j+1)%3) * A((i+1)%3,(j+2)%3);
}   

/**
   Returns a cofactor of a 2x2 or 3x3 matrix.

   \pre  2 <= A.dim() <= 3
   \pre  2 <= A.cols() <= 3
   */
template <class Matrix>
inline typename Matrix::value_type  
cofactor (size_t i, size_t j, const Matrix& A) 
{
   const size_t                  DIM (A.dim());
   typename Matrix::value_type   v(0); 

   assert (DIM <= 3);

   if (DIM == 2)
      v = matscal::cofactor2(i, j, A);
   else if (DIM == 3)
      v = matscal::cofactor3(i, j, A);

   return v;
}    


/**
   Sets the diagonal of a matrix to a scalar value.

   \verbatim     
      A(i,i) = v 
   \endverbatim
   */
template <class Matrix>
void
copy_diag (typename Matrix::value_type v, Matrix& A) 
{
   size_t    i = std::min(size_t(A.rows()), size_t(A.cols()));

   if (i) do {
      --i;
      A.operator()(i,i) = v;  
   } while (i);
}    

/**
   Adds a scalar to the diagonal of a matrix.

   \verbatim     
      A(i,i) += v 
   \endverbatim
   */
template <class Matrix>
void
add_diag (typename Matrix::value_type v, Matrix& A) 
{
   typedef typename Matrix::value_type    T;

   size_t    i = std::min(size_t(A.rows()), size_t(A.cols()));

   if (i && (v != T(0))) do 
   {
      --i;
      A.operator()(i,i) += v;  
   } while (i);
}    

/**
   Multiplies the diagonal of a matrix by a scalar value.

   \verbatim     
      A(i,i) *= v 
   \endverbatim
   */
template <class Matrix>
void
mult_diag (typename Matrix::value_type v, Matrix& A) 
{
   typedef typename Matrix::value_type    T;

   size_t    i = std::min(size_t(A.rows()), size_t(A.cols()));

   if (i && (v != T(1))) do {
      --i;
      A.operator()(i,i) *= v;  
   } while (i);
}    

/**
   Divides the diagonal of a matrix by a scalar value.

   \verbatim     
      A(i,i) /= v 
   \endverbatim

   \note    Prefer multiply by 1/v for non integer types.
   */
template <class Matrix>
void
div_diag (typename Matrix::value_type v, Matrix& A) 
{
   typedef typename Matrix::value_type    T;

   size_t    i = std::min(size_t(A.rows()), size_t(A.cols()));

   if (i && (v != T(1))) do {
      --i;
      A.operator()(i,i) /= v;  
   } while (i);
}    


/**
   Gets the trace of a matrix.

   \verbatim     
      trace = Sum A(i,i)
   \endverbatim
   */
template <class Matrix>
typename Matrix::value_type
trace (const Matrix& A) 
{
   const size_t                     N  = A.rows();
   typename Matrix::value_type      tr(0);

   assert (A.cols() == A.rows());

   for (size_t i = 0; i < N; ++i)
      tr += A.operator()(i,i);  

   return tr;
}    

/**
   Sets a row of a matrix to a scalar value.

   \verbatim     
      A(i,*) = v 
   \endverbatim
   */
template <class Matrix>
inline void
copy_row (typename Matrix::value_type v, Matrix& A, size_t i) 
{
   vecscal::copy_n(v, A.row_begin(i), A.cols());
}

/**
   Adds a scalar value to a row of a matrix.

   \verbatim     
      A(i,*) += v 
   \endverbatim
   */
template <class Matrix>
inline void
add_row (typename Matrix::value_type v, Matrix& A, size_t i) 
{
   vecscal::add_n(v, A.row_begin(i), A.cols());
}

/**
   Multiplies a row of a matrix by a scalar value.

   \verbatim     
      A(i,*) *= v 
   \endverbatim
   */
template <class Matrix>
inline void
mult_row (typename Matrix::value_type v, Matrix& A, size_t i) 
{
   vecscal::mult_n(v, A.row_begin(i), A.cols());
}

/**
   Divides a row of a matrix by a scalar value.

   \verbatim     
      A(i,*) /= v 
   \endverbatim

   \note    Prefer multiply by 1/v for non integer types.
   */
template <class Matrix>
inline void
div_row (typename Matrix::value_type v, Matrix& A, size_t i) 
{
   vecscal::div_n(v, A.row_begin(i), A.cols());
}


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Negates a matrix.

   \verbatim     
      A(i,j) = -A(i,j) 
   \endverbatim
   */
template <class Matrix>
void
negate (Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// NEGATE //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__negate (Matrix& A, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::negate(A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::negate_n(A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__negate (Matrix& A, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::negate(A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__negate (Matrix& A, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      matscal::negate(*b);
}

// Definition of negate.
template <class Matrix>
inline void
negate (Matrix& A) 
{
   if (A.has_contiguous_storage())
      vecscal::negate(A);
   else
      matscal::__negate(A, 
                        typename Matrix::blocking_tag(), 
                        typename Matrix::diagonal_storage_tag());
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Copies a scalar to a matrix.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      A(i,j) = v 
   \endverbatim
   */
template <class Matrix>
void
copy (typename Matrix::value_type v, Matrix& A); 

#endif


#ifndef DOXYGEN_SKIP

// COPY //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__copy (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::copy(v, A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::copy_n(v, A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__copy (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::copy(v, A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__copy (typename Matrix::value_type v, Matrix& A, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      matscal::copy(v, *b);
}

// Definition of copy.
template <class Matrix>
inline void
copy (typename Matrix::value_type v, Matrix& A) 
{
   if (A.has_contiguous_storage())
      vecscal::copy(v, A);
   else
      matscal::__copy(v, A, 
                      typename Matrix::blocking_tag(), 
                      typename Matrix::diagonal_storage_tag());
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Adds a scalar to a matrix.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      A(i,j) += v 
   \endverbatim
   */
template <class Matrix>
void
add (typename Matrix::value_type v, Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// ADD //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__add (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::add(v, A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::add_n(v, A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__add (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::add(v, A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__add (typename Matrix::value_type v, Matrix& A, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      matscal::add(v, *b);
}

// Definition of add.
template <class Matrix>
inline void
add (typename Matrix::value_type v, Matrix& A) 
{
   typedef typename Matrix::value_type    T;

   if (v == T(0))
      return;

   if (A.has_contiguous_storage())
      vecscal::add(v, A);
   else
      matscal::__add(v, A, 
                     typename Matrix::blocking_tag(), 
                     typename Matrix::diagonal_storage_tag());
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Subtract a scalar to a matrix.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      A(i,j) -= v 
   \endverbatim
   */
template <class Matrix>
void
subtract (typename Matrix::value_type v, Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// SUBTRACT //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__subtract (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::subtract(v, A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::subtract_n(v, A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__subtract (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::subtract(v, A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__subtract (typename Matrix::value_type v, Matrix& A, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      matscal::subtract(v, *b);
}

// Definition of subtract.
template <class Matrix>
inline void
subtract (typename Matrix::value_type v, Matrix& A) 
{
   typedef typename Matrix::value_type    T;

   if (v == T(0))
      return;

   if (A.has_contiguous_storage())
      vecscal::subtract(v, A);
   else
      matscal::__subtract(v, A, 
                          typename Matrix::blocking_tag(), 
                          typename Matrix::diagonal_storage_tag());
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Multiplies a matrix by a scalar.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      A(i,j) *= v 
   \endverbatim
   */
template <class Matrix>
void
mult (typename Matrix::value_type v, Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// MULT //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__mult (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::mult(v, A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::mult_n(v, A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__mult (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::mult(v, A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__mult (typename Matrix::value_type v, Matrix& A, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      matscal::mult(v, *b);
}

// Definition of mult.
template <class Matrix>
inline void
mult (typename Matrix::value_type v, Matrix& A) 
{
   typedef typename Matrix::value_type    T;

   if (v == T(1))
      return;

   if (A.has_contiguous_storage())
      vecscal::mult(v, A);
   else
      matscal::__mult(v, A, 
                      typename Matrix::blocking_tag(), 
                      typename Matrix::diagonal_storage_tag());
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION


/**
   Divides a matrix by a scalar.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      A(i,j) /= v 
   \endverbatim

   \note    Prefer multiply by 1/v for non integer types.
   */
template <class Matrix>
void
div (typename Matrix::value_type v, Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// MULT //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__div (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::div(v, A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::div_n(v, A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__div (typename Matrix::value_type v, Matrix& A, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::segment_iterator   Aend(A.seg_end());
   typename Matrix::segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      vecscal::div(v, A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__div (typename Matrix::value_type v, Matrix& A, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      matscal::div(v, *b);
}

// Definition of div.
template <class Matrix>
inline void
div (typename Matrix::value_type v, Matrix& A) 
{
   typedef typename Matrix::value_type    T;

   if (v == T(1))
      return;

   if (A.has_contiguous_storage())
      vecscal::div(v, A);
   else
      matscal::__div(v, A, 
                     typename Matrix::blocking_tag(), 
                     typename Matrix::diagonal_storage_tag());
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   One norm.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \return The sum of the norm of the elements in a matrix.

   \verbatim   
      sum (|A(i,j)|)  
   \endverbatim
   */
template <class Matrix> 
typename Matrix::value_type
one_norm (const Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// ONE_NORM //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__one_norm (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      s += vecscal::one_norm(A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      s += vecscal::one_norm_n(A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__one_norm (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      s += vecscal::one_norm(A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__one_norm (const Matrix& A, typename Matrix::value_type& s, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      s += matscal::one_norm(*b);
}

// Definition of one_norm.
template <class Matrix>
inline typename Matrix::value_type
one_norm (const Matrix& A) 
{
   typename Matrix::value_type      s(0);

   if (A.has_contiguous_storage())
      s = vecscal::one_norm(A);
   else
      matscal::__one_norm(A, s,
                          typename Matrix::blocking_tag(), 
                          typename Matrix::diagonal_storage_tag());
   return s;
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Square of two norm.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).
   \return The sum of two norms in a matrix.

   \verbatim   
      sum (A(i,j) * A(i,j))  
   \endverbatim
   */
template <class Matrix> 
typename Matrix::value_type
sqr_two_norm (const Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// SQR_TWO_NORM //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__sqr_two_norm (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      s += vecscal::sqr_two_norm(A_j);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      s += vecscal::sqr_two_norm_n(A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin());
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__sqr_two_norm (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      s += vecscal::sqr_two_norm(A_j);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__sqr_two_norm (const Matrix& A, typename Matrix::value_type& s, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      s += matscal::sqr_two_norm(*b);
}

// Definition of sqr_two_norm.
template <class Matrix>
inline typename Matrix::value_type
sqr_two_norm (const Matrix& A) 
{
   typename Matrix::value_type   s(0);

   if (A.has_contiguous_storage())
      s = vecscal::sqr_two_norm(A);
   else
      matscal::__sqr_two_norm(A, s,
                              typename Matrix::blocking_tag(), 
                              typename Matrix::diagonal_storage_tag());
   return s;
}

#endif //DOXYGEN_SKIP


/**
   Two norm (usual Euclidian norm).

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).
   \return The square root of the sum of two norms in a matrix.

   \verbatim   
      sqrt (sum (A(i,j) * A(i,j)))  
   \endverbatim
   */
template <class Matrix> 
inline typename Matrix::value_type
two_norm (const Matrix& A) 
{
   const typename Matrix::value_type   result   = sqrt(matscal::sqr_two_norm(A));     
   return result; 
}


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Maximum norm (infinite norm).

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).
   \return The max norms in a matrix.

   \verbatim    
      max (|A(i,j)|)  
   \endverbatim
   \pre     The matrix must not be empty (A.cols() > 0 and A.rows() > 0).
   \pre     Operator `Tabs` must be defined for the element type.
   \pre     Operator `<` must be defined for the element type.
   */
template <class Matrix> 
typename Matrix::value_type
max_norm (const Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// MAX_NORM //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__max_norm (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      si = vecscal::max_norm(A_j);
      s = std::max(s, si);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::max_norm_n(A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin(), s);
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__max_norm (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      si = vecscal::max_norm(A_j);
      s = std::max(s, si);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__max_norm (const Matrix& A, typename Matrix::value_type& s, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
   {
      const typename Matrix::value_type   si = matscal::max_norm(*b);
      s = std::max(s, si);
   }
}

// Definition of max_norm.
template <class Matrix>
inline typename Matrix::value_type
max_norm (const Matrix& A) 
{
   typename Matrix::value_type      s(0);

   assert (!A.empty());

   if (A.has_contiguous_storage())
      s = vecscal::max_norm(A);
   else
      matscal::__max_norm(A, s,
                          typename Matrix::blocking_tag(), 
                          typename Matrix::diagonal_storage_tag());
   return s;
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Maximum value.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).
   \return The max value in a matrix.

   \verbatim    
      max (A(i,j))  
   \endverbatim
   \pre     The matrix must not be empty (A.cols() > 0 and A.rows() > 0).
   \pre     Operator < must be defined for the element type.
   */
template <class Matrix> 
typename Matrix::value_type
max_value (const Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// MAX_VALUE //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__max_value (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      si = vecscal::max_value(A_j);
      s = std::max(s, si);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
   {
      si = *(std::max_element(A.diagonal_begin(), A.diagonal_end()));
      s = std::max(s, si);
   }
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__max_value (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      si = vecscal::max_value(A_j);
      s = std::max(s, si);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__max_value (const Matrix& A, typename Matrix::value_type& s, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
   {
      const typename Matrix::value_type   si = matscal::max_value(*b);
      s = std::max(s, si);
   }
}

// Definition of max_value.
template <class Matrix>
inline typename Matrix::value_type
max_value (const Matrix& A) 
{
   typedef typename Matrix::value_type   T;  

   T     s    = cm2::abs_min<T>::value();   

   assert (!A.empty());

   if (A.has_contiguous_storage())
      s = vecscal::max_value(A);
   else
      matscal::__max_value(A, s,
                           typename Matrix::blocking_tag(), 
                           typename Matrix::diagonal_storage_tag());
   return s;
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Minimum value.

   The matrix can be blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).
   \return The min value in a matrix.

   \verbatim    
      min(A(i,j))  
   \endverbatim
   \pre     The matrix must not be empty (A.cols() > 0 and A.rows() > 0).
   \pre     Operator < must be defined for the element type.
   */
template <class Matrix> 
typename Matrix::value_type
min_value (const Matrix& A);

#endif


#ifndef DOXYGEN_SKIP

// MIN_VALUE //
// non blocked, separate diagonal.
template <class Matrix> 
inline void
__min_value (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      si = vecscal::min_value(A_j);
      s = std::min(s, si);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
   {
      si = *(std::min_element(A.diagonal_begin(), A.diagonal_end()));
      s = std::min(s, si);
   }
}

// non blocked, non-separate diagonal.
template <class Matrix> 
inline void
__min_value (const Matrix& A, typename Matrix::value_type& s, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      si = vecscal::min_value(A_j);
      s = std::min(s, si);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix> 
inline void
__min_value (const Matrix& A, typename Matrix::value_type& s, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
   {
      const typename Matrix::value_type   si = matscal::min_value(*b);
      s = std::min(s, si);
   }
}

// Definition of min_value.
template <class Matrix>
inline typename Matrix::value_type
min_value (const Matrix& A) 
{
   typedef typename Matrix::value_type   T;   

   T     s    = std::numeric_limits<T>::max();   

   assert (!A.empty());

   if (A.has_contiguous_storage())
      s = vecscal::min_value(A);
   else
      matscal::__min_value(A, s,
                           typename Matrix::blocking_tag(), 
                           typename Matrix::diagonal_storage_tag());
   return s;
}

#endif //DOXYGEN_SKIP


#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Increases a range by the range of a matrix.

   \verbatim  
      R.first = min(R.first, A(i,j))
      R.second = max(R.second, A(i,j))
   \endverbatim

   \pre  The range \p R must be a valid range (`R.first <= R.second`).
   \pre  Operator < must be defined for the element type.
   */
template <class Matrix, class Range> 
void
inc_range (const Matrix& A, Range& R);

#endif


#ifndef DOXYGEN_SKIP

// INC_RANGE //
// non blocked, separate diagonal.
template <class Matrix, class Range> 
inline void
__inc_range (const Matrix& A, Range& R, cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      vecscal::inc_range(A_j, R);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      vecscal::inc_range_n(A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin(), R);
}

// non blocked, non-separate diagonal.
template <class Matrix, class Range> 
inline void
__inc_range (const Matrix& A, Range& R, cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());
   typename Matrix::value_type                     si;

   if (A_j < Aend) do {
      vecscal::inc_range(A_j, R);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix, class Range> 
inline void
__inc_range (const Matrix& A, Range& R, cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      matscal::inc_range (*b, R);
}

// Definition of inc_range.
template <class Matrix, class Range>
inline void
inc_range (const Matrix& A, Range& R) 
{
   if (A.has_contiguous_storage())
      vecscal::inc_range(A, R);
   else
      matscal::__inc_range(A, R,
                           typename Matrix::blocking_tag(), 
                           typename Matrix::diagonal_storage_tag());
}

#endif //DOXYGEN_SKIP


/**
   Computes the range of a matrix (first = min, second = max)

   If empty, first = numeric_limits\<T\>::max(), second = -numeric_limits\<T\>::max()
   \pre  Operator < must be defined for the element type.
   */
template <class Matrix> 
inline std::pair <typename Matrix::value_type, typename Matrix::value_type>
range (const Matrix& A) 
{
   typedef typename Matrix::value_type    T;
   std::pair<T,T>   R(std::numeric_limits<T>::max(), cm2::abs_min<T>::value());   
   matscal::inc_range(A, R);
   return R;
}

#ifndef CM2_NO_TMPL_FUNC_DECLARATION

/**
   Counter with predicate.

   \return The number of elements in a matrix that match a predicate.
   */
template <class Matrix, class Predicate> 
size_t
count_if (const Matrix& A, Predicate pred);

#endif


#ifndef DOXYGEN_SKIP

// COUNT_IF //
// non blocked, separate diagonal.
template <class Matrix, class Predicate> 
inline void
__count_if (const Matrix& A, Predicate pred, size_t& n , cm2::non_blocked_tag, cm2::separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      n += vecscal::count_if(A_j, pred);
      ++A_j;
   } while (A_j < Aend);

   if (!A.is_unit_diagonal()) 
      n += vecscal::count_if_n(A.diagonal_begin(), A.diagonal_end() - A.diagonal_begin(), pred);
}

// non blocked, non-separate diagonal.
template <class Matrix, class Predicate> 
inline void
__count_if (const Matrix& A, Predicate pred, size_t& n , cm2::non_blocked_tag, cm2::non_separate_diagonal_tag) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j(A.seg_begin());

   if (A_j < Aend) do {
      n += vecscal::count_if(A_j, pred);
      ++A_j;
   } while (A_j < Aend);
}

// blocked.
template <class Matrix, class Predicate> 
inline void
__count_if (const Matrix& A, Predicate pred, size_t& n , cm2::blocked_tag, typename Matrix::diagonal_storage_tag) 
{
   for (typename Matrix::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b)
      n += matscal::count_if(*b, pred);
}

// Definition of count_if.
template <class Matrix, class Predicate>
inline size_t
count_if (const Matrix& A, Predicate pred) 
{
   size_t    n (0);

   if (A.has_contiguous_storage())
      n = vecscal::count_if(A, pred);
   else
      matscal::__count_if(A, pred, n,
                          typename Matrix::blocking_tag(), 
                          typename Matrix::diagonal_storage_tag());
   return n;
}

#endif //DOXYGEN_SKIP


/**
   Normalizes a matrix (divides by its two norm).

   \verbatim     
      A(i,j) /= two_norm(A)  
   \endverbatim
   \pre The matrix must not be empty nor null.
   */
template <class Matrix>
inline Matrix&
normalize (Matrix& A) 
{ 
   matscal::mult(1./matscal::two_norm(A), A); 
   return A;
}

/**
   Tests all elements in a matrix against a scalar.

   Returns true if all elements equal to the scalar, false if at least one element does not equal to the scalar.
   */
template <class Matrix>
bool
equal (const Matrix& A, typename Matrix::value_type v) 
{
   const size_t      M(A.rows());
   const size_t      N(A.cols());
   bool              ok(!A.empty());

   for (size_t j = 0; ok && (j < N); ++j)
      ok = vecscal::equal_n(A.data_col(j), M, v);

   return ok;
}



#ifndef DOXYGEN_SKIP

//
template <class Matrix, class ColSizeTag>
inline typename Matrix::value_type
__pseudo_det (const Matrix& A, ColSizeTag, cm2::fixed_size_tag<1>) 
{   
   typename Matrix::value_type         s;
   s = vecscal::sqr_two_norm_n(A.begin(), A.rows());
   s = sqrt(s);
   return s;
}
//
template <class Matrix>
inline typename Matrix::value_type
__pseudo_det (const Matrix& A, cm2::fixed_size_tag<2>, cm2::fixed_size_tag<2>) 
{
   return matscal::det2(A);
}
//
template <class Matrix>
typename Matrix::value_type
__pseudo_det (const Matrix& A, cm2::fixed_size_tag<3>, cm2::fixed_size_tag<2>) 
{
   return vecvec::cross_two_norm3(A.begin(), A.begin()+3);
}
//
template <class Matrix>
typename Matrix::value_type
__pseudo_det (const Matrix& A, cm2::fixed_size_tag<3>, cm2::fixed_size_tag<3>) 
{
   return matscal::det3(A);
}

//
template <class Matrix>
typename Matrix::value_type
__pseudo_det (const Matrix& A, cm2::dynamic_size_tag, cm2::dynamic_size_tag) 
{
   typedef typename Matrix::col_size_tag  mat_col_size_tag;

   typename Matrix::value_type      s(0);

   if (A.cols() == 1)
      s = matscal::__pseudo_det(A, mat_col_size_tag(), cm2::fixed_size_tag<1>());
   else if ((A.rows() == 2) && (A.cols() == 2))
      s = matscal::__pseudo_det(A, cm2::fixed_size_tag<2>(), cm2::fixed_size_tag<2>());
   else if ((A.rows() == 3) && (A.cols() == 2))
      s = matscal::__pseudo_det(A, cm2::fixed_size_tag<3>(), cm2::fixed_size_tag<2>());
   else if ((A.rows() == 3) && (A.cols() == 3))
      s = matscal::__pseudo_det(A, cm2::fixed_size_tag<3>(), cm2::fixed_size_tag<3>());

   return s;
}


#endif // DOXYGEN_SKIP

/**
   Computes the pseudo determinant of a matrix A of size MxN.

   The definition of the pseudo determinant depends on the dimensions of the matrix :
      - N = 1        pseudo det = two_norm(A).
      - N = M = 2    pseudo det = det.
      - N = 2, M = 3 pseudo det = norm2 of the cross product of the two columns.
      - N = M = 3    pseudo det = det.

   \verbatim     
      B = invT(A) , with A % transpose(B) = Id
   \endverbatim     

   \pre  Matrix \p A must be of fixed size Mx1, 2x2, 2x3 or 3x3.  
   */
template <class Matrix>
inline typename Matrix::value_type
pseudo_det (const Matrix& A) 
{
   typedef typename Matrix::row_size_tag  mat_row_size_tag;
   typedef typename Matrix::col_size_tag  mat_col_size_tag;

   return matscal::__pseudo_det(A, mat_col_size_tag(), mat_row_size_tag());
}


/**
   Symmetrizes a matrix by adding its transpose matrix.

   A += transpose(A)
   \pre  Matrix \p A must be square (A.rows() == A.cols()).
   */
template <class MatA>
void
symmetrize (MatA& A) 
{
   const size_t      N(A.cols());

   assert (A.rows() == A.cols());

   for (size_t j = 0; j < N; ++j)
   {
      for (size_t i = 0; i < j; ++i)
         A.operator()(i,j) = (A.operator()(j,i) += A.operator()(i,j));
      A.operator()(j,j) *= 2.0;
   }
}
//@}

} // namespace matscal
} // end namespace cm2

#endif

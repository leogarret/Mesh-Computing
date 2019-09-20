/**
   \file       matmat.h
   \brief      Definition of matrix-matrix template functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3441 $
   $Date: 2019-01-08 14:15:26 +0100 (mar., 08 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATRIX_MATRIX_OPS_H__
#define __CM2_MATRIX_MATRIX_OPS_H__


#include "matvec_tags.h"
#include "math1_aux.h"
#include "vector_fixed.h"
#include "matscal.h"

#ifndef INLINE
#  define INLINE  inline
#endif

#ifndef CM2_RESTRICT
#  define CM2_RESTRICT
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (push)
#pragma warning (disable : 4068)             // Disable unrecognized OMP pragma.
#endif


/**
   \namespace  cm2::matmat
   \brief      Namespace for Matrix-Matrix template functions.
   */

namespace cm2 { 
namespace matmat {

/** \defgroup MatMatGroup Matrix-Matrix Template Functions */
//@{


/**
   Appends a matrix to another one. 

   If the target matrix \p M2 is empty upon entry, 
   it is resized to the dimensions of the source matrix \p M1.

   \param[in]     M1    The source matrix to be appended to \p M2.
   \param[in,out] M2    The target matrix.
   \return        Returns true if the operation succeed. 
                  False in case of memory allocation error.

   \pre           If none of the two matrices are empty upon entry, 
                  they must have the same number of rows.
   \warning       The matrices should not overlap.
   */
template <class Mat1, class Mat2>
bool 
push_back (const Mat1& M1, Mat2& M2)  
{ 
   const size_t               C1    = M1.cols();
   const size_t               C2    = M2.cols();
   const size_t               LD1   = M1.ld();
   const size_t               R1    = M1.rows();
   const size_t               R2    = M2.rows();
   const size_t               R     = std::max(R1, R2);
   typename Mat1::const_iterator p1 = M1.begin();
   typename Mat2::iterator       p2;
   size_t                     ld2;
   bool                       ok(true);

   assert (M1.empty() || M2.empty() || (M1.rows() == M2.rows()));

   if (C2+C1 > M2.cols_capacity())
      ok = M2.reserve(R, C2 + std::max(C1,C2));             // At least double the capacity of the matrix.

   ok &= M2.resize(R, C2+C1);

   if (ok)
   {
      ld2 = M2.ld();
      p2 = M2.begin() + ld2*C2;

      if ((LD1 == R) && (ld2 == R))                       // Contiguous storage.
         vecvec::copy_n(p1, p2, R*C1);
      else
      {
         for (size_t j = 0; j < C1; ++j, p1 += LD1, p2 += ld2)
            vecvec::copy_n(p1, p2, R);      
      }
   }

   return ok;
}   


/**
   Appends a batch of matrices to another one. 

   This function is more memory efficient than successively appending several source matrices to the target matrix.

   The number of rows of all source matrices must be equal, and also equal to the
   number of rows of the target matrix if not empty.

   \param[in]     M1_beg      Iterator to the first source matrix.
   \param[in]     M1_end      Iterator to the past-the-end source matrix = the next after the last.
   \param[in,out] M2          The target matrix.
                              Upon exit, all matrices between \p M1_beg and M1_end are successively appended to \p M2.

   \return        Returns true if the operation succeed. 
                  False in case of memory allocation error.

   \pre           All source and target matrices must have same number of rows (or be empty).

   If the target matrix \p M2 is empty upon entry, 
   it is resized to the dimensions of the source matrix \p M1.
   */
template <class Mat1Iterator, class Mat2>
bool 
push_back (Mat1Iterator M1_beg, Mat1Iterator M1_end, Mat2& M2)  
{ 
   size_t               R2(M2.rows());
   size_t               C2(M2.cols());
   Mat1Iterator         pm;
   bool                 ok;

   // Check dimensions.
   for (pm = M1_beg; pm < M1_end; ++pm)
   {
      if (pm->empty()) continue;
      C2 += pm->cols();
      if (R2 == 0) R2 = pm->rows();
      if (pm->rows() != R2)                     return false;     // Error.       
   }

   ok = M2.reserve(R2, C2);

   // Copy.
   if (ok)
   {
      for (pm = M1_beg; pm < M1_end; ++pm)
         M2.push_back(*pm);
   }

   return ok;
}   


/**
   Appends a specific batch of columns to a matrix.

   \param[in]     M1          The source matrix.
   \param[in]     M1_cols     The indices of the columns in \p M1 to be copied. \n
                              Must all refer to valid columns in matrix \p M1.
   \param[in,out] M2          The target matrix.
   \return        Returns true if the operation succeed. 
                  False in case of memory allocation error.

   \pre           All the values in \p M1_cols are smaller than \p M1.cols(). 
   \pre           M1.rows() == M2.rows() or M2 is empty.
   \warning       Multiple indices cause multiple copies.
   */
template <class Mat1, class Mat2, class Vector>
bool 
push_back (const Mat1& M1, const Vector& M1_cols, Mat2& M2)  
{
   const size_t               N1    = M1_cols.size();
   const size_t               C2    = M2.cols();
   const size_t               R1    = M1.rows();
   const size_t               R2    = M2.rows();
   const size_t               R     = std::max(R1, R2);
   typename Mat2::iterator    p2;
   size_t                     ld2, J;
   bool                       ok;

   if (C2+N1 > M2.cols_capacity())
      M2.reserve(R, C2 + std::max(N1,C2));            // At least double the capacity of the matrix.

   ok = M2.resize(R, C2+N1);

   if (ok)
   {
      ld2 = M2.ld();
      p2 = M2.begin() + ld2*C2;

      for (size_t j = 0; j < N1; ++j, p2 += ld2)
      {
         J = M1_cols[j];
         vecvec::copy_n(M1.data_col(J), p2, R);
      }
   }

   return ok;
}

/**
   Appends a specific column of a matrix to another a matrix.

   \param[in]     M1          The source matrix.
   \param[in]     j1          The column index in \p M1 to be appended to \p M2. \n
                              Must all refer to a valid column in matrix \p M1.
   \param[in,out] M2          The target matrix.
   \return        Returns true if the operation succeed. 
                  False in case of memory allocation error.

   \pre           M1.rows() == M2.rows() or M2 is empty.
   */
template <class Mat1, class Mat2>
bool 
push_back (const Mat1& M1, size_t j1, Mat2& M2)  
{
   const size_t               C2    = M2.cols();
   const size_t               R1    = M1.rows();
   const size_t               R2    = M2.rows();
   const size_t               R     = std::max(R1, R2);
   bool                       ok;

   if (C2+1 > M2.cols_capacity())
      M2.reserve(R, C2 + std::max(size_t(1),C2));  // At least double the capacity of the matrix.

   ok = M2.resize(R, C2+1);
   if (ok)
      vecvec::copy_n(M1.data_col(j1), M2.data_col(C2), R);

   return ok;
}


/**
   Tests the equality between two matrices.

   \note    If matrices \p M1 and \p M2 have different dimensions, they are not equal.
   */
template <class Mat1, class Mat2>
bool
equal (const Mat1& M1, const Mat2& M2) 
{
   const size_t   COLS  = M1.cols();
   bool           ok    = (M1.rows() == M2.rows()) && (M1.cols() == M2.cols());

   typename Mat1::const_segment_iterator  c1 = M1.seg_begin();
   typename Mat2::const_segment_iterator  c2 = M2.seg_begin();
   for (size_t j = 0; (j < COLS) && ok; ++j, ++c1, ++c2)
      ok = vecvec::equal(c1, c2);

   return ok;
}


/**
   Copies the diagonal of a matrix to the diagonal of another matrix.

   \verbatim     
      B(i,i) = a * A(i,i)  
      for i = 0...A.dim()-1  
   \endverbatim

   \pre  B.dim() >= A.dim()
   */
template <class T, class MatA, class MatB>
void
copy_diag (T a, const MatA& A, MatB& B) 
{
   size_t    i  = A.dim();

   assert (A.dim() <= B.dim());

   if (i) do {
      --i;
      B.operator()(i,i) = a * A.operator()(i,i);  
   } while (i);
}    

/**
   Copy between the diagonal of two matrices, with shift.

   \verbatim     
      B(i+shift,i+shift) = a * A(i,i)  
      for i = 0...A.dim()-1  
   \endverbatim

   \pre  A.dim() + shift <= B.dim()
   */
template <class T, class MatA, class MatB>
void
copy_diag (T a, const MatA& A, size_t shift, MatB& B) 
{
   const size_t    I  = A.dim() + shift;
   typename MatA::const_diagonal_iterator    da = A.diagonal_begin();

   assert (A.dim() + shift <= B.dim());

   for (size_t i = shift; i != I; ++i, ++da)
      B.operator()(i,i) = a * (*da);  
}    

/**
   AXPY between the diagonal of two matrices.

   \verbatim     
      B(i,i) += a * A(i,i)  
      for i = 0...A.dim()-1  
   \endverbatim

   \pre  B.dim() >= A.dim()
   */
template <class T, class MatA, class MatB>
void
axpy_diag (T a, const MatA& A, MatB& B) 
{
   const size_t       I  = A.dim();
   typename MatA::const_diagonal_iterator    da = A.diagonal_begin();

   assert (A.dim() <= B.dim());

   for (size_t i = 0; i != I; ++i, ++da)
      B.operator()(i,i) += a * (*da);  
}    

/**
   AXPY between the diagonal of two matrices, with shift.

   \verbatim     
      B(i+shift,i+shift) += a * A(i,i)  
      for i = 0...A.dim()-1  
   \endverbatim

   \pre  A.dim() + shift <= B.dim()
   */
template <class T, class MatA, class MatB>
void
axpy_diag (T a, const MatA& A, size_t shift, MatB& B) 
{
   const size_t    I  = A.dim() + shift;
   typename MatA::const_diagonal_iterator    da = A.diagonal_begin();

   assert (A.dim() + shift <= B.dim());

   for (size_t i = shift; i != I; ++i, ++da)
      B.operator()(i,i) += a * (*da);  
}    

/**
   Copies a row of a matrix to a row of another matrix.

   \verbatim     
      B(ib,j) = A(ia,j) 
      for j = 0...A.cols()-1  
   \endverbatim

   \pre  B.cols() >= A.cols()
   \pre  ia < A.rows()
   \pre  ib < B.rows()
   */
template <class MatA, class MatB>
inline void
copy_row (const MatA& A, size_t ia, MatB& B, size_t ib) 
{
   assert (A.cols() <= B.cols());
   assert (A.rows() > ia);
   assert (B.rows() > ib);
   vecvec::copy_n(A.row_begin(ia), B.row_begin(ib), A.cols());
}

/**
   Adds a row of a matrix to a row of another matrix.

   \verbatim     
      B(ib,j) += A(ia,j) 
      for j = 0...A.cols()-1  
   \endverbatim

   \pre  B.cols() >= A.cols()
   \pre  ia < A.rows()
   \pre  ib < B.rows()
    */
template <class MatA, class MatB>
inline void
add_row (const MatA& A, size_t ia, MatB& B, size_t ib) 
{
   assert (A.cols() <= B.cols());
   assert (A.rows() > ia);
   assert (B.rows() > ib);
   vecvec::add_n(A.row_begin(ia), B.row_begin(ib), A.cols());
}

/**
   AXPY a row of a matrix to a row of another matrix.

   \verbatim     
      B(ib,j) += a * A(ia,j)
      for j = 0...A.cols()-1  
   \endverbatim

   \pre  B.cols() >= A.cols()
   \pre  ia < A.rows()
   \pre  ib < B.rows()
    */
template <class MatA, class MatB, class T>
inline void
axpy_row (T a, const MatA& A, size_t ia, MatB& B, size_t ib) 
{
   assert (A.cols() <= B.cols());
   assert (A.rows() > ia);
   assert (B.rows() > ib);
   vecvec::axpy_n(a, A.row_begin(ia), B.row_begin(ib), A.cols());
}


#ifndef DOXYGEN_SKIP

// AXPY
template <class T, class MatA, class MatB>
void
__axpy (T a, const MatA& A, typename MatA::shape_tag, full_tag, typename MatA::orientation_tag, 
                   MatB& B, typename MatA::shape_tag, full_tag, typename MatA::orientation_tag) 
{
   if (A.has_contiguous_storage() && B.has_contiguous_storage())
      vecvec::axpy_n(a, A.begin(), B.begin(), A.entries());
   else
   {
      const typename MatA::const_segment_iterator     A_end = A.seg_end();
      typename MatA::const_segment_iterator           A_j   = A.seg_begin();
      typename MatB::segment_iterator                 B_j   = B.seg_begin();
      while (A_j < A_end)
      {
         vecvec::axpy(a, A_j, B_j);
         ++A_j; ++B_j;
      }
   }
}

// 
template <class T, class MatA, class MatB>
void
__axpy (T a, const MatA& A, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag, 
                   MatB& B, typename MatA::shape_tag,     full_tag, typename MatA::orientation_tag) 
{
   const typename MatA::const_segment_iterator     A_end = A.seg_end();
   typename MatA::const_iterator                   A_ij, A_j_end;

   for (typename MatA::const_segment_iterator A_j = A.seg_begin(); A_j < A_end; ++A_j) 
   {
      const size_t    j     = A_j.index();
      size_t          i     = A_j.index_begin();

      for (A_ij = A_j.begin(), A_j_end = A_j.end(); A_ij < A_j_end; ++i, ++A_ij)
      {
         const T     s = a * (*A_ij);
         if (s != T(0))
            B.operator()(i, j) += s;
      }
   }
}

// 
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, typename MatA::shape_tag,     full_tag, typename MatA::orientation_tag, 
                   MatB& B, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag) 
{
   matmat::__axpy(a, A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(), 
                     B, typename MatA::shape_tag(),     full_tag(), typename MatA::orientation_tag());   
}

// 
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag, 
                   MatB& B, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag) 
{
   matmat::__axpy(a, A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(), 
                     B, typename MatA::shape_tag(),     full_tag(), typename MatA::orientation_tag());   
}

// 
template <class T, class MatA, class MatB>
void
__axpy (T a, const MatA& A,     symmetric_tag, typename MatA::fullness_tag, typename MatA::orientation_tag, 
                   MatB& B, non_symmetric_tag, typename MatB::fullness_tag, typename MatA::orientation_tag) 
{
   const typename MatA::const_segment_iterator     A_end = A.seg_end();
   typename MatA::const_iterator                   A_ij, A_j_end;

   for (typename MatA::const_segment_iterator A_j = A.seg_begin(); A_j < A_end; ++A_j) 
   {
      const size_t    j     = A_j.index();
      size_t          i     = A_j.index_begin();
      for (A_ij = A_j.begin(), A_j_end = A_j.end(); A_ij < A_j_end; ++i, ++A_ij) 
      {
         const T  s = a * (*A_ij);
         if (s == T(0))
            continue;
         B.operator()(i,j) += s;
         if (i != j) 
            B.operator()(j,i) += s;
      }
   }
}

// 
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, separate_diagonal_tag, size_t shift,
                   MatB& B, typename MatB::diagonal_storage_tag) 
{
   if (!B.is_unit_diagonal())
   {
      if (A.is_unit_diagonal()) 
         matscal::add_diag(a, B);
      else
         matmat::axpy_diag(a, A, shift, B);
   }

   matmat::__axpy(a, A, typename MatA::shape_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(),
                     B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag());
}

// 
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, non_separate_diagonal_tag, size_t,
                   MatB& B, non_separate_diagonal_tag) 
{
   matmat::__axpy(a, A, typename MatA::shape_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(),
                     B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag());
}

// 
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, non_separate_diagonal_tag, size_t,
                   MatB& B,     separate_diagonal_tag) 
{
   matmat::__axpy(a, A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(),
                     B, typename MatB::shape_tag(), non_full_tag(), typename MatB::orientation_tag());
}

// Non-blocked Matrix.
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, non_blocked_tag, size_t shift, MatB& B) 
{
   matmat::__axpy(a, A, typename MatA::diagonal_storage_tag(), shift,
                     B, typename MatB::diagonal_storage_tag());
}

// Blocked Matrix.
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, blocked_tag, size_t shift, MatB& B) 
{
   for (typename MatA::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b) 
   {
      matmat::__axpy(a, *b, typename MatA::block_type::blocking_tag(), shift, B);
      shift += b->dim();
   }
}

// 
template <class T, class MatA, class MatB>
inline void
__axpy (T a, const MatA& A, size_t shift, MatB& B) 
{
   matmat::__axpy(a, A, typename MatA::blocking_tag(), shift, B);
}


// COPY_SCALE
template <class T, class MatA, class MatB>
void
__copy_scale (T a, const MatA& A, typename MatA::shape_tag, full_tag, typename MatA::orientation_tag, 
                         MatB& B, typename MatA::shape_tag, full_tag, typename MatA::orientation_tag) 
{
   if (A.has_contiguous_storage() && B.has_contiguous_storage())
      vecvec::copy_scale_n(a, A.begin(), B.begin(), A.entries());
   else
   {
      const typename MatA::const_segment_iterator     A_end = A.seg_end();
      typename MatA::const_segment_iterator           A_j   = A.seg_begin();
      typename MatB::segment_iterator                 B_j   = B.seg_begin();
      while (A_j < A_end)
      {
         vecvec::copy_scale(a, A_j, B_j);
         ++A_j; ++B_j;
      }
   }
}

// 
template <class T, class MatA, class MatB>
void
__copy_scale (T a, const MatA& A, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag, 
                         MatB& B, typename MatA::shape_tag,     full_tag, typename MatA::orientation_tag) 
{
   const typename MatA::const_segment_iterator     A_end = A.seg_end();
   typename MatA::const_iterator                   A_ij, A_j_end;

   for (typename MatA::const_segment_iterator A_j = A.seg_begin(); A_j < A_end; ++A_j) 
   {
      const size_t    j     = A_j.index();
      size_t          i     = A_j.index_begin();

      for (A_ij = A_j.begin(), A_j_end = A_j.end(); A_ij < A_j_end; ++i, ++A_ij) 
         B.operator()(i,j) = a * (*A_ij);
   }
}

// 
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, typename MatA::shape_tag,     full_tag, typename MatA::orientation_tag, 
                         MatB& B, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag) 
{
   matmat::__copy_scale(a, A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(), 
                           B, typename MatA::shape_tag(),     full_tag(), typename MatA::orientation_tag());   
}

// 
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag, 
                         MatB& B, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag) 
{
   matmat::__copy_scale(a, A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(), 
                           B, typename MatA::shape_tag(),     full_tag(), typename MatA::orientation_tag());   
}

// 
template <class T, class MatA, class MatB>
void
__copy_scale (T a, const MatA& A,     symmetric_tag, typename MatA::fullness_tag, typename MatA::orientation_tag, 
                         MatB& B, non_symmetric_tag, typename MatB::fullness_tag, typename MatA::orientation_tag) 
{
   const typename MatA::const_segment_iterator     A_end = A.seg_end();
   typename MatA::const_iterator                   A_ij, A_j_end;

   for (typename MatA::const_segment_iterator A_j = A.seg_begin(); A_j < A_end; ++A_j) 
   {
      const size_t    j     = A_j.index();
      size_t          i     = A_j.index_begin();
      for (A_ij = A_j.begin(), A_j_end = A_j.end(); A_ij < A_j_end; ++i, ++A_ij) 
      {
         const T  s = a * (*A_ij);
         B.operator()(i,j) = s;
         if (i != j) 
            B.operator()(j,i) = s;
      }
   }
}

// 
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, separate_diagonal_tag, size_t shift,
                         MatB& B, typename MatB::diagonal_storage_tag) 
{
   if (!B.is_unit_diagonal())
   {
      if (A.is_unit_diagonal()) 
         matscal::copy_diag(a, B);
      else
         matmat::copy_diag(a, A, shift, B);
   }

   matmat::__copy_scale(a, A, typename MatA::shape_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(),
                           B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag());
}

// 
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, non_separate_diagonal_tag, size_t,
                         MatB& B, non_separate_diagonal_tag) 
{
   matmat::__copy_scale (a, A, typename MatA::shape_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(),
                            B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag());
}

// 
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, non_separate_diagonal_tag, size_t,
                         MatB& B,     separate_diagonal_tag) 
{
   matmat::__copy_scale(a, A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(),
                           B, typename MatB::shape_tag(), non_full_tag(), typename MatB::orientation_tag());
}

// Non-blocked Matrix.
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, non_blocked_tag, size_t shift, MatB& B) 
{
   matmat::__copy_scale(a, A, typename MatA::diagonal_storage_tag(), shift,
                           B, typename MatB::diagonal_storage_tag());
}

// Blocked Matrix.
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, blocked_tag, size_t shift, MatB& B) 
{
   for (typename MatA::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b) 
   {
      matmat::__copy_scale(a, *b, typename MatA::block_type::blocking_tag(), shift, B);
      shift += b->dim();
   }
}

// 
template <class T, class MatA, class MatB>
inline void
__copy_scale (T a, const MatA& A, size_t shift, MatB& B) 
{
   matmat::__copy_scale(a, A, typename MatA::blocking_tag(), shift, B);
}



// COPY
template <class MatA, class MatB>
void
__copy (const MatA& A, typename MatA::shape_tag, full_tag, typename MatA::orientation_tag, 
              MatB& B, typename MatA::shape_tag, full_tag, typename MatA::orientation_tag) 
{
   if (A.has_contiguous_storage() && B.has_contiguous_storage())
      vecvec::copy_n(A.begin(), B.begin(), A.entries());
   else
   {
      const typename MatA::const_segment_iterator     A_end = A.seg_end();
      typename MatA::const_segment_iterator           A_j   = A.seg_begin();
      typename MatB::segment_iterator                 B_j   = B.seg_begin();
      while (A_j < A_end)
      {
         vecvec::copy(A_j, B_j);
         ++A_j; ++B_j;
      }
   }
}

// 
template <class MatA, class MatB>
void
__copy (const MatA& A, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag, 
              MatB& B, typename MatA::shape_tag,     full_tag, typename MatA::orientation_tag) 
{
   const typename MatA::const_segment_iterator     A_end = A.seg_end();
   typename MatA::const_iterator                   A_ij, A_j_end;

   for (typename MatA::const_segment_iterator A_j = A.seg_begin(); A_j < A_end; ++A_j) 
   {
      const size_t    j     = A_j.index();
      size_t          i     = A_j.index_begin();
      for (A_ij = A_j.begin(), A_j_end = A_j.end(); A_ij < A_j_end; ++i, ++A_ij)
         B.operator()(i, j) = *A_ij;
   }
}

// 
template <class MatA, class MatB>
inline void
__copy (const MatA& A, typename MatA::shape_tag,     full_tag, typename MatA::orientation_tag, 
              MatB& B, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag) 
{
   matmat::__copy(A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(), 
                  B, typename MatA::shape_tag(),     full_tag(), typename MatA::orientation_tag());   
}

// 
template <class MatA, class MatB>
inline void
__copy (const MatA& A, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag, 
              MatB& B, typename MatA::shape_tag, non_full_tag, typename MatA::orientation_tag) 
{
   matmat::__copy(A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(), 
                  B, typename MatA::shape_tag(),     full_tag(), typename MatA::orientation_tag());   
}

// 
template <class MatA, class MatB>
void
__copy (const MatA& A,     symmetric_tag, typename MatA::fullness_tag, typename MatA::orientation_tag, 
              MatB& B, non_symmetric_tag, typename MatB::fullness_tag, typename MatA::orientation_tag) 
{
   const typename MatA::const_segment_iterator     A_end = A.seg_end();
   typename MatA::const_iterator                   A_ij, A_j_end;

   for (typename MatA::const_segment_iterator A_j = A.seg_begin(); A_j < A_end; ++A_j) 
   {
      const size_t    j     = A_j.index();
      size_t          i     = A_j.index_begin();
      for (A_ij = A_j.begin(), A_j_end = A_j.end(); A_ij < A_j_end; ++i, ++A_ij) 
      {
         B.operator()(i,j) = *A_ij;
         if (i != j) 
            B.operator()(j,i) = *A_ij;
      }
   }
}

// 
template <class MatA, class MatB>
inline void
__copy (const MatA& A, separate_diagonal_tag, size_t shift,
              MatB& B, typename MatB::diagonal_storage_tag) 
{
   if (!B.is_unit_diagonal())
   {
      if (A.is_unit_diagonal()) 
         matscal::copy_diag(1, B);
      else
         matmat::copy_diag(1, A, shift, B);
   }

   matmat::__copy(A, typename MatA::shape_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(),
                  B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag());
}

// 
template <class MatA, class MatB>
inline void
__copy (const MatA& A, non_separate_diagonal_tag, size_t,
              MatB& B, non_separate_diagonal_tag) 
{
   matmat::__copy(A, typename MatA::shape_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(),
                  B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag());
}

// 
template <class MatA, class MatB>
inline void
__copy (const MatA& A, non_separate_diagonal_tag, size_t,
              MatB& B,     separate_diagonal_tag) 
{
   matmat::__copy(A, typename MatA::shape_tag(), non_full_tag(), typename MatA::orientation_tag(),
                  B, typename MatB::shape_tag(), non_full_tag(), typename MatB::orientation_tag());
}

// Non-blocked Matrix.
template <class MatA, class MatB>
inline void
__copy (const MatA& A, non_blocked_tag, size_t shift, MatB& B) 
{
   matmat::__copy(A, typename MatA::diagonal_storage_tag(), shift,
                  B, typename MatB::diagonal_storage_tag());
}

// Blocked Matrix.
template <class MatA, class MatB>
inline void
__copy (const MatA& A, blocked_tag, size_t shift, MatB& B) 
{
   for (typename MatA::const_block_iterator b = A.blocks_begin(); b < A.blocks_end(); ++b) 
   {
      matmat::__copy(*b, typename MatA::block_type::blocking_tag(), shift, B);
      shift += b->dim();
   }
}

// 
template <class MatA, class MatB>
inline void
__copy (const MatA& A, size_t shift, MatB& B) 
{
   matmat::__copy(A, typename MatA::blocking_tag(), shift, B);
}

#endif // DOXYGEN_SKIP


/**
   AXPY between two matrices.

   \verbatim     
      B(i,j) += a * A(i,j) 
   \endverbatim

   \pre A.rows() == B.rows()
   \pre A.cols() == B.cols()
   */
template <class MatA, class MatB, class T>
inline void
axpy (T a, const MatA& A, MatB& B) 
{
   assert (A.cols() == B.cols());
   assert (A.rows() == B.rows());

   // Fast return.
   if (a == T(0)) 
      return;

   matmat::__axpy(a, A, size_t(0), B);
}

/**
   Adds a matrix to another matrix.

   \verbatim     
      B(i,j) += A(i,j) 
   \endverbatim

   \pre A.rows() == B.rows()
   \pre A.cols() == B.cols()
   */
template <class MatA, class MatB>
inline void
add (const MatA& A, MatB& B) 
{
   typedef typename MatA::value_type     T;
   matmat::axpy(T(1), A, B);
}

/**
   Subtracts a matrix to another matrix.

   \verbatim     
      B(i,j) -= A(i,j) 
   \endverbatim

   \pre A.rows() == B.rows()
   \pre A.cols() == B.cols()
   */
template <class MatA, class MatB>
inline void
subtract (const MatA& A, MatB& B) 
{
   typedef typename MatA::value_type     T;
   matmat::axpy(T(-1), A, B);
}

/**
   Copies with scaling a matrix to another matrix.

   \verbatim     
      B(i,j) = a * A(i,j) 
   \endverbatim

   \pre  A.rows() == B.rows()
   \pre  A.cols() == B.cols()

   \warning    This function copies the profile of matrix A into matrix B.\n
   \warning    An error will probably occur if profile of A is not equal or not included in profile of B.
   \warning    If profile of A is strictly included in profile of B, some terms in B will not be 
               affected. Set matrix B to 0 before to make sure all terms are equal.\n
   */
template <class MatA, class MatB, class T>
inline void
copy_scale (T a, const MatA& A, MatB& B) 
{
   assert (A.cols() == B.cols());
   assert (A.rows() == B.rows());

   // Fast return.
   if (A.empty())
      return;

   matmat::__copy_scale(a, A, typename MatA::blocking_tag(), size_t(0), B);
}

/**
   Copies a matrix to another matrix.

   \verbatim     
      B(i,j) = A(i,j) 
   \endverbatim

   \pre  A.rows() == B.rows()
   \pre  A.cols() == B.cols()

   \warning    This function copies the profile of matrix A into matrix B.\n
   \warning    An error will probably occur if profile of A is not equal or not included in profile of B.
   \warning    If profile of A is strictly included in profile of B, some terms in B will not be 
               affected. Set matrix B to 0 before to make sure all terms are equal.\n
   */
template <class MatA, class MatB>
inline void
copy (const MatA& A, MatB& B) 
{
   assert (A.cols() == B.cols());
   assert (A.rows() == B.rows());

   // Fast return.
   if (A.empty())
      return;

   matmat::__copy(A, typename MatA::blocking_tag(), size_t(0), B);
}


/**
   Adds the transpose of a matrix into another matrix.

   \verbatim     
      B(i,j) += A(j,i) 
   \endverbatim

   \pre A.cols() == B.rows()
   \pre A.rows() == B.cols()
   \warning To get B = transpose(A) , don't forget to set \p B to zero before this call.
   \todo    This function should be rewritten for better performance. 
   */
template <class MatA, class MatB>
void
transpose (const MatA& A, MatB& B) 
{
   const size_t M  = A.rows();
   const size_t N  = A.cols();

   assert (A.cols() == B.rows());
   assert (A.rows() == B.cols());

   for (size_t i = 0; i < M; ++i)
      for (size_t j = 0; j < N; ++j) 
         B(j,i) += A.operator()(i,j);
}


/**
   Matrix-matrix multiplication, with scaling.

   \verbatim     
      C += a * A % B 
   \endverbatim

   \pre A.cols() == B.rows()
   \pre A.rows() == C.rows()
   \pre B.cols() == C.cols()
   \warning To get C = a * A % B , don't forget to set \p C to zero prior to the multiplication.
   */
template <class T, class MatA, class MatB, class MatC>
inline void
mult (T a, const MatA& A, const MatB& B, MatC& C,
      size_t cache_size = 0, int nthreads = 1);

/**
   Matrix-matrix multiplication.

   \verbatim     
      C += A % B 
   \endverbatim

   \pre A.cols() == B.rows()
   \pre A.rows() == C.rows()
   \pre B.cols() == C.cols()
   \warning To get C = A % B , don't forget to set \p C to zero prior to the multiplication.
   */
template <class MatA, class MatB, class MatC>
inline void
mult (const MatA& A, const MatB& B, MatC& C,
      size_t cache_size = 0, int nthreads = 1)
{
   typedef typename MatA::value_type   T;
   matmat::mult(T(1), A, B, C, cache_size, nthreads);
}

/**
   Matrix-matrix transpose multiplication, with scaling.

   \verbatim     
      C += a * transpose(A) % B 
   \endverbatim

   \pre A.rows() == B.rows()
   \pre A.cols() == C.rows()
   \pre B.cols() == C.cols()
   \warning To get C = a * transpose(A) % B , don't forget to set \p C to zero prior to the multiplication.
   */
template <class T, class MatA, class MatB, class MatC>
inline void
transpose_mult 
   (T a, const MatA& A, const MatB& B, MatC& C,
    size_t cache_size = 0, int nthreads = 1);

/**
   Matrix-matrix transpose multiplication.

   \verbatim     
      C += transpose(A) % B 
   \endverbatim

   \pre A.rows() == B.rows()
   \pre A.cols() == C.rows()
   \pre B.cols() == C.cols()
   \warning To get C = transpose(A) % B , don't forget to set \p C to zero prior to the multiplication.
   */
template <class MatA, class MatB, class MatC>
inline void
transpose_mult 
   (const MatA& A, const MatB& B, MatC& C, 
    size_t cache_size = 0, int nthreads = 1) 
{
   typedef typename MatA::value_type   T;
   matmat::transpose_mult(T(1), A, B, C, cache_size, nthreads);
}


/**
   Diagonal mult (with scaling).

   \verbatim     
      C += a * D % B 
   \endverbatim

   \pre D.dim() == B.rows()
   \pre B.rows() == C.rows()
   \pre B.cols() == C.cols()
   \warning To get C = D % B , don't forget to set \p C to zero prior to the multiplication.
   */
template <class T, class DiagD, class MatB, class MatC>
inline void
diag_mult (T a, const DiagD& D, MatB& B, MatC& C, int nthreads);


/**
   Diagonal mult (with scaling).

   \verbatim     
      B = D % B 
   \endverbatim

   \pre D.dim() == B.rows()
   */
template <class DiagD, class MatB>
inline void
diag_mult (const DiagD& D, MatB& B, int nthreads);


#ifndef DOXYGEN_SKIP

/* MULT.
   Version for three full rectangular matrices.
   \note    Uses hierarchic block + cache tiling.
   */
template <class T, class MatA, class MatB, class MatC>
void
__mult
   (T a, 
    const MatA& A, non_symmetric_tag, full_tag, col_oriented_tag, 
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag, 
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag,
    size_t cache_size, int nthreads) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename MatC::value_type         TC;

   const size_t                                    ROWS  = A.rows();
   const size_t                                    COLS  = B.cols();
   const size_t                                    ACOLS = A.cols();
   const typename MatA::const_segment_iterator     A_0   = A.seg_begin();
   const typename MatB::const_segment_iterator     B_0   = B.seg_begin();
   const typename MatC::segment_iterator           C_0   = C.seg_begin();
   typename MatA::const_segment_iterator           A_I0, A_I1;
   typename MatB::const_segment_iterator           B_J0, B_J1;
   typename MatC::segment_iterator                 C_J0, C_J1;
   const TB* CM2_RESTRICT                          pb;
   TC* CM2_RESTRICT                                pc;
   size_t                                          Ci(ACOLS), Cj(COLS), ci, cj, ni, nj;
   size_t                                          I0, I1, J0, J1;

   assert (A.rows() == C.rows());
   assert (B.cols() == C.cols());

   // Compute the number of columns fitting in cache.
   {
      const size_t   a_  = A.rows() * sizeof(TA);
      const size_t   b_  = B.rows() * sizeof(TB);
      const size_t   c_  = C.rows() * sizeof(TC);
      if ((cache_size > 0) && (cache_size < Ci*a_ + Cj*(b_+c_)))     // Doesn't fit in cache.
      {
         const double r = double(cache_size) / double(Ci*a_ + Cj*(b_+c_));
         Ci = std::max(size_t(r * double(ACOLS)), size_t(1));
         Cj = std::max(size_t(r * double(COLS)), size_t(1));
      }
   }

   // Loop on blocks of B-C.
   for (J0 = 0, B_J0 = B_0, C_J0 = C_0; J0 < COLS; J0 += nj, B_J0 += nj, C_J0 += nj)
   {
      pb = B_J0.begin();                  // Load first cols of B.
      pc = C_J0.begin();                  // Load first cols of C.
      assert (B.last_incore_col() > J0);
      assert (C.last_incore_col() > J0);
      nj = std::min(B.last_incore_col(), C.last_incore_col()) - J0;

      // Loop on blocks of A.
      for (I0 = 0, A_I0 = A_0; I0 < ACOLS; I0 += ni, A_I0 += ni)
      {
         (void)A_I0.begin();              // Load first cols of A.
         assert (A.last_incore_col() > I0);
         ni = A.last_incore_col() - I0;

         // Loop on cache blocks of B-C.
         for (J1 = J0, B_J1 = B_J0, C_J1 = C_J0; J1 < J0 + nj; J1 += cj, B_J1 += cj, C_J1 += cj)
         {
            cj = std::min(Cj, nj);
            cj = std::min(cj, J0 + nj - J1);

            // Loop on cache blocks of A.
            for (I1 = I0, A_I1 = A_I0; I1 < I0 + ni; I1 += ci, A_I1 += ci)
            {
               ci = std::min(Ci, ni);
               ci = std::min(ci, I0 + ni - I1);

               // Loop inside B-C block.
#ifdef _OPENMP
               if ((nthreads > 1) && (cj > 1) && (ROWS*ci*cj > CM2_OMP_MIN_LOOP))            
               {
                  #pragma omp parallel for private(pb,pc) schedule(static) num_threads(nthreads)       // Parallelization over columns.
                  for (int j = 0; j < int(cj); ++j)
                  {
                     typename MatA::const_segment_iterator     A_i(A_I1);
                     typename MatB::const_segment_iterator     B_j(B_J1);  B_j += j; 
                     typename MatC::segment_iterator           C_j(C_J1);  C_j += j;

                     pb = B_j.begin() + I1;  pc = C_j.begin();
                     for (size_t i = 0; i < ci; ++i, ++pb, ++A_i)
                        vecvec::axpy_n(a * (*pb), A_i.begin(), pc, ROWS);
                  }
               }
               else
#endif
               {
                  typename MatA::const_segment_iterator     A_i(A_I1);
                  typename MatB::const_segment_iterator     B_j(B_J1); 
                  typename MatC::segment_iterator           C_j(C_J1); 

                  for (size_t j = 0; j < cj; ++j, ++B_j, ++C_j, A_i = A_I1) 
                  {
                     pb = B_j.begin() + I1;  pc = C_j.begin();
                     for (size_t i = 0; i < ci; ++i, ++pb, ++A_i)
                        vecvec::axpy_n_par(a * (*pb), A_i.begin(), pc, ROWS, nthreads);               // Parallelization inside columns.
                  }
               }
            }
         }
      }
   }
}


/* MULT.
   Version for two full rectangular matrices (A and B) and one full symmetric matrix (C).
   \note    Uses hierarchic block + cache tiling.
   */
template <class T, class MatA, class MatB, class MatC>
void
__mult
   (T a, 
    const MatA& A, non_symmetric_tag, full_tag, col_oriented_tag, 
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag,
          MatC& C,     symmetric_tag, full_tag, col_oriented_tag,
    size_t cache_size, int nthreads) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename MatC::value_type         TC;

   const size_t                                    COLS  = B.cols();
   const size_t                                    ACOLS = A.cols();
   const typename MatA::const_segment_iterator     A_0   = A.seg_begin();
   const typename MatB::const_segment_iterator     B_0   = B.seg_begin();
   const typename MatC::segment_iterator           C_0   = C.seg_begin();
   typename MatA::const_segment_iterator           A_I0, A_I1;
   typename MatB::const_segment_iterator           B_J0, B_J1;
   typename MatC::segment_iterator                 C_J0, C_J1;
   const TB* CM2_RESTRICT                          pb;
   TC* CM2_RESTRICT                                pc;
   size_t                                          Ci(ACOLS), Cj(COLS), ci, cj, ni, nj;
   size_t                                          I0, I1, J0, J1;

   assert (A.rows() == C.dim());
   assert (B.cols() == C.dim());

   // Compute the number of columns fitting in cache.
   {
      const size_t   a_  = A.rows() * sizeof(TA);
      const size_t   b_  = B.rows() * sizeof(TB);
      const size_t   c_  = C.dim()  * sizeof(TC);
      if ((cache_size > 0) && (cache_size < Ci*a_ + Cj*(b_+c_)))     // Doesn't fit in cache.
      {
         const double r = double(cache_size) / double(Ci*a_ + Cj*(b_+c_));
         Ci = std::max(size_t(r * double(ACOLS)), size_t(1));
         Cj = std::max(size_t(r * double(COLS)), size_t(1));
      }
   }
               
   // Loop on blocks of B-C.
   for (J0 = 0, B_J0 = B_0, C_J0 = C_0; J0 < COLS; J0 += nj, B_J0 += nj, C_J0 += nj)
   {
      pb = B_J0.begin();                  // Load first cols of B.
      assert (B.last_incore_col() > J0);
      nj = B.last_incore_col() - J0;

      // Loop on blocks of A.
      for (I0 = 0, A_I0 = A_0; I0 <= ACOLS; I0 += ni, A_I0 += ni)
      {
         (void)A_I0.begin();              // Load first cols of A.
         assert (A.last_incore_col() > I0);
         ni = A.last_incore_col() - I0;

         // Loop on cache blocks of B-C.
         for (J1 = J0, B_J1 = B_J0, C_J1 = C_J0; J1 < J0 + nj; J1 += cj, B_J1 += cj, C_J1 += cj)
         {
            cj = std::min(Cj, nj);
            cj = std::min(cj, J0+nj-J1);

            // Loop on cache blocks of A.
            for (I1 = I0, A_I1 = A_I0; I1 < I0 + ni; I1 += ci, A_I1 += ci)
            {
               ci = std::min(Ci, ni);
               ci = std::min(ci, I0+ni-I1);

               // Loop inside B-C block.
#ifdef _OPENMP
               if ((nthreads > 1) && (cj > 1) && ((J1+(cj+1)/2)*ci*cj > CM2_OMP_MIN_LOOP))   
               {
                  #pragma omp parallel for private(pb,pc) schedule(static) num_threads(nthreads)       // Parallelization over columns.
                  for (int j = 0; j < int(cj); ++j)
                  {
                     typename MatA::const_segment_iterator     A_i(A_I1);
                     typename MatB::const_segment_iterator     B_j(B_J1);  B_j += j; 
                     typename MatC::segment_iterator           C_j(C_J1);  C_j += j;

                     pb = B_j.begin() + I1;  pc = C_j.begin();
                     for (size_t i = 0; i < ci; ++i, ++pb, ++A_i)
                        vecvec::axpy_n(a * (*pb), A_i.begin(), pc, J1+j+1);
                     }
               } 
               else
#endif
               {
                  typename MatA::const_segment_iterator     A_i(A_I1);
                  typename MatB::const_segment_iterator     B_j(B_J1); 
                  typename MatC::segment_iterator           C_j(C_J1); 

                  for (size_t j = 0; j < cj; ++j, ++B_j, ++C_j, A_i = A_I1)   
                  {
                     pb = B_j.begin() + I1;  pc = C_j.begin();
                     for (size_t i = 0; i < ci; ++i, ++pb, ++A_i)
                        vecvec::axpy_n_par(a * (*pb), A_i.begin(), pc, J1+j+1, nthreads);             // Parallelization inside columns.
                  }
               } 
            }
         }
      }
   }
}


// MULT (A symmetric, non-blocked).
template <class T, class MatA, class MatB, class MatC>
void
__mult
   (T a, 
    const MatA& A, symmetric_tag, non_blocked_tag,
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag,
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag,
    size_t /*cache_size*/, int nthreads) 
{
   const size_t                              COLS  = B.cols();
   typename MatB::const_segment_iterator     b0    = B.seg_begin();
   typename MatC::segment_iterator           c0    = C.seg_begin();
   size_t                                    cols(0);

   for (size_t j0 = 0; j0 < COLS; j0 += cols, b0 += cols, c0 += cols)
   {
      (void)b0.begin();                   // Load first cols of B.
      (void)c0.begin();                   // Load first cols of C.
      assert (std::min(B.last_incore_col(), C.last_incore_col()) > j0);
      cols = std::min(B.last_incore_col(), C.last_incore_col()) - j0;

#ifdef _OPENMP
      if ((nthreads > 1) && (cols > 1) && (2*A.entries()*cols > CM2_OMP_MIN_LOOP))    // Parallel version.
      {
         #pragma omp parallel for schedule(static) num_threads(nthreads)
         for (int j = 0; j < int(cols); ++j)
         {
            typename MatB::const_segment_iterator     b(b0);  b += j; 
            typename MatC::segment_iterator           c(c0);  c += j; 
            matvec::mult(a, A, b, c);
         }
      }
      else
#endif
      {
         typename MatB::const_segment_iterator     b(b0);
         typename MatC::segment_iterator           c(c0);
         for (size_t j = 0; j < cols; ++j, ++b, ++c)     
            matvec::mult(a, A, b, c, nthreads);
      }
   }
}


// MULT (A symmetric, blocked).
template <class T, class MatA, class MatB, class MatC>
void
__mult 
   (T a, 
    const MatA& A, symmetric_tag, blocked_tag, 
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag, 
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag, 
    size_t /*cache_size*/, int nthreads) 
{
   const size_t                              COLS  = B.cols();
   typename MatA::const_block_iterator       A0;
   typename MatB::const_segment_iterator     b0    = B.seg_begin();
   typename MatC::segment_iterator           c0    = C.seg_begin();
   size_t                                    blocks(0);
   int                                       cols(0);
   bool                                      parallel_cond;

   for (size_t j0 = 0; j0 < COLS; j0 += cols, b0 += cols, c0 += cols) 
   {
      (void)b0.begin();                   // Load first cols of B.
      (void)c0.begin();                   // Load first cols of C.
      assert (std::min(B.last_incore_col(), C.last_incore_col()) > j0);
      cols = int(std::min(B.last_incore_col(), C.last_incore_col()) - j0);

      for (A0 = A.blocks_begin(); A0 < A.blocks_end(); A0 += blocks) 
      {
         (void)A0->data();                // Load A0 and following blocks.
         blocks = A.end_incore_block_id() - A0.ID();
         assert(blocks > 0);

         parallel_cond = (nthreads > 1) && (cols > 1) && (2*A.incore_entries()*cols > CM2_OMP_MIN_LOOP);
         #pragma omp parallel for if(parallel_cond) schedule(static) num_threads(nthreads)
         for (int j = 0; j < cols; ++j)
         {
            typename MatA::const_block_iterator    Ak(A0);
            typename MatB::const_segment_iterator  b(b0);  b += j; 
            typename MatC::segment_iterator        c(c0);  c += j; 
            for (size_t k = 0; k < blocks; ++k, ++Ak) 
               matvec::__mult(a, *Ak, b.begin(), c.begin(), /*nthreads=>*/ 1);
         }
      }
   } 
}
  

// MULT.
template <class T, class MatA, class MatB, class MatC>
inline void
__mult
   (T a, 
    const MatA& A, non_symmetric_tag, non_full_tag, typename MatA::orientation_tag,
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag,
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag,
    size_t cache_size, int nthreads) 
{
   matmat::__mult(a, 
                  A, symmetric_tag(), typename MatA::blocking_tag(), 
                  B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(), 
                  C, typename MatC::shape_tag(), typename MatC::fullness_tag(), typename MatC::orientation_tag(), 
                  cache_size, nthreads);
}

// MULT (A non-symmetric).
template <class T, class MatA, class MatB, class MatC>
inline void
__mult 
   (T a, const MatA& A, non_symmetric_tag, const MatB& B, MatC& C, size_t cache_size, int nthreads) 
{
   matmat::__mult(a, 
                  A,        non_symmetric_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(), 
                  B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(), 
                  C, typename MatC::shape_tag(), typename MatC::fullness_tag(), typename MatC::orientation_tag(), 
                  cache_size, nthreads);
}


// MULT (A symmetric).
template <class T, class MatA, class MatB, class MatC>
inline void
__mult 
   (T a, const MatA& A, symmetric_tag, const MatB& B, MatC& C, size_t cache_size, int nthreads) 
{
   matmat::__mult(a, 
                  A, symmetric_tag(), typename MatA::blocking_tag(), 
                  B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(), 
                  C, typename MatC::shape_tag(), typename MatC::fullness_tag(), typename MatC::orientation_tag(), 
                  cache_size, nthreads);
}


// MULT (small matrices, C non-symmetric).
template <class T, class MatA, class MatB, class MatC>
INLINE void
__mult_small (T a, const MatA& A, const MatB& B, MatC& C,
              cm2::non_symmetric_tag) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename cm2::biggest2_type<TA,TB>::type      TS;

   const size_t         ACOLS = A.cols();
   const size_t         AROWS = A.rows();
   const size_t         BCOLS = B.cols();

   for (size_t j = 0; j < BCOLS; ++j)
   {
      for (size_t i = 0; i < AROWS; ++i)
      {
         TS     s(0);
         for (size_t k = 0; k < ACOLS; ++k)
            s += A(i, k) * B(k, j);
         C(i, j) += a * s;
      }
   }
}

// MULT (small matrices, C symmetric).
template <class T, class MatA, class MatB, class MatC>
INLINE void
__mult_small (T a, const MatA& A, const MatB& B, MatC& C,
              cm2::symmetric_tag) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename cm2::biggest2_type<TA,TB>::type      TS;

   const size_t         ACOLS = A.cols();
   const size_t         BCOLS = B.cols();

   for (size_t j = 0; j < BCOLS; ++j)
   {
      for (size_t i = 0; i <= j; ++i)
      {
         TS    s(0);
         for (size_t k = 0; k < ACOLS; ++k)
            s += A(i, k) * B(k, j);
         C(i, j) += a * s;
      }
   }
}

/*
   Definition of mult (with scaling).
   C += a * A % B
   */
template <class T, class MatA, class MatB, class MatC>
inline void
mult (T a, const MatA& A, const MatB& B, MatC& C, 
      size_t cache_size, int nthreads) 
{
   assert(B.cols() == C.cols());

   // Fast return.
   if ((a == T(0)) || B.empty())
      return;

   assert(A.cols() == B.rows());
   assert(A.rows() == C.rows());

   if (A.rows() * A.cols() * B.cols() <= 32)
      matmat::__mult_small(a, A, B, C, typename MatC::shape_tag());
   else
      matmat::__mult(a, A, typename MatA::shape_tag(), 
                     B, C, cache_size, nthreads);
}




/* TRANSPOSE MULT.
   Version for three full rectangular matrices.
   \note    Uses hierarchic block + cache tiling.
   */
template <class T, class MatA, class MatB, class MatC>
void
__transpose_mult
   (T a, 
    const MatA& A, non_symmetric_tag, full_tag, col_oriented_tag, 
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag, 
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag,
    size_t cache_size, int nthreads) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename MatC::value_type         TC;

   const size_t                                    ROWS  = A.rows();
   const size_t                                    COLS  = B.cols();
   const size_t                                    ACOLS = A.cols();
   const typename MatA::const_segment_iterator     A_0   = A.seg_begin();
   const typename MatB::const_segment_iterator     B_0   = B.seg_begin();
   const typename MatC::segment_iterator           C_0   = C.seg_begin();
   typename MatA::const_segment_iterator           A_I0, A_I1;
   typename MatB::const_segment_iterator           B_J0, B_J1;
   typename MatC::segment_iterator                 C_J0, C_J1;
   const TB* CM2_RESTRICT                          pb;
   TC* CM2_RESTRICT                                pc;
   size_t                                          Ci(ACOLS), Cj(COLS), ci, cj, ni, nj;
   size_t                                          I0, I1, J0, J1;

   assert (A.rows() == B.rows());
   assert (B.cols() == C.cols());

   // Compute the number of columns fitting in cache.
   {
      const size_t   a_  = A.rows() * sizeof(TA);
      const size_t   b_  = B.rows() * sizeof(TB);
      const size_t   c_  = C.rows() * sizeof(TC);
      if ((cache_size > 0) && (cache_size < Ci*a_ + Cj*(b_+c_)))     // Doesn't fit in cache.
      {
         const double r = double(cache_size) / double(Ci*a_ + Cj*(b_+c_));
         Ci = std::max(size_t(r * double(ACOLS)), size_t(1));
         Cj = std::max(size_t(r * double(COLS)), size_t(1));
      }
   }

   // Loop on blocks of B-C.
   for (J0 = 0, B_J0 = B_0, C_J0 = C_0; J0 < COLS; J0 += nj, B_J0 += nj, C_J0 += nj)
   {
      pb = B_J0.begin();                        // Load first cols of B.
      pc = C_J0.begin();                        // Load first cols of C.
      assert (B.last_incore_col() > J0);
      assert (C.last_incore_col() > J0);
      nj = std::min(B.last_incore_col(), C.last_incore_col()) - J0;

      // Loop on blocks of A.
      for (I0 = 0, A_I0 = A_0; I0 < ACOLS; I0 += ni, A_I0 += ni)
      {
         (void)A_I0.begin();                    // Load first cols of A.
         assert (A.last_incore_col() > I0);
         ni = A.last_incore_col() - I0;

         // Loop on cache blocks of B-C.
         for (J1 = J0, B_J1 = B_J0, C_J1 = C_J0; J1 < J0 + nj; J1 += cj, B_J1 += cj, C_J1 += cj)
         {
            cj = std::min(Cj, nj);
            cj = std::min(cj, J0 + nj - J1);

            // Loop on cache blocks of A.
            for (I1 = I0, A_I1 = A_I0; I1 < I0 + ni; I1 += ci, A_I1 += ci)
            {
               ci = std::min(Ci, ni);
               ci = std::min(ci, I0 + ni - I1);

               // Loop inside B-C block.
#ifdef _OPENMP
               if ((nthreads > 1) && (cj > 1) && (ROWS*ci*cj > CM2_OMP_MIN_LOOP))    
               {
                  #pragma omp parallel for private(pb,pc) schedule(static) num_threads(nthreads)       // Parallelization over columns.
                  for (int j = 0; j < int(cj); ++j)
                  {
                     typename MatA::const_segment_iterator     A_i(A_I1);
                     typename MatB::const_segment_iterator     B_j(B_J1);  B_j += j; 
                     typename MatC::segment_iterator           C_j(C_J1);  C_j += j;

                     pb = B_j.begin();  pc = C_j.begin() + I1;
                     for (size_t i = 0; i < ci; ++i, ++pc, ++A_i)
                        *pc += a * vecvec::dot_n(A_i.begin(), pb, ROWS);
                  }
               }
               else                                                        
#endif
               {
                  typename MatA::const_segment_iterator     A_i(A_I1);
                  typename MatB::const_segment_iterator     B_j(B_J1); 
                  typename MatC::segment_iterator           C_j(C_J1); 

                  for (size_t j = 0; j < cj; ++j, ++B_j, ++C_j, A_i = A_I1)
                  {
                     pb = B_j.begin();  pc = C_j.begin() + I1;
                     for (size_t i = 0; i < ci; ++i, ++pc, ++A_i)
                        *pc += a * vecvec::dot_n_par(A_i.begin(), pb, ROWS, nthreads);    // Parallelization inside columns.
                  }
               }
            }
         }
      }
   }
}


/* TRANSPOSE MULT.
   Version for two full rectangular matrices (A and B) and one full symmetric matrix (C).
   \note    Uses hierarchic block + cache tiling.
   */
template <class T, class MatA, class MatB, class MatC>
void
__transpose_mult
   (T a, 
    const MatA& A, non_symmetric_tag, full_tag, col_oriented_tag, 
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag,
          MatC& C,     symmetric_tag, full_tag, col_oriented_tag,
    size_t cache_size, int nthreads) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename MatC::value_type         TC;

   const size_t                                    ROWS  = A.rows();
   const size_t                                    COLS  = A.cols();
   const typename MatA::const_segment_iterator     A_0   = A.seg_begin();
   const typename MatB::const_segment_iterator     B_0   = B.seg_begin();
   const typename MatC::segment_iterator           C_0   = C.seg_begin();
   typename MatA::const_segment_iterator           A_I0, A_I1;
   typename MatB::const_segment_iterator           B_J0, B_J1;
   typename MatC::segment_iterator                 C_J0, C_J1;
   const TB* CM2_RESTRICT                          pb;
   TC* CM2_RESTRICT                                pc;
   size_t                                          Ci(COLS), ci, cj, ni, nj;
   size_t                                          I0, I1, J0, J1;

   assert (A.cols() == B.cols());
   assert (A.cols() == C.dim());

   // Compute the number of columns fitting in cache.
   {
      const size_t   a_  = A.rows() * sizeof(TA);
      const size_t   b_  = B.rows() * sizeof(TB);
      const size_t   c_  = C.dim()  * sizeof(TC);
      if ((cache_size > 0) && (cache_size < Ci*(a_+b_+c_)))          // Doesn't fit in cache.
      {
         const double r = double(cache_size) / double(Ci*(a_+b_+c_));
         Ci = std::max(size_t(r * double(COLS)), size_t(1));
      }
   }

   // Loop on blocks of B-C.
   for (J0 = 0, B_J0 = B_0, C_J0 = C_0; J0 < COLS; J0 += nj, B_J0 += nj, C_J0 += nj)
   {
      pb = B_J0.begin();                        // Load first cols of B.
      assert (B.last_incore_col() > J0);
      nj = B.last_incore_col() - J0;

      // Loop on blocks of A.
      for (I0 = 0, A_I0 = A_0; I0 < COLS; I0 += ni, A_I0 += ni)
      {
         (void)A_I0.begin();                    // Load first cols of A.
         assert (A.last_incore_col() > I0);
         ni = A.last_incore_col() - I0;

         // Loop on cache blocks of B-C.
         for (J1 = J0, B_J1 = B_J0, C_J1 = C_J0; J1 < J0 + nj; J1 += cj, B_J1 += cj, C_J1 += cj)
         {
            cj = std::min(Ci, nj);
            cj = std::min(cj, J0+nj-J1);

            // Loop on cache blocks of A.
            for (I1 = I0, A_I1 = A_I0; I1 < I0 + ni; I1 += ci, A_I1 += ci)
            {
               ci = std::min(Ci, ni);
               ci = std::min(ci, I0+ni-I1);

               // Loop inside B-C block.
#ifdef _OPENMP
               if ((nthreads > 1) && (cj > 1) && (ROWS*ci*cj > CM2_OMP_MIN_LOOP)) 
               {
                  #pragma omp parallel for private(pb,pc) schedule(static) num_threads(nthreads)      // Parallelization over columns.
                  for (int j = 0; j < int(cj); ++j)
                  {
                     typename MatA::const_segment_iterator     A_i(A_I1);
                     typename MatB::const_segment_iterator     B_j(B_J1);  B_j += j; 
                     typename MatC::segment_iterator           C_j(C_J1);  C_j += j;
                     const size_t   I_ = std::min(I1+ci, J1+j+1);

                     pb = B_j.begin();  pc = C_j.begin() + I1;
                     for (size_t i = I1; i < I_; ++i, ++pc, ++A_i)
                        *pc += a * vecvec::dot_n(A_i.begin(), pb, ROWS);
                  }
               }
               else                                                        
#endif
               {
                  typename MatA::const_segment_iterator     A_i(A_I1);
                  typename MatB::const_segment_iterator     B_j(B_J1); 
                  typename MatC::segment_iterator           C_j(C_J1); 

                  for (size_t j = 0; j < cj; ++j, ++B_j, ++C_j, A_i = A_I1)
                  {
                     const size_t   I_ = std::min(I1+ci, J1+j+1);
                     pb = B_j.begin();  pc = C_j.begin() + I1;
                     for (size_t i = I1; i < I_; ++i, ++pc, ++A_i)
                        *pc += a * vecvec::dot_n_par(A_i.begin(), pb, ROWS, nthreads);    // Parallelization inside columns.
                  }
               }
            }
         }
      }
   }
}


// TRANSPOSE MULT (A symmetric, non-blocked).
template <class T, class MatA, class MatB, class MatC>
void
__transpose_mult
   (T a, 
    const MatA& A, symmetric_tag, non_blocked_tag,
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag,
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag,
    size_t /*cache_size*/, int nthreads) 
{
   const size_t                                 COLS  = B.cols();
   typename MatB::const_segment_iterator        b0    = B.seg_begin();
   typename MatC::segment_iterator              c0    = C.seg_begin();
   size_t                                       cols(0);

   for (size_t j0 = 0; j0 < COLS; j0 += cols, b0 += cols, c0 += cols)
   {
      (void)b0.begin();                   // Load first cols of B.
      (void)c0.begin();                   // Load first cols of C.
      assert (std::min(B.last_incore_col(), C.last_incore_col()) > j0);
      cols = std::min(B.last_incore_col(), C.last_incore_col()) - j0;

#ifdef _OPENMP
      if ((nthreads > 1) && (cols > 1) && (2*A.entries()*cols > CM2_OMP_MIN_LOOP))    // Parallel version.
      {
         #pragma omp parallel for schedule(static) num_threads(nthreads)
         for (int j = 0; j < int(cols); ++j)
         {
            typename MatB::const_segment_iterator     b(b0);  b += j; 
            typename MatC::segment_iterator           c(c0);  c += j; 
            matvec::transpose_mult(a, A, b, c);
         }
      }
      else
#endif
      {
         typename MatB::const_segment_iterator     b(b0);
         typename MatC::segment_iterator           c(c0);
         for (size_t j = 0; j < cols; ++j, ++b, ++c)     
            matvec::transpose_mult(a, A, b, c, nthreads);
      }
   }
}

// TRANSPOSE MULT (A symmetric, blocked).
template <class T, class MatA, class MatB, class MatC>
void
__transpose_mult 
   (T a, 
    const MatA& A, symmetric_tag, blocked_tag, 
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag, 
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag, 
    size_t /*cache_size*/, int nthreads) 
{
   const size_t                              COLS  = B.cols();
   typename MatA::const_block_iterator       A0;
   typename MatB::const_segment_iterator     b0    = B.seg_begin();
   typename MatC::segment_iterator           c0    = C.seg_begin();
   size_t                                    blocks(0);
   int                                       cols(0);
   bool                                      parallel_cond;

   for (size_t j0 = 0; j0 < COLS; j0 += cols, b0 += cols, c0 += cols) 
   {
      (void)b0.begin();                   // Load first cols of B.
      (void)c0.begin();                   // Load first cols of C.
      assert (std::min(B.last_incore_col(), C.last_incore_col()) > j0);
      cols = int(std::min(B.last_incore_col(), C.last_incore_col()) - j0);

      for (A0 = A.blocks_begin(); A0 < A.blocks_end(); A0 += blocks) 
      {
         (void)A0->data();                // Load A0 and following blocks.
         blocks = A.end_incore_block_id() - A0.ID();
         assert (blocks > 0);

         parallel_cond = (nthreads > 1) && (cols > 1) && (2*A.incore_entries()*cols > CM2_OMP_MIN_LOOP);
         #pragma omp parallel for if(parallel_cond) schedule(static) num_threads(nthreads)
         for (int j = 0; j < cols; ++j)
         {
            typename MatA::const_block_iterator    Ak(A0);
            typename MatB::const_segment_iterator  b(b0); b += j; 
            typename MatC::segment_iterator        c(c0); c += j; 
            for (size_t k = 0; k < blocks; ++k, ++Ak) 
               matvec::__transpose_mult(a, *Ak, b.begin(), c.begin(), /*nthreads=>*/ 1);
         }
      }
   } 
}
  

// TRANSPOSE MULT.
template <class T, class MatA, class MatB, class MatC>
inline void
__transpose_mult
   (T a, 
    const MatA& A, non_symmetric_tag, non_full_tag, typename MatA::orientation_tag,
    const MatB& B, non_symmetric_tag, full_tag, col_oriented_tag,
          MatC& C, non_symmetric_tag, full_tag, col_oriented_tag,
    size_t cache_size, int nthreads) 
{
   matmat::__transpose_mult(a, 
                            A, symmetric_tag(), typename MatA::blocking_tag(), 
                            B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(), 
                            C, typename MatC::shape_tag(), typename MatC::fullness_tag(), typename MatC::orientation_tag(), 
                            cache_size, nthreads);
}


// TRANSPOSE MULT (A non-symmetric).
template <class T, class MatA, class MatB, class MatC>
inline void
__transpose_mult 
   (T a, const MatA& A, non_symmetric_tag, const MatB& B, MatC& C, size_t cache_size, int nthreads) 
{
   matmat::__transpose_mult(a, 
                            A,        non_symmetric_tag(), typename MatA::fullness_tag(), typename MatA::orientation_tag(), 
                            B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(), 
                            C, typename MatC::shape_tag(), typename MatC::fullness_tag(), typename MatC::orientation_tag(), 
                            cache_size, nthreads);
}


// TRANSPOSE MULT (A symmetric).
template <class T, class MatA, class MatB, class MatC>
inline void
__transpose_mult 
   (T a, const MatA& A, symmetric_tag, const MatB& B, MatC& C, size_t cache_size, int nthreads) 
{
   matmat::__transpose_mult(a, 
                            A, symmetric_tag(), typename MatA::blocking_tag(), 
                            B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(),               
                            C, typename MatC::shape_tag(), typename MatC::fullness_tag(), typename MatC::orientation_tag(), 
                            cache_size, nthreads);
}


// TRANSPOSE MULT (small matrices, C non-symmetric).
template <class T, class MatA, class MatB, class MatC>
INLINE void
__transpose_mult_small (T a, const MatA& A, const MatB& B, MatC& C,
                        cm2::non_symmetric_tag) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename cm2::biggest2_type<TA,TB>::type      TS;

   const size_t         ACOLS = A.cols();
   const size_t         AROWS = A.rows();
   const size_t         BCOLS = B.cols();

   for (size_t j = 0; j < BCOLS; ++j)
   {
      for (size_t i = 0; i < ACOLS; ++i)
      {
         TS    s(0);
         for (size_t k = 0; k < AROWS; ++k)
            s += A(k, i) * B(k, j);
         C(i, j) += a * s;
      }
   }
}

// TRANSPOSE MULT (small matrices, C symmetric).
template <class T, class MatA, class MatB, class MatC>
INLINE void
__transpose_mult_small (T a, const MatA& A, const MatB& B, MatC& C,
                        cm2::symmetric_tag) 
{
   typedef typename MatA::value_type         TA;
   typedef typename MatB::value_type         TB;
   typedef typename cm2::biggest2_type<TA,TB>::type      TS;

   const size_t         AROWS = A.rows();
   const size_t         BCOLS = B.cols();

   for (size_t j = 0; j < BCOLS; ++j)
   {
      for (size_t i = 0; i <= j; ++i)
      {
         TS    s(0);
         for (size_t k = 0; k < AROWS; ++k)
            s += A(k, i) * B(k, j);
         C(i, j) += a * s;
      }
   }
}

// Definition of transpose_mult (with scaling).
template <class T, class MatA, class MatB, class MatC>
inline void
transpose_mult 
   (T a, const MatA& A, const MatB& B, MatC& C, 
    size_t cache_size, int nthreads) 
{
   assert(B.cols() == C.cols());

   // Fast return.
   if ((a == T(0)) || B.empty())
      return;

   assert(A.rows() == B.rows());
   assert(A.cols() == C.rows());

   if (A.rows() * A.cols() * B.cols() <= 32)
      matmat::__transpose_mult_small(a, A, B, C, typename MatC::shape_tag());
   else
      matmat::__transpose_mult(a, A, typename MatA::shape_tag(), 
                               B, C, cache_size, nthreads);
}



/* DIAG MULT.
   Version for a diagonal matrix and two full rectangular matrices.

   C += a * D % B

   \note    Uses hierarchic block + cache tiling.
   */
template <class T, class DiagD, class MatB, class MatC>
void
__diag_mult
   (T a, const DiagD& D,
    const MatB& B, cm2::non_symmetric_tag, cm2::full_tag, cm2::col_oriented_tag, 
          MatC& C, cm2::non_symmetric_tag, cm2::full_tag, cm2::col_oriented_tag, 
    int nthreads) 
{
   typedef typename MatB::value_type         TB;
   typedef typename MatC::value_type         TC;

   const size_t                                    DIM   = D.size();
   const size_t                                    COLS  = B.cols();
   const typename MatB::const_segment_iterator     B_0   = B.seg_begin();
   const typename MatC::segment_iterator           C_0   = C.seg_begin();
   typename MatB::const_segment_iterator           B_J0, B_J1;
   typename MatC::segment_iterator                 C_J0, C_J1;
   const TB* CM2_RESTRICT                          pb;
   TC* CM2_RESTRICT                                pc;
   size_t                                          J0, J1, nj;

   // Loop on blocks of B-C.
   for (J0 = 0, B_J0 = B_0, C_J0 = C_0; J0 < COLS; J0 += nj, B_J0 += nj, C_J0 += nj)
   {
      pb = B_J0.begin();                  // Load first cols of B.
      pc = C_J0.begin();                  // Load first cols of C.
      assert (B.last_incore_col() > J0);
      assert (C.last_incore_col() > J0);
      nj = std::min(B.last_incore_col(), C.last_incore_col()) - J0;

      // Loop on columns of B-C.
      for (J1 = J0, B_J1 = B_J0, C_J1 = C_J0; J1 < J0 + nj; J1 += nj, B_J1 += nj, C_J1 += nj)
      {
#ifdef _OPENMP
         if ((nthreads > 1) && (nj > 1) && (DIM * nj > CM2_OMP_MIN_LOOP))            
         {
            #pragma omp parallel for private(pb,pc) schedule(static) num_threads(nthreads)         // Parallelization over columns.
            for (int j = 0; j < int(nj); ++j)
            {
               typename MatB::const_segment_iterator     B_j(B_J1);  B_j += j; 
               typename MatC::segment_iterator           C_j(C_J1);  C_j += j;

               pb = B_j.begin();  pc = C_j.begin();
               vecvec::axypz_n(a, D.begin(), pb, pc, DIM); 
            }
         }
         else
#endif
         {
            typename MatB::const_segment_iterator     B_j(B_J1); 
            typename MatC::segment_iterator           C_j(C_J1); 

            for (size_t j = 0; j < nj; ++j, ++B_j, ++C_j) 
            {
               pb = B_j.begin();  pc = C_j.begin();
               vecvec::axypz_n_par(a, D.begin(), pb, pc, DIM, nthreads);                      // Parallelization inside columns.
            }
         }
      }
   }
}


/* DIAG MULT.
   Version for a diagonal matrix and one full rectangular matrix.

   B = D % B

   \note    Uses hierarchic block + cache tiling.
   */
template <class DiagD, class MatB>
void
__diag_mult
   (const DiagD& D, MatB& B, cm2::non_symmetric_tag, cm2::full_tag, cm2::col_oriented_tag, int nthreads) 
{
   typedef typename MatB::value_type         TB;

   const size_t                                    DIM   = D.size();
   const size_t                                    COLS  = B.cols();
   const typename MatB::segment_iterator           B_0   = B.seg_begin();
   typename MatB::segment_iterator                 B_J0, B_J1;
   TB* CM2_RESTRICT                                pb;
   size_t                                          J0, J1, nj;

   // Loop on blocks of B.
   for (J0 = 0, B_J0 = B_0; J0 < COLS; J0 += nj, B_J0 += nj)
   {
      pb = B_J0.begin();                  // Load first cols of B.
      assert (B.last_incore_col() > J0);
      nj = B.last_incore_col() - J0;

      // Loop on columns of B.
      for (J1 = J0, B_J1 = B_J0; J1 < J0 + nj; J1 += nj, B_J1 += nj)
      {
         // Loop inside B block.
#ifdef _OPENMP
         if ((nthreads > 1) && (nj > 1) && (DIM * nj > CM2_OMP_MIN_LOOP))            
         {
            #pragma omp parallel for private(pb) schedule(static) num_threads(nthreads)   // Parallelization over columns.
            for (int j = 0; j < int(nj); ++j)
            {
               typename MatB::segment_iterator     B_j(B_J1);  B_j += j; 
               pb = B_j.begin();  
               vecvec::mult_n(D.begin(), pb, DIM); 
            }
         }
         else
#endif
         {
            typename MatB::segment_iterator     B_j(B_J1); 

            for (size_t j = 0; j < nj; ++j, ++B_j) 
            {
               pb = B_j.begin();  
               vecvec::mult_n_par(D.begin(), pb, DIM, nthreads);                   // Parallelization inside columns.
            }
         }
      }
   }
}


// DIAG MULT (small matrices).
template <class T, class DiagD, class MatB, class MatC>
INLINE void
__diag_mult_small (T a, const DiagD& D, const MatB& B, MatC& C) 
{
   const size_t         ROWS  = C.rows();
   const size_t         COLS  = C.cols();

   for (size_t j = 0; j < COLS; ++j)
      for (size_t i = 0; i < ROWS; ++i)
         C(i, j) += a * (D[i] * B(i, j));
}

template <class DiagD, class MatB>
INLINE void
__diag_mult_small (const DiagD& D, MatB& B) 
{
   const size_t         ROWS  = B.rows();
   const size_t         COLS  = B.cols();

   for (size_t j = 0; j < COLS; ++j)
      for (size_t i = 0; i < ROWS; ++i)
         B(i, j) *= D[i];
}


/*
   Definition of diag-mat mult (with scaling).
   C += a * D % B
   */
template <class T, class DiagD, class MatB, class MatC>
inline void
diag_mult (T a, const DiagD& D, MatB& B, MatC& C, int nthreads) 
{
   assert(B.rows() == C.rows());
   assert(B.cols() == C.cols());

   // Fast return.
   if ((a == T(0)) || B.empty())
      return;

   assert(D.size() == B.rows());

   if (C.rows() * C.cols() <= 32)
      matmat::__diag_mult_small(a, D, B, C);
   else
      matmat::__diag_mult(a, D, 
                          B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(), 
                          C, typename MatC::shape_tag(), typename MatC::fullness_tag(), typename MatC::orientation_tag(), 
                          nthreads);
}

/*
   Definition of diag-mat mult (without scaling).
   B = D % B
   */
template <class DiagD, class MatB>
inline void
diag_mult (const DiagD& D, MatB& B, int nthreads) 
{
   // Fast return.
   if (D.empty() || B.empty())
      return;

   assert(D.size() == B.rows());

   if (B.rows() * B.cols() <= 32)
      matmat::__diag_mult_small(D, B);
   else
      matmat::__diag_mult(D, 
                          B, typename MatB::shape_tag(), typename MatB::fullness_tag(), typename MatB::orientation_tag(), 
                          nthreads);
}

#endif // DOXYGEN_SKIP



#ifndef DOXYGEN_SKIP

// 
template <class MatB, class MatC>
struct __Bt_A_B_op 
{
template <class T, class MatA>
void operator() (T a, const MatA& A, const MatB& B, MatC& C,
                 size_t cache_size = 0, int nthreads = 1) const
{
   typedef typename MatB::value_type   TB;

   MatB     AB(A.rows(), B.cols(), TB(0));
   matmat::mult(a, A, B, AB, cache_size, nthreads);
   matmat::transpose_mult(B, AB, C, cache_size, nthreads);
}

// Specialization for MatA = double
inline void operator() (double a, double A, const MatB& B, MatC& C,
                        size_t cache_size = 0, int nthreads = 1) const
{
   if (B.has_contiguous_storage())
      matmat::transpose_mult(a*A, B, B, C, cache_size, nthreads);     // Crashes if B is OOC.
   else
   {
      MatB     B_(B.rows(), B.cols());
      matmat::copy(B, B_);
      matmat::transpose_mult(a*A, B_, B, C, cache_size, nthreads);
   }
}
};

#endif

/**
   transpose(B) % A % B

   \verbatim     
      C += a * transpose(B) % A % B 
   \endverbatim

   \pre A.rows() == A.cols() == B.rows()
   \pre C.rows() == C.cols() == B.cols()
   \pre B is either a dense2D or a matrix_fixed matrix. There is no specific shape condition for A and C (they can be symmetric or not).
   \warning To get C = Bt_A_B , don't forget to set \p C to zero prior to the multiplication.
   */
template <class T, class MatA, class MatB, class MatC>
inline void
Bt_A_B (T a, const MatA& A, const MatB& B, MatC& C,
        size_t cache_size = 0, int nthreads = 1)
{
   // Fast return.
   if (a == T(0)) 
      return;

   matmat::__Bt_A_B_op<MatB, MatC>()(a, A, B, C, cache_size, nthreads);
}


/**
   B % A % transpose(B)

   \verbatim     
      C += a * B % A % transpose(B)
   \endverbatim

   \pre A.rows() == A.cols() == B.cols()
   \pre C.rows() == C.cols() == B.rows()
   \pre B is either a dense2D or a matrix_fixed matrix. There is no specific shape condition for A and C (they can be symmetric or not).
   \warning To get C = B_A_Bt , don't forget to set \p C to zero prior to the multiplication.
   */
template <class T, class MatA, class MatB, class MatC>
void
B_A_Bt (T a, const MatA& A, const MatB& B, MatC& C,
        size_t cache_size = 0, int nthreads = 1)
{
   // Fast return.
   if (a == T(0)) 
      return;

   typename MatB::transpose_type     Bt(B.cols(), B.rows(), T(0));
   matmat::transpose(B, Bt);
   matmat::Bt_A_B(a, A, Bt, C, cache_size, nthreads);
}


#ifndef DOXYGEN_SKIP

template <class T, class MatA, class MatB>
void
__Tinverse (const MatA& A, MatB& B, T& det, 
            symmetric_tag, symmetric_tag, cm2::fixed_size_tag<2>, cm2::fixed_size_tag<2>) 
{
   det = matscal::det2(A);
   if (det == 0.0)
      return;
   const T   det_inv = 1. / det;
   B(0,0) = A(1,1) * det_inv;
   B(1,1) = A(0,0) * det_inv;
   B(0,1) = -A(1,0) * det_inv;
}

template <class T, class MatA, class MatB>
void
__Tinverse (const MatA& A, MatB& B, T& det, 
            typename MatA::shape_tag, non_symmetric_tag, cm2::fixed_size_tag<2>, cm2::fixed_size_tag<2>) 
{
   det = matscal::det2(A);
   if (det == 0.0)
      return;
   const T   det_inv = 1. / det;
   B(0,0) = A(1,1) * det_inv;
   B(1,1) = A(0,0) * det_inv;
   B(0,1) = -A(1,0) * det_inv;
   B(1,0) = -A(0,1) * det_inv;
}

template <class T, class MatA, class MatB>
void
__Tinverse (const MatA& A, MatB& B, T& det, 
            non_symmetric_tag, non_symmetric_tag, cm2::fixed_size_tag<3>, cm2::fixed_size_tag<2>) 
{
   const typename MatA::const_iterator    a0    = A.data_col(0);
   const typename MatA::const_iterator    a1    = A.data_col(1);
   typename MatB::iterator                b0    = B.data_col(0);
   typename MatB::iterator                b1    = B.data_col(1);
   cm2::vector_fixed<T,3>                 D;

   vecvec::copy_cross(a0, a1, D);
   det = vecscal::sqr_two_norm(D);
   if (det == 0.0)
      return;
   vecscal::mult(1./det, D);
   det = ::sqrt(det);
   vecvec::copy_cross(a1, D, b0);
   vecvec::copy_cross(D, a0, b1);
}

template <class MatA, class MatB, class T>
void
__Tinverse (const MatA& A, MatB& B, T& det, 
            non_symmetric_tag, non_symmetric_tag, cm2::fixed_size_tag<3>, cm2::fixed_size_tag<3>) 
{
   const typename MatA::const_iterator    a0    = A.data_col(0);
   const typename MatA::const_iterator    a1    = A.data_col(1);
   const typename MatA::const_iterator    a2    = A.data_col(2);
   typename MatB::iterator                b0    = B.data_col(0);
   typename MatB::iterator                b1    = B.data_col(1);
   typename MatB::iterator                b2    = B.data_col(2);

   vecvec::copy_cross (a1, a2, b0);
   vecvec::copy_cross (a2, a0, b1);
   vecvec::copy_cross (a0, a1, b2);
   det = vecvec::dot3 (a0, b0);
   if (det != 0.0)
      vecscal::mult_n (1./det, b0, 9);
}

//
template <class T, class MatA, class MatB>
inline void
__Tinverse (const MatA& A, MatB& B, T& det, 
            typename MatA::shape_tag, typename MatB::shape_tag, cm2::dynamic_size_tag, cm2::dynamic_size_tag) 
{
   if ((A.rows() == 2) && (A.cols() == 2))
      matmat::__Tinverse (A, B, det, typename MatA::shape_tag(), typename MatB::shape_tag(), 
                          cm2::fixed_size_tag<2>(), cm2::fixed_size_tag<2>());
   else if ((A.rows() == 3) && (A.cols() == 2))
      matmat::__Tinverse (A, B, det, typename MatA::shape_tag(), typename MatB::shape_tag(), 
                          cm2::fixed_size_tag<3>(), cm2::fixed_size_tag<2>());
   else if ((A.rows() == 3) && (A.cols() == 3))
      matmat::__Tinverse (A, B, det, typename MatA::shape_tag(), typename MatB::shape_tag(), 
                          cm2::fixed_size_tag<3>(), cm2::fixed_size_tag<3>());
}

#endif // DOXYGEN_SKIP



/**
   Generalized transposed inverse of a MxN matrix, with (M,N) = (2,2), (3,2) or (3,3).

   Computes the (pseudo) inverse together with the (pseudo) determinant.
   The definition of the pseudo inverse and pseudo determinant the depends on the dimensions of the matrix :
      - M = N = 2    pseudo inverse = inverse.
                     pseudo det = det.
      - M = N = 3    pseudo inverse = inverse.
                     pseudo det = det.
      - M = 3, N = 2 pseudo inverse = contravariant matrix
                     pseudo det = norm2 of the cross product of the two columns.

   \verbatim     
                      -1
      B = transpose (A  )
   \endverbatim     

   \pre  Matrix \p A must be of fixed dimensions 2x2, 3x2 or 3x3.  
   \post A % transpose(B) = Id(M)
   \post transpose(A) % B = Id(N)
   */
template <class Matrix, class T>
inline void
transpose_inverse (const Matrix& A, Matrix& invT_A, T& det) 
{
   typedef typename Matrix::shape_tag     mat_shape_tag;
   typedef typename Matrix::row_size_tag  mat_row_size_tag;
   typedef typename Matrix::col_size_tag  mat_col_size_tag;

   assert (A.rows() == invT_A.rows());
   assert (A.cols() == invT_A.cols());
   matmat::__Tinverse (A, invT_A, det, 
                       mat_shape_tag(), mat_shape_tag(), mat_col_size_tag(), mat_row_size_tag());
}



template <class T>
inline bool
__Cholesky_factor (T& a, T& b, T& c)
{
   T              t;
   bool           ok;

   if ((a == 0.) && (b == 0.))
   {
      ok = (c >= 0.0);
      if (ok)
         c = ::sqrt(c);
      return ok;
   }

   assert (std::isfinite(a) && std::isfinite(b) && std::isfinite(c));
   ok = (a > 0.);
   if (ok)
   {
      c = c*a - b*b;
      ok = (c >= 0.);
      if (ok)
      {
         a = ::sqrt(a);
         c = ::sqrt(c);
         t = 1. / a;
         b *= t;
         c *= t;
      }
   }

   return ok;
}

template <class T>
inline bool
__Cholesky_factor (T& a, T& b, T& c, T& d, T& e, T& f)
{
   T              t;
   bool           ok;

   if ((a == 0.) && (b == 0.) && (d == 0.))
      return __Cholesky_factor (c, e, f);
   if ((b == 0.) && (c == 0.) && (e == 0.))
      return __Cholesky_factor (a, d, f);

   assert (std::isfinite(a) && std::isfinite(b) && std::isfinite(c));
   assert (std::isfinite(d) && std::isfinite(e) && std::isfinite(f));
   ok = (a > 0.);
   if (ok)
   {
      c = c*a - b*b;
      e = e*a - b*d;
      ok = (c > 0.);
      if (ok)
      {
         a = ::sqrt(a);
         c = ::sqrt(c);
         e /= c;
         t = 1. / a;
         b *= t;
         c *= t;
         d *= t;
         e *= t;
         f -= d*d + e*e;
         ok = (f >= 0.);
         if (ok)
            f = ::sqrt(f);
      }
   }

   return ok;
}

template <class MatrixA, class Factor>
inline bool
__Cholesky_factor (const MatrixA& A, Factor& L, cm2::symmetric_tag, cm2::fixed_size_tag<2>)
{
   typedef typename Factor::value_type      T;
   T&    a(L(0,0));
   T&    b(L(1,0));
   T&    c(L(1,1));
   a = A(0,0);
   b = A(1,0); c = A(1,1);
   return __Cholesky_factor (a, b, c);
}

template <class MatrixA, class Factor>
inline bool
__Cholesky_factor (const MatrixA& A, Factor& L, cm2::symmetric_tag, cm2::fixed_size_tag<3>)
{
   typedef typename Factor::value_type      T;
   T&    a(L(0,0));
   T&    b(L(1,0));
   T&    c(L(1,1));
   T&    d(L(2,0));
   T&    e(L(2,1));
   T&    f(L(2,2));
   a = A(0,0);
   b = A(1,0); c = A(1,1);
   d = A(2,0); e = A(2,1); f = A(2,2);
   return __Cholesky_factor (a, b, c, d, e, f);
}

/**
   Compute the L Cholesky factorization of a small symmetric real matrix.

   A = L % Lt

   \pre  Matrix \p A must be symmetric real of size 2x2 or 3x3.  
   */
template <class MatrixA, class Factor>
inline bool
Cholesky_factor (const MatrixA& A, Factor& L)
{
   typedef typename MatrixA::shape_tag     matA_shape_tag;
   return matmat::__Cholesky_factor (A, L, matA_shape_tag(), cm2::fixed_size_tag<MatrixA::DIM>());
}

//@}

} // end namespace matmat

} // end namespace cm2


#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (pop)
#endif

#endif

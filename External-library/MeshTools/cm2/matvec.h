/**
   \file       matvec.h
   \brief      Definition of matrix-vector template functions.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3608 $
   $Date: 2019-04-25 09:58:41 +0200 (jeu., 25 avr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATRIX_VECTOR_OPS_H__
#define __CM2_MATRIX_VECTOR_OPS_H__


#include "matvec_tags.h"
#include "math1_aux.h"
#include "matscal.h"
#include "symmetric_fixed.h"


#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (push)
#pragma warning (disable : 4068)             // Disable unrecognized OMP pragma.
#endif


/**
   \namespace  cm2::matvec
   \brief      Namespace for Matrix-Vector template functions.
   */

namespace cm2 {
namespace matvec {


/** \defgroup MatVecGroup Matrix-Vectors Template Functions */
//@{

/**
   Appends a column to a matrix.

   \return  Success flag.
   \pre     If the matrix is not empty upon entry, 
            the size of vector \p V must be equal to the number of rows in the matrix (or \p V empty). \n
            If the matrix is empty upon entry, the matrix is resized to `V.size()` rows and one column (if \p V not empty).
   */
template <class Matrix, class Vector>
bool 
push_back (const Vector& V, Matrix& A)  
{ 
   const size_t      R(V.size());
   const size_t      C(A.cols());
   bool              ok(A.empty() || (V.size() == A.rows()));

   if (ok && (R > 0))
   {
      if (C == A.cols_capacity())
         A.reserve(R, std::max(size_t(4),2*C));         // Double the capacity of the matrix.

      ok = A.resize(R, C+1);

      if (ok)
         vecvec::copy_n(V.begin(), A.data_col(C), R);
   }

   return ok;
}   

/**
   Copies a vector to the diagonal of a matrix.
   Uses the min of V.size(), A.cols(), A.rows().

   \verbatim     
      A(i,i) = V[i]  
   \endverbatim
   */
template <class MatX, class Vec>
void
copy_diag (const Vec& V, MatX& A) 
{
   const size_t    ia = std::min(size_t(A.rows()), size_t(A.cols()));
   size_t          i  = std::min(ia, size_t(V.size()));

   if (i) do {
      --i;
      A(i,i) = V[i];  
   } while (i);
}    

/**
   Adds a vector to the diagonal of a matrix.
   Uses the min of V.size(), A.cols(), A.rows().

   \verbatim     
      A(i,i) += V[i]  
   \endverbatim
   */
template <class MatX, class Vec>
void
add_diag (const Vec& V, MatX& A) 
{
   const size_t    ia = std::min(size_t(A.rows()), size_t(A.cols()));
   size_t          i  = std::min(ia, size_t(V.size()));

   if (i) do {
      --i;
      A(i,i) += V[i];  
   } while (i);
}    

/**
   Subtracts a vector to the diagonal of a matrix.
   Uses the min of V.size(), A.cols(), A.rows().

   \verbatim     
      A(i,i) -= V[i]  
   \endverbatim 
   */
template <class MatX, class Vec>
void
subtract_diag (const Vec& V, MatX& A) 
{
   const size_t    ia = std::min(size_t(A.rows()), size_t(A.cols()));
   size_t          i  = std::min(ia, size_t(V.size()));

   if (i) do {
      --i;
      A(i,i) -= V[i];  
   } while (i);
}    

/**
   Multiplies the diagonal of a matrix by a vector (element by element).
   Uses the min of V.size(), A.cols(), A.rows().

   \verbatim     
      A(i,i) *= V[i]  
   \endverbatim
   */
template <class MatX, class Vec>
void
mult_diag (const Vec& V, MatX& A) 
{
   const size_t    ia = std::min(size_t(A.rows()), size_t(A.cols()));
   size_t          i  = std::min(ia, size_t(V.size()));

   if (i) do {
      --i;
      A(i,i) *= V[i];  
   } while (i);
}    

/**
   Divides the diagonal of a matrix by a vector (element by element).
   Uses the min of V.size(), A.cols(), A.rows().

   \verbatim     
      A(i,i) /= V[i]  
   \endverbatim
   \pre V doesn't contain any null component.
   */
template <class MatX, class Vec>
void
div_diag (const Vec& V, MatX& A) 
{
   const size_t    ia = std::min(size_t(A.rows()), size_t(A.cols()));
   size_t          i  = std::min(ia, size_t(V.size()));

   if (i) do {
      --i;
      A(i,i) /= V[i];  
   } while (i);
}    

/**
   Copies a vector to a row of a matrix.
   Uses the min of V.size() and A.cols().

   \verbatim     
      A(i,j) = V[j]  
   \endverbatim
   */
template <class Matrix, class Vector>
INLINE void
copy_row (const Matrix& A, size_t i, Vector& V) 
{
   const size_t    N  = std::min(size_t(V.size()), size_t(A.cols()));
   vecvec::copy_n(A.row_begin(i), V.begin(), N);
}

/**
   Copies a row of a matrix to a vector.
   Uses the min of V.size() and A.cols().

   \verbatim     
      V[j] = A(i,j)  
   \endverbatim
   */
template <class Matrix, class Vector>
INLINE void
copy_row (const Vector& V, Matrix& A, size_t i) 
{
   const size_t    N  = std::min(size_t(V.size()), size_t(A.cols()));
   vecvec::copy_n(V.begin(), A.row_begin(i), N);
}

/**
   Adds a vector to a row of a matrix.
   Uses the min of V.size() and A.cols().

   \verbatim     
      A(i,j) += V[j]  
   \endverbatim
   */
template <class Matrix, class Vector>
INLINE void
add_row (const Vector& V, Matrix& A, size_t i) 
{
   const size_t    N  = std::min(size_t(V.size()), size_t(A.cols()));
   vecvec::add_n(V.begin(), A.row_begin(i), N);
}

/**
   Adds a row of a matrix to a vector.
   Uses the min of V.size() and A.cols().

   \verbatim     
      V[j] += A(i,j)  
   \endverbatim 
   */
template <class Matrix, class Vector>
INLINE void
add_row (const Matrix& A, size_t i, Vector& V) 
{
   const size_t    N  = std::min(size_t(V.size()), size_t(A.cols()));
   vecvec::add_n(A.row_begin(i), V.begin(), N);
}

/**
   AXPY between a row of a matrix and a vector.
   Uses the min of V.size() and A.cols().

   \verbatim     
      A(i,j) += a * V[j]  
   \endverbatim
   */
template <class T, class Matrix, class Vector>
INLINE void
axpy_row (T a, const Vector& V, Matrix& A, size_t i) 
{
   const size_t    N  = std::min(size_t(V.size()), size_t(A.cols()));
   vecvec::axpy_n(a, V.begin(), A.row_begin(i), N);
}

/**
   AXPY between a row of a matrix and a vector.
   Uses the min of V.size() and A.cols().

   \verbatim     
      V[j] += a * A(i,j)  
   \endverbatim
   */
template <class T, class Matrix, class Vector>
INLINE void
axpy_row (T a, const Matrix& A, size_t i, Vector& V) 
{
   const size_t    N  = std::min(size_t(V.size()), size_t(A.cols()));
   vecvec::axpy_n(a, A.row_begin(i), V.begin(), N);
}


#ifndef DOXYGEN_SKIP

// MULT //
// Non-symmetric matrix (column-oriented, non-blocked).
template <class T, class Matrix, class IterX, class IterY>
void
__mult (T scale, const Matrix& A, 
        cm2::non_symmetric_tag, cm2::col_oriented_tag, cm2::non_blocked_tag,
        IterX x, IterY y, int /*nthreads*/) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j;

   // AXPY algorithm.
   for (A_j = A.seg_begin(); A_j < Aend; ++A_j)
   {
      const size_t j(A_j.index()), ibeg(A_j.index_begin()), hj(A_j.size());
      vecvec::axpy_n(*(x+j) * scale, A_j.begin(), y+ibeg, hj);
   }
}

// Non-symmetric matrix (row-oriented, non-blocked).
template <class T, class Matrix, class IterX, class IterY>
void
__mult (T scale, const Matrix& A, 
        cm2::non_symmetric_tag, cm2::row_oriented_tag, cm2::non_blocked_tag,
        IterX x, IterY y, int DUM_OMP(nthreads)) 
{
   const size_t                  NI(A.segments());
   const typename Matrix::const_segment_iterator   A_0(A.seg_begin());
   typename Matrix::const_segment_iterator         A_i;
#ifdef _OPENMP
   const bool  parallel_cond = (nthreads > 1) && (NI > 1) && (A.entries() > CM2_OMP_MIN_LOOP);
#endif

   // Dot product algorithm.
   #pragma omp parallel for if(parallel_cond) private(A_i) schedule(static) num_threads(nthreads)
   for (int k = 0; k < int(NI); ++k)      // Parallel loop over A-segments.
   {
      A_i = A_0;  A_i += k;
      const size_t i(A_i.index()), jbeg(A_i.index_begin()), hi(A_i.size());
      *(y+i) += scale * vecvec::dot_n(A_i.begin(), x+jbeg, hi);
   }
}

// Symmetric matrix (any orientation, non-blocked).
template <class T, class Matrix, class MatrixOrient, class IterX, class IterY>
void
__mult (T scale, const Matrix& A, 
        cm2::symmetric_tag, MatrixOrient, cm2::non_blocked_tag,
        IterX x, IterY y, int /*nthreads*/) 
{
   const typename Matrix::const_segment_iterator   Aend(A.seg_end());
   typename Matrix::const_segment_iterator         A_j;

   for (A_j = A.seg_begin(); A_j < Aend; ++A_j)
   {
      const size_t j(A_j.index()), ibeg(A_j.index_begin()), hj(A_j.size());
      *(y+j) += scale * vecvec::dot_n(A_j.begin(), x+ibeg, hj);
      vecvec::axpy_n(*(x+j) * scale, A_j.begin(), y+ibeg, std::min(hj,j-ibeg));    // min(hj,j-ibeg) to avoid diagonal processed twice.
   }
}

///
template <class T, class Matrix, class IterX, class IterY> 
INLINE void
__mult (T scale, const Matrix& A, IterX x, IterY y, int nthreads);


// Blocked matrix.
#ifdef _OPENMP

template <class T, class BlockIterator, class IterX, class IterY>
INLINE void
__mult_omp (T scale, BlockIterator A0, int nbr_blocks, 
            cm2::non_symmetric_tag, cm2::upper_right_storage_tag,
            cm2::separate_diagonal_tag /*BlockDiagonalTag*/,
            IterX x, IterY y, int nthreads) 
{
   typedef typename BlockIterator::value_type                  block_type;
   typedef typename block_type::storage_tag                    storage_tag;
   typedef typename block_type::orientation_tag                orientation_tag;
   typedef typename block_type::blocking_tag                   block_blocking_tag;
   typedef typename cm2::orientation_as_upper<storage_tag, orientation_tag>::tag  ur_orientation_tag;

   BlockIterator     Ak;
   
   #pragma omp parallel for private(Ak) schedule(static) num_threads(nthreads)
   for (int k = 0; k < nbr_blocks; ++k)      // Parallel loop over in-core blocks.
   {
      Ak = A0;  Ak += k;
      const size_t         i0(Ak->first_index());
      const size_t         n(Ak->dim());
      
      if (Ak->segments_entries() > 0)
         matvec::__mult(scale, *Ak, 
                        cm2::non_symmetric_tag(), ur_orientation_tag(), block_blocking_tag(),
                        x, y, /*nthreads=>*/ 1);
                     
      if (Ak->is_unit_diagonal()) 
         vecvec::axpy_n(scale, x+i0, y+i0, n);
      else
         vecvec::axypz_n(scale, Ak->diagonal_begin(), x+i0, y+i0, n);
   }
}

template <class T, class BlockIterator, class IterX, class IterY>
INLINE void
__mult_omp (T scale, BlockIterator A0, int nbr_blocks, 
            cm2::non_symmetric_tag, cm2::lower_left_storage_tag,
            cm2::separate_diagonal_tag /*BlockDiagonalTag*/,
            IterX x, IterY y, int nthreads) 
{
   typedef typename BlockIterator::value_type                  block_type;
   typedef typename block_type::storage_tag                    storage_tag;
   typedef typename block_type::orientation_tag                orientation_tag;
   typedef typename block_type::blocking_tag                   block_blocking_tag;
   typedef typename cm2::orientation_as_lower<storage_tag, orientation_tag>::tag  ll_orientation_tag;

   for (int k = 0; k < nbr_blocks; ++k, ++A0)      
   {
      const size_t         i0(A0->first_index());
      const size_t         n(A0->dim());
      
      if (A0->segments_entries() > 0)
         matvec::__mult(scale, *A0, 
                        cm2::non_symmetric_tag(), ll_orientation_tag(), block_blocking_tag(),
                        x, y, nthreads);        // Parallel loop inside each block.
                     
      if (A0->is_unit_diagonal()) 
         vecvec::axpy_n_par(scale, x+i0, y+i0, n, nthreads);
      else
         vecvec::axypz_n_par(scale, A0->diagonal_begin(), x+i0, y+i0, n, nthreads);
   }
}

template <class T, class BlockIterator, class IterX, class IterY>
INLINE void
__mult_omp (T scale, BlockIterator A0, int nbr_blocks, 
            cm2::non_symmetric_tag, cm2::separate_diagonal_tag /*BlockDiagonalTag*/,
            IterX x, IterY y, int nthreads) 
{
   typedef typename BlockIterator::value_type               block_type;
   typedef typename block_type::storage_tag                 storage_tag;

   matvec::__mult_omp(scale, A0, nbr_blocks, 
                      cm2::non_symmetric_tag(), storage_tag(),
                      cm2::separate_diagonal_tag(),
                      x, y, nthreads);
}

template <class T, class BlockIterator, class IterX, class IterY>
INLINE void
__mult_omp (T scale, BlockIterator A0, int nbr_blocks, 
            cm2::symmetric_tag, cm2::separate_diagonal_tag /*BlockDiagonalTag*/,
            IterX x, IterY y, int nthreads) 
{
   typedef typename BlockIterator::value_type                  block_type;
   typedef typename block_type::storage_tag                    storage_tag;
   typedef typename block_type::orientation_tag                orientation_tag;
   typedef typename block_type::blocking_tag                   block_blocking_tag;
   typedef typename cm2::orientation_as_lower<storage_tag, orientation_tag>::tag  ll_orientation_tag;

   matvec::__mult_omp(scale, A0, nbr_blocks, 
                      cm2::non_symmetric_tag(), cm2::upper_right_storage_tag(), cm2::separate_diagonal_tag(),
                      x, y, nthreads);

   for (int k = 0; k < nbr_blocks; ++k, ++A0)         
      matvec::__mult(scale, *A0, 
                     cm2::non_symmetric_tag(), ll_orientation_tag(), block_blocking_tag(),
                     x, y, nthreads);  
}

#endif         // _OPENMP


// Blocked matrix (any shape, any orientation).
template <class T, class Matrix, class MatrixShape, class MatrixOrient, class IterX, class IterY>
INLINE void
__mult (T scale, const Matrix& A,
        MatrixShape, MatrixOrient, cm2::blocked_tag,
        IterX x, IterY y, int nthreads)
{
   const size_t      NB(A.blocks());
   typename Matrix::const_block_iterator     A0(A.blocks_begin());
   size_t            nb, se;
   
   for (size_t B0 = 0; B0 < NB; B0 += nb, A0 += nb)
   {
      (void)A0->data();                         // Load A0 and following blocks.
      nb = A.incore_blocks();
      assert(nb <= NB - B0);
      se = A.incore_entries();
      
#ifdef _OPENMP
      const bool  parallel_cond = (nthreads > 1) && (nb > 1) && (se > CM2_OMP_MIN_LOOP);
      if (parallel_cond)
         __mult_omp(scale, A0, nb, 
                    MatrixShape(), typename Matrix::block_type::diagonal_storage_tag(),
                    x, y, nthreads);
      else
#endif
      {
         typename Matrix::const_block_iterator     Ak(A0);
         for (size_t k = 0; k < NB; ++k, ++Ak)
            matvec::__mult(scale, *Ak, x, y, nthreads);
      }
   }
}


// Separate-diagonal storage.
template <class T, class Matrix, class IterX, class IterY> 
void
__mult (T scale, const Matrix& A, 
        cm2::separate_diagonal_tag,
        IterX x, IterY y, int nthreads) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;
   typedef typename Matrix::orientation_tag     mat_orientation_tag;
   typedef typename Matrix::blocking_tag        mat_blocking_tag;

   const size_t         i0(A.first_index());
   const size_t         n(A.dim());

   if (A.segments_entries() > 0)
      matvec::__mult(scale, A, mat_shape_tag(), mat_orientation_tag(), 
                     mat_blocking_tag(), x, y, nthreads);

   if (A.is_unit_diagonal()) 
      vecvec::axpy_n_par(scale, x+i0, y+i0, n, nthreads);
   else
      vecvec::axypz_n_par(scale, A.diagonal_begin(), x+i0, y+i0, n, nthreads);
}

// Non-separate diagonal storage.
template <class T, class Matrix, class IterX, class IterY> 
INLINE void
__mult (T scale, const Matrix& A, 
        cm2::non_separate_diagonal_tag,
        IterX x, IterY y, int nthreads) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;
   typedef typename Matrix::orientation_tag     mat_orientation_tag;
   typedef typename Matrix::blocking_tag        mat_blocking_tag;

   matvec::__mult(scale, A, mat_shape_tag(), mat_orientation_tag(), 
                  mat_blocking_tag(), x, y, nthreads);
}

///
template <class T, class Matrix, class IterX, class IterY> 
INLINE void
__mult (T scale, const Matrix& A, IterX x, IterY y, int nthreads) 
{
   typedef typename Matrix::diagonal_storage_tag      mat_diagonal_storage_tag;

   matvec::__mult(scale, A, mat_diagonal_storage_tag(), x, y, nthreads);
}

// Small matrices.
template <class T, class Matrix, class IterX, class IterY> 
INLINE void
__mult_small (T scale, const Matrix& A, IterX x, IterY y) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterX>::value_type>::type    TS;

   const size_t      m(A.rows()), n(A.cols());
   IterX             xj;
   TS                s;

   for (size_t i = 0; i < m; ++i, ++y)
   {
      s = T(0); xj = x;
      for (size_t j = 0; j < n; ++j, ++xj)
         s += A(i, j) * (*xj);
      *y += scale * s;
   }
}

#endif // DOXYGEN_SKIP


/**
   Multiplies a matrix by a vector, with scaling.

   The matrix can be row- or column-oriented (defined by the `orientation_tag`),
   symmetric or not (`shape_tag`), blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      y += a * A % x
      y[i] += a * A(i,j) % x[j], sum on j, for i = 0..A.rows()-1  
   \endverbatim

   \pre     A.cols() == x.size()
   \pre     A.rows() == y.size()
   \warning To get y = a * A % x , don't forget to set \p y to zero prior to the multiplication.
   */
template <class T, class Matrix, class VecX, class VecY>
INLINE void
mult (T a, const Matrix& A, const VecX& x, VecY& y, int nthreads = 1) 
{
   assert(A.cols() == x.size());
   assert(A.rows() == y.size());

   // Fast return.
   if (a == T(0)) 
      return;

   if (A.entries() <= 12)
      matvec::__mult_small(a, A, x.begin(), y.begin());
   else
      matvec::__mult(a, A, x.begin(), y.begin(), nthreads);
}

/**
   Multiplies a matrix by a vector.

   The matrix can be row- or column-oriented (defined by the `orientation_tag`),
   symmetric or not (`shape_tag`), blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      y += A % x
      y[i] += A(i,j) % x[j], sum on j, for i = 0..A.rows())-1  
   \endverbatim

   \pre     A.cols() == x.size()
   \pre     A.rows() == y.size()
   \warning To get y = A % x , don't forget to set \p y to zero prior to the multiplication.
   */
template <class Matrix, class VecX, class VecY>
INLINE void
mult (const Matrix& A, const VecX& x, VecY& y, int nthreads = 1) 
{
   typedef typename Matrix::value_type   T;

   matvec::mult(T(1), A, x, y, nthreads);
}



#ifndef DOXYGEN_SKIP
// TRANSPOSE MULT 

// Non-blocked matrix (any shape, any orientation).
template <class T, class Matrix, class MatrixShape, class MatrixOrient, class IterX, class IterY>
INLINE void
__transpose_mult 
   (T scale, const Matrix& A, 
    MatrixShape, MatrixOrient, cm2::non_blocked_tag,
    IterX x, IterY y, int nthreads) 
{
   typedef typename cm2::transpose_orientation<typename Matrix::orientation_tag>::tag     Tmat_orientation_tag;

   matvec::__mult(scale, A, MatrixShape(), Tmat_orientation_tag(), cm2::non_blocked_tag(), x, y, nthreads);
}

// Blocked matrix.
#ifdef _OPENMP
template <class T, class BlockIterator, class IterX, class IterY>
INLINE void
__transpose_mult_omp (T scale, BlockIterator A0, int nbr_blocks, 
                      cm2::non_symmetric_tag, cm2::separate_diagonal_tag /*BlockDiagonalTag*/,
                      IterX x, IterY y, int nthreads) 
{
   typedef typename BlockIterator::value_type               block_type;
   typedef typename block_type::storage_tag                 block_storage_tag;
   typedef typename cm2::transpose_storage<block_storage_tag>::tag   Tblock_storage_tag;

   matvec::__mult_omp(scale, A0, nbr_blocks, 
                      cm2::non_symmetric_tag(), Tblock_storage_tag(),
                      cm2::separate_diagonal_tag(),
                      x, y, nthreads);
}

template <class T, class BlockIterator, class IterX, class IterY>
INLINE void
__transpose_mult_omp (T scale, BlockIterator A0, int nbr_blocks, 
                      cm2::symmetric_tag, cm2::separate_diagonal_tag /*BlockDiagonalTag*/,
                      IterX x, IterY y, int nthreads) 
{
   matvec::__mult_omp(scale, A0, nbr_blocks, 
                      cm2::symmetric_tag(), cm2::separate_diagonal_tag(),
                      x, y, nthreads);
}

#endif         // _OPENMP

// Blocked matrix (any shape, any orientation).
template <class T, class Matrix, class MatrixShape, class MatrixOrient, class IterX, class IterY>
void
__transpose_mult 
   (T scale, const Matrix& A, 
    MatrixShape, MatrixOrient, cm2::blocked_tag,
    IterX x, IterY y, int nthreads) 
{
   const size_t      NB(A.blocks());
   typename Matrix::const_block_iterator     A0(A.blocks_begin());
   size_t            nb, se;
   
   for (size_t B0 = 0; B0 < NB; B0 += nb, A0 += nb)
   {
      (void)A0->data();                            // Load A0 and following blocks.
      nb = A.incore_blocks();
      assert(nb <= NB - B0);
      se = A.incore_entries();
      
#ifdef _OPENMP
      const bool  parallel_cond = (nthreads > 1) && (nb > 1) && (se > CM2_OMP_MIN_LOOP);
      if (parallel_cond)
         __transpose_mult_omp(scale, A0, nb, 
                              MatrixShape(), typename Matrix::block_type::diagonal_storage_tag(),
                              x, y, nthreads);
      else
#endif
      {
         typename Matrix::const_block_iterator     Ak(A0);
         for (size_t k = 0; k < NB; ++k, ++Ak)
            matvec::__transpose_mult(scale, *Ak, x, y, nthreads);
      }
   }
}

// Transpose mult (separate-diagonal storage).
template <class T, class Matrix, class IterX, class IterY> 
void
__transpose_mult 
   (T scale, const Matrix& A, 
    cm2::separate_diagonal_tag,
    IterX x, IterY y, int nthreads) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;
   typedef typename Matrix::orientation_tag     mat_orientation_tag;
   typedef typename Matrix::blocking_tag        mat_blocking_tag;

   const size_t         i0    = A.first_index();
   const size_t         n     = A.dim();

   if (A.segments_entries() > 0)
      matvec::__transpose_mult(scale, A, mat_shape_tag(), mat_orientation_tag(), 
                               mat_blocking_tag(), x, y, nthreads);

   if (A.is_unit_diagonal()) 
      vecvec::axpy_n_par(scale, x+i0, y+i0, n, nthreads);
   else
      vecvec::axypz_n_par(scale, A.diagonal_begin(), x+i0, y+i0, n, nthreads);
}

// Transpose mult (non-separate diagonal storage).
template <class T, class Matrix, class IterX, class IterY> 
INLINE void
__transpose_mult 
   (T scale, const Matrix& A, 
    cm2::non_separate_diagonal_tag,
    IterX x, IterY y, int nthreads) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;
   typedef typename Matrix::orientation_tag     mat_orientation_tag;
   typedef typename Matrix::blocking_tag        mat_blocking_tag;

   matvec::__transpose_mult(scale, A, mat_shape_tag(), mat_orientation_tag(), 
                            mat_blocking_tag(), x, y, nthreads);
}

///
template <class T, class Matrix, class IterX, class IterY> 
INLINE void
__transpose_mult (T scale, const Matrix& A, IterX x, IterY y, int nthreads) 
{
   typedef typename Matrix::diagonal_storage_tag      mat_diagonal_storage_tag;

   matvec::__transpose_mult(scale, A, mat_diagonal_storage_tag(), x, y, nthreads);
}

// Small matrices.
template <class T, class Matrix, class IterX, class IterY> 
INLINE void
__transpose_mult_small (T scale, const Matrix& A, IterX x, IterY y) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterX>::value_type>::type    TS;

   const size_t      m(A.rows()), n(A.cols());
   IterX             xi;
   TS                s;

   for (size_t j = 0; j < n; ++j, ++y)
   {
      s = T(0); xi = x;
      for (size_t i = 0; i < m; ++i, ++xi)
         s += A(i, j) * (*xi);
      *y += scale * s;
   }
}

#endif // DOXYGEN_SKIP

/**
   Multiplies a transposed matrix by a vector, with scaling.

   The matrix can be row- or column-oriented (defined by the `orientation_tag`),
   symmetric or not (`shape_tag`), blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      y += a * transpose(A) % x
      y[i] += a * A(j,i) % x[j], sum on j, for i = 0..A.cols()-1  
   \endverbatim

   \pre     A.rows() == x.size()
   \pre     A.cols() == y.size()
   \warning To get y = a * transpose(A) % x , don't forget to set \p y to zero prior to the multiplication.
   */
template <class T, class Matrix, class VecX, class VecY>
INLINE void
transpose_mult (T a, const Matrix& A, const VecX& x, VecY& y, int nthreads = 1) 
{
   assert(A.rows() == x.size());
   assert(A.cols() == y.size());

   // Fast return.
   if (a == T(0)) 
      return;

   if (A.entries() <= 12)
      matvec::__transpose_mult_small(a, A, x.begin(), y.begin());
   else
      matvec::__transpose_mult(a, A, x.begin(), y.begin(), nthreads);
}

/**
   Multiplies a transposed matrix by a vector.

   The matrix can be row- or column-oriented (defined by the `orientation_tag`),
   symmetric or not (`shape_tag`), blocked or not (`blocking_tag`), 
   with separate diagonal or not (`diagonal_storage_tag`).

   \verbatim     
      y += transpose(A) % x
      y[i] += A(j,i) % x[j], sum on j, for i = 0..A.cols()-1
   \endverbatim

   \pre     A.rows() == x.size()
   \pre     A.cols() == y.size()
   \warning To get y = transpose(A) % x , don't forget to set \p y to zero prior to the multiplication.
   */
template <class Matrix, class VecX, class VecY>
INLINE void
transpose_mult (const Matrix& A, const VecX& x, VecY& y, int nthreads = 1) 
{
   typedef typename Matrix::value_type    T;

   matvec::transpose_mult(T(1), A, x, y, nthreads);
}




#ifndef DOXYGEN_SKIP

// Vt_A_V //
// Non-symmetric matrix, non-blocked, any orientation.
template <class Matrix, class IterV>
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::non_symmetric_tag, cm2::non_blocked_tag, IterV v,
          int DUM_OMP(nthreads)) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterV>::value_type>::type    T;

   const size_t            NI(A.segments());
   const typename Matrix::const_segment_iterator   A_0(A.seg_begin());
   typename Matrix::const_segment_iterator         A_i;
   T                       s(0);
#ifdef _OPENMP
   const bool  parallel_cond = (nthreads > 1) && (NI > 1) && (A.entries() > CM2_OMP_MIN_LOOP);
#endif

   #pragma omp parallel for if(parallel_cond) reduction(+:s) private(A_i) schedule(static) num_threads(nthreads)
   for (int k = 0; k < int(NI); ++k)      // Parallel loop over A-segments.
   {
      A_i = A_0;  A_i += k;
      const size_t i(A_i.index()), jbeg(A_i.index_begin()), hi(A_i.size());
      s += *(v+i) * vecvec::dot_n(v+jbeg, A_i.begin(), hi);
   }

   return s;
}

// Symmetric matrix, non-blocked, any orientation.
template <class Matrix, class IterV>
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::symmetric_tag, cm2::non_blocked_tag, IterV v,
          int DUM_OMP(nthreads)) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterV>::value_type>::type    T;

   const size_t            NI(A.segments());
   const typename Matrix::const_segment_iterator   A_0(A.seg_begin());
   typename Matrix::const_segment_iterator         A_i;
   T                       s(0);
#ifdef _OPENMP
   const bool  parallel_cond = (nthreads > 1) && (NI > 1) && (2*A.entries() > CM2_OMP_MIN_LOOP);
#endif

   #pragma omp parallel for if(parallel_cond) reduction(+:s) private(A_i) schedule(static) num_threads(nthreads)
   for (int k = 0; k < int(NI); ++k)      // Parallel loop over A-segments.
   {
      A_i = A_0;  A_i += k;
      const size_t i(A_i.index()), jbeg(A_i.index_begin()), hi(A_i.size());
      T            si;

      if (hi == 0)
         continue;
      si = T(2) * vecvec::dot_n(v+jbeg, A_i.begin(), hi);
      if (jbeg+hi >= i+1)
         si -= *(v+i) * (*(A_i.begin()+i-jbeg));      // Count diagonal terms only once.
      s += *(v+i) * si;     
   }

   return s;
}

// Non-blocked matrix.
template <class Matrix, class IterV>
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::non_blocked_tag, IterV v, int nthreads) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;

   return matvec::__Vt_A_V(A, mat_shape_tag(), cm2::non_blocked_tag(), v, nthreads);
}


// Declarations.
template <class Matrix, class IterV> 
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::separate_diagonal_tag, IterV v, int nthreads);

template <class Matrix, class IterV> 
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::non_separate_diagonal_tag, IterV v, int nthreads);


// Blocked matrix.
template <class Matrix, class IterV>
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::blocked_tag, IterV v, int nthreads) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterV>::value_type>::type    T;

   typedef typename Matrix::block_type::diagonal_storage_tag        block_diagonal_storage_tag;

   const size_t            NB(A.blocks());
   typename Matrix::const_block_iterator     A0(A.blocks_begin());
   typename Matrix::const_block_iterator     Ak(A0);
   size_t                  nb, se;
   bool                    parallel_cond;
   T                       s(0);

   for (size_t B0 = 0; B0 < NB; B0 += nb, A0 += nb)
   {
      (void)A0->data();                         // Load A0 and following blocks.
      nb = A.incore_blocks();
      assert(nb <= NB - B0);
      se = A.incore_entries();
      parallel_cond = (nthreads > 1) && (nb > 1) && (se > CM2_OMP_MIN_LOOP);

      #pragma omp parallel for if(parallel_cond) reduction(+:s) private(Ak) schedule(static) num_threads(nthreads)
      for (int k = 0; k < int(nb); ++k)         // Parallel loop over in-core blocks.
      {
         Ak = A0;  Ak += k;
         s += matvec::__Vt_A_V(*Ak, block_diagonal_storage_tag(), v, 
                               /*nthreads=>*/ parallel_cond ? 1 : nthreads);
      }
   }

   return s;
}

// Separate-diagonal storage.
template <class Matrix, class IterV> 
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::separate_diagonal_tag, IterV v, int nthreads) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterV>::value_type>::type    T;
   typedef typename Matrix::blocking_tag        mat_blocking_tag;

   const size_t      i0(A.first_index());
   const size_t      n(A.dim());
   T                 s(0);

   if (A.segments_entries() > 0)
      s += matvec::__Vt_A_V(A, mat_blocking_tag(), v, nthreads);

   if (A.is_unit_diagonal()) 
      s += vecscal::sqr_two_norm_n_par(v+i0, n, nthreads);
   else
      s += vecvec::dot_n_par(A.diagonal_begin(), v+i0, v+i0, n, nthreads);

   return s;
}

// Non-separate diagonal storage.
template <class Matrix, class IterV>
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V (const Matrix& A, cm2::non_separate_diagonal_tag, IterV v, int nthreads) 
{
   typedef typename Matrix::blocking_tag        mat_blocking_tag;

   return matvec::__Vt_A_V(A, mat_blocking_tag(), v, nthreads);
}

// Small non-symmetric matrices.
template <class Matrix, class IterV>
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V_small (const Matrix& A, cm2::non_symmetric_tag, IterV v) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterV>::value_type>::type    T;

   const size_t      n(A.dim());
   T                 s(0);

   for (size_t j = 0; j < n; ++j)
   {
      IterV       vi(v);
      T           si(0);

      for (size_t i = 0; i < n; ++i, ++vi)
         si += A(i, j) * (*vi);

      s += si * *(v+j);
   }

   return s;
}

// Small symmetric matrices (dim <= 3).
template <class Matrix, class IterV>
INLINE typename cm2::biggest2_type<typename Matrix::value_type,
                                   typename std::iterator_traits<IterV>::value_type>::type
__Vt_A_V_small (const Matrix& A, cm2::symmetric_tag, IterV v) 
{
   typedef typename cm2::biggest2_type<typename Matrix::value_type,
                                       typename std::iterator_traits<IterV>::value_type>::type    T;

   const size_t      n(A.dim());
   T                 s(0);

   assert(n <= 3);

   if (n >= 1)
      s = A(0, 0)*v[0]*v[0];
   if (n >= 2)
      s += (A(1, 1)*v[1] + 2*A(0, 1)*v[0]) * v[1];
   if (n == 3)
      s += (A(2, 2)*v[2] + 2*(A(0, 2)*v[0] + A(1, 2)*v[1])) * v[2];

   return s;
}
#endif // DOXYGEN_SKIP


/**
   transpose(V) % A % V

   \return  transpose(V) % A % V
   \pre     A.rows() = A.cols() = V.size()
   */
template <class Matrix, class Vec>
typename cm2::biggest2_type<typename Matrix::value_type,
                            typename Vec::value_type>::type
Vt_A_V (const Matrix& A, const Vec& V, int nthreads = 1u)
{
   typedef typename Matrix::shape_tag                 mat_shape_tag;
   typedef typename Matrix::diagonal_storage_tag      mat_diagonal_storage_tag;
   
   assert(A.rows() == A.cols());
   assert(A.rows() == V.size());

   if (V.size() <= 3)
      return matvec::__Vt_A_V_small(A, mat_shape_tag(), V.begin());
   else
      return matvec::__Vt_A_V(A, mat_diagonal_storage_tag(), 
                              V.begin(), nthreads);
}


// TENSOR

#ifndef DOXYGEN_SKIP

template <class T, class VecX, class Matrix>
void
__rank1 (T a, const VecX& Vx, Matrix& A, 
         cm2::symmetric_tag) 
{
   const size_t                           M(Vx.size());
   const typename VecX::const_iterator    x0(Vx.begin()); 
   typename VecX::const_iterator          x, y; 
   typename Matrix::value_type            s;
   size_t                                 i, j;

   for (j = 0, y = x0; j < M; ++j, ++y) 
   {
      s = a * (*y);
      for (i = 0, x = x0; i <= j; ++i, ++x)
         A.operator()(i,j) += s * (*x);
   }
}

template <class T, class VecX, class Matrix>
void
__rank1 (T a, const VecX& Vx, Matrix& A, 
         cm2::non_symmetric_tag) 
{
   const size_t                           M(Vx.size());
   const typename VecX::const_iterator    x0(Vx.begin()); 
   typename VecX::const_iterator          y(x0); 
   typename Matrix::value_type            s;

   for (size_t j = 0; j < M; ++j, ++y)
   {
      s = a * (*y);
      vecvec::axpy_n(s, x0, A.data_col(j), M);
   }
}

template <class T, class VecX, class VecY, class Matrix>
void
__rank2 (T a, const VecX& Vx, const VecY& Vy, Matrix& A, 
         cm2::non_symmetric_tag) 
{
   const size_t                           M(Vx.size());
   const size_t                           N(Vy.size());
   const typename VecX::const_iterator    x0(Vx.begin()); 
   typename VecX::const_iterator          y(Vy.begin()); 
   typename Matrix::value_type            s;

   for (size_t j = 0; j < N; ++j, ++y)
   {
      s = a * (*y);
      vecvec::axpy_n(s, x0, A.data_col(j), M);
   }
}

template <class T, class VecX, class VecY, class Matrix>
void
__rank2sym (T a, const VecX& Vx, const VecY& Vy, Matrix& A, 
            cm2::non_symmetric_tag __tag) 
{
   matvec::__rank2 (a, Vx, Vy, A, __tag);
   matvec::__rank2 (a, Vy, Vx, A, __tag);
}

template <class T, class VecX, class VecY, class Matrix>
void
__rank2sym (T a, const VecX& Vx, const VecY& Vy, Matrix& A, cm2::symmetric_tag) 
{
   const size_t                           M(Vx.size());
   const size_t                           N(Vy.size());
   const typename VecX::const_iterator    x0(Vx.begin()); 
   const typename VecX::const_iterator    y0(Vy.begin()); 
   typename VecX::const_iterator          x;
   typename VecY::const_iterator          y;
   typename Matrix::value_type            s;
   size_t                                 i, j, P;

   for (j = 0, y = y0; j < N; ++j, ++y) 
   {
      P = std::min(j, M);
      s = a * (*y);
      for (i = 0, x = x0; i <= P; ++i, ++x)
         A.operator()(i,j) += s * (*x);
   }

   for (j = 0, x = x0; j < M; ++j, ++x) 
   {
      P = std::min(j, N);
      s = a * (*x);
      for (i = 0, y = y0; i <= P; ++i, ++y)
         A.operator()(i,j) += s * (*y);
   }
}

template <class T, class Vec, class Matrix>
void
__square_skew (T a, const Vec& V, Matrix& A, cm2::symmetric_tag tag) 
{ 
   cm2::symmetric_fixed<T,3>    VV(0.);

   matvec::__rank1(a, V, VV, tag);

   A(0,0) -= VV(1,1) + VV(2,2);
   A(0,1) += VV(0,1);
   A(1,1) -= VV(0,0) + VV(2,2);
   A(0,2) += VV(0,2);
   A(1,2) += VV(1,2);
   A(2,2) -= VV(0,0) + VV(1,1);
}

template <class T, class Vec, class Matrix>
void
__square_skew (T a, const Vec& V, Matrix& A, cm2::non_symmetric_tag) 
{ 
   cm2::symmetric_fixed<T,3>    VV(0.);

   matvec::__rank1(a, V, VV, cm2::symmetric_tag());

   A(0,0) -= VV(1,1) + VV(2,2);
   A(1,0) += VV(0,1);
   A(2,0) += VV(0,2);
   A(0,1) += VV(0,1);
   A(1,1) -= VV(0,0) + VV(2,2);
   A(2,1) += VV(1,2);
   A(0,2) += VV(0,2);
   A(1,2) += VV(1,2);
   A(2,2) -= VV(0,0) + VV(1,1);
}

#endif   // DOXYGEN_SKIP


/**
   Rank-1 update with one vector.

   Adds the rank-1 of a vector to a matrix, with scaling.

   \verbatim    
      A(i,j) += a * x[i]*x[j]  
   \endverbatim

   \pre     x.size() <= A.rows()
   \pre     x.size() <= A.cols()
   \warning To get A(i,j) = a * x[i]*x[j] , don't forget to set \p A to zero before this call.
   */
template <class T, class Vec, class Matrix>
INLINE void
rank1 (T a, const Vec& x, Matrix& A) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;

   // Fast return.
   if (a == T(0)) 
      return;

   assert(x.size() <= A.rows());
   assert(x.size() <= A.cols());
   matvec::__rank1(a, x, A, mat_shape_tag());
}

/**
   Rank-1 update with one vector.

   Adds the rank-1 of a vector to a matrix.

   \verbatim    
      A(i,j) += x[i]*x[j]  
   \endverbatim

   \pre     x.size() <= A.rows()
   \pre     x.size() <= A.cols()
   \warning To get A(i,j) = x[i]*x[j] , don't forget to set \p A to zero before this call.
   */
template <class Vec, class Matrix>
INLINE void
rank1 (const Vec& x, Matrix& A) 
{
   typedef typename Matrix::value_type   T;
   matvec::rank1(T(1), x, A);
}

/**
   Rank-2 update between two vectors.

   Adds the rank-2 of two vectors to a matrix, with scaling.

   \verbatim    
      A(i,j) += a * x[i]*y[j]  
   \endverbatim

   \pre     x.size() <= A.rows()
   \pre     y.size() <= A.cols()
   \warning To get A(i,j) = a * x[i]*y[j] , don't forget to set \p A to zero before this call.
   */
template <class T, class VecX, class VecY, class Matrix>
INLINE void
rank2 (T a, const VecX& x, const VecY& y, Matrix& A) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;

   // Fast return.
   if (a == T(0)) 
      return;

   assert(x.size() <= A.rows());
   assert(y.size() <= A.cols());
   matvec::__rank2(a, x, y, A, mat_shape_tag());
}

/**
   Symmetric rank-2 update between two vectors.

   Adds the symmetric rank-2 of two vectors to a matrix, with scaling.

   \verbatim    
      A(i,j) += a * (x[i]*y[j] + x[j]*y[i]) 
   \endverbatim

   \note    2*a* x[i]*y[i] is added on the diagonal.

   \pre     x.size() <= A.rows()
   \pre     x.size() <= A.col()
   \pre     y.size() <= A.rows()
   \pre     y.size() <= A.cols()
   \warning To get A(i,j) = a * (x[i]*y[j] + x[j]*y[i]) , don't forget to set \p A to zero before this call.
   */
template <class T, class VecX, class VecY, class Matrix>
INLINE void
rank2sym (T a, const VecX& x, const VecY& y, Matrix& A) 
{
   typedef typename Matrix::shape_tag           mat_shape_tag;

   // Fast return.
   if (a == T(0)) 
      return;

   assert(x.size() <= A.rows());
   assert(x.size() <= A.cols());
   assert(y.size() <= A.rows());
   assert(y.size() <= A.cols());
   matvec::__rank2sym(a, x, y, A, mat_shape_tag());
}


/**
   Cross products between all columns of a matrix and a vector.

   \verbatim    
      B.col(j) += A.col(j) X x  
   \endverbatim

   \pre     A.rows() >= 3
   \pre     B.rows() >= 3
   \pre     x.size() >= 3
   \pre     B.cols() >= A.cols()
   \warning To get B.col(j) = A.col(j) X x , don't forget to set \p B to zero before this call.
   */
template <class Vec, class Matrix>
void
cross (const Matrix& A, const Vec& x, Matrix& B) 
{
   for (size_t j = 0; j < A.cols(); ++j) 
   {
      B(0,j) += A(1,j)*x[2] - A(2,j)*x[1];
      B(1,j) += A(2,j)*x[0] - A(0,j)*x[2];
      B(2,j) += A(0,j)*x[1] - A(1,j)*x[0];
   }
}

/**
   Cross products between a vector and all columns of a matrix.

   \verbatim    
      B.col(j) += x X A.col(j)  
   \endverbatim
   \pre     A.rows() >= 3
   \pre     B.rows() >= 3
   \pre     x.size() >= 3
   \pre     B.cols() >= A.cols()
   \warning To get B.col(j) = x X A.col(j) , don't forget to set \p B to zero before this call.
   */
template <class Vec, class Matrix>
void
cross (const Vec& x, const Matrix& A, Matrix& B) 
{
   for (size_t j = 0; j < A.cols(); ++j) 
   {
      B(0,j) += x[1]*A(2,j) - x[2]*A(1,j);
      B(1,j) += x[2]*A(0,j) - x[0]*A(2,j);
      B(2,j) += x[0]*A(1,j) - x[1]*A(0,j);
   }
}

/**
   Skew matrix update (hat operator).

   Adds the skew matrix of a vector x.
   The skew matrix of a vector is the matrix skew(x) such that:
      skew(x) % y = x ^ y  for any y.

   \verbatim    
      A += skew(x)  
   \endverbatim
   \pre     x.size() >= 3
   \pre     A.rows() >= 3
   \pre     A.cols() >= 3
   \warning To get A = skew(x) , don't forget to set \p A to zero before this call.
   */
template <class Vec, class Matrix>
void
skew (const Vec& x, Matrix& A) 
{  
   A(0,1) -= x[2];
   A(1,0) += x[2];
   A(0,2) += x[1];
   A(2,0) -= x[1];
   A(1,2) -= x[0];
   A(2,1) += x[0];
}

/**
   Skew matrix update, with scaling (hat operator).

   AXPY the skew matrix of a vector x.
   The skew matrix of a vector is the matrix skew(x) such that:
      skew(x) % y = x X y  for any y.

   \verbatim    
      A += a * skew(x)  
   \endverbatim
   \pre     x.size() >= 3
   \pre     A.rows() >= 3
   \pre     A.cols() >= 3
   \warning To get A = a * skew(v) , don't forget to set \p A to zero before this call.
    */
template <class T, class Vec, class Matrix>
void
skew (T a, const Vec& x, Matrix& A) 
{ 
   if (a == T(0)) 
      return;

   A(0,1) -= a * x[2];
   A(1,0) += a * x[2];
   A(0,2) += a * x[1];
   A(2,0) -= a * x[1];
   A(1,2) -= a * x[0];
   A(2,1) += a * x[0];
}


/**
   Square skew matrix update, with scaling (square hat operator).

   AXPY the square skew matrix of a vector V:
   \f$
      A = A + a \ \left [ \begin{array}{ccc}
                               -{V_y}^2-{V_z}^2 &    V_x V_y       &    V_x V_z       \\
                                    V_y V_x     & -{V_x}^2-{V_z}^2 &    V_y V_z       \\ 
                                    V_z V_x     &    V_z V_y       & -{V_x}^2-{V_y}^2 \\
                          \end{array} \right ] 
   \f$

   \verbatim    
      A += a * skew(V) % skew(V) 
   \endverbatim

   skew(V) is a skew-symmetric matrix.

   skew(V) % skew(V) is a symmetric matrix.

   \pre     V.size() >= 3
   \pre     A.rows() >= 3
   \pre     A.cols() >= 3
   \warning To get A = a * skew(V) % skew(V), don't forget to set \p A to zero before this call.
    */
template <class T, class Vec, class Matrix>
INLINE void
square_skew (T a, const Vec& V, Matrix& A) 
{ 
   typedef typename Matrix::shape_tag           mat_shape_tag;

   // Fast return.
   if (a == T(0)) 
      return;

   assert(V.size() >= 3);
   assert(A.rows() >= 3);
   assert(A.cols() >= 3);
   matvec::__square_skew(a, V, A, mat_shape_tag());
}


/**
   Square skew matrix update(square hat operator).

   \verbatim    
      A += skew(V) % skew(V) 
   \endverbatim

   skew(V) is a skew-symmetric matrix.

   skew(V) % skew(V) is a symmetric matrix.

   \pre     V.size() >= 3
   \pre     A.rows() >= 3
   \pre     A.cols() >= 3
    */
template <class Vec, class Matrix>
INLINE void
square_skew (const Vec& V, Matrix& A) 
{ 
   typedef typename Matrix::shape_tag           mat_shape_tag;
   typedef typename Vec::value_type             T;

   assert(V.size() >= 3);
   assert(A.rows() >= 3);
   assert(A.cols() >= 3);
   matvec::__square_skew(T(1), V, A, mat_shape_tag());
}
//@}

}  // namespace matvec
}  // namespace cm2


#if defined(_MSC_VER) && (_MSC_VER < 1400) 
#pragma warning (pop)
#endif

#endif

#ifndef __CM2_API_TOOLS_H__
#define __CM2_API_TOOLS_H__

/**
   \file       cm2_api_tools.h
   \brief      Some simple functions that may be useful.
   \copyright  (C)1999-2019, Computing Objects, France. info@computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

namespace cm2 {

///
template <class Vector0, class Vector1, class Vector2>
void
make_orthonormal_basis (Vector0& B0, Vector1& B1, Vector2& B2) 
{
   const double   s  = vecscal::normalize(B0);

   if (s == 0.0) 
      B0[0] = 1.0;

   vecvec::copy_cross(B0, B1, B2);   

   if (vecscal::equal(B2, 0.0)) 
   {  
      // Compute a new perpendicular 3rd node and make B2 = B0 ^ node3.
      const size_t   i = vecscal::min_norm_index(B0);    // node3[i] = 1
      vecscal::copy(0.0, B1);
      B1[i] = 1.0;
      vecvec::copy_cross(B0, B1, B2);
   }

   vecvec::copy_cross(B2, B0, B1);
   vecscal::normalize(B1);                   // Make sure directors are normalized and orthogonal.
   vecscal::normalize(B2);                   //
}

///
template <class Matrix>
void
make_orthonormal_basis2 (Matrix& B) 
{
   typename Matrix::segment_type    B0 = B.seg(0);
   typename Matrix::segment_type    B1 = B.seg(1);
   DoubleVec3                       B2;

   make_orthonormal_basis(B0, B1, B2); 
}

///
template <class Matrix>
inline void
make_orthonormal_basis (Matrix& B)           
{ 
   typename Matrix::segment_type    B0 = B.seg(0);
   typename Matrix::segment_type    B1 = B.seg(1);
   typename Matrix::segment_type    B2 = B.seg(2);

   make_orthonormal_basis(B0, B1, B2); 
}

///
template <class Vector, class Matrix>
inline void
make_orthonormal_basis (Vector& B0, Matrix& B12)           
{ 
   typename Matrix::segment_type    B1 = B12.seg(0);
   typename Matrix::segment_type    B2 = B12.seg(1);

   make_orthonormal_basis(B0, B1, B2); 
}


/** 
   Linear interpolation in a 1-D domain.

   Returns the index `i` of the lower bound of \p d in \p domain and the coefficient for this index.

   If `i` < size-1, the linear interpolation is `w*range[i] + (1.-w)*range[i+1]` 
   with `w = (domain[i+1] - d) / (domain[i+1] - domain[i])`.

   If `i` == size-1, the linear interpolation is `range[i]` with `w == 1`.

   If size == 0 return `std::pair(CM2_NONE, 0.0)`

   \pre     The domain vector must be sorted.
   */
template <class VectorD>
std::pair<size_t, double>
interpolation (const typename VectorD::value_type& d, const VectorD& domain) 
{
   typename VectorD::const_iterator       p;
   double                        w;
   size_t                        i;

   if (domain.size() == 0) 
      return std::pair<size_t, double>(size_t(CM2_NONE), 0.0);
   if (domain.size() == 1) 
      return std::pair<size_t, double>(0, 1.0);

   p = std::lower_bound(ALL(domain), d);      // lowest position of domains >= d.

   if (p == domain.end())        
      return std::pair<size_t, double>(domain.size()-1, 1.0);
   if (p == domain.begin()) 
      return std::pair<size_t, double>(0, 1.0);

   i = p - domain.begin();
   w = (double)(domain[i] - d) / (double)(domain[i] - domain[i-1]);

   return std::pair<size_t, double>(i-1, w);
}

/** 
   Linear interpolation in a 1-D domain.

   If size == 0 return 0

   \pre     The domain vector must be sorted.
   \pre     Sizes of domain and range must be equal.
   */
template <class VectorD, class VectorR>
typename VectorR::value_type
interpolation (const typename VectorD::value_type& d, const VectorD& domain, const VectorR& range) 
{
   typedef typename VectorR::value_type   RV_type;

   const std::pair<unsigned, double>   P(cm2::interpolation(d, domain));
   const size_t                        i(P.first);
   const double                        wi(P.second);
   typename VectorR::value_type        v1, v2;

   assert(domain.size() == range.size());

   if (i == CM2_NONE) 
      return RV_type(0);

   v1 = range[i];
   v2 = (i < range.size()-1) ? range[i+1] : v1;

   return v2 + wi * (v1 - v2);         // linear interpolation.
}

/** 
   Bilinear interpolation in a 2-D domain.

   If size == 0 return 0

   \pre     The domain vectors must be sorted.
   \pre     D1.size() == R.rows().
   \pre     D2.size() == R.cols().
   */
template <class VectorD1, class VectorD2, class MatrixR>
typename MatrixR::value_type
interpolation (const typename VectorD1::value_type& d1, const typename VectorD2::value_type& d2,
               const VectorD1& D1, const VectorD2& D2, const MatrixR& R) 
{
   typedef typename MatrixR::value_type   RV_type;

   const std::pair<unsigned, double>   Pi(cm2::interpolation(d1, D1));
   const std::pair<unsigned, double>   Pj(cm2::interpolation(d2, D2));
   const size_t                        i(Pi.first);
   const size_t                        j(Pj.first);
   const double                        wi(Pi.second);
   const double                        wj(Pj.second);
   typename MatrixR::value_type        v11, v12, v21, v22;
      
   assert(D1.size() == R.rows());
   assert(D2.size() == R.cols());

   if ((i == CM2_NONE) || (j == CM2_NONE)) 
      return RV_type(0);

   v11 = R(i,j);
   v21 = (i < D1.size()-1) ? R(i+1,j) : v11;
   v12 = (j < D2.size()-1) ? R(i,j+1) : v11;
   if ((i < D1.size()-1) && (j < D2.size()-1)) v22 = R(i+1,j+1);
   else if (i < D1.size()-1) v22 = v21;
   else v22 = v12;
  
   v21 += wi * (v11 - v21);
   v22 += wi * (v12 - v22);
   return v22 + wj * (v21 - v22);
}


/// Int to type template class for meta programming.
template <int N>
struct int_to_type      { enum { val = N }; };

}  // end namespace cm2

#endif      // __CM2_API_TOOLS_H__

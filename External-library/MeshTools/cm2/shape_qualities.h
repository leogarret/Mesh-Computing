/**
   \file       shape_qualities.h
   \brief      Declarations of the shape-quality related routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it - even the modified version.
 ******************************************************************************************/

#ifndef __CM2_SHAPE_QUALITIES_H__
#define __CM2_SHAPE_QUALITIES_H__


namespace cm2 {
namespace meshtools {

/****************************************************************************************/
/** @name Jacobians */
//@{

/**
   Computes the jacobians of all elements of a mesh.

   `J[i]` is the jacobian (computed at centroid) of element at column `i` in matrix \p connect.

   The jacobians are a measure of the extent of the element:
      - Edge elements: 2 * length.
      - Face elements: 2 * area (approximative for warped quads).
      - Solid elements: 6 * area (approximative for warped solids).

   \param[in]  pos            The coordinates matrix.
   \param[in]  connect        The connectivity matrix.
   \param[in]  FE_type        The type of the elements (\ref fe_geo_page). \n
                              Only linear types are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                              cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                              cm2::CM2_SOLID_MIX.
   \param[out] J              The jacobians. The initial vector is overwritten.

   \return     Error code. Returns 0 when successful. \n
               Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int 
jacobians (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type, DoubleVec& J);

//@}



/****************************************************************************************/
/** @name Shape quality */
//@{

/**
   Computes the shape qualities of all elements of a mesh.

   `Qs[i]` is the quality of element at column `i` in matrix \p connect.

   The quality measure is normalized between 0 and 1. 
   1 = perfect shape (equilateral triangle, tetra or square quad...)

   \param[in]  pos            The coordinates matrix.
   \param[in]  connect        The connectivity matrix.
   \param[in]  FE_type        The type of the elements (\ref fe_geo_page). \n
                              Only linear types are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                              cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                              cm2::CM2_SOLID_MIX.
   \param[out] Qs             The output qualities between 0 and 1. The initial vector is overwritten.

   \return     Error code. Returns 0 when successful. \n
               Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int 
shape_qualities (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type, DoubleVec& Qs);

/**
   Finds the element the minimum shape quality in a mesh.

   The quality measure is normalized between 0 and 1. 
   1 = perfect shape (equilateral triangle, tetra or square quad...)

   \param[in]  pos            The coordinates matrix.
   \param[in]  connect        The connectivity matrix.
   \param[in]  FE_type        The type of the elements (\ref fe_geo_page). \n
                              Only linear types are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                              cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                              cm2::CM2_SOLID_MIX.
   \param[out] qmin           The minimum quality.
   \param[out] jmin           The column index of the element with minimum quality.

   \return     Error code. Returns 0 when successful. \n
               Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int 
min_quality (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type, double& qmin, unsigned& jmin);

/**
   Adds in an histogram the shape qualities of all elements of a mesh 
   (very simple function provided for convenience)

   The bins of the histogram must have been previously set.

   \param[in]  pos            The coordinates matrix.
   \param[in]  connect        The connectivity matrix.
   \param[in]  FE_type        The type of the elements (\ref fe_geo_page). \n
                              Only linear types are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                              cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                              cm2::CM2_SOLID_MIX.
   \param[in,out] histo_Qs    Qualities are counted into the histogram (bins must have been previously defined).

   \return     Error code. Returns 0 when successful. \n
               Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \warning    To count only the hits for this mesh, the hits of the histogram must have been cleared before.
   */
inline int 
shape_qualities 
   (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type, misc::histogram& histo_Qs)
{
   DoubleVec      Qs;
   int            err;

   err = meshtools::shape_qualities(pos, connect, FE_type, Qs);
   if (!err) 
      histo_Qs.process(Qs);

   return err;
}

//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

/**
   \file       interpolate2d.h
   \brief      Declarations of some interpolation routines on 2-D meshes.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_INTERPOLATE_2D_H__
#define __CM2_INTERPOLATE_2D_H__


namespace cm2 {
namespace meshtools2d {

/****************************************************************************************/
/** @name Fields interpolation on 2-D meshes */
//@{

/**
   Interpolates a scalar field (doubles) defined on the nodes of a 2-D mesh.

   The interpolated values are computed on a specific set of nodes that are not in the mesh.

   \param[in]     pos            The 2-D coordinates matrix (2xNODS). 
   \param[in]     connectM       The connectivity matrix of the mesh (3xNEFS).
                                 Each node of the mesh should have a valid associated field value.
   \param[in]     neighbors      The neighbors matrix (3xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectM,
                                 the vector gives one face connected to it.
                                 For nodes not in \p connectM, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] field          The scalar field (vector of size NODS).
   \param[in]     nodes          The node IDs for which the field values must be computed.
                                 Values for nodes of the mesh are not recomputed.
   \param[in]     FE_type        The type of the elements (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 2.
   \pre        connectM.rows() == 3 (i.e. triangle mesh), connectM.rows() == 4 (all-quad or mixed mesh) or empty.
   \pre        field.size() == pos.cols().
   */
CM2MESHTOOLS2D_API int 
interpolate 
   (const DoubleMat& pos, const UIntMat& connectM, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleVec& field, const UIntVec& nodes, cm2::element_type FE_type);                          


/**
   Interpolates a vectorial field (doubles) defined on the nodes of a 2-D mesh.

   The interpolated values are computed on a specific set of nodes that are not in the mesh.

   \param[in]     pos            The 2-D coordinates matrix (2xNODS). 
   \param[in]     connectM       The connectivity matrix of the face mesh (3xNEFS or 4xNEFS).
                                 Each node of the mesh should have a valid associated field vector.
   \param[in]     neighbors      The neighbors matrix (3xNEFS or 4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectM,
                                 the vector gives one face connected to it.
                                 For nodes not in \p connectM, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] field          The vectorial field (matrix of dimensions FxNODS).
                                 Each component is interpolated independently from the others.
   \param[in]     nodes          The node IDs for which the field vectors must be computed.
                                 Vectors for nodes already in the mesh are not recomputed.
   \param[in]     FE_type        The type of the elements (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \note       Metrics cannot be interpolated with this function as the metric components are not 
               independent from each other. Use meshtools2d::interpolate_metrics instead.
   \pre        pos.rows() == 2.
   \pre        connectM.rows() == 3 (i.e. triangle mesh), connectM.rows() == 4 (all-quad or mixed mesh) or empty.
   \pre        field.cols() == pos.cols().
   */
CM2MESHTOOLS2D_API int 
interpolate 
   (const DoubleMat& pos, const UIntMat& connectM, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleMat& field, const UIntVec& nodes, cm2::element_type FE_type);                          
//@}


}  // end namespace meshtools2d
}  // end namespace cm2

#endif

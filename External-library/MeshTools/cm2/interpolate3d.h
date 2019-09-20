/**
   \file       interpolate3d.h
   \brief      Declarations of some interpolation routines on 3-D meshes.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_INTERPOLATE_3D_H__
#define __CM2_INTERPOLATE_3D_H__


namespace cm2 {
namespace meshtools3d {


/****************************************************************************************/
/** @name Interpolation of fields on 3-D tetrahedral meshes */
//@{

/**
   Interpolates a scalar field (doubles) defined on the nodes of a tetrahedral mesh.

   The interpolated values are computed on a specific set of nodes that are not in the mesh.

   \param[in]     pos            The 3-D coordinates matrix (3xNODS). 
   \param[in]     connectTH4     The connectivity matrix of the tetrahedron mesh (4xNEFS).
                                 Each node of the mesh should have a valid associated field value.
   \param[in]     neighbors      The neighbors matrix (4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectTH4,
                                 the vector gives one tetrahedron connected to it.
                                 For nodes not in \p connectTH4, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] field          The scalar field (vector of size NODS).
   \param[in]     nodes          The node IDs for which the field values must be computed.
                                 Values for nodes of the mesh are not recomputed.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre           \p pos.rows() == 3.
   \pre           \p connectTH4.rows() == 4 (i.e. tetrahedral mesh).
   \pre           \p field.size() == \p pos.cols().
   */
CM2MESHTOOLS3D_API int 
interpolate 
   (const DoubleMat& pos, const UIntMat& connectTH4, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleVec& field, const UIntVec& nodes);                          


/**
   Interpolates a vectorial field (doubles) defined on the nodes of a tetrahedral mesh.

   The interpolated values are computed on a specific set of nodes that are not in the mesh.

   \param[in]     pos            The 3-D coordinates matrix (3xNODS). 
   \param[in]     connectTH4     The connectivity matrix of the tetrahedron mesh (4xNEFS).
                                 Each node of the mesh should have a valid associated field value.
   \param[in]     neighbors      The neighbors matrix (4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectTH4,
                                 the vector gives one tetrahedron connected to it.
                                 For nodes not in \p connectTH4, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] field          The vectorial field (matrix of dimensions FxNODS).
                                 Each component is interpolated independently from the others.
   \param[in]     nodes          The node IDs for which the field vectors must be computed.
                                 Vectors for nodes already in the mesh are not recomputed.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   
   \warning       Metrics cannot be interpolated with this function as the metric components are not 
                  independent from each other. Use meshtools3d::interpolate_metrics instead.
   \pre           \p pos.rows() == 3.
   \pre           \p connectTH4.rows() == 4 (i.e. tetrahedral mesh).
   \pre           \p field.cols() == \p pos.cols().
   */
CM2MESHTOOLS3D_API int 
interpolate 
   (const DoubleMat& pos, const UIntMat& connectTH4, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleMat& field, const UIntVec& nodes);                             
//@}

}  // end namespace meshtools3d
}  // end namespace cm2

#endif

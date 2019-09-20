/**
   \file       metrics2d.h
   \brief      Declarations of some metric generation routines for 2-D meshes.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_METRICS_2D_H__
#define __CM2_METRICS_2D_H__


namespace cm2 {
namespace meshtools2d {


/****************************************************************************************/
/** @name Anisotropic metrics generation routines on 2-D triangle meshes */
//@{

/**
   Generates a set of 3-D anisotropic metrics for a given 3-D surface triangle mesh.

   For each node of the surface mesh, a 3-D anisotropic metric is computed. \n
   The value for the direction perpendicular to the normal is based on the \p hn parameter. \n
   The values for the directions tangent to the surface use the lengths of the adjacent edges. 

   The new metric at a node of the surface is the intersection metric (minimum metric) 
   between all metrics computed on each adjacent triangles, and the initial metric at this node if any.

   Note that an intersection between a metric M and a null metric (6 null values) gives metric M. \n
   The null metric is the identity for the intersection operation.

   \param[in]     pos         The coordinates matrix (3XNODS). 
   \param[in]     connectT3   The connectivity of the triangle mesh as a 3xNE matrix.
   \param[in]     hn          This parameter controls the value of the metric along the normal to the surface mesh.
                              The value along the normal to the elements is defined by:
                                 -  `hn`, when `hn > 0`.
                                 -  `|hn| * ht`, when `hn < 0`.  
                                 -  `ht`, when `hn = 0`. \n
                                    With `ht` = the average surface mesh size computed using the length of the 
                                    adjacent edges. This gives an isotropic metric.   
   \param[in,out] M           The array of metrics. Resized to \p pos.cols() if shorter. 
                              Only the columns related to nodes in \p connectT3 are affected.                              

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   \note          Edges with null length are discarded.   
   \pre           \p pos.rows() >= 3.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           \p connectT3.rows() == 3 or \p connectT3 is empty.
   \see           meshtools1d::metrics_gen_aniso3d
   */
CM2MESHTOOLS2D_API int 
metrics_gen_aniso3d (const DoubleMat& pos, const UIntMat& connectT3, double hn, DoubleMat& M);                          

//@}



/****************************************************************************************/
/** @name Interpolation of isotropic metrics on 2-D meshes */
//@{

/**
   Interpolates a field of isotropic metrics defined on a planar mesh.

   \param[in]     pos            The 2-D coordinates matrix (2xNODS). 
   \param[in]     connectM       The connectivity matrix of the mesh (3xNEFS or 4xNEFS).
   \param[in]     neighbors      The neighbors matrix (3xNEFS or 4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectT3,
                                 the vector gives one face connected to it.
                                 For nodes not in \p connectT3, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The isotropic metrics.
                                 Only the metrics for the nodes not in the mesh will be computed.
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. positive) unchanged.
   \param[in]     FE_type        The type of the elements (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 2.
   \pre        connectM.rows() == 3 (i.e. triangle mesh), connectM.rows() == 4 (all-quad or mixed mesh) or empty.
   \pre        metrics.size() == pos.cols().
   */
CM2MESHTOOLS2D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectM, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleVec& metrics, bool invalid_metrics_only, cm2::element_type FE_type);                          


/**
   Interpolates a field of isotropic metrics defined on a planar mesh.

   The interpolated metrics are computed on a specific set of nodes that are not in the mesh.

   \param[in]     pos            The 2-D coordinates matrix (2xNODS). 
   \param[in]     connectM       The connectivity matrix of the mesh (3xNEFS).
   \param[in]     neighbors      The neighbors matrix (3xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectM,
                                 the vector gives one face connected to it.
                                 For nodes not in \p connectM, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The isotropic metrics.
   \param[in]     nodes          The node IDs for which the metrics must be computed.
                                 Metrics of nodes already in the mesh are not recomputed.
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. positive) unchanged.
   \param[in]     FE_type        The type of the elements (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 2.
   \pre        connectM.rows() == 3 (i.e. triangle mesh), connectM.rows() == 4 (all-quad or mixed mesh) or empty.
   */
CM2MESHTOOLS2D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectM, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleVec& metrics, const UIntVec& nodes, bool invalid_metrics_only, cm2::element_type FE_type);  

//@}


/****************************************************************************************/
/** @name Interpolation of anisotropic metrics on 2-D meshes */
//@{

/**
   Interpolates a field of 2-D or 3-D anisotropic metrics defined on a planar mesh.

   \param[in]     pos            The coordinates matrix (2xNODS). 
   \param[in]     connectM       The connectivity matrix of the mesh (3xNEFS or 4xNEFS).
   \param[in]     neighbors      The neighbors matrix (3xNEFS or 4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectM,
                                 the vector gives one face connected to it.
                                 For nodes not in \p connectM, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The 2-D or 3-D anisotropic metrics (3xNODS or 6xNODS).
                                 Only the metrics for the nodes not in the mesh will be computed.
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. definite positive) unchanged.
   \param[in]     FE_type        The type of the elements (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 2.
   \pre        connectM.rows() == 3 (i.e. triangle mesh), connectM.rows() == 4 (all-quad or mixed mesh) or empty.
   \pre        metrics.rows() == 3 or 6.
   \pre        metrics.cols() == pos.cols().
   */
CM2MESHTOOLS2D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectM, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleMat& metrics, bool invalid_metrics_only, cm2::element_type FE_type);                          


/**
   Interpolates a field of 2-D or 3-D anisotropic metrics defined on a planar mesh.

   The interpolated metrics are computed on a specific set of nodes that are not in the mesh.

   \param[in]     pos            The coordinates matrix (2xNODS). 
   \param[in]     connectM       The connectivity matrix of the mesh (3xNEFS or 4xNEFS).
   \param[in]     neighbors      The neighbors matrix (3xNEFS or 4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectM,
                                 the vector gives one face connected to it.
                                 For nodes not in \p connectM, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The 2-D or 3-D anisotropic metrics (3xNODS or 6xNODS).
   \param[in]     nodes          The node IDs for which the metrics must be computed.
                                 Metrics of nodes already in the mesh are not recomputed.
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. definite positive) unchanged.
   \param[in]     FE_type        The type of the elements (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 2.
   \pre        connectM.rows() == 3 (i.e. triangle mesh), connectM.rows() == 4 (all-quad or mixed mesh) or empty.
   \pre        metrics.rows() == 3 or 6.
   \pre        metrics.cols() == pos.cols().
   */
CM2MESHTOOLS2D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectM, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleMat& metrics, const UIntVec& nodes, bool invalid_metrics_only, cm2::element_type FE_type);                          

//@}


}  // end namespace meshtools2d
}  // end namespace cm2

#endif

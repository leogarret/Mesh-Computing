/**
   \file       metrics3d.h
   \brief      Declarations of some metric generation routines for 3-D meshes.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_METRICS_3D_H__
#define __CM2_METRICS_3D_H__


namespace cm2 {
namespace meshtools3d {


/****************************************************************************************/
/** @name Interpolation of isotropic metrics on 3-D meshes */
//@{

/**
   Interpolates a field of isotropic metrics defined on a tetrahedral mesh.

   \param[in]     pos            The 3-D coordinates matrix (3xNODS). 
   \param[in]     connectTH4     The connectivity matrix of the tetrahedral mesh (4xNEFS).
   \param[in]     neighbors      The neighbors matrix (4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectTH4,
                                 the vector gives one tetrahedron connected to it.
                                 For nodes not in \p connectTH4, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The isotropic metrics.
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. strictly positive) unchanged.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 3.
   \pre        connectTH4.rows() == 4 (i.e. tetrahedral mesh).
   \pre        metrics.size() == pos.cols().
   */
CM2MESHTOOLS3D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectTH4, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleVec& metrics, bool invalid_metrics_only);                          


/**
   Interpolates a field of isotropic metrics defined on a tetrahedral mesh

   The interpolated metrics are computed on a specific set of nodes.

   \param[in]     pos            The 3-D coordinates matrix (3xNODS). 
   \param[in]     connectTH4     The connectivity matrix of the tetrahedral mesh (4xNEFS).
   \param[in]     neighbors      The neighbors matrix (4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectTH4,
                                 the vector gives one tetrahedron connected to it.
                                 For nodes not in \p connectTH4, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The isotropic metrics.
   \param[in]     nodes          The node IDs for which the metrics must be computed.
                                 Metrics for nodes of the mesh are not recomputed.
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. strictly positive) unchanged.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 3.
   \pre        connectTH4.rows() == 4 (i.e. tetrahedral mesh).
   \pre        metrics.size() == pos.cols().
   */
CM2MESHTOOLS3D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectTH4, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleVec& metrics, const UIntVec& nodes, bool invalid_metrics_only);                          

//@}


/****************************************************************************************/
/** @name Interpolation of anisotropic metrics on 3-D meshes */
//@{

/**
   Interpolates a field of 3-D anisotropic metrics defined on a tetrahedral mesh.

   \param[in]     pos            The 3-D coordinates matrix (3xNODS). 
   \param[in]     connectTH4     The connectivity matrix of the tetrahedral mesh (4xNEFS).
   \param[in]     neighbors      The neighbors matrix (4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectTH4,
                                 the vector gives one tetrahedron connected to it.
                                 For nodes not in \p connectTH4, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The 3-D anisotropic metrics (6xNODS).
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. definite positive) unchanged.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 3.
   \pre        connectTH4.rows() == 4 (i.e. tetrahedral mesh).
   \pre        metrics.rows() == 6.
   \pre        metrics.cols() == pos.cols().
   */
CM2MESHTOOLS3D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectTH4, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleMat& metrics, bool invalid_metrics_only);                          


/**
   Interpolates a field of 3-D anisotropic metrics defined on a tetrahedral mesh.
   The interpolated metrics are computed on a specific set of nodes.

   \param[in]     pos            The 3-D coordinates matrix (3xNODS). 
   \param[in]     connectTH4     The connectivity matrix of the tetrahedral mesh (4xNEFS).
   \param[in]     neighbors      The neighbors matrix (4xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     ancestors      The ancestors vector. For each node referenced in \p connectTH4,
                                 the vector gives one tetrahedron connected to it.
                                 For nodes not in \p connectTH4, the ancestor should be `CM2_NONE` (i.e. `unsigned(-1)`).
                                 Can be computed by the meshtools::get_ancestors function.
   \param[in,out] metrics        The 3-D anisotropic metrics (6xNODS).
   \param[in]     nodes          The node IDs for which the metrics must be computed.
                                 Metrics for nodes of the mesh are not recomputed.
   \param[in]     invalid_metrics_only    Flag to recompute only the invalid metrics,
                                 and leave the valid ones (i.e. definite positive) unchanged.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        pos.rows() == 3.
   \pre        connectTH4.rows() == 4 (i.e. tetrahedral mesh).
   \pre        metrics.rows() == 6.
   \pre        metrics.cols() == pos.cols().
   */
CM2MESHTOOLS3D_API int 
interpolate_metrics 
   (const DoubleMat& pos, 
    const UIntMat& connectTH4, const UIntMat& neighbors, const UIntVec& ancestors,
    DoubleMat& metrics, const UIntVec& nodes, bool invalid_metrics_only);                          

//@}

}  // end namespace meshtools3d
}  // end namespace cm2

#endif

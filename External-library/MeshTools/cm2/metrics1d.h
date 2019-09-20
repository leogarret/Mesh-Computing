/**
   \file       metrics1d.h
   \brief      Declarations of some metric generation routines for 1D meshes.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_METRICS_1D_H__
#define __CM2_METRICS_1D_H__


namespace cm2 {
namespace meshtools1d {

/****************************************************************************************/
/** @name Anisotropic metrics generation routines along 1-D meshes */
//@{

/**
   Generates 2-D anisotropic metrics for a given set of edges.

   For each edge, a 2-D anisotropic metric is computed based on the normal to the edge,
   the \p hn parameter and the length of the edge.\n 
   Then, the intersection metrics (minimum metric) with the input metrics (if any) at both vertices is computed to 
   define the new metrics.

   \param[in]     pos            The coordinates matrix. 
   \param[in]     connectE       The connectivity of the 1-D mesh as a 2xNE.
   \param[in]     hn             This parameter controls the metric along the normal to the mesh. \n
                                 The metric along the normal to the elements is set to:
                                    -  hn, when hn > 0 (constant size in the normal direction).
                                    -  ht, when hn == 0 
                                       (ht is the tangent mesh size computed using the length of the edges). 
                                       This case makes an isotropic metric.   
                                    -  |hn| * ht, when hn < 0.  
   \param[in,out] M              The array of metrics. Resized to pos.cols() if shorter. \n
                                 Only the columns related to nodes in \p connectE are affected.\n
                                 The new metrics are the intersection metrics (minimum metrics) between the computed metrics
                                 and the input metrics (if any).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \note          Edges with null length are discarded.
   \note          Intersection between a metric M and a null metric (3 null values) gives metric M.
                  (i.e. the null metric is the identity metric for the intersection operation).

   \pre           \p pos.rows() >= 2.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           \p connectE.rows() = 2 or 3, or connectE is empty.
   */
CM2MESHTOOLS1D_API int 
metrics_gen_aniso2d (const DoubleMat& pos, const UIntMat& connectE, double hn, DoubleMat& M);                          


/**
   Generates 2-D anisotropic metrics for a given set of nodes.

   For each node, a 2-D anisotropic metric is computed based on the \p hn and \p ht parameters
   and the normal to this node.\n 
   Then, the intersection metric (minimum metric) with the input metric (if any) at this node is computed to 
   define the new metric.

   \param[in]     normals        The normals to the nodes (normalized to 1). 
   \param[in]     node_IDs       The nodes for which the metric must be computed. \n
                                 Only the normals associated to these IDs are used and
                                 only the metrics associated to these IDs are affected.
   \param[in]     hn             The size along the normal to the nodes.  
   \param[in]     ht             The size along the perpendicular to the normal to the nodes.  
   \param[in,out] M              The array of metrics. Resized to \p normals.cols() if shorter. \n
                                 Only the columns related to the nodes in \p indices are affected.\n
                                 The new metrics are the intersection metrics (minimum metrics) between the computed metrics
                                 and the input metrics (if any).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \note          Nodes with null normal are discarded.
   \note          Intersection between a metric M and a null metric (3 null values) gives metric M.
                  (i.e. the null metric is the identity metric for the intersection operation).

   \pre           \p normals.rows() = 2.
   \pre           All vectors in \p normals should be normalized to 1.
   \pre           All the values in the \p node_IDs array are smaller than \p normals.cols().
   */
CM2MESHTOOLS1D_API int 
metrics_gen_aniso2d (const DoubleMat& normals, const UIntVec& node_IDs, double hn, double ht, DoubleMat& M);                          


/**
   Generates 3-D anisotropic metrics for a given line mesh.

   For each node in the line mesh, a 3-D anisotropic metric is computed based on the \p hn parameter
   and on the size of the adjacent edges.\n 
   Then, the intersection metric (minimum metric) with the input metric (if any) at this node is computed to 
   define the new metric.

   \param[in]     pos            The coordinates matrix (dimensions 3xNODS). 
   \param[in]     connectE       The connectivity of the 1-D mesh as a 2xNE.
   \param[in]     hn             This parameter controls the metric along the normal to the mesh. \n
                                 The metric along the normals to the elements is set to:
                                    -  hn, when hn > 0 (constant size in the normal directions).
                                    -  ht, when hn == 0 
                                       (ht is the tangent mesh size computed using the length of the edges). 
                                       This case makes an isotropic metric.   
                                    -  |hn| * ht, when hn < 0.  
   \param[in,out] M              The array of metrics. Resized to pos.cols() if shorter. \n
                                 Only the columns related to nodes in \p connectE are affected.\n
                                 The new metrics are the intersection metrics (minimum metrics) between the computed metrics
                                 and the input metrics (if any).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \note          Edges with null length are discarded.
   \note          Intersection between a metric M and a null metric (3 null values) gives metric M.
                  (i.e. the null metric is the identity metric for the intersection operation).

   \pre           \p pos.rows() >= 3.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           \p connectE.rows() = 2 or 3, or \p connectE is empty.
   \see           meshtools3d::metrics_gen_aniso3d.
   */
CM2MESHTOOLS1D_API int 
metrics_gen_aniso3d (const DoubleMat& pos, const UIntMat& connectE, double hn, DoubleMat& M);                          


/**
   Generates 3-D anisotropic metrics for a given line mesh (different sizes along normal and bi-normal).

   For each node in the line mesh, a 3-D anisotropic metric is computed based on the \p hn and \p hb parameters
   and on the size of the adjacent edges.\n 
   Then, the intersection metric (minimum metric) with the input metric (if any) at this node is computed to 
   define the new metric.

   \param[in]     pos            The coordinates matrix (dimensions 3xNODS). 
   \param[in]     connectE       The connectivity of the 1-D mesh as a 2xNE.
   \param[in]     hn             This parameter controls the metric along the normal N1 to the mesh 
                                 (normal in the curvature direction). \n
                                 The metric along the normal to the elements is set to:
                                    -  hn, when hn > 0 (constant size in the normal directions).
                                    -  ht, when hn == 0 
                                       (ht is the tangent mesh size computed using the length of the edges). 
                                       This case makes an isotropic metric.   
                                    -  |hn| * ht, when hn < 0.  
   \param[in]     hb             This parameter controls the metric along the normal N2 to the mesh
                                 (bi-normal defined as N2 = T0 x N1). \n
                                 The metric along the normal to the elements is set to:
                                    -  hb, when hb > 0 (constant size in the normal directions).
                                    -  ht, when hb == 0 
                                       (ht is the tangent mesh size computed using the length of the edge). 
                                       This case makes an isotropic metric.   
                                    -  |hb| * ht, when hb < 0.  
   \param[in,out] M              The array of metrics. Resized to pos.cols() if shorter. \n
                                 Only the columns related to nodes in \p connectE are affected.\n
                                 The new metrics are the intersection metrics (minimum metrics) between the computed metrics
                                 and the input metrics (if any).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \warning       Experimental.
   \note          Edges with null length are discarded.
   \note          In case of null curvature, \p hb is considered equal to \p hn.
   \note          Intersection between a metric M and a null metric (3 null values) gives metric M.
                  (i.e. the null metric is the identity metric for the intersection operation).

   \pre           \p pos.rows() >= 3.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           \p connectE.rows() = 2 or 3, or \p connectE is empty.
   \see           meshtools3d::metrics_gen_aniso3d.
   */
CM2MESHTOOLS1D_API int 
metrics_gen_aniso3d (const DoubleMat& pos, const UIntMat& connectE, double hn, double hb, DoubleMat& M);                          

//@}


/****************************************************************************************/
/** @name Interpolation of metrics on 1-D meshes */
//@{

/**
   Interpolates a field of scalar metrics on a line mesh.

   \param[in]     Us             The curvilinear coordinates of the points where the \p Smetrics is defined.
   \param[in]     Smetrics       The input metrics.
   \param[in]     Ug             The curvilinear coordinates of the points where the \p Gmetrics should
                                 be computed.
   \param[out]    Gmetrics       The output metrics. \n
                                 \p Gmetrics[i] at curvilinear coordinates \p Ug[i] is computed as the 
                                 interpolation between the two \p Smetrics values bracketing \p Ug[i].

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        \p Us.size() == \p Smetrics.size().
   \post       \p Ug.size() <= \p Gmetrics.size().
   */
CM2MESHTOOLS1D_API int 
interpolate_metrics (const DoubleVec& Us, const DoubleVec& Smetrics, const DoubleVec& Ug, DoubleVec& Gmetrics);


/**
   Interpolates a field of tensorial metrics on a line mesh.

   \param[in]     Us             The curvilinear coordinates of the points where the \p Smetrics is defined.
   \param[in]     Smetrics       The input metrics.
   \param[in]     Ug             The curvilinear coordinates of the points where the \p Gmetrics should
                                 be computed.
   \param[out]    Gmetrics       The output metrics. \n
                                 \p Gmetrics[i] at curvilinear coordinates \p Ug[i] is computed as the 
                                 interpolation between the two \p Smetrics values bracketing \p Ug[i].

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   
   \pre        \p Us.size() == \p Smetrics.size().
   \post       \p Ug.size() <= \p Gmetrics.size().
   */
CM2MESHTOOLS1D_API int 
interpolate_metrics (const DoubleVec& Us, const DoubleMat& Smetrics, const DoubleVec& Ug, DoubleMat& Gmetrics);

//@}

}  // end namespace meshtools1d
}  // end namespace cm2

#endif

#ifndef __CM2_METRICS_H__
#define __CM2_METRICS_H__

/**
   \file       metrics.h
   \brief      Declarations of some metric-specific routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/


namespace cm2 {
namespace meshtools {


/****************************************************************************************/
/** @name Isometric metrics generation based on edge lengths */
//@{

/**
   Generates isotropic metrics on the nodes of a given mesh (1-D, 2-D or 3-D).

   For each node in the mesh, a metric (i.e. mesh size) is computed as a mean of the 
   lengths of the adjacent edges. 
   
   \f$
      H_i = { \left( {\frac{1}{N_i}} ~ \sum_{edges\;ij} {{L_{ij}}^w} \right) } ^ {\frac{1}{w}}
   \f$

   where:

      \f$ H_i \f$ = averaged metric at node i.

      \f$ L_{ij} \f$ = length of edge ij.

      \f$ N_i \f$ = number of edges ij connected to node i (counted with multiplicity).


   \param[in]     pos            The coordinates matrix. 
   \param[in]     connectM       The connectivity of the mesh (only linear elements are allowed).
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page). \n
                                 Only linear types are allowed.
   \param[in,out] H              The vector of metrics. Resized to \p pos.cols() if shorter. \n
                                 Only the indices related to nodes in \p connectM are affected. \n
                                 Others are left unchanged.
   \param[in]     w              The type of averaging used to compute the values in \p H.
                                 For instance:
                                    -  w = 0.0      => min value                         
                                    -  w = 1.0      => arithmetic mean
                                    -  w = 2.0      => geometric mean
                                    -  w = DBL_MAX  => max value  

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \note          Multiplicity: edges shared by N elements are taken into account N times.
   \note          Edges with null length are discarded.

   \pre           \p pos.rows() == 2 or 3, or \p pos is empty.
   \pre           \p connectM.rows() is compatible with \p FE_type, or \p connectM is empty.
   \pre           All the values in \p connectM are smaller than \p pos.cols().
   \pre           \p w >= 0.
   */
CM2MESHTOOLS_API int 
metrics_gen_iso 
   (const DoubleMat& pos, const UIntMat& connectM, cm2::element_type FE_type, 
    DoubleVec& H, double w);                          

//@}



/****************************************************************************************/
/** @name Routines to check the validity of isotropic and anisotropic metrics */
//@{

/**
   Tests the validity of several isotropic metrics.

   \param[in]     sizes       The vector of isotropic metrics.\n
                              For node `i`, `sizes[i]` is valid when `sizes[i] > 0`.

   \return        The first index `i` such that `sizes[nodes[i]]` is invalid.\n
                  Returns sizes.size() if all metrics are valid.
   */
CM2MESHTOOLS_API unsigned
first_invalid_metric (const DoubleVec& sizes);

/**
   Tests the validity of several isotropic metrics.

   \param[in]     nodes       The node IDs for which the metrics must be tested.
   \param[in]     sizes       The vector of isotropic metrics.\n
                              For node `i`, `sizes[i]` is valid when `sizes[i] > 0`.

   \return        The first index `i` such that `sizes[nodes[i]]` is invalid.\n
                  Returns `nodes.size()` if all metrics of \p nodes are valid.
   */
CM2MESHTOOLS_API unsigned
first_invalid_metric (const UIntVec& nodes, const DoubleVec& sizes);

/**
   Tests the validity of several isotropic metrics.

   \param[in]     sizes       The vector of isotropic metrics.
   \return        true when the metric associated to each node is valid (i.e. > 0).
   */
CM2MESHTOOLS_API bool
are_valid_metrics (const DoubleVec& sizes);

/**
   Tests the validity of several isotropic metrics.

   \param[in]     nodes       The node IDs for which the metrics must be tested.
   \param[in]     sizes       The vector of isotropic metrics.
   \return        true when the metric associated to each node is valid (i.e. > 0).
   */
CM2MESHTOOLS_API bool
are_valid_metrics (const UIntVec& nodes, const DoubleVec& sizes);

/**
   Tests the validity of a 2-D anisotropic metric.

   \param[in]     M2          The 2-D anisotropic metric as a 2x2 symmetric matrix.
                              The metric is valid when it has two strictly positive eigen values.
   \return        true when the metric is valid.
   */
CM2MESHTOOLS_API bool 
is_valid_metric (const DoubleSym2& M2);  

/**
   Tests the validity of a 3-D anisotropic metric.

   \param[in]     M3          The 3-D anisotropic metric as a 3x3 symmetric matrix.
                              The metric is valid when it has two strictly positive eigen values.
   \return        true when the metric is valid.
   */
CM2MESHTOOLS_API bool 
is_valid_metric (const DoubleSym3& M3);  

/**
   Tests the validity of several anisotropic metrics.

   \param[in]     metrics     The 2-D (resp. 3D) anisotropic metrics as a 3xN (resp. 6xN) matrix. \n
                              metrics.col(i) stores the 3 (resp. 6) scalars for the 2x2 (resp. 3x3) symmetric matrix at node `i`
                              in a column-major order. \n
                              For instance, in 2-D, `metrics.col(i)` stores the 3 scalars for the 2x2 symmetric matrix `Mi` at node `i` as:
                              \verbatim
                                         |mxx  mxy |
                                    Mi = |         |
                                         |mxy  myy |
                              with
                                    mxx = metrics(0,i)
                                    mxy = metrics(1,i)
                                    myy = metrics(2,i)
                              \endverbatim
                              metrics.col(i) is valid when all its eigen values are strictly positive.
   \return        The first index `i` such that `metrics.col(i)` is invalid. \n
                  Returns `metrics.cols()` if all metrics are valid.

   \pre           \p metrics.rows() == 3 (dim = 2) or \p metrics.rows() == 6 (dim = 3).
   */
CM2MESHTOOLS_API unsigned
first_invalid_metric (const DoubleMat& metrics);

/**
   Tests the validity of several anisotropic metrics (2-D or 3-D).

   \param[in]     nodes       The node IDs for which the metrics must be tested.
   \param[in]     metrics     The 2-D (resp. 3D) anisotropic metrics as a 3xN (resp. 6xN) matrix. \n
                              metrics.col(i) stores the 3 (resp. 6) scalars for the 2x2 (resp. 3x3) symmetric matrix at node `i`
                              in a column-major order. \n
                              For instance, in 2-D, `metrics.col(i)` stores the 3 scalars for the 2x2 symmetric matrix `Mi` at node `i` as:
                              \verbatim
                                         |mxx  mxy |
                                    Mi = |         |
                                         |mxy  myy |
                              with
                                    mxx = metrics(0,i)
                                    mxy = metrics(1,i)
                                    myy = metrics(2,i)
                              \endverbatim
                              metrics.col(i) is valid when all its eigen values are strictly positive.
   \return        The first index `i` such that `metrics.col(nodes[i])` is invalid. \n
                  Returns `nodes.size()` if all metrics of \p nodes are valid.

   \pre           \p metrics.rows() == 3 (dim = 2) or \p metrics.rows() == 6 (dim = 3).
   */
CM2MESHTOOLS_API unsigned
first_invalid_metric (const UIntVec& nodes, const DoubleMat& metrics);

/**
   Tests the validity of several anisotropic metrics.

   \param[in]     metrics     The 2-D (resp. 3D) anisotropic metrics as a 3xN (resp. 6xN) matrix.
   \return        true when the metric associated to each node is valid (i.e. matrix is positive-definite).
   */
CM2MESHTOOLS_API bool
are_valid_metrics (const DoubleMat& metrics);

/**
   Tests the validity of several anisotropic metrics.

   \param[in]     nodes       The node IDs for which the metrics must be tested.
   \param[in]     metrics     The 2-D (resp. 3D) anisotropic metrics as a 3xN (resp. 6xN) matrix.
   \return        true when the metric associated to each node is valid (i.e. matrix is positive-definite).
   */
CM2MESHTOOLS_API bool
are_valid_metrics (const UIntVec& nodes, const DoubleMat& metrics);

/**
   Finds the valid metrics (aniso only).

   \param[in]     metrics     The 2-D (resp. 3D) anisotropic metrics as a 3xN (resp. 6xN) matrix.
   \param[in,out] valid_IDs   The columns IDs in \p metrics with valid (i.e. positive-definite) matrix
                              are appended.
   */
CM2MESHTOOLS_API void
valid_metrics (const DoubleMat& metrics, UIntVec& valid_IDs);

//@}


/****************************************************************************************/
/** @name Metrics bounding */
//@{

/**
   Computes the minimum 2-D metric between two metrics (also called _intersection_ metric).

   This intersection metric is computed as the largest ellipse contained in both ellipses associated with the two metrics.

   Note that an intersection between a metric M and a null metric (3 null values, giving an infinite ellipse) gives metric M.
   The null metric is the identity metric for the intersection operation.

   \param[in]     M0          The first 2-D metric.
   \param[in]     M1          The second 2-D metric.
   \param[out]    Mmin        The resulting minimum metric.
   \return        true when \p Mmin has been properly computed (i.e. when matrix \p M0 was positive-definite).\n
                  Otherwise \p Mmin is returned as \ p M0.
   */
CM2MESHTOOLS_API bool
minimum_metric (const DoubleSym2& M0, const DoubleSym2& M1, DoubleSym2& Mmin);

/**
   Computes the minimum 3-D metric between two metrics (also called _intersection_ metric).
   
   This intersection metric is computed as the largest H-ellipse contained in both ellipses associated with the two metrics.

   Note that an intersection between a metric M and a null metric (6 null values, giving an infinite ellipse) gives metric M.

   The null metric is the identity metric for the intersection operation.

   \param[in]     M0          The first 3-D metric.
   \param[in]     M1          The second 3-D metric.
   \param[out]    Mmin        The resulting minimum metric.
   \return        true when \p Mmin has been properly computed (i.e. when matrix \p M0 was positive-definite).\n
                  Otherwise \p Mmin is returned as \ p M0.
   */
CM2MESHTOOLS_API bool
minimum_metric (const DoubleSym3& M0, const DoubleSym3& M1, DoubleSym3& Mmin);

/**
   Computes the maximum 2-D metric between two metrics (also called _union_ metric).

   This union metric is computed as the smallest H-ellipse containing both ellipses associated with the two metrics.

   Note that an union between a metric M and an infinite metric (2 diagonal `DBL_MAX` values) gives metric M.

   The infinite metric is the identity metric for the union operation.

   \param[in]     M0          The first 2-D metric.
   \param[in]     M1          The second 2-D metric.
   \param[out]    Mmax        The resulting maximum metric.
   \return        true when \p Mmax has been properly computed (i.e. when matrix \p M0 was
                  positive-definite).\n
                  Otherwise \p Mmax is returned as \ p M0.
   */
CM2MESHTOOLS_API bool
maximum_metric (const DoubleSym2& M0, const DoubleSym2& M1, DoubleSym2& Mmax);

/**
   Computes the maximum 3-D metric between two metrics (also called _union_ metric).
   
   This union metric is computed as the smallest H-ellipse containing both ellipses associated with the two metrics.

   Note that an union between a metric M and an infinite metric (3 diagonal `DBL_MAX` values) gives metric M.
   The infinite metric is the identity metric for the union operation.

   \param[in]     M0          The first 3-D metric.
   \param[in]     M1          The second 3-D metric.
   \param[out]    Mmax        The resulting maximum metric.
   \return        true when \p Mmax has been properly computed (i.e. when matrix \p M0 was positive-definite).\n
                  Otherwise \p Mmax is returned as \ p M0.
   */
CM2MESHTOOLS_API bool
maximum_metric (const DoubleSym3& M0, const DoubleSym3& M1, DoubleSym3& Mmax);

/**
   Bounds (clamps) a 2-D metric between two metrics.

   \param[in]     M2             The 2-D metric to be bound.
                                 M2 is replaced with the union metric with \p min_h (isotropic metric), then
                                 M2 is replaced with the intersection metric with \p max_h (isotropic metric).
   \param[in]     min_h          The minimum H value.
   \param[in]     max_h          The maximum H value.
   \return        Error code. Number of bounded metrics when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          No check that \p min_h <= \p max_h.
   */
CM2MESHTOOLS_API int
bound_metric (DoubleSym2& M2, double min_h, double max_h);

/**
   Bounds (clamps) a 3-D metric between two metrics.

   \param[in]     M3             The 3-D metric to be bound.
                                 M3 is replaced with the union metric with \p min_h (isotropic metric), then
                                 M3 is replaced with the intersection metric with \p max_h (isotropic metric).
   \param[in]     min_h          The minimum H value.
   \param[in]     max_h          The maximum H value.
   \return        Error code. Number of bounded metrics when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          No check that \p min_h <= \p max_h.
   */
CM2MESHTOOLS_API int
bound_metric (DoubleSym3& M3, double min_h, double max_h);

/**
   Bounds (clamps) the metrics in a vector (isotropic case).

   \param[in,out] H              The vector of isotropic metrics to be modified.
                                 `H[i]` is replaced with \p min_h if `H[i] < min_h`, then
                                 `H[i]` is replaced with \p max_h if `H[i] > max_h`.
   \param[in]     min_h          The minimum H value.
   \param[in]     max_h          The maximum H value.
   \return        Error code. Number of bounded metrics when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          No check that \p min_h <= \p max_h.

   */
CM2MESHTOOLS_API int
bound_metrics (DoubleVec& H, double min_h, double max_h);

/**
   Bounds (clamps) the metrics in a vector (isotropic case) and returns the bounded node IDs.

   \param[in,out] H              The vector of isotropic metrics to be modified.
                                 `H[i]` is replaced with \p min_h if `H[i] < min_h`, then
                                 `H[i]` is replaced with \p max_h if `H[i] > max_h`.
   \param[in]     min_h          The minimum H value.
   \param[in]     max_h          The maximum H value.
   \param[out]    capped         The vector of the modified node IDs.
                                 Emptied upon entry.
   \return        Error code. Number of bounded metrics when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          No check that \p min_h <= \p max_h.
   */
CM2MESHTOOLS_API int
bound_metrics (DoubleVec& H, double min_h, double max_h, UIntVec& capped);

/**
   Bounds (clamps) the metrics in a matrix (anisotropic case).

   \param[in,out] M              The matrix of anisotropic metrics to be modified.
                                 `M.col(i)` is replaced with the union metric with \p min_h (isotropic metric), then
                                 `M.col(i)` is replaced with the intersection metric with \p max_h (isotropic metric).
   \param[in]     min_h          The minimum H value.
   \param[in]     max_h          The maximum H value.
   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           M.rows() == 3 or 6 (or M empty).
   \note          No check that \p min_h <= \p max_h.
   */
CM2MESHTOOLS_API int
bound_metrics (DoubleMat& M, double min_h, double max_h);

/**
   Bounds the metrics in a matrix (anisotropic case) and returns the bounded node IDs.

   \param[in,out] M              The matrix of anisotropic metrics to be modified.
                                 `M.col(i)` is replaced with the union metric with \p min_h (isotropic metric), then
                                 `M.col(i)` is replaced with the intersection metric with \p max_h (isotropic metric).
   \param[in]     min_h          The minimum H value.
   \param[in]     max_h          The maximum H value.
   \param[out]    capped         The vector of the modified node IDs.
                                 Emptied upon entry.
   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           M.rows() == 3 or 6 (or M empty).
   \note          No check that \p min_h <= \p max_h.
   */
CM2MESHTOOLS_API int
bound_metrics (DoubleMat& M, double min_h, double max_h, UIntVec& capped);

/**
   Bounds the metrics in a mesh (isotropic case) so that the H-variations (i.e. H-gradients)
   along all edges are below a given value (max gradation).
   
   Along an edge \f$ N_0 N_1 \f$, the H-variation is defined by the following formula:

      \f$ \mbox{H-variation} (N_0 N_1) = |h_1 - h_0| / L \f$

   where:
      - \f$ h_0 \f$ = Metric at node 0 (isotropic case).
      - \f$ h_1 \f$ = Metric at node 1 (isotropic case).
      - \f$ L \f$ = length of segment \f$ N_0 N_1 \f$.

   \param[in]     pos            The coordinates matrix.
   \param[in]     connect        The connectivity matrix of the mesh.
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page).
   \param[in]     nodeIDs        The nodes for which the metric shall not (resp. shall) be changed 
                                 when \p hard_flag = true (resp. false).
   \param[in]     hard_flag      Flag to tell whether the metric of the nodes in \p nodeIDs must be 
                                 considered as fixed and all others can be changed (true) 
                                 or the opposite (false).
   \param[in]     sgn            The allowed "direction" of metric changes:
                                    - If `sgn = -1`, metrics are allowed to reduce only.
                                    - If `sgn = +1`, metrics are allowed to increase only.
                                    - If `sgn = 0`, metrics are allowed to increase and reduce.
   \param[in]     max_gradation  The max allowable H-variation.
                                 If max_gradation = `DBL_MAX` no metric bounding is performed.
   \param[in,out] metrics        The metric at each node.

   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           All the values in hard_nodes are smaller than \p pos.cols().
   \pre           max_gradation >= 0.
   \pre           metrics.size() >= pos.cols().

   \note          Due to the hard nodes constraints, some edges 
                  may still have a H-variations above the threshold upon exit.
   */
CM2MESHTOOLS_API int
bound_metric_variations
   (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type,
    const UIntVec& nodeIDs, bool hard_flag, int sgn, double max_gradation, DoubleVec& metrics);

/**
   Bounds the metrics in a mesh (anisotropic case) so that the H-variations (i.e. H-gradients)
   along all edges are below a given value (max gradation).
   
   \param[in]     pos            The coordinates matrix.
   \param[in]     connect        The connectivity matrix of the mesh.
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page).
   \param[in]     nodeIDs        The nodes for which the metric shall not (resp. shall) be changed 
                                 when \p hard_flag = true (resp. false).
   \param[in]     hard_flag      Flag to tell whether the metric of the nodes in \p nodeIDs must be 
                                 considered as fixed and all others can be changed (true) 
                                 or the opposite (false).
   \param[in]     sgn            The allowed "direction" of metric changes:
                                    - If `sgn = -1`, metrics are allowed to reduce only.
                                    - If `sgn = +1`, metrics are allowed to increase only.
                                    - If `sgn = 0`, metrics are allowed to increase and reduce.
   \param[in]     max_gradation  The max allowable H-variation.
                                 If max_gradation = `DBL_MAX` no metric bounding is performed.
   \param[in,out] metrics        The metric at each node.

   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           All the values in hard_nodes are smaller than \p pos.cols().
   \pre           \p max_gradation >= 0.
   \pre           \p metrics.cols() >= \p pos.cols().
   \pre           (\p metrics.rows() == 3 and \p pos.rows() == 2) or 
                  (\p metrics.rows() == 6 and \p pos.rows() == 3) or \p metrics empty.

   \note          Due to the hard nodes constraints, some edges 
                  may still have a H-variation above the threshold upon exit.
   */
CM2MESHTOOLS_API int
bound_metric_variations 
   (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type,
    const UIntVec& nodeIDs, bool hard_flag, int sgn, double max_gradation, DoubleMat& metrics);

/**
   Bounds the metrics in a mesh (isotropic case) so that the H-shocks along all edges
   are below a given value (max gradation).
   
   Along an edge \f$ N_0 N_1 \f$, the H-shock is defined by the following formula:

      \f$ \mbox{H-shock} (N_0 N_1) = \max(h_0/h_1,h_1/h_0)^{1/L} - 1 \f$

   where:
      - \f$ h_0 \f$ = Metric at node 0 (isotropic case).
      - \f$ h_1 \f$ = Metric at node 1 (isotropic case).
      - \f$ L \f$ = length of segment \f$ N_0 N_1 \f$ with respect to metrics \f$ h_0 \f$ and \f$ h_1 \f$.

   \param[in]     pos            The coordinates matrix.
   \param[in]     connect        The connectivity matrix of the mesh.
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page).
   \param[in]     nodeIDs        The nodes for which the metric shall not (resp. shall) be changed 
                                 when \p hard_flag = true (resp. false).
   \param[in]     hard_flag      Flag to tell whether the metric of the nodes in \p nodeIDs must be 
                                 considered as fixed and all others can be changed (true) 
                                 or the opposite (false).
   \param[in]     sgn            The allowed "direction" of metric changes:
                                    - If `sgn = -1`, metrics are allowed to reduce only.
                                    - If `sgn = +1`, metrics are allowed to increase only.
                                    - If `sgn = 0`, metrics are allowed to increase and reduce.
   \param[in]     max_gradation  The max allowable H-shock. \n
                                 If max_gradation = `DBL_MAX` no metric bounding is performed.
   \param[in,out] metrics        The metric at each node.

   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           All the values in hard_nodes are smaller than \p pos.cols().
   \pre           \p max_gradation >= 0.
   \pre           \p metrics.size() >= \p pos.cols().

   \note          Due to the hard nodes constraints, some edges 
                  may still have a H-shock above the threshold upon exit.
   */
CM2MESHTOOLS_API int
bound_metric_gradations 
   (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type,
    const UIntVec& nodeIDs, bool hard_flag, int sgn, double max_gradation, DoubleVec& metrics);

/**
   Bounds the metrics in a mesh (anisotropic case) so that the H-shocks along all edges
   are below a given value (max gradation).
   
   \param[in]     pos            The coordinates matrix.
   \param[in]     connect        The connectivity matrix of the mesh.
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page).
   \param[in]     nodeIDs        The nodes for which the metric shall not (resp. shall) be changed 
                                 when \p hard_flag = true (resp. false).
   \param[in]     hard_flag      Flag to tell whether the metric of the nodes in \p nodeIDs must be 
                                 considered as fixed and all others can be changed (true) 
                                 or the opposite (false).
   \param[in]     sgn            The allowed "direction" of metric changes:
                                    - If `sgn = -1`, metrics are allowed to reduce only.
                                    - If `sgn = +1`, metrics are allowed to increase only.
                                    - If `sgn = 0`, metrics are allowed to increase and reduce.
   \param[in]     max_gradation  The max allowable H-shock.
                                 If max_gradation = `DBL_MAX` no metric bounding is performed.
   \param[in,out] metrics        The metric at each node.

   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           All the values in hard_nodes are smaller than \p pos.cols().
   \pre           \p max_gradation >= 0.
   \pre           \p metrics.cols() >= \p pos.cols().
   \pre           (\p metrics.rows() == 3 and \p pos.rows() == 2) or 
                  (\p metrics.rows() == 6 and \p pos.rows() == 3) or \p metrics empty.

   \note          Due to the hard nodes constraints, some edges 
                  may still have a H-shock above the threshold upon exit.
   */
CM2MESHTOOLS_API int
bound_metric_gradations 
   (const DoubleMat& pos, const UIntMat& connect, cm2::element_type FE_type,
    const UIntVec& nodeIDs, bool hard_flag, int sgn, double max_gradation, DoubleMat& metrics);

//@}


/****************************************************************************************/
/** @name Eigen pairs of metrics */
//@{

/**
   Computes the eigen values (increasing order) and eigen vectors of a 2x2 symmetric matrix.

   Solves \f$A X = \lambda X\f$.

   \param[in ]    A              The 2x2 symmetric matrix.
   \param[out]    eigenvalues    The eigen values (from smallest to largest).
   \param[out]    eigenvectors   The eigen vectors (normalized) associated to the eigen values, stored column-wise.
   \return        Error flag.    True when the eigen-pairs have been successfully computed.
                                 False otherwise.

   \post    The eigen vectors are normalized and orthogonal.
   \post    \p eigenvalues[0] <= \p eigenvalues[1]

   \note    This function works also for non positive matrices, but bear in mind that
            anisotropic metrics must have only _strictly-positive_ eigen values.
   */
CM2MESHTOOLS_API bool
eigen_pairs (const DoubleSym2& A, DoubleVec2& eigenvalues, DoubleMat2x2& eigenvectors);

/**
   Computes the eigen pairs (eigen vectors and eigen values) of the matrix A-1 * B.

   Solves \f$B X = \lambda A X\f$.

   \param[in ]    A              First 2x2 symmetric matrix.
   \param[in ]    B              Second 2x2 symmetric matrix.
   \param[out]    eigenvalues    The eigen values (from smallest to largest).
   \param[out]    eigenvectors   The eigen vectors (normalized) associated to the eigen values, stored column-wise.
   \return        Error flag.    True when the eigen-pairs have been successfully computed.
                                 False otherwise.

   \pre     A must be invertible.
   \post    The eigen vectors are normalized but _not_ orthogonal.
   \post    \p eigenvalues[0] <= \p eigenvalues[1]

   \note    This function works also for non positive matrices, but bear in mind that
            anisotropic metrics must have only _strictly-positive_ eigen values.
   */
CM2MESHTOOLS_API bool
eigen_pairs (const DoubleSym2& A, const DoubleSym2& B, DoubleVec2& eigenvalues, DoubleMat2x2& eigenvectors);

/**
   Computes the eigen values (increasing order) and eigen vectors of a 3x3 symmetric definitive positive 
   matrix.

   Solves \f$A X = \lambda X\f$.

   \param[in ]    A              The 3x3 symmetric matrix.
   \param[out]    eigenvalues    The eigen values (from smallest to largest).
   \param[out]    eigenvectors   The eigen vectors (normalized) associated to the eigen values, stored column-wise.
   \return        Error flag.    True when the eigen-pairs have been successfully computed.
                                 False otherwise.

   \post    The eigen vectors are normalized and orthogonal.
   \post    \p eigenvalues[0] <= \p eigenvalues[1] <= \p eigenvalues[2]

   \note    The eigen vectors are orthonormal but the determinant can be equal to -1 (not always a positive trihedron).
   \note    This function works also for non positive matrices, but bear in mind that
            anisotropic metrics must have only _strictly-positive_ eigen values.
   */
CM2MESHTOOLS_API bool
eigen_pairs (const DoubleSym3& A, DoubleVec3& eigenvalues, DoubleMat3x3& eigenvectors);

/**
   Computes the eigen pairs (eigen vectors and eigen values) of the matrix A-1 * B.

   Solves \f$B X = \lambda A X\f$.

   \param[in ]    A              First 3x3 symmetric matrix.
   \param[in ]    B              Second 3x3 symmetric matrix.
   \param[out]    eigenvalues    The eigen values (from smallest to largest).
   \param[out]    eigenvectors   The eigen vectors (normalized) associated to the eigen values, stored column-wise.
   \return        Error flag.    True when the eigen-pairs have been successfully computed.
                                 False otherwise.

   \pre     A must be invertible.
   \post    The eigen vectors are normalized but _not_ orthogonal.
   \post    \p eigenvalues[0] <= \p eigenvalues[1] <= \p eigenvalues[2]

   \note    This function works also for non positive matrices, but bear in mind that
            anisotropic metrics must have only _strictly-positive_ eigen values.
   */
CM2MESHTOOLS_API bool
eigen_pairs (const DoubleSym3& A, const DoubleSym3& B, DoubleVec3& eigenvalues, DoubleMat3x3& eigenvectors);

//@}



/****************************************************************************************/
/** @name Transformation of metrics */
//@{

/**
   Converts an isotropic metric to a 2-D anisotropic metric.

   The equivalent 2-D anisotropic metric is computed as:

   \verbatim
        -        -
       |  1       |
       | ---   0  |
       | h*h      |
   M = |          |
       |       1  |
       |  0   --- |
       |      h*h |
        -        -
   \endverbatim

   \param[in]     h        The isotropic value.
   \param[out]    M2       The equivalent 2-D anisotropic matrix.
   */
CM2MESHTOOLS_API void
metric_iso_to_aniso (double h, DoubleSym2& M2);

/**
   Converts an isotropic metric to a 3-D anisotropic metric.

   The equivalent 3-D anisotropic metric is computed as:

   \verbatim
        -             -
       |  1            |
       | ---   0    0  |
       | h*h           |
       |               |
       |       1       |
   M = |  0   ---   0  |
       |      h*h      |
       |               |
       |            1  |
       |  0    0   --- |
       |           h*h |
        -             -
   \endverbatim

   \param[in]     h           The isotropic value.
   \param[out]    M3          The equivalent 3-D anisotropic matrix.
   */
CM2MESHTOOLS_API void
metric_iso_to_aniso (double h, DoubleSym3& M3);

/**
   Converts isotropic metrics to 2-D anisotropic metrics.

   \param[in]     H              The isotropic values.
   \param[in,out] metrics2D      The resulting 2-D anisotropic matrices, stored column-wise,
                                 each computed as in meshtools::metric_iso_to_aniso2d:
                                 \verbatim
                                           M2 for node #i --
                                                            |
                                                            v
                                                 -                     -
                                                |          MXX          |
                                   metrics2D =  |   ...    MXY    ...   |
                                                |          MYY          |
                                                 -                     -
                                                            ^
                                                            |
                                                   col #i --
                                 \endverbatim
   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note    Values in \p metrics2D (even valid metrics) are overwritten upon exit.
            This is a difference with overwrite_invalid_metrics.
   \pre     \p metrics2D.rows() == 3.
   \pre     \p metrics2D.cols() >= \p H.size().
   */
CM2MESHTOOLS_API int
metrics_iso_to_aniso2d (const DoubleVec& H, DoubleMat& metrics2D);

/**
   Converts isotropic metrics to 3-D anisotropic metrics.

   \param[in]     H              The isotropic values.
   \param[in,out] metrics3D      The resulting 3-D anisotropic matrices, stored column-wise,
                                 each computed as in meshtools::metric_iso_to_aniso2d:
                                 \verbatim
                                           M2 for node #i --
                                                            |
                                                            v
                                                 -                     -
                                                |          MXX          |
                                   metrics3D =  |   ...    MXY    ...   |
                                                |          MYY          |
                                                |          MXZ          |
                                                |          MYZ          |
                                                |          MZZ          |
                                                 -                     -
                                                            ^
                                                            |
                                                   col #i --
                                 \endverbatim
   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note    Values in \p metrics3D (even valid metrics) are overwritten upon exit.
            This is a difference with overwrite_invalid_metrics.
   \pre     \p metrics3D.rows() == 6.
   \pre     \p metrics3D.cols() >= \p H.size().
   */
CM2MESHTOOLS_API int
metrics_iso_to_aniso3d (const DoubleVec& H, DoubleMat& metrics3D);

/**
   Overwrites the invalid metrics in a vector by the valid values from another one.

   \param[in,out] H           The vector of metrics to be modified.
                              `H[i]` is replaced with `default_H[i]` if `H[i]` is negative or null
                              and if `default_H[i]` is strictly positive.
   \param[in]     default_H   The vector of metrics to replace.
   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.
   */
CM2MESHTOOLS_API int
overwrite_invalid_metrics (DoubleVec& H, const DoubleVec& default_H);

/**
   Overwrites the invalid anisotropic metrics in a matrix by the equivalent metrics
   made with the valid values from a vector.

   \param[in,out] M              The matrix of anisotropic metrics to be modified.
                                 If invalid, `M.col(i)` is replaced with the equivalent anisotropic metric
                                 computed with `default_H[i]` if `default_H[i]` is strictly positive.
   \param[in]     default_H      The vector of metrics to replace.
   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p M.rows() == 3 (2-D anisotropic) or \p M.rows() == 6 (3-D anisotropic) or \p M is empty.
   \see           meshtools::metric_iso_to_aniso, meshtools::is_valid_metric.
   */
CM2MESHTOOLS_API int
overwrite_invalid_metrics (DoubleMat& M, const DoubleVec& default_H);

/**
   Overwrites the invalid anisotropic metrics in a matrix by the valid metrics from another one.

   \param[in,out] M              The matrix of anisotropic metrics to be modified.
                                 `M.col(i)` is replaced with `default_M.col(i)`
                                 if `M.col(i)` is invalid and if `default_M.col(i)` is valid.
   \param[in]     default_M      The matrix of metrics to replaced.

   \return        Error code. Zero when OK. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p M.rows() = \p default_M.rows()
   \pre           \p M.cols() = \p default_M.cols()
   \pre           \p M.rows() == 3 (2-D anisotropic) or \p M.rows() == 6 (3-D anisotropic) or \p M is empty.
   \see           meshtools::metric_iso_to_aniso, meshtools::is_valid_metric.
   */
CM2MESHTOOLS_API int
overwrite_invalid_metrics (DoubleMat& M, const DoubleMat& default_M);

/**
   Transforms an isotropic metric into a 2-D anisotropic metric using a 3x2 transformation matrix.

   This routine can be used for surface meshing, using the local basis as the transformation matrix.

   \param[in]     h              The isotropic value.
   \param[in]     B2             The 3x2 transformation matrix.
   \param[out]    M2             The resulting 2-D anisotropic matrix computed as:
                                    M2 = 1/(h*h) * transpose(B2) % B2

   \note    if \p h <= 0, \p M2 is set to 0.
   \note    In order to get a valid 2-D anisotropic metric (i.e. definite positive),
            the transformation matrix \p B2 should not be singular (no null column, two columns not collinear).
   */
CM2MESHTOOLS_API void
metric_iso_to_aniso2d (double h, const DoubleMat3x2& B2, DoubleSym2& M2);

/**
   Transforms a batch of isotropic metrics into 2-D anisotropic metrics using 
   3x2 transformation (non-orthonormal) matrices.

   This routine can be used for surface meshing, using the local basis as the transformation matrix.

   \param[in]     metrics_iso    The isotropic values.
   \param[in]     local_bases    The 3x2 transformation matrices, as a 6xN matrix.\n
                                 The transformation matrices are stored column-wise.\n
                                 The transformation matrix for node `i` is stored in column `local_bases.col(i)`:
                                 the first 3 values are the first column and the next three values 
                                 are the second column.
                                 \verbatim
                                               B2 for node #i --
                                                                |
                                                                v
                                                    -                       -
                                                   |           V0X           |
                                                   |           V0Y           |
                                  local_bases  =   |   ...     V0Z     ...   |
                                                   |           V1X           |
                                                   |           V1Y           |
                                                   |           V1Z           |
                                                    -                       -
                                                                ^
                                                                |
                                                       col #i --
                                 \endverbatim
                                 The two columns should not be null nor collinear (but can be non-orthogonal and not normalized).
   \param[in,out]    metrics2D   The resulting 2-D anisotropic matrices, stored column-wise
                                 and each computed as in meshtools::metric_iso_to_aniso2d.
                                 \verbatim
                                           M2 for node #i --
                                                            |
                                                            v
                                                 -                     -
                                                |          MXX          |
                                   metrics2D =  |   ...    MXY    ...   |
                                                |          MYY          |
                                                 -                     -
                                                            ^
                                                            |
                                                   col #i --
                                 \endverbatim
                                 The initial metrics are overwritten.
   \return     The error code. \n
               Returns a negative value when an error was detected.\n
               Returns 0 when all transformations have been successfully computed.
           
   \note    The transformation matrices must be stored column-wise in \p local_bases.                        
   \note    In order to get a valid 2-D anisotropic metric (i.e. definite positive),
            the matching transformation matrix in \p local_bases should not be singular 
            (no null column, two columns not collinear), and the matching isotropic scalar
            should be strictly positive.

   \pre     \p local_bases.cols() == \p metrics_iso.size()
   \pre     \p metrics2D.rows() == 3.
   \pre     \p metrics2D.cols() >= \p metrics_iso.size().
   */
CM2MESHTOOLS_API int
metrics_iso_to_aniso2d (const DoubleVec& metrics_iso, const DoubleMat& local_bases, DoubleMat& metrics2D);

/**
   Transforms a 3-D anisotropic metric into a 2-D anisotropic metric using a 3x2 transformation matrix.

   This routine can be used for surface meshing, using the local basis as the transformation matrix.

   \param[in]     M3       The 3-D anisotropic metric.
   \param[in]     B2       The 3x2 transformation matrix.
   \param[out]    M2       The resulting 2-D anisotropic matrix computed as:
                              `M2 = transpose(B2) % M3 % B2`

   \note    In order to get a valid 2-D anisotropic metric (i.e. definite positive),
            the transformation matrix \p B2 should not be singular (no null column, two columns not collinear).
   \note    The validity (i.e. definite-positive) of the input 3-D anisotropic metric \p M3 is not checked.
            When not valid, the resulting \p M2 matrix is likely to be also invalid.
   */
CM2MESHTOOLS_API void
metric_aniso3d_to_aniso2d 
   (const DoubleSym3& M3, const DoubleMat3x2& B2, DoubleSym2& M2);

/**
   Transforms a batch of 3-D anisotropic metrics into 2-D anisotropic metrics using 3x2 transformation matrices.

   This routine can be used for surface meshing, using the local basis as the transformation matrix.

   \param[in]     metrics3D         The 3-D anisotropic metrics stored column-wise (dimensions 6xN).
   \param[in]     local_bases       The 3x2 transformation matrices (dimensions 6xN).
   \param[in,out] metrics2D         The resulting 2-D anisotropic matrices stored column-wise (dimensions 3xN),
                                    each computed as in meshtools::metric_aniso3d_to_aniso2d.\n
                                    The initial metrics are overwritten.
   \return     The error code. \n
               Returns 0 when all transformations have been successfully computed.\n
               Returns a negative value when an error was detected.\n
               Returned value = -k => the k-th argument had an illegal value.
        
   \note       The transformation matrices are stored column-wise in \p local_bases.
   \note       In order to get a valid 2-D anisotropic metric (i.e. definite positive),
               the matching 3-D metric must be valid and the transformation matrix 
               not singular (no null column, two columns not collinear).
   \pre        \p metrics3D.rows() == 6.
   \pre        \p metrics2D.rows() == 3.
   \pre        \p local_bases.cols() == \p metrics3D.cols().
   \pre        \p metrics2D.cols() >= \p metrics3D.cols().
   */
CM2MESHTOOLS_API int
metrics_aniso3d_to_aniso2d 
   (const DoubleMat& metrics3D, const DoubleMat& local_bases, DoubleMat& metrics2D);

//@}


}  // end namespace meshtools
}  // end namespace cm2

#endif

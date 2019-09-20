/**
   \file       mesh_spline.h
   \brief      Definitions of some mesh creation routines on splines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3500 $
   $Date: 2019-02-14 16:46:02 +0100 (jeu., 14 févr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_SPLINE_H__
#define __CM2_MESH_SPLINE_H__


namespace cm2 {
namespace meshtools1d {

/****************************************************************************************/
/** @name Mesh splines */
//@{

/**
   Compute a point's coordinates on a spline (2-D version).
   
   The spline is a G1 cubic curve defined by two summits (\p ib and \p ic) and two tangents. \n
   The first and the last points (\p ia and \p id) are used to define the tangents at the beginning 
   and at the end of the spline.

   These tangents are computed as follow:
      - At the beginning of the spline (i.e. at point \p ib), 
        the tangent is in the direction of (pos.col(ic) - pos.col(ia)).
      - At the end of the spline (i.e. at point \p ic), 
        the tangent is in the direction of (pos.col(id) - pos.col(ib)).

   If the index of the first node equals to the second, the tangent at the beginning is free. \n
   If the index of the last node equals the one before, the tangent at the end is free.

   \param[in]     pos               The coordinates matrix. New points are appended.
   \param[in]     ia                The index of the first point that define the spline.
                                    Used to compute the tangent at \p ib.
   \param[in]     ib                The index of the first point of the spline.
   \param[in]     ic                The index of the last point of the spline.
   \param[in]     id                The index of the last point that define the spline.
                                    Used to compute the tangent at \p ic.
   \param[in]     s                 The curvilinear parameter (between 0 and 1) of the point to compute.
   \param[out]    Ps                The coordinates of the point with curvilinear parameter equal to \p s.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
                  
   \note       Loops are avoided by decreasing the degree of the spline (i.e. by setting free one
               or two tangents).
               
   \pre        Dimension = 2 (\p pos.rows() == 2).
   \pre        \p ia < \p pos.cols()
   \pre        \p ib < \p pos.cols()
   \pre        \p ic < \p pos.cols()
   \pre        \p id < \p pos.cols()
   */
CM2MESHTOOLS1D_API int 
spline_point (const DoubleMat& pos, unsigned ia, unsigned ib, unsigned ic, unsigned id, double s, 
              DoubleVec2& Ps);  

/**
   Compute a point's coordinates on a spline (3-D version).
   
   See previous function (2-D version).
   
   \param[in]     pos               The coordinates matrix. New points are appended.
   \param[in]     ia                The index of the first point that define the spline.
                                    Used to compute the tangent at \p ib.
   \param[in]     ib                The index of the first point of the spline.
   \param[in]     ic                The index of the last point of the spline.
   \param[in]     id                The index of the last point that define the spline.
                                    Used to compute the tangent at \p ic.
   \param[in]     s                 The curvilinear parameter (between 0 and 1) of the point to compute.
   \param[out]    Ps                The coordinates of the point with curvilinear parameter equal to \p s.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       Loops are avoided by decreasing the degree of the spline (i.e. by setting free one
               or two tangents).

               \pre        Dimension = 3 (\p pos.rows() == 3).
   \pre        \p ia < \p pos.cols()
   \pre        \p ib < \p pos.cols()
   \pre        \p ic < \p pos.cols()
   \pre        \p id < \p pos.cols()
   */
CM2MESHTOOLS1D_API int 
spline_point (const DoubleMat& pos, unsigned ia, unsigned ib, unsigned ic, unsigned id, double s, 
              DoubleVec3& Ps);  

/**
   Creates a mesh on a poly-spline with specific isotropic sizes defined at the spline points.
   
   The spline is a made of pieces of G1 cubic curves going through all points but the first and the last. \n
   If N points are specified in the parameter \p spline_nodes, the poly-spline goes through points 
   spline_nodes[1], spline_nodes[2] ... spline_nodes[N-2]. \n
   The first and the last points are used to define the tangents at the beginning and at the end of the poly-spline.

   These tangents are computed as follow:
      - At the beginning of the poly-spline (i.e. at spline_nodes[1], 
        the tangent is in the direction of (pos.col(spline_nodes[2]) - pos.col(spline_nodes[0]))
      - At the end of the poly-spline (i.e. at spline_nodes[N-2], 
        the tangent is in the direction of (pos.col(spline_nodes[N-1]) - pos.col(spline_nodes[N-3]))

   If the index of the first node equals to the second, the tangent at the beginning is free. \n
   If the index of the last node equals the one before, the tangent at the end is free.

   \param[in,out] pos               The coordinates matrix. New points are appended.
   \param[in]     spline_nodes      The index of the nodes that define the splines.
   \param[in]     spline_metrics    The mesh sizes at the poly-spline nodes. \n
                                    The metric associated to the i-th spline node is defined by `spline_metrics[i]`. \n
                                    The values for the first spline node and the last spline node are not used. 
   \param[in]     force_even        Flag to force the global number of created edges on the poly-spline to be even. \n
                                    Useful when the quadmesh::mesher is to be used.
   \param[in]     min_n             The minimum number of edges that should be created along the whole poly-spline.
   \param[in]     max_n             The maximum number of edges that should be created along the whole poly-spline.
   \param[in]     max_chordal_err   The maximum allowable chordal error (i.e. distance) between the mesh and the poly-spline:
                                       - If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                         The distance between the elements and the poly-spline will be bounded by
                                         \p max_chordal_error or the size specified by the user in \p spline_metrics, whichever is lower.
                                       - If \p max_chordal_error < 0, the chordal error is considered as _relative_ 
                                         with respect to the min curvature radius along the poly-spline. \n
                                         The distance between the elements and the poly-spline will be bounded
                                         by - \p max_chordal_error * min (curvature radius)
                                         or the size specified by the user in \p spline_metrics, whichever is lower.
                                    The chordal error control is disabled when \p max_chordal_error = 0 or +-DBL_MAX.
   \param[in]     min_h             Minimum mesh size due to the chordal control (security). 
                                    The mesh size will not be reduced (by the \p max_chordal_err criterion) beyond this limit.
   \param[in]     target_metric     The target mesh size in the middle of the spline.\n
                                    The mesh sizes tend to converge to this value (if higher than the value
                                    caused by \p max_chordal_err and the \p min_h threshold) in the middle of the spline
                                    under control of \p max_gradation.\n
                                    Mostly useful when the poly-spline is made of a single spline 
                                    (i.e. \p spline_nodes.size() == 4).
   \param[in]     max_gradation     The max mesh gradation. The lower, the smoother size gradations. \n
                                    \p max_gradation = 0.5 usually gives good results. \n
                                    Set \p max_gradation = `DBL_MAX` to disable any gradation control.
   \param[in,out] indices           The indices of the nodes are appended, beginning with `spline_nodes[1]`, 
                                    then new nodes created on the whole poly-spline, then `spline_nodes[NS-2]` 
                                    (with `NS = spline_nodes.size()`).
   \param[in,out] Us                The curvilinear coordinates between 0 and 1 of the nodes associated to the \p indices
                                    (appended in same order as in \p indices). \n
                                    New appended parameters are sorted ascendingly. \n
                                    Starts with a parameter equal to 0.0 for the first node, then the parameters 
                                    for the intermediate nodes, and finally the value 1.0 for the last node.
   \param[in,out] metrics           The mesh sizes at the nodes in \p indices (appended in same order as in \p indices). \n
                                    Note that values given in \p spline_metrics may be reduced by the chordal control if any.
   \param[out]    G_distance        The max distance between the poly-spline and the broken line defined by the \p spline_nodes.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          At least one edge (two when \p force_even == `true`) is created on the whole poly-spline.
   \note          Loops are avoided on a spline-by-spline basis (but the whole poly-spline may have loops).
   \warning       Do not forget to add a node index at the beginning of \p spline_nodes to define the first tangent
                  (equal to the first spline point if the tangent is free).
   \warning       Do not forget to add a node index at the end of \p spline_nodes to define the last tangent
                  (equal to the last spline point if the tangent is free).

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        \p spline_nodes.size() >= 4.
   \pre        All the values referenced in \p spline_metrics are > 0.
   \pre        \p min_n > 0
   \pre        \p min_n <= \p max_n
   \post       Appended values in \p Us are sorted ascendingly, between 0 and 1.

   \see        mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_spline (DoubleMat& pos, const UIntVec& spline_nodes, const DoubleVec& spline_metrics, 
             bool force_even, unsigned min_n, unsigned max_n, 
             double max_chordal_err, double min_h, double target_metric, double max_gradation, 
             UIntVec& indices, DoubleVec& Us, DoubleVec& metrics, 
             double& G_distance);  


/**
   Creates a mesh on a poly-spline with specific anisotropic sizes.
   
   See previous function (isotropic version).

   \param[in,out] pos               The coordinates matrix. New points are appended.
   \param[in]     spline_nodes      The index of the nodes that define the spline.
   \param[in]     spline_metrics    The metrics at the poly-spline nodes. \n
                                    The metric associated to the i-th spline node is defined by `spline_metrics.col(i)`. \n
                                    The metrics for the first spline node and the last spline node are not used. 
   \param[in]     force_even        Flag to force the global number of created edges on the poly-spline to be even. \n
                                    Useful when the quadmesh::mesher is to be used.
   \param[in]     min_n             The minimum number of edges that should be created along the whole poly-spline.
   \param[in]     max_n             The maximum number of edges that should be created along the whole poly-spline.
   \param[in]     max_chordal_err   The maximum allowable chordal error (i.e. distance) between the mesh and the poly-spline:
                                       - If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                         The distance between the elements and the poly-spline will be bounded by
                                         \p max_chordal_error or the size specified by the user in \p spline_metrics, whichever is lower.
                                       - If \p max_chordal_error < 0, the chordal error is considered as _relative_ 
                                         with respect to the min curvature radius along the poly-spline. \n
                                         The distance between the elements and the poly-spline will be bounded 
                                         by - \p max_chordal_error * min (curvature radius)
                                         or the size specified by the user in \p spline_metrics, whichever is lower.
                                    The chordal error control is disabled when max_chordal_error = 0 or +-DBL_MAX.
   \param[in]     min_h             Minimum mesh size due to the chordal control (security) . 
                                    The mesh size will not be reduced (by the \p max_chordal_err criterion) beyond this limit.
   \param[in]     target_metric     The target mesh size in the middle of the spline.\n
                                    The mesh sizes tend to converge to this value (if higher than the value
                                    caused by \p max_chordal_err and the \p min_h threshold) in the middle of the spline
                                    under control of \p max_gradation.\n
                                    Mostly useful when the poly-spline is made of a single spline 
                                    (i.e. \p spline_nodes.size() == 4).
   \param[in]     max_gradation     The max mesh gradation. The lower, the smoother size gradations. \n
                                    \p max_gradation = 0.5 usually gives good results. \n
                                    Set \p max_gradation = `DBL_MAX` to disable any gradation control.
   \param[in,out] indices           The indices of the nodes are appended, beginning with `spline_nodes[1]`, 
                                    then new nodes created on the whole poly-spline, then `spline_nodes[NS-2]` 
                                    (with `NS = spline_nodes.size()`).
   \param[in,out] Us                The curvilinear coordinates between 0 and 1 of the nodes associated to the \p indices
                                    (appended in same order as in \p indices). \n
                                    New appended parameters are sorted ascendingly. \n
                                    Starts with a parameter equal to 0.0 for the first node, then the parameters 
                                    for the intermediate nodes, and finally the value 1.0 for the last node.
   \param[in,out] metrics           The metrics at the nodes in \p indices (appended in same order as in \p indices). \n
                                    Note that values given in \p spline_metrics may be reduced by the chordal control if any.
   \param[out]    G_distance        The max distance between the poly-spline and the broken line defined by the spline nodes.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          At least one edge (two when \p force_even == `true`) is created on the whole poly-spline.
   \note          Loops are avoided on a spline-by-spline basis (but the whole poly-spline may have loops).
   \warning       Do not forget to add a node index at the beginning of \p spline_nodes to define the first tangent
                  (equal to the first spline point if the tangent is free).
   \warning       Do not forget to add a node index at the end of \p spline_nodes to define the last tangent
                  (equal to the last spline point if the tangent is free).

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        \p spline_nodes.size() >= 4.
   \pre        All columns referenced in \p spline_metrics are valid (i.e. two strictly positive eigen-values).
   \pre        \p min_n > 0
   \pre        \p min_n <= \p max_n
   \pre        \p max_gradation >= 0.
   \post       Appended values in \p Us are sorted ascendingly, between 0 and 1.

   \see        mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_spline (DoubleMat& pos, const UIntVec& spline_nodes, const DoubleMat& spline_metrics, 
             bool force_even, unsigned min_n, unsigned max_n, 
             double max_chordal_err, double min_h, double target_metric, double max_gradation, 
             UIntVec& indices, DoubleVec& Us, DoubleMat& metrics, 
             double& G_distance);  
//@}

}  // end namespace meshtools1d
}  // end namespace cm2

#endif

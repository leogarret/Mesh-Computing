/**
   \file       mesh_line.h
   \brief      Definitions of some mesh creation routines on lines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3500 $
   $Date: 2019-02-14 16:46:02 +0100 (jeu., 14 févr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_LINE_H__
#define __CM2_MESH_LINE_H__


namespace cm2 {
namespace meshtools1d {

/****************************************************************************************/
/** @name Generates meshes along lines with internal specific sizes */
//@{

/**
   Creates a mesh along a line defined by several geometric points and isotropic metrics. 

   This function can be used when the internal points are not hard nodes and don't need to be present
   in the mesh.

   \param[in,out] pos               The coordinates matrix. New points are appended.
   \param[in]     geo_nodes         The geometric nodes that approximate the line. \n
                                    These points must be located along the line _successively_. \n
                                    The first and the last node in this array are the extremities of the line. \n
                                    Must _not_ be empty. Must contains at least two nodes (the extremities of the line).
   \param[in]     metricsG          The mesh sizes. \n
                                    The size associated to the i-th geo node is defined by \p metricsG[i]. \n
                                    The considered metrics may be reduced by the chordal control if any.
   \param[in]     force_even        Flag to force the number of created edges to be even. \n
                                    Useful when the quadmesh::mesher is to be used.
   \param[in]     min_n             The minimum number of edges that should be created along the line.
   \param[in]     max_n             The maximum number of edges that should be created along the line.
   \param[in]     max_chordal_error The maximum allowable chordal error (i.e. distance) between the mesh and the line:
                                       - If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                         The distance between the elements and the initial line will be bounded by
                                         \p max_chordal_error or the size specified by the user in \p metricsG, whichever is lower.
                                       - If \p max_chordal_error < 0, the chordal error is considered as _relative_ 
                                         with respect to the curvature radius along the initial line (approximative discrete curvature). \n
                                         The distance between the elements and the initial line will be bounded
                                         by - \p max_chordal_error * curvature radius
                                         or the size specified by the user in \p metricsG, whichever is lower.
                                    The chordal error control is disabled when \p max_chordal_error = 0 or +-DBL_MAX.
   \param[in]     min_h             Minimum mesh size due to the chordal control (security) . 
                                    The mesh size will not be reduced (by the \p max_chordal_err criterion) beyond this limit.
   \param[in,out] indices           The indices of the nodes are appended, beginning with `spline_nodes[1]`, 
                                    then new nodes created on the whole poly-spline, then `spline_nodes[NS-2]` 
                                    (with `NS = spline_nodes.size()`).
   \param[in,out] Us                The curvilinear coordinates between 0 and 1 of the nodes associated to the \p indices
                                    (appended in same order as in \p indices). \n
                                    New appended parameters are sorted ascendingly. \n
                                    Starts with a parameter equal to 0.0 for the first node, then the parameters 
                                    for the intermediate nodes, and finally the value 1.0 for the last node.
   \param[in,out] metrics           The metrics at the nodes in \p indices (appended in same order as in \p indices).
                                 
   \note          When successful, at least one edge (two when \p force_even == true) is created on the line. 
   \note          The new nodes are located on the initial polyline.
   
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2 or 3).
   \pre        \p geo_nodes.size() >= 2
   \pre        \p geo_nodes.size() = \p metricsG.size()
   \pre        All the values in \p metricsG are > 0.
   \pre        \p min_n > 0
   \pre        \p min_n <= \p max_n
   \post       Appended values in \p Us are sorted ascendingly, between 0 and 1.

   \see        mesh_straight, extrude_translate, extrude_rotate, mesh_spline.
   */
CM2MESHTOOLS1D_API int 
mesh_line (DoubleMat& pos, 
           const UIntVec& geo_nodes, const DoubleVec& metricsG,
           bool force_even, unsigned min_n, unsigned max_n, double max_chordal_error, double min_h,
           UIntVec& indices, DoubleVec& Us, DoubleVec& metrics);  


/**
   Creates a mesh along a line defined by several geometric points and isotropic metrics. 

   Same as above with specific curvilinear coordinates.

   \param[in,out] pos               The coordinates matrix. New points are appended.
   \param[in]     geo_nodes         The geometric nodes that approximate the line. \n
                                    These points must be located along the line _successively_. \n
                                    The first and the last node in this array are the extremities of the line. \n
                                    Must _not_ be empty. Must contains at least two nodes (the extremities of the line).
   \param[in]     UG                The curvilinear coordinates (sorted ascendingly) of the geo_nodes along the polyline.
   \param[in]     metricsG          The mesh sizes. \n
                                    The size associated to the i-th geo node is defined by \p metricsG.col(i). \n
                                    The considered metrics may be reduced by the chordal control if any.
   \param[in]     force_even        Flag to force the number of created edges to be even. \n
                                    Useful when the quadmesh::mesher is to be used.
   \param[in]     min_n             The minimum number of edges that should be created along the line.
   \param[in]     max_n             The maximum number of edges that should be created along the line.
   \param[in]     max_chordal_error The maximum allowable chordal error (i.e. distance) between the mesh and the line:
                                       - If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                         The distance between the elements and the initial line will be bounded by
                                         \p max_chordal_error or the size specified by the user in \p metricsG, whichever is lower.
                                       - If \p max_chordal_error < 0, the chordal error is considered as _relative_ 
                                         with respect to the curvature radius along the initial line (approximative discrete curvature). \n
                                         The distance between the elements and the initial line will be bounded 
                                         by - \p max_chordal_error * curvature radius
                                         or the size specified by the user in \p metricsG, whichever is lower.
                                    The chordal error control is disabled when \p max_chordal_error = 0 or +-DBL_MAX.
   \param[in]     min_h             Minimum mesh size due to the chordal control (security) . 
                                    The mesh size will not be reduced (by the \p max_chordal_err criterion) beyond this limit.
   \param[in,out] indices           The indices of the nodes are appended, beginning with `spline_nodes[1]`, 
                                    then new nodes created on the whole poly-spline, then `spline_nodes[NS-2]` 
                                    (with `NS = spline_nodes.size()`).
   \param[in,out] Us                The curvilinear coordinates between 0 and 1 of the nodes associated to the \p indices
                                    (appended in same order as in \p indices). \n
                                    New appended parameters are sorted ascendingly. \n
                                    Starts with a parameter equal to 0.0 for the first node, then the parameters 
                                    for the intermediate nodes, and finally the value 1.0 for the last node.
   \param[in,out] metrics           The metrics at the nodes in \p indices (appended in same order as in \p indices).
                                 
   \note          When successful, at least one edge (two when \p force_even == true) is created on the line. 
   \note          The new nodes are located on the initial polyline.
   
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2 or 3).
   \pre        \p geo_nodes.size() >= 2
   \pre        \p geo_nodes.size() = \p metricsG.size()
   \pre        \p UG.size() = \p geo_nodes.size()
   \pre        Values in UG are sorted increasingly.
   \pre        All the values in \p metricsG are > 0.
   \pre        \p min_n > 0
   \pre        \p min_n <= \p max_n
   \post       Appended values in \p Us are sorted ascendingly, between 0 and 1.

   \see        mesh_straight, extrude_translate, extrude_rotate, mesh_spline.
   */
CM2MESHTOOLS1D_API int 
mesh_line (DoubleMat& pos, 
           const UIntVec& geo_nodes, const DoubleVec& UG, const DoubleVec& metricsG,
           bool force_even, unsigned min_n, unsigned max_n, double max_chordal_error, double min_h,
           UIntVec& indices, DoubleVec& Us, DoubleVec& metrics);  


/**
   Creates a mesh along a line defined by several geometric points and anisotropic metrics. 

   This function can be used when the internal points are not hard nodes and don't need to be present
   in the mesh.

   \param[in,out] pos               The coordinates matrix. New points are appended.
   \param[in]     geo_nodes         The geometric nodes that approximate the line. \n
                                    These points must be located along the line _successively_. \n
                                    The first and the last node in this array are the extremities of the line. \n
                                    Must _not_ be empty. Must contains at least two nodes (the extremities of the line).
   \param[in]     metricsG          The metrics. \n
                                    The metric associated to the i-th geo node is defined by \p metricsG.col(i). \n
                                    The considered metrics may be reduced by the chordal control if any.
   \param[in]     force_even        Flag to force the number of created edges to be even.
                                    Useful when the quadmesh::mesher is to be used.
   \param[in]     min_n             The minimum number of edges that should be created along the line.
   \param[in]     max_n             The maximum number of edges that should be created along the line.
   \param[in]     max_chordal_error The maximum allowable chordal error (i.e. distance) between the mesh and the line:
                                       - If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                         The distance between the elements and the initial line will be bounded by
                                         \p max_chordal_error or the size specified by the user in \p metricsG, whichever is lower.
                                       - If \p max_chordal_error < 0, the chordal error is considered as _relative_ 
                                         with respect to the curvature radius along the initial line (approximative discrete curvature). \n
                                         The distance between the elements and the initial line will be bounded
                                         by - \p max_chordal_error * curvature radius
                                         or the size specified by the user in \p metricsG, whichever is lower.
                                    The chordal error control is disabled when \p max_chordal_error = 0 or +-DBL_MAX.
   \param[in]     min_h             Minimum mesh size due to the chordal control (security) . 
                                    The mesh size will not be reduced (by the \p max_chordal_err criterion) beyond this limit.
   \param[in,out] indices           The indices of the nodes are appended, beginning with `spline_nodes[1]`, 
                                    then new nodes created on the whole poly-spline, then `spline_nodes[NS-2]` 
                                    (with `NS = spline_nodes.size()`).
   \param[in,out] Us                The curvilinear coordinates between 0 and 1 of the nodes associated to the \p indices
                                    (appended in same order as in \p indices). \n
                                    New appended parameters are sorted ascendingly. \n
                                    Starts with a parameter equal to 0.0 for the first node, then the parameters 
                                    for the intermediate nodes, and finally the value 1.0 for the last node.
   \param[in,out] metrics           The metrics at the nodes in \p indices (appended in same order as in \p indices).
                                 
   \note          When successful, at least one edge (two when \p force_even == true) is created on the line. 
   \note          The new nodes are located on the initial polyline.
   
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2 or 3).
   \pre        \p geo_nodes.size() >= 2
   \pre        \p geo_nodes.size() = \p metricsG.cols()
   \pre        All metrics must be positive-definite (valid). 
   \pre        \p min_n > 0
   \pre        \p min_n <= \p max_n
   \post       Values in \p new_U are sorted ascendingly, between 0 and 1.

   \see        mesh_straight, extrude_translate, extrude_rotate, mesh_spline.
   */
CM2MESHTOOLS1D_API int 
mesh_line (DoubleMat& pos, 
           const UIntVec& geo_nodes, const DoubleMat& metricsG,
           bool force_even, unsigned min_n, unsigned max_n, double max_chordal_error, double min_h,
           UIntVec& indices, DoubleVec& Us, DoubleMat& metrics);  


/**
   Creates a mesh along a line defined by several geometric points, 
   with an anisotropic metric given at some specific soft nodes. 

   Same as above with specific curvilinear coordinates.

   \param[in,out] pos               The coordinates matrix. New points are appended.
   \param[in]     geo_nodes         The geometric nodes that approximate the line. \n
                                    These points must be located along the line _successively_. \n
                                    The first and the last node in this array are the extremities of the line. \n
                                    Must _not_ be empty. Must contains at least two nodes (the extremities of the line).
   \param[in]     UG                The curvilinear coordinates (sorted ascendingly) of the \p geo_nodes along the polyline.
   \param[in]     metricsG          The metrics. \n
                                    The metric associated to the i-th geo node is defined by \p metricsG.col(i). \n
                                    The considered metrics may be reduced by the chordal control if any.
   \param[in]     force_even        Flag to force the number of created edges to be even. \n
                                    Useful when the quadmesh::mesher is to be used.
   \param[in]     min_n             The minimum number of edges that should be created along the line.
   \param[in]     max_n             The maximum number of edges that should be created along the line.
   \param[in]     max_chordal_error The maximum allowable chordal error (i.e. distance) between the mesh and the line:
                                       - If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                         The distance between the elements and initial the line will be bounded by
                                         \p max_chordal_error or the size specified by the user in \p metricsG, whichever is lower.
                                       - If \p max_chordal_error < 0, the chordal error is considered as _relative_ 
                                         with respect to the curvature radius along the initial line (approximative discrete curvature). \n
                                         The distance between the elements and the initial line will be bounded
                                         by - \p max_chordal_error * curvature radius
                                         or the size specified by the user in \p metricsG, whichever is lower.
                                    The chordal error control is disabled when \p max_chordal_error = 0 or +-DBL_MAX.
   \param[in]     min_h             Minimum mesh size due to the chordal control (security) . 
                                    The mesh size will not be reduced (by the \p max_chordal_err criterion) beyond this limit.
   \param[in,out] indices           The indices of the nodes are appended, beginning with `spline_nodes[1]`, 
                                    then new nodes created on the whole poly-spline, then `spline_nodes[NS-2]` 
                                    (with `NS = spline_nodes.size()`).
   \param[in,out] Us                The curvilinear coordinates between 0 and 1 of the nodes associated to the \p indices
                                    (appended in same order as in \p indices). \n
                                    New appended parameters are sorted ascendingly. \n
                                    Starts with a parameter equal to 0.0 for the first node, then the parameters 
                                    for the intermediate nodes, and finally the value 1.0 for the last node.
   \param[in,out] metrics           The metrics at the nodes in \p indices (appended in same order as in \p indices).
                                 
   \note          When successful, at least one edge (two when \p force_even == true) is created on the line. 
   \note          The new nodes are located on the initial polyline.
   
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2 or 3).
   \pre        \p geo_nodes.size() >= 2
   \pre        \p geo_nodes.size() = \p metricsG.cols()
   \pre        \p UG.size() = \p geo_nodes.size()
   \pre        Values in \p UG are sorted increasingly.
   \pre        All metrics must be positive-definite (valid). 
   \pre        \p min_n > 0
   \pre        \p min_n <= \p max_n
   \post       Values in \p new_U are sorted ascendingly, between \p UG.front() and \p UG.back().

   \see        mesh_straight, extrude_translate, extrude_rotate, mesh_spline.
   */
CM2MESHTOOLS1D_API int 
mesh_line (DoubleMat& pos, 
           const UIntVec& geo_nodes, const DoubleVec& UG, const DoubleMat& metricsG, 
           bool force_even, unsigned min_n, unsigned max_n, double max_chordal_error, double min_h,
           UIntVec& indices, DoubleVec& Us, DoubleMat& metrics);  
//@}

}  // end namespace meshtools1d
}  // end namespace cm2

#endif

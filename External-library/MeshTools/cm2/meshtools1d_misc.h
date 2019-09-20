/**
   \file       meshtools1d_misc.h
   \brief      Declarations of some miscellaneous meshtools1d routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3540 $
   $Date: 2019-03-14 16:59:18 +0100 (jeu., 14 mars 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS1D_MISC_H__
#define __CM2_MESHTOOLS1D_MISC_H__


namespace cm2 {
namespace meshtools1d {


/****************************************************************************************/
/** @name Normals and curvatures computation routine along 1-D meshes */
//@{

/**
   Averaged nodal normals along a line mesh (2-D only).
   
   \if noCond
   \verbatim
              ---
              \     w
              /    L    D
              ---   ij   ij
     D  =  ------------------
      i        ---
               \     w
               /    L    
               ---   ij  
   \endverbatim
   \endif

   \f$
      D_i = \frac{\sum_{edges\;ij}{L_{ij}^w ~ D_{ij}}}{\sum_{edges\;ij} {L_{ij}^w}}
   \f$

   where:

      \f$ D_i \f$ = averaged normal at node i.

      \f$ L_{ij} \f$ = length of edge ij.

      \f$ D_{ij} \f$ = normal to edge ij (normalized to 1).


   \param[in]     pos            The coordinates matrix (2-D only). 
   \param[in]     connectE       The connectivity matrix of the edges.
   \param[in]     w              The averaging power used to compute the normals. \n
                                 For instance:
                                    -  w = -DBL_MAX => normal associated to the shortest edge.                         
                                    -  w = 0        => arithmetic mean.
                                    -  w = +DBL_MAX => normal associated to the longest edge.  
   \param[in]     normalize_flag  Flag to tell whether the averaged normals should be normalized or not. \n
                                 If false, the computed normals are the mean of normalized normals.
   \param[out]    D              The nodal normals (data upon entry are lost). \n
                                 Only the columns with indices in \p connectE are affected (other are left null).  

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The normals to the elements are computed as the cross product between OZ and the edge direction.
   \note          Only the first two nodes of each edge (first two rows in \p connectE) are used. \n
                  Curvatures along high-order edges are not considered.

   \warning       Edges should follow the same orientation (either all clock-wise or all counter clock-wise). 

   \pre           \p pos.rows() == 2 or \p pos is empty.
   \pre           \p connectE.rows() >= 2 or \p connectE is empty.
                  Only the first two rows are used (linear elements).
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   */
CM2MESHTOOLS1D_API int 
normals (const DoubleMat& pos, const UIntMat& connectE, double w, bool normalize_flag, DoubleMat& D);

/**
   Approximate nodal curvatures (inverse of radius) along a line mesh.

   \param[in]     pos            The coordinates matrix (2-D or 3-D). 
   \param[in]     nodes          The node IDs along the line. \n
                                 Nodes should be in successive order along the line.
   \param[out]    invR           The nodal curvatures. \n
                                 For a node `nodes[i]`, the curvature `invR[i]` is computed using the difference
                                 of coordinates with the node before (`nodes[i-1]`) and the node after (`nodes[i+1]`).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The curvature at first node is set to the same value at the node after.
   \note          The curvature at last node is set to the same value at the node before.

   \pre           \p pos.rows() == 2 or 3.
   \pre           All the values in \p nodes are smaller than \p pos.cols().
   \post          \p invR.size() == \p nodes.size().
   */
CM2MESHTOOLS1D_API int 
curvatures (const DoubleMat& pos, const UIntVec& nodes, DoubleVec& invR);

/**
   Approximative nodal curvatures (inverse of radius) at each node along a line mesh.

   Computation of approximate curvatures based on tangents only using the FEM method 
   (Stokes formula + lumped integration).

   \param[in]     pos1D          The 1-D coordinates vector of the nodes. 
   \param[in]     connectE       The connectivity matrix of the mesh (cm2::CM2_EDGE2 only).
   \param[in]     local_bases    The local tangents. \n
                                 The tangents T (derivative of the line with respect to reference parameter `U`: DP/Du)
                                 are stored column-wise in matrix \p local_bases. 
   \param[out]    curvatures     The nodal curvatures defined as:
                                    -  H[i] = ||DT/Du||    Norm of the derivative of the tangent `T` with respect to parameter `U`.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connectE.rows() == 2 or \p connectE is empty.
   \pre           All the values in the connectivity matrix \p connectE are smaller than \p pos1D.size().
   \pre           \p local_bases.rows() == 3 and \p local_bases.cols() == \p pos1D.size().
   \post          \p curvatures.size() == \p pos1D.cols() (except when error).
   */
CM2MESHTOOLS1D_API int
parametric_curvatures
   (const DoubleVec& pos1D, const UIntMat& connectE, const DoubleMat& local_bases, DoubleVec& curvatures);

//@}



/****************************************************************************************/
/** @name Miscellaneous routines for 1-D meshes */
//@{

/**
   Length of a line mesh.

   \param[in]     pos            The coordinates matrix (2-D or 3-D). 
   \param[in]     nodes          The node IDs along the line. \n
                                 Nodes should be in successive order along the line.
   \param[out]    L              The length of the mesh.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 2 or 3.
   \pre           \p nodes.size() >= 2.
   \pre           All the values in \p nodes are smaller than \p pos.cols().
   */
CM2MESHTOOLS1D_API int 
length (const DoubleMat& pos, const UIntVec& nodes, double& L);

/**
   Length of a line mesh.

   \param[in]     pos            The coordinates matrix (2-D or 3-D). 
   \param[in]     connectE       The connectivity matrix of the edges.
   \param[out]    L              The length of the mesh.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 2 or 3.
   \pre           \p connectE.rows() == 2 or \p connectE is empty.
   \pre           All the values in \p connectE are smaller than \p pos.cols().
   */
CM2MESHTOOLS1D_API int 
length (const DoubleMat& pos, const UIntMat& connectE, double& L);

 /**
   Interpolates nodes onto a polyline.

   \param[in,out]     pos     The coordinates matrix. \n
                              Only the coordinates of \p nodes1 are changed. \n
                              `nodes1[i]` at curvilinear coordinates `U1[i]` is computed as the 
                              interpolation between the two points of \p nodes0 bracketing `U1[i]`.
   \param[in]         nodes0  The nodes that define the polyline on which \p nodes1 will be interpolated.
   \param[in]         U0      The curvilinear coordinates associated to \p nodes0.
   \param[in]         nodes1  The nodes to be interpolated.
   \param[in]         U1      The curvilinear coordinates associated to \p nodes1.

   \pre     \p U0.size() == \p nodes0.size()
   \pre     \p U1.size() == \p nodes1.size()
   \pre     \p U0 must be sorted ascendingly.
   \pre     \p U1 must be sorted ascendingly.
   \pre     \p nodes0 and \p nodes1 must have empty intersection (no same node in the two sets).
   */
CM2MESHTOOLS1D_API int 
interpolate_coordinates
   (DoubleMat& pos, 
    const UIntVec& nodes0, const DoubleVec& U0,
    const UIntVec& nodes1, const DoubleVec& U1);


/**
   Sorts nodes by increasing curvilinear coordinates.

   \param[in,out]     Us      The curvilinear coordinates.
   \param[in,out]     nodes   The nodes associated to the curvilinear coordinates.

   \pre     \p Us.size() == \p nodes.size()
   \post    \p Us is sorted ascendingly.
   */
CM2MESHTOOLS1D_API int 
sort (DoubleVec& Us, UIntVec& nodes);


/**
   Subdivides a 1-D line. 

   \param[in]     Us                The coordinates vector. Must be sorted ascendingly. \n
                                    New points may be appended upon exit.
   \param[in]     metrics           The sizes associated to the points in \p Us. \n
                                    New metrics may be appended upon exit.
   \param[in]     nodes             The indices of the nodes along the 1-D mesh defining the line, sorted by increasing `U` parameter.
   \param[in]     force_up          Flag to round up the number of new nodes.
   \param[in]     force_even        Flag to force the number of created edges to be even.
   \param[in]     min_n             The minimum number of edges that should be created along the line.
   \param[in]     max_n             The maximum number of edges that should be created along the line.
   \param[out]    indices           The indices of the nodes along the 1-D mesh, sorted by increasing `U` parameter.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Values in \p Us must be sorted ascendingly.
   \pre        All the values in \p metrics are > 0.
   \pre        \p min_n > 0
   \pre        \p min_n <= max_n
   \post       \p indices.front() == \p nodes.front()
   \post       \p indices.back() == \p nodes.back()
   */
CM2MESHTOOLS1D_API int 
subdivide
   (DoubleVec& Us, DoubleVec& metrics, const UIntVec& nodes,
    bool force_up, bool force_even, unsigned min_n, unsigned max_n,
    UIntVec& indices);

//@}


}  // end namespace meshtools1d
}  // end namespace cm2

#endif

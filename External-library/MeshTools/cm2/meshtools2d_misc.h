/**
   \file       meshtools2d_misc.h
   \brief      Declarations of some miscellaneous meshtools2d routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3540 $
   $Date: 2019-03-14 16:59:18 +0100 (jeu., 14 mars 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS2D_MISC_H__
#define __CM2_MESHTOOLS2D_MISC_H__


namespace cm2 {
namespace meshtools2d {

/****************************************************************************************/
/** @name Area computation */
//@{

/**
   Computes the area of a triangle or quadrangle mesh (planar or not).

   \param[in]     pos         The coordinates matrix (2-D or 3-D).
   \param[in]     connectM    The connectivity matrix of the surface mesh (linear 3-node or 4-node elements only, 
                              all-triangle, pure quadrangle or mixed triangle/quadrangle).
   \param[out]    s           The total area of the mesh.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \note       In 2-D (when \p pos.rows() == 2), the orientation of the each element is positive when
               oriented counter clock-wise (CCW), and negative when oriented clock-wise (CW).
   \note       In 3-D (when \p pos.rows() == 3), the orientation of each element is not relevant. \n
               The computed area for each element is always a positive value (or null if the 
               element is degenerated).
   \note       In 3-D (when \p pos.rows() == 3), the area of warped quads is only approximated. 
   \note       To compute the area of a 6-node triangle mesh or 9-node quad mesh (or any high order 2-D mesh), 
               the connectivity matrix must be reduced to a view on the 3 or 4 first rows to consider the
               elements as linear. \n
               The computed area is then only approximated if the high-order elements were curved.

   \pre        \p pos.rows() == 2 or \p pos.rows() == 3
   \pre        \p connectM.rows() == 3 or \p connectM.rows() == 4 or \p connectM is empty.
   \pre        All nodes IDs referenced in \p connectM should be lesser than pos.cols().

   \see        File fe_defs.h for local node numbering in elements.
   */
CM2MESHTOOLS2D_API int 
area (const DoubleMat& pos, const UIntMat& connectM, double& s);                          

//@}


/****************************************************************************************/
/** @name Angle computation */
//@{

/**
   Computes the angles (min and max) at nodes of each element in a triangle or quadrangle mesh
   (planar or not).

   Unit for angles (if not normalized) is radian (between 0 and 2*pi):
      - An equilateral triangle have all its three angles equal to pi/3 (60 deg.).
      - A rectangular have all its four angles equal to pi/2 (90 deg.).

   \param[in]     pos         The coordinates matrix (2-D or 3-D).
   \param[in]     connectM    The connectivity matrix of the surface mesh
                              (all-triangle, pure quadrangle or mixed triangle/quadrangle).
   \param[in]     normalized  Flag to normalize the angles. \n
                              Angles in triangles are divided by pi/3 (1 becomes the angle for an equilateral triangle). \n
                              Angles in quads are divided by pi/2 (1 becomes the angle for a rectangle).
   \param[out]    min_angles  `min_angles[i]` is the minimum angle in element `i`.
   \param[out]    max_angles  `max_angles[i]` is the maximum angle in element `i`.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \note       In 2-D (when \p pos.rows() == 2), the orientation of the each element is positive when
               oriented counter clock-wise (CCW), and negative when oriented clock-wise (CW).
   \note       In 3-D (when \p pos.rows() == 3), the orientation of each element is not relevant.
               The computed angles for each element are always positive (or null if the 
               element is degenerated).

   \pre        \p pos.rows() == 2 or \p pos.rows() == 3
   \pre        \p connectM.rows() == 3 or \p connectM.rows() == 4 or \p connectM is empty.
   \pre        All nodes IDs referenced in \p connectM should be lesser than \p pos.cols().

   \see        File fe_defs.h for local node numbering in elements.
   */
CM2MESHTOOLS2D_API int 
angles 
   (const DoubleMat& pos, const UIntMat& connectM, bool normalized,
    DoubleVec& min_angles, DoubleVec& max_angles);

//@}


/****************************************************************************************/
/** @name Normals computation routine on surface meshes */
//@{

/**
   Computes the nodal averaged normals along a 3-D surface.
   
   \if noCond
   \verbatim
              ---
              \     w
              /    S    D
              ---   E    E
     D  =  ------------------
      i        ---
               \     w
               /    S    
               ---   E  
   \endverbatim
   \endif

   \f$
      D_i = \frac{\sum_{E}{{S_E}^w ~ D_E}}{\sum_{E} {{S_E}^w}}
   \f$

   where:

      \f$ D_i \f$ = averaged normal at node i.

      \f$ S_E \f$ = area of face E.

      \f$ D_E \f$ = normal to face E (normalized to 1).


   \param[in]     pos            The coordinates matrix. 
   \param[in]     connectM       The connectivity matrix of the surface mesh (linear 3-node or 4-node elements only, 
                                 all-triangle, all-quad or mixed triangle/quadrangle).
   \param[in]     w              The averaging power on areas used to compute the normals.
                                 For instance:
                                    -  w = `-DBL_MAX` => normal associated to the smallest face.                         
                                    -  w = `1`        => arithmetic mean with weights = areas.
                                    -  w = `+DBL_MAX` => normal associated to the largest face.  
   \param[in]     normalize_flag  Flag to tell whether the averaged normals should be normalized or not. \n
                                 If false, the computed normals are the mean of normalized normals.
   \param[out]    D              The nodal normals (data upon entry are lost). \n
                                 Only the columns with indices in \p connectM are affected (other are left null).  

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          For warped quad, the elementary normal \f$ D_E \f$ is computed at centroid (one normal per element).
   \warning       Faces should follow the same orientation (either all outward or all inward). 

   \pre           \p pos.rows() == 3 or \p pos is empty.
   \pre           \p connectM.rows() == 3 or \p connectM.rows() == 4 or \p connectM is empty.
   \pre           All the values in \p connectM are smaller than \p pos.cols().
   */
CM2MESHTOOLS2D_API int 
normals 
   (const DoubleMat& pos, const UIntMat& connectM, double w, bool normalize_flag, DoubleMat& D);

//@}


/****************************************************************************************/
/** @name Curvatures computation routines on surface meshes */
//@{

/**
   Computes the two approximative principal curvatures (directions and inverse of radii) 
   at each node of a triangle 3-D surface mesh.

   The curvatures are computed by averaging the discreet curvatures along edges in a "disk" around
   each node.

   \param[in]     pos            The coordinates matrix as a 3xNODS matrix. 
   \param[in]     connectM       The connectivity matrix of the mesh as a 3xNEFS matrix (cm2::CM2_FACET3 only).
   \param[in]     neighbors      The neighbors matrix (3xNEFS).
                                 Can be computed by the meshtools::get_neighbors function.
   \param[in]     R              Radius of the "disks". \n
                                 For a node `Ni` the curvatures are averaged over the elements in the "disk" of `Ni`. \n
                                 The "disk" is defined by the elements directly connected to `Ni` (ring-1)
                                 plus all neighboring elements with centroid within distance \p R from `Ni` stopping
                                 at \p boundaries and ridges (angle > \p angle_max).
   \param[in]     angle_max      Only contributions of pairs of elements with angle smaller than this threshold
                                 (in radian) are used in the computation of the curvatures.
   \param[in]     max_nefs_in_disks    Limit the "disks" to the \p max_nefs_in_disks closest elements.
   \param[in]     boundaries     Optional matrix (2xNE) to define additional sharp edges (in addition to those with
                                 angle greater than \p angle_max).
   \param[out]    curvatures     The approximated nodal curvatures as a 8xNODS matrix. \n
                                 The two principal curvature directions and the two principal curvatures, stored column-wise. \n
                                 The first three rows are for the major principal curvature direction (normalized). \n 
                                 The next three rows are for the minor principal curvature direction (normalized). \n 
                                 The associated curvatures are stored in rows #6 and #7.\n
                                 For each node `i`:
                                    - `curvatures(0,i)..curvatures(2,i)` = \f$ T_0(i) \f$ is the curvature direction
                                      (normalized) associated to \f$ K_0(i) \f$ the curvature with maximum magnitude. \n
                                    - `curvatures(3,i)..curvatures(5,i)` = \f$ T_1(i) \f$ is the curvature direction
                                      (normalized) associated to \f$ K_1(i) \f$ the curvature with minimum magnitude. \n
                                    - `curvatures(6,i)` = \f$ K_0(i) \f$.
                                    - `curvatures(7,i)` = \f$ K_1(i) \f$.
                                    .
                                 The curvature directions are such that for a cylinder for instance, 
                                 the maximal curvature direction is parallel to the axis of the cylinder. \n
                                 The normal at node `i` can be deduced from \f$ T_0(i) \f$ and \f$ T_1(i) \f$ using the following 
                                 cross product formula: \f$ D(i) = T_0(i) \times T_1(i) \f$ .

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The ring-1 elements are always used, even if \p R <= 0.
   \note          For a sharp node `Ni`, i.e. node in the \p boundaries or along a ridge (angle > \p angle_max),
                  the curvatures are computed using the disk associated to the lowest element containing `Ni`.

   \pre           \p pos.rows() == 3.
   \pre           \p connectM.rows() == 3 or \p connectM is empty.
   \pre           \p neighbors.rows() == 3 and \p neighbors.cols() = \p connectM.cols().
   \pre           All the values in the \p connectM are smaller than \p pos.cols().
   \post          \p curvatures.rows() == 8 and \p curvatures.cols() == \p pos.cols() (except when error).

   \see           "COHEN-STEINER, D., AND MORVAN, J.-M., 2003. Restricted Delaunay triangulations and normal cycle". \n
                  In ACM Symp. on Computational Geometry.
   */
CM2MESHTOOLS2D_API int
principal_curvatures
   (const DoubleMat& pos, const UIntMat& connectM, const UIntMat& neighbors, 
    double R, double angle_max, unsigned max_nefs_in_disks, const UIntMat& boundaries,
    DoubleMat& curvatures);


/**
   Approximative nodal curvature tensor (2x2 tensors) of a mesh.

   The mesh must be in the parametric coordinates system (2-D).

   Computation of approximate curvatures based on tangents only using the FEM method 
   (Stokes formula + lumped integration).

   \param[in]     pos2D          The coordinates matrix as a 2xNODS matrix. 
                                 The mesh must be in the _reference_ coordinates system (`u`, `v`).
   \param[in]     connectM       The connectivity matrix of the mesh (cm2::CM2_FACET3 only).
   \param[in]     local_bases    The local tangents. \n
                                 The two tangents `Bu` and `Bv` (derivative of the surface with respect to 
                                 reference parameters `u` and `v`: DP/Du and DP/Dv) are stored column-wise 
                                 in matrix \p local_bases. \n
                                 First the three components of `Bu` then the three components of `Bv`.
   \param[out]    curvatures     The nodal curvatures.  
                                 The curvatures H are 2x2 symmetric matrices defined as:
                                    -  H(0,0) = <DBu/Du, N>   Dot product between the derivative of `Bu` (first local tangent) with respect to `u`
                                                              (i.e. second derivative of `P(u,v)` with respect to `u`, `u`), and the normal `N` to the surface. 
                                    -  H(0,1) = <DBu/Dv, N>   Dot product between the derivative of `Bu` (first local tangent) with respect to `v`, 
                                                              or `Bv` (second local tangent) with respect to `u`
                                                              (i.e. second derivative of `P(u,v)` with respect to `u`, `v`), and the normal `N` to the surface. 
                                    -  H(1,1) = <DBv/Dv, N>   Dot product between the derivative of `Bv` (second local tangent) with respect to `v`
                                                              (i.e. second derivative of `P(u,v)` with respect to `v`, `v`), and the normal `N` to the surface. 
                                 These three values are stored column-wise in matrix \p curvatures.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The mesh must be in the _reference_ coordinates system (`u`, `v`).

   \pre           \p pos2D.rows() == 2 or \p pos2D is empty.
   \pre           \p connectM.rows() == 3 or \p connectM is empty.
   \pre           All the values in \p connectM are smaller than \p pos2D.cols().
   \pre           \p local_bases.rows() == 6 and \p local_bases.cols() == \p pos2D.cols().
   \post          \p curvatures.rows() == 3 and \p curvatures.cols() == \p pos2D.cols() (except when error).
   */
CM2MESHTOOLS2D_API int
parametric_curvatures
   (const DoubleMat& pos2D, const UIntMat& connectM, const DoubleMat& local_bases, DoubleMat& curvatures);

//@}


}  // end namespace meshtools2d
}  // end namespace cm2

#endif

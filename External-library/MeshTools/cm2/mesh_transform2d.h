/**
   \file       mesh_transform2d.h
   \brief      Declarations of some 2-D mesh transformation routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_TRANSFORM2D_H__
#define __CM2_MESH_TRANSFORM2D_H__


namespace cm2 {
namespace meshtools2d {

/****************************************************************************************/
/** @name Reorientation of 2-D elements */
//@{

/**
   Changes the orientation of the faces in a 2-D mesh (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX). \n
   Linear elements only.

   This flip the orientation (i.e. the normal) of the element.
   Applied twice does not change anything.

   \param[in]     connect     The connectivity matrix.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Swaps the local nodes #0 and #2.
   \warning       Doesn't swap high-order nodes in FACET6, FACEQ8 or FACEQ9.
                  Use flip function below instead.

   \pre           \p connect.rows() >= 3 or \p connect is empty.

   \see           \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS2D_API int
flip (UIntMat& connect);

/**
   Changes the orientation of the faces in a 2-D mesh 
   (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX, cm2::CM2_FACET6, cm2::CM2_FACEQ8 or cm2::CM2_FACEQ9).

   This flip the orientation (i.e. the normal) of the element.
   Applied twice does not change anything.

   \param[in]     connect     The connectivity matrix.
   \param[in]     FE_type     The type of the elements (see \ref fe_geo_page).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \note          For linear elements (cm2::CM2_FACET3, cm2::CM2_FACEQ4, cm2::CM2_FACE_MIX), swaps the local nodes #0 and #2. \n
                  For high-order elements (cm2::CM2_FACET6, cm2::CM2_FACEQ8 or cm2::CM2_FACEQ9), swap also the mid-edge nodes.
   \note          Works also with mixed connectivity matrices 
                  (i.e. \p connect.rows() == 4 and \p FE_type == cm2::CM2_FACE_MIX) and
                  mixed high-order matrices
                  (i.e. \p connect.rows() == 9 and \p FE_type == cm2::CM2_FACE_MIX).

   \pre           \p Number of rows in \p connect matches \p FE_type or \p connect is empty.

   \see           \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS2D_API int
flip (UIntMat& connect, cm2::element_type FE_type);


/**
   Changes the orientation of the triangles in a 2-D mesh (FACET3, FACEQ4 or FACE_MIX) so that
   all elements have similar orientation as a reference element.

   "Similar orientation" for elements means that each edge is oriented 
   oppositely in the two adjacent elements.

   The algorithm involved is a neighboring search starting from element \p ref_element.

   If code == 0, only the elements in the same subdomain(1) as element \p ref_element
   will be visited. Other elements will remain unchanged.\n
   If code != 0, all elements will be visited. Elements in other subdomains than that of \p ref_element, if any, 
   will be reoriented as the first (lowest ID) element in each subdomain.

   \note A subdomain is a domain's subset of connected elements.
         Two elements are in the same subdomain when there is an element-path between them which 
         is not crossed by any boundary.

   The \p neighbors matrix is defined by: 

      `neighbors(i,j)` = neighbor of element `j` by the edge `i`
      with the local node and edge IDs defined as follow: 

      \verbatim
      
            N1
             *
             | \
             |   \
             |     \ E2
          E0 |       \
             |         \
             |           \
             *------------*
            N2       E1     N0   

      

            N3      E2       N2
             *---------------*
             |               |
             |               |
             |               |
           E3|               |E1
             |               |
             |               |
             |               |
             *---------------*
            N0      E0       N1   

      \endverbatim
   
   \param[in,out] connectM          The connectivity matrix (3xNEFS or 4xNEFS). \n
                                    Mixed meshes allowed.
   \param[in,out] neighbors         The neighbors matrix (3xNEFS or 4xNEFS).
   \param[in]     ref_element       The index of the reference element in \p connectM.
   \param[in]     code              The code to tell whether all elements must be processed (\p code != 0),
                                    or only the elements among the subdomain of \p ref_element (\p code == 0).

   \return     Number of flipped elements. Returns a negative value in case of error.
               Returned value = -k => the k-th argument had an illegal value.

   \pre        \p connectM.rows() == 3, or \p connectM.rows() == 4, or \p connectM is empty.
   \pre        \p neighbors.rows() == 3 or \p neighbors.rows() == 4
   \pre        \p neighbors.cols() == \p connectM.cols()    
   \pre        \p ref_element < \p connectM.cols()  

   \note       The initial neighbors matrix can be computed by the meshtools::get_neighbors function.
   */
CM2MESHTOOLS2D_API int 
mesh_reorient (UIntMat& connectM, UIntMat& neighbors, unsigned ref_element, int code);                          


/**
   Splits quads into triangles (2 triangles each).

   The quadrangles are split always the same way:

   \verbatim

             N3           N2                 N3           N2
             *-------------*                 *-------------*
             |             |                 |            /|
             |             |                 |          /  |
             |             |                 |        /    |
             |             |                 |      /      |
             |             |        =>       |    /        |
             |             |                 |  /          |
             |             |                 |/            |
             *-------------*                 *-------------*
             N0           N1                 N0           N1

                                            T0 = {N0, N1, N2}
                                            T1 = {N2, N3, N0}
   \endverbatim

   \param[in]     connectM          The connectivity matrix of the input mesh (dimension 4xNEFS or 3xNEFS). \n
                                    Mixed meshes allowed.
   \param[in,out] connectT          The connectivity matrix of the all-triangle mesh. \n
                                    Each quad in \p connectT gives two triangles appended to this matrix. \n
                                    Triangles in \p connectT are simply appended to this matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value. \n
                  Else returns the number of quads split.

   \note          The elements are split or copied successively in the order of the \p connectM matrix.
   \note          This routine always splits the quads along edge N0-N2.
   \note          All-triangle meshes (connectM.rows() == 3) are appended to \p connectT with no transformation.

   \pre           \p connectM.empty() or \p connectM.rows() = 3 or 4 (all-triangle/all-quad/mixed mesh).
   \post          \p connectT.empty() or \p connectT.rows() = 3 (all-triangle mesh).

   \warning       Node-equivalent quads may be split in different ways. \n
                  Example: quads {N0, N1, N2, N3} and {N1, N0, N3, N2} are node-equivalent. \n
                  However they are split into {N0, N1, N2} + {N0, N2, N3} and {N1, N0, N3} + {N1, N3, N2}.
   */
CM2MESHTOOLS2D_API int 
split_Q4_into_T3 (const UIntMat& connectM, UIntMat& connectT);                          


/**
   Splits quads into triangles (2 triangles each).

   \param[in]     pos               The coordinates matrix. No new node is created.
   \param[in]     connectM          The connectivity matrix of the input mesh (dimension 4xNEFS or 3xNEFS).\n
                                    Mixed meshes allowed.
   \param[in,out] connectT          The connectivity matrix of the triangle mesh.\n
                                    Each quad in \p connectM is converted into two triangles 
                                    (best minimum shape-quality) which are appended to 
                                    this matrix \p connectT.

   \return        The number of quads split. Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value. \n
                  Else returns the number of quads split.

   \note          The elements are split or copied successively in the order of the \p connectM matrix.
   \note          All-triangle meshes (connectM.rows() == 3) are appended to \p connectT with no transformation.
   \note          In case of a perfect rectangular quad, the two splits are equivalent with respect 
                  to the quality of the generated triangles. \n
                  However, this routine guarantees that the same split is chosen when several quads
                  are node-equivalent. \n
                  Example: quads {N0, N1, N2, N3} and {N1, N0, N3, N2} are node-equivalent. They
                  lead to a node-equivalent pair of triangles (for instance {N0, N1, N2} + {N0, N2, N3} and
                  {N0, N3, N2} + {N0, N2, N1}).

   \pre           \p connectM.empty() or \p connectM.rows() = 3 or 4 (all-triangle/all-quad/mixed mesh).
   \post          \p connectT.empty() or \p connectT.rows() = 3 (all-triangle mesh).
   */
CM2MESHTOOLS2D_API int 
split_Q4_into_T3 (const DoubleMat& pos, const UIntMat& connectM, UIntMat& connectT);                          


/**
   Splits bad quads into triangles (2 triangles each).

   A quad is considered bad when its geometric quality or angle quality is below a specified threshold.

   \param[in]     pos               The coordinates matrix. No new node is created.
   \param[in]     connectM0         The connectivity matrix of the input mesh (dimension 4xNEFS). \n
                                    Mixed meshes allowed.
   \param[in]     min_Q4_geometric_quality    The geometric quality (between 0 and 1) under which the quads 
                                    should be split into two triangles. \n
                                    When min_Q4_geometric_quality > 1, all quads are split.
   \param[in]     min_Q4_angle_quality    The angle quality (between 0 and 1) under which the quads 
                                    should be split into two triangles. \n
                                    When min_Q4_angle_quality > 1, all quads are split.
   \param[in]     split_criterion   When a quad has to be split, the two triangles should maximize/minimize this criterion:
                                       -  0: Default split strategy: quad {N0, N1, N2, N3}
                                             is split into {N0, N1, N2} + {N2, N3, N0}.
                                       -  1: Maximizes the min shape quality of the two triangles.
                                       -  2: Maximizes the min angle quality of the two triangles.
                                       -  3: Minimizes the max area of the two triangles.
                                       -  4: Minimizes the total area of the two triangles.
                                       -  5: Minimizes the angle between of the two triangles (3-D only).
                                       -  6: Minimizes the "curvature" between of the two triangles 
                                             (product between the angle and the length of the common edge, 3-D only).
                                       -  Any other value: same as 0 (default strategy).
   \param[out]    good_IDs          The unsplit quads and all the triangles (columns IDs in matrix \p connectM0).
   \param[out]    bad_IDs           The split quads (columns IDs in matrix \p connectM0).
   \param[in,out] connectM1         The new connectivity. \n
                                    Each bad quad in \p connectM0 gives two triangles appended to this matrix. \n
                                    Good quads and triangles in \p connectM0 are appended first to this matrix (\p good_IDs.size() columns),
                                    then the split triangles (2 * \p bad_IDs.size() columns). \n
                                    To be considered as good, a quad must have its geometric quality greater than \p min_Q4_geometric_quality 
                                    and its angle quality greater than \p min_Q4_angle_quality. 

   \return        The number of quads split. Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value. \n
                  Else returns the number of quads split.

   \note          The split triangles are appended to \p connectM1 as 4-node columns with 
                  last node equals to `CM2_NONE`. \n
                  Even when all quads are split and \p connectM1 were empty upon entry, \p connectM1 still has 
                  four rows upon exit (all IDs in the last rows set to `CM2_NONE`). \n
   \note          In case of a rectangular quad, the two splits are equivalent with respect 
                  to the quality of the generated triangles. \n
                  However, this routine guarantees that the same split is chosen when several quads
                  are node-equivalent. \n
                  Example: quads {N0, N1, N2, N3} and {N1, N0, N3, N2} are node-equivalent. They
                  lead to a node-equivalent pair of triangles (for instance {N0, N1, N2} + {N0, N2, N3} and
                  {N0, N3, N2} + {N0, N2, N1}).
   \note          The elements order in the \p connectM1 matrix is different from the order
                  given by preceding `split_Q4_into_T3` functions:
                  good elements are appended first, then the split triangles of the bad ones.

   \pre           \p connectM0.empty() or \p connectM0.rows() = 4 (all-quad/mixed mesh).
   \post          \p connectM1.empty() or \p connectM1.rows() = 4 (all-quad/mixed mesh).
   */
CM2MESHTOOLS2D_API int 
split_Q4_into_T3 
   (const DoubleMat& pos, const UIntMat& connectM0, 
    double min_Q4_geometric_quality, double min_Q4_angle_quality, int split_criterion,
    UIntVec& good_IDs, UIntVec& bad_IDs, UIntMat& connectM1);                          

//@}


/****************************************************************************************/
/** @name Special transformation for degenerated quads */
//@{

/**
   Transforms the degenerated quads with two consecutive identical node IDs into 
   CM2-compliant triangles stored in a mixed mesh (i.e. triangle with `CM2_NONE` as the 4-th node).

   In CM2 MeshTools, triangles in a mixed quad-triangle connectivity matrix
   must have the special value `CM2_NONE` as the 4-th node.
   
   Elements like {i, i, j, k}, {i, j, j, k}, {i, j, k, k} and {i, j, k, i}
   are replaced with {i, j, k, CM2_NONE}.

   \param[in]     connect  The mixed quad/triangle connectivity matrix. Must have 4 rows.

   \return        The number of quads converted. Returns a negative value in case of error.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect is empty or \p connect.rows() = 4

   \warning       Elements like {i, j, i, k} and {i, j, k, j} are not converted into {i, j, k, `CM2_NONE`}.
   \see           meshtools::remove_degenerated.
   */
CM2MESHTOOLS2D_API int 
convert_degenerated_Q4 (UIntMat& connect);

//@}


}  // end namespace meshtools2d
}  // end namespace cm2

#endif

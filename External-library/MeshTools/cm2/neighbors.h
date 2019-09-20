/**
   \file       neighbors.h
   \brief      Declarations of some routines to compute neighbors matrices.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3601 $
   $Date: 2019-04-18 15:22:01 +0200 (jeu., 18 avr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_NEIGHBORS_H__
#define __CM2_NEIGHBORS_H__


namespace cm2 {
namespace meshtools {


/****************************************************************************************/
/** @name Ancestors, neighbors and boundaries */
//@{

/**
   For each node in a mesh, finds an element connected to it.
   
   \param[in]     connect        The connectivity matrix.
   \param[in,out] ancestors      The ancestors vector. 
                                 If not empty upon entry, this vector is not resized and
                                 only the nodes with an index strictly lower than ancestors.size() are given a new ancestor.\n
                                 If empty upon entry, this vector is resized to the largest
                                 node ID referenced in \p connect. 
                                 In this case, if a node is orphan (no element connected to it), 
                                 its ancestor value is set to `CM2_NONE` (-1).
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \warning       To recompute all the ancestors, clear vector \p ancestors before entry.
   \note          Time and memory complexity: linear = O(\p connect.entries()).
   \note          All CM2 meshers provide such an array upon exit.
   */
CM2MESHTOOLS_API int 
get_ancestors 
   (const UIntMat& connect, UIntVec& ancestors);                          


/**
   Computes the neighbors matrix of a mesh given by a connectivity matrix.

   The \p neighbors matrix is defined by: 

   `neighbors(i,j)` = neighbor of element `j` by the boundary `i` (supposed to be unique).
   
   \param[in]     connect        The connectivity matrix.
   \param[in]     FE_type        The type of the elements (\ref fe_geo_page). \n
                                 Only linear elements are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                                 cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                                 cm2::CM2_SOLID_MIX.
   \param[in]     accept_multiple_neighbors  Flag used when a boundary is shared by more than two elements:
                                        - If set to true, `neighbors(i,j)` holds one arbitrary ID
                                          among the adjacent elements, and the \p neighbors is no longer reflexive.
                                        - If set to false, `neighbors(i,j)` = CM2_NONE as soon as several
                                          neighbors of `j` by boundary `i` are detected.
   \param[out]    neighbors      The output neighbors matrix. The initial matrix is lost.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect should have the right number of rows (= number of nodes defined by \p FE_type) or empty. \n
                  Exceptions: 
                     - When \p FE_type = cm2::CM2_FACE_MIX, \p connect can be also a pure-triangle mesh (3-row matrix) or a pure-quad mesh.
                     - When \p FE_type = cm2::CM2_SOLID_MIX, \p connect can be also a pure-tetrahedron mesh (4-row matrix), 
                       a pure-pyramid mesh (5-row matrix), a pure-wedge mesh (6-row matrix) or a pure-hex mesh.
                     .
                  You shouldn't however set for instance \p FE_type = cm2::CM2_SOLID_MIX and \p connect be a 5-row matrix 
                  with a mix of tetrahedrons and pyramids (must be pure pyramid).
   \note          The mesh can be a mixed type (cm2::CM2_FACE_MIX or cm2::CM2_SOLID_MIX).
   \note          Time and memory complexity: linear = O(\p connect.entries()).
   \note          All CM2 meshers provide such an array upon exit.
   \see           \ref fe_geo_page for the definition of the local nodes and boundaries ordering.
   */
CM2MESHTOOLS_API int 
get_neighbors 
   (const UIntMat& connect, cm2::element_type FE_type, bool accept_multiple_neighbors, UIntMat& neighbors);                          


/**
   Computes the boundaries of a mesh (boundaries connected to only one element).
   
   \param[in]     connect        The connectivity matrix.
   \param[in]     neighbors      The neighbors matrix defined by: 
                                    `neighbors(i,j)` = neighbor of element `j` by the boundary `i`. \n
                                 This array is provided by all CM2 meshers or can be computed with meshtools::get_neighbors.
   \param[in]     FE_type        The type of the elements (\ref fe_geo_page). \n
                                 Only linear elements are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                                 cm2::CM2_FACE_MIX, cm2::CM2_TETRA4,cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                                 cm2::CM2_SOLID_MIX.
   \param[out]    boundaries     Upon exit the connectivity matrix of the boundary mesh 
                                 (boundaries of elements with no neighbor in \p neighbors) is appended. 
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect.cols() == \p neighbors.cols()
   \pre           \p connect has the right number of rows (= number of nodes defined by \p FE_type) or empty. \n
                  Exceptions: 
                     - When \p FE_type = cm2::CM2_FACE_MIX, \p connect can be also a pure-triangle mesh (3-row matrix) or a pure-quad mesh.
                     - When \p FE_type = cm2::CM2_SOLID_MIX, \p connect can be also a pure-tetrahedron mesh (4-row matrix), 
                       a pure-pyramid mesh (5-row matrix), a pure-wedge mesh (6-row matrix) or a pure-hex mesh.
                     .
                  You shouldn't however set for instance \p FE_type = cm2::CM2_SOLID_MIX and \p connect be a 5-row matrix 
                  with a mix of tetrahedrons and pyramids (must be pure pyramid).
   \pre           Upon entry \p boundaries is empty or has the right number of rows (= number of nodes of boundary type).
   \note          The mesh can be a mixed type (cm2::CM2_FACE_MIX or cm2::CM2_SOLID_MIX).
   \note          Boundaries are oriented inward to the elements.
   \note          meshtools::get_mesh_boundaries and meshtools::get_mesh_bounding_elements give the same boundaries, in same order. \n
                  The first function gives boundaries in _explicit_ form (connectivity matrix of the boundaries)
                  whereas the latter gives boundaries in _implicit_ form (element ID and local boundary ID).
   \note          Time and memory complexity: linear = O(\p connect.entries()).
   \see           \ref fe_geo_page for the definition of the local nodes.
   \see           meshtools::get_mesh_bounding_elements, meshtools::get_colors_boundaries.
   */
CM2MESHTOOLS_API int 
get_mesh_boundaries 
   (const UIntMat& connect, const UIntMat& neighbors, cm2::element_type FE_type, UIntMat& boundaries);                          

/**
   Computes the boundaries of a mesh (boundaries connected to only one element).
   
   \param[in]     connect        The connectivity matrix.
   \param[in]     neighbors      The neighbors matrix defined by: 
                                    `neighbors(i,j)` = neighbor of element `j` by the boundary `i`. \n
                                 This array is provided by all CM2 meshers or can be computed with meshtools::get_neighbors.
   \param[in]     FE_type        The type of the elements (\ref fe_geo_page).
                                 Only linear elements are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                                 cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                                 cm2::CM2_SOLID_MIX.
   \param[out]    KBs            Upon exit the elements and their local boundary ID connected to no neighbor 
                                 in \p neighbors are appended. \n
                                 Elements and boundary IDs are stored in compressed form:
                                    `KBs[i] = Ki * nbr_elfe_boundaries + bi` \n
                                 where:
                                    - `Ki`: ID of the element with no neighbor.
                                    - `bi`: local boundary ID of element `Ki` with no neighbor.
                                    .
                                 Elements and local boundary IDs can be retrieved using the following simple formulas:
                                 \code
                                    Ki = KBs[i] / nbr_elfe_boundaries;
                                    bi = KBs[i] % nbr_elfe_boundaries;
                                 \endcode
                                 If interested only in Ks, just divide \p KBs by \p nbr_elfe_boundaries:
                                 `vecscal::div(nbr_elfe_boundaries, KBs;`
   \param[out]    nbr_elfe_boundaries  The number of boundaries considered for the type of element. \n
                                 May be different from meshtools::nbr_boundaries(\p FE_type) in case of mixed type. \n
                                 For instance if \p FE_type = cm2::CM2_FACE_MIX and \p connectM.rows() == 3,
                                 elements will be considered as triangles and \p nbr_elfe_boundaries = 3 upon exit
                                 (instead of 4 normally for cm2::CM2_FACE_MIX).
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect.cols() == \p neighbors.cols()
   \pre           \p connect has the right number of rows (= number of nodes defined by \p FE_type) or empty. \n
                  Exceptions: 
                     - When \p FE_type = cm2::CM2_FACE_MIX, \p connect can be also a pure-triangle mesh (3-row matrix) or a pure-quad mesh.
                     - When \p FE_type = cm2::CM2_SOLID_MIX, \p connect can be also a pure-tetrahedron mesh (4-row matrix), 
                       a pure-pyramid mesh (5-row matrix), a pure-wedge mesh (6-row matrix) or a pure-hex mesh.
                     .
                  You shouldn't however set for instance \p FE_type = cm2::CM2_SOLID_MIX and \p connect be a 5-row matrix 
                  with a mix of tetrahedrons and pyramids (must be pure pyramid).
   \pre           \p neighbors has the right number of rows (= number of boundaries defined by \p FE_type) or empty.
   \note          The mesh can be a mixed type (cm2::CM2_FACE_MIX or cm2::CM2_SOLID_MIX).
   \note          meshtools::get_mesh_boundaries and meshtools::get_mesh_bounding_elements give the same boundaries, in same order.\n
                  The first function gives boundaries in _explicit_ form (connectivity matrix of the boundaries) 
                  whereas the latter gives boundaries in _implicit_ form (element IDs and local boundary IDs).
   \note          Time and memory complexity: linear = O(\p connect.entries()).
   \see           \ref fe_geo_page for the definition of the local nodes.
   \see           meshtools::get_mesh_boundaries, meshtools::get_colors_boundaries, 
                  meshtools::get_elements_on_boundaries, meshtools::nbr_boundaries.
   */
CM2MESHTOOLS_API int 
get_mesh_bounding_elements 
   (const UIntMat& connect, const UIntMat& neighbors, cm2::element_type FE_type, 
    UIntVec& KBs, unsigned& nbr_elfe_boundaries);                          

/**
   Computes the boundaries of _all_ elements in a mesh.
   
   \param[in]     connect        The connectivity matrix.
   \param[in]     FE_type        The type of the elements (\ref fe_geo_page). \n
                                 Only linear elements are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                                 cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                                 cm2::CM2_SOLID_MIX.
   \param[in,out] boundaries     Upon exit the connectivity matrix of the element boundaries is appended 
                                 (following the element's boundaries local order). \n
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect has the right number of rows (= number of nodes defined by \p FE_type) or empty. \n
                  Exceptions: 
                     - When \p FE_type = cm2::CM2_FACE_MIX, \p connect can be also a pure-triangle mesh (3-row matrix) or a pure-quad mesh.
                     - When \p FE_type = cm2::CM2_SOLID_MIX, \p connect can be also a pure-tetrahedron mesh (4-row matrix), 
                       a pure-pyramid mesh (5-row matrix), a pure-wedge mesh (6-row matrix) or a pure-hex mesh.
                     .
                  You shouldn't however set for instance \p FE_type = cm2::CM2_SOLID_MIX and \p connect be a 5-row matrix 
                  with a mix of tetrahedrons and pyramids (must be pure pyramid).
   \pre           Upon entry \p boundaries is empty or has the right number of rows (= number of nodes of boundary type).
   \note          The mesh can be a mixed type (cm2::CM2_FACE_MIX or cm2::CM2_SOLID_MIX).
   \note          Boundaries are oriented inward to the elements.
   \note          Time and memory complexity: linear = O(\p connect.entries()).
   \warning       Boundaries shared by two (or more) elements are copied two (or more) times.
   \warning       The resulting matrix \p boundaries may be very big, especially for 3-D meshes. \n
                  Memory shortage should be considered as a potential issue.
   \see           \ref fe_geo_page for the definition of the local nodes and boundaries ordering.
   */
CM2MESHTOOLS_API int 
get_element_boundaries 
   (const UIntMat& connect, cm2::element_type FE_type, UIntMat& boundaries);                          
//@}


/****************************************************************************************/
/** @name Nodes -> elements graph, nodes -> nodes graph, boundaries -> elements mappings */
//@{

/**
   Computes the elements connected to each node.

   The connectivity matrix gives, for each element, the node IDs of this element.

   This compressed graph (or CSR) gives, for each node, the IDs of the elements connected to it.
   
   \param[in]     connect        The connectivity matrix.
   \param[out]    xadj           The CSR indices (positions in \p adjncy where node connections are located).
   \param[out]    adjncy         The CSR connections. \n
                                 Elements connected to node `i` are 
                                 at `adjncy[k]` with `xadj[i] <= k < xadj[i+1]` (past-the-end) sorted in ascending order. \n
                                 You can access them also through `adjncy.sub_vector(xadj[i], xadj[i+1] - xadj[i])`.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Time and memory complexity: linear = O(\p connect.entries()).
   \note          Elements in the connectivity matrix \p connect can be mixed (irrelevant nodes 
                  should be equal to `CM2_NONE`).
   */
CM2MESHTOOLS_API int 
get_connected_elements 
   (const UIntMat& connect, UIntVec& xadj, UIntVec& adjncy);                          

/**
   Computes the nodes connected to each node (nodal graph).
   
   \param[in]     connect        The connectivity matrix.
   \param[out]    xadj           The CSR indices (positions in \p adjncy where node connections are located).
   \param[out]    adjncy         The CSR connections. \n
                                 Nodes connected to node `i` (except node `i` itself) are at 
                                 `adjncy[k]` with `xadj[i] <= k < xadj[i+1]` (past-the-end) sorted in ascending order. \n
                                 You can access them also through `adjncy.sub_vector(xadj[i], xadj[i+1] - xadj[i])`.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Time and memory complexity: linear = O(\p connect.entries()).
   \note          Elements in \p connect can be mixed (irrelevant nodes should be equal to `CM2_NONE`).
   */
CM2MESHTOOLS_API int 
get_node_graph 
   (const UIntMat& connect, UIntVec& xadj, UIntVec& adjncy);                          


/**
   Retrieves the elements sharing at least one specified boundary.

   \param[in]     connectB       The connectivity matrix of the boundaries. \n
                                 Only linear elements are allowed.
   \param[in]     connectM       The connectivity matrix of the elements. \n
                                 Only linear elements are allowed.
   \param[in]     neighbors      The neighbors matrix defined by: 
                                    `neighbors(i,j)` = neighbor of element `j` by the boundary `i`. \n
                                 This array is provided by all CM2 meshers or can be computed with meshtools::get_neighbors.
   \param[in]     ancestors      The ancestors vector defined by:
                                    `ancestors(i)` = any element connected to `i`. \n
                                 This array is provided by all CM2 meshers or can be computed with meshtools::get_ancestors.
   \param[in]     FE_type        The type of the elements (\ref fe_geo_page). \n
                                 Only linear elements are allowed: cm2::CM2_EDGE2, cm2::CM2_FACET3, cm2::CM2_FACEQ4,
                                 cm2::CM2_FACE_MIX, cm2::CM2_TETRA4, cm2::CM2_PYRAMID5, cm2::CM2_WEDGE6, cm2::CM2_HEXA8,
                                 cm2::CM2_SOLID_MIX.
   \param[out]    KBs            The elements and their boundary ID sharing at least one boundary in \p connectB. \n
                                 Elements and boundary IDs are stored in compressed form:
                                    `KBs[i] = Ki * nbr_elfe_boundaries + bi` \n
                                 where:
                                    - `Ki`: ID of the element = the column ID in \p connectM of an element with
                                       a boundary similar to `connectB.col(i)`.
                                    - `bi`: local boundary ID of element `Ki`.
                                    .
                                 Elements and local boundary IDs can be retrieved using the following simple formulas:
                                 \code
                                    Ki = KBs[i] / nbr_elfe_boundaries;
                                    bi = KBs[i] % nbr_elfe_boundaries;
                                 \endcode
                                 `KBs[i] = CM2_NONE` when there is no such element in \p connectM. \n
                                 If several elements of \p connectM share a similar (internal) boundary in
                                 \p connectB, the returned element ID is the one with boundary
                                 with same orientation. \n
                                 If interested only in Ks, just divide \p KBs by \p nbr_elfe_boundaries:
                                 `vecscal::div(nbr_elfe_boundaries, KBs;`
   \param[out]    nbr_elfe_boundaries  The number of boundaries considered for the type of element. \n
                                 May be different from meshtools::nbr_boundaries(\p FE_type) in case of mixed type. \n
                                 For instance if \p FE_type = cm2::CM2_FACE_MIX and \p connectM.rows() == 3,
                                 elements will be considered as triangles and \p nbr_elfe_boundaries = 3 upon exit
                                 (instead of 4 normally for cm2::CM2_FACE_MIX).
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect.cols() == \p neighbors.cols()
   \pre           \p connect has the right number of rows (= number of nodes defined by \p FE_type) or empty. \n
                  Exceptions: 
                     - When \p FE_type = cm2::CM2_FACE_MIX, \p connect can be also a pure-triangle mesh (3-row matrix) or a pure-quad mesh.
                     - When \p FE_type = cm2::CM2_SOLID_MIX, \p connect can be also a pure-tetrahedron mesh (4-row matrix), 
                       a pure-pyramid mesh (5-row matrix), a pure-wedge mesh (6-row matrix) or a pure-hex mesh.
                     .
                  You shouldn't however set for instance \p FE_type = cm2::CM2_SOLID_MIX and \p connect be a 5-row matrix 
                  with a mix of tetrahedrons and pyramids (must be pure pyramid).
   \pre           \p connectB has the right number of rows (= number of nodes of the boundary type of \p FE_type) or empty.
   \pre           \p neighbors has the right number of rows (= number of boundaries defined by \p FE_type) or empty.
   \post          \p KBs.size() == \p connectB.cols().
   \note          The mesh can be a mixed type (cm2::CM2_FACE_MIX or cm2::CM2_SOLID_MIX).
   \note          Time and memory complexity: linear = O(\p connectB.cols()).
   \see           meshtools::get_mesh_boundaries, meshtools::get_mesh_bounding_elements, meshtools::nbr_boundaries.
   */
CM2MESHTOOLS_API int
get_elements_on_boundaries
   (const UIntMat& connectB,
    const UIntMat& connectM, const UIntMat& neighbors, const UIntVec& ancestors, cm2::element_type FE_type, 
    UIntVec& KBs, unsigned& nbr_elfe_boundaries);                          
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

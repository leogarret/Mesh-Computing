/**
   \file       subdomains.h
   \brief      Declarations of some routines to compute the subdomains in a mesh.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_SUBDOMAINS_H__
#define __CM2_SUBDOMAINS_H__


namespace cm2 {
namespace meshtools {


/****************************************************************************************/
/** @name Subdomains 

    A "subdomain" is a subset of connected elements.
    Two elements are in the same subdomain when there is at least one
    element-path between them crossing no boundary.
    */

//@{

/**
   Finds the groups of connected elements (manifold subdomains).

   Two elements are said to be _connected_ (i.e. in the same subdomain)
   when there is a path between them. 

   The element boundaries are:
      - Faces for 3-D elements.
      - Edges for 2-D elements.
      - Nodes for 1-D elements.

   The \p neighbors matrix is defined by: 

      `neighbors(i,j)` = neighbor of element `j` by the boundary `i`.
   
   \param[in]     neighbors         The neighbors matrix.
                                    This matrix can be computed with meshtools::get_neighbors.
   \param[out]    ordered_elements  The elements IDs ordered as subdomains.
                                    The elements of the first subdomain are at the beginning of this vector,
                                    then the elements of the second subdomain (if any) and so on.
                                    The initial content of this vector is lost.
                                    Upon exit, the size of this vector equals to NEFS = `neighbors.cols()`.
   \param[out]    begin_parts       The indices in ordered_elements of the beginning of the subdomains.
                                    Part `i` starts at `ordered_elements[begin_parts[i]]` and ends at
                                    `ordered_elements[begin_parts[i+1]-1]`. \n
                                    If the number of subdomains is `PARTS`,
                                    the size of this vector equals upon exit to `PARTS + 1`.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \warning       The total domain should be _manifold_ (the element boundaries should be shared
                  by no more than two elements).
   \note          Faces or solid elements connected by nodes only may not be in the same subdomain. 
   \note          Solid and face/edge elements will never be in the same subdomain.
   \note          Face and edge elements will never be in the same subdomain.
   \see           \ref fe_geo_page for the definition of the local nodes and boundaries ordering.
   */
CM2MESHTOOLS_API int 
get_subdomains (const UIntMat& neighbors, UIntVec& ordered_elements, UIntVec& begin_parts);                          


/**
   Reorders the elements in a mesh. 

   Elements in the same subdomain are reordered consecutively in the connect matrix.
   The elements of the first subdomain are moved to the beginning of matrix \p connect,
   then the elements of the second subdomain (if any) and so on.

   Two elements are said to be _connected_ (i.e. in the same subdomain)
   when there is a path between them. 

   The \p neighbors matrix is reordered and modified accordingly. \n
   The \p neighbors matrix is defined by: \n
      `neighbors(i,j)` = neighbor of element `j` by the boundary `i`.
   
   The mesh of subdomain `i` can be found in the global \p connect matrix with:
   \code
      UIntMat  connect_i, neighbors_i;
      unsigned beg  = begin_parts[i];
      unsigned nefs = begin_parts[i+1] - beg;
      unsigned nods = connect.rows();
      connect_i   = connect.sub_matrix (0, beg, nods, nefs);
      neighbors_i = neighbors.sub_matrix (0, beg, nods, nefs);
   \endcode

   \param[in,out] connect           The connectivity matrix (`NEFS` elements in `PARTS` subdomains).
   \param[in,out] neighbors         The neighbors matrix.
                                    Upon entry, this matrix can be computed with meshtools::get_neighbors.
   \param[out]    begin_parts       The indices in ordered_elements of the beginning of the subdomains.
                                    Subdomain `i` starts at `begin_parts[i]` and ends at `begin_parts[i+1]-1`.
                                    Upon exit, the size of this vector equals to `PARTS+1`.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \see           \ref fe_geo_page for the definition of the local nodes and boundaries ordering.
   */
CM2MESHTOOLS_API int 
get_subdomains (UIntMat& connect, UIntMat& neighbors, UIntVec& begin_parts);                          

/**
   Finds the boundaries between elements of different colors.

   \param[in]     connect           The connectivity matrix of the elements.
   \param[in]     neighbors         The neighbors matrix defined by: 
                                       `neighbors(i,j)` = neighbor of element `j` by the boundary `i`.
                                    This matrix can be computed with meshtools::get_neighbors or be
                                    an output of a CM2 mesher.
   \param[in]     colors            The "colors" of the elements.
   \param[in]     FE_type           The type of the elements (\ref fe_geo_page). 
   \param[in]     sym_flag          For intern boundaries: 
                                       - True: consider both boundaries (with opposite orientations).
                                       - False: consider only one (with arbitrary orientation).
                                       .
   \param[in,out] boundaries        Appended (as connectivity): boundaries between elements of different colors 
                                    and external boundaries.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          External boundaries are also appended (considered as boundaries between a color
                  and the "void" color).
   \note          Intern boundaries are pushed twice, with opposite orientations.
   \pre           \p connect has the right number of rows (= number of nodes defined by \p FE_type) or empty.
   \pre           \p neighbors has the right number of rows (= number of boundaries defined by \p FE_type) or empty.
   \pre           \p connect.cols() == \p neighbors.cols()
   \pre           \p connect.cols() == \p colors.size().
   \pre           Upon entry, \p boundaries is empty or has the right number of rows 
                  (= number of nodes of boundary type).
   \see           \ref fe_geo_page for the definition of the local nodes and boundaries ordering.
   */
CM2MESHTOOLS_API int 
get_colors_boundaries 
   (const UIntMat& connect, const UIntMat& neighbors, const UIntVec& colors, 
    cm2::element_type FE_type, bool sym_flag, 
    UIntMat& boundaries);                          

/**
   Reorders a mixed connectivity matrix according to the number of nodes of the elements.

   In a mixed quad/triangle matrix for instance (matrix of dimensions 4xNEFS in which the 4-th nodes of the triangles are set to 
   `CM2_NONE`), quads are moved to the beginning of the matrix and triangles are moved to the end.

   In a mixed 3-D matrix (matrix of dimensions 8xNEFS in which the 5-th, 6-th, 7-th and 8-th nodes of the tets are set to `CM2_NONE`, 
   the 6-th, 7-th and 8-th nodes of pyramids are set to `CM2_NONE` and the 7-th and 8-th nodes of wedges are set to `CM2_NONE`), 
   hexes are moved to the beginning of the matrix, then wedges, then pyramids, then tets.

   Upon exit the elements are in the following order:
      - In 2-D: quadrangles, triangles.
      - In 3-D: hexes, wedges, pyramids, tetrahedrons.

   \param[in,out] connect           The connectivity matrix. \n
                                    Upon exit, elements are ordered as decreasing number of nodes.
   \param[out]    nbr_nodes         Gives the number of local nodes for each group of similar elements. \n
                                    `nbr_nodes[i]` is the number of nodes of the elements of group `i`. \n
                                    This vector has size equal to the number of groups (i.e. number of
                                    different types of element).
   \param[out]    past_group        Gives the past-the-end column for each group of similar elements. \n
                                    `past_group[i]` is the past-the-end ID (i.e. column ID in \p connect)
                                    of the elements of group `i`. \n
                                    Group 0 goes from 0 to `past_group[0]-1`. \n
                                    Group `i` (i > 0) goes from `past_group[i-1]` to `past_group[i]-1`.
   \param[out]    permutation_IDs   The permutation array. \n
                                    Element `i` initially at column `i` in \p connect 
                                    is now at column \p permutation_IDs[i] in \p connect.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          \p nbr_nodes.size() == \p past_group.size().
   \post          \p permutation_IDs.size() == \p connect.cols().
   \see           \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS_API int 
sort_elfes (UIntMat& connect, 
            UIntVec& nbr_nodes, UIntVec& past_group, UIntVec& permutation_IDs);                          

/**
   Reorders a mixed connectivity matrix according to the number of nodes of the elements,
   with neighbors matrix.

   Same as before but change also the neighbors matrix accordingly.

   \param[in,out] connect           The connectivity matrix. \n
                                    Upon exit, elements are ordered as decreasing number of nodes.
   \param[in,out] neighbors         The neighbor matrix. 
   \param[out]    nbr_nodes         Gives the number of local nodes for each group of similar elements. \n
                                    `nbr_nodes[i]` is the number of nodes of the elements of group `i`. \n
                                    This vector has size equal to the number of groups (i.e. number of
                                    different types of element).
   \param[out]    past_group        Gives the past-the-end column for each group of similar elements. \n
                                    past_group[i] is the past-the-end ID (i.e. column ID in \p connect and \p neighbors)
                                    of the elements of group `i`. \n
                                    Group 0 goes from 0 to `past_group[0]-1`. \n
                                    Group `i` (i > 0) goes from `past_group[i-1]` to `past_group[i]-1`.
   \param[out]    permutation_IDs   The permutation array. \n
                                    Element `i` initially at column `i` in \p connect 
                                    is now at column `permutation_IDs[i]` in \p connect.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \see           \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS_API int 
sort_elfes (UIntMat& connect, UIntMat& neighbors,
            UIntVec& nbr_nodes, UIntVec& past_group, UIntVec& permutation_IDs);                          
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

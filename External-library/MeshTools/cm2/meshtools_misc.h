/**
   \file       meshtools_misc.h
   \brief      Declarations of some miscellaneous meshtools routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS_MISC_H__
#define __CM2_MESHTOOLS_MISC_H__


namespace cm2 {
namespace meshtools {

/**
   \ingroup FEtypes

   Gives the dimension of a type of element (0, 1, 2 or 3).

   Returns:
      -  0 for cm2::CM2_UNKNOWN_ELEMENT_TYPE, cm2::CM2_NODE.
      -  1 for edge elements.
      -  2 for face elements.
      -  3 for volume elements.

   \see  \ref fe_geo_page.
   */
CM2MESHTOOLS_API unsigned
element_dim (cm2::element_type FE);

/**
   \ingroup FEtypes

   Gives the number of nodes for a type of element.

   \see  \ref fe_geo_page.
   */
CM2MESHTOOLS_API unsigned
nbr_nodes (cm2::element_type FE);

/**
   \ingroup FEtypes

   Gives the number of base nodes for a type of element.

   Base nodes are the nodes of the associated linear element.

   This function is equivalent to cm2::meshtools::nbr_nodes for linear elements 
   (`CM2_NODE, `CM2_EDGE2`, `CM2_FACET3`, `CM2_FACEQ4`, `CM2_TETRA4`, `CM2_WEDGE6`, `CM2_HEXA8`).

   However, for quadratic elements:
      - cm2::CM2_EDGE3       => 2
      - cm2::CM2_FACET6      => 3
      - cm2::CM2_FACEQ9      => 4
      - cm2::CM2_TETRA10     => 4
      - cm2::CM2_PYRAMID14   => 5
      - cm2::CM2_WEDGE18     => 6
      - cm2::CM2_HEXA27      => 8

   \see  \ref fe_geo_page.
   */
CM2MESHTOOLS_API unsigned
nbr_base_nodes (cm2::element_type FE);

/**
   \ingroup FEtypes

   Gives the number of boundaries for a type of element.

   Returns:
      -  0 for cm2::CM2_UNKNOWN_ELEMENT_TYPE, cm2::CM2_NODE.
      -  2 for edges.
      -  3 for triangles.
      -  4 for quads, tetrahedrons and cm2::CM2_FACE_MIX.
      -  5 for pyramids and wedges.
      -  6 for hexahedrons and cm2::CM2_SOLID_MIX.

   \see  \ref fe_geo_page.
   */
CM2MESHTOOLS_API unsigned
nbr_boundaries (cm2::element_type FE);


/****************************************************************************************/
/** @name IDs checking */
//@{

/**
   Return the highest ID different than `CM2_NONE`.

   \param[in]     indices     The set of indices to be searched.
   \return        The highest ID (`CM2_NONE` if empty).
   */
CM2MESHTOOLS_API unsigned 
max_ID (const UIntVec& indices);                          

/**
   Return the highest ID different than `CM2_NONE`.

   \param[in]     connect     A connectivity matrix of indices to be searched.
   \return        The highest ID (`CM2_NONE` if empty).
   */
CM2MESHTOOLS_API unsigned 
max_ID (const UIntMat& connect);                          

/**
   Checks that all the input indices are lesser than a given value.

   \param[in]     indices     The set of indices to be checked.
   \param[in]     lesser_than The max threshold for the indices.
   \return        True when all indices are lesser than the threshold (or when empty).

   \note          The special index `CM2_NONE` (i.e. `unsigned(-1)`) is not checked.
   */
CM2MESHTOOLS_API bool 
check_IDs (const UIntVec& indices, unsigned lesser_than);                          

/**
   Checks that all the indices in a connectivity matrix are lesser than a given value.

   \param[in]     connect     A connectivity matrix of indices to be checked.
   \param[in]     lesser_than The max threshold for the indices.
   \return        True when all indices are lesser than the threshold (or when empty).

   \note          The special index `CM2_NONE` (i.e. `unsigned(-1)`) is not checked.
   */
CM2MESHTOOLS_API bool 
check_IDs (const UIntMat& connect, unsigned lesser_than);                          

/**
   Checks that a connectivity matrix is valid and that all its indices are lesser than a given value.

   \param[in]     connect     A connectivity matrix of indices to be checked.
   \param[in]     FE_type     The type of the elements (see \ref fe_geo_page).
   \param[in]     lesser_than The max threshold for the indices. 
   \return        True when all indices are lesser than the threshold and that the number
                  of rows match the element type (or when the matrix is empty).
   */
CM2MESHTOOLS_API bool 
check_IDs (const UIntMat& connect, cm2::element_type FE_type, unsigned lesser_than);                          
//@}


/****************************************************************************************/
/** @name IDs retrieving */
//@{

/**
   Finds positions of values in two vectors.

   \param[in]     V0          The first vector to compare.
   \param[in]     V1          The second vector to compare.
   \param[out]    IDs         The IDs of the elements of \p V1 in \p V0.
                              Element `j` in \p V1 is at position `IDs[j]` in \p V0.
   \param[out]    inv_IDs     The reciprocal vector of \p IDs.
                              Element `j` in \p V0 is at column `inv_IDs[j]` in \p V1.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          \p IDs.size() == \p V1.size()
   \post          \p inv_IDs.size() == \p V0.size()

   \note          The special index `CM2_NONE` (i.e. `unsigned(-1)`) is used for
                  elements not present in the other vector.
   \note          In case of duplicate values `v0` in \p V0, the matching index in \p IDs, if any, 
                  is the lowest index of `v0` in \p V0.
   \note          In case of duplicate values `v1` in \p V1, the matching index in \p inv_IDs, if any, 
                  is the lowest index of `v1` in \p V1.
   */
CM2MESHTOOLS_API int 
get_IDs (const UIntVec& V0, const UIntVec& V1, UIntVec& IDs, UIntVec& inv_IDs);                          

/**
   Finds positions of columns in two matrices.

   \param[in]     M0          The first matrix to compare.
   \param[in]     M1          The second matrix to compare.
   \param[out]    IDs         The IDs of the columns of \p M1 in \p M0.
                              Column `j` in \p M1 matches column `IDs[j]` in \p M0.
   \param[out]    inv_IDs     The reciprocal vector of \p IDs.
                              Column `j` in \p M0 matches column `inv_IDs[j]` in \p M1.
   \param[in]     strict_compare_flag  If false: columns [1 2 3 4] and [3 4 1 2] are considered
                              equal to each other (any rotation of indices is allowed).
                              If true: the columns must be strictly equal to each other 
                              (same values and same order).

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p M0.rows() == \p M1.rows()
   \post          \p IDs.size() == \p M1.cols()
   \post          \p inv_IDs.size() == \p M0.cols()

   \note          The special index `CM2_NONE` (i.e. `unsigned(-1)`) is used for
                  elements not present in the other matrix.
   \note          In case of duplicate matching vectors in \p M0, the index in \p IDs
                  is the lowest of these column IDs in \p M0.
   \note          In case of duplicate matching vectors in \p M1, the index in \p inv_IDs
                  is the lowest of these column IDs in \p M1.
   */
CM2MESHTOOLS_API int 
get_IDs (const UIntMat& M0, const UIntMat& M1, UIntVec& IDs, UIntVec& inv_IDs,
         bool strict_compare_flag);                          
//@}


/****************************************************************************************/
/** @name Value changes and permutations */
//@{

/**
   Shift indices in a vector (for instance before appending a new mesh to an old one).

   This function adds a shift to all valid indices (i.e. values not equal to `CM2_NONE`).

   \param[in,out] IDs            The indices.
                                 All indices different from `CM2_NONE` are shifted.
   \param[in]     shift          The shift for the indices.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          When all initial values in \p IDs are different from `CM2_NONE`, 
                  this function is equivalent to `vecscal::add(shift, IDs)`. \n
                  `CM2_NONE` values are not shifted.
   */
CM2MESHTOOLS_API int 
shift_indices (UIntVec& IDs, int shift);

/**
   Shift indices in a connectivity matrix (for instance before appending a new mesh to an old one).

   This function adds a shift to all valid node indices of the connectivity matrix (i.e. values not equal to `CM2_NONE`).

   \param[in,out] connect        The connectivity matrix.
                                 All indices different from `CM2_NONE` are shifted.
   \param[in]     shift          The shift for the indices.
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page).

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Pure meshes (with FE_type not equal to cm2::CM2_FACE_MIX and cm2::CM2_SOLID_MIX)
                  are assumed to contain no `CM2_NONE` values. \n
                  In these cases, this function is equivalent to `matscal::add(shift, connect)`. \n
                  A special treatment is used only in the mixed cases (cm2::CM2_FACE_MIX and cm2::CM2_SOLID_MIX)
                  to keep the `CM2_NONE` values unchanged.
   */
CM2MESHTOOLS_API int 
shift_indices (UIntMat& connect, int shift, cm2::element_type FE_type);

/**
   Changes the values in a 1-D array.

   \param[in,out] vals           The array to be modified.
                                 `vals[i]` is changed to `new_vals[vals[i]]`
                                 (if `vals[i] < new_vals.size()`, otherwise not changed).
   \param[in]     new_vals       The new values.

   \post    The size of the vector \p vals is not modified.
   */
CM2MESHTOOLS_API void
change_indices (UIntVec& vals, const UIntVec& new_vals);

/**
   Changes the values in a 2-D array.

   \param[in,out] vals           The array to be modified.
                                 `vals(i,j)` is changed to `new_vals[vals(i,j)]` 
                                 (if `vals(i,j) < new_vals.size()`, otherwise not changed).
   \param[in]     new_vals       The new values.

   \post    The dimensions of the matrix \p vals are not modified.
   */
CM2MESHTOOLS_API void
change_indices (UIntMat& vals, const UIntVec& new_vals);

/**
   Permutes the values in a 1-D array (UIntVec).

   \param[in,out] vals           The array to be modified.
                                 Value at index `i` is moved to index `new_indices[i]`
                                 (if `new_indices[i] < vals.size()`, otherwise not moved).
   \param[in]     new_indices    The permutation array.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post    The size of the vector \p vals is not modified.
   */
CM2MESHTOOLS_API int
permutation (UIntVec& vals, const UIntVec& new_indices);

/**
   Permutes the values in a 1-D array (DoubleVec).

   \param[in,out] vals           The array to be modified.
                                 Value at index `i` is moved to index `new_indices[i]`
                                 (if `new_indices[i] < vals.size()`, otherwise not moved).
   \param[in]     new_indices    The permutation array.
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post    The size of the vector \p vals is not modified.
   */
CM2MESHTOOLS_API int
permutation (DoubleVec& vals, const UIntVec& new_indices);

/**
   Gets the order of elements according to their values.

   The mesh generators in CM2 MeshTools provide an array of `colors` indicating 
   which subdomain each element belongs to. \n
   These `colors` arrays (indexed by element IDs) are not sorted by increasing `color` value. \n
   For example, in a mesh of 5 elements and two subdomains, the `colors` array may be: \n
      `colors = [0 0 1 1 0]` \n
   indicating that elements 0, 1 and 4 are in subdomain #0 and elements 2 and 3 are in subdomain #1.

   This function can be used to gather elements by similar `color`.

   With the previous example, a call to `get_sorted_order` gives: 
      - `order = [0 1 4 2 3]` (the elements IDs sorted by increasing value).
      - `xOrder = [0 3 5]` (the beginnings in `order` for each different value). 
        Hence, the elements with value 0 are between `order[xOrder[0]]` and `order[xOrder[1]]`
        (i.e. between `order[0] and `order[3]`, which are IDs `[0 1 4]`).

   \param[in]     values         The values array.
   \param[out]    order          The order of element IDs sorted by increasing value.
   \param[out]    xOrder         The beginnings of each different value in \p order. \n
                                 Element IDs with same value `v` are
                                 between `order[xOrder[v]]` and `order[xOrder[v+1]]` (past-the-end).

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post    \p order.size() == \p values.size().
   \post    \p xOrder.size() == max value in \p values + 1.
   */
CM2MESHTOOLS_API int
get_sorted_order (const UIntVec& values, UIntVec& order, UIntVec& xOrder);
//@}


/****************************************************************************************/
/** @name Bounding box */
//@{

/**
   Enlarges a 2-D bounding box to enclose a set of points.

   To get the bounding box of the set of points, starts with an initial box such as:

   \verbatim
      minBox = {+DBL_MAX, +DBL_MAX}
      maxBox = {-DBL_MAX, -DBL_MAX}
   \endverbatim
   
   \param[in]     pos            The coordinates matrix.
   \param[in,out] minBox         The point of the bounding box with minimum X, Y coordinates.
   \param[in,out] maxBox         The point of the bounding box with maximum X, Y coordinates.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        \p pos.rows() == 2
   \post       Upon exit, the coordinates of \p minBox are lesser or equal to that upon entry.
   \post       Upon exit, the coordinates of \p maxBox are greater or equal to that upon entry.
   */
CM2MESHTOOLS_API int 
inc_bounding_box (const DoubleMat& pos, DoubleVec2& minBox, DoubleVec2& maxBox);                          

/**
   Enlarges a 2-D bounding box to enclose a set of points.

   To get the bounding box of the set of points, starts with an initial box such as:

   \verbatim
      minBox = {+DBL_MAX, +DBL_MAX}
      maxBox = {-DBL_MAX, -DBL_MAX}
   \endverbatim
   
   \param[in]     pos         The coordinates matrix.
   \param[in]     indices     The indices of the points.
   \param[in,out] minBox      The point of the bounding box with minimum X, Y coordinates.
   \param[in,out] maxBox      The point of the bounding box with maximum X, Y coordinates.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        \p pos.rows() == 2
   \pre        `vecscal::max_value(indices) < pos.cols()`
   \post       Upon exit, the coordinates of \p minBox are lesser or equal to that upon entry.
   \post       Upon exit, the coordinates of \p maxBox are greater or equal to that upon entry.
   */
CM2MESHTOOLS_API int 
inc_bounding_box (const DoubleMat& pos, const UIntVec& indices,
                  DoubleVec2& minBox, DoubleVec2& maxBox);                          

/**
   Enlarges a 2-D bounding box to enclose a mesh.

   To get the bounding box of the mesh, starts with an initial box such as:
   \verbatim
      minBox = {DBL_MAX, DBL_MAX, DBL_MAX}
      maxBox = {-DBL_MAX, -DBL_MAX, -DBL_MAX}
   \endverbatim
   
   \param[in]     pos         The coordinates matrix.
   \param[in]     connect     The connectivity matrix.
   \param[in,out] minBox      The point of the bounding box with minimum X, Y coordinates.
   \param[in,out] maxBox      The point of the bounding box with maximum X, Y coordinates.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        \p pos.rows() == 2
   \pre        `matscal::max_value(connect) < pos.cols()`
   \post       Upon exit, the coordinates of \p minBox are lesser or equal to that upon entry.
   \post       Upon exit, the coordinates of \p maxBox are greater or equal to that upon entry.
   */
CM2MESHTOOLS_API int 
inc_bounding_box (const DoubleMat& pos, const UIntMat& connect,
                  DoubleVec2& minBox, DoubleVec2& maxBox);                          

/**
   Enlarges a 3-D bounding box to enclose a set of points.

   To get the bounding box of the set of points, starts with an initial box such as:

   \verbatim
      minBox = {DBL_MAX, DBL_MAX, DBL_MAX}
      maxBox = {-DBL_MAX, -DBL_MAX, -DBL_MAX}
   \endverbatim
   
   \param[in]     pos         The coordinates matrix.
   \param[in,out] minBox      The point of the bounding box with minimum X, Y, Z coordinates.
   \param[in,out] maxBox      The point of the bounding box with maximum X, Y, Z coordinates.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        \p pos.rows() == 3
   \post       Upon exit, the coordinates of \p minBox are lesser or equal to that upon entry.
   \post       Upon exit, the coordinates of \p maxBox are greater or equal to that upon entry.
   */
CM2MESHTOOLS_API int 
inc_bounding_box (const DoubleMat& pos,
                  DoubleVec3& minBox, DoubleVec3& maxBox);                          

/**
   Enlarges a 3-D bounding box to enclose a set of points.

   To get the bounding box of the set of points, starts with an initial box such as:

   \verbatim
      minBox = {DBL_MAX, DBL_MAX, DBL_MAX}
      maxBox = {-DBL_MAX, -DBL_MAX, -DBL_MAX}
   \endverbatim
   
   \param[in]     pos         The coordinates matrix.
   \param[in]     indices     The indices of the points.
   \param[in,out] minBox      The point of the bounding box with minimum X, Y, Z coordinates.
   \param[in,out] maxBox      The point of the bounding box with maximum X, Y, Z coordinates.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        \p pos.rows() == 3
   \pre        `vecscal::max_value(indices) < pos.cols()`
   \post       Upon exit, the coordinates of \p minBox are lesser or equal to that upon entry.
   \post       Upon exit, the coordinates of \p maxBox are greater or equal to that upon entry.
   */
CM2MESHTOOLS_API int 
inc_bounding_box (const DoubleMat& pos, const UIntVec& indices,
                  DoubleVec3& minBox, DoubleVec3& maxBox);                          

/**
   Enlarges a 3-D bounding box to enclose a mesh.

   To get the bounding box of the mesh, starts with an initial box such as:

   \verbatim
      minBox = {DBL_MAX, DBL_MAX, DBL_MAX}
      maxBox = {-DBL_MAX, -DBL_MAX, -DBL_MAX}
   \endverbatim
   
   \param[in]     pos         The coordinates matrix.
   \param[in]     connect     The connectivity matrix.
   \param[in,out] minBox      The point of the bounding box with minimum X, Y, Z coordinates.
   \param[in,out] maxBox      The point of the bounding box with maximum X, Y, Z coordinates.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        \p pos.rows() == 3
   \pre        `matscal::max_value(connect) < pos.cols()`
   \post       Upon exit, the coordinates of \p minBox are lesser or equal to that upon entry.
   \post       Upon exit, the coordinates of \p maxBox are greater or equal to that upon entry.
   */
CM2MESHTOOLS_API int 
inc_bounding_box (const DoubleMat& pos, const UIntMat& connect,
                  DoubleVec3& minBox, DoubleVec3& maxBox);                          
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

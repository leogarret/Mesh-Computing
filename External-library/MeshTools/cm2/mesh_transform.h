/**
   \file       mesh_transform.h
   \brief      Definitions of the mesh transformation routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3608 $
   $Date: 2019-04-25 09:58:41 +0200 (jeu., 25 avr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_TRANSFORM_H__
#define __CM2_MESH_TRANSFORM_H__


namespace cm2 {
namespace meshtools {

/****************************************************************************************/
/** @name Points/mesh translation */
//@{

/**
   Translates all nodes. 2-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     T           The translation vector.
    
   \return        The number of points effectively translated (i.e. \p pos.cols() if \p T != 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   */
CM2MESHTOOLS_API int 
translate (DoubleMat& pos, const DoubleVec2& T);

/**
   Translates a batch of nodes. 2-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     T           The translation vector.
   \param[in]     indices     The indices of the points to be translated.

   \return        The number of points effectively translated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the vector \p indices are left unchanged.
   \warning       Multiple indices cause multiple translations (nodes are translated several times).
   */
CM2MESHTOOLS_API int 
translate (DoubleMat& pos, const DoubleVec2& T, const UIntVec& indices);

/**
   Translates the nodes of a mesh. 2-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     T           The translation vector.
   \param[in]     connect     The connectivity matrix of the nodes to be translated.

   \return        The number of points effectively translated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the connectivity matrix are left unchanged.
   \note          Nodes referenced several times in the connectivity matrix are translated only once.
   */
CM2MESHTOOLS_API int 
translate (DoubleMat& pos, const DoubleVec2& T, const UIntMat& connect);

/**
   Translates all nodes. 3-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     T           The translation vector.

   \return        The number of points effectively translated (i.e. \p pos.cols() if T != 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or \p pos.rows() upon entry, whatever the largest.
   */
CM2MESHTOOLS_API int 
translate (DoubleMat& pos, const DoubleVec3& T);

/**
   Translates a batch of nodes. 3-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     T           The translation vector.
   \param[in]     indices     The indices of the points to be translated.

   \return        The number of points effectively translated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the vector \p indices are left unchanged.
   \warning       Multiple indices cause multiple translations (nodes are translated several times).
   */
CM2MESHTOOLS_API int 
translate (DoubleMat& pos, const DoubleVec3& T, const UIntVec& indices);

/**
   Translates the nodes of a mesh. 3-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     T           The translation vector.
   \param[in]     connect     The connectivity matrix of the nodes to be translated.

   \return        The number of points effectively translated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the connectivity matrix are left unchanged.
   \note          Nodes referenced several times in the connectivity matrix are translated only once.
   */
CM2MESHTOOLS_API int 
translate (DoubleMat& pos, const DoubleVec3& T, const UIntMat& connect);
//@}


/****************************************************************************************/
/** @name Points/mesh rotation */
//@{

/**
   Rotates all nodes. 2-D version.

   The rotation is around the axis defined by the centre and the vector.
   The norm of the rotation vector is the angular value of the rotation (radians).

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The rotation centre.
   \param[in]     R           The rotation angle (radians unit).

   \return        The number of points effectively rotated (i.e. \p pos.cols() if \p R != 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   */
CM2MESHTOOLS_API int 
rotate (DoubleMat& pos, const DoubleVec2& C, double R);

/**
   Rotates a batch of nodes. 2-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The rotation centre.
   \param[in]     R           The rotation angle (radians unit).
   \param[in]     indices     The indices of the points to be rotated.

   \return        The number of points effectively rotated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the vector \p indices are left unchanged.
   \warning       Multiple indices cause multiple rotations (nodes are rotated several times).
   */
CM2MESHTOOLS_API int 
rotate (DoubleMat& pos, const DoubleVec2& C, double R, const UIntVec& indices);

/**
   Rotates the nodes of a mesh. 2-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The rotation centre.
   \param[in]     R           The rotation angle (radians unit).
   \param[in]     connect     The connectivity matrix of the nodes to be rotated.

   \return        The number of points effectively rotated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the connectivity matrix are left unchanged.
   \note          Nodes referenced several times in the connectivity matrix are rotated only once.
   */
CM2MESHTOOLS_API int 
rotate (DoubleMat& pos, const DoubleVec2& C, double R, const UIntMat& connect);

/**
   Rotates all nodes. 3-D version.

   The rotation is around the axis defined by the centre and the vector.
   The norm of the rotation vector is the angular value of the rotation (radians).

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The rotation centre.
   \param[in]     R           The rotation vector. The norm is the angular value (radians unit).

   \return        The number of points effectively rotated (i.e. \p pos.cols() if \p R != 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   */
CM2MESHTOOLS_API int 
rotate (DoubleMat& pos, const DoubleVec3& C, const DoubleVec3& R);

/**
   Rotates a batch of nodes. 3-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The rotation centre.
   \param[in]     R           The rotation vector. The norm is the angular value (radians unit).
   \param[in]     indices     The indices of the points to be rotated.

   \return        The number of points effectively rotated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the vector \p indices are left unchanged.
   \warning       Multiple indices cause multiple rotations (nodes are rotated several times).
   */
CM2MESHTOOLS_API int 
rotate (DoubleMat& pos, const DoubleVec3& C, const DoubleVec3& R, const UIntVec& indices);

/**
   Rotates the nodes of a mesh. 3-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The rotation centre.
   \param[in]     R           The rotation vector. The norm is the angular value (radians unit).
   \param[in]     connect     The connectivity matrix of the nodes to be rotated.

   \return        The number of points effectively rotated. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the connectivity matrix are left unchanged.
   \note          Nodes referenced several times in the connectivity matrix are rotated only once.
   */
CM2MESHTOOLS_API int 
rotate (DoubleMat& pos, const DoubleVec3& C, const DoubleVec3& R, const UIntMat& connect);
//@}


/****************************************************************************************/
/** @name Points/mesh zooming */
//@{

/**
   Zooms all nodes. 2-D version.

   Zoom nodes from a centre and with a zooming factor.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The zoom centre.
   \param[in]     Zv          The zoom factor.

   \return        The number of points effectively zoomed (i.e. \p pos.cols() if \p Zv != 1). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   */
CM2MESHTOOLS_API int 
zoom (DoubleMat& pos, const DoubleVec2& C, double Zv);

/**
   Zooms a specific batch of nodes. 2-D version.

   Zoom nodes from a centre and with a zooming factor.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The zoom centre.
   \param[in]     Zv          The zoom factor.
   \param[in]     indices     The indices of the points to be zoomed.

   \return        The number of points effectively zoomed. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the vector \p indices are left unchanged.
   \warning       Multiple indices cause multiple zooms (nodes are zoomed several times).
   */
CM2MESHTOOLS_API int 
zoom (DoubleMat& pos, const DoubleVec2& C, double Zv, const UIntVec& indices);

/**
   Zooms the nodes of a mesh. 2-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The zoom centre.
   \param[in]     Zv          The zoom factor.
   \param[in]     connect     The connectivity matrix of the nodes to be zoomed.

   \return        The number of points effectively zoomed. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 2 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the connectivity matrix are left unchanged.
   \note          Nodes referenced several times in the connectivity matrix are zoomed only once.
   */
CM2MESHTOOLS_API int 
zoom (DoubleMat& pos, const DoubleVec2& C, double Zv, const UIntMat& connect);

/**
   Zooms all nodes. 3-D version.

   Zoom nodes from a centre and with a zooming factor.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The zoom centre.
   \param[in]     Zv          The zoom factor.

   \return        The number of points effectively zoomed (i.e. \p pos.cols() if \p Zv != 1). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or pos.rows() upon entry, whatever the largest.
   */
CM2MESHTOOLS_API int 
zoom (DoubleMat& pos, const DoubleVec3& C, double Zv);

/**
   Zooms a specific batch of nodes. 3-D version.

   Zoom nodes from a centre and with a zooming factor.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The zoom centre.
   \param[in]     Zv          The zoom factor.
   \param[in]     indices     The indices of the points to be zoomed.

   \return        The number of points effectively zoomed. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the vector \p indices are left unchanged.
   \warning       Multiple indices cause multiple zooms (nodes are zoomed several times).
   */
CM2MESHTOOLS_API int 
zoom (DoubleMat& pos, const DoubleVec3& C, double Zv, const UIntVec& indices);

/**
   Zooms the nodes of a mesh. 3-D version.

   \param[in,out] pos         The coordinates matrix.
   \param[in]     C           The zoom centre.
   \param[in]     Zv          The zoom factor.
   \param[in]     connect     The connectivity matrix of the nodes to be zoomed.

   \return        The number of points effectively zoomed. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          Upon exit, the number of rows equals to 3 or \p pos.rows() upon entry, whatever the largest.
   \note          Nodes with ID greater or equal to \p pos.cols() are discarded.
   \note          Nodes not referenced in the connectivity matrix are left unchanged.
   \note          Nodes referenced several times in the connectivity matrix are zoomed only once.
   */
CM2MESHTOOLS_API int 
zoom (DoubleMat& pos, const DoubleVec3& C, double Zv, const UIntMat& connect);
//@}


/****************************************************************************************/
/** @name Points/mesh copying  */
//@{

/**
   Copies a mesh (nodes and connectivity).

   This function duplicates the nodes found in matrix \p connect_src 
   (duplicated coordinates are appended to matrix \p pos) and create the new 
   connectivity matrix \p connect_tar.

   \param[in,out] pos               The coordinates matrix. \n
                                    Upon exit, copies of the coordinates of the nodes found in \p connect_src are appended
                                    to the back of \p pos.
   \param[out]    connect_tar       The connectivity matrix of the new mesh. \n
                                    Overwrites any input matrix.
   \param[in]     connect_src       The source connectivity matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in \p connect_src are smaller than \p pos.cols(). 
   */
CM2MESHTOOLS_API int 
copy_mesh (DoubleMat& pos, UIntMat& connect_tar, const UIntMat& connect_src);

/**
   Copies and symmetrises a mesh (nodes and connectivity). 2-D version.

   This function copies and symmetrises the nodes found in  
   a connectivity matrix (mirror copy) and create the new connectivity matrix.

   \param[in,out] pos               The coordinates matrix. \n
                                    Upon exit, copies of the coordinates of the nodes found in 
                                    \p connect_src are appended to the back of \p pos.
   \param[out]    connect_tar       The connectivity matrix of the new mesh. \n
                                    Overwrites any input matrix.
   \param[in]     C                 A point on the symmetry plane.
   \param[in]     D                 A vector perpendicular to the symmetry plane (need not to be normalized).
   \param[in]     connect_src       The source connectivity matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in \p connect_src are smaller than \p pos.cols(). 
   \post          Upon exit, the number of rows equals to 2 or pos.rows() upon entry, whatever the largest.

   \warning       The orientation of the elements is inverted 
                  (except for 1-D elements parallel to the mirror plane). \n
                  Use meshtools1d::flip or meshtools2d::flip to restore the orientation.
   */
CM2MESHTOOLS_API int 
copy_sym_mesh 
   (DoubleMat& pos, UIntMat& connect_tar, 
    const DoubleVec2& C, const DoubleVec2& D, const UIntMat& connect_src);

/**
   Copies and symmetrises a mesh (nodes and connectivity). 3-D version.

   This function copies and symmetrises the nodes found in  
   a connectivity matrix (mirror copy) and create the new connectivity matrix.

   \param[in,out] pos               The coordinates matrix. \n
                                    Upon exit, copies of the coordinates of the nodes found in 
                                    \p connect_src are appended to the back of \p pos.
   \param[out]    connect_tar       The connectivity matrix of the new mesh. \n
                                    Overwrites any input matrix.
   \param[in]     C                 A point on the symmetry plane.
   \param[in]     D                 A vector perpendicular to the symmetry plane (need not to be normalized).
   \param[in]     connect_src       The source connectivity matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in \p connect_src are smaller than \p pos.cols(). 
   \post          Upon exit, the number of rows equals to 3 or pos.rows() upon entry, whatever the largest.

   \warning       The orientation of the elements is inverted 
                  (except for 1-D and 2-D elements parallel to the mirror plane). \n
                  Use meshtools1d::flip, meshtools2d::flip or meshtools3d::flip (depending on the dimension
                  of the elements) to restore the orientation.
   */
CM2MESHTOOLS_API int 
copy_sym_mesh 
   (DoubleMat& pos, UIntMat& connect_tar, 
    const DoubleVec3& C, const DoubleVec3& D, const UIntMat& connect_src);

/**
   Duplicate a mesh N-times (nodes and connectivity), translate and rotate each copy. 2-D version.

   This function duplicates the nodes found in matrix \p connect_src 
   (duplicated coordinates are appended to matrix \p pos) and create the new 
   connectivity matrix \p connect_tar.

   \param[in,out] pos               The coordinates matrix. \n
                                    Upon exit, copies of the coordinates of the nodes found in \p connect_src are appended
                                    to the back of \p pos.
   \param[out]    connect_tar       The connectivity matrix of the new meshes. \n
                                    The copies are appended to the initial matrix.
   \param[in]     T                 The translation vector. Applied to each new copy.
   \param[in]     C                 The centre of the rotation.
   \param[in]     R                 The rotation angle (radians unit). Applied to each new copy.
   \param[in]     N                 The number of copies. The initial mesh is copied N times and each copy is translated by T 
                                    and rotated by R from the previous copy.
   \param[in]     connect_src       The source connectivity matrix.

   \return        Error code. Returns 0 when successful. \n 
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in \p connect_src are smaller than \p pos.cols(). 
   \post          Upon exit, the number of rows equals to 2 or pos.rows() upon entry, whatever the largest.
   \note          Nodes are not merged between copies.
   \note          connect_tar does not contain the initial mesh connect_src. \n
                  But since the copies are appended to the initial matrix, the user can
                  put connect_src into connect_tar upon entry to have N+1 copies of connect_src.
   */
CM2MESHTOOLS_API int 
copy_mesh 
   (DoubleMat& pos, UIntMat& connect_tar, 
    const DoubleVec2& T, const DoubleVec2& C, double R, 
    unsigned N, const UIntMat& connect_src);

/**
   Duplicate a mesh (nodes and connectivity). 3-D version.

   This function duplicates the nodes found in matrix \p connect_src 
   (duplicated coordinates are appended to matrix \p pos) and create the new 
   connectivity matrix \p connect_tar.

   \param[in,out] pos               The coordinates matrix. \n
                                    Upon exit, copies of the coordinates of the nodes found in \p connect_src are appended
                                    to the back of \p pos.
   \param[out]    connect_tar       Destination connectivity matrix of the new meshes. \n
                                    The copies are appended to the initial matrix.
   \param[in]     T                 The translation vector. Applied to each new copy.
   \param[in]     C                 The centre of the rotation.
   \param[in]     R                 The rotation angle (radians unit). Applied to each new copy.
   \param[in]     N                 The number of copies. The initial mesh is copied N times and each copy is translated 
                                    by T and rotated by R from the previous copy.
   \param[in]     connect_src       The source connectivity matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in \p connect_src are smaller than \p pos.cols(). 
   \post          Upon exit, the number of rows equals to 3 or pos.rows() upon entry, whatever the largest.
   \note          Nodes are not merged between copies.
   \note          connect_tar does not contain the initial mesh connect_src. \n
                  But since the copies are appended to the initial matrix, the user can
                  put connect_src into connect_tar upon entry to have N+1 copies of connect_src.
   */
CM2MESHTOOLS_API int 
copy_mesh 
   (DoubleMat& pos, UIntMat& connect_tar, 
    const DoubleVec3& T, const DoubleVec3& C, const DoubleVec3& R, 
    unsigned N, const UIntMat& connect_src);
//@}


/****************************************************************************************/
/** @name Points/mesh merging */
//@{

/**
   Merges points that are closer to each other than a given distance.

   Let `A` and `B` be two points (i.e column index) in matrix \p pos. \n
   If `A` is close to `B` (closer than the threshold), they are replaced with a single node 
   (the smaller one).
   
   \param[in,out] pos            The coordinates matrix. \n
                                 Upon exit, the merged points have the same coordinates (copy of coordinates)
                                 but the number of points in the matrix is not changed.
   \param[out]    new_node_IDs   The new indices of the nodes. \n
                                 i-th node has now index given by `new_node_IDs[i]`. \n
                                 If not merged with another one: `new_node_IDs[i] == i`.
   \param[in]     distance       The tolerance for the merging. Must be >= 0. 

   \return        Returns the number of points merged with another (i.e. replaced with). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Merges propagate: if node `A` is merged into `B` (`AB` <= \p distance) and `B` into `C` (`BC` <= \p distance),
                  `A` is finally merged into `C` (even if `AC` > \p distance).
   \note          If several nodes are merged together, the new coordinates and indices for these nodes are those of the lowest 
                  rank (first nodes have higher priority).
   \note          The algorithm complexity is N*log(N), with N = number of columns of the matrix \p pos.

   \pre           \p distance >= 0.0
   \post          \p new_node_IDs.size() == \p pos.cols()
   */
CM2MESHTOOLS_API int 
merge (DoubleMat& pos, UIntVec& new_node_IDs, double distance);

/**
   Same as above without the `new_node_IDs` parameter.

   \param[in,out] pos            The coordinates matrix. \n
                                 Upon exit, the merged points have the same coordinates (copy of coordinates)
                                 but the number of points in the matrix is not changed.
   \param[in]     distance       The tolerance for the merging. Must be >= 0.

   \return        Returns the number of points merged with another (i.e. replaced with). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p distance >= 0.0
   */
CM2MESHTOOLS_API int 
merge (DoubleMat& pos, double distance);

/**
   Merges points that are closer to each other than a given distance.

   Replaces only nodes in \p src_node_IDs with nodes in \p tgt_node_IDs.

   \param[in]  pos            The coordinates matrix. \n
                              Upon exit, the merged points have same indices but coordinates in the matrix
                              are not changed.
   \param[in]  src_node_IDs   The nodes than can be changed with a node of \p tgt_node_IDs.
   \param[in]  tgt_node_IDs   The nodes than can take place of nodes of \p src_node_IDs.
   \param[out] new_node_IDs   The new indices of the nodes. \n
                              i-th node has now index given by `new_node_IDs[i]`. \n
                              If not merged with another one: `new_node_IDs[i] == i`.
   \param[in]  distance       The tolerance for the merging. Must be >= 0.    

   \return     Returns the number of points merged with another (i.e. replaced with). \n
               Returns a negative value in case of error. \n
               Returned value = -k => the k-th argument had an illegal value.

   \pre        \p distance >= 0.0
   \post       \p new_node_IDs.size() == \p pos.cols()

   \note       Merges propagate: if node `A` is merged into `B` (`AB` <= \p distance) and `B` into `C` (`BC` <= \p distance),
               `A` is finally merged into `C` (even if `AC` > \p distance).
   \note       If several nodes are merged together, the new index of these nodes is the one with
               lowest rank in \p tgt_node_IDs (first nodes in \p tgt_node_IDs have higher priority).
   \note       The algorithm's complexity is Nt * log(Ns), with Nt = `tgt_node_IDs.size()` and Ns = `src_node_IDs.size()`.
   \note       Vectors \p src_node_IDs and \p tgt_node_IDs can share same nodes. They can even be equal.
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, const UIntVec& src_node_IDs, const UIntVec& tgt_node_IDs, 
       UIntVec& new_node_IDs, double distance);
    
/**
   Merges points that are closer to each other than a given distance.

   Same as above but with same arrays of nodes IDs (source = target).

   Replace nodes only in \p node_IDs with nodes in \p node_IDs.

   \param[in]  pos            The coordinates matrix. \n
                              Upon exit, the merged points have same index but coordinates in the matrix
                              are not changed.
   \param[in]  node_IDs       The nodes than can be merged.
   \param[out] new_node_IDs   The new indices of the nodes. \n
                              i-th node has now index given by `new_node_IDs[i]`. \n
                              If not merged with another one: `new_node_IDs[i] == i`.
   \param[in]  distance       The tolerance for the merging. Must be >= 0.    

   \return     Returns the number of points merged with another (i.e. replaced with). \n
               Returns a negative value in case of error. \n
               Returned value = -k => the k-th argument had an illegal value.

   \pre        \p distance >= 0.0
   \post       \p new_node_IDs.size() == \p pos.cols()
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, const UIntVec& node_IDs, UIntVec& new_node_IDs, double distance);
    
/**
   Merges nodes in a mesh that are closer to each other than a given distance.
   
   \param[in]     pos            The coordinates matrix. \n
                                 Upon exit, the merged points have same index but coordinates in the matrix
                                 are not changed.
   \param[in,out] connect        The connectivity matrix.
   \param[out]    new_node_IDs   The new indices of the nodes. \n
                                 i-th node has now index given by `new_node_IDs[i]`. \n
                                 If not merged with another one: `new_node_IDs[i] == i`.
   \param[in]     distance       The tolerance for the merging. Must be >= 0.         
   \param[in]     merge_type     Control the way nodes can be merged:
                                   -#  0 : The algorithm will merge nodes that belong to any element (inter merge). 
                                   -#  1 : The algorithm will only merge nodes that belong to the same element (intra merge).
                                   -#  2 : The algorithm for merge_type = 1 is repeated until no pair of nodes can 
                                           be merged anymore.        

   \return        Returns the number of merged nodes. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Merges propagate (\p merge_type = 0 and 2): 
                  if node `A` is merged into `B` (`AB` <= \p distance) and `B` into `C` (`BC` <= \p distance),
                  `A` is finally merged into `C` (even if `AC` > \p distance).
   \note          Points in matrix \p pos that are not referenced in matrix \p connect are never merged, even if 
                  they are close enough.
   \note          If several nodes are merged together, the new coordinates and index of these nodes are those of the
                  lowest rank node in \p connect (first nodes in \p connect have higher priority).
   \note          The algorithm complexity is N*log(N), with N = number of nodes in the matrix \p connect.
   \note          The distance must be chosen small enough to avoid mesh collapsing,
                  and big enough to successfully detect close points.
   \note          Elements with multiple identical nodes are not eliminated
                  from the connectivity matrix. \n
                  To remove them, use meshtools::remove_degenerated. \n
                  To remove no longer used nodes, use meshtools::simplify.
                                   
   \pre        \p distance >= 0.0
   \post       \p new_node_IDs.size() == \p pos.cols()
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, UIntMat& connect, UIntVec& new_node_IDs, 
       double distance, unsigned merge_type);

/**
   Same as above without the `new_node_IDs` parameter.

   \param[in]     pos            The coordinates matrix. \n
                                 Upon exit, the merged points have same index but coordinates in the matrix
                                 are not changed.
   \param[in,out] connect        The connectivity matrix.
   \param[in]     distance       The tolerance for the merging. Must be >= 0. 
   \param[in]     merge_type     Control the way nodes can be merged:
                                   -#  0 : The algorithm will merge nodes that belong to any element (inter merge). 
                                   -#  1 : The algorithm will only merge nodes that belong to the same element (intra merge).
                                   -#  2 : The algorithm for merge_type = 1 is repeated until no pair of nodes can 
                                           be merged anymore.                                                      

   \return        Returns the number of merged nodes. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.
                                   
   \pre        \p distance >= 0.0
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, UIntMat& connect, 
       double distance, unsigned merge_type);

/**
   Merges nodes in two meshes that are closer to a given distance.

   Nodes to be merged can be in the same mesh or in different meshes.

   Merged nodes are replaced with the first node encountered (starting from the beginning of the first
   connectivity matrix to the end of the second matrix).

   \param[in]     pos            The coordinates matrix. \n
                                 Upon exit, the merged points have same index but coordinates in the matrix
                                 are not changed.
   \param[in,out] connect1       First connectivity matrix.
   \param[in,out] connect2       Second connectivity matrix.
   \param[out]    new_node_IDs   The new indices of the nodes. \n
                                 i-th node has now index given by `new_node_IDs[i]`. 
                                 If not merged with another one: `new_node_IDs[i] == i`.
   \param[in]     distance       The tolerance for close detection. Must be >= 0. 
   \param[in]     merge_type     Control the way nodes can be merged:
                                    -# 0 : The algorithm will merge nodes that belong to any element (inter merge).\n
                                           This is the type of merging needed to merge nodes between different meshes. 
                                    -# 1 : The algorithm will only merge nodes that belong to the same element (intra merge).
                                    -# 2 : The algorithm for merge_type = 1 is repeated until no pair of nodes can 
                                           be merged anymore.        

   \return        Returns the number of merged nodes. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.
                               
   \note          The connectivity matrices don't have to be similar (they can have different types of elements).
   \note          Merges propagate (\p merge_type = 0 and 2): 
                  if node `A` is merged into `B` (`AB` <= \p distance) and `B` into `C` (`BC` <= \p distance),
                  `A` is finally merged into `C` (even if `AC` > \p distance).
   \note          Points in matrix \p pos that are not referenced in matrix \p connect are never merged, even if 
                  they are close enough.
   \note          If several nodes within a connectivity matrix are merged together, 
                  the new coordinates and index of these nodes are those of the 
                  lowest rank node in this connectivity matrix. 
   \note          If several nodes belonging to different meshes are merged together, 
                  the new coordinates and index of these nodes are those of the
                  lowest rank node in \p connect1.
   \note          The algorithm complexity is N*log(N), with N = number of nodes in the matrices \p connect1 and  \p connect2.
   \note          Elements with multiple identical nodes are not eliminated
                  from the connectivity matrix. \n
                  To remove them, use meshtools::remove_degenerated. \n
                  To remove no longer used nodes, use meshtools::simplify.

   \pre           \p distance >= 0.0
   \post          \p new_node_IDs.size() == \p pos.cols()
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, UIntMat& connect1, UIntMat& connect2, UIntVec& new_node_IDs, 
       double distance, unsigned merge_type = 0);

/**
   Same as above without the `new_node_IDs` parameter.

   \param[in]     pos            The coordinates matrix. \n
                                 Upon exit, the merged points have same index but coordinates in the matrix
                                 are not changed.
   \param[in,out] connect1       First connectivity matrix.
   \param[in,out] connect2       Second connectivity matrix.
   \param[in]     distance       The tolerance for close detection. Must be >= 0. 
   \param[in]     merge_type     Control the way nodes can be merged:
                                    -# 0 : The algorithm will merge nodes that belong to any element (inter merge).\n
                                           This is the type of merging needed to merge nodes between different meshes. 
                                    -# 1 : The algorithm will only merge nodes that belong to the same element (intra merge).
                                    -# 2 : The algorithm for merge_type = 1 is repeated until no pair of nodes can 
                                           be merged anymore.        

   \return        Returns the number of merged nodes. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.
                               
   \note          Elements with multiple identical nodes are not eliminated
                  from the connectivity matrix. \n
                  To remove them, use meshtools::remove_degenerated. \n
                  To remove no longer used nodes, use meshtools::simplify.

   \pre           \p distance >= 0.0
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, UIntMat& connect1, UIntMat& connect2, 
       double distance, unsigned merge_type = 0);

/**
   Merges nodes in several meshes that are closer to a given distance.

   Nodes to be merged can be in the same mesh or in different meshes.

   Merged nodes are replaced with the first node encountered (starting from the beginning of the first
   connectivity matrix to the end of the last matrix).

   \param[in]     pos            The coordinates matrix. \n
                                 Upon exit, the merged points have same index but coordinates in the matrix
                                 are not changed.
   \param[in]     connect_beg    Pointer to the first connectivity matrix. \n
                                 Any degenerated element (having several identical nodes) 
                                 due to the merging process (or already existing) are removed from the connectivity matrices.
   \param[in]     connect_end    Pointer to the "past-the-end" connectivity matrix = the next after the last.
   \param[out]    new_node_IDs   The new indices of the nodes. \n
                                 i-th node has now index given by `new_node_IDs[i]`. \n
                                 If not merged with another one: `new_node_IDs[i] == i`.
   \param[in]     distance       The tolerance for close detection. Must be >= 0. 
   \param[in]     merge_type     Control the way nodes can be merged:
                                    -# 0 : The algorithm will merge nodes that belong to any element (inter merge).\n
                                           This is the type of merging needed to merge nodes between different meshes. 
                                    -# 1 : The algorithm will only merge nodes that belong to the same element (intra merge).
                                    -# 2 : The algorithm for merge_type = 1 is repeated until no pair of nodes can 
                                           be merged anymore.        

   \return        Returns the number of merged nodes. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p distance >= 0.0
   \post          \p new_node_IDs.size() == \p pos.cols()

   \note          If several nodes within a connectivity matrix are merged together, 
                  the new coordinates and index of these nodes are those of the
                  lowest rank node in this connectivity matrix. 
   \note          If several nodes between several connectivity matrix are merged together, 
                  the new coordinates and index of these nodes are those of the
                  lowest rank node in the array of connectivity matrices (first matrices have higher priority).
   \note          The algorithm complexity is N*log(N), with N = number of nodes in the matrices \p connect_beg .. \p connect_end.
   \note          Elements with multiple identical nodes are not eliminated from the connectivity matrix. \n
                  To remove them, use meshtools::remove_degenerated. \n
                  To remove no longer used nodes, use meshtools::simplify.

   \warning       \p connect_beg and \p connect_end should be pointers to an array of matrices such as a
                  `UIntMat[nbr_matrices]`, `dense1D<UIntMat>` or `std::array<UIntMat>`.
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, UIntMat* connect_beg, UIntMat* connect_end, UIntVec& new_node_IDs, 
       double distance, unsigned merge_type = 0);

/**
   Same as above without the `new_node_IDs` parameter.

   \param[in]     pos            The coordinates matrix. \n
                                 Upon exit, the merged points have same index but coordinates in the matrix
                                 are not changed.
   \param[in]     connect_beg    Pointer to the first connectivity matrix. \n
                                 Any degenerated element (having several identical nodes) 
                                 due to the merging process (or already existing) are removed from the connectivity matrices.
   \param[in]     connect_end    Pointer to the "past-the-end" connectivity matrix = the next after the last.
   \param[in]     distance       The tolerance for close detection. Must be >= 0. 
   \param[in]     merge_type     Control the way nodes can be merged:
                                    -# 0 : The algorithm will merge nodes that belong to any element (inter merge).\n
                                           This is the type of merging needed to merge nodes between different meshes.
                                    -# 1 : The algorithm will only merge nodes that belong to the same element (intra merge).
                                    -# 2 : The algorithm for merge_type = 1 is repeated until no pair of nodes can 
                                           be merged anymore.        

   \return        Returns the number of merged nodes. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p distance >= 0.0

   \note          Elements with multiple identical nodes are not eliminated from the connectivity matrix. \n
                  To remove them, use meshtools::remove_degenerated. \n
                  To remove no longer used nodes, use meshtools::simplify.

   \warning       \p connect_beg and \p connect_end should be pointers to an array of matrices such as a
                  `UIntMat[nbr_matrices]`, `dense1D<UIntMat>` or `std::array<UIntMat>`.
   */
CM2MESHTOOLS_API int 
merge (const DoubleMat& pos, UIntMat* connect_beg, UIntMat* connect_end, 
       double distance, unsigned merge_type = 0);
//@}


/****************************************************************************************/
/** @name Nodes/mesh filtering */
//@{

/**
   Gets the unique values in a vector of indices.

   \param[in,out] indices_tar       Unique values of \p indices_src are appended 
                                    (except `CM2_NONE` and values in \p remove_indices).
   \param[in]     indices_src       Vector of indices with possibly some duplicates.
   \param[in]     remove_indices    Vector of indices to exclude from copying into \p indices_tar.

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The special index `CM2_NONE` (i.e. `unsigned(-1)`) is discarded.
   \warning       Indices are appended to \p indices_tar. 
                  Clear \p indices_tar before if you want only the nodes of \p indices_src.
   */
CM2MESHTOOLS_API int 
unique_indices (UIntVec& indices_tar, const UIntVec& indices_src, 
                const UIntVec remove_indices = UIntVec());

/**
   Gets the unique values in a matrix of indices.

   \param[in,out] indices_tar       Unique values of \p connect_src are appended (except `CM2_NONE`).
   \param[in]     connect_src       Matrix of connectivity.
   \param[in]     remove_indices    Vector of indices to exclude from copying into \p indices_tar.

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The special index `CM2_NONE` (i.e. `unsigned(-1)`) is discarded.
   \warning       Indices are appended to \p indices_tar. 
                  Clear \p indices_tar before if you want only the nodes of \p connect_src.
   */
CM2MESHTOOLS_API int 
unique_indices (UIntVec& indices_tar, const UIntMat& connect_src, 
                const UIntVec remove_indices = UIntVec());

/**
   Gets the unique values in several matrices of indices.

   \param[in,out] indices_tar       Unique values in all the connectivity matrices are appended (except `CM2_NONE`).
   \param[in]     connect_src_beg   Pointer to the first source matrix.
   \param[in]     connect_src_end   Pointer to the past-the-end source matrix = the next after the last.
   \param[in]     remove_indices    Vector of indices to exclude from copying into \p indices_tar.

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The sources matrices may have different number of rows.
   \note          The indices of the nodes are appended to the vector \p indices_tar starting with
                  the first matrix, then the new nodes found in the second matrix and so on.
   \warning       Indices are appended to \p indices_tar. 
                  Clear \p indices_tar before if you want only the nodes of the connect matrices.
   \note          The special index `CM2_NONE` (i.e. `unsigned(-1)`) is discarded.
   */
CM2MESHTOOLS_API int 
unique_indices (UIntVec& indices_tar, const UIntMat* connect_src_beg, const UIntMat* connect_src_end,
                const UIntVec remove_indices = UIntVec());

/**
   Counts the nodes referenced in a connectivity matrix (except `CM2_NONE`), with no duplicates.

   \param[in]     connect           The connectivity matrix.

   \return        The number of unique nodes referenced in the connectivity matrix.\n
                  Returns a negative value in case of error.\n
                  Returned value = -k => the k-th argument had an illegal value.
   */
CM2MESHTOOLS_API int 
count_unique_indices (const UIntMat& connect);

/**
   Counts the nodes referenced in several matrices (except `CM2_NONE`), with no duplicates.

   Connectivity matrices don't have to be similar (you can mix `CM2_FACET3`, `CM2_TETRA4`, `CM2_HEXA8`... matrices).

   \param[in]     connect_beg       Pointer to the first connectivity matrix.
   \param[in]     connect_end       Pointer to the past-the-end connectivity matrix = the next after the last.

   \return        The number of unique nodes referenced in the connectivity matrices. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Sources matrices may have different number of rows.
   \warning       connect_beg and connect_end should be pointers to an array of matrices such as a
                  static array (UIntMat[nbr_matrices]).
   */
CM2MESHTOOLS_API int 
count_unique_indices (const UIntMat* connect_beg, const UIntMat* connect_end);

/**
   Eliminates the unreferenced nodes in several meshes.

   This function eliminates from matrix \p pos all unreferenced nodes
   and updates the connectivity matrices accordingly.

   \param[in,out] pos            The coordinates matrix. \n
                                 Upon exit, only the points greater or equal than \p N0 referenced 
                                 in the connectivity matrices remain.
   \param[in]     connect_beg    Pointer to the first connectivity matrix. \n
                                 Indices in the connectivity matrices are changed according to the nodes changes
                                 in the \p pos matrix.
   \param[in]     connect_end    Pointer to the past-the-end connectivity matrix = the next after the last.
   \param[out]    new_node_IDs   The new indices of the nodes. \n
                                 i-th node has now index given by `new_node_IDs[i]`. \n
                                 If removed, `new_node_IDs[i] == CM2_NONE`. \n
                                 If not moved, `new_node_IDs[i] == i`.
   \param[in]     N0             Optional. The first node that can be simplified. \n
                                 Nodes below \p N0 are not eliminated, even if not referenced by any of the connectivity matrices.

   \return        Returns the number of removed coordinates in matrix \p pos (>= 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols(). 
   \post          \p new_node_IDs.size() == \p pos.cols()
   \warning       connect_beg and connect_end should be pointers to a continuous array of matrices (such as a C-style array).
                  Example:
                  \code
                     UIntMat  mats[3] = {connect0, connect1, connect3};
                     meshtools::simplify (pos, mats, mats+3, new_node_IDs);
                  \endcode
   \note          The columns of the non-eliminated points in \p pos are left in the same order as upon entry.
   \note          Connectivity matrices don't have to be similar (you can mix `CM2_FACET3`, `CM2_TETRA4`, `CM2_HEXA8`... matrices).
   */
CM2MESHTOOLS_API int 
simplify (DoubleMat& pos, UIntMat* connect_beg, UIntMat* connect_end, UIntVec& new_node_IDs, unsigned N0 = 0);

/**
   Eliminates the unreferenced nodes in a mesh.

   \param[in,out] pos            The coordinates matrix. \n
                                 Upon exit, only the points greater or equal than \p N0 referenced 
                                 in the connectivity matrix remain.
   \param[in,out] connect        The connectivity matrix.
   \param[out]    new_node_IDs   The new indices of the nodes. \n
                                 i-th node has now index given by `new_node_IDs[i]`. \n
                                 If removed, `new_node_IDs[i] == CM2_NONE`. \n
                                 If not moved, `new_node_IDs[i] == i`.
   \param[in]     N0             Optional. The first node that can be simplified. \n
                                 Nodes below \p N0 are not eliminated, even if not referenced by the connectivity matrix.

   \return        Returns the number of removed coordinates in matrix \p pos (>= 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols(). 
   \post          \p new_node_IDs.size() == \p pos.cols()
   \note          The columns of the non-eliminated points in \p pos are left in the same order as upon entry.
   \note          This function is provided for convenience only. It is equivalent to:
                  \code
                     UIntMat  mats[1] = {connect};
                     meshtools::simplify (pos, mats, mats+1, new_node_IDs, N0);
                  \endcode
   */
CM2MESHTOOLS_API int 
simplify (DoubleMat& pos, UIntMat& connect, UIntVec& new_node_IDs, unsigned N0 = 0);

/**
   Eliminates the unreferenced nodes in a mesh.

   \param[in,out] pos            The coordinates matrix. \n
                                 Upon exit, only the points referenced in the connectivity matrix remain.
   \param[in,out] connect        The connectivity matrix.
   \param[in]     N0             Optional. The first node that can be simplified. \n
                                 Nodes below \p N0 are not eliminated, even if not referenced by the connectivity matrix.

   \return        Returns the number of removed coordinates in matrix \p pos (>= 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols(). 
   \post          \p new_node_IDs.size() == \p pos.cols()
   \note          The columns of the non-eliminated points in \p pos are left in the same order as upon entry.
   \note          This function is provided for convenience only. It is equivalent to:
                  \code
                     UIntVec  new_node_IDs;
                     UIntMat  mats[1] = {connect};
                     meshtools::simplify (pos, mats, mats+1, new_node_IDs, N0);
                  \endcode
   */
CM2MESHTOOLS_API int 
simplify (DoubleMat& pos, UIntMat& connect, unsigned N0 = 0);

/**
   Eliminates the unreferenced nodes simultaneously in two meshes.

   \param[in,out] pos            The coordinates matrix.
                                 Upon exit, only the points referenced in the connectivity matrix remain.
   \param[in,out] connect0       First connectivity matrix.
   \param[in,out] connect1       Second connectivity matrix.
   \param[in]     N0             Optional. The first node that can be simplified. \n
                                 Nodes below \p N0 are not eliminated, even if not referenced by the connectivity matrix.

   \return        Returns the number of removed coordinates in matrix \p pos upon exit (>= 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \note          This function is provided for convenience only. It is equivalent to:
                  \code
                     UIntVec  new_node_IDs;
                     UIntMat  mats[] = {connect0, connect1};
                     meshtools::simplify (pos, mats, mats+2, new_node_IDs, N0);
                  \endcode
   */
CM2MESHTOOLS_API int 
simplify (DoubleMat& pos, UIntMat& connect0, UIntMat& connect1, unsigned N0 = 0);

/**
   Eliminates the elements with multiple identical node IDs.

   \param[in,out] connect           The connectivity matrix.
   \param         aggress_flag      Optional. Flag to tell whether elements with multiple undefined nodes (i.e. `CM2_NONE`)
                                    should be removed (default `aggress_flag = true`) or kept (`aggress_flag = false`).

   \return        Returns the number of removed columns in matrix \p connect (>= 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \warning       No geometric quality test is performed here.
                  Only the elements with multiple identical nodes are considered to be degenerated and are removed.
   \warning       Quads with two identical nodes are removed.
                  To keep them as triangles, one should call meshtools2d::convert_degenerated_Q4 before.
   */
CM2MESHTOOLS_API int 
remove_degenerated (UIntMat& connect, bool aggress_flag = true);

/**
   Eliminates the elements with multiple identical node IDs.

   Same as above but with an additional output array \p new_element_IDs to give where
   the elements are moved in the \p connect matrix.

   \param[in,out] connect           The connectivity matrix.
   \param[out]    new_element_IDs   The new indices of the elements. \n
                                    i-th element has now index given by `new_element_IDs[i]`. \n
                                    If removed, `new_element_IDs[i] == CM2_NONE`. \n
                                    If not moved, `new_element_IDs[i] == i`.
   \param         aggress_flag      Optional. Flag to tell whether elements with multiple undefined nodes (i.e. `CM2_NONE`)
                                    should be removed (default `aggress_flag = true`) or kept (`aggress_flag = false`).

   \return        Returns the number of removed columns in matrix \p connect (>= 0). \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          The size of \p new_element_IDs is equal to the size that \p connect had upon entry.
   \post          `new_element_IDs[i] <= i` or `new_element_IDs[i] == CM2_NONE`. 

   \warning       No geometric quality test is performed here.
                  Only the elements with multiple identical nodes are considered to be degenerated and are removed.
   \warning       Quads with two identical nodes are removed.
                  To keep them as triangles, one should call meshtools2d::convert_degenerated_Q4 before.
   */
CM2MESHTOOLS_API int 
remove_degenerated (UIntMat& connect, UIntVec& new_element_IDs, bool aggress_flag = true);
//@}


/****************************************************************************************/
/** @name Miscellaneous transformations */
//@{

/**
   Clipping of a mesh (ecorche).

   Eliminates the elements that have at least one node located in front of a plane
   defined by a point and a direction vector.

   Every element that has at least one node \p P for which the dot product between P0P and D is negative is discarded.

   \param[in]     pos            The coordinates matrix.
   \param[in]     connect_src    The source connectivity matrix.
   \param[in]     P0             A point on the clipping plane.
   \param[in]     D              The director of the clipping plane (doesn't need to be normalized).
   \param[in,out] connect_tar    The resulting mesh (kept elements are appended at the back).

   \return        Error code. Returns 0 when successful. \n
                  Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        \p pos.rows() <= 3
   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connect_tar.rows() == \p connect_src.rows() or \p connect_tar is empty.
   */
CM2MESHTOOLS_API int 
clip (const DoubleMat& pos, const UIntMat& connect_src, 
      const DoubleVec3& P0, const DoubleVec3& D,
      UIntMat& connect_tar);
//@}


}  // end namespace meshtools
}  // end namespace cm2


#endif

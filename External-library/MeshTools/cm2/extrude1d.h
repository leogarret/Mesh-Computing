/**
   \file       extrude1d.h
   \brief      Definitions of the point-to-1D-mesh extrude routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_EXTRUDE_1D_H__
#define __CM2_EXTRUDE_1D_H__


namespace cm2 {
namespace meshtools1d {

/****************************************************************************************/
/** @name Extrude translate into an EDGE2 mesh */
//@{

/**
   Creates a straight line mesh from a node and a translation (2-D version).
   
   Example:
   \verbatim

   INPUT :

      pos   = [0
               0
               0]
      N0    = 0
      T     = [1.5 3 0]
      N     = 3


   OUTPUT :
                                     
      pos = [0  0.5  1  1.5
             0   1   2   3          (stored column-wise)
             0   0   0   0 ]

      indices = [0 1 2 3]       

      3 elements :


                      3        
                     *            _
                    /             /|
                   /             /
                 2/             /
                 *             /
                /             /  
               /             /   T
              /             / 
            1*             /
            /             /   
           /             /
          /             /
      P0 *             /
         0    

   \endverbatim
      
   Can be used in conjunction with meshtools1d::indices_to_connectE2 to convert indices into a
   connectivity matrix.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     T              The translation vector.
   \param[in]     NE             The number of elements to divide the translation vector.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended). \n
                                 The index \p N0 is first appended, then the \p N new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector causes the elements to be degenerated.

   \pre        \p N0 < \p pos.cols()
   \pre        \p NE > 0
   \pre        \p pos.rows() == 2, or \p pos is empty.
   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   unsigned N0,   
                   const DoubleVec2& T,
                   unsigned NE,
                   UIntVec& indices);                          


/**
   Creates a straight line mesh from a node and a translation (3-D version).
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     T              The translation vector.
   \param[in]     NE             The number of elements to divide the translation vector \p T.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended).
                                 The index \p N0 is first appended, then the \p N new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector causes the elements to be degenerated.

   \pre        \p N0 < \p pos.cols()
   \pre        \p NE > 0
   \pre        \p pos.rows() == 3, or \p pos is empty.
   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   unsigned N0,   
                   const DoubleVec3& T,
                   unsigned NE,
                   UIntVec& indices);                          

/**
   Creates a straight line mesh from a point and a translation (2-D version).

   Same as above with a starting point that is not already in matrix \p pos.
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     P0             The coordinates of the starting point.
   \param[in]     T              The translation vector.
   \param[in]     NE             The number of elements to divide the translation vector \p T.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended). \n
                                 The index of the new node at \p P0 is first appended, then the \p N new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector \p T causes the elements to be degenerated.
   \pre        \p NE > 0
   \pre        \p pos.rows() == 2 or \p pos is empty.
   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   const DoubleVec2& P0,   
                   const DoubleVec2& T,
                   unsigned NE,
                   UIntVec& indices);                          


/**
   Creates a straight line mesh from a point and a translation (3-D version).
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     P0             The coordinates of the starting point.
   \param[in]     T              The translation vector.
   \param[in]     NE             The number of elements to divide the translation vector.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended). \n
                                 The index of the new node at \p P0 is first appended, then the \p N new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector \p T causes the elements to be degenerated.

   \pre        \p NE > 0
   \pre        \p pos.rows() == 3 or \p pos is empty.

   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   const DoubleVec3& P0,   
                   const DoubleVec3& T,
                   unsigned NE,
                   UIntVec& indices);                          


/**
   Creates a straight line mesh from a node and a translation with specific sizes (2-D version).

   Same as above with a starting and an ending size instead of a number of nodes.

   The nodes are placed so that the edge lengths adjust as well as possible to the target size
   values given at the extremities (i.e. \p h0 and \p h1).
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     T              The translation vector.
   \param[in]     h0             The size of the edge near the starting node (\p N0).
   \param[in]     h1             The size of the edge near the ending node.
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes.
                                 The index \p N0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector \p T causes the elements to be degenerated.
   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p N0 < \p pos.cols()
   \pre        \p h0 > 0
   \pre        \p h1 > 0
   \pre        \p pos.rows() == 2 or \p pos is empty.

   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   unsigned N0, 
                   const DoubleVec2& T,
                   double h0,
                   double h1,
                   bool force_even,                        
                   UIntVec& indices);

/**
   Creates a straight line mesh from a node and a translation with specific sizes (3-D version).
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     T              The translation vector.
   \param[in]     h0             The size of the edge near the starting node (\p N0).
   \param[in]     h1             The size of the edge near the ending node.
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes. \n
                                 The index \p N0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector \p T causes the elements to be degenerated.
   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p N0 < \p pos.cols()
   \pre        \p h0 > 0
   \pre        \p h1 > 0
   \pre        \p pos.rows() == 3 or \p pos is empty.

   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   unsigned N0, 
                   const DoubleVec3& T,
                   double h0,
                   double h1,
                   bool force_even,                        
                   UIntVec& indices);

/**
   Creates a straight line mesh from a point and a translation with specific sizes (2-D version).

   Same as above with a starting point that is not already in matrix "pos".
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     P0             The coordinates of the starting point.
   \param[in]     T              The translation vector.
   \param[in]     h0             The size of the edge near the starting node (\p P0).
   \param[in]     h1             The size of the edge near the ending node.
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes. \n
                                 The index of the new node at P0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector \p T causes the elements to be degenerated.
   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p h0 > 0
   \pre        \p h1 > 0
   \pre        \p pos.rows() == 2 or \p pos is empty.

   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   const DoubleVec2& P0,   
                   const DoubleVec2& T,
                   double h0,
                   double h1,
                   bool force_even,                        
                   UIntVec& indices);

/**
   Creates a straight line mesh from a point and a translation with specific sizes (3-D version).
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     P0             The coordinates of the starting point.
   \param[in]     T              The translation vector.
   \param[in]     h0             The size of the edge near the starting node (\p P0).
   \param[in]     h1             The size of the edge near the ending node.
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes. \n
                                 The index of the new node at \p P0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null translation vector \p T causes the elements to be degenerated.
   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p h0 > 0
   \pre        \p h1 > 0
   \pre        \p pos.rows() == 3 or \p pos is empty.

   \see        indices_to_connectE2, mesh_straight, extrude_rotate.
   */
CM2MESHTOOLS1D_API int 
extrude_translate (DoubleMat& pos,
                   const DoubleVec3& P0,   
                   const DoubleVec3& T,
                   double h0,
                   double h1,
                   bool force_even,                        
                   UIntVec& indices);
//@}


/****************************************************************************************/
/** @name Extrude rotate into an EDGE2 mesh */
//@{

/**
   Creates an arc mesh from a node and a rotation.
   Example:
   \verbatim

   INPUT :

      pos   = [1
               0
               0]
      N0    = 0
      C     = [0 0 0]
      R     = [0 0 M_PI/2]
      NE     = 2


   OUTPUT :                    
                                     
      pos = [1  0.707  0
             0  0.707  1           (stored column-wise)
             0  0      0]

      indices = [0 1 2]       

      2 elements:


                      2 
                      *----___
                              ---__          
                                    -_  1  
                                       *
                                        |
                                         |
                                          |
                                           |
                                           |
                                            |
                                            |
                      C                     |
                      x                     *            
                                            0

   \endverbatim
      
   Can be used in conjunction with meshtools1d::indices_to_connectE2 to convert indices into a
   connectivity matrix.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     NE             The number of elements to divide the rotation.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended). \n
                                 The index \p N0 is first appended, then the \p N new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector or a centre of rotation at position of point \p N0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               output \p indices vector (i.e. \p N0). 

   \pre        \p N0 < \p pos.cols()
   \pre        \p NE > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                unsigned N0, 
                const DoubleVec3& C,
                const DoubleVec3& R,
                unsigned NE,
                UIntVec& indices);                          


/**
   Creates an arc mesh from a node and a rotation in the XY plane (2-D version).
   Same as above but for rotations in the Z=0 plane.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle (radian unit).
   \param[in]     NE             The number of elements to divide the rotation.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended). \n
                                 The index \p N0 is first appended, then the \p N new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector or a centre of rotation at position of point \p N0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               output \p indices vector (i.e. \p N0). 

   \pre        \p N0 < \p pos.cols()
   \pre        \p NE > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                unsigned N0, 
                const DoubleVec2& C,
                double Rz,
                unsigned NE,
                UIntVec& indices);                          


/**
   Creates an arc mesh from a point and a rotation (3-D version).
   Same as above with a starting point that is not already in matrix "pos".
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     P0             The coordinates of the starting node.
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     NE             The number of elements to divide the rotation.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended). \n
                                 The index of the new node at \p P0 is first appended, then the \p N new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector or a centre of rotation at position \p P0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               \p indices vector.
               If the \p indices vector were empty before the call, the following code will close the circle:
               \code
                  indices.back() = indices.front();
               \endcode

   \pre        \p NE > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                const DoubleVec3& P0,
                const DoubleVec3& C,
                const DoubleVec3& R,
                unsigned NE,
                UIntVec& indices);                          

/**
   Creates an arc mesh from a point and a rotation (2-D version).
   Same as above but for rotations in the Z=0 plane.
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     P0             The coordinates of the starting node.
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle (radian unit).
   \param[in]     NE             The number of elements to divide the rotation.
   \param[in,out] indices        The indices of the nodes (\p NE + 1 indices are appended). \n
                                 The index of the new node at \p P0 is first appended, then the \p NE new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector or a centre of rotation at position \p P0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               \p indices vector.
               If the \p indices vector were empty before the call, the following code will close the circle:
               \code
                  indices.back() = indices.front();
               \endcode

   \pre        \p NE > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                const DoubleVec2& P0,
                const DoubleVec2& C,
                double Rz,
                unsigned NE,
                UIntVec& indices);                          

/**
   Creates an arc mesh from a node and a rotation with specific sizes (3-D version).

   Same as above with a starting and an ending size instead of a number of nodes.

   The nodes are placed so that the edge lengths adjust as well as possible to the target size
   values given at the extremities (i.e. \p h0 and \p h1).
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     h0             The size of the edge near the starting node (\p N0).
   \param[in]     h1             The size of the edge near the ending node.
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes. \n
                                 The index \p N0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector \p R or a centre of rotation at position of \p N0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               \p indices vector (i.e. \p N0).

   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p N0 < \p pos.cols()
   \pre        \p h0 > 0
   \pre        \p h1 > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                unsigned N0, 
                const DoubleVec3& C,
                const DoubleVec3& R,
                double h0,
                double h1,
                bool force_even,                        
                UIntVec& indices);

/**
   Creates an arc mesh from a node and a rotation with specific sizes (2-D version).

   Same as above with a starting and an ending size instead of a number of nodes.

   The nodes are placed so that the edge lengths adjust as well as possible to the target size
   values given at the extremities (i.e. \p h0 and \p h1).
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle (radian unit).
   \param[in]     h0             The size of the edge near the starting node (\p N0).
   \param[in]     h1             The size of the edge near the ending node.
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes. \n
                                 The index \p N0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector \p Rz or a centre of rotation at position of \p N0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               \p indices vector (i.e. \p N0).
   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p N0 < \p pos.cols()
   \pre        \p h0 > 0
   \pre        \p h1 > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                unsigned N0, 
                const DoubleVec2& C,
                double Rz,
                double h0,
                double h1,
                bool force_even,                        
                UIntVec& indices);

/**
   Creates an arc mesh from a point and a rotation with specific sizes (3D version).

   Same as above with a starting point that is not already in matrix "pos".
   
   \param[in,out] pos               The coordinates matrix. New points are appended.
   \param[in]     P0                The position of the starting point.
   \param[in]     C                 The centre of the rotation.
   \param[in]     R                 The rotation vector. The norm is the angular value (radians unit).
   \param[in]     h0                The size of the edge near the starting point (\p P0).
   \param[in]     h1                The size of the edge near the ending node.
   \param[in]     force_even        Flag to force the number of created edges to be even. \n
                                    Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices           The indices of the nodes. \n
                                    The index of the new node at \p P0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector \p R or a centre of rotation at position \p P0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               \p indices vector.
               If the \p indices vector were empty before the call, the following code will close the circle:
               \code
                  indices.back() = indices.front();
               \endcode
   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p N0 < \p pos.cols()
   \pre        \p h0 > 0
   \pre        \p h1 > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                const DoubleVec3& P0,
                const DoubleVec3& C,
                const DoubleVec3& R,
                double h0,
                double h1,
                bool force_even,                        
                UIntVec& indices);

/**
   Creates an arc mesh from a point and a rotation with specific sizes (2-D version).

   Same as above but for rotations in the Z=0 plane.
   
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     P0             The position of the starting point.
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle (radian unit).
   \param[in]     h0             The size of the edge near the starting point (\p P0).
   \param[in]     h1             The size of the edge near the ending node.
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes. \n
                                 The index of the new node at \p P0 is first appended, then the new nodes index.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       A null rotation vector \p Rz or a centre of rotation at position \p P0
               causes the elements to be degenerated.
   \note       To get a full closed circle, you need to replace the last index with the first one in the 
               \p indices vector.
               If the \p indices vector were empty before the call, the following code will close the circle:
               \code
                  indices.back() = indices.front();
               \endcode
   \note       This function creates at least one edge (two when \p force_even == true).

   \pre        \p N0 < \p pos.cols()
   \pre        \p h0 > 0
   \pre        \p h1 > 0

   \see        indices_to_connectE2, extrude_translate.
   */
CM2MESHTOOLS1D_API int 
extrude_rotate (DoubleMat& pos,
                const DoubleVec2& P0,
                const DoubleVec2& C,
                double Rz,
                double h0,
                double h1,
                bool force_even,                        
                UIntVec& indices);

//@}


}  // end namespace meshtools1d
}  // end namespace cm2

#endif

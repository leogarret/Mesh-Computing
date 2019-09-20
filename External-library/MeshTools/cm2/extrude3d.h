/**
   \file       extrude3d.h
   \brief      Declarations of the 2-D mesh -> 3-D mesh extrusion routines (sweeping).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_EXTRUDE3D_H__
#define __CM2_EXTRUDE3D_H__



namespace cm2 {
namespace meshtools3d {

/****************************************************************************************/
/** @name Extrude translate a 2-D mesh into a 3-D mesh */
//@{

/**
   Extrudes (sweeps) with translation a 2-D mesh into a structured 3-D mesh.

   Extruding a 2-D mesh with N0 elements into 3-D mesh will produce a mesh with N0*N1 elements. 
   Each layer will have a side length equal to ||T|| / N1.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connect2D      The connectivity matrix of the 2-D mesh (N0 elements). \n
                                 This mesh can be a all-triangle mesh (3xN0), a pure quadrangle mesh (4xN0)
                                 or a mixed mesh (4xN0 with some 4-th nodes equal to CM2_NONE).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of layers.
   \param[in,out] connect3D      The connectivity of the output mesh (WEDGE6 and/or HEXA8). \n
                                 N0*N1 new columns are appended. \n
                                 If \p connect2D is a all-triangle mesh, \p connect3D will have
                                 6 rows upon exit (pure wedge mesh). \n
                                 If \p connect2D is a all-quad mesh or a mixed mesh, \p connect3D will have
                                 8 rows upon exit (pure hex or mixed wedge/hex mesh).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 3 (i.e. dim = 3).
   \pre           \p connect2D.rows() == 3 or 4 or \p connect2D is empty.
   \pre           \p connect3D.rows() == 6 or 8 or \p connect3D is empty.

   \warning       This function does not check the validity of the resulting 3-D elements.
   \warning       To get 3-D elements in \p connect3D oriented as expected by the CM2 FEM package, 
                  the angle between the normals to the \p connect2D mesh and vector \p T must be in ]-pi/2 ; +pi/2[. \n
                  Otherwise, the jacobians of the solid elements will be negative.
   \note          A null translation vector causes the elements to be degenerated (flat elements).

   \see           extrude_rotate, extrude_translate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS3D_API int 
extrude_translate (DoubleMat& pos, 
                   const UIntMat& connect2D, 
                   const DoubleVec3& T,
                   unsigned N1,
                   UIntMat& connect3D);                          


/**
   Extrudes (sweeps) with translation a 2-D mesh into a structured 3-D mesh, with specific sizes.

   The nodes are placed so that the edge lengths adjust as well as possible to the target size
   values given at the extremities (i.e. \p h0 and \p h1).
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connect2D      The connectivity matrix of the 2-D mesh (N0 elements). \n
                                 This mesh can be a all-triangle mesh (3xN0), a pure quadrangle mesh (4xN0)
                                 or a mixed mesh (4xN0 with some 4-th nodes equal to CM2_NONE).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The "height" of the layer near the base 2D mesh (\p connect2D).
   \param[in]     h1             The "height" of the layer near the top 2D mesh (\p connect2D).
   \param[in]     force_even     Flag to force the number of edges along the translation vector to be even.
   \param[in,out] connect3D      The connectivity of the output mesh (WEDGE6 and/or HEXA8).  \n
                                 If \p connect2D is a all-triangle mesh, \p connect3D will have
                                 6 rows upon exit (pure wedge mesh). \n
                                 If \p connect2D is a all-quad mesh or a mixed mesh, \p connect3D will have
                                 8 rows upon exit (pure hex or mixed wedge/hex mesh).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 3 (i.e. dim = 3).
   \pre           \p connect2D.rows() == 3 or 4 or \p connect2D is empty.
   \pre           \p connect3D.rows() == 6 or 8 or \p connect3D is empty.

   \warning       This function does not check the validity of the resulting 3-D elements.
   \warning       To get 3-D elements in \p connect3D oriented as expected by the CM2 FEM package, 
                  the angle between the normals to the \p connect2D mesh and vector \p T must be in ]-pi/2 ; +pi/2[. \n
                  Otherwise, the jacobians of the solid elements will be negative.
   \note          A null translation vector causes the elements to be degenerated (flat elements).

   \see           extrude_rotate, extrude_translate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS3D_API int 
extrude_translate (DoubleMat& pos, 
                   const UIntMat& connect2D, 
                   const DoubleVec3& T,
                   double h0, double h1, bool force_even,
                   UIntMat& connect3D);                          

//@}


/****************************************************************************************/
/** @name Extrude rotate a 2-D mesh into a 3-D mesh */
//@{

/**
   Extrudes (sweeps) with rotation a 2-D mesh into a structured 3-D mesh.

   Extruding a 2-D mesh with N0 elements into 3-D mesh will produce a mesh with N0*N1 elements. 
   Each layer will have a side length equal to ||T|| / N1.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connect2D      The connectivity matrix of the 2-D mesh (N0 elements). \n
                                 This mesh can be a all-triangle mesh (3xN0), a pure quadrangle mesh (4xN0)
                                 or a mixed mesh (4xN0 with some 4-th nodes equal to CM2_NONE).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The total rotation vector. The norm is the angular value (radians unit).
   \param[in]     N1             The number of layers.
   \param[in,out] connect3D      The connectivity of the output mesh (WEDGE6 and/or HEXA8). \n
                                 N0*N1 new columns are appended. \n
                                 If \p connect2D is a all-triangle mesh, \p connect3D will have
                                 6 rows upon exit (pure wedge mesh). \n
                                 If \p connect2D is a all-quad mesh or a mixed mesh, \p connect3D will have
                                 8 rows upon exit (pure hex or mixed wedge/hex mesh).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 3 (i.e. dim = 3).
   \pre           \p connect2D.rows() == 3 or 4 or \p connect2D is empty.
   \pre           \p connect3D.rows() == 6 or 8 or \p connect3D is empty.

   \warning       This function does not check the validity of the resulting 3-D elements.
   \warning       To get 3-D elements in \p connect3D oriented as expected by the CM2 FEM package, 
                  the angle between 2 consecutive layers must be less than pi
                  (otherwise the jacobians of the solid elements will be negative).
   \note          A null rotation vector or a centre of rotation inside the 2-D mesh
                  causes the 3-D elements to be degenerated. In the former case the generated elements are flat.
   \note          To get a full closed tore, you need to merge the base 2-D mesh with the top 2-D mesh
                  (use the meshtools::merge function on the 3-D mesh for instance).

   \see           extrude_translate, extrude_rotate_T3, extrude_rotate_Q4.
   */
CM2MESHTOOLS3D_API int 
extrude_rotate (DoubleMat& pos, 
                const UIntMat& connect2D, 
                const DoubleVec3& C,
                const DoubleVec3& R,
                unsigned N1,
                UIntMat& connect3D);                          


/**
   Extrudes (sweeps) with rotation a 2-D mesh into a structured 3-D mesh, with specific sizes.

   The nodes are placed so that the edge lengths adjust as well as possible to the target size
   values given at the extremities (i.e. \p h0 and \p h1).
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connect2D      The connectivity matrix of the 2-D mesh (N0 elements). \n
                                 This mesh can be a all-triangle mesh (3xN0), a pure quadrangle mesh (4xN0)
                                 or a mixed mesh (4xN0 with some 4-th nodes equal to CM2_NONE).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The total rotation vector. The norm is the angular value (radians unit).
   \param[in]     h0             The "height" of the layer near the base 2D mesh (node \p connect2D(0,0)).
   \param[in]     h1             The "height" of the layer near the top 2D mesh (node rotated from \p connectE2(0,0)).
   \param[in]     force_even     Flag to force the number of edges along the translation vector to be even.
   \param[in,out] connect3D      The connectivity of the output mesh (WEDGE6 and/or HEXA8). \n
                                 If \p connect2D is a all-triangle mesh, \p connect3D will have
                                 6 rows upon exit (pure wedge mesh). \n
                                 If \p connect2D is a all-quad mesh or a mixed mesh, \p connect3D will have
                                 8 rows upon exit (pure hex or mixed wedge/hex mesh).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 3 (i.e. dim = 3).
   \pre           \p connect2D.rows() == 3 or 4 or \p connect2D is empty.
   \pre           \p connect3D.rows() == 6 or 8 or \p connect3D is empty.

   \warning       This function does not check the validity of the resulting 3-D elements.
   \warning       To get 3-D elements in \p connect3D oriented as expected by the CM2 FEM package, 
                  the angle between 2 consecutive layers must be less than pi
                  (otherwise, the jacobians of the solid elements will be negative).
   \note          A null rotation vector or a centre of rotation inside the 2-D mesh
                  causes the 3-D elements to be degenerated. In the former case the generated elements are flat.
   \note          To get a full closed tore, you need to merge the base 2-D mesh with the top 2-D mesh
                  (use the meshtools::merge function on the 3-D mesh for instance).

   \see           extrude_translate, extrude_rotate_T3, extrude_rotate_Q4.
   */
CM2MESHTOOLS3D_API int 
extrude_rotate (DoubleMat& pos, 
                const UIntMat& connect2D, 
                const DoubleVec3& C,
                const DoubleVec3& R,
                double h0, double h1, bool force_even,
                UIntMat& connect3D);                          

//@}


/****************************************************************************************/
/** @name Extrudes with both rotation and translation a 2-D mesh into a 3-D mesh */
//@{

/**
   Extrudes (sweeps) with rotation and translation a 2-D mesh into a structured 3-D mesh.

   Extruding a 2-D mesh with N0 elements into 3-D mesh will produce a mesh with N0*N1 elements. 

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connect2D      The connectivity matrix of the 2-D mesh (N0 elements). \n
                                 This mesh can be a all-triangle mesh (3xN0), a pure quadrangle mesh (4xN0)
                                 or a mixed mesh (4xN0 with some 4-th nodes equal to CM2_NONE).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The total rotation vector. The norm is the angular value (radians unit).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of layers.
   \param[in,out] connect3D      The connectivity of the output mesh (WEDGE6 and/or HEXA8). \n
                                 N0*N1 new columns are appended. \n
                                 If \p connect2D is a all-triangle mesh, \p connect3D will have
                                 6 rows upon exit (pure wedge mesh). \n
                                 If \p connect2D is a all-quad mesh or a mixed mesh, \p connect3D will have
                                 8 rows upon exit (pure hex or mixed wedge/hex mesh).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 3 (i.e. dim = 3).
   \pre           \p connect2D.rows() == 3 or 4 or \p connect2D is empty.
   \pre           \p connect3D.rows() == 6 or 8 or \p connect3D is empty.

   \warning       This function does not check the validity of the resulting 3-D elements.
   \see           extrude_translate, extrude_rotate, extrude_rotate_T3, extrude_rotate_Q4.
   */
CM2MESHTOOLS3D_API int 
extrude_spiral (DoubleMat& pos, 
                const UIntMat& connect2D, 
                const DoubleVec3& C,
                const DoubleVec3& R,
                const DoubleVec3& T,
                unsigned N1,
                UIntMat& connect3D);                          


/**
   Extrudes (sweeps) with rotation a 2-D mesh into a structured 3-D mesh, with specific sizes.

   The nodes are placed so that the edge lengths adjust as well as possible to the target size
   values given at the extremities (i.e. \p h0 and \p h1).
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connect2D      The connectivity matrix of the 2-D mesh (N0 elements). \n
                                 This mesh can be a all-triangle mesh (3xN0), a pure quadrangle mesh (4xN0)
                                 or a mixed mesh (4xN0 with some 4-th nodes equal to CM2_NONE).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The "height" of the layer near the base 2D mesh (along the \p T vector only).
   \param[in]     h1             The "height" of the layer near the top 2D mesh (along the \p T vector only).
   \param[in]     force_even     Flag to force the number of edges along the translation vector to be even.
   \param[in,out] connect3D      The connectivity of the output mesh (WEDGE6 and/or HEXA8). \n
                                 If \p connect2D is a all-triangle mesh, \p connect3D will have
                                 6 rows upon exit (pure wedge mesh). \n
                                 If \p connect2D is a all-quad mesh or a mixed mesh, \p connect3D will have
                                 8 rows upon exit (pure hex or mixed wedge/hex mesh).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 3 (i.e. dim = 3).
   \pre           \p connect2D.rows() == 3 or 4 or \p connect2D is empty.
   \pre           \p connect3D.rows() == 6 or 8 or \p connect3D is empty.

   \warning       This function does not check the validity of the resulting 3-D elements.
   \see           extrude_translate, extrude_rotate, extrude_rotate_T3, extrude_rotate_Q4.
   */
CM2MESHTOOLS3D_API int 
extrude_spiral (DoubleMat& pos, 
                const UIntMat& connect2D, 
                const DoubleVec3& C,
                const DoubleVec3& R,
                const DoubleVec3& T,
                double h0, double h1, bool force_even,
                UIntMat& connect3D);                          
//@}

}  // end namespace meshtools3d
}  // end namespace cm2

#endif

/**
   \file       extrude2d.h
   \brief      Declarations of the 1-D mesh -> 2-D mesh extrusion routines (sweeping).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_EXTRUDE2D_H__
#define __CM2_EXTRUDE2D_H__


namespace cm2 {
namespace meshtools2d {

/****************************************************************************************/
/** @name Extrude translate into FACEQ4 or FACET3 mesh */
//@{

/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh (2-D version).

   Let a E2 mesh with N0 edges. \n
   Extruding with N1 layers will produce a cm2::CM2_FACEQ4 mesh with N0 * N1 elements. \n
   Each layer will have a side length equals to ||T|| / N1.

   The orientation of the elements is given by the cross product of 
   the orientation of the matching E2 element and the translation vector.
   
   Example:
   \verbatim

   INPUT:

      pos = [0  1  2 
             0  0  0       (stored column-wise)
             0  0  0]

      connectE2 = [0 1     (stored column-wise)
                   1 2]

      T = [1.5 3 0]

      N1    = 3


   OUTPUT:
                                     
      pos = [0  1  2  0.5  1.5  2.5  1  2  3  1.5  2.5  3.5
             0  0  0   1    1    1   2  2  2   3    3    3     (stored column-wise)
             0  0  0   0    0    0   0  0  0   0    0    0 ]

      connectQ4 = [0 3 6  1 4 7               (stored column-wise)
                   1 4 7  2 5 8
                   4 7 10 5 8 11              
                   3 6 9  4 7 10]             

      4 elements :


                      9       10      11
                     *-------*-------*           _
                    /       /       /            /|
                   /   2   /   5   /            /
                 6/      7/       /            /
                 *-------*-------*8           /
                /       /       /            /  
               /   1   /   4   /            /   T
              /      4/       /            / 
            3*-------*-------*5           /
            /       /       /            /   
           /   0   /   3   /            /
          /       /       /            /
         *-------*-------*            /
         0      1      2

   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of elements to divide the translation vector (number of layers).
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \pre        \p pos.rows() == 2
   \see        mesh_struct_Q4, extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_Q4 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec2& T,
                      unsigned N1,
                      UIntMat& connectQ4);                          

/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh (3-D version).

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of elements to divide the translation vector (number of layers).
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \see        mesh_struct_Q4, extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_Q4 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec3& T,
                      unsigned N1,
                      UIntMat& connectQ4);                          

/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh with specific sizes at
   the beginning and at the end of the translation (2-D version).

   The nodes are placed so that the sweep edge lengths near the centroid of the edge mesh
   adjust as well as possible to the target size values \p h0 and \p h1.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The size of the edges near the base \p connectE2 mesh (along the translation vector \p T).
   \param[in]     h1             The size of the edges near the top \p connectE2 mesh (along the translation vector \p T).
   \param[in]     force_even     Flag to force the number of edges along the translation vector to be even.
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \pre        \p pos.rows() == 2
   \see        mesh_struct_Q4, extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_Q4 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec2& T,
                      double h0,
                      double h1,
                      bool force_even,
                      UIntMat& connectQ4);                          


/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh 
   with specific sizes at the beginning and at the end of the translation (3-D version).
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The size of the edges near the base \p connectE2 mesh (along the translation vector \p T).
   \param[in]     h1             The size of the edges near the top \p connectE2 mesh (along the translation vector \p T).
   \param[in]     force_even     Flag to force the number of edges along the translation vector to be even.
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \see        mesh_struct_Q4, extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_Q4 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec3& T,
                      double h0,
                      double h1,
                      bool force_even,
                      UIntMat& connectQ4);                          


/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh (2-D version).

   Let a mesh with N0 edges. \n
   Extruding with N1 layers will produce a FACET3 mesh with 2 * N0 * N1 elements. \n
   Each layer will have a side length equals to ||T|| / N1.

   The orientation of the elements is given by the cross product of 
   the orientation of the matching E2 element and the translation vector.
   
 
   Example:
   \verbatim

   INPUT:

      pos = [0  1  2 
             0  0  0       (stored column-wise)
             0  0  0]

      connectE2 = [0 1     (stored column-wise)
                   1 2]

      N1    = 2

      translation = [1 2 0]


   OUTPUT (Right pattern):
                                     
      pos = [0  1  2  0.5  1.5  2.5  1  2  3
             0  0  0   1    1    1   2  2  2    (stored column-wise)
             0  0  0   0    0    0   0  0  0]


      connectivity = [0 0 3 3 1 1 4 4           (stored column-wise)
                      1 4 4 7 2 5 5 8
                      4 3 7 6 5 4 8 7]          

      4 elements :

                 6       7                     
                 *-------*-------*8          _
                / 3   / / 7   / /            /|  
               /   /   /   /   /            /   T
              / /   2 / /   6 /            / 
            3*-------*4------*5           /
            / 1   / / 5   / /            /   
           /   /   /   /   /            /
          / /   0 / /   4 /            /
         *-------*-------*            /
         0      1      2


   OUTPUT ("Union-Jack" pattern):                        
                                     
      pos = [0  1  2  0.5  1.5  2.5  1  2  3
             0  0  0   1    1    1   2  2  2    (stored column-wise)
             0  0  0   0    0    0   0  0  0]


      connectivity = [0 0 3 4 1 2 4 4           (stored column-wise)
                      1 4 4 7 2 5 5 8
                      4 3 6 6 4 4 8 7]          (element are always oriented as the external contour) 

      4 elements :

                  6      7                     
                 *-------*-------*8          _
                / \  3  / 7   / /            /| 
               /   \   /   /   /            /   T
              /  2  \0/ /   6 /            / 
            3*-------*4------*5           /
            / 1   / / \  5  /            /   
           /   /   /   \   /            /
          / /   0 /  4  \ /            /
         *-------*-------*            /
         0      1      2

   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of elements to divide the translation vector (number of layers).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \pre        \p pos.rows() == 2
   \see        mesh_struct_T3, extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_T3 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec2& T,
                      unsigned N1,
                      int pattern,
                      UIntMat& connectT3);                          


/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh (3-D version).

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of elements to divide the translation vector (number of layers).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \see        mesh_struct_T3, extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_T3 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec3& T,
                      unsigned N1,
                      int pattern,
                      UIntMat& connectT3);
                                

/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 
   mesh with specific sizes at the beginning and at the end of the translation (2-D version).

   The nodes are placed so that the sweep edge lengths near the centroid of the edge mesh
   adjust as well as possible to the target size values \p h0 and \p h1.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The size of the edges near the base \p connectE2 mesh (along the translation vector \p T).
   \param[in]     h1             The size of the edges near the top \p connectE2 mesh (along the translation vector \p T).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in]     force_even     Flag to force the number of edges along the translation vector to be even.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \pre        \p pos.rows() == 2
   \see        mesh_struct_T3, extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_T3 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec2& T,
                      double h0,
                      double h1,
                      int pattern,
                      bool force_even,                         
                      UIntMat& connectT3);

/**
   Extrudes (sweeps) with translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 
   mesh with specific sizes at the beginning and at the end of the translation (3-D version).
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The size of the edges near the base \p connectE2 mesh (along the translation vector \p T).
   \param[in]     h1             The size of the edges near the top \p connectE2 mesh (along the translation vector \p T).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in]     force_even     Flag to force the number of edges along the translation vector to be even.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null translation vector causes the elements to be degenerated.
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \see        mesh_struct_T3, extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_translate_T3 (DoubleMat& pos, 
                      const UIntMat& connectE2, 
                      const DoubleVec3& T,
                      double h0,
                      double h1,
                      int pattern,
                      bool force_even,                         
                      UIntMat& connectT3);

//@}


/****************************************************************************************/
/** @name Extrude rotate into FACEQ4 or FACET3 mesh */
//@{

/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh.

   Let a mesh with N0 edges. \n
   Extruding with N1 layers will produce a Q4 mesh with N0 * N1 elements. 

   The orientation of the elements is given by the direction of the rotation vector.
 
   Example:
   \verbatim

   INPUT:

      pos = [0  1  2 
             0  0  0       (stored column-wise)
             0  0  0]

      connectE2 = [0 1     (stored column-wise)
                   1 2]
      N1    = 2

      rot_centre = [-1 0 0]

      rot_vector = [0 0 pi/2]


   OUTPUT:
                                     
      4 elements:
                       
                      7*----___
                       |        ---__          
                       |              -_ 4  
                       |      3         *
                      6*---__         /  |
                       |     -- _ 3 /     |
                       |   1      *        |
                       |        /  |       |
                      5*--__  /     |   2   |
                           2*        |      |
                             |   0   |       |
                             |       |       |
                             *-------*-------*            
                             0       1       2

   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     N1             The number of elements to divide the rotation vector (number of layers).
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \see        extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_Q4 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   unsigned N1,
                   UIntMat& connectQ4);                          


/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh.

   For rotations in the plane Z = 0.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle around Oz (radians unit).
   \param[in]     N1             The number of elements to divide the rotation vector (number of layers).
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \see        extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_Q4 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec2& C,
                   double Rz,
                   unsigned N1,
                   UIntMat& connectQ4);                          


/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh 
   with specific sizes at the beginning and at the end of the rotation.

   The nodes are placed so that the sweep edge lengths near the centroid of the edge mesh
   adjust as well as possible to the target size values \p h0 and \p h1.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     h0             The size of the edges near the base mesh (at node \p connectE2(0,0)).
   \param[in]     h1             The size of the edges near the top mesh (at node rotated from \p connectE2(0,0)).
   \param[in]     force_even     Flag to force the number of edges along the rotation (number of layers) to be even.
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \see        extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_Q4 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   double h0, double h1, bool force_even,  
                   UIntMat& connectQ4);
                                         
/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh 
   with specific sizes at the beginning and at the end of the rotation.

   For rotations in the plane Z = 0.
      
   The nodes are placed so that the sweep edge lengths near the centroid of the edge mesh
   adjust as well as possible to the target size values \p h0 and \p h1.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle around Oz (radians unit).
   \param[in]     h0             The size of the edges near the base mesh (at node \p connectE2(0,0)).
   \param[in]     h1             The size of the edges near the top mesh (at node rotated from \p connectE2(0,0)).
   \param[in]     force_even     Flag to force the number of edges along the rotation (number of layers) to be even.
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \see        extrude_rotate_T3, extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_Q4 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec2& C,
                   double Rz,
                   double h0, double h1, bool force_even,  
                   UIntMat& connectQ4);

/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh.

   Let a mesh with N0 edges. \n
   Extruding with N1 layers will produce a cm2::CM2_FACET3 mesh with 2 * N0 * N1 elements. 

   The orientation of the elements is given by the direction of the rotation vector.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     N1             The number of elements to divide the rotation vector (number of layers).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \see        extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_T3 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   unsigned N1, 
                   int pattern,
                   UIntMat& connectT3);                          

/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh. \n
   For rotations in the plane Z = 0.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle around Oz (radians unit).
   \param[in]     N1             The number of elements to divide the rotation vector (number of layers).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \see        extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_T3 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec2& C,
                   double Rz,
                   unsigned N1, 
                   int pattern,
                   UIntMat& connectT3);                          

/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh 
   with specific sizes at the beginning and at the end of the rotation.

   The nodes are placed so that the sweep edge lengths near the centroid of the edge mesh
   adjust as well as possible to the target size values \p h0 and \p h1.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The rotation vector. The norm is the angular value (radians unit).
   \param[in]     h0             The size of the edges near the base mesh (at node \p connectE2(0,0)).
   \param[in]     h1             The size of the edges near the top mesh (at node rotated from \p connectE2(0,0)).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in]     force_even     Flag to force the number of edges along the rotation (number of layers) to be even.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p pos.rows() == 2 or 3
   \post       \p pos.rows() == 3
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \see        extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_T3 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   double h0, double h1, 
                   int pattern, 
                   bool force_even,                         
                   UIntMat& connectT3);

/**
   Extrudes (sweeps) with rotation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh 
   with specific sizes at the beginning and at the end of the rotation.

   For rotations in the plane Z = 0.
    
   The nodes are placed so that the sweep edge lengths near the centroid of the edge mesh
   adjust as well as possible to the target size values \p h0 and \p h1.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     Rz             The rotation angle around Oz (radians unit).
   \param[in]     h0             The size of the edges near the base mesh (at node \p connectE2(0,0)).
   \param[in]     h1             The size of the edges near the top mesh (at node rotated from \p connectE2(0,0)).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in]     force_even     Flag to force the number of edges along the rotation (number of layers) to be even.
   \param[in,out] connectT3      The connectivity of the output mesh. 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \note       A null rotation vector or a centre of rotation inside the edge mesh
               causes the elements to be degenerated.
   \note       To get a full closed disk, you need to merge the initial edge mesh with the last edge line
               (use the meshtools::merge function for instance).
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \see        extrude_rotate_Q4, extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_rotate_T3 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec2& C,
                   double Rz,
                   double h0, double h1, 
                   int pattern,
                   bool force_even,                         
                   UIntMat& connectT3);

//@}


/****************************************************************************************/
/** @name Extrudes with both rotation and translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh */
//@{

/**
   Extrudes (sweeps) with rotation and translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACET3 mesh.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The total rotation angle (radians unit).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of layers.
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in,out] connectT3      The connectivity of the output mesh. \n
                                 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \pre           \p pos.rows() == 2 or 3
   \post          \p pos.rows() == 3
   \pre           \p connectE2.rows() == 2 or \p connectE2 is empty.
   \pre           \p connectT3.rows() == 3 or \p connectT3 is empty.

   \warning       This function does not check the validity of the resulting elements.
   \see           extrude_translate_T3, extrude_rotate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_spiral_T3 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   const DoubleVec3& T,
                   unsigned N1,
                   int pattern,
                   UIntMat& connectT3);                          


/**
   Extrudes (sweeps) a edge mesh with rotation and translation into a structured cm2::CM2_FACET3 mesh.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The total rotation angle (radians unit).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The size of the edges near the base mesh (along the translation vector \p T).
   \param[in]     h1             The size of the edges near the top mesh (along the translation vector \p T).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in]     force_even     Flag to force the number of edges along the translation (number of layers) to be even.
   \param[in,out] connectT3      The connectivity of the output mesh. \n
                                 2*N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \pre           \p pos.rows() == 2 or 3
   \post          \p pos.rows() == 3
   \pre           \p connectE2.rows() == 2 or \p connectE2 is empty.
   \pre           \p connectT3.rows() == 3 or \p connectT3 is empty.

   \warning       This function does not check the validity of the resulting elements.
   \see           extrude_translate_T3, extrude_rotate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_spiral_T3 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   const DoubleVec3& T,
                   double h0, double h1, 
                   int pattern,
                   bool force_even,                         
                   UIntMat& connectT3);                          


/**
   Extrudes (sweeps) with rotation and translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The total rotation angle (radians unit).
   \param[in]     T              The total translation vector.
   \param[in]     N1             The number of layers.
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \pre           \p pos.rows() == 2 or 3
   \post          \p pos.rows() == 3
   \pre           \p connectE2.rows() == 2 or \p connectE2 is empty.
   \pre           \p connectQ4.rows() == 4 or \p connectQ4 is empty.

   \warning       This function does not check the validity of the resulting elements.
   \see           extrude_translate_T3, extrude_rotate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_spiral_Q4 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   const DoubleVec3& T,
                   unsigned N1,
                   UIntMat& connectQ4);                          

/**
   Extrudes (sweeps) with rotation and translation a cm2::CM2_EDGE2 mesh into a structured cm2::CM2_FACEQ4 mesh.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     connectE2      The connectivity matrix of the edge mesh (N0 elements).
   \param[in]     C              The centre of the rotation.
   \param[in]     R              The total rotation angle (radians unit).
   \param[in]     T              The total translation vector.
   \param[in]     h0             The size of the edges near the base mesh (along the translation vector \p T).
   \param[in]     h1             The size of the edges near the top mesh (along the translation vector \p T).
   \param[in]     force_even     Flag to force the number of edges along the translation (number of layers) to be even.
   \param[in,out] connectQ4      The connectivity of the output mesh. N0*N1 new columns are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value:
                     - -k   => the k-th argument had an illegal value
                     - -199 => memory allocation error.

   \pre           \p pos.rows() == 2 or 3
   \post          \p pos.rows() == 3
   \pre           \p connectE2.rows() == 2 or \p connectE2 is empty.
   \pre           \p connectQ4.rows() == 4 or \p connectQ4 is empty.

   \warning       This function does not check the validity of the resulting elements.
   \see           extrude_translate_T3, extrude_rotate_T3.
   */
CM2MESHTOOLS2D_API int 
extrude_spiral_Q4 (DoubleMat& pos, 
                   const UIntMat& connectE2, 
                   const DoubleVec3& C,
                   const DoubleVec3& R,
                   const DoubleVec3& T,
                   double h0, double h1, 
                   bool force_even,                         
                   UIntMat& connectQ4);                          

//@}

}  // end namespace meshtools2d
}  // end namespace cm2

#endif

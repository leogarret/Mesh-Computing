/**
   \file       mesh_struct2d.h
   \brief      Definitions of the structured 2-D mesh creation routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_STRUCT2D_H__
#define __CM2_MESH_STRUCT2D_H__


namespace cm2 {
namespace meshtools2d {

/****************************************************************************************/
/** @name Structured FACEQ4 or FACET3 mesh (regular grid) */
//@{

/**
   Makes structured FACEQ4 mesh (regular grid). 

   Takes boundary points as input and creates new points inside and returns element connectivity as output.
   The boundary points define a closed and preferably convex domain.

   The mesh is generated upon the Coons surface defined by the contour.
   
   Example:
   \verbatim

   Mesh of a square

   INPUT:

      pos = [0  1  2  2  2  1  0  0
             0  0  0  1  2  2  2  1]   (stored column-wise)

      indices = [0 1 2 3 4 5 6 7 0]

      N1    = 2


   OUTPUT:
                                      __ new node
                                     |
                                     V

      pos = [0  1  2  2  2  1  0  0  1
             0  0  0  1  2  2  2  1  1]      (stored column-wise)

      connectivity = [0 7 1 8                (stored column-wise)
                      1 8 2 3
                      8 5 3 4                
                      7 6 8 5]               (element are always oriented as the external contour) 

      4 elements:

               
                    line #2
               6      5
               *------*------*4      ^ 
               |      |      |       |
               |      |      |       |
   line #3     |     8|      |       |
   N1 elems   7*------*------*3      | line #1 : N1 elements
               |      |      |       |
               |      |      |       |
               |      |      |       |
               *------*------*       |
               0      1      2
                    line #0
            
   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     indices        The indices of the boundary nodes, successively, with no duplicate
                                 except for the first and the last ones, which should be equal to define a closed contour.
   \param[in]     N1             The number of elements along line #1 (= number of elements along line #3).
   \param[in,out] connectQ4      The connectivity of the output mesh. 
                                 N0*N1 new columns (elements) are appended.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Number of element along line #0 = number of elements along line #2.
               This implies that "indices.size()-1 - 2*N1" be even.
               N0 = N2 = (indices.size()-1 - 2*N1) / 2
   \pre        connectQ4.rows() == 4 or connectQ4 is empty upon entry.
   \pre        All the values in \p indices are smaller than \p pos.cols().
   \pre        The first index must be duplicated at the end of \p indices: indices().front() == indices.back().
   \note       The quality of the generated elements is not checked (elements may be warped because of bad contour).
   \see        extrude_translate_Q4.
   */
CM2MESHTOOLS2D_API int 
mesh_struct_Q4 
   (DoubleMat& pos, const UIntVec& indices, unsigned N1, UIntMat& connectQ4);                          



/**
   Makes structured FACET3 mesh (regular grid). 

   Takes boundary points as input and creates new points inside and returns points and element connectivity as output.
   The boundary points define a closed and preferably convex domain.

   The mesh is generated upon the Coons surface defined by the contour.

   Three options for the mesh pattern: left-oriented, right-oriented or "Union-Jack".
   
   Example:
   Mesh of a square

   \verbatim

   INPUT:

      pos = [0  1  2  2  2  1  0  0
             0  0  0  1  2  2  2  1]   (stored column-wise)

      indices = [0 1 2 3 4 5 6 7 0]

      N1    = 2



   OUTPUT (Right pattern):   

                                      __ new node
                                     |
                                     V

      pos = [0  1  2  2  2  1  0  0  1
             0  0  0  1  2  2  2  1  1]   (stored column-wise)

      connectT3 = [0 0 7 7 1 1 8 8        (stored column-wise)
                   1 8 8 5 2 3 3 4
                   8 7 5 6 3 8 4 5]       (element are always oriented as the external contour) 
                                          

      8 elements:


                    line #2
               6      5      4
               *------*------*       ^ 
               |    / |    / |       |
               |  /   |  /   |       |
   line #3     |/    8|/     |       |
   N1 elems   7*------*------*3      | line #1 : N1 elements
               |    / |    / |       |
               |  /   |  /   |       |
               |/     |/     |       |
               *------*------*       |
               0      1      2
                    line #0



   OUTPUT (Union-Jack pattern):        
     
                                      __ new node
                                     |
                                     V

      pos = [0  1  2  2  2  1  0  0  1
             0  0  0  1  2  2  2  1  1]   (stored column-wise)

      connectT3 = [0 0 7 8 1 2 8 8        (stored column-wise)
                   1 8 8 5 2 3 3 4
                   8 7 6 6 8 8 4 5]       (element are always oriented as the external contour) 
                                          

      8 elements:

         6      5      4
         *------*------* 
         |\     |    / |
         |  \   |  /   |
         |    \8|/     |
        7*------*------*3
         |    / |\     |
         |  /   |  \   |
         |/     |    \ |
         *------*------*
         0      1      2

   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     indices        The indices of the boundary nodes, successively, with no duplicate
                                 except that the first and the last index must be equal (closed contour).
   \param[in]     N1             The number of elements along line #1 (= number of elements along line #3).
   \param[in]     pattern        The orientation of the triangles:
                                    -  0: Left pattern.
                                    -  1: Right pattern.
                                    -  2: "Union-Jack" pattern.
   \param[in,out] connectT3      The connectivity of the output mesh. 
                                 2*N0*N1 new columns (elements) are appended.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Number of element along line #0 = number of elements along line #2.
               This implies that "indices.size()-1 - 2*N1" be even.
               N0 = N2 = (indices.size()-1 - 2*N1) / 2
   \pre        connectT3.rows() == 3 or connectT3 is empty.
   \pre        All the values in \p indices are smaller than \p pos.cols().
   \pre        The first index must be duplicated at the end of \p indices: indices().front() == indices.back().
   \note       The quality of the generated elements is not checked (elements may be warped because of bad contour).
   \see        extrude_translate_T3.
   */
CM2MESHTOOLS2D_API int 
mesh_struct_T3 
   (DoubleMat& pos, const UIntVec& indices, unsigned N1, int pattern, UIntMat& connectT3);  


/**
   Compute metrics inside a regular grid by double interpolation (isotropic version). 

   \param[in]     indices        The indices of the boundary nodes, successively, with no duplicate
                                 except that the first and the last index must be equal (closed contour).
   \param[in]     N1             The number of elements along line #1 (= number of elements along line #3).
   \param[in,out] metrics        The metrics. 
                                 Upon entry, nodes in \p indices should have a valid metric in this array.
                                 Upon exit, metrics for the nodes inside the grid are appended.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Number of element along line #0 = number of elements along line #2.
               This implies that "indices.size()-1 - 2*N1" be even.
               N0 = N2 = (indices.size()-1 - 2*N1) / 2
   \pre        The first index must be duplicated at the end of \p indices: indices().front() == indices.back().
   \post       The IDs of the nodes inside the grid follow the same scheme as mesh_struct_Q4 and mesh_struct_T3.
   \see        mesh_struct_Q4, mesh_struct_T3
   */
CM2MESHTOOLS2D_API int 
metrics_struct 
   (const UIntVec& indices, unsigned N1, DoubleVec& metrics);  

/**
   Compute metrics inside a regular grid by double interpolation (anisotropic version). 

   \param[in]     indices        The indices of the boundary nodes, successively, with no duplicate
                                 except that the first and the last index must be equal (closed contour).
   \param[in]     N1             The number of elements along line #1 (= number of elements along line #3).
   \param[in,out] metrics        The metrics. 
                                 Upon entry, nodes in \p indices should have a valid metric in this array.
                                 Upon exit, metrics for the nodes inside the grid are appended.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Number of element along line #0 = number of elements along line #2.
               This implies that "indices.size()-1 - 2*N1" be even.
               N0 = N2 = (indices.size()-1 - 2*N1) / 2
   \pre        The first index must be duplicated at the end of \p indices: indices().front() == indices.back().
   \post       The IDs of the nodes inside the grid follow the same scheme as mesh_struct_Q4 and mesh_struct_T3.
   \see        mesh_struct_Q4, mesh_struct_T3.
   */
CM2MESHTOOLS2D_API int 
metrics_struct 
   (const UIntVec& indices, unsigned N1, DoubleMat& metrics);  

//@}

}  // end namespace meshtools2d
}  // end namespace cm2

#endif

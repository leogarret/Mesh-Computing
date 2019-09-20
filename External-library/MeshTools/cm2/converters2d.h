/**
   \file       converters2d.h
   \brief      Definitions of the 2-D elements converter routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_CONVERTERS2D_H__
#define __CM2_CONVERTERS2D_H__


namespace cm2 {
namespace meshtools2d {

/****************************************************************************************/
/** @name Transformations / conversion */
//@{

/**
   Converts cm2::CM2_FACET3 elements into higher degree elements (cm2::CM2_FACET6, `FACET9` and over).

   Nodes are locally numbered as follow:
      -  0        -> 2              : Initial summits
      -  3        -> 3 + Ne - 1     : New nodes on edge 1-2, equally spaced, from N1 to N2.
      -  3 + Ne   -> 3 + 2*Ne - 1   : New nodes on edge 2-0, equally spaced, from N2 to N0.
      -  3 + 2*Ne -> 3 + 3*Ne - 1   : New nodes on edge 0-1, equally spaced, from N0 to N1.
      -  3 + 3*Ne -> 3 + 3*Ne + Ni  : New nodes inside. Equally spaced, numbered row by row, left to right, 
                                      starting with the row close to edge 2-0.

   Example:
   Conversion of a T3 into a T6
   \verbatim

   INPUT:

      pos = [2  0  0
             0  2  0                         (stored column-wise)
             0  0  0]

      connectivity = [0                      (stored column-wise)
                      1
                      2]                    

      Ne  = 1                                (1 more node on each edge)
      Ni  = 0                                (0 central node)


   OUTPUT:

      pos = [2  0  0  0  1  1
             0  2  0  1  0  1                (stored column-wise)
             0  0  0  0  0  0]

      connectivity = [0                      (stored column-wise)
                      1
                      2                
                      3
                      4
                      5]                    

       1
       *
       | \
       |   \
       |     \
      3*       *5
       |         \
       |           \
       |             \
       *-------*-------*
       2       4       0

   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in,out] connectM       The connectivity of the mesh.
                                 Input as a 3xNE matrix, and output as a (3 + 3 * \p Ne + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Ni             The number of nodes to add inside (only values among [0, 1, 3]).
   \param[in]     connectE       Optional boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges (usually boundary edges). \n
                                 Only high-order nodes for edges not in \p connectE are created.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 3 or \p connectM is empty.
   \pre        \p Ni = 0, 1 or 3 only.
   \pre        \p connectE is empty or \p connectE.rows() == 2 + \p Ne. 

   \note       The new nodes along the edges are created in an arbitrary order. \n
               But the internal nodes (when \p Ni > 0) are always created after the new edge nodes 
               (higher global node ID).
   \note       The new edge nodes are created on the straight line between the initial summits of each edge. \n
               The internal nodes are created on the linear surface.
   \note       Curved elements are allowed in \p connectE.

   \see        convert_Q4, meshtools1d::convert_E2.
   */
CM2MESHTOOLS2D_API int 
convert_T3 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Ni, const UIntMat& connectE);                          


/**
   Converts cm2::CM2_FACET3 elements into higher degree elements (cm2::CM2_FACET6, `FACET9` and over).

   Same as above without the connectE parameter.
      
   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in,out] connectM       The connectivity of the mesh.
                                 Input as a 3xNE matrix, and output as a (3 + 3 * \p Ne + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Ni             The number of nodes to add inside (only values among [0, 1, 3]).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 3 or \p connectM is empty.
   \pre        \p Ni = 0, 1 or 3 only.
   */
CM2MESHTOOLS2D_API int 
convert_T3 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Ni);


/**
   Converts cm2::CM2_FACEQ4 elements into higher degree elements (cm2::CM2_FACEQ8, cm2::CM2_FACEQ9 and over).

   Nodes are locally numbered as follow:
      -  0        -> 3              : Initial summits
      -  4        -> 4 + Ne - 1     : New nodes on edge 0-1, equally spaced, from N0 to N1.
      -  4 + Ne   -> 4 + 2*Ne - 1   : New nodes on edge 1-2, equally spaced, from N1 to N2.
      -  4 + 2*Ne -> 4 + 3*Ne - 1   : New nodes on edge 2-3, equally spaced, from N2 to N3.
      -  4 + 3*Ne -> 4 + 4*Ne - 1   : New nodes on edge 3-0, equally spaced, from N3 to N0.
      -  4 + 4*Ne -> 4 + 4*Ne + Ni  : New nodes inside. Equally spaced, numbered row by row, left to right, 
                                      starting with the row close to edge 0-1.

   Example:
   Conversion of a Q4 into a Q9
   \verbatim

   INPUT:

      pos = [0  2  2  0
             0  0  2  2                      (stored column-wise)
             0  0  0  0]

      connectivity = [0                      (stored column-wise)
                      1
                      2                
                      3]                    

      Ne  = 1                                (1 more node on each edge)
      Ni  = 1                                (1 central node)


   OUTPUT:

      pos = [0  2  2  0  1  2  1  0  1
             0  0  2  2  0  1  2  1  1       (stored column-wise)
             0  0  0  0  0  0  0  0  0]

      connectivity = [0                      (stored column-wise)
                      1
                      2                
                      3
                      4
                      5
                      6
                      7
                      8]                    

         3      6      2
         *------*------*
         |             |
         |             |
         |             |
        7*     8*      *5
         |             |
         |             |
         |             |
         *------*------*
         0      4      1

   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in,out] connectM       The connectivity of the mesh. \n
                                 Input as a 4xNE matrix, and output as a (4 + 4 * \p Ne + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0)
   \param[in]     Ni             The number of nodes to add inside (any square value).
   \param[in]     connectE       Optional boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges (usually boundary edges). \n
                                 Only high-order nodes for edges not in \p connectE are created.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 4 or \p connectM is empty.
   \pre        \p Ni = k * k, with any k >= 0
   \pre        \p connectE is empty or \p connectE.rows() == 2 + Ne. 

   \note       The new nodes along the edges are created in an arbitrary order. \n
               But the internal nodes (when \p Ni > 0) are always created after the new edge nodes.
   \note       The new edge nodes are created on the straight line between the initial summits of each edge. \n
               The internal nodes are created on the bilinear surface.
   \note       Curved elements are allowed in \p connectE.
   \see        convert_T3, meshtools1d::convert_E2.
   */
CM2MESHTOOLS2D_API int 
convert_Q4 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Ni, const UIntMat& connectE);                          

/**
   Converts cm2::CM2_FACEQ4 elements into higher degree elements (cm2::CM2_FACEQ8, cm2::CM2_FACEQ9 and over).

   Same as above without the connectE parameter.

   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in,out] connectM       The connectivity of the mesh.
                                 Input as a 4xNE matrix, and output as a (4 + 4 * \p Ne + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0)
   \param[in]     Ni             The number of nodes to add inside (any square value).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connect.rows() == 4 or \p connect is empty.
   \pre        \p Ni = k * k, with any k >= 0
   */
CM2MESHTOOLS2D_API int 
convert_Q4 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Ni);

/**
   Converts mixed cm2::CM2_FACET3 / cm2::CM2_FACEQ4 elements into higher degree elements 
   (cm2::CM2_FACET6 / cm2::CM2_FACEQ8, cm2::CM2_FACET6 / cm2::CM2_FACEQ9 and over).

   Nodes are locally numbered as in the convert_T3 and convert_Q4 functions.

   Using this function avoid node duplicates along the boundaries between triangles and quads
   (as would do two separate calls to convert_T3 and convert_Q4).
      
   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in,out] connectQ4      The connectivity of the quad mesh. \n
                                 Input as a 4xNQ matrix, and output as a (4 + 4 * \p Ne + \p Ni) x NQ matrix.
   \param[in,out] connectT3      The connectivity of the triangle mesh. \n
                                 Input as a 3xNT matrix, and output as a (3 + 3 * \p Ne + \p Ni) x NT matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0)
   \param[in]     Nqi            The number of nodes to add inside the quadrangles (any square value).
   \param[in]     Nti            The number of nodes to add inside the triangles (only values among [0, 1, 3]).
   \param[in]     connectE       Optional boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges (usually boundary edges). \n
                                 Only high-order nodes for edges not in \p connectE are created.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre        \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \pre        \p connectT3.rows() == 3 or \p connectT3 is empty.
   \pre        \p Nqi = k * k, with any k >= 0
   \pre        \p Nti = 0, 1 or 3.
   \pre        \p connectE is empty or \p connectE.rows() == 2 + \p Ne. 

   \note       The new nodes along the edges are created in an arbitrary order. \n
               But the internal nodes (when \p Nti > 0 or \p Nqi > 0) are always created after the new edge nodes.
   \note       The new edge nodes are created on the straight line between the initial summits of each edge. \n
               The internal nodes are created on the bilinear surface.
   \note       Curved elements are allowed in \p connectE.
   \note       A mixed mesh can be sorted with meshtools::sort_elfes to put quads first, then triangles.
   \see        convert_T3, convert_Q4, meshtools1d::convert_E2.
   */
CM2MESHTOOLS2D_API int 
convert_mixed (DoubleMat& pos, UIntMat& connectQ4, UIntMat& connectT3,
               unsigned Ne, unsigned Nqi, unsigned Nti, const UIntMat& connectE);                          

/**
   Converts mixed cm2::CM2_FACET3 / cm2::CM2_FACEQ4 elements into higher degree elements 
   (cm2::CM2_FACET6 / cm2::CM2_FACEQ8, cm2::CM2_FACET6 / cm2::CM2_FACEQ9 and over).

   Nodes are locally numbered as in the convert_T3 and convert_Q4 functions.

   Using this function avoid node duplicates along the boundaries between triangles and quads
   (as would do two separate calls to convert_T3 and convert_Q4).

   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in,out] connectQ4      The connectivity of the quad mesh. \n
                                 Input as a 4xNQ matrix, and output as a (4 + 4 * \p Ne + \p Ni) x NQ matrix.
   \param[in,out] connectT3      The connectivity of the triangle mesh. \n
                                 Input as a 3xNT matrix, and output as a (3 + 3 * \p Ne + \p Ni) x NT matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nqi            The number of nodes to add inside the quadrangles (any square value).
   \param[in]     Nti            The number of nodes to add inside the triangles (only values among [0, 1, 3]).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre           \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \pre           \p connectT3.rows() == 3 or \p connectT3 is empty.
   \pre           \p Nqi = k * k, with any k >= 0
   \pre           \p Nti = 0, 1 or 3.

   \note          A mixed mesh can be sorted with meshtools::sort_elfes to put quads first, then triangles.
   */
CM2MESHTOOLS2D_API int 
convert_mixed (DoubleMat& pos, UIntMat& connectQ4, UIntMat& connectT3,
               unsigned Ne, unsigned Nqi, unsigned Nti);


/**
   Transforms a mixed 2-D mesh into a all-quad mesh by splitting each triangle into three sub-quads 
   and each quad into four sub-quads.
      
   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in]     connectQ4      The connectivity of the quad mesh (dimensions 4xNQ).
   \param[in]     connectT3      The connectivity of the triangle mesh (dimensions 3xNT).
   \param[in]     connectE       Optional boundary connectivity matrix (dimensions 3xNE). \n
                                 This matrix can be used when some edge mid-nodes are already 
                                 present in matrix \p pos for some specific edges (usually boundary edges). \n
                                 Only mid-edge nodes for edges not in \p connectE are created.
   \param[in,out] connect_pure   The connectivity of the all-quad mesh (dimensions 4x(3*NT+4*NQ)).
                                 Split elements are appended to this matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre           \p connectQ4.rows() == 4 or \p connectQ4 is empty.
   \pre           \p connectT3.rows() == 3 or \p connectT3 is empty.
   \pre           \p connectE is empty or \p connectE.rows() == 3. 
   \pre           \p connect_pure.rows() == 4 or \p connect_pure is empty.

   \note          The new edge nodes are created on the straight line between the initial summits of each edge. \n
                  The internal nodes are created on the bilinear surface.
   \note          Conformity is preserved.
   \note          A mixed mesh can be sorted with meshtools::sort_elfes to put quads first, then triangles.
   \note          A node smoothing (by quadmesh::mesher) may be useful to improve the quality of the resulting all-quad mesh.
   */
CM2MESHTOOLS2D_API int 
split_into_Q4 (DoubleMat& pos, const UIntMat& connectQ4, const UIntMat& connectT3, const UIntMat& connectE, UIntMat& connect_pure);                          
//@}

}  // end namespace meshtools2d
}  // end namespace cm2

#endif

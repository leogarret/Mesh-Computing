/**
   \file       converters3d.h
   \brief      Declarations of the 3-D elements converter routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_CONVERTERS3D_H__
#define __CM2_CONVERTERS3D_H__


namespace cm2 {
namespace meshtools3d {

/****************************************************************************************/
/** @name Transformations / conversion */
//@{

/**
   Changes the orientation of tetrahedrons (TETRA4).

   Swaps the local nodes #0 and #1 of each element.

   Applied twice does not change anything.

   \param[in]     connectM        The connectivity matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect.rows() == 4, or \p connect is empty.
   \see           \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int
flip_TH4 (UIntMat& connectM);


/**
   Changes the orientation of wedges (WEDGE6).

   For each element, swaps the opposite triangle faces, 
   i.e. swaps local nodes #0, #1 and #2 with nodes #3, #4 and #5.
   Applied twice does not change anything.

   \param[in]     connectM        The connectivity matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect.rows() == 6, or \p connect is empty.
   \see           \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int
flip_W6 (UIntMat& connectM);


/**
   Changes the orientation of hexahedrons (HEXA8).

   For each element, swaps local nodes #0, #1, #2 and #3 with nodes #3, #4, #5 and #6.

   Applied twice does not change anything.

   \param[in]     connectM        The connectivity matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connect.rows() == 8, or \p connect is empty.
   \see           \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int
flip_H8 (UIntMat& connectM);


/**
   Converts TETRA4 elements into higher degree elements (TETRA10 and over).

   Nodes are locally numbered as follow:
   \verbatim
      0                 -> 3                     : Initial summits
      4                 -> 4 +   Ne - 1          : New nodes on edge 1-2, equally spaced, from N1 to N2.
      4 + Ne            -> 4 + 2*Ne - 1          : New nodes on edge 2-0, equally spaced, from N2 to N0.
      4 + 2*Ne          -> 4 + 3*Ne - 1          : New nodes on edge 0-1, equally spaced, from N0 to N1.
      4 + 3*Ne          -> 4 + 4*Ne - 1          : New nodes on edge 3-0, equally spaced, from N3 to N0.
      4 + 4*Ne          -> 4 + 5*Ne - 1          : New nodes on edge 3-1, equally spaced, from N3 to N1.
      4 + 5*Ne          -> 4 + 6*Ne - 1          : New nodes on edge 3-2, equally spaced, from N3 to N2.
      4 + 6*Ne          -> 4 + 6*Ne + Nft - 1    : New nodes on face 1-2-3, equally spaced, row by row, left to right.
      4 + 6*Ne + Nft    -> 4 + 6*Ne + 2*Nft - 1  : New nodes on face 2-0-3, equally spaced, row by row, left to right.
      4 + 6*Ne + 2*Nft  -> 4 + 6*Ne + 3*Nft - 1  : New nodes on face 1-3-0, equally spaced, row by row, left to right.
      4 + 6*Ne + 3*Nft  -> 4 + 6*Ne + 4*Nft - 1  : New nodes on face 2-1-0, equally spaced, row by row, left to right.
      4 + 6*Ne          -> 4 + 6*Ne + Ni         : New nodes inside, equally spaced, the node close to N0 first,
                                                   then the node close to N1, N2, then N3 (for the case Ni = 4).
   \endverbatim
 
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 4xNE matrix, and output as a (4 + 6 * \p Ne + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nft            The number of nodes to add on each face (only values among [0, 1, 3]).
   \param[in]     Ni             The number of nodes to add inside (only values among [0, 1, 4]).
   \param[in]     connectB       Optional triangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific faces (usually boundary elements). \n
                                 Only high-order nodes for elements not in \p connectB are created.
   \param[in]     connectE       Optional edge connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges. \n
                                 Only high-order nodes for edges not in \p connectE are created.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        Upon entry, \p connectM.rows() == 4 or \p connectM is empty.
   \pre        \p connectB is empty or \p connectB.rows() == 3 + 3 * \p Ne + \p Nf. 
   \pre        \p connectE is empty or \p connectE.rows() == 2 + \p Ne. 
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Ni = {0, 1, 4}

   \note       The new high-order nodes along the edges are always created before the nodes on
               the faces (higher global node ID), themselves before the node inside the elements.
   \note       The new nodes are created on the straight line between the initial summits of each edge
               and on the interpolated faces (triangles).
   \note       Curved elements are allowed in \p connectB and \p connectE.

   \see        meshtools3d::convert_H8, meshtools3d::convert_W6, meshtools3d::convert_P5, 
               meshtools2d::convert_Q4, meshtools2d::convert_T3.
   \see        \p fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int 
convert_TH4 (DoubleMat& pos, UIntMat& connectM,
             unsigned Ne, unsigned Nft, unsigned Ni,                         
             const UIntMat& connectB, const UIntMat& connectE);                          

/**
   Converts TETRA4 elements into higher degree elements (TETRA10 and over).

   Same as above without the \p connectB and \p connectE parameters.
 
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 4xNE matrix, and output as a (4 + 6 * \p Ne + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nft            The number of nodes to add on each face (only values among [0, 1, 3]).
   \param[in]     Ni             The number of nodes to add inside (only values among [0, 1, 4]).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        Upon entry, \p connectM.rows() == 4 or \p connectM is empty.
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Ni = {0, 1, 4}
   */
CM2MESHTOOLS3D_API int 
convert_TH4 (DoubleMat& pos, UIntMat& connectM,
             unsigned Ne, unsigned Nft, unsigned Ni);


/**
   Converts PYRAMID5 elements into higher degree elements (PYRAMID14 and over).

   Nodes are locally numbered as follow:
   \verbatim
      0        -> 4              : Initial summits.
      5        -> 5 +   Ne - 1   : New nodes on edge 0-4, equally spaced, from N0 to N4.
      5 + Ne   -> 5 + 2*Ne - 1   : New nodes on edge 1-4, equally spaced, from N1 to N4.
      5 + 2*Ne -> 5 + 3*Ne - 1   : New nodes on edge 2-4, equally spaced, from N2 to N4.
      5 + 3*Ne -> 5 + 4*Ne - 1   : New nodes on edge 3-4, equally spaced, from N3 to N4.
      5 + 4*Ne -> 5 + 5*Ne - 1   : New nodes on edge 0-1, equally spaced, from N0 to N1.
      5 + 5*Ne -> 5 + 6*Ne - 1   : New nodes on edge 1-2, equally spaced, from N1 to N2.
      5 + 6*Ne -> 5 + 7*Ne - 1   : New nodes on edge 2-3, equally spaced, from N2 to N3.
      5 + 7*Ne -> 5 + 8*Ne - 1   : New nodes on edge 3-0, equally spaced, from N3 to N0.

      5 + 8*Ne          -> 5 + 8*Ne + Nft - 1    : New nodes on face 0-4-1, equally spaced.
      5 + 8*Ne +  Nft   -> 5 + 8*Ne + 2*Nft - 1  : New nodes on face 1-4-2, equally spaced.
      5 + 8*Ne +  2*Nft -> 5 + 8*Ne + 3*Nft - 1  : New nodes on face 2-4-3, equally spaced.
      5 + 8*Ne +  3*Nft -> 5 + 8*Ne + 4*Nft - 1  : New nodes on face 3-4-0, equally spaced.

      5 + 8*Ne + 4*Nft       -> 5 + 8*Ne + 4*Nft + Nfq - 1    : New nodes on face 0-1-2-3, equally spaced, row by row, left to right.

      5 + 8*Ne + 4*Nft + Nfq -> 5 + 8*Ne + 4*Nft + Nfq + Ni   : New node inside (generated along the segment starting from 
                                                                the center of the quad face to the summit of the pyramid).
   \endverbatim

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 5xNE matrix, and output as a (5 + 8 * \p Ne + 4 * \p Nft + \p Nfq + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nfq            The number of nodes to add on each quadrangle (any square value).
   \param[in]     Nft            The number of nodes to add on each triangle (only values among [0, 1, 3]).
   \param[in]     Ni             The number of nodes to add inside (only values among [0, 1]).
                                 
   \param[in]     connectBQ      Optional quadrangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific elements (usually boundary elements).
   \param[in]     connectBT      Optional triangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific elements (usually boundary elements). \n
                                 Only high-order nodes for elements not in \p connectBT nor in \p connectBQ are created.
   \param[in]     connectE       Optional edge connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges. \n
                                 Only high-order nodes for edges not in \p connectE are created.

v
   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 5 or \p connectM is empty.
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Nfq = k * k, for any k >= 0
   \pre        \p connectBQ is empty or \p connectBQ.rows() == 4 + 4 * \p Ne + \p Nfq. 
   \pre        \p connectBT is empty or \p connectBT.rows() == 3 + 3 * \p Ne + \p Nft. 
   \pre        \p connectE is empty or \p connectE.rows() == 2 + \p Ne. 

   \note       The new high-order nodes along the edges are always created before the nodes on
               the faces (higher global node ID), themselves before the node inside the elements.
   \note       The new nodes are created on the straight line between the initial summits of each edge
               and on the interpolated faces (triangles and quadrangles).
   \note       Curved elements are allowed in \p connectBQ, \p connectBT and \p connectE.

   \see        meshtools3d::convert_H8, meshtools3d::convert_W6, meshtools3d::convert_TH4, 
               meshtools2d::convert_T3, meshtools2d::convert_Q4.
   \see        \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int 
convert_P5 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Nft, unsigned Nfq, unsigned Ni,                        
            const UIntMat& connectBQ, const UIntMat& connectBT, const UIntMat& connectE);                          

/**
   Converts PYRAMID5 elements into higher degree elements (PYRAMID14 and over).

   Same as above without the \p connectBQ, \p connectBT and \p connectE parameters.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 5xNE matrix, and output as a (5 + 8 * \p Ne + 4 * \p Nft + \p Nfq + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nfq            The number of nodes to add on each quadrangle (any square value).
   \param[in]     Nft            The number of nodes to add on each triangle (only values among [0, 1, 3]).
   \param[in]     Ni             The number of nodes to add inside (any value >= 0).

v
   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 5 or \p connectM is empty.
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Nfq = k * k, for any k >= 0
   */
CM2MESHTOOLS3D_API int 
convert_P5 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Nfq, unsigned Nft, unsigned Ni);


/**
   Converts WEDGE6 elements into higher degree elements (WEDGE18 and over).

   Nodes are locally numbered as follow:
   \verbatim
      0        -> 5              : Initial summits.
      6        -> 6 +   Ne - 1   : New nodes on edge 0-3, equally spaced, from N0 to N3.
      6 + Ne   -> 6 + 2*Ne - 1   : New nodes on edge 1-4, equally spaced, from N1 to N4.
      6 + 2*Ne -> 6 + 3*Ne - 1   : New nodes on edge 2-5, equally spaced, from N2 to N5.
      6 + 3*Ne -> 6 + 4*Ne - 1   : New nodes on edge 1-2, equally spaced, from N1 to N2.
      6 + 4*Ne -> 6 + 5*Ne - 1   : New nodes on edge 2-0, equally spaced, from N2 to N0.
      6 + 5*Ne -> 6 + 6*Ne - 1   : New nodes on edge 0-1, equally spaced, from N0 to N1.
      6 + 6*Ne -> 6 + 7*Ne - 1   : New nodes on edge 4-5, equally spaced, from N4 to N5.
      6 + 7*Ne -> 6 + 8*Ne - 1   : New nodes on edge 5-3, equally spaced, from N5 to N3.
      6 + 8*Ne -> 6 + 9*Ne - 1   : New nodes on edge 3-4, equally spaced, from N3 to N4.

      6 + 9*Ne         -> 6 + 9*Ne +   Nft - 1  : New nodes on face 0-1-2, equally spaced.
      6 + 9*Ne +  Nft  -> 6 + 9*Ne + 2*Nft - 1  : New nodes on face 3-5-4, equally spaced.

      6 + 9*Ne + 2*Nft         -> 6 + 9*Ne + 2*Nft + Nfq - 1    : New nodes on face 2-1-4-5, equally spaced, row by row, left to right.
      6 + 9*Ne + 2*Nft + Nfq   -> 6 + 9*Ne + 2*Nft + 2*Nfq - 1  : New nodes on face 5-3-0-2, equally spaced, row by row, left to right.
      6 + 9*Ne + 2*Nft + 2*Nfq -> 6 + 9*Ne + 2*Nft + 3*Nfq - 1  : New nodes on face 0-3-4-1, equally spaced, row by row, left to right.

      6 + 9*Ne + 2*Nft + 3*Nfq -> 6 + 9*Ne + 2*Nft + 3*Nfq + Ni : New node inside (0 or 1).
   \endverbatim

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 6xNE matrix, and output as a (6 + 9 * \p Ne + 2 * \p Nft + 3 * \p Nfq + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nfq            The number of nodes to add on each quadrangle (any square value).
   \param[in]     Nft            The number of nodes to add on each triangle (only values among [0, 1, 3]).
   \param[in]     Ni             The number of nodes to add inside (only values among [0, 1]).
   \param[in]     connectBQ      Optional quadrangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific elements (usually boundary elements).
   \param[in]     connectBT      Optional triangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific elements (usually boundary elements). \n
                                 Only high-order nodes for elements not in \p connectBT nor in \p connectBQ are created.
   \param[in]     connectE       Optional edge connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges. \n
                                 Only high-order nodes for edges not in \p connectE are created.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 6 or \p connectM is empty.
   \pre        \p Nfq = k * k, for any k >= 0
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Ni = {0, 1}
   \pre        \p connectBQ is empty or \p connectBQ.rows() == 4 + 4 * \p Ne + \p Nfq. 
   \pre        \p connectBT is empty or \p connectBT.rows() == 3 + 3 * \p Ne + \p Nft. 
   \pre        \p connectE is empty or \p connectE.rows() == 2 + \p Ne. 

   \note       The new high-order nodes along the edges are always created before the nodes on
               the faces (higher global node ID), themselves before the node inside the elements.
   \note       The new nodes are created on the straight line between the initial summits of each edge
               and on the interpolated faces (triangles and quadrangles).
   \note       Curved elements are allowed in \p connectBQ, \p connectBT and \p connectE.

   \see        meshtools3d::convert_H8, meshtools3d::convert_P5, meshtools3d::convert_TH4, 
               meshtools2d::convert_Q4, meshtools2d::convert_T3.
   \see        \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int 
convert_W6 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Nfq, unsigned Nft, unsigned Ni,                        
            const UIntMat& connectBQ, const UIntMat& connectBT, const UIntMat& connectE);                          

/**
   Converts WEDGE6 elements into higher degree elements (WEDGE18 and over).

   Same as above without the \p connectBQ, \p connectBT and \p connectE parameters.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 6xNE matrix, and output as a (6 + 9 * \p Ne + 2 * \p Nft + 3 * \p Nfq + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nfq            The number of nodes to add on each quadrangle (any square value).
   \param[in]     Nft            The number of nodes to add on each triangle (only values among [0, 1, 3]).
   \param[in]     Ni             The number of nodes to add inside (only values among [0, 1]).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 6 or \p connectM is empty.
   \pre        \p Nfq = k * k, for any k >= 0
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Ni = {0, 1}
   */
CM2MESHTOOLS3D_API int 
convert_W6 (DoubleMat& pos, 
            UIntMat& connectM,
            unsigned Ne, unsigned Nfq, unsigned Nft, unsigned Ni);


/**
   Converts HEXA8 elements into higher degree elements (HEXA28, HEXA27 and over).

   Nodes are locally numbered as follow:
   \verbatim
      0        -> 7              : Initial summits
      8        -> 8 + Ne - 1     : New nodes on edge 0-4, equally spaced, from N0 to N4.
      8 + Ne   -> 8 + 2*Ne - 1   : New nodes on edge 1-5, equally spaced, from N1 to N5.
      8 + 2*Ne -> 8 + 3*Ne - 1   : New nodes on edge 2-6, equally spaced, from N2 to N6.
      8 + 3*Ne -> 8 + 4*Ne - 1   : New nodes on edge 3-7, equally spaced, from N3 to N7.
      8 + 4*Ne -> 8 + 5*Ne - 1   : New nodes on edge 0-3, equally spaced, from N0 to N3.
      8 + 5*Ne -> 8 + 6*Ne - 1   : New nodes on edge 4-7, equally spaced, from N4 to N7.
      8 + 6*Ne -> 8 + 7*Ne - 1   : New nodes on edge 5-6, equally spaced, from N5 to N6.
      8 + 7*Ne -> 8 + 8*Ne - 1   : New nodes on edge 1-2, equally spaced, from N1 to N2.
      8 + 8*Ne -> 8 + 9*Ne - 1   : New nodes on edge 0-1, equally spaced, from N0 to N1.
      8 + 9*Ne -> 8 +10*Ne - 1   : New nodes on edge 3-2, equally spaced, from N3 to N2.
      8 +10*Ne -> 8 +11*Ne - 1   : New nodes on edge 7-6, equally spaced, from N7 to N6.
      8 +11*Ne -> 8 +12*Ne - 1   : New nodes on edge 4-5, equally spaced, from N4 to N5.

      8 +12*Ne        -> 8 +12*Ne +   Nf - 1 : New nodes on face {0 3 7 4}, equally spaced, row by row, left to right.
      8 +12*Ne + Nf   -> 8 +12*Ne + 2*Nf - 1 : New nodes on face {1 5 6 2}, equally spaced, row by row, left to right.
      8 +12*Ne + 2*Nf -> 8 +12*Ne + 3*Nf - 1 : New nodes on face {0 4 5 1}, equally spaced, row by row, left to right.
      8 +12*Ne + 3*Nf -> 8 +12*Ne + 4*Nf - 1 : New nodes on face {3 2 6 7}, equally spaced, row by row, left to right.
      8 +12*Ne + 4*Nf -> 8 +12*Ne + 5*Nf - 1 : New nodes on face {0 1 2 3}, equally spaced, row by row, left to right.
      8 +12*Ne + 5*Nf -> 8 +12*Ne + 6*Nf - 1 : New nodes on face {7 6 5 4}, equally spaced, row by row, left to right.

      8 +12*Ne + 6*Nf -> 8 +12*Ne + 6*Nf + Ni : New nodes inside, equally spaced, left to right, U, V, then W axes.
   \endverbatim
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 8xNE matrix, and output as a (8 + 12 * \p Ne + 6 * \p Nf + \p Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nf             The number of nodes to add on each face (any square value).
   \param[in]     Ni             The number of nodes to add inside (any cubic value).
   \param[in]     connectB       Optional quadrangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific faces (usually boundary elements). \n
                                 Only high-order nodes for elements not in \p connectB are created.
   \param[in]     connectE       Optional edge connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges. \n
                                 Only high-order nodes for edges not in \p connectE are created.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 8 or \p connectM is empty.
   \pre        \p Nf = k * k, for any k >= 0
   \pre        \p Ni = k * k * k, for any k >= 0
   \pre        \p connectB is empty or \p connectB.rows() == 4 + 4 * \p Ne + \p Nf. 
   \pre        \p connectE is empty or \p connectE.rows() == 2 + \p Ne. 

   \note       The new high-order nodes along the edges are always created before the nodes on
               the faces (higher global node ID), themselves before the node inside the elements.
   \note       The new nodes are created on the straight line between the initial summits of each edge
               and on the interpolated faces (quadrangles).
   \note       Curved elements are allowed in \p connectB and \p connectE.

   \see        meshtools3d::convert_W6, meshtools3d::convert_P5, meshtools3d::convert_TH4, 
               meshtools2d::convert_Q4, meshtools2d::convert_T3.
   \see        \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int 
convert_H8 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Nf, unsigned Ni,                          
            const UIntMat& connectB, const UIntMat& connectE);                          

/**
   Converts HEXA8 elements into higher degree elements (HEXA28, HEXA27 and over).

   Same as above without the \p connectB and \p connectE parameters.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectM       The connectivity matrix of the mesh. \n
                                 Input as a 8xNE matrix, and output as a (8 + 12 * \p Ne + 6 * \p Nf + Ni) x NE matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nfq            The number of nodes to add on each face (any square value).
   \param[in]     Ni             The number of nodes to add inside (any cubic value).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre        \p connectM.rows() == 8 or \p connectM is empty.
   \pre        \p Nf = k * k, for any k >= 0
   \pre        \p Ni = k * k * k, for any k >= 0
   */
CM2MESHTOOLS3D_API int 
convert_H8 (DoubleMat& pos, UIntMat& connectM,
            unsigned Ne, unsigned Nfq, unsigned Ni);


/**
   Converts mixed 3-D elements into higher degree elements.

   Nodes are locally numbered as in the convert_TH4, convert_P5, convert_W6 and convert_H8 functions.

   Using this function avoid node duplicates along the boundaries between elements
   (as would do two separate calls to convert_TH4, convert_P5, convert_W6 and convert_H8).
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectH8      The connectivity matrix of the hex mesh. \n
                                 Input as a 8xNH matrix, and output as a (8 + 12 * \p Ne + 6 * \p Nfq+ \p Nhi) x NH matrix.
   \param[in,out] connectW6      The connectivity matrix of the wedge mesh. \n
                                 Input as a 6xNW matrix, and output as a (6 + 9 * \p Ne + 3 * \p Nfq + 2 * \p Nft + \p Nwi) x NW matrix.
   \param[in,out] connectP5      The connectivity matrix of the pyramid mesh. \n
                                 Input as a 5xNP matrix, and output as a (5 + 8 * \p Ne + \p Nfq + 4 * \p Nft + \p Npi) x NP matrix.
   \param[in,out] connectTH4     The connectivity matrix of the tet mesh. \n
                                 Input as a 4xNT matrix, and output as a (4 + 6 * \p Ne + 4 * \p Nft + \p Nti) x NT matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nfq            The number of nodes to add on each quadrangle (any square value).
   \param[in]     Nft            The number of nodes to add on each triangle (only values among [0, 1, 3]).
   \param[in]     Nhi            The number of nodes to add inside the hexes (any cubic value).
   \param[in]     Nwi            The number of nodes to add inside the wedges (only values among [0, 1]).
   \param[in]     Npi            The number of nodes to add inside the pyramids (any value >= 0).
   \param[in]     Nti            The number of nodes to add inside the tets (only values among [0, 1, 4]).
   \param[in]     connectBQ      Optional quadrangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific elements (usually boundary elements).
   \param[in]     connectBT      Optional triangles boundary connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific elements (usually boundary elements). \n
                                 Only high-order nodes for elements not in \p connectBT nor in \p connectBQ are created.
   \param[in]     connectE       Optional edge connectivity matrix. \n
                                 This matrix can be used when some high-degree nodes are already 
                                 present in matrix \p pos for some specific edges. \n
                                 Only high-order nodes for edges not in \p connectE are created.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre        \p connectH8.rows() == 8 or \p connectH8 is empty.
   \pre        \p connectW6.rows() == 6 or \p connectW6 is empty.
   \pre        \p connectP5.rows() == 5 or \p connectP5 is empty.
   \pre        \p connectTH4.rows() == 4 or \p connectTH4 is empty.
   \pre        \p Nfq = k * k, for any k >= 0
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Nhi = k * k * k, for any k >= 0
   \pre        \p Nwi = {0, 1}
   \pre        \p Nti = {0, 1, 4}
   \pre        \p connectBQ is empty or \p connectBQ.rows() == 4 + 4 * \p Ne + \p Nfq. 
   \pre        \p connectBT is empty or \p connectBT.rows() == 3 + 3 * \p Ne + \p Nft. 
   \pre        \p connectE is empty or \p connectE.rows() == 2 + \p Ne. 

   \note       The new high-order nodes along the edges are always created before the nodes on
               the faces (higher global node ID), themselves before the node inside the elements.
   \note       The new nodes are created on the straight line between the initial summits of each edge
               and on the interpolated faces (quadrangles).
   \note       Curved elements are allowed in \p connectBQ, \p connectBT and \p connectE.

   \see        meshtools3d::convert_W6, meshtools3d::convert_P5, meshtools3d::convert_TH4, 
               meshtools2d::convert_Q4, meshtools2d::convert_T3.
   \see        \ref fe_geo_page for the definition of the local nodes.
   */
CM2MESHTOOLS3D_API int 
convert_mixed (DoubleMat& pos, UIntMat& connectH8, UIntMat& connectW6, UIntMat& connectP5, UIntMat& connectTH4,
               unsigned Ne, unsigned Nfq, unsigned Nft, unsigned Nhi, unsigned Nwi, unsigned Npi, unsigned Nti,                           
               const UIntMat& connectBQ, const UIntMat& connectBT, const UIntMat& connectE);                          


/**
   Converts mixed 3-D elements into higher degree elements.

   Same as above without the \p connectBQ, \p connectBT and \p connectE parameters.
      
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in,out] connectH8      The connectivity matrix of the hex mesh. \n
                                 Input as a 8xNH matrix, and output as a (8 + 12 * \p Ne + 6 * \p Nfq + \p Nhi) x NH matrix.
   \param[in,out] connectW6      The connectivity matrix of the wedge mesh. \n
                                 Input as a 6xNW matrix, and output as a (6 + 9 * \p Ne + 3 * \p Nfq + 2 * \p Nft + \p Nwi) x NW matrix.
   \param[in,out] connectP5      The connectivity matrix of the pyramid mesh. \n
                                 Input as a 5xNP matrix, and output as a (5 + 8 * \p Ne + \p Nfq + 4 * \p Nft + \p Npi) x NP matrix.
   \param[in,out] connectTH4     The connectivity matrix of the tet mesh. \n
                                 Input as a 4xNT matrix, and output as a (4 + 6 * \p Ne + 4 * \p Nft + \p Nti) x NT matrix.
   \param[in]     Ne             The number of nodes to add on each edge (any value >= 0).
   \param[in]     Nfq            The number of nodes to add on each quadrangle (any square value).
   \param[in]     Nft            The number of nodes to add on each triangle (only values among [0, 1, 3]).
   \param[in]     Nhi            The number of nodes to add inside the hexes (any cubic value).
   \param[in]     Nwi            The number of nodes to add inside the wedges (only values among [0, 1]).
   \param[in]     Npi            The number of nodes to add inside the pyramids (any value >= 0).
   \param[in]     Nti            The number of nodes to add inside the tets (only values among [0, 1, 4]).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre        \p connectH8.rows() == 8 or \p connectH8 is empty.
   \pre        \p connectW6.rows() == 6 or \p connectW6 is empty.
   \pre        \p connectP5.rows() == 5 or \p connectP5 is empty.
   \pre        \p connectTH4.rows() == 4 or \p connectTH4 is empty.
   \pre        \p Nfq = k * k, for any k >= 0
   \pre        \p Nft = {0, 1, 3}
   \pre        \p Nhi = k * k * k, for any k >= 0
   \pre        \p Nwi = {0, 1}
   \pre        \p Nti = {0, 1, 4}
   */
CM2MESHTOOLS3D_API int 
convert_mixed (DoubleMat& pos, UIntMat& connectH8, UIntMat& connectW6, UIntMat& connectP5, UIntMat& connectTH4,
               unsigned Ne, unsigned Nfq, unsigned Nft, unsigned Nhi, unsigned Nwi, unsigned Npi, unsigned Nti);                          


/**
   Transforms a mixed 3-D mesh (without any pyramid!) into a all-hex mesh by splitting 
   each tetrahedron into four sub-hexes, each wedge into six sub-hexes and each hexahedron into eight sub-hexes.
      
   \param[in,out] pos            The coordinates matrix. New nodes are appended.
   \param[in,out] connectH8      The connectivity matrix of the hex mesh (dimension 8 x NH).
   \param[in,out] connectW6      The connectivity matrix of the wedge mesh (dimension 6 x NW).
   \param[in,out] connectTH4     The connectivity matrix of the tet mesh (dimension 4 x NT).
   \param[in]     connectBQ      Optional quadrangles boundary connectivity matrix. \n
                                 This matrix can be used when some edge mid-nodes are already 
                                 present in matrix \p pos for some specific Q4 elements (usually boundary elements).
   \param[in]     connectBT      Optional triangles boundary connectivity matrix. \n
                                 This matrix can be used when some edge mid-nodes are already 
                                 present in matrix \p pos for some specific T3 elements (usually boundary elements).
   \param[in]     connectE       Optional edge connectivity matrix. \n
                                 This matrix can be used when some edge mid-nodes are already 
                                 present in matrix \p pos for some specific edge elements (usually boundary edges).
   \param[in,out] connect_pure   The connectivity of the all-hex mesh (dimensions 8 x (4 * NT + 6 * NW + 8 * NH)).
                                 Split elements are appended to this matrix.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre           \p connectH8.rows() == 8 or \p connectH8 is empty.
   \pre           \p connectW6.rows() == 6 or \p connectW6 is empty.
   \pre           \p connectTH4.rows() == 4 or \p connectTH4 is empty.
   \pre           \p connectBQ is empty or \p connectBQ.rows() == 9. 
   \pre           \p connectBT is empty or \p connectBT.rows() == 7. 
   \pre           \p connectE is empty or \p connectE.rows() == 3. 
   \pre           \p connect_pure.rows() == 8 or \p connect_pure is empty.

   \note       The new edge nodes are created on the straight line between the initial summits of each edge. \n
               The internal face nodes are created on the bilinear surfaces.
   \note       Conformity is preserved.
   \note       A mixed mesh can be sorted with meshtools::sort_elfes to put hexes first, then wedges and tets.
   \note       A node smoothing (by hexamesh::mesher) may be useful to improve the quality of the resulting all-hex mesh.
   */
CM2MESHTOOLS3D_API int 
split_into_H8 
   (DoubleMat& pos, const UIntMat& connectH8, const UIntMat& connectW6, const UIntMat& connectTH4,
    const UIntMat& connectBQ, const UIntMat& connectBT, const UIntMat& connectE, 
    UIntMat& connect_pure);                          
//@}

}  // end namespace meshtools3d
}  // end namespace cm2

#endif

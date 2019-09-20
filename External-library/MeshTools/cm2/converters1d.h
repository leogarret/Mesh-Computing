/**
   \file       converters1d.h
   \brief      Declarations of the 1D elements converter routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_CONVERTERS1D_H__
#define __CM2_CONVERTERS1D_H__


namespace cm2 {
namespace meshtools1d {

/****************************************************************************************/
/** @name Transformations / conversion */
//@{

/**
   Transforms a series of node indices into a mesh of CM2_EDGE2 elements.

   \param[in]     indices           Indices of the nodes.
   \param[in,out] connectE2         The connectivity matrix (new columns are appended). \n
                                    The matrix is resized to 2x(N0+N-1), with N0 = number of columns on input 
                                    and N = \p indices.size()
                                    \code
                                       connect(0,N0+j) = indices[j];
                                       connect(1,N0+j) = indices[j+1];
                                    \endcode

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Does not create any new node.

   \pre           \p connectE2.rows() == 2 or \p connectE2 is empty.
   \pre           \p indices.size() >= 2
   */
CM2MESHTOOLS1D_API int 
indices_to_connectE2 (const UIntVec& indices, UIntMat& connectE2);  


/**
   Transforms a series of node indices into a mesh of CM2_EDGE3 elements.

   \param[in]     indices           Indices of the nodes.
   \param[in,out] connectE3         The connectivity matrix (new columns are appended). \n
                                    The matrix is resized to 3x(N0+(N-1)/2), with N0 = number of columns on input 
                                    and N = \p indices.size()
                                    \code
                                       connect(0,N0+j) = indices[2*j];
                                       connect(1,N0+j) = indices[2*j+2];
                                       connect(2,N0+j) = indices[2*j+1];
                                    \endcode

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Does not create any new node.

   \pre           \p connectE3.rows() == 3 or \p connectE3 is empty.
   \pre           \p indices.size() = 2*k+1 with k > 0.
   */
CM2MESHTOOLS1D_API int 
indices_to_connectE3 (const UIntVec& indices, UIntMat& connectE3);  

/**
   Transforms a 3-node edge connectivity matrix into a 2-node edge connectivity matrix (doubled).

   \param[in]     connectE3         The 3-node edge connectivity matrix.
   \param[in,out] connectE2         The 2-node edge connectivity matrix. \n
                                    For each edge N0-N2-N1 in connectE3, two edges (N0-N2 and N2-N1) are appended.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          To simply discard the high-order nodes of the edges, use a 2-row view
                  on the connectivity matrix:
                  \code
                  UIntMat  connectE2 = connectE3.sub_matrix(0,0,2,connectE3.cols());
                  \endcode

   \pre           \p connectE3.rows() == 3 or \p connectE3 is empty.
   \pre           \p connectE2.rows() == 2 or \p connectE2 is empty.
   */
CM2MESHTOOLS1D_API int 
E3_to_2E2 (const UIntMat& connectE3, UIntMat& connectE2);  


/**
   Converts E2 elements into higher degree elements (E3, E4 and over).

   Nodes are locally numbered as follow:
      -  0 and 1           : initial summits
      -  2 to 2 + Ne - 1   : new nodes inside edge

   Example:
   Conversion of a E2 into a E4.
   \verbatim

   INPUT:

         *--------------|--------------*
         0              1              2

      pos = [0  3  6  
             0  0  0                         (coordinates stored column-wise)
             0  0  0]

      connectivity = [0  1                   (connectivity stored column-wise)
                      1  2]                    

      Ne  = 2                                (2 more nodes on each edge)


   OUTPUT:                          

      pos = [0  3  6  1  2  4  5  
             0  0  0  0  0  0  0             (coordinates stored column-wise)
             0  0  0  0  0  0  0]

      connectivity = [0  1                   (connectivity stored column-wise)
                      1  2
                      3  5             
                      4  6]                    

         *----|----|----*----|----|----*
         0    3    4    1    5    6    2

   \endverbatim
      
   \param[in,out] pos         The coordinates matrix. New nodes are appended.
   \param[in,out] connectE    The connectivity of the mesh. \n
                              Input as a 2xNE matrix, and output as a (2+Ne)xNE matrix
   \param[in]     Ne          The number of nodes to add on each edge (any value >= 0)

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       New nodes are created following the order of the edges in \p connectE.
               First the new node(s) for edge #0 (column 0 in \p connectE), 
               then for edge #1 (column 1 in \p connectE), and so on.
   \note       The internal nodes are created on the straight line between the initial summits of each edge.
               To get curved elements, you need to map these nodes to the "true" parametric line.
   \note       New nodes are equally spaced between the initial summits of each edge.
   
   \pre        All the values in the connectivity matrix \p connectE are smaller than \p pos.cols().
   \pre        \p connectE.rows() == 2 or \p connectE is empty.
   */
CM2MESHTOOLS1D_API int 
convert_E2 (DoubleMat& pos, UIntMat& connectE, unsigned Ne);                          

//@}

}  // end namespace meshtools1d
}  // end namespace cm2

#endif

/**
   \file       mesh_straight.h
   \brief      Definitions of some mesh creation routines on straight lines and broken lines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3500 $
   $Date: 2019-02-14 16:46:02 +0100 (jeu., 14 févr. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_STRAIGHT_H__
#define __CM2_MESH_STRAIGHT_H__


namespace cm2 {
namespace meshtools1d {

/****************************************************************************************/
/** @name Mesh straight lines and broken lines */
//@{

/**
   Creates a straight mesh between two nodes.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     N1             The index of the end node.
   \param[in]     NE             The number of elements to create between \p N0 and \p N1.
   \param[in,out] indices        The indices of the nodes (\a NE+1 indices are appended). \n
                                 The index \p N0 is first appended, then \p NE-1 new node indices,
                                 and finally index \p N1.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        \p N0 < \p pos.cols()
   \pre        \p N1 < \p pos.cols()

   \see        extrude_translate, mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_straight (DoubleMat& pos, 
               unsigned N0, unsigned N1, 
               unsigned NE,
               UIntVec& indices);  


/**
   Creates a straight mesh between three nodes.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     N1             The index of the middle node.
   \param[in]     N2             The index of the end node.
   \param[in]     NE             The number of elements to create between \p N0 and \p N2.
   \param[in,out] indices        The indices of the nodes (\a NE+1 indices are appended). \n
                                 The index \p N0 is first appended, then some new node indices,
                                 then \p N1, then some new node indices and finally index \p N2. \n
                                 The number of new indices in the two segments depends on their
                                 respective lengths. The longest segment will have more indices.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       When \p NE is odd, the number of indices between \p N0 - \p N1, and \p N1 - \p N2 is different. \n
               The longest segment will have one more index (arbitrary tie-breaking when lengths are equal).

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        \p N0 < \p pos.cols()
   \pre        \p N1 < \p pos.cols()
   \pre        \p N2 < \p pos.cols()

   \see        mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_straight (DoubleMat& pos, 
               unsigned N0, unsigned N1, unsigned N2,
               unsigned NE,
               UIntVec& indices);  


/**
   Creates a straight mesh between two nodes with specific sizes.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     N0             The index of the starting node.
   \param[in]     N1             The index of the end node.
   \param[in]     h0             The size of the edge near the starting node (N0).
   \param[in]     h1             The size of the edge near the ending node (N1).
   \param[in]     force_even     Flag to force the number of created edges to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes (\a N+1 indices are appended). \n
                                 The index \p N0 is first appended, then the \p N-1 new node indices,
                                 and finally index \p N1.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       When successful, at least one edge (two when \p force_even == `true`) is created between 
               \p N0 and \p N1. 

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        \p N0 < \p pos.cols()
   \pre        \p N1 < \p pos.cols()
   \pre        \p h0 > 0
   \pre        \p h1 > 0
   \see        extrude_translate, mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_straight (DoubleMat& pos, 
               unsigned N0, unsigned N1,
               double h0, double h1, 
               bool force_even,
               UIntVec& indices);  


/**
   Creates a broken-line mesh between some imposed nodes (hard nodes).
  
   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     hard_nodes     The index of the hard nodes that define the broken line.
   \param[in]     NE             The total number of elements to create (approximative).
   \param[in]     force_even     Flag to force the number of created edges between each pair of hard nodes to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes are appended beginning with hard_nodes[0], 
                                 then new nodes created between \p hard_nodes[0] and \p hard_nodes[1], 
                                 then \p hard_nodes[1] and so on.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note       When successful, at least one edge (two when \p force_even == `true`) is created between 
               each successive pair of hard nodes. \n
               This can lead to a difference between the total number of created edges and the target value \p NE.
               
   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        All the values in \p hard_nodes are smaller than \p pos.cols().
   \see        mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_straight (DoubleMat& pos, 
               const UIntVec& hard_nodes,
               unsigned NE, 
               bool force_even, 
               UIntVec& indices);  


/**
   Creates a broken-line mesh between some imposed nodes (hard nodes) with specific (isotropic) sizes.  

   The nodes are placed so that the edge lengths adjust as well as possible to the target
   size values given at the hard nodes.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     hard_nodes     The index of the hard nodes that define the broken line.
   \param[in]     sizes          The specific sizes at the hard nodes. \n
                                 The indices in this vector are relative to the vector hard_nodes:
                                 the value associated to the i-th hard node is defined by \p sizes[i] (not \p sizes[hard_nodes[i]]).
   \param[in]     force_even     Flag to force the number of created edges between each pair of hard nodes to be even. \n
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes are appended beginning with \p hard_nodes[0], 
                                 then new nodes created between \p hard_nodes[0] and \p hard_nodes[1], 
                                 then \p hard_nodes[1] and so on.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          When successful, at least one edge (two when \p force_even == `true`) is created between 
                  each successive pair of hard nodes. 

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        \p hard_nodes.size() >= 2.
   \pre        \p sizes.size() == \p hard_nodes.size()
   \pre        All the values in \p hard_nodes are smaller than \p pos.cols().
   \pre        All the values in \p sizes are > 0.
   \see        mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_straight (DoubleMat& pos, 
               const UIntVec& hard_nodes,
               const DoubleVec& sizes, 
               bool force_even, 
               UIntVec& indices);  

/**
   Creates a broken-line mesh between some imposed nodes (hard nodes) with specific anisotropic metrics.  

   The nodes are placed so that the edge lengths adjust as well as possible to the target
   metrics given at the hard nodes.

   \param[in,out] pos            The coordinates matrix. New points are appended.
   \param[in]     hard_nodes     The index of the hard nodes that define the broken line.
   \param[in]     metrics        The specific metrics at the hard nodes. \n
                                 The column IDs in this matrix are relative to the vector hard_nodes:
                                 the metric associated to the i-th hard node is defined by \p metrics.col(i) (not \p metrics.col(hard_nodes[i])).
   \param[in]     force_even     Flag to force the number of created edges between each pair of hard nodes to be even.
                                 Useful when the quadmesh::mesher is to be used.
   \param[in,out] indices        The indices of the nodes are appended beginning with \p hard_nodes[0], 
                                 then new nodes created between \p hard_nodes[0] and \p hard_nodes[1], 
                                 then \p hard_nodes[1] and so on.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          When successful, at least one edge (two when \p force_even == `true`) is created between 
                  each successive pair of hard nodes. 

   \pre        Dimension = 2 or 3 (\p pos.rows() == 2, or 3).
   \pre        \p hard_nodes.size() >= 2.
   \pre        \p metrics.cols() == \p hard_nodes.size()
   \pre        All the values in \p hard_nodes are smaller than \p pos.cols().
   \pre        All the columns in \p metrics are definite-positive
   \see        mesh_line.
   */
CM2MESHTOOLS1D_API int 
mesh_straight (DoubleMat& pos, 
               const UIntVec& hard_nodes,
               const DoubleMat& metrics, 
               bool force_even, 
               UIntVec& indices);  

//@}


}  // end namespace meshtools1d
}  // end namespace cm2

#endif

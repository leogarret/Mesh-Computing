/**
   \file       edge_qualities.h
   \brief      Declarations of the edge-quality related routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_EDGE_QUALITIES_H__
#define __CM2_EDGE_QUALITIES_H__


namespace cm2 {
namespace meshtools {

/****************************************************************************************/
/** @name Edges quality

   The "quality" of an edge is its length with respect to the metrics defined at its nodes. \n
   We use the following formula:

   \f$ q_h(N_0 N_1) = L \ \frac{\ln(\frac{h_0}{h_1})}{h_0 - h_1} \f$

   where:
      - \f$ h_0 \f$ = Metric at node 0 (isotropic case).
      - \f$ h_1 \f$ = Metric at node 1 (isotropic case).
      - \f$ L \f$ = Length of segment \f$ N_0 N_1 \f$.

   A length-quality of 1 is a perfect length.
   The edge is too short when its length-quality is below 1. It's too long when it's over 1. */

//@{

/**
   Gets all the edges (unique) in a mesh.

   \param[in]     connectM    The connectivity matrix.
   \param[in]     FE_type     The type of the elements (see \ref fe_geo_page).
   \param[in,out] connectE    The matrix of the edges (dimensions 2 x `nedgs`).\n
                              Edges are appended at the back of the matrix.\n
                              Edges are unique but their relative order is insignificant.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connectE.rows() == 2 or \p connectE is empty.
   */
CM2MESHTOOLS_API int 
copy_edges (const UIntMat& connectM, cm2::element_type FE_type, UIntMat& connectE);

/**
   Gets all the edges (unique) in several meshes.

   \param[in]     connect_beg    Pointer to the beginning of the array of the connectivity matrices.
   \param[in]     connect_end    Pointer to the past-the-end connectivity matrix = the next after the last.
   \param[in]     FE_types       The types of the elements (see \ref fe_geo_page).
   \param[in,out] connectE       The matrix of the edges (dimensions 2 x `nedgs`).\n
                                 Edges are appended at the back of the matrix.\n
                                 Edges are unique but their relative order is insignificant.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p FE_types.size() == \p connect_end - \p connect_beg
   \pre           \p connectE.rows() == 2 or \p connectE is empty.
   */
CM2MESHTOOLS_API int 
copy_edges (const UIntMat* connect_beg, const UIntMat* connect_end,
            const IntVec& FE_types, UIntMat& connectE);

/**
   Compute the length measure of a segment with two size values at its summits.

   \param[in]     L              The actual length of the segment (must be >= 0).
   \param[in]     h0             The first size value (must be > 0). 
   \param[in]     h1             The second size value (must be > 0). 
   \param[out]    qh             The length measure with respect to \p h0 and \p h1. \n
   \note                         The quality equals to 1 when the length \p L is optimal according to \p h0 and \p h1. \n
                                 The quality is greater than 1 when the length \p L is too long. \n
                                 The quality is lesser than 1 when the length \p L is too short. \n

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.
   */
CM2MESHTOOLS_API int 
edge_quality (double L, double h0, double h1, double& qh);

/**
   Adds in an histogram the qualities of a batch of edges (isotropic version).

   \param[in]     pos            The coordinates matrix.
   \param[in]     connectM       The connectivity matrix.
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page).
   \param[in]     metrics        The target (i.e. optimal) sizes at each node.\n  
                                 To get a quality of 1, all edges connected to the i-th node must have length equal to `size[i]`.
   \param[in,out] histo_Qh       Qualities are counted into the histogram (bins must have been previously defined).
   \param[in]     excludedE      Optional. The edges to exclude.  

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \warning       To count only the hits for this mesh, the hits of the histogram must have been cleared before.
   */
CM2MESHTOOLS_API int 
edge_qualities 
   (const DoubleMat& pos, const UIntMat& connectM, cm2::element_type FE_type, 
    const DoubleVec& metrics, misc::histogram& histo_Qh, const UIntMat& excludedE = UIntMat());

/**
   Adds in an histogram the qualities of a batch of edges (anisotropic version).

   \param[in]     pos            The coordinates matrix.
   \param[in]     connectM       The connectivity matrix.
   \param[in]     FE_type        The type of the elements (see \ref fe_geo_page).
   \param[in]     metrics        The target (i.e. optimal) metric at each node.  
   \param[in,out] histo_Qh       Qualities are counted into the histogram (bins must have been previously defined).
   \param[in]     excludedE      Optional. The edges to exclude.  

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \warning       To count only the hits for this mesh, the hits of the histogram must have been cleared before.
   */
CM2MESHTOOLS_API int 
edge_qualities 
   (const DoubleMat& pos, const UIntMat& connectM, cm2::element_type FE_type, 
    const DoubleMat& metrics, misc::histogram& histo_Qh, const UIntMat& excludedE = UIntMat());

/**
   Adds in an histogram the qualities of a batch of edges (isotropic version).

   \param[in]     pos            The coordinates matrix.
   \param[in]     connect_beg    Pointer to the first connectivity matrix.
   \param[in]     connect_end    Pointer to the past-the-end connectivity matrix = the next after the last.
   \param[in]     FE_types       The types of the elements (see \ref fe_geo_page).
   \param[in]     metrics        The target (i.e. optimal) sizes at each node.\n  
                                 To get a quality of 1, all edges connected to the i-th node must have length equal to `size[i]`.
   \param[in,out] histo_Qh       Qualities are counted into the histogram (bins must have been previously defined).
   \param[in]     excludedE      Optional. The edges to exclude.  

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre           \p metrics.size() == \p pos.cols()
   \pre           \p FE_types.size() == \p connect_end - \p connect_beg
   \see           \ref fe_geo_page.
   */
CM2MESHTOOLS_API int 
edge_qualities 
   (const DoubleMat& pos, const UIntMat* connect_beg, const UIntMat* connect_end, const IntVec& FE_types, 
    const DoubleVec& metrics, misc::histogram& histo_Qh, const UIntMat& excludedE = UIntMat());

/**
   Adds in an histogram the qualities of a batch of edges (anisotropic version).

   \param[in]     pos            The coordinates matrix.
   \param[in]     connect_beg    Pointer to the first connectivity matrix.
   \param[in]     connect_end    Pointer to the past-the-end connectivity matrix = the next after the last.
   \param[in]     FE_types       The types of the elements (see \ref fe_geo_page).
   \param[in]     metrics        The target (i.e. optimal) metric at each node.  
   \param[in,out] histo_Qh       Qualities are counted into the histogram (bins must have been previously defined).
   \param[in]     excludedE      Optional. The edges to exclude.  

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre           \p sizes.size() == \p pos.cols()
   \pre           \p FE_types.size() == \p connect_end - \p connect_beg
   \see           \ref fe_geo_page.
   */
CM2MESHTOOLS_API int 
edge_qualities 
   (const DoubleMat& pos, const UIntMat* connect_beg, const UIntMat* connect_end, const IntVec& FE_types, 
    const DoubleMat& metrics, misc::histogram& histo_Qh, const UIntMat& excludedE = UIntMat());
//@}


/****************************************************************************************/
/** @name H-shock

   The H-shock is a measure of the metric variations along the edges in a mesh.
   
   Along an edge \f$ N_0 N_1 \f$, the H-shock is defined by the following formula:

   \f$ \mbox{H-shock}(N_0 N_1) = \max{\left( \frac{h_0}{h_1},\frac{h_1}{h_0} \right)} ^ {\frac{1}{L}} - 1 \f$

   where:
      - \f$ h_0 \f$ = Metric at node 0 (isotropic case).
      - \f$ h_1 \f$ = Metric at node 1 (isotropic case).
      - \f$ L \f$ = Length of segment \f$ N_0 N_1 \f$ with respect to metrics \f$ h_0 \f$ and \f$ h_1 \f$.
   */

//@{

/**
   Computes the H-shock in a mesh (isotropic case).

   For a node `i`, `H_shock[i]` is the max value of the H-shocks along all edges connected to `i`. \n
   It is a positive scalar. 

   \param[in]     pos            The coordinates matrix.
   \param[in]     connectE       The edges as a connectivity matrix.\n
                                 Edges can be extracted from a mesh (or several meshes) by the function meshtools::copy_edges.
   \param[in]     metrics        The isotropic metric at each node.
   \param[in,out] Hshock         The H-shock at each node. \n
                                 For a node Ni, if `Hshock[Ni]` exists upon entry, 
                                 the value is replaced with the new H-shock value computed on the edges \p connectE 
                                 if this new value is larger.

   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \post          \p H_shock.size() >= \p pos.cols()
   */
CM2MESHTOOLS_API int 
H_shock 
   (const DoubleMat& pos, const UIntMat& connectE, const DoubleVec& metrics, DoubleVec& Hshock);

/**
   Computes the H-shock in a mesh (anisotropic case).

   The H-shock is a measure of the metric variations along the edges in a mesh.

   For a node Ni, H_shock[Ni] is the max value of the H-shocks along all edges connected to Ni.
   It is a positive scalar. 

   \param[in]     pos            The coordinates matrix.
   \param[in]     connectE       The edges as a connectivity matrix.\n
                                 Edges can be extracted from a mesh (or several meshes) by the function meshtools::copy_edges.
   \param[in]     metrics        The anisotropic metric at each node.
   \param[in,out] Hshock         The H-shock at each node. \n
                                 For a node Ni, if `Hshock[Ni]` exists upon entry, 
                                 the value is replaced with the new H-shock value computed on the edges \p connectE 
                                 if this new value is larger.
                                  
   \return        Returns a negative value in case of error. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \post          \p H_shock.size() >= \p pos.cols()
   */
CM2MESHTOOLS_API int 
H_shock 
   (const DoubleMat& pos, const UIntMat& connectE, const DoubleMat& metrics, DoubleVec& Hshock);
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

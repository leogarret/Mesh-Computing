/**
   \file       mesh_transform1d.h
   \brief      Declarations of some 1-D mesh transformation routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_TRANSFORM1D_H__
#define __CM2_MESH_TRANSFORM1D_H__


namespace cm2 {
namespace meshtools1d {


/****************************************************************************************/
/** @name Reorientation of 1-D elements */
//@{

/**
   Changes the orientation of the edges in a 1D mesh (CM2_EDGE2).

   Swaps the local nodes #0 and #1 of each element. This reverse the edges. \n
   Applied twice does not change anything.

   \param[in]     connectE2         The connectivity matrix (2xN).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connectE2.rows() == 2, or connectE2 is empty.
   */
CM2MESHTOOLS1D_API int
flip (UIntMat& connectE2);


/**
   Changes the orientation of the edges so that all elements have similar orientation 
   as a reference element.

   The algorithm involved is a neighboring search starting from element \p ref_element.

   If code == 0, only the elements in the same subdomain(1) as element \p ref_element
   will be visited. Other elements will remain unchanged.\n
   If code != 0, all elements will be visited. Elements in other subdomains than that of \p ref_element, if any, 
   will be reoriented as the first (lowest ID) element in each subdomain.

   \note A subdomain is a domain's subset of connected elements.

   The \p neighbors matrix is defined by: 

      `neighbors(i,j)` = neighbor of element `j` by the boundary `i`
      with the local node and boundary IDs defined as follow: 

      \verbatim

            N0             N1
         E1  *-------------*  E0

      \endverbatim
   
   \param[in,out] connect           The connectivity matrix (2xNEFS).
   \param[in,out] neighbors         The neighbors matrix (2xNEFS).
   \param[in]     ref_element       The index of the reference element in \p connect.
   \param[in]     code              The code to tell whether all elements must be processed (\p code != 0),
                                    or only the elements among the subdomain of \p ref_element (\p code == 0).

   \return     Number of flipped elements. Returns a negative value in case of error.
               Returned value = -k => the k-th argument had an illegal value.

   \pre        \p connect.rows() == 2, or \p connect is empty.
   \pre        \p neighbors.rows() == 2
   \pre        \p neighbors.cols() == \p connect.cols()    
   \pre        \p ref_element < \p connect.cols()  

   \note       The initial neighbors matrix can be computed by the meshtools::get_neighbors function.
   */
CM2MESHTOOLS1D_API int 
mesh_reorient (UIntMat& connect, UIntMat& neighbors, unsigned ref_element, int code);                          


}  // end namespace meshtools1d
}  // end namespace cm2

#endif

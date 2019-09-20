#ifndef __CM2_TRANSLATOR_MEDIT__
#define __CM2_TRANSLATOR_MEDIT__

/**
   \file       translator_medit.h
   \brief      Declarations of the routines for the MEDIT output file format.
   \see        http://www.ann.jussieu.fr/~frey/logiciels/medit.html
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#include "fe_defs.h"


namespace cm2 {
namespace meshtools {

/****************************************************************************************/
/** @name MEDIT output */
//@{

/**
   MEDIT output of a mesh (INRIA's format).

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect           The connectivity matrix.
   \param[in]     FE_type           The type of the elements (see \ref fe_geo_page).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int
medit_output (const char* filename, 
              const DoubleMat& pos, 
              const UIntMat& connect, 
              cm2::element_type FE_type);

/**
   MEDIT output of a mesh (INRIA's format) with multiple subdomains.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect           The connectivity matrix.
   \param[in]     FE_type           The type of the elements (see \ref fe_geo_page).
   \param[in]     refs              The references of the elements.
                                    Each reference will be translated into a MEDIT's subdomain.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           \p refs.size() == \p connect.cols()
   */
CM2MESHTOOLS_API int
medit_output (const char* filename, 
              const DoubleMat& pos, 
              const UIntMat& connect, 
              cm2::element_type FE_type,
              const UIntVec& refs);

/**
   MEDIT output of two meshes (INRIA's format).

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect1          The connectivity matrix #1.
   \param[in]     connect2          The connectivity matrix #2.
   \param[in]     FE_type1          The type of elements of mesh #1 (see \ref fe_geo_page).
   \param[in]     FE_type2          The type of elements of mesh #2 (see \ref fe_geo_page).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int
medit_output (const char* filename, 
              const DoubleMat& pos, 
              const UIntMat& connect1, 
              const UIntMat& connect2,
              cm2::element_type FE_type1,
              cm2::element_type FE_type2);

/**
   MEDIT output of several meshes (INRIA's format).

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect_beg       Pointer to the first connectivity matrix.
   \param[in]     connect_end       Pointer to the past-the-end connectivity matrix = the next after the last.
   \param[in]     FE_types          The types of elements (see \ref fe_geo_page).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre           \p FE_types.size() == \p connect_end - \p connect_beg
   */
CM2MESHTOOLS_API int
medit_output (const char* filename, 
              const DoubleMat& pos, 
              const UIntMat* connect_beg,
              const UIntMat* connect_end,
              const IntVec& FE_types);


/**
   MEDIT output for scalar fields (`bb` format).

   This function is useful to visualize scalar fields (2-D or 3-D).

   \param[in]     filename          The file name.
   \param[in]     dim               The dimension of the space (2 or 3).
   \param[in]     V                 The vector of the scalar values.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Must be associated to a mesh file (`.mesh`).
   \note          The positions in \p V correspond to the node IDs in the mesh.
   */
CM2MESHTOOLS_API int
medit_scalar (const char* filename, unsigned dim, const DoubleVec& V);

/**
   MEDIT output for vector fields (`bb` format).

   This function is useful to visualize vector fields (2-D or 3-D).

   \param[in]     filename          The file name.
   \param[in]     dim               The dimension of the space (2 or 3).
   \param[in]     M                 The vectors stored column-wise.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Must be associated to a mesh file (`.mesh`).
   \note          The columns in \p M correspond to the node IDs in the mesh.
   */
CM2MESHTOOLS_API int
medit_vector (const char* filename, unsigned dim, const DoubleMat& M);

/**
   MEDIT output for tensors (symmetric real definite positive matrices) (`bb` format).

   This function is useful to visualize anisotropic metrics (2-D or 3-D).

   \param[in]     filename          The file name.
   \param[in]     M                 The tensors stored column-wise.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \pre           M.rows() == 3 or M.rows() == 6.

   \note          Must be associated to a mesh file (`.mesh`).
   \note          The columns in \p M correspond to the node IDs in the mesh.
   */
CM2MESHTOOLS_API int
medit_tensor (const char* filename, const DoubleMat& M);
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

#ifndef __CM2_TRANSLATOR_ENSIGHT__
#define __CM2_TRANSLATOR_ENSIGHT__

/**
   \file       translator_ensight.h
   \brief      Declarations of the routines for the EnSight output file format.
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
/** @name EnSight output */
//@{

/**
   EnSight output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect           The connectivity matrix.
   \param[in]     FE_type           The type of the elements (see \ref fe_geo_page).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int
ensight_output (const char* filename, 
                const DoubleMat& pos, 
                const UIntMat& connect, 
                cm2::element_type FE_type);

/**
   EnSight output of two meshes.

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
ensight_output (const char* filename, 
                const DoubleMat& pos, 
                const UIntMat& connect1, 
                const UIntMat& connect2,
                cm2::element_type FE_type1,
                cm2::element_type FE_type2);

/**
   EnSight output of several meshes.

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
ensight_output (const char* filename, 
                const DoubleMat& pos, 
                const UIntMat* connect_beg,
                const UIntMat* connect_end,
                const IntVec& FE_types);
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

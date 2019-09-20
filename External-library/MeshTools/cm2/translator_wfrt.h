#ifndef __CM2_TRANSLATOR_WFRT__
#define __CM2_TRANSLATOR_WFRT__

/**
   \file       translator_wfrt.h
   \brief      Declarations of the routines for the Alias' Wavefront OBJ file format (input and output).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3619 $
   $Date: 2019-05-10 11:06:57 +0200 (ven., 10 mai 2019) $ 
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
/** @name Wavefront input/output */
//@{

/**
   Wavefront OBJ input of a mesh.

   \param[in]     filename          The file name.
   \param[out]    pos               The coordinates matrix. The initial matrix is lost.
   \param[out]    normals           The normals at the vertices, if any. The initial matrix is lost.
   \param[out]    textures          The textures at the vertices, if any. The initial matrix is lost.
   \param[out]    connectT          The triangle connectivity matrix. The initial matrix is lost.
   \param[out]    connectQ          The quadrangle connectivity matrix. The initial matrix is lost.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          Can read only triangle and quadrangle polygons.
   \note          The initial matrices \p pos, \p connectT and \p connectQ are lost.
   */
CM2MESHTOOLS_API int
WavefrontOBJ_input 
   (const char* filename, DoubleMat& pos, DoubleMat& normals, DoubleMat& textures, 
    UIntMat& connectT, UIntMat& connectQ);

/**
   Wavefront obj output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect           The connectivity matrix. Only one type of element.
   \param[in]     FE_type           The type of the elements: CM2_FACET3 or CM2_FACEQ4 (see \ref fe_geo_page).

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int
WavefrontOBJ_output
   (const char* filename, const DoubleMat& pos, 
    const UIntMat& connect, cm2::element_type FE_type);

/**
   Wavefront obj output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix (dimension 3xNODS).
   \param[in]     connect1          The connectivity matrix #1.
   \param[in]     connect2          The connectivity matrix #2.
   \param[in]     FE_type1          The type of elements of mesh #1: CM2_FACET3 or CM2_FACEQ4 (see \ref fe_geo_page).
   \param[in]     FE_type2          The type of elements of mesh #2: CM2_FACET3 or CM2_FACEQ4 (see \ref fe_geo_page).

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 3.
   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int
WavefrontOBJ_output
   (const char* filename, const DoubleMat& pos, 
    const UIntMat& connect1, const UIntMat& connect2,
    cm2::element_type FE_type1, cm2::element_type FE_type2);

/**
   Wavefront obj output of a mesh with possibly normals and texture coordinates.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     normals           The normals (x, y, z) at the vertices, if any. 
   \param[in]     textures          The textures coordinates (u, v) at the vertices, if any. 
   \param[in]     connectT          The triangle connectivity matrix. 
   \param[in]     connectQ          The quadrangle connectivity matrix. 

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p connectT is empty or (\p connectT.rows() == 3). Triangle mesh.
   \pre           \p connectQ is empty or (\p connectQ.rows() == 4). Quadrangle mesh.
   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \pre           \p normals.rows() == 3 and \p normals.cols() = \p pos.cols(), or \p normals.empty().
   \pre           \p textures.rows() == 2 and \p textures.cols() = \p pos.cols(), or \p textures.empty().
   */
CM2MESHTOOLS_API int
WavefrontOBJ_output
   (const char* filename, const DoubleMat& pos, const DoubleMat& normals, const DoubleMat& textures, 
    const UIntMat& connectT, const UIntMat& connectQ);
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

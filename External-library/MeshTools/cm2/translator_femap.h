#ifndef __CM2_TRANSLATOR_FEMAP__
#define __CM2_TRANSLATOR_FEMAP__

/**
   \file       translator_femap.h
   \brief      Declarations of the routines for the FEMAP neutral file format (input and output).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

/*
-----------------------------------------------------------------------------------
SUPPORTED ELEMENTS : FEMAP TYPE <-> CM2 TYPE
-----------------------------------------------------------------------------------
0D
9 <-> CM2_NODE
-----------------------------------------------------------------------------------
1D
0 <-> CM2_EDGE2
1 <-> CM2_EDGE3
-----------------------------------------------------------------------------------
2D
2 <-> CM2_FACET3
3 <-> CM2_FACET6
4 <-> CM2_FACEQ4
5 <-> CM2_FACEQ8
-----------------------------------------------------------------------------------
3D
6 <-> CM2_TETRA4
7 <-> CM2_WEDGE6
8 <-> CM2_HEXA8
10 <-> CM2_TETRA10
12 <-> CM2_HEXA20
-----------------------------------------------------------------------------------
*/



namespace cm2 {
namespace meshtools {

/****************************************************************************************/
/** @name FEMAP input/output */
//@{

/**
   FEMAP input of a mesh.

   \param[in]     filename          The file name.
   \param[out]    pos               The coordinates matrix.
   \param[out]    connect           The connectivity matrix. Only one type of element.
   \param[out]    FE_type           The type of the elements (see \ref fe_geo_page).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The initial matrices (\p pos and \p connect) are lost.
   \post          \p pos.rows() == 3 or \p pos.empty().
   \warning       The following FEMAP types are not supported: 
                  `Wedge15`, `Rigid`, `MultiList`, `Contact`, and `Weld`.
   \warning       Node IDs are -1 shifted from file (IDs start at zero in CM2).
   \warning       Element IDs start at zero in CM2.
   \warning       Still experimental.
   */
CM2MESHTOOLS_API int
FEMAP_input
   (const char* filename, DoubleMat& pos, 
    UIntMat& connect, cm2::element_type& FE_type);

/**
   FEMAP input of a mesh.

   \param[in]     filename          The file name.
   \param[out]    pos               The coordinates matrix.
   \param[out]    connect1          The connectivity matrix #1. Only one type of element.
   \param[out]    connect2          The connectivity matrix #2. Only one type of element.
   \param[out]    FE_type1          The type of the elements in matrix connect1 (see \ref fe_geo_page).
   \param[out]    FE_type2          The type of the elements in matrix connect2 (see \ref fe_geo_page).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The initial matrices (\a pos, \p connect1 and \p connect2) are lost.
   \post          \p pos.rows() == 3 or \p pos.empty().
   \warning       The following FEMAP types are not supported: 
                  `Wedge15`, `Rigid`, `MultiList`, `Contact`, and `Weld`.
   \warning       Node IDs are -1 shifted from file (IDs start at zero in CM2).
   \warning       Element IDs start at zero in CM2.
   \warning       Still experimental.
   */
CM2MESHTOOLS_API int
FEMAP_input
   (const char* filename, DoubleMat& pos, 
    UIntMat& connect1, UIntMat& connect2, 
    cm2::element_type& FE_type1, cm2::element_type& FE_type2);

/**
   FEMAP input of a mesh.

   \param[in]     filename          The file name.
   \param[out]    pos               The coordinates matrix.
   \param[out]    connect           The connectivity matrix. It may contain several blocks with different types.
   \param[out]    xConnect          The block indices. The i-th block in \p connect starts at `xConnect[i]` and 
                                    ends at `xConnect[i+1]` (past-the-end).
   \param[out]    fe_types          The types of element stored in each block.
   \param[in]     refs              The references (colors) of the elements.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The initial matrices (\a pos and \p connect) and vectors 
                  (\a xConnect, \p fe_types and \p refs) are lost.
   \post          \p pos.rows() == 3 or \p pos.empty().
   \post          \p connect.empty() or \p fe_types.size()+1 == \p xConnect.cols().
   \post          \p refs.size() == \p connect.cols().
   \warning       The following FEMAP types are not supported: 
                  `Wedge15`, `Rigid`, `MultiList`, `Contact`, and `Weld`.
   \warning       Node IDs are -1 shifted from file (IDs start at zero in CM2).
   \warning       Element IDs start at zero in CM2.
   \warning       Still experimental.
   */
CM2MESHTOOLS_API int
FEMAP_input
   (const char* filename, DoubleMat& pos, UIntMat& connect,
    UIntVec& xConnect, IntVec& fe_types, UIntVec& refs);

/**
   FEMAP output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix (dimension 3xNODS).
   \param[in]     connect           The connectivity matrix. Only one type of element.
   \param[in]     FE_type           The type of the elements (see \ref fe_geo_page).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 2 or 3 or \p pos.empty().
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \warning       The following CM2 types are not supported: 
                  cm2::CM2_FACEQ9, cm2::CM2_PYRAMID5, cm2::CM2_PYRAMID14, cm2::CM2_WEDGE18, and cm2::CM2_HEXA27.
   \warning       Node and element IDs are +1 shifted on file (IDs start at one on file).
   \warning       Still experimental.
   */
CM2MESHTOOLS_API int
FEMAP_output
   (const char* filename, const DoubleMat& pos, 
    const UIntMat& connect, cm2::element_type FE_type);

/**
   FEMAP output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix (dimension 3xNODS).
   \param[in]     connect1          The connectivity matrix #1.
   \param[in]     connect2          The connectivity matrix #2.
   \param[in]     FE_type1          The type of elements of mesh #1 (see \ref fe_geo_page).
   \param[in]     FE_type2          The type of elements of mesh #2 (see \ref fe_geo_page).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 2 or 3 or \p pos.empty().
   \pre           All the values in the connectivity matrices are smaller than \p pos.cols().
   \warning       The following CM2 types are not supported: 
                  cm2::CM2_FACEQ9, cm2::CM2_PYRAMID5, cm2::CM2_PYRAMID14, cm2::CM2_WEDGE18, and cm2::CM2_HEXA27.
   \warning       Node and element IDs are +1 shifted on file (IDs start at one on file).
   \warning       Still experimental.
   */
CM2MESHTOOLS_API int
FEMAP_output
   (const char* filename, const DoubleMat& pos, 
    const UIntMat& connect1, const UIntMat& connect2,
    cm2::element_type FE_type1, cm2::element_type FE_type2);

/**
   FEMAP output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix (dimension 3xNODS).
   \param[in]     connect           The connectivity matrix. It may contain several blocks with different types.
   \param[in]     xConnect          The block indices. The i-th block in \p connect starts at `xConnect[i]` and 
                                    ends at `xConnect[i+1]` (past-the-end).
   \param[in]     fe_types          The types of element stored in each block.
   \param[in]     refs              The references (colors) of the elements.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 2 or 3 or \p pos.empty().
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           connect.empty() or fe_types.size()+1 == xConnect.cols().
   \pre           \p refs.size() == \p connect.cols().
   \warning       Node and element IDs are +1 shifted on file (IDs start at one on file).
   \warning       The following CM2 types are not supported: 
                  cm2::CM2_FACEQ9, cm2::CM2_PYRAMID5, cm2::CM2_PYRAMID14, cm2::CM2_WEDGE18, and cm2::CM2_HEXA27.
   \warning       Still experimental.
*/
CM2MESHTOOLS_API int
FEMAP_output
   (const char* filename, const DoubleMat& pos, const UIntMat& connect,
    const UIntVec& xConnect, const IntVec& fe_types, const UIntVec& refs);
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

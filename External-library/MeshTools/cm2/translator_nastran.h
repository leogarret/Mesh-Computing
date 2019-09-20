#ifndef __CM2_TRANSLATOR_NASTRAN__
#define __CM2_TRANSLATOR_NASTRAN__

/**
   \file       translator_nastran.h
   \brief      Declarations of the routines for the NASTRAN (BULK data) file format (input and output).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3445 $
   $Date: 2019-01-11 12:02:52 +0100 (ven., 11 janv. 2019) $ 
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
/** @name NASTRAN input/output */
//@{

/**
   NASTRAN input of a mesh.

   \param[in]     filename          The file name.
   \param[out]    pos               The coordinates matrix.
   \param[out]    connect           The connectivity matrix. Only one type of element.
   \param[out]    FE_type           The type of the elements (see \ref fe_geo_page).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          \p pos.rows() == 3 or \p  pos.empty().
   \note          The initial matrices (\p pos and \p connect) are lost.
   \note          The current NASTRAN supported types are:
                  `CBAR`, `CBEAM`, `CROD`, `CTRIA3`, `CTRIA6`, `CQUAD4`, `CQUAD8`, 
                  `CTETRA` (4-node and 10-node), `CPYRAM` (5-node and 13-node), 
                  `CPENTA` (6-node and 15-node), `CHEXA` (8-node and 20-node).
   \note          Small-field, large-field and free-field formats are supported.
   \warning       Node and element IDs are -1 shifted from file (IDs are 0-based in CM2).
   */
CM2MESHTOOLS_API int
NASTRAN_input
   (const char* filename, DoubleMat& pos, UIntMat& connect, cm2::element_type& FE_type);

/**
   NASTRAN input of a mesh.

   \param[in]     filename          The file name.
   \param[out]    pos               The coordinates matrix.
   \param[out]    connect1          The connectivity matrix #1. Only one type of element.
   \param[out]    connect2          The connectivity matrix #2. Only one type of element.
   \param[out]    FE_type1          The type of the elements in matrix connect1 (see \ref fe_geo_page).
   \param[out]    FE_type2          The type of the elements in matrix connect2 (see \ref fe_geo_page).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          \p pos.rows() == 3 or \p pos.empty().
   \note          The initial matrices (\p pos, \p connect1 and \p connect2) are lost.
   \note          The current NASTRAN supported types are:
                  `CBAR`, `CBEAM`, `CROD`, `CTRIA3`, `CTRIA6`, `CQUAD4`, `CQUAD8`, 
                  `CTETRA` (4-node and 10-node), `CPYRAM` (5-node and 13-node), 
                  `CPENTA` (6-node and 15-node), `CHEXA` (8-node and 20-node).
   \note          Small-field, large-field and free-field formats are supported.
   \warning       Node and element IDs are -1 shifted from file (IDs are 0-based in CM2).
   */
CM2MESHTOOLS_API int
NASTRAN_input
   (const char* filename, DoubleMat& pos, 
    UIntMat& connect1, UIntMat& connect2, 
    cm2::element_type& FE_type1, cm2::element_type& FE_type2);

/**
   NASTRAN input of a mesh.

   \param[in]     filename          The file name.
   \param[out]    pos               The coordinates matrix.
   \param[out]    connect           The connectivity matrix. It may contain several blocks with different types.
   \param[out]    xConnect          The block indices. The i-th block in \p connect starts at `xConnect[i]` and 
                                    ends at `xConnect[i+1]` (past-the-end).
   \param[out]    fe_types          The types of element stored in each block.
   \param[out]    refs              The references (colors) of the elements.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \post          \p pos.rows() == 3 or \p pos.empty().
   \post          \p connect.empty() or \p fe_types.size()+1 == \p xConnect.cols().
   \post          \p refs.size() == \p connect.cols().
   \note          The initial matrices (\p pos and \p connect) and vectors 
                  (\p xConnect, \p fe_types and \p refs) are lost.
   \note          The current NASTRAN supported types are:
                  `CBAR`, `CBEAM`, `CROD`, `CTRIA3`, `CTRIA6`, `CQUAD4`, `CQUAD8`, 
                  `CTETRA` (4-node and 10-node), `CPYRAM` (5-node and 13-node), 
                  `CPENTA` (6-node and 15-node), `CHEXA` (8-node and 20-node).
   \note          Small-field, large-field and free-field formats are supported.
   \warning       Node and element IDs are -1 shifted from file (IDs are 0-based in CM2).
   */
CM2MESHTOOLS_API int
NASTRAN_input (const char* filename, DoubleMat& pos, UIntMat& connect,
               UIntVec& xConnect, IntVec& fe_types, UIntVec& refs);

/**
   NASTRAN output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix (dimension 3xNODS).
   \param[in]     connect           The connectivity matrix. Only one type of element.
   \param[in]     FE_type           The type of the elements (see \ref fe_geo_page).
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 2 or 3 or \p pos.empty().
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \note          The following CM2 types are converted to their NASTRAN serendipity closest type:
                  cm2::CM2_EDGE3 to 2-node `CBAR` (discarding last node), 
                  cm2::CM2_FACEQ9 to 8-node `CQUAD8` (discarding last node), 
                  cm2::CM2_PYRAMID14 to 13-node `CPYRAM` (discarding last node), 
                  cm2::CM2_WEDGE18 to 15-node `CPENTA` (discarding last three nodes)
                  and cm2::CM2_HEXA27 to 20-node `CHEXA` (discarding last seven nodes).
   \note          Outputs GRIDs (i.e. nodes) in large-field format and Connections (i.e. elements) in free-field format.
   \warning       Node and element IDs are +1 shifted on file (IDs are 1-based in NASTRAN).
   */
CM2MESHTOOLS_API int
NASTRAN_output
   (const char* filename, const DoubleMat& pos, 
    const UIntMat& connect, cm2::element_type FE_type);

/**
   NASTRAN output of a mesh.

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
   \note          The following CM2 types are converted to their NASTRAN serendipity closest type:
                  cm2::CM2_EDGE3 to 2-node `CBAR` (discarding last node), 
                  cm2::CM2_FACEQ9 to 8-node `CQUAD8` (discarding last node), 
                  cm2::CM2_PYRAMID14 to 13-node `CPYRAM` (discarding last node), 
                  cm2::CM2_WEDGE18 to 15-node `CPENTA` (discarding last three nodes)
                  and cm2::CM2_HEXA27 to 20-node `CHEXA` (discarding last seven nodes).
   \note          Outputs GRIDs (i.e. nodes) in large-field format and Connections (i.e. elements) in free-field format.
   \warning       Node and element IDs are +1 shifted on file (IDs are 1-based in NASTRAN).
   */
CM2MESHTOOLS_API int
NASTRAN_output
   (const char* filename, const DoubleMat& pos, 
    const UIntMat& connect1, const UIntMat& connect2,
    cm2::element_type FE_type1, cm2::element_type FE_type2);

/**
   NASTRAN output of a mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect           The connectivity matrix. It may contain several blocks with different types.
   \param[in]     xConnect          The block indices. The i-th block in \p connect starts at `xConnect[i]` and 
                                    ends at `xConnect[i+1]` (past-the-end).
   \param[in]     fe_types          The types of element stored in each block.
   \param[in]     refs              The references (colors) of the elements.
   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \pre           \p pos.rows() == 2 or 3 or \p pos.empty().
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   \pre           \p connect.empty() or \p fe_types.size()+1 == \p xConnect.cols().
   \pre           \p refs.size() == \p connect.cols().
   \note          The following CM2 types are converted to their NASTRAN serendipity closest type:
                  cm2::CM2_EDGE3 to 2-node `CBAR` (discarding last node), 
                  cm2::CM2_FACEQ9 to 8-node `CQUAD8` (discarding last node), 
                  cm2::CM2_PYRAMID14 to 13-node `CPYRAM` (discarding last node), 
                  cm2::CM2_WEDGE18 to 15-node `CPENTA` (discarding last three nodes)
                  and cm2::CM2_HEXA27 to 20-node `CHEXA` (discarding last seven nodes).
   \note          Outputs GRIDs (i.e. nodes) in large-field format and Connections (i.e. elements) in free-field format.
   \warning       Node and element IDs are +1 shifted on file (IDs are 1-based in NASTRAN).
   */
CM2MESHTOOLS_API int
NASTRAN_output (const char* filename, const DoubleMat& pos,
                const UIntMat& connect, const UIntVec& xConnect,
                const IntVec& fe_types, const UIntVec& refs);
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

#ifndef __CM2_TRANSLATOR_STL__
#define __CM2_TRANSLATOR_STL__

/**
   \file       translator_stl.h
   \brief      Declarations of the routines for the STL file format (input and output).
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
/** @name STL input/output */
//@{

/**
   STL input of a triangle mesh.

   \param[in]     filename          The file name.
                                    The associated file can be ASCII 
                                    (must contain "solid" in the first file line)
                                    or binary.
   \param[out]    pos               The coordinates matrix. The initial matrix is lost.
   \param[out]    connect           The connectivity matrix. The initial matrix is lost.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The initial matrices \p pos and \p connect are lost.
   \note          Multiple "solids" in ASCII files are concatenated in a single 
                  output \p connect matrix.
   */
CM2MESHTOOLS_API int
STL_input (const char* filename, DoubleMat& pos, UIntMat& connect);

/**
   STL output of a triangle mesh.

   \param[in]     filename          The file name.
   \param[in]     pos               The coordinates matrix.
   \param[in]     connect           The connectivity matrix.
   \param[in]     ASCII             ASCII / Binary flag. Default = ASCII.

   \return        Error code. Returns 0 when successful. \n
                  Returned value = -k => the k-th argument had an illegal value.

   \warning       In ASCII mode, the coordinates are written with `double` precision (i.e. `"%lf"` format),
                  loosing no precision. \n
                  Whereas, the STL binary mode supporting only single precision float values, the coordinates
                  are converted (`double` to `float` conversions) prior to writing. \n
                  This can cause some accuracy issues when reading back a STL binary file.
   \pre           \p connect is either empty or its rows number is equal to 3. \n
                  It should contain a triangle mesh.
   \pre           All the values in the connectivity matrix are smaller than \p pos.cols().
   */
CM2MESHTOOLS_API int
STL_output (const char* filename, const DoubleMat& pos, const UIntMat& connect,
            bool ASCII = true);
//@}

}  // end namespace meshtools
}  // end namespace cm2

#endif

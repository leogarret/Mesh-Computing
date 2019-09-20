/**
   \file       math1.h
   \brief      File to include all CM2 math containers.
   \details    This file provides access to the numerical types of the library (cm2::DoubleVec3, cm2::UIntVec, cm2::DoubleSym...) 
   \details    The template mathematical functions are available through the math1ops.h header.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATH1_H__
#define __CM2_MATH1_H__


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"


// Define CM2MATH1IMPORT, CM2MATH1EXPORT, CM2MATH1LOCAL and CM2MATH1PUBLIC.
#if (defined(WIN32) | defined(WIN64))
   #if defined(_USRDLL)
      #define CM2MATH1EXPORT __declspec(dllexport)
   #else
      #define CM2MATH1EXPORT
   #endif
   #if defined(CM2_LIBIMPORT)
      #define CM2MATH1IMPORT
   #else
      #define CM2MATH1IMPORT __declspec(dllimport)
   #endif
   #define CM2MATH1LOCAL
   #define CM2MATH1PUBLIC
#elif defined(CM2_USE_GCC_VISIBILITY_ATTRIBS)
   #define CM2MATH1IMPORT
   #define CM2MATH1EXPORT __attribute__ ((visibility("default")))
   #define CM2MATH1LOCAL __attribute__ ((visibility("hidden")))
   #define CM2MATH1PUBLIC __attribute__ ((visibility("default")))
#else
   #define CM2MATH1IMPORT
   #define CM2MATH1EXPORT
   #define CM2MATH1LOCAL
   #define CM2MATH1PUBLIC
#endif

// Define CM2MATH1_API for DLL builds.
#ifdef CM2MATH1_EXPORTS
   #define CM2MATH1_API CM2MATH1EXPORT
#else
   #define CM2MATH1_API CM2MATH1IMPORT
#endif


// CM2 API
#include "math1_aux.h"
#include "dense1d.h"
#include "dense2d.h"
#include "symmetric_full.h"
#include "vector_fixed.h"
#include "matrix_fixed.h"
#include "symmetric_fixed.h"
#include "triangular_lower.h"
#include "triangular_upper.h"
#include "vecio.h"
#include "matio.h"



/**
   \namespace cm2
   \brief Main namespace for CM2.
*/
namespace cm2 {

#ifdef CM2_EXPLICIT_INSTANCE_EXPORT

/*
   Explicit instantiations.

   These explicit instantiations are exported by this DLL. \n
   They can be exchanged between DLLs. \n
   Other math containers can be instantiated in other DLLs 
   but they are not exported by this math1 DLL.
*/ 
template class CM2MATH1_API Ptr<math1::ref_counted>;

template class CM2MATH1_API vector_num<unsigned>;
template class CM2MATH1_API vector_num<int>;
template class CM2MATH1_API vector_num<float>;
template class CM2MATH1_API vector_num<double>;
template class CM2MATH1_API vector_num<std::complex<double> >;

template class CM2MATH1_API Ptr<vector_num<unsigned> >;
template class CM2MATH1_API Ptr<vector_num<int> >;
template class CM2MATH1_API Ptr<vector_num<float> >;
template class CM2MATH1_API Ptr<vector_num<double> >;
template class CM2MATH1_API Ptr<vector_num<std::complex<double> > >;

template class CM2MATH1_API dense1D<unsigned>;
template class CM2MATH1_API dense1D<int>;
template class CM2MATH1_API dense1D<float>;
template class CM2MATH1_API dense1D<double>;
template class CM2MATH1_API dense1D<std::complex<double> >;

template class CM2MATH1_API dense2D<unsigned>;
template class CM2MATH1_API dense2D<int>;
template class CM2MATH1_API dense2D<float>;
template class CM2MATH1_API dense2D<double>;
template class CM2MATH1_API dense2D<std::complex<double> >;

template class CM2MATH1_API symmetric_full<unsigned>;
template class CM2MATH1_API symmetric_full<int>;
template class CM2MATH1_API symmetric_full<float>;
template class CM2MATH1_API symmetric_full<double>;
template class CM2MATH1_API symmetric_full<std::complex<double> >;

template class CM2MATH1_API vector_fixed<unsigned,2>;
template class CM2MATH1_API vector_fixed<unsigned,3>;
template class CM2MATH1_API vector_fixed<unsigned,4>;
template class CM2MATH1_API vector_fixed<unsigned,6>;
template class CM2MATH1_API vector_fixed<unsigned,8>;
template class CM2MATH1_API vector_fixed<unsigned,9>;
template class CM2MATH1_API vector_fixed<unsigned,12>;

template class CM2MATH1_API vector_fixed<float,2>;
template class CM2MATH1_API vector_fixed<float,3>;
template class CM2MATH1_API vector_fixed<float,4>;
template class CM2MATH1_API vector_fixed<float,6>;
template class CM2MATH1_API vector_fixed<float,8>;
template class CM2MATH1_API vector_fixed<float,9>;
template class CM2MATH1_API vector_fixed<float,12>;

template class CM2MATH1_API vector_fixed<double,2>;
template class CM2MATH1_API vector_fixed<double,3>;
template class CM2MATH1_API vector_fixed<double,4>;
template class CM2MATH1_API vector_fixed<double,6>;
template class CM2MATH1_API vector_fixed<double,8>;
template class CM2MATH1_API vector_fixed<double,9>;
template class CM2MATH1_API vector_fixed<double,12>;

template class CM2MATH1_API matrix_fixed<unsigned,2,2>;
template class CM2MATH1_API matrix_fixed<unsigned,3,1>;
template class CM2MATH1_API matrix_fixed<unsigned,2,3>;
template class CM2MATH1_API matrix_fixed<unsigned,3,2>;
template class CM2MATH1_API matrix_fixed<unsigned,3,3>;
template class CM2MATH1_API matrix_fixed<unsigned,3,4>;

template class CM2MATH1_API matrix_fixed<double,2,2>;
template class CM2MATH1_API matrix_fixed<double,3,1>;
template class CM2MATH1_API matrix_fixed<double,2,3>;
template class CM2MATH1_API matrix_fixed<double,3,2>;
template class CM2MATH1_API matrix_fixed<double,3,3>;
template class CM2MATH1_API matrix_fixed<double,3,4>;

template class CM2MATH1_API symmetric_fixed<unsigned,2>;
template class CM2MATH1_API symmetric_fixed<unsigned,3>;

template class CM2MATH1_API symmetric_fixed<float,2>;
template class CM2MATH1_API symmetric_fixed<float,3>;

template class CM2MATH1_API symmetric_fixed<double,2>;
template class CM2MATH1_API symmetric_fixed<double,3>;

#endif   // CM2_EXPLICIT_INSTANCE_EXPORT



/** \defgroup VectorVariableInst cm2::dense1D instantiations exported from the math1 DLL */
//@{
/// Typedef for exported class dense1D<unsigned> (variable-sized vector of unsigned).
typedef dense1D<unsigned>                          UIntVec;
/// Typedef for exported class dense1D<int> (variable-size vector of int).
typedef dense1D<int>                               IntVec;
/// Typedef for exported class dense1D<float> (variable-size vector of float).
typedef dense1D<float>                             FloatVec;
/// Typedef for exported class dense1D<double> (variable-size vector of double).
typedef dense1D<double>                            DoubleVec;
/// Typedef for exported class dense1D<complex<double>> (variable-size vector of complex<double>s).
typedef dense1D<std::complex<double> >             DoubleZVec;
//@}


/** \defgroup MatrixVariableInst cm2::dense2D instantiations exported from the math1 DLL */
//@{
/// Typedef for exported class dense2D<unsigned> (variable-size matrix of unsigned).
typedef dense2D<unsigned>                          UIntMat;
/// Typedef for exported class dense2D<int> (variable-size matrix of int).
typedef dense2D<int>                               IntMat;
/// Typedef for exported class dense2D<float> (variable-size matrix of float).
typedef dense2D<float>                             FloatMat;
/// Typedef for exported class dense2D<double> (variable-size matrix of double).
typedef dense2D<double>                            DoubleMat;
/// Typedef for exported class dense2D<complex<double>> (variable-size matrix of complex<double>s).
typedef dense2D<std::complex<double> >             DoubleZMat;
//@}


/** \defgroup MatrixSymVariableInst cm2::symmetric_full instantiations exported from the math1 DLL */
//@{
/// Typedef for exported class symmetric_full<unsigned> (variable-size symmetric matrix of unsigned).
typedef symmetric_full<unsigned>                   UIntSym;
/// Typedef for exported class symmetric_full<int> (variable-size symmetric matrix of int).
typedef symmetric_full<int>                        IntSym;
/// Typedef for exported class symmetric_full<float> (variable-size symmetric matrix of float).
typedef symmetric_full<float>                      FloatSym;
/// Typedef for exported class symmetric_full<double> (variable-size symmetric matrix of double).
typedef symmetric_full<double>                     DoubleSym;
/// Typedef for exported class symmetric_full<complex<double>> (variable-size symmetric matrix of complex<double>s).
typedef symmetric_full<std::complex<double> >      DoubleZSym;
//@}


/** \defgroup VectorFixedInst cm2::vector_fixed instantiations exported from the math1 DLL */
//@{
/// Typedef for exported class vector_fixed<unsigned, 2> (fixed-size vector of 2 unsigned).
typedef vector_fixed<unsigned, 2>                  UIntVec2;

/// Typedef for exported class vector_fixed<unsigned, 3> (fixed-size vector of 3 unsigned).
typedef vector_fixed<unsigned, 3>                  UIntVec3;

/// Typedef for exported class vector_fixed<unsigned, 4> (fixed-size vector of 4 unsigned).
typedef vector_fixed<unsigned, 4>                  UIntVec4;

/// Typedef for exported class vector_fixed<unsigned, 5> (fixed-size vector of 5 unsigned).
typedef vector_fixed<unsigned, 5>                  UIntVec5;

/// Typedef for exported class vector_fixed<unsigned, 6> (fixed-size vector of 6 unsigned).
typedef vector_fixed<unsigned, 6>                  UIntVec6;

/// Typedef for exported class vector_fixed<unsigned, 7> (fixed-size vector of 7 unsigned).
typedef vector_fixed<unsigned, 7>                  UIntVec7;

/// Typedef for exported class vector_fixed<unsigned, 8> (fixed-size vector of 8 unsigned).
typedef vector_fixed<unsigned, 8>                  UIntVec8;

/// Typedef for exported class vector_fixed<unsigned, 9> (fixed-size vector of 9 unsigned).
typedef vector_fixed<unsigned, 9>                  UIntVec9;

/// Typedef for exported class vector_fixed<double, 2> (fixed-size vector of 2 doubles).
typedef vector_fixed<double, 2>                    DoubleVec2;

/// Typedef for exported class vector_fixed<double, 3> (fixed-size vector of 3 doubles).
typedef vector_fixed<double, 3>                    DoubleVec3;

/// Typedef for exported class vector_fixed<double, 4> (fixed-size vector of 4 doubles).
typedef vector_fixed<double, 4>                    DoubleVec4;

/// Typedef for exported class vector_fixed<double, 5> (fixed-size vector of 5 doubles).
typedef vector_fixed<double, 5>                    DoubleVec5;

/// Typedef for exported class vector_fixed<double, 6> (fixed-size vector of 6 doubles).
typedef vector_fixed<double, 6>                    DoubleVec6;

/// Typedef for exported class vector_fixed<double, 7> (fixed-size vector of 7 doubles).
typedef vector_fixed<double, 7>                    DoubleVec7;

/// Typedef for exported class vector_fixed<double, 8> (fixed-size vector of 8 doubles).
typedef vector_fixed<double, 8>                    DoubleVec8;

/// Typedef for exported class vector_fixed<double, 9> (fixed-size vector of 9 doubles).
typedef vector_fixed<double, 9>                    DoubleVec9;
//@}


/** \defgroup MatrixFixedInst cm2::matrix_fixed instantiations exported from the math1 DLL */
//@{
/// Typedef for exported class matrix_fixed<unsigned, 2, 2> (fixed-size 2x2 matrix of unsigned).
typedef matrix_fixed<unsigned, 2, 2>               UIntMat2x2;

/// Typedef for exported class matrix_fixed<unsigned, 3, 1> (fixed-size 3x1 matrix of unsigned).
typedef matrix_fixed<unsigned, 3, 1>               UIntMat3x1;

/// Typedef for exported class matrix_fixed<unsigned, 3, 2> (fixed-size 3x2 matrix of unsigned).
typedef matrix_fixed<unsigned, 3, 2>               UIntMat3x2;

/// Typedef for exported class matrix_fixed<unsigned, 2, 3> (fixed-size 2x3 matrix of unsigned).
typedef matrix_fixed<unsigned, 2, 3>               UIntMat2x3;

/// Typedef for exported class matrix_fixed<unsigned, 3, 3> (fixed-size 3x3 matrix of unsigned).
typedef matrix_fixed<unsigned, 3, 3>               UIntMat3x3;

/// Typedef for exported class matrix_fixed<unsigned, 3, 4> (fixed-size 3x4 matrix of unsigned).
typedef matrix_fixed<unsigned, 3, 4>               UIntMat3x4;

/// Typedef for exported class matrix_fixed<double, 2, 2> (fixed-size 2x2 matrix of doubles).
typedef matrix_fixed<double, 2, 2>                 DoubleMat2x2;

/// Typedef for exported class matrix_fixed<double, 3, 1> (fixed-size 3x1 matrix of doubles).
typedef matrix_fixed<double, 3, 1>                 DoubleMat3x1;

/// Typedef for exported class matrix_fixed<double, 3, 2> (fixed-size 3x2 matrix of doubles).
typedef matrix_fixed<double, 3, 2>                 DoubleMat3x2;

/// Typedef for exported class matrix_fixed<double, 2, 3> (fixed-size 2x3 matrix of doubles).
typedef matrix_fixed<double, 2, 3>                 DoubleMat2x3;

/// Typedef for exported class matrix_fixed<double, 3, 3> (fixed-size 3x3 matrix of doubles).
typedef matrix_fixed<double, 3, 3>                 DoubleMat3x3;

/// Typedef for exported class matrix_fixed<double, 3, 4> (fixed-size 3x4 matrix of doubles).
typedef matrix_fixed<double, 3, 4>                 DoubleMat3x4;
//@}


/** \defgroup SymmetricFixedInst cm2::symmetric_fixed instantiations exported from the math1 DLL */
//@{
/// Typedef for exported class symmetric_fixed<unsigned,2> (fixed-size symmetric 2x2 matrix of unsigned).
typedef symmetric_fixed<unsigned,2>                UIntSym2;

/// Typedef for exported class symmetric_fixed<unsigned,3> (fixed-size symmetric 3x3 matrix of unsigned).
typedef symmetric_fixed<unsigned,3>                UIntSym3;

/// Typedef for exported class symmetric_fixed<float,2> (fixed-size symmetric 2x2 matrix of floats).
typedef symmetric_fixed<float,2>                   FloatSym2;

/// Typedef for exported class symmetric_fixed<float,3> (fixed-size symmetric 3x3 matrix of floats).
typedef symmetric_fixed<float,3>                   FloatSym3;

/// Typedef for exported class symmetric_fixed<double,2> (fixed-size symmetric 2x2 matrix of doubles).
typedef symmetric_fixed<double,2>                  DoubleSym2;

/// Typedef for exported class symmetric_fixed<double,3> (fixed-size symmetric 3x3 matrix of doubles).
typedef symmetric_fixed<double,3>                  DoubleSym3;
//@}


/** \defgroup CM2VecConstants Global Constant Vectors */
//@{
/// The global static vector representing the global axis OX.
static const DoubleVec3      Ox(1., 0., 0.);

/// The global static vector representing the global axis OY.
static const DoubleVec3      Oy(0., 1., 0.);

/// The global static vector representing the global axis OZ.
static const DoubleVec3      Oz(0., 0., 1.);
//@}

}  // namespace cm2

#endif

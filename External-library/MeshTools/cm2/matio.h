/**
   \file       matio.h
   \brief      Declarations of the I/O functions for matrices (ASCII only).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATRIX_IO_H__
#define __CM2_MATRIX_IO_H__


// SYSTEM
#include <ctype.h>


/**
   \namespace  cm2::matio
   \brief      Namespace for matrix I/O functions (ASCII only).
   */

namespace cm2 { 
namespace matio {

/**@name Matrix I/O members (text mode) */
//@{  

/**
   Reads a matrix from an input stream.

   Stream must contains data formatted such as:
   \verbatim
      2x3[6 7  1
          1 10 2]
   \endverbatim

   \tparam        IStream  Input stream class.
   \tparam        Matrix   Matrix class with clear(), resize(n,m) member and access operator(i,j).
   \param[in,out] s        The input stream (template parameter).
   \param[in,out] M        The matrix.
   \return        0 (good) or -1 (IO error).

   \note    This function should be used only with dynamic matrices equipped with members
            clear(), resize() and operator \verbatim T& operator() (size_t i, size_t j) \endverbatim.
   */
template <class IStream, class Matrix>
int 
read (IStream& s, Matrix& M) 
{
   typedef typename Matrix::value_type   value_type;

   value_type        v;
   char              c (' ');
   size_t            m, n;            // Matrix dimensions

   M.clear();

   do { s.get(c); } while (s.good() && ::isspace(int(c)));
   if (!s.good()) return -1;     // error
   if (::isdigit(int(c))) s.putback(c);   
   s >> m;
   if (!s.good()) return -1;     // error

   do { s.get(c); } while (s.good() && ::isspace(int(c)));
   if (!s.good()) return -1;     // error
   if (::isdigit(int(c))) s.putback(c);   
   s >> n;
   if (!s.good()) return -1;     // error

   do { s >> c; } while (s.good() && ::isspace(int(c)));  // Eat the leading [, if it exists
   if (!s.good()) return -1;     // error
   if (c != '[') { s.putback(c); }
   
   if (!M.resize(m,n))
      return -199;               // Memory error.

   for (size_t i = 0; (i < m) && s.good(); i++) 
   {
      for (size_t j = 0; (j < n) && s.good(); j++) 
      {
         s >> v;
         M(i,j) = v;
      }
   }
   
   if (s.good()) 
   {
      do { s.get(c); } while (s.good() && ::isspace(int(c)));  // Eat the trailing ]
   }
   if (s.good() && (c != ']')) { s.putback(c); }

   return s.good() ? 0 : -1;
}


/**
   Reads a transposed matrix from an input stream.

   Stream must contain data formatted such as:
   \verbatim
      3x2[6 1
          7 10
          1 2]
   \endverbatim

   This will result in the matrix:
   \verbatim
      2x3[6 7  1
          1 10 2]
   \endverbatim

   \tparam        IStream  Input stream class.
   \tparam        M        Matrix class with clear(), resize(n,m) member and access operator(i,j).
   \param[in,out] s        The input stream (template parameter).
   \param[in,out] M        The matrix.
   \return        0 (good) or -1 (IO error).

   \note    This function should be used only with dynamic matrices equipped with members
            clear(), resize() and operator \verbatim T& operator() (size_t i, size_t j) \endverbatim.
   */
template <class IStream, class Matrix>
int 
transpose_read (IStream& s, Matrix& M) 
{
   typedef typename Matrix::value_type   value_type;

   value_type        v;
   char              c (' ');
   size_t            m, n;            // Matrix dimensions

   M.clear();

   do { s.get(c); } while (s.good() && ::isspace(int(c)));
   if (!s.good()) return -1;     // error
   if (::isdigit(int(c))) s.putback(c);   
   s >> n;
   if (!s.good()) return -1;     // error

   do { s.get(c); } while (s.good() && ::isspace(int(c)));
   if (!s.good()) return -1;     // error
   if (::isdigit(int(c))) s.putback(c);   
   s >> m;
   if (!s.good()) return -1;     // error

   do { s >> c; } while (s.good() && ::isspace(int(c)));  // Eat the leading [, if exists
   if (!s.good()) return -1;     // error
   if (c != '[') { s.putback(c); }
 
   if (!M.resize(m,n))
      return -199;               // Memory error.

   for (size_t j = 0; (j < n) && s.good(); j++) 
   {
      for (size_t i = 0; (i < m) && s.good(); i++) 
      {
         s >> v;
         M(i,j) = v;
      }
   }
  
   if (s.good()) 
   {
      do { s.get(c); } while (s.good() && ::isspace(int(c)));  // Eat the trailing ]
   }
   if (s.good() && (c!=']')) { s.putback(c); }

   return s.good() ? 0 : -1;
}


/**
   Writes a matrix to an output stream.

   The output format is like:
   \verbatim
      2x3[6 7  1
          1 10 2]
   \endverbatim

   \tparam        OStream  Output stream class.
   \tparam        Matrix   Matrix class with rows(), cols() and access operator(i,j).
   \param[in,out] s        The output stream (template parameter).
   \param[in]     M        The matrix.
   */
template <class OStream, class Matrix>
void
write (OStream& s, const Matrix& M)
{
   const size_t      m  = M.rows();
   const size_t      n  = M.cols();

   s << int(m) << " x " << int(n) << "[ ";
   for (size_t i = 0; (i < m) && s.good(); ++i) 
   {
      s << "\n     ";
      for (size_t j = 0; (j < n) && s.good(); ++j)
         s << M(i,j) << " ";
   }
   s << "]\n";
}


/**
   Writes a matrix to an output stream.

   A matrix such as:
   \verbatim
      2x3[6 7  1
          1 10 2]
   \endverbatim

   The output is transposed and will look like:
   \verbatim
      3x2[6 1
          7 10
          1 2]
   \endverbatim

   \tparam        OStream  Output stream class.
   \tparam        Matrix   Matrix class with rows(), cols() and access operator(i,j).
   \param[in,out] s        The output stream (template parameter).
   \param[in]     M        The matrix.
   */
template <class Ostream, class Matrix>
void
transpose_write (Ostream& s, const Matrix& M)
{
   const size_t      m  = M.rows();
   const size_t      n  = M.cols();

   s << int(n) << " x " << int(m) << "[ ";
   for (size_t j = 0; (j < n) && s.good(); ++j) 
   {
      s << "\n     ";
      for (size_t i = 0; (i < m) && s.good(); ++i)
         s << M(i,j) << " ";
   }
   s << "]\n";
}
//@}

} // end namespace matio
} // end namespace cm2

#endif


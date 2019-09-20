/**
   \file       vecio.h
   \brief      Declarations of the I/O functions for vetors (ASCII only).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_VECTOR_IO_H__
#define __CM2_VECTOR_IO_H__


 // SYSTEM
#include <ctype.h>


/**
   \namespace  cm2::vecio
   \brief      Namespace for vector I/O functions (ASCII only).
   */

namespace cm2 { 
namespace vecio {


/**@name Vector I/O members (text mode) */
//@{  

/**
   Reads a vector from an input stream.

   The stream must contain data between brackets such as: 
      [6 7 1 10] or 4[6 7 1 10]

   \note    This function should be used only with dynamic vectors equipped with members 
            clear(), reserve() and push_back().

   \tparam        IStream  Input stream class.
   \tparam        Vector   Dynamic vector class with clear(), reserve(n) and push_back(v) members.

   \param[in,out] s        The input stream (template class).
   \param[in,out] V        The vector (template class).
   \return                 Size of read vector (good) or -1 (IO error).
  */
template <class IStream, class Vector>
int
read (IStream& s, Vector& V) 
{
   typedef typename Vector::value_type   value_type;

   value_type        v;
   char              c(' ');
   size_t            sz(0);
  
   V.clear();

   // Skip any white space.
   do { s.get(c); } while (s.good() && ::isspace(int(c)));

   // Read size (optional).
   if (::isdigit(int(c)) && s.good()) 
   {
      s.putback(c);   
      s >> sz;
      V.reserve(sz);
      do { s.get(c); } while (s.good() && ::isspace(int(c)));
   }
   
   if (s.good() && (c != '['))
   {
      s.putback(c);   
      return -1;                    // error.
   }

   do { s.get(c); } while (s.good() && ::isspace(int(c)));

   while (s.good() && (c != ']')) 
   {
      s.putback(c);
      if (s >> v) 
         V.push_back(v);
      do {
         s.get(c);
      } while (s.good() && ::isspace(int(c)));
   }

   return s.good() ? int(V.size()) : -1;
}


/**
   Writes a vector to an output stream.

   The output format is like:
      4[6 7 1 10 ]

   \tparam        OStream  Output stream class.
   \tparam        Vector   Vector class with `begin()` and `size()` members and `const_iterator`.

   \param[in,out] s        The output stream (template class).
   \param[in]     V        The vector (template class).
   \param[in]     n        The number of elements on the same line.\n
                           Default = `UINT_MAX`.
   */
template <class Ostream, class Vector>
void
write (Ostream& s, const Vector& V, size_t n = UINT_MAX)
{
   size_t                           N     = V.size();
   typename Vector::const_iterator  p     = V.begin();

   s << int(N) << "[";
   while ((N >= n) && s.good())
   {
      N -= n;
      for (size_t k = 0; (k < n) && s.good(); ++k)
         s << *p++ << " ";
      s << "\n";
   }
   while ((N-- > 0) && s.good())
      s << *p++ << " ";
   s << "]\n";
}
//@}


} // end namespace vecio
} // end namespace cm2

#endif


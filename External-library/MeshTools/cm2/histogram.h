/**
   \file       histogram.h
   \brief      Definition of the cm2::misc::histogram class.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_HISTOGRAM_H__
#define __CM2_HISTOGRAM_H__

#include <stdio.h>


namespace cm2 {
namespace misc {

/**
   Histogram.

   An histogram is made up of B bins (`unsigned` counters), separated with B + 1 boundaries (`double`).
   For each value processed, the bins in which it belongs is incremented by one.
   */
class CM2MISC_API histogram 
{
public:

/**
   Default constructor.
   No bins, no hits.
   */
histogram();

/**
   Histogram with N bins.
   N bins are created, whose boundaries are scaled with the min and max of D. 
   Data in D are then processed in the histogram. \p D.size() hits.
   */
histogram (size_t N, const DoubleVec& D);

/**
   Histogram with boundaries given by B. 
   Constructs an histogram with \p B.size() - 1 bins.

   \note    B values are sorted internally.
   \pre     B.size() >= 2
   */
histogram (const DoubleVec& B);

/// Copy constructor (hard copy).
histogram (const histogram& x);

/// Copy operator (hard copy).
histogram&
operator= (const histogram& x);


/**
   Clears and reinitializes the histogram with N bins.
   N bins are created, whose boundaries are scaled with the min and max of D. 
   Data in D are then processed in the histogram. \p D.size() hits.
   */
void
reinit (size_t N, const DoubleVec& D);

/**
   Clears and reinitializes the histogram with boundaries \p B. 
   Constructs an histogram with \p B.size() - 1 bins.

   \note    B values are sorted internally.
   \pre     B.size() >= 2
   */
void
reinit (const DoubleVec& B);

/**
   Clears the histogram (hits and boundaries).
   */
void
clear();

/**@name Queries on bins and hits */
//@{

/// Returns the number of bins (N bins).
size_t        
bins() const                           { return _hits.size(); }

/// Returns the bins boundaries (N+1 boundaries).
const DoubleVec&      
bin_boundaries() const                 { return _bounds; }

/// Returns the lower boundary of bin \p i = upper boundary of bin \p i - 1.
double
lower_bin_bound (size_t i) const       { return _bounds[i]; }

/// Returns the hits in all bins (N values).
const UIntVec&      
all_hits() const                       { return _hits; }

/// Returns the hits in bin \p i.
unsigned
hits (size_t i) const                  { return _hits[i]; }

/// Returns the total number of hits (included larger and smaller).
unsigned        
total_hits() const;

/// Returns the hits over the largest bin.
unsigned        
larger() const                         { return _largerValues; }

/// Returns the largest value processed.
double
max_value() const                      { return _max; }

/// Returns the hits under the smallest bin.
unsigned        
smaller() const                        { return _smallerValues; }

/// Returns the smallest value processed.
double
min_value() const                      { return _min; }

/// Returns the average of all the processed values.
double
mean_value() const;

//@}


/**@name Functions that change the number of hits */
//@{

/// Resets all hits to zero (but do not change the bin bounds).
void
clear_hits();

/// Processes \p val in the histogram.
void            
process (double val);

/// Processes \p val \p N times in the histogram.
void            
process (double val, unsigned N);

/// Processes the values in vector \p vals.
void            
process (const DoubleVec& vals);
//@}


/*------------------------ Protected members -----------------------*/
private:
///
DoubleVec         _bounds;
///
UIntVec           _hits;
///
unsigned          _largerValues;
///
unsigned          _smallerValues;
///
double            _min;
///
double            _max;
///
double            _sum;

};


/// Formatted print of the histogram onto a stream.
template <class Stream>
void
print (const histogram& H, Stream& strm)
{
   char     str[256];                                       

   ::sprintf (str, "Total number of bins     : %12d \n", unsigned (H.bins()));
   strm << str;
   ::sprintf (str, "Total number of counts   : %12d \n", unsigned (H.total_hits()));  
   strm << str;
   ::sprintf (str, "Number of larger values  : %12d \n", unsigned (H.larger()));  
   strm << str;
   ::sprintf (str, "Number of smaller values : %12d \n", unsigned (H.smaller()));  
   strm << str;

   if (H.total_hits()) 
   {
      ::sprintf (str, "V max                    : %E\n", H.max_value()); 
      strm << str;
      ::sprintf (str, "V mean                   : %E\n", H.mean_value()); 
      strm << str;
      ::sprintf (str, "V min                    : %E\n", H.min_value()); 
      strm << str;
   }

   ::sprintf (str, "\nBin number       -- Bin boundaries --          Hits\n\n");
   strm << str;
   for (size_t i = H.bins(); i >= 1; --i) 
   {
      const double   lb = H.lower_bin_bound(i-1);
      const double   ub = H.lower_bin_bound(i);
      const unsigned ht = H.hits(i-1);
      if ((lb > -DBL_MAX) && (ub < DBL_MAX))
         ::sprintf (str, "%5d% 16.2f% 16.2f%14d\n", int(i-1), lb, ub, ht);
      else if (ub < DBL_MAX)
         ::sprintf (str, "%5d%s% 16.2f%14d\n", int(i-1),     "            -INF", ub, ht);
      else if (lb > -DBL_MAX)
         ::sprintf (str, "%5d% 16.2f%s%14d\n", int(i-1), lb, "            +INF", ht);
      else 
         ::sprintf (str, "%5d%s%s%14d\n", int(i-1),          "            -INF", "            +INF", ht);
      strm << str;
   }
}


}  // end namespace misc
}  // end namespace cm2

#endif

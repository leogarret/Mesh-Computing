/**
   \file       math1ops.h
   \brief      File to include all math1 template functions (cm2::vecscal, cm2::vecvec, 
               cm2::matscal, cm2::matvec and cm2::matmat).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   This file must be included to provide access to the template mathematical functions nested
   in the namespaces cm2::vecscal, cm2::vecvec, cm2::matscal, cm2::matvec and cm2::matmat.
   The mathematical types in file math1.h are designed to work with these template functions.
   Many of these template functions can however work well with other types of vector and matrix 
   as soon as they provide basic STL-type members such as begin(), end() and size().

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MATH1_OPS_H__
#define __CM2_MATH1_OPS_H__


// CONFIGURATION
#include "cm2_api_config.h"
#include "cm2_api_stl.h"

// API
#include "matvec_tags.h"
#include "vecscal.h"
#include "vecvec.h"
#include "matscal.h"
#include "matvec.h"
#include "matmat.h"
#include "vecio.h"
#include "matio.h"


#endif


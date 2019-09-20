/**
   \file       mesh_curve_chord.h
   \brief      Definition of the template meshing routine for parametric curves.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_CURVE_CHORD_H__
#define __CM2_MESH_CURVE_CHORD_H__


#include "meshtools.h"
#include "converters1d.h"
#include "meshtools1d_misc.h"
#include "mesh_transform1d.h"
#include "metrics1d.h"
#include "timer.h"

#ifdef CM2_DBG_FILES
#include <fstream>
#include <iostream>
#endif


namespace cm2 {
namespace meshtools1d {


#ifndef DOXYGEN_SKIP

/** Declarations of auxiliary routine used by meshtools1d::mesh_curv_param.  **/

/// Auxiliary routine to transform 3-D aniso metrics into iso metrics.
inline int
__metrics_3d_aniso_to_iso(const DoubleMat& M3D, const DoubleMat& local_bases, DoubleVec& H);

/// Auxiliary routine to transform a 3-D iso metric into 1-D (U) metrics.
inline int
__metrics_3d_to_1d(double h, const DoubleMat& local_bases, DoubleVec& M1D);

/// Auxiliary routine to transform 3-D (iso) metrics into 1-D (U) metrics.
inline int
__metrics_3d_to_1d(const DoubleVec& M3D, const DoubleMat& local_bases, DoubleVec& M1D);

/// Auxiliary routine used to bound metrics by curvatures metrics.
template <class Curve>
int
__chordal_control
   (const Curve& C, 
    const DoubleVec& pos1D, const UIntVec& bgm_nodes, const DoubleMat& local_bases, 
    double max_chordal_error, unsigned chordal_control_type, bool flag1D, double min_h3d,
    DoubleVec& metrics1D);

#endif // DOXYGEN_SKIP


/****************************************************************************************/
/** @name CAD meshing on parametric curves. */
//@{

/**
   Meshes a parametric 3-D curve. _Experimental_.

   This function is a template function. This enables the user to mesh her/his own parametric curves.

   Another advantage of the template function is that an experienced user can freely adapt it
   to his/her specific needs.

   \tparam     Curve     Parametric curve.
   
   The `Curve` template class should be equipped with the following members:
   \code
      int get_3D_coordinates (const DoubleVec& pos1D, DoubleMat& pos3D) const;
      int get_1D_coordinates (const DoubleMat& pos3D, DoubleVec& pos1D) const;
      int get_local_bases (const DoubleVec& pos1D, DoubleMat& local_bases) const;
      int get_local_curvature (const DoubleVec& pos1D, DoubleVec& curvatures) const;
   \endcode

   The `Curve::get_3D_coordinates` function computes the 3-D coordinates of a set of
   1-D points located along the reference curve (U). \n
   The 3-D coordinates associated to the point at index `j` of `pos1D` must be returned in column `j` of `pos3D.`\n
   This function should return zero when successful and a negative value (-1 for instance) when failed.
   
   The `Curve::get_1D_coordinates` function is the reciprocal function of the previous one.\n
   It gives the 1-D reference coordinate (U) of a set of 3-D points. \n
   The reference coordinate of the point at index `j` of `pos3D` must be returned at index `j` of `pos1D`.\n
   Note that this function is called only for the two extremity vertices if parameters \p u0 != \p u1
   or for high-order nodes when \p high_order_type > 0.\n
   This function should return zero when successful and a negative value (-1 for instance) when failed.
   
   The `Curve::get_local_bases` function computes the tangent on the curve at a set
   of points given by their reference coordinates (U). These tangents must _not_ be normalized.\n
   They are the mere derivatives of the curve with respect to the reference parameter U.\n
   The tangent at the point at index `j` of `pos1D` must be returned in column `j` of `local_bases`. \n
   This function should return zero when successful.
   
   The `Curve::get_local_curvatures` function computes the curvatures on the curve at a set
   of points given by their reference coordinates U. \n
   The curvature H is defined as:
   \verbatim
          ||D2P/Du2 x DP/Du||     ||DB/Du x B||
      H = -------------------  =  -------------
               ||DP/Du||3             ||B||3
   \endverbatim
   Norm of cross-product between the derivative of `B` (local tangent) with respect to `U`
   (i.e. second derivative of P(u) with respect to u2) and the normalized tangent `B` to the curve. \n
   This function should return zero when successful and a negative value (-1 for instance) when failed.\n
   Should you leave the implementation of this member empty (returning -1 for instance), approximative curvatures 
   computed from variations of the tangents will be used instead.
   
   It is the responsibility of the user to provide this template function with a coherent `Curve` class.\n
   That is, a class for which `get_1D_coordinates` and `get_3D_coordinates` do reciprocal computations,
   and `get_local_bases` computes the derivative of the curve with respect to the reference parameter, 
   and `get_local_curvatures` computes the `H` data as defined above.\n
   This template function does not control this coherence.

   Should the parametric curve be not regular on the domain to be meshed 
   (i.e. with some singularity or degeneracies leading to null tangent vectors) 
   this template functions will use default data instead. \n
   This may lead however to locally poor mesh quality.
                                                         
   \param[in]     C                          The parametric curve to be meshed (template parameter).
   \param[in,out] pos3D                      The 3-D coordinates of the nodes. \n
                                             New nodes created along the curve are appended.
   \param[in]     N0                         The index of the first vertex (column in \p pos3D of the starting point of the curve).
   \param[in]     N1                         The index of the last vertex (column in \p pos3D of the ending point of the curve).
   \param[in]     u0                         The U parameter of node \p N0 along the curve.
   \param[in]     u1                         The U parameter of node \p N1 along the curve.
   \param[in,out] h0                         Upon entry, the requested mesh size at node \p N0. Should be > 0. \n
                                             Upon exit, if \p update_h0h1, the effective mesh size at node \p N0 
                                             is reduced by chordal error control if any. \n
                                             Same value as upon entry if \p update_h0h1 = false.
   \param[in,out] h1                         The requested mesh size near node \p N1. Should be > 0. 
                                             Upon exit, if \p update_h0h1, the effective mesh size at node \p N1 
                                             is reduced by chordal error control if any. \n      
                                             Same value as upon entry if \p update_h0h1 = false.
   \param[in]     target_h                   The requested mesh size between \p N0 and \p N1.\n
   \param[in]     force_even                 Flag to force the number of created edges to be even.
   \param[in]     min_n                      The minimum number of edges that should be created along the curve.
   \param[in]     max_n                      The maximum number of edges that should be created along the curve.
   \param[in]     max_chordal_error          Maximum chordal error allowed:
                                                - If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                                  The distance between the elements and the curve will be bounded by \p max_chordal_error 
                                                  or the interpolated size between \p h0 and \p h1, whichever is lower.
                                                - If \p max_chordal_error < 0, the chordal error is considered as _relative_ with respect to the curvature radii. \n
                                                  The distance between the elements and the curve will be bounded by - \p max_chordal_error * curvature radii,
                                                  or the interpolated size between \p h0 and \p h1, whichever is lower.
                                             The chordal error control is disabled when \p max_chordal_error = 0 or +-DBL_MAX
                                             or when \p chordal_control_type = 0.
   \param[in]     min_h                      Minimum mesh size.\n
                                             The mesh sizes will not be reduced (by the \p max_chordal_error criterion) beyond this limit. \n
                                             This is to avoid too fine meshes in high curvature zones. \n
                                             Set this value to 0 to remove this minimum limit.
   \param[in]     chordal_control_type       Type of algorithm to compute the chordal metric:
                                                - 0: The chordal error control is disabled. \n
                                                     `Curve::get_local_curvatures` is still needed anyway.
                                                - 1: The curvatures are computed from variations of the tangents. \n
                                                     `Curve::get_local_curvatures` is still needed anyway.
                                                - 2: The curvatures are provided by `Curve::get_local_curvatures`. \n
                                                     If `Curve::get_local_curvatures` returns a negative value, the approximative curvatures 
                                                     are used as fall-back.
   \param[in]     max_gradation              The max mesh gradation. The lower, the smoother size gradations. 
                                             \p max_gradation = 0.5 usually gives good results. \n
                                             Set \p max_gradation = `DBL_MAX` to disable any gradation control.
   \param[out]    connectE                   The edges as a connectivity matrix (with 2 + \p high_order_type rows).
   \param[out]    pos1D                      The 1D-coordinates (i.e. curvilinear coordinates) of the new nodes along the curve
                                             sequentially, except for the first (\p N0), the last (\p N1) and high-order nodes (if any). \n
                                             In case of high-order nodes, their curvilinear coordinates are appended
                                             after those of the regular (linear) nodes.
   \param[in]     high_order_type            To convert into high-order elements:
                                                - 0: Keep linear elements (cm2::CM2_EDGE2).
                                                - 1: Convert to cm2::CM2_EDGE3. 
                                                - 2: Convert to cm2::CM2_EDGE4...
                                             \note    Upon exit, high-order nodes are located onto the curve.
                                             Default: 0 (linear elements).
   \param[in]     max_bgm_remeshings         [OPT] Max number of remeshing iterations to generate a background mesh. \n
                                             More iterations help when tangents and curvatures vary significantly onto the surface. \n
                                             Set \p max_bgm_meshings = 0 to get fastest meshing (but may lead to curvature miss-capture).\n
                                             Default: 4.
   \param[in]     update_h0h1                [OPT] Flag to allow reductions of \p h0 and \p h1 due to chordal error control.\n
                                             Default: true.
                                                   
   \return     Error code. Returns 0 when successful. \n
               Returned value = -k => the k-th argument had an illegal value.\n
               Returns -199 in case of insufficient memory.\n
               Returns -200 in case of other errors (`Curve::get_1D_coordinates` failure...)\n

   \pre        N0 < pos3D.cols().
   \pre        N1 < pos3D.cols().
   \pre        h0 > 0.
   \pre        h1 > 0.
   \note       The gradation between \p h0, \p target_h and \p h1 is controlled by \p max_gradation.
   \warning    Parametric singularities may cause mesh distortion.
   \see        meshtools2d::mesh_surface_param.
   */
template <class Curve>
int
mesh_curve_param
   (const Curve& C, 
    DoubleMat& pos3D, unsigned N0, unsigned N1, double u0, double u1, 
    double& h0, double& h1, double target_h,
    bool force_even, unsigned min_n, unsigned max_n, 
    double max_chordal_error, double min_h, unsigned chordal_control_type, 
    double max_gradation,
    UIntMat& connectE, DoubleVec& pos1D,
    unsigned high_order_type = 0, unsigned max_bgm_remeshings = 4,
    bool update_h0h1 = true)                          
{
   const size_t                     NP             = pos3D.cols();
   const bool                       HAS_BGM        = false;          // No user-defined BGM taken into account so far.
   const bool                       HINODES_ON_1D  = false;          // If false, the high-order nodes are generated on the 3-D mesh
                                                                     // and then projected onto the curve.
                                                                     // If true, generate high-order nodes on the 1-D mesh, 
                                                                     // then map them to the 3-D curve as the other linear nodes
                                                                     // (a bit faster but may lead to tweaked elements).
   const double                     QH_MAX         = 1.10;           // Stop remeshing when Qh max gets below this value.
   DoubleMat                        local_bases, pos3D_, tmpM;
   DoubleVec                        metrics3D, metrics1D, pos1D_, tmpV;
   UIntVec                          bgm_nodes, indices, hnodes; 
   UIntMat                          connectBGM;
   unsigned                         Ni, Nj;
   size_t                           high_order_nods(0);              // Used only when high_order_type > 0.
   size_t                           n0, n1;
   double                           min_h1d(0.), max_h1d(0.), L, qh, Qh_max;
   bool                             flip;
   int                              ret(0);                          // The return code.

   // Output of the parameters (debug only).
#ifdef CM2_DBG_FILES
   std::ofstream       dbg_strm("dbg_info.dat");
   dbg_strm.flags(std::ios::scientific);
   dbg_strm.precision(16);
   dbg_strm << "h0                    = " << h0 << std::endl;
   dbg_strm << "h1                    = " << h1 << std::endl;
   dbg_strm << "target_h              = " << target_h << std::endl;
   dbg_strm << "force_even            = " << int(force_even) << std::endl;
   dbg_strm << "min_n                 = " << min_n << std::endl;
   dbg_strm << "max_n                 = " << max_n << std::endl;
   dbg_strm << "max_chordal_error     = " << max_chordal_error << std::endl;
   dbg_strm << "chordal_control_type  = " << chordal_control_type << std::endl;
#endif

   connectE.clear();
   pos1D.clear();

   // Check some data.
   if (N0 >= NP)                       return -3;
   if (N1 >= NP)                       return -4;
   if (h0 <= 0.)                       return -7;
   if (h1 <= 0.)                       return -8;

   // Compute the reference coordinate for all nodes (call Curve::get_1D_coordinates).
   if (u0 != u1)
      pos1D.push_back(u0, u1);
   else
   {
      pos1D.resize(2, 0.0);
      matmat::push_back(pos3D, UIntVec2(N0,N1), pos3D_);
      ret = C.get_1D_coordinates(pos3D_, pos1D);
      assert ((ret < 0) || (pos1D.size() == 2));
   }

   // Reorient curve with increasing u parameter.
   flip = (pos1D[0] > pos1D[1]);
   if (flip)
   {
      std::swap(N0, N1);
      std::swap(h0, h1);
      std::swap(pos1D[0], pos1D[1]);
   }
   u0 = pos1D[0];
   u1 = pos1D[1];

   // Add null Z coordinate to the 3-D points if needed.
   if (!pos3D.resize(3, NP, 0.0))
      return -199;

   n0 = n1 = 2;
   bgm_nodes.push_back(N0, N1);
   metrics3D.push_back(h0, h1);
   pos3D_.clear();
   matmat::push_back(pos3D, bgm_nodes, pos3D_);

   // Compute the tangents for all nodes (call Curve::get_local_bases).
   // The local bases will be used to transform the 3-D metrics into 1-D metrics.
   if ((ret >= 0) && !local_bases.resize(3, n0, 0.0))
      return -199;
   ret = C.get_local_bases(pos1D, local_bases);
   assert ((ret < 0) || ((local_bases.rows() == 3) && (local_bases.cols() == n0)));

   // Output of the reference coordinates and local bases (debug only).
#ifdef CM2_DBG_FILES
   dbg_strm << "Ref coordinates.\n";
   vecio::write(dbg_strm, pos1D);
   dbg_strm << std::endl;
   dbg_strm << "Local bases.\n";
   matio::transpose_write(dbg_strm, local_bases);
   dbg_strm << std::endl;
#endif

   // Compute size bounds.
   if (ret >= 0)
   {
      max_h1d = u1 - u0;
#ifdef CM2_DBG_FILES
      dbg_strm << "min_h1d = " << min_h1d << std::endl;
      dbg_strm << "max_h1d = " << max_h1d << std::endl << std::endl;
#endif
   }

   if (target_h <= 0.) 
      target_h = ::sqrt(h0 * h1);

   if (!HAS_BGM && (ret >= 0))
   {
      // Default background mesh = 1 element.
      bgm_nodes.clear();
      bgm_nodes.push_back(0, 1);
      connectBGM.push_back(UIntVec2(0, 1));

      // Transform the 3-D metrics into 1-D metrics.
      const double   h = (target_h > 0.) ? target_h : (::sqrt(h0 * h1));
      ret = __metrics_3d_to_1d(h, local_bases, metrics1D);
   }

   // Compute a background mesh adapted to metric variations.
   for (unsigned mesh_iter = 0; !HAS_BGM && (mesh_iter <= max_bgm_remeshings) && (ret >= 0); ++mesh_iter)
   {
      // Clamp the 1-D metrics.
      ret = meshtools::bound_metrics(metrics1D, min_h1d, max_h1d);

      // Reduce some metrics to limit size gradation.
      if (ret >= 0)
      {
         DoubleMat      pos2D(2,n1,0.);
         vecvec::copy_n(pos1D.begin(), pos2D.row_begin(0), n1);
         ret = meshtools::bound_metric_gradations(pos2D, connectBGM, CM2_EDGE2,     // Requires pos.rows() > 1.
                                                  UIntVec(), true, -1, /*max_gradation=>*/ 0.5, 
                                                  metrics1D);
      }

      // (Re)generate the background mesh.
      if (ret >= 0)
      {
         indices.clear();
         ret = meshtools1d::subdivide(pos1D, metrics1D, bgm_nodes, 
                                      /*force_up=>*/ true,
                                      /*force_even=>*/ false, 
                                      /*min_n=>*/ 5,
                                      /*max_n=>*/ UINT_MAX,
                                      indices);
      }
      if (ret < 0)
         break;

      connectBGM.clear();
      meshtools1d::indices_to_connectE2(indices, connectBGM);

      // Eliminate the unused nodes (keep only connectBGM).
      {
         DoubleMat      pos1Dm(1, pos1D.size(), 1, pos1D);
         meshtools::simplify(pos1Dm, connectBGM);  
         pos1D = pos1Dm.as_vector();
      }
      bgm_nodes.clear();
      bgm_nodes.push_back(0);
      for (size_t j = 1; j < connectBGM.cols(); ++j)
         bgm_nodes.push_back(connectBGM(0,j));
      bgm_nodes.push_back(1);
      n1 = pos1D.size();

      // Compute the local bases for the new nodes (call Curve::get_local_bases).
      if (!local_bases.resize(3, n1))
         return -199;
      ret = C.get_local_bases(pos1D.sub_vector(n0,n1-n0), 
                              tmpM = local_bases.sub_matrix(0,n0,3,n1-n0));    // New nodes of the BGM.

      // Default 1-D metric on new nodes.
      if (ret >= 0)
      {
         const double   h = (target_h > 0.) ? target_h : (::sqrt(h0 * h1));
         ret = __metrics_3d_to_1d(h, local_bases, metrics1D);
      }

      // Take the curvatures into account.
      if (ret >= 0)
         ret = __chordal_control(C, pos1D, bgm_nodes, local_bases, 
                                 /*max_chordal_error=>*/ -0.01,         // 1%
                                 /*chordal_control_type=>*/ 2,          // Use exact curvatures if possible.
                                 /*flag1D=>*/ true,
                                 min_h,
                                 metrics1D);

      // Compute Qh max.
      if (ret >= 0)
      {
         Qh_max = 0.0;
         for (size_t j = 0; j < connectBGM.cols(); ++j)
         {
            Ni = connectBGM(0,j);
            Nj = connectBGM(1,j);
            L = ::fabs(pos1D[Ni] - pos1D[Nj]);
            meshtools::edge_quality(L, metrics1D[Ni], metrics1D[Nj], qh);
            Qh_max = std::max(Qh_max, qh);
         }
         if (Qh_max < QH_MAX)
            break;
      }
   }     // mesh_iter.

   if (ret < 0)
      return ret;

   metrics1D.resize(n0);
   metrics3D.resize(n0);
   if (!metrics3D.push_back_n(0., n1-n0) || !metrics1D.push_back_n(0., n1-n0))
      return -199;                                                      // Memory error.
   if (!pos3D_.resize(3, n1, 0.))
      return -199;                     // Memory error.

   if (C.get_3D_coordinates(pos1D.sub_vector(n0, n1-n0), 
                            tmpM = pos3D_.sub_matrix(0, n0, 3, n1-n0)) < 0)
      return -200;                     // Internal error.

   // If no BGM, set the 3-D metrics on the nodes of the BGM.
   if (!HAS_BGM)
   {
      // Uniform metrics3D on inside nodes.  
      if (target_h > 0.)  
         vecscal::copy_n(target_h, metrics3D.data()+n0, n1-n0);

      // Interpolate the metrics of the hard nodes onto the nodes of the background mesh (nodes N0 -> N1).
      else
         ret = meshtools1d::interpolate_metrics(pos1D.sub_vector(0,n0), metrics3D.sub_vector(0,n0),
                                                pos1D.sub_vector(n0,n1-n0), 
                                                tmpV = metrics3D.sub_vector(n0,n1-n0));
   }

   // Adapt the inner 3D metrics (up and down) to respect max_gradation (metrics3D[0] and metrics3D[1] are fixed).
   hnodes.clear();
   hnodes.push_back(0, 1);             // Fix metrics3D[0] and metrics3D[1].
   if (ret >= 0)
      ret = meshtools::bound_metric_gradations(pos3D_,
                                               connectBGM, CM2_EDGE2,
                                               hnodes, true, 0, max_gradation,
                                               metrics3D);

   // Bound the 3-D metrics (down only) with curvature metrics.
   if (ret >= 0)
      ret = __chordal_control(C, pos1D, bgm_nodes, local_bases, 
                              max_chordal_error, chordal_control_type, /*flag1D=>*/ false, min_h,
                              metrics3D);

   // Adapt the 3D metrics (down only) to respect max_gradation.
   if (ret >= 0)
   {
      if (!update_h0h1)                // Restore metrics3D[0] and metrics3D[1] to initial values.
      {
         metrics3D[0] = h0;
         metrics3D[1] = h1;
      }
      else
         hnodes.clear();

      ret = meshtools::bound_metric_gradations(pos3D_,
                                               connectBGM, CM2_EDGE2,
                                               hnodes, true, -1, max_gradation,
                                               metrics3D);
   }

   // Transform the 3-D metrics into 1-D metrics.
   if (ret >= 0)
      ret = __metrics_3d_to_1d(metrics3D, local_bases, metrics1D);

   // Clamp the 1-D metrics.
   if (ret >= 0)
      ret = meshtools::bound_metrics(metrics1D, min_h1d, max_h1d);

   // Output metrics1D after curvature bounding (debug only).
#ifdef CM2_DBG_FILES
   dbg_strm << "metrics1D1.\n";
   vecio::write(dbg_strm, metrics1D);
   dbg_strm << std::endl;
#endif

   if (ret < 0)
      return ret;

   // New metrics at N0 and N1.
   h0 = metrics3D[0];
   h1 = metrics3D[1];

   // Now, we can do the 1-D mesh.
   indices.clear();
   ret = meshtools1d::subdivide(pos1D, metrics1D, bgm_nodes, 
                                /*force_up=>*/ false,
                                force_even, min_n, max_n,
                                indices);
   meshtools1d::indices_to_connectE2(indices, connectE);

   // Eliminate the BGM (keep only connectE).
   {
      DoubleMat      pos1Dm(1, pos1D.size(), 1, pos1D);
      meshtools::simplify(pos1Dm, connectE);  
      pos1D = pos1Dm.as_vector();
   }

   // Convert to high-degree elements on the 1-D mesh, if required.
   if ((ret >= 0) && (high_order_type > 0) && HINODES_ON_1D)
   {
      DoubleMat      pos1Dm(1, pos1D.size(), 1, pos1D);
      meshtools1d::convert_E2(pos1Dm, connectE, /*Ne=>*/ high_order_type); 
      pos1D = pos1Dm.as_vector();
   }
   n1 = pos1D.size();

   // Map the new 1-D nodes onto the 3-D curve (call Curve::get_3D_coordinates).
   if (!pos3D.resize(3, NP + n1-n0, 0.0))
      return -199;
   pos1D = pos1D.sub_vector(n0, n1-n0);
   pos3D_ = pos3D.sub_matrix(0, NP, 3, n1-n0);
   if ((ret >= 0) && (C.get_3D_coordinates(pos1D, pos3D_) < 0))
      return -200;

   // Update data3D.
   {
      indices.clear();
      indices.push_back(N0, N1);
      indices.resize(n1);
      UIntVec::iterator       pi = indices.begin() + 2;
      unsigned                n  = unsigned(NP);
      for (size_t i = 2; i < n1; ++i)    // New 1-D nodes.
         *pi++ = n++; 
      meshtools::change_indices(connectE, indices);
   }

   if (flip)
   {
      meshtools1d::flip(connectE);
      std::swap(h0, h1);
   }

   // Output of the 3-D anisotropic mesh (debug only).
#ifdef CM2_DBG_FILES
   meshtools::medit_output("mesh3d.mesh", pos3D, connectE, CM2_EDGE2);
#endif

   // Optional post-treatments.
   if (ret >= 0)
   {
      // Convert to high-degree elements on the 3-D mesh, if required.
      if ((high_order_type > 0) && !HINODES_ON_1D)
      {
         n1 = pos3D.cols();
         meshtools1d::convert_E2(pos3D, connectE, /*Ne=>*/ high_order_type); 
         high_order_nods = pos3D.cols() - n1;

         // Project the new high-order nodes onto the curve.
         pos3D_ = pos3D.sub_matrix(0,n1,3,high_order_nods);       // View to the new high-order nodes.
         pos1D_.resize(pos3D_.cols());
         ret = C.get_1D_coordinates(pos3D_, pos1D_);
         if (ret >= 0) 
         {
            ret = C.get_3D_coordinates(pos1D_, pos3D_);
            pos1D.push_back(pos1D_);
         }
      }
   }

   return ret;     
}                       



/**
   Meshes a parametric 3-D curve. _Experimental_.

   Same as above but with 3D-aniso metrics \p M0 and \p M1, 
   instead of iso metrics `h0` and `h1`.
   */
template <class Curve>
int
mesh_curve_param
   (const Curve& C,
    DoubleMat& pos3D, unsigned N0, unsigned N1, double u0, double u1,
    DoubleSym3& M0, DoubleSym3& M1, double target_h,
    bool force_even, unsigned min_n, unsigned max_n,
    double max_chordal_error, double min_h, unsigned chordal_control_type,
    double max_gradation,
    UIntMat& connectE, DoubleVec& pos1D,
    unsigned high_order_type = 0, unsigned max_bgm_remeshings = 4, bool update_M0M1 = true)
{
   DoubleMat         M3D, pos3D_, local_bases(3,2);
   DoubleVec         H;
   double            h0(0.), h1(0.);
   int               ret(0);

   connectE.clear();
   pos1D.clear();

   // Checks metrics.
   if (!meshtools::is_valid_metric(M0))
      return -7;
   if (!meshtools::is_valid_metric(M1))
      return -8;

   pos1D.clear();

   // From M0 and M1, compute h0 and h1.
   if (u0 != u1)
      pos1D.push_back(u0, u1);
   else
   {
      pos1D.resize(2);
      matmat::push_back(pos3D, UIntVec2(N0,N1), pos3D_);
      ret = C.get_1D_coordinates(pos3D_, pos1D);
      assert((ret < 0) || (pos1D.size() == 2));
   }

   if (ret >= 0)
      ret = C.get_local_bases(pos1D, local_bases);

   assert((ret < 0) || ((local_bases.rows() == 3) && (local_bases.cols() == 2)));

   if (ret >= 0)
   {
      M3D.push_back(M0.as_vector());
      M3D.push_back(M1.as_vector());
      ret = __metrics_3d_aniso_to_iso(M3D, local_bases, H);
      h0 = H[0];
      h1 = H[1];
   }

   // Mesh to curve.
   if (ret >= 0)
      ret = mesh_curve_param(C, pos3D, N0, N1, u0, u1,
                             h0, h1, target_h,
                             force_even, min_n, max_n,
                             max_chordal_error, min_h, chordal_control_type,
                             max_gradation,
                             connectE, pos1D,
                             high_order_type, max_bgm_remeshings, update_M0M1);

   // Correction of M0 and M1.
   if ((ret >= 0) && (h0 < H[0]))
   {
      assert(update_M0M1);
      DoubleSym3     Mi(0.);
      DoubleVec3     Vi(local_bases.data_col(0));
      const double   v = vecscal::sqr_two_norm(Vi);
      matvec::rank1(1./((h0*h0)*v), Vi, Mi);
      if (meshtools::minimum_metric(M0, Mi, Mi))      // M0 is valid.
         M0 = Mi;
   }
   if ((ret >= 0) && (h1 < H[1]))
   {
      assert(update_M0M1);
      DoubleSym3     Mi(0.);
      DoubleVec3     Vi(local_bases.data_col(1));
      const double   v = vecscal::sqr_two_norm(Vi);
      matvec::rank1(1./((h1*h1)*v), Vi, Mi);
      if (meshtools::minimum_metric(M1, Mi, Mi))      // M1 is valid.
         M1 = Mi;
   }

   return ret;
}
//@}



#ifndef DOXYGEN_SKIP

/// Auxiliary routine to transform 3-D aniso metrics into iso metrics.
inline int
__metrics_3d_aniso_to_iso (const DoubleMat& M3D, const DoubleMat& local_bases, DoubleVec& H)
{
   const size_t               NN    = M3D.cols();
   DoubleMat::const_iterator  pl    = local_bases.begin();
   DoubleVec::const_iterator  p3d   = M3D.begin();
   DoubleVec::iterator        ph;

   assert(M3D.cols() == local_bases.cols());

   if (NN == 0)
      return 0;

   assert((M3D.rows() == 6) && (M3D.ld() == 6));
   assert((local_bases.rows() == 3) && (local_bases.ld() == 3));

   H.clear();
   if (!H.resize(NN, 0.0))
      return -199;                  // Memory error.

   ph = H.begin();
   for (size_t j = 0; j < NN; ++j, ++ph, p3d+=6, pl+=3)
   {
      const DoubleSym3     Mi(p3d);
      const DoubleVec3     Vi(pl);
      double               v = vecscal::sqr_two_norm(Vi);
      double               s = matvec::Vt_A_V(Mi, Vi);

      if (v == 0.0)
      {
         v = 1.0;
         s = (1./3.) * (Mi(0, 0) + Mi(1, 1) + Mi(2, 2));     // Take an average value.
      }
      if (s > 0.0)
         *ph = ::sqrt(v/s);
   }

   return 0;         // OK.
}

/// Auxiliary routine to transform a 3-D iso metric into 1-D (U) metrics.
inline int
__metrics_3d_to_1d (double h, const DoubleMat& local_bases, DoubleVec& M1D)
{
   const size_t               NN    = local_bases.cols();
   DoubleMat::const_iterator  pl    = local_bases.begin();
   DoubleVec::iterator        p1d;

   if (NN == 0)
      return 0;

   assert((local_bases.rows() == 3) && (local_bases.ld() == 3));

   M1D.clear();
   if (!M1D.resize(NN, 0.0))
      return -199;                  // Memory error.

   p1d = M1D.begin();
   for (size_t j = 0; j < NN; ++j, ++p1d, pl+=3)
   {
      const double s = vecscal::sqr_two_norm3(pl);
      if (s > 0.0)
         *p1d = h / ::sqrt(s);
   }

   return 0;         // OK.
}


/// Auxiliary routine to transform 3-D iso metrics into 1-D (U) metrics.
inline int
__metrics_3d_to_1d (const DoubleVec& M3D, const DoubleMat& local_bases, DoubleVec& M1D)
{
   const size_t               NN    = M3D.size();
   DoubleMat::const_iterator  pl    = local_bases.begin();
   DoubleVec::const_iterator  p3d   = M3D.begin();
   DoubleVec::iterator        p1d;

   assert(M3D.size() == local_bases.cols());

   if (NN == 0)
      return 0;

   assert((local_bases.rows() == 3) && (local_bases.ld() == 3));

   M1D.clear();
   if (!M1D.resize(NN, 0.0))
      return -199;                  // Memory error.

   p1d = M1D.begin();
   for (size_t j = 0; j < NN; ++j, ++p1d, ++p3d, pl+=3)
   {
      const double s = vecscal::sqr_two_norm3(pl);
      if (s > 0.0)
         *p1d = *p3d / ::sqrt(s);
   }

   return 0;         // OK.
}


/// Auxiliary routine to bound metrics by curvature metrics.
template <class Curve>
int
__chordal_control
   (const Curve& C, 
    const DoubleVec& pos1D, const UIntVec& bgm_nodes, const DoubleMat& local_bases, 
    double max_chordal_error, unsigned chordal_control_type, bool flag1D, 
    double min_h3d, 
    DoubleVec& metrics)
{
   const size_t               NODS  = pos1D.size();
   const double               MAX_C = (min_h3d > 0.0) ? 2./min_h3d : DBL_MAX;
   UIntMat                    connectE;
   DoubleVec                  curvatures;
   DoubleVec::iterator        p1, pc;
   DoubleMat::const_iterator  pL;
   double                     t(1.), c, eps, h;
   int                        ret(0);

   // Fast return.
   if ((chordal_control_type == 0) || (max_chordal_error == 0.0) || (max_chordal_error == DBL_MAX)
       || (max_chordal_error < -1.))
      return 0;

   assert (local_bases.cols() == NODS);
   assert (local_bases.rows() == 3);
   assert (local_bases.ld() == 3);
   assert (metrics.size() == NODS);
   
   // Compute the curvature at each node.
   if (chordal_control_type == 1)
   {
      meshtools1d::indices_to_connectE2(bgm_nodes, connectE);
      ret = meshtools1d::parametric_curvatures(pos1D, connectE, local_bases, curvatures);      // Approximative curvatures.
   }
   else
   {
      if (!curvatures.resize(NODS))
         return -199;         // Memory error.
      ret = C.get_local_curvatures(pos1D, curvatures);                                         // Exact curvatures (user-supplied).

      // If C.get_local_curvatures is bogus, use approximative curvatures.
      if (ret < 0)
      {
         meshtools1d::indices_to_connectE2(bgm_nodes, connectE);
         ret = meshtools1d::parametric_curvatures(pos1D, connectE, local_bases, curvatures); 
      }
   }
   if (ret < 0) 
      return ret;

   // Bound the metrics with the curvature metrics.
   assert (curvatures.size() == NODS);
   pc = curvatures.begin();
   pL = local_bases.begin();
   p1 = metrics.begin();
   for (size_t i = 0; i < NODS; ++i, ++pc, ++p1, pL += 3)
   {
      c = *pc;
      if ((c <= 0.0) || (c > MAX_C))
         continue;
      if (flag1D)
         t = ::sqrt(vecscal::sqr_two_norm3(pL));
      if (max_chordal_error > 0.0)              // Absolute chordal error.
         eps = max_chordal_error * c;
      else                                      // Relative chordal error (relative to curvature radius).
         eps = -max_chordal_error;
      if (eps <= 1.0)
      {
         h = (2. * ::sqrt(eps*(2.-eps))) / (t * c);
         *p1 = std::min(*p1, h);
      }
   }

   return ret;
}

#endif // DOXYGEN_SKIP

}  // end namespace meshtools1d
}  // end namespace cm2

#endif

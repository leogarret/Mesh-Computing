/**
   \file       mesh_surface_chord.h
   \brief      Definition of the template meshing routine for parametric surfaces.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3612 $
   $Date: 2019-05-02 16:21:05 +0200 (jeu., 02 mai 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESH_SURFACE_CHORD_H__
#define __CM2_MESH_SURFACE_CHORD_H__


#include "cm2_api_tools.h"
#include "meshtools.h"
#include "meshtools1d.h"
#include "converters2d.h"
#include "meshtools2d_misc.h"
#include "mesh_transform2d.h"
#include "metrics2d.h"
#include "timer.h"

#ifdef CM2_DBG_FILES
#include <fstream>
#include <iostream>
#endif


namespace cm2 {
namespace meshtools2d {


#ifndef DOXYGEN_SKIP

/** Declarations of auxiliary routine used by meshtools2d::mesh_surface_param. **/

/// Auxiliary routine to change indices in data's arrays.
template <class MeshData>
void
__change_node_indices2 (MeshData& data, const UIntVec& new_indices);

/// Auxiliary routine to compute the range of the boundary lengths.
inline void
__lengths_range (const DoubleMat& pos, const UIntMat& connectB, double& hmin, double& hmax, double& havg);

/// Auxiliary routine used to bound metrics by curvatures metrics.
template <class Surface>
int
__chordal_control
   (const Surface& S, 
    const DoubleMat& pos2D, const UIntMat& connectM, const DoubleMat& local_bases, 
    size_t N0, size_t N1,
    double max_chordal_error, unsigned chordal_control_type, double min_h3d, double max_h3d,
    DoubleMat& metrics);

/// Auxiliary routine to adapt the 3D metrics to respect max_gradation.
template <class Surface, class MeshData>
int
__bound_metric3D_gradations 
   (const Surface& S, const MeshData& data2D,
    size_t N0, size_t N1, const UIntVec& hnodes, int sgn, double max_gradation, 
    DoubleMat& pos3D, DoubleMat& metrics3D);

/// Auxiliary routine for boundary user-provided high-order nodes.
template <class MeshData>
int
__treat_midnodes (MeshData& data2D);

/// Auxiliary routine to compute 3-D surface.
template <class MeshData>
void
__compute_areas2 (MeshData& data3D);

/// Auxiliary routine for high-order nodes.
inline void
__generate_high_order_nodes 
   (DoubleMat& pos, UIntMat& connectM, unsigned nefsQ, const UIntMat& connectE, unsigned high_order_type);

/// Auxiliary routine to adapt mesher's settings to 2D meshing (triamesh version).
template <class Mesher>
void 
__set_tmp_settings2 (Mesher& mesher2D, cm2::int_to_type<CM2_FACET3>);

/// Auxiliary routine to adapt mesher's settings to 2D meshing (quadmesh version).
template <class Mesher>
void 
__set_tmp_settings2 (Mesher& mesher2D, cm2::int_to_type<CM2_FACE_MIX>);

/// Auxiliary routine used by meshtools2d::mesh_surface_param to split quads (dummy version).
template <class Mesher>
unsigned 
__bad_quads_split2 (const Mesher& mesher2D, typename Mesher::data_type& data3D, cm2::int_to_type<CM2_FACET3>);

/// Auxiliary routine used by meshtools2d::mesh_surface_param to split bad quads (quadmesh version).
template <class Mesher>
unsigned
__bad_quads_split2 (const Mesher& mesher2D, typename Mesher::data_type& data3D, cm2::int_to_type<CM2_FACE_MIX>);

#endif // DOXYGEN_SKIP


/****************************************************************************************/
/** @name CAD meshing on parametric surfaces. */
//@{

/**
   Meshes a parametric 3-D surface using a 2-D _anisotropic_ mesher and mappings.

   This function is a template function. This enables the user to mesh her/his own parametric surface.

   Another advantage of the template function is that an experienced user can freely adapt it
   to his specific needs.

   The meshes generated can be _isotropic_ on the surface (i.e. triangles tend to be equilateral and
   quadrangles tend to be square), or _anisotropic_ on the surface depending on the 3-D metrics provided 
   by the user (or if anisotropic chordal control is specified).

   A default metric field is computed to produce isotropic meshes on the surface.

   The user can also do adaptive meshes (isotropic or anisotropic) on the surface with the help
   of a "background mesh" (field `data3D.background_mesh`) and a set of metrics at each node of 
   this mesh (field `data3D.metrics`).
   The background mesh is also used by this function to compute the local bases on the surface.

   \tparam     Surface     Parametric surface.
   
   The Surface template class should be equipped with the following members:
   \code
      int get_3D_coordinates (const DoubleMat& pos2D, DoubleMat& pos3D);
      int get_2D_coordinates (const DoubleMat& pos3D, const UIntVec& nodes, DoubleMat& pos2D);
      int get_local_bases (const DoubleMat& pos2D, DoubleMat& local_bases);
      int get_local_curvatures (const DoubleMat& pos2D, DoubleMat& curvatures);
   \endcode
   
   The `Surface::get_3D_coordinates` function should compute the 3-D coordinates of a set of
   2-D points located on the reference plane (UV). \n
   The 3-D coordinates associated to the point in column `j` of `pos2D` must be returned in column `j` of `pos3D`.\n
   This function should return zero when successful and a negative value (-1 for instance) when failed.
   
   The `Surface::get_2D_coordinates` function is the reciprocal function of the previous one.\n
   It should give the 2-D reference coordinates (UV) of a set of 3-D points. \n
   The reference coordinates associated to the point at column `j` of `pos3D` must be returned in column `j` of `pos2D`.\n
   The parameter `nodes` can be used for fast retrieving of the UV coordinates when they have been precomputed.\n
   The point at column `j` of `pos3D` refers to node `nodes[j]`. \n
   Note that this function is called only for the hard nodes (nodes of the boundary mesh, the background mesh and internal hard nodes, if any)
   or for high-order nodes when \p high_order_type > 0.\n
   This function should return zero when successful and a negative value (-1 for instance) when failed.
   
   The `Surface::get_local_bases` function computes the two tangents `Bu` and `Bv` on the surface at a set
   of points given by their reference coordinates. These tangents must _not_ be normalized.\n
   They are the mere derivatives of the surface with respect to the two reference parameters.\n
   The two tangents at the point in column `j` of `pos2D` must be returned in column `j` of `local_bases`. \n
   The first three values are for the first tangent (with respect to the first ref coordinate), then
   the next three are for the second tangent. \n
   This function should return zero when successful and a negative value (-1 for instance) when failed.
   
   The `Surface::get_local_curvatures` function computes the curvatures on the surface at a set
   of points given by their reference coordinates. \n
   The curvatures `H` are 2x2 symmetric matrices defined as:
      -  H(0,0) = <DB0/Du, N>   Dot product between the derivative of `Bu` (first local tangent) with respect to `u`
                                (i.e. second derivative of `P(u,v)` with respect to `u`, `u`), and the normal `N` to the surface. 
      -  H(0,1) = <DB0/Dv, N>   Dot product between the derivative of `Bu` (first local tangent) with respect to `v`, 
                                or `Bv` (second local tangent) with respect to `u`
                                (i.e. second derivative of `P(u,v)` with respect to `u`, `v`), and the normal `N` to the surface. 
      -  H(1,1) = <DB1/Dv, N>   Dot product between the derivative of `Bv` (second local tangent) with respect to `v`
                                (i.e. second derivative of `P(u,v)` with respect to `v`, `v`), and the normal `N` to the surface. 
   These three values should be stored column-wise in matrix `curvatures`. \n
   This function should return zero when successful and a negative value (-1 for instance) when failed. \n
   Should you leave the implementation of this member empty (returning -1 for instance), approximative curvatures 
   computed from the variations of the tangents will be used instead.
   
   It is the responsibility of the user to provide this template function with a coherent `Surface` class.\n
   That is, a class for which `get_2D_coordinates` and `get_3D_coordinates` do reciprocal computations,
   and `get_local_bases` computes the derivatives of the surface with respect to the two reference parameters, 
   and `get_local_curvatures` computes the `H` matrix as defined above. \n
   This template function does not control this coherence.

   Should the parametric surface be not regular on the domain to be meshed 
   (i.e. with some singularity or degeneracies leading to null or collinear tangent vectors) 
   this template functions will use default data instead. \n
   This may lead however to locally poor mesh quality (the mesh can then be remeshed by CM2 SurfRemesh T3/Q4).

   \tparam     AnisoMesher    Anisotropic mesher.
   
   The AnisoMesher template class should be equipped with the following member:
   `void run (typename AnisoMesher::data_type& data);`

   This class should be either triamesh_aniso::mesher to generate triangle surface meshes or
   quadmesh_aniso::mesher to generate quadrangle surface meshes.
   
   The \p data3D parameter is the structure gathering all the input and output data, just like for
   any other unstructured mesher of CM2 MeshTools. \n
   The type of \p data3D depends on the type of the anisotropic mesher. 
   It is either triamesh_aniso::mesher::data_type or quadmesh_aniso::mesher::data_type.
   
   The two differences with the data for these 2-D anisotropic meshers are in the dimensions of
   the `pos` and `metrics` fields. \n
   Here, the `pos` must be a 3xNODS matrix, and the metrics must be a matrix of 3-D anisotropic metrics (dimensions 6xNODS).

   \tparam     AuxMesher   Auxiliary mesher class.

   The AuxMesher template class should be equipped with the following member:
   `void run (typename AuxMesher::data_type& data);`
   
   This class (triamesh_aniso::mesher) is used as an auxiliary mesher to produce a background mesh if none is provided in 
   data3D.background_mesh, and a convex hull mesh to interpolate the default metrics computed along the
   boundaries.

   \warning    In the case of triamesh_aniso::mesher, the use of the same instance for the main \p mesher2D
               and for \p aux_mesher is prohibited.
                                                         
   \param[in]     S                          The parametric surface to be meshed (template parameter).
   \param[in]     mesher2D                   The 2-D anisotropic mesher (triamesh_aniso::mesher or quadmesh_aniso::mesher).
   \param[in,out] data3D                     The data for the %mesher. See triamesh_aniso::mesher::data_type for instance. \n
                                             The `data3D.pos` matrix is a 3xN matrix (usually), and the `data3D.metrics` is either:
                                                -  Empty (default metrics will be used).
                                                -  1xN matrix (isotropic metrics),
                                                -  6xN matrix (3-D anisotropic metrics).
                                             As usual, the mesh connectivity is put upon exit in `data3D.connectM` 
                                             (3xNEFS or 4xNEFS if \p high_order_type = 0, 6xNEFS or 8xNEFS if \p high_order_type = 1,
                                              6xNEFS or 9xNEFS if \p high_order_type = 2). \n
                                             Upon entry (in `REGULARIZE_MODE`), `data3D.connectM` should have dimensions equal
                                             to 3xNEFS or 4xNEFS (linear elements only).
   \param[in]     aux_mesher                 Auxiliary mesher (triamesh_aniso::mesher).
   \param[in]     max_chordal_error          Maximum chordal error allowed:
                                                -  If \p max_chordal_error > 0, the chordal error is considered as _absolute_. \n
                                                   The distance between the elements and the surface will be bounded by \p max_chordal_error 
                                                   or the size specified by the user in \p data3D.metrics, whichever is lower.
                                                -  If \p max_chordal_error < 0, the chordal error is considered as _relative_ with respect to the curvature radii. \n
                                                   The distance between the elements and the surface will be bounded by - \p max_chordal_error * curvature radii,
                                                   or the size specified by the user in \p data3D.metrics, whichever is lower.
                                             The chordal error control is disabled when \p max_chordal_error = 0 or +-DBL_MAX,
                                             or when \p chordal_control_type = 0.
   \param[in]     min_h                      Minimum mesh size. \n
                                             The mesh sizes will not be reduced (by the \p max_chordal_error criterion) beyond this limit. \n
                                             This is to avoid too fine meshes in high curvature zones. \n
                                             Set this value to 0 to remove this minimum limit (not recommended).
   \param[in]     chordal_control_type       Type of algorithm to compute the chordal metric:
                                                -  0: The chordal error control is disabled. \n
                                                      `Surface::get_local_curvatures` is still needed anyway.
                                                -  1: Isotropic chordal control: element sizes are limited by the same (isotropic) chordal value.\n
                                                      The curvatures are computed from variations of the tangents 
                                                      (approximative curvatures computed with a least-square method).\n
                                                      `Surface::get_local_curvatures` is still needed anyway.
                                                -  2: Anisotropic chordal control: element sizes are limited by two different (anisotropic) chordal values.\n
                                                      The curvatures are computed from variations of the tangents 
                                                      (approximative curvatures computed with a least-square method).\n
                                                      `Surface::get_local_curvatures` is still needed anyway.
                                                -  3: Isotropic chordal control: element sizes are limited by the same (isotropic) chordal value.\n
                                                      The curvatures are provided by `Surface::get_local_curvatures`.\n
                                                      If `Surface::get_local_curvatures` returns a negative value, the approximative curvatures 
                                                      are used as fall-back.
                                                -  4: Anisotropic chordal control: element sizes are limited by two different (anisotropic) chordal values.\n
                                                      The curvatures are provided by `Surface::get_local_curvatures`.\n
                                                      If `Surface::get_local_curvatures` returns a negative value, the approximative curvatures 
                                                      are used as fall-back.
   \param[in]     high_order_type            [OPT] To convert into high-order elements:
                                                -  0: Keep linear elements (cm2::CM2_FACEQ4 and cm2::CM2_FACET3).
                                                -  1: Convert to cm2::CM2_FACEQ8 and cm2::CM2_FACET6. 
                                                -  2: Convert to cm2::CM2_FACEQ9 and cm2::CM2_FACET6.
                                             \note    Upon exit, high-order nodes are located onto the surface.
                                             \note    When high_order_type > 0, data3D.connectB can be upon entry a cm2::CM2_EDGE3 mesh (3-node edges),
                                                      with the mid-nodes located at different positions than the middle of the edges. \n
                                                      These mid-nodes will be used in the final surface mesh. \n
                                                      Missing mid-nodes will be created "along" the edges (but also projected exactly onto the surface).
                                             \note    When high_order_type > 0, the split of bad quads (settings.min_Q4_angle_quality) is disabled.
                                             Default: 0 (linear elements).
   \param[in]     max_bgm_remeshings         [OPT] Max number of remeshing iterations to generate a background mesh. \n
                                             More iterations help when tangents and curvatures vary significantly onto the surface. \n
                                             Set max_bgm_meshings = 0 to get fastest meshing (but may lead to curvature miss-capture). \n
                                             Default: 4.
   \param[in]     recompute_Qs_flag          [OPT] Flag to compute the actual shape quality of the elements on the 3-D surface. \n
                                             By default, the computed qualities are the anisotropic qualities on the reference plane 
                                             and they can be significantly different from the quality on the 3-D surface. \n
                                             Note that the qualities computed with this flag on are the _isotropic_ qualities on the 3-D surface. \n
                                             In case of an anisotropic mesh on the surface, this flag should be set to false. \n
                                             Default: true.
   \param[in]     compute_area_flag          [OPT] Flag to compute the surface area of the 3-D mesh. \n
                                             Default: true.
   \param[in]     progress_start             [OPT] Start value for the progress bar. \n
                                             Default: 0
   \param[in]     progress_range             [OPT] Range value for the progress bar. \n
                                             Default: 1
                                                   
   \return     Error code. Returns 0 when successful. \n
               Returned value = -k => the k-th argument had an illegal value. \n
               Returns -199 in case of insufficient memory. \n
               Returns -200 in case of other errors (Surface::get_2D_coordinates failure...) \n
               The error code can also be checked in the data3D.error_code, as well as the warning code
               in data3D.warning_code.

   \note       Only _isotropic_ target sizes are taken into account. \n
               The first component of `mesher2D.settings.target_metric` is used to define
               the target size (i.e. mesh size inside the surface).
   \note       There is no mesh gradation control when `mesher2D.settings.target_metric(0,0) <= 0` 
               or when `mesher2D.settings.max_gradation <= 0`.
   \note       With a mesh gradation control (`mesher2D.settings.target_metric(0,0) > 0` and 
               when `mesher2D.settings.max_gradation > 0`), the input metrics on the background mesh (if any) are discarded.
   \note       Upon exit, the \p data3D.metrics is a 3-D anisotropic metric arrays (6-row matrix) defined only
               at the hard nodes.
   \note       Edge qualities are not computed, even when `mesher2D.settings.compute_Qh_flag = true`.

   \warning    Parametric singularities may cause mesh distortion.
               In these cases, disabling mesh gradation (\p mesher2D.settings.max_gradation = 0) may help.  

   \see        triamesh_aniso::mesher, triamesh_aniso::mesher::data_type.
   \see        quadmesh_aniso::mesher, quadmesh_aniso::mesher::data_type.
   \see        meshtools1d::mesh_curve_param.
   */
template <class Surface, class AnisoMesher, class AuxMesher>
int
mesh_surface_param
   (const Surface& S, AnisoMesher& mesher2D, typename AnisoMesher::data_type& data3D, AuxMesher& aux_mesher, 
    double max_chordal_error, double min_h, unsigned chordal_control_type, 
    unsigned high_order_type = 0, unsigned max_bgm_remeshings = 4, 
    bool recompute_Qs_flag = true, bool compute_area_flag = true,
    double progress_start = 0.0, double progress_range = 1.0)                          
{
   const size_t                     INODS          = data3D.isolated_nodes.size();
   const size_t                     RPNTS          = data3D.repulsive_points.size();
   const UIntMat                    ISOL_NODES     = UIntMat(INODS, 1, INODS, data3D.isolated_nodes);          // As matrix.
   const UIntMat                    REP_POINTS     = UIntMat(RPNTS, 1, RPNTS, data3D.repulsive_points);        // As matrix.
   const UIntMat                    HRD_MATS[]     = {data3D.connectB, data3D.background_mesh, ISOL_NODES, REP_POINTS};    // Hard matrices.
   const size_t                     NP             = data3D.pos.cols();
   const size_t                     NB             = data3D.connectB.cols();     
   const size_t                     NE             = data3D.connectB.rows();        // 2 (or 3 when high_order_type > 0).
   const bool                       HAS_BGM        = !data3D.background_mesh.empty();
   const bool                       HINODES_ON_2DA = false;          // If false, the high-order nodes are generated on the 3-D mesh
                                                                     // and then projected onto the surface.
                                                                     // If true, generate high-order nodes on the 2-D aniso mesh, 
                                                                     // then map them to the 3-D surface as the other linear nodes
                                                                     // (a bit faster but may lead to tweaked elements).
   const double                     QH_MAX         = 1.50;           // Stop remeshing when Qh max gets below this value.
   const double                     TARGET_H       = mesher2D.settings.target_metric(0,0);
   const bool                       TARGET_H_OK    = (TARGET_H > 0.0);
   const double                     MIN_H2D_RATIO  = 1E-2;
   const typename AnisoMesher::settings_type  SETTINGS0   = mesher2D.settings;     // Save parameters.
   typename AnisoMesher::data_type  data2D;
   typename AuxMesher::data_type    aux_data2D;
   timer                            the_timer; 
   DoubleMat                        pos2D, pos3D;
   DoubleMat                        local_bases, LB;
   DoubleMat                        metrics3D, boundary_M3;
   UIntVec                          nodes, inv_nodes, hnodes; 
   size_t                           high_order_nods(0);              // Used only when high_order_type > 0.
   size_t                           N0, N1, N1_;
   double                           min_h2d, max_h2d, avg_h2d, min_h3d, max_h3d, avg_h3d, h;
   double                           pg_rng(0.);
   int                              ret(0);                          // The return code.

   assert (cm2::element_type(AuxMesher::generated_FE_type) == CM2_FACET3);   // The aux mesher must be a triangle mesher.

#ifdef CM2_DBG_FILES
   // Output of the parameters (debug only).
   std::ofstream       dbg_strm("dbg_info.dat");
   dbg_strm.flags(std::ios::scientific);
   dbg_strm.precision(16);
   dbg_strm << "max_chordal_error     = " << max_chordal_error << std::endl;
   dbg_strm << "chordal_control_type  = " << chordal_control_type << std::endl;
   dbg_strm << "min_h                 = " << min_h << std::endl;
   dbg_strm << "high_order_type       = " << high_order_type << std::endl;
   dbg_strm << "max_bgm_remeshings    = " << max_bgm_remeshings << std::endl;
   dbg_strm << "recompute_Qs_flag     = " << recompute_Qs_flag << std::endl;
   dbg_strm << "compute_area_flag     = " << compute_area_flag << std::endl;

   // Output of the initial 3-D boundary data (debug only).
   data3D.save ("data3d0.dat");   
#endif

   the_timer.start(); 

   // Progress bar.
   if (mesher2D.settings.interrupt_hdl)
      (*mesher2D.settings.interrupt_hdl)(mesher2D.settings.pass_thru, progress_start += pg_rng);
   pg_rng = 0.05 * progress_range;     // The next steps are supposed to account for 5%.

   // Check some data.
   if (data3D.connectB.empty())
      return 0;
   if (mesher2D.settings.basic_mode == AnisoMesher::settings_type::CONVEX_HULL_MODE)
      return -2;                                                     // CONVEX_HULL_MODE not allowed.
   if ((data3D.metrics.rows() != 0) &&                               // No metrics,
       (data3D.metrics.rows() != 1) &&                               // isotropic metrics,
       (data3D.metrics.rows() != 6))                                 // or 3-D anisotropic metrics.
      return -3;                                                     // else error.
   if ((void*)(&mesher2D) == (void*)(&aux_mesher))
      return -4;                                                     // No same mesher.
   if ((NE == 3) && (high_order_type == 0))
      return -8;                                                     // High-order edges are allowed only when high_order_type = 1 or 2.
   if (!data3D.background_mesh.empty() && (data3D.background_mesh.rows() != 3))
       return data3D.error_code = AnisoMesher::data_type::CM2_BACKGROUND_MESH_ERROR;          // Wrong BGM.

   if (meshtools::unique_indices(nodes, data3D.background_mesh) < 0) 
      return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
   if (!meshtools::are_valid_metrics(nodes, data3D.metrics))
       return data3D.error_code = AnisoMesher::data_type::CM2_BACKGROUND_MESH_ERROR;          // Wrong BGM or metrics.

   // Add null Z coordinate to the 3-D points if needed.
   if (!data3D.pos.resize(3, NP, 0.0))
      return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;

   // Get the IDs of all the 3-D nodes.
   ret = meshtools::unique_indices(nodes, HRD_MATS, HRD_MATS+4);     // The hard nodes first.
   if (ret < 0) 
      return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
   N0 = nodes.size();
   ret = meshtools::unique_indices(nodes, data3D.connectM, nodes);   // Then the soft nodes.
   if (ret < 0) 
      return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
   N1 = nodes.size();

   // Compact the matrices data3D.pos into pos3D and data3D.metrics into metrics3D.
   // Only the data for the "nodes" are pushed into pos3D and metrics3D.
   if (vecscal::max_value(nodes) > NP)
      return data3D.error_code = AnisoMesher::data_type::CM2_DATA_ERROR;
   if (!matmat::push_back(data3D.pos, nodes, pos3D))
      return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
   if (!data3D.metrics.empty())
   {
      if ((data3D.metrics.cols() < NP) && ! data3D.metrics.resize(data3D.metrics.rows(), NP, 0.0))
         return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
      if (!matmat::push_back(data3D.metrics, nodes, metrics3D))
         return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;

      if (metrics3D.rows() == 1)                                     // Isotropic metrics.
      {
         DoubleVec      default_H(metrics3D.cols());
         vecvec::copy_n(metrics3D.row_begin(0), default_H.begin(), metrics3D.cols());
         metrics3D.clear();
         if (!metrics3D.resize(6, default_H.size(), 0.0))
            return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
         meshtools::metrics_iso_to_aniso3d(default_H, metrics3D);    // Transform into 3D aniso metrics.
      }
   }

   // Change the indices in the arrays because of the previous compression.
   inv_nodes.resize(NP, CM2_NONE);
   for (size_t i = 0; i < N1; ++i) inv_nodes[nodes[i]] = unsigned(i);
   data2D.connectM.copy(data3D.connectM);                           // For the REGULARIZE_MODE.
   data2D.connectB.copy(data3D.connectB);
   data2D.isolated_nodes.copy(data3D.isolated_nodes);
   data2D.background_mesh.copy(data3D.background_mesh);
   __change_node_indices2(data2D, inv_nodes);
   if (NE == 3)
      data2D.connectB.resize(2, NB);                                // Forget for the moment the mid-nodes.

   if (!data2D.pos.resize(2, N1, 0.) || !local_bases.resize(6, N1, 0.) || !metrics3D.resize(6, N1, 0.))
      return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;

   // Compute the reference coordinates for all hard nodes (call Surface::get_2D_coordinates).
   if (S.get_2D_coordinates(pos3D, nodes, data2D.pos) < 0)
      return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
   assert ((data2D.pos.rows() == 2) && (data2D.pos.cols() == N1));

   // Compute the local bases for all hard nodes (call Surface::get_local_bases).
   // The local bases will be used to transform the 3-D metrics into 2-D metrics.
   if (S.get_local_bases(data2D.pos, local_bases) < 0)
      return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
   assert ((local_bases.rows() == 6) && (local_bases.cols() == N1));

   // Compute size bounds.
   __lengths_range(data2D.pos, data2D.connectB, min_h2d, max_h2d, avg_h2d);
   __lengths_range(data3D.pos, data3D.connectB, min_h3d, max_h3d, avg_h3d);

#ifdef CM2_DBG_FILES
   dbg_strm << "min_h2d = " << min_h2d << std::endl;
   dbg_strm << "max_h2d = " << max_h2d << std::endl;
   dbg_strm << "min_h3d = " << min_h3d << std::endl;
   dbg_strm << "max_h3d = " << max_h3d << std::endl << std::endl;

   // Output of the reference coordinates and local bases (debug only).
   dbg_strm << "Ref coordinates.\n";
   matio::transpose_write(dbg_strm, data2D.pos);
   dbg_strm << std::endl;
   dbg_strm << "Local bases.\n";
   matio::transpose_write(dbg_strm, local_bases);
   dbg_strm << std::endl;

   // Output of the initial 3-D boundary data after compression (debug only).
   data3D.save("data3d1.dat");   
   meshtools::medit_output("E2uv.mesh", data2D.pos, data2D.connectB, CM2_EDGE2);
   meshtools::medit_output("E2.mesh", data3D.pos, data3D.connectB, CM2_EDGE2);
#endif

   if (min_h <= 0.0)                         // Security on min_h.
      min_h = 1E-6 * min_h3d;

   // Progress bar.
   if (mesher2D.settings.interrupt_hdl)
      (*mesher2D.settings.interrupt_hdl)(mesher2D.settings.pass_thru, progress_start += pg_rng);
   pg_rng = 0.50 * progress_range;           // The next steps are supposed to account for 50%.

   // If missing, compute a background mesh adapted to metric gradients.
   if (!HAS_BGM)
   {
      aux_mesher.settings.reset();
      aux_mesher.settings.interrupt_hdl = mesher2D.settings.interrupt_hdl;
      aux_mesher.settings.optim_level = 2;
      aux_mesher.settings.shape_quality_weight = 0.30;

      aux_data2D.pos = data2D.pos;
      aux_data2D.connectB = data2D.connectB;
      aux_data2D.isolated_nodes.copy(data2D.isolated_nodes);

      // Setup metrics2D on the boundary nodes.
      if (!aux_data2D.metrics.resize(3, N1, 0.0))
         return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
      h = ::sqrt(avg_h3d * max_h3d);
      h = std::min(h, 4.*avg_h3d);
      {
         DoubleSym2        M2;
         for (size_t i = 0; i < N1; ++i)
         {
            meshtools::metric_iso_to_aniso2d(h, local_bases.data_col(i), M2);
            vecvec::copy3(M2.data(), aux_data2D.metrics.data_col(i));
         }
      }

      N1_ = N1;

      for (unsigned mesh_iter = 0; mesh_iter <= max_bgm_remeshings; ++mesh_iter)
      {
         const double   pg_rng_i =  0.85 * pg_rng / (max_bgm_remeshings+1);    // Each iteration accounts for this progress share.

         // Clamp the 2-D metrics.
         if (ret >= 0)
            ret = meshtools::bound_metrics(aux_data2D.metrics, MIN_H2D_RATIO * min_h2d, 0.125 * max_h2d);

         // Reduce some metrics to limit size gradation.
         if (ret >= 0)
            ret = meshtools::bound_metric_gradations(aux_data2D.pos, aux_data2D.connectM, CM2_FACET3,
                                                     UIntVec(), true, -1, /*max_gradation=>*/ 0.5, 
                                                     aux_data2D.metrics);

         // (Re)generate the background mesh (incremental).
         if (ret >= 0)
         {
            aux_mesher.settings.no_clamped_edges_flag = (mesh_iter == 0);      // Clamped edges treatment only at first iter.
            aux_mesher.settings.progress_start = progress_start;
            aux_mesher.settings.progress_range = 0.75 * pg_rng_i;              // 75% of the time / iteration in the mesher.
            aux_mesher.run(aux_data2D);                               
            ret = int(aux_data2D.error_code);
            if ((ret >= 0) && (aux_data2D.warning_code == AuxMesher::data_type::CM2_INTERRUPTION))
               ret = int(AnisoMesher::data_type::CM2_INTERRUPTION);
         }
         N1 = aux_data2D.pos.cols();
         if ((ret < 0) || (N1 == N1_))
            break;

         // Compute the local bases for the new nodes (call Surface::get_local_bases).
         if (!local_bases.resize(6, N1, 0.0))
            return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
         if (S.get_local_bases(aux_data2D.pos.sub_matrix(0, N1_, 2, N1-N1_),
                               LB = local_bases.sub_matrix(0, N1_, 6, N1-N1_)) < 0) 
            return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
         assert ((local_bases.rows() == 6) && (local_bases.cols() == N1));

         // Default 2-D metric on new nodes.
         {
            DoubleSym2        M2;
            for (size_t i = N1_; i < N1; ++i)
            {
               meshtools::metric_iso_to_aniso2d(h, local_bases.data_col(i), M2);
               vecvec::copy3(M2.data(), aux_data2D.metrics.data_col(i));
            }
         }

         // Take the curvatures into account.
         ret = __chordal_control(S, aux_data2D.pos, aux_data2D.connectM, local_bases,
                                 (mesh_iter > 0 ? N1_ : 0), N1,
                                 /*max_chordal_error=>*/ -0.01,       // 1%
                                 /*chordal_control_type=>*/ 4,        // Use exact aniso curvatures if possible.
                                 min_h, max_h3d,
                                 aux_data2D.metrics);
         if (ret < 0)
            break;

         // Progress bar.
         if (mesher2D.settings.interrupt_hdl)
            (*mesher2D.settings.interrupt_hdl)(mesher2D.settings.pass_thru, progress_start += pg_rng_i);

//meshtools::medit_output("bgm.mesh", aux_data2D.pos, aux_data2D.connectM, CM2_FACET3);
//meshtools::medit_tensor("bgm.bb", aux_data2D.metrics);

         // Compute Qh max.
         {
            misc::histogram      histo_Qh;
            meshtools::edge_qualities(aux_data2D.pos, aux_data2D.connectM, CM2_FACET3,
                                      aux_data2D.metrics, histo_Qh, aux_data2D.connectB);
            if (histo_Qh.max_value() < QH_MAX)
               break;
         }

         while (N1_ < N1)
            { aux_data2D.isolated_nodes.push_back(unsigned(N1_)); ++N1_; }
      }     // mesh_iter.

      data2D.pos = aux_data2D.pos;
      data2D.background_mesh = aux_data2D.connectM;      // The background mesh.
      data2D.metrics = aux_data2D.metrics;
   }

   if (ret < 0)
   {
      switch (ret)
      {
         case int(AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR): 
            return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
         case int(AnisoMesher::data_type::CM2_NODE_ERROR): 
            return data3D.error_code = AnisoMesher::data_type::CM2_NODE_ERROR;
         case int(AnisoMesher::data_type::CM2_EDGE_ERROR): 
            return data3D.error_code = AnisoMesher::data_type::CM2_EDGE_ERROR;
         case int(AnisoMesher::data_type::CM2_BOUNDARY_ERROR): 
            return data3D.error_code = AnisoMesher::data_type::CM2_BOUNDARY_ERROR;
         case int(AnisoMesher::data_type::CM2_INTERRUPTION): 
            return data3D.warning_code = AnisoMesher::data_type::CM2_INTERRUPTION;
         case int(AnisoMesher::data_type::CM2_LICENSE_ERROR): 
            return data3D.error_code = AnisoMesher::data_type::CM2_LICENSE_ERROR;
         default:
            return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
      }
   }

   // Progress bar.
   if (mesher2D.settings.interrupt_hdl)
      (*mesher2D.settings.interrupt_hdl)(mesher2D.settings.pass_thru, progress_start += pg_rng);
   pg_rng = 0.40 * progress_range;     // The next steps are supposed to account for 40%.

   // Metrics 3D on the boundary nodes.
   if ((chordal_control_type == 2) || (chordal_control_type == 4))
      ret = meshtools1d::metrics_gen_aniso3d(pos3D, data2D.connectB, 
                                             /*hn=>*/ 0.,                        // Tangent size along normal.
                                             /*hb=>*/ std::max(TARGET_H, 0.),    // Target size along bi-normal.
                                             boundary_M3);
   else
      ret = meshtools1d::metrics_gen_aniso3d(pos3D, data2D.connectB, 
                                             /*hn=>*/ 0.,                        // Tangent size along all directions.
                                             boundary_M3);                      
   if (ret >= 0)
      ret = meshtools::bound_metrics(boundary_M3, min_h, DBL_MAX);
   if (ret >= 0)
      ret = meshtools::overwrite_invalid_metrics(/*target=>*/ metrics3D, /*source=>*/ boundary_M3);

   // If no BGM, set the 3-D metrics on the nodes of the BGM.
   if ((ret >= 0) && !HAS_BGM)
   {
      if (!metrics3D.resize(6, N1, 0.0))
         return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;

      // Uniform metrics3D on new inside nodes (N0 -> N1) and isolated nodes.  
      if (TARGET_H_OK)                  
      {
         DoubleSym3     M3;

         meshtools::metric_iso_to_aniso(TARGET_H, M3);

         for (size_t i = N0; i < N1; ++i)
            vecvec::copy_n(M3.data(), metrics3D.data_col(i), 6);

         UIntVec::const_iterator       pi(data2D.isolated_nodes.begin());
         assert(INODS == data2D.isolated_nodes.size());
         for (size_t i = 0; i < INODS; ++i, ++pi)
            vecvec::copy_n(M3.data(), metrics3D.data_col(*pi), 6);
      }

      // Interpolate the metrics of the hard nodes onto the nodes of the background mesh.
      else
      {
         // Generate the triangulation of the hard nodes (to interpolate the metrics3D on the nodes of the BGM).
         aux_data2D.clear();
         aux_data2D.pos = data2D.pos;
         aux_data2D.connectB = data2D.connectB;
         aux_data2D.metrics = data2D.metrics;
         aux_mesher.settings.reset();
         aux_mesher.settings.refine_flag = false;
         aux_mesher.run(aux_data2D);
         ret = int(aux_data2D.error_code);
         if ((ret >= 0) && !aux_data2D.connectM.empty())
            ret = meshtools2d::interpolate_metrics(aux_data2D.pos, aux_data2D.connectM, 
                                                   aux_data2D.neighbors, aux_data2D.ancestors,
                                                   metrics3D, /*invalid_metrics_only=>*/ true, CM2_FACET3);
      }

      // Reset natural metrics on boundary nodes.
      matmat::copy(boundary_M3, LB = metrics3D.sub_matrix(0,0,6,boundary_M3.cols()));
   }

   hnodes.clear();
   meshtools::unique_indices(hnodes, data2D.connectB);

   // Adapt the 3D metrics (up and down) to respect max_gradation (metrics of boundary nodes are fixed).
   if (ret >= 0)
      ret = __bound_metric3D_gradations(S, data2D, 
                                        N0, N1, hnodes, 0, mesher2D.settings.max_gradation,
                                        pos3D, metrics3D);

   // Bound the 3-D metrics (down only) with curvature metrics.
   if (ret >= 0)
      ret = __chordal_control(S, data2D.pos, data2D.background_mesh, local_bases, 
                              0, N1,
                              max_chordal_error, chordal_control_type, min_h, max_h3d,
                              metrics3D);

   // Adapt the 3D metrics to respect max_gradation (down only, metrics of boundary nodes are fixed).
   if (ret >= 0)
      ret = __bound_metric3D_gradations(S, data2D, 
                                        N0, N1, hnodes, -1, mesher2D.settings.max_gradation,
                                        pos3D, metrics3D);

   // Transform the 3-D metrics into 2-D metrics.
   data2D.metrics.clear();
   if (!data2D.metrics.resize(3, N1, 0.))
      return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
   if (ret >= 0)
      ret = meshtools::metrics_aniso3d_to_aniso2d(metrics3D, local_bases, data2D.metrics);

   // Clamp the 2-D metrics.
   if (ret >= 0)
      ret = meshtools::bound_metrics(data2D.metrics, MIN_H2D_RATIO*min_h2d, max_h2d);

   // In case of high-order nodes along the boundary.
   if ((ret >= 0) && (NE == 3))     
      ret = __treat_midnodes(data2D);

   if (ret < 0)
   {
      switch (ret)
      {
         case int(AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR): 
            return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
         default:
            return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
      }
   }

   // Now, we can do the 2-D anisotropic mesh.
#ifdef CM2_DBG_FILES
   // Output of the background mesh (debug only).
   meshtools::medit_output("bgm.mesh", data2D.pos, data2D.background_mesh, CM2_FACET3);
   meshtools::medit_tensor("bgm.bb", data2D.metrics);
   data2D.save("data2da.dat");                  // Save data for debug purpose.
#endif
   __set_tmp_settings2(mesher2D, cm2::int_to_type<AnisoMesher::generated_FE_type>());
   mesher2D.settings.progress_start = progress_start;
   mesher2D.settings.progress_range = pg_rng;    
   mesher2D.run(data2D);
   mesher2D.settings = SETTINGS0;
   ret = data2D.error_code;

   // Progress bar.
   if (mesher2D.settings.interrupt_hdl)
      (*mesher2D.settings.interrupt_hdl)(mesher2D.settings.pass_thru, progress_start += pg_rng);
   pg_rng = 0.05 * progress_range;

   // Output of the intermediate 2-D anisotropic mesh (debug only).
#ifdef CM2_DBG_FILES
   if (ret >= 0)
   {
      meshtools::medit_output("mesh2da.mesh", data2D.pos, data2D.connectM, CM2_FACE_MIX);
      meshtools::medit_tensor("mesh2da.bb", data2D.metrics);
   }
#endif

   // Eliminate the BGM (keep only connectM).
   data2D.background_mesh.clear(); 
   meshtools::simplify(data2D.pos, data2D.connectM, unsigned(N0));            

   // Return to the initial data2D.connectB.
   if (NE == 3) 
      data2D.connectB.resize(NE, NB);        

   // Convert to high-degree elements on the 2-D aniso mesh, if required.
   if ((ret >= 0) && (high_order_type > 0) && HINODES_ON_2DA)
      __generate_high_order_nodes(data2D.pos, data2D.connectM, data2D.nefs_Q4, data2D.connectB, high_order_type);
   N1 = data2D.pos.cols();

   // Map the new 2-D nodes onto the 3-D surface (call Surface::get_3D_coordinates).
   if (!data3D.pos.reserve(3, NP + (N1-N0), /*growth_rate=>*/ 2))
      return data3D.error_code = AnisoMesher::data_type::CM2_SYSTEM_MEMORY_ERROR;
   data3D.pos.resize(3, NP + (N1-N0), 0.0);
   pos2D = data2D.pos.sub_matrix(0, N0, 2, N1-N0);
   pos3D = data3D.pos.sub_matrix(0, NP, 3, N1-N0);
   if ((ret >= 0) && (S.get_3D_coordinates(pos2D, pos3D) < 0))
      return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;

   // Update data3D.
   {
      UIntVec&          indices(inv_nodes);                          // Alias.

      indices.clear();
      indices.resize(N1);
      vecvec::copy_n(nodes.begin(), indices.begin(), N0);            // Hard nodes.
      UIntVec::iterator       pi(indices.begin() + N0);
      unsigned                n(static_cast<unsigned>(NP));
      for (size_t i = N0; i < N1; ++i)                               // New 2-D nodes.
         *pi++ = n++; 
      data2D.background_mesh.clear();
      __change_node_indices2(data2D, indices);    
      data2D.pos = data3D.pos;
      data3D.shallow_copy(data2D);
      data3D.total_nods = unsigned(data3D.pos.cols());
      metrics3D.resize(6, N0);
      data3D.metrics = metrics3D;                                    // 3-D metrics at hard nodes only.
   }

   // Post-treatments.
   if (ret >= 0)
   {
      // Split quads with angle quality below mesher2D.settings.min_Q4_angle_quality.
      if ((high_order_type == 0) || !HINODES_ON_2DA)                 // Disable splitting when high-order on 2D aniso (because too complicated...)
         __bad_quads_split2(mesher2D, data3D, cm2::int_to_type<AnisoMesher::generated_FE_type>());

      if (recompute_Qs_flag && !data3D.connectM.empty())
      {
         // Recompute shape qualities.
         data3D.histo_Qs.clear_hits();
         meshtools::shape_qualities(data3D.pos, data3D.connectM, CM2_FACE_MIX, data3D.shape_qualities);
         data3D.histo_Qs.process(data3D.shape_qualities);
         data3D.Qmin = data3D.histo_Qs.min_value();

         // Update quality warning & error.
         if (data3D.warning_code == AnisoMesher::data_type::CM2_SHAPE_QUALITY_WARNING)
         {
            data3D.warning_code = AnisoMesher::data_type::CM2_NO_WARNING;
            data3D.msg1[0] = data3D.msg2[0] = '\0';
         }
         if ((data3D.Qmin < CM2_EPSILON) && !data3D.error_code && !data3D.warning_code)
         {
            data3D.error_code = AnisoMesher::data_type::CM2_DEGENERATED_ELEMENT;
#if (_MSC_VER >= 1400) 
            ::strcpy_s(data3D.msg1, 256, "DEGENERATED ELEMENT");
#else
            ::strcpy(data3D.msg1, "DEGENERATED ELEMENT");
#endif
         }
         if ((data3D.Qmin < 0.01) && !data3D.error_code && !data3D.warning_code)
         {
            data3D.warning_code = AnisoMesher::data_type::CM2_SHAPE_QUALITY_WARNING;
#if (_MSC_VER >= 1400) 
            ::strcpy_s(data3D.msg1, 256, "SHAPE QUALITY WARNING");
#else
            ::strcpy(data3D.msg1, "SHAPE QUALITY WARNING");
#endif
         }
      }

      if (data3D.boundary_sgn == -1)
      {
         meshtools2d::flip(data3D.connectM);
         meshtools2d::flip(data3D.neighbors);
         data3D.boundary_sgn = 1;
      }

      // Recompute ancestors.
      data3D.ancestors.clear();
      data3D.ancestors.resize(data3D.pos.cols(), CM2_NONE);
      meshtools::get_ancestors(data3D.connectM, data3D.ancestors);

      // Compute the area.
      if (compute_area_flag)
         __compute_areas2(data3D);

      // Output of the 3-D anisotropic mesh (debug only).
#ifdef CM2_DBG_FILES
      meshtools::medit_output("mesh3d.mesh", data3D.pos, data3D.connectM, CM2_FACE_MIX);
#endif

      // Convert to high-degree elements on the 3-D mesh, if required.
      if ((high_order_type > 0) && !HINODES_ON_2DA)
      {
         N1 = data3D.pos.cols();
         __generate_high_order_nodes(data3D.pos, data3D.connectM, data3D.nefs_Q4, data3D.connectB, high_order_type);
         high_order_nods = data3D.pos.cols() - N1;

         // Project the new high-order nodes onto the surface.
         pos3D = data3D.pos.sub_matrix(0,N1,3,high_order_nods);           // View to the new high-order nodes.
         data2D.pos.resize(2, pos3D.cols());
         nodes.clear();
         ret = S.get_2D_coordinates(pos3D, nodes, data2D.pos);
         if (ret >= 0) 
            ret = S.get_3D_coordinates(data2D.pos, pos3D);
         if (NE == 3)
            high_order_nods += data3D.connectB.cols();
         data3D.nods += unsigned(high_order_nods);
         data3D.total_nods = unsigned(data3D.pos.cols());
         if (ret < 0)
            return data3D.error_code = AnisoMesher::data_type::CM2_INTERNAL_ERROR;
      }
   }

   // Progress bar.
   if (mesher2D.settings.interrupt_hdl)
      (*mesher2D.settings.interrupt_hdl)(mesher2D.settings.pass_thru, progress_start += pg_rng);

   data3D.total_time = the_timer.elapsed_time();
   if (data3D.total_time > 0)
      data3D.speed = data3D.nefs / data3D.total_time;
   else
      data3D.speed = DBL_MAX;

   return int(data3D.error_code);     
}                       

//@}



#ifndef DOXYGEN_SKIP

/// Local auxiliary routine used by meshtools2d::mesh_surface_param.
template <class MeshData>
void
__change_node_indices2 (MeshData& data, const UIntVec& new_indices)
{
   meshtools::change_indices(data.connectM, new_indices);
   meshtools::change_indices(data.connectB, new_indices);
   meshtools::change_indices(data.isolated_nodes, new_indices);
   meshtools::change_indices(data.background_mesh, new_indices);
}


/// Compute the range of the boundary lengths.
inline void
__lengths_range (const DoubleMat& pos, const UIntMat& connectB, double& hmin, double& hmax, double& havg)
{
   const size_t               DIM   = pos.rows();
   const size_t               NEFS  = connectB.cols();
   const size_t               LD    = connectB.ld();
   UIntMat::const_iterator    pK    = connectB.begin();
   double                     L2;

   hmin = DBL_MAX;
   hmax = havg = 0.0;

   if (NEFS == 0)
      return;

   if (DIM == 2)
   {
      DoubleVec2     minB(DBL_MAX), maxB(-DBL_MAX);
      for (size_t j = 0; j < NEFS; ++j, pK += LD)
      {  
         L2 = vecvec::diff_sqr_two_norm2(pos.data_col(*pK), pos.data_col(*(pK+1)));
         if (L2 > 0.0)
         {
            hmin = std::min(hmin, L2);
            havg += L2;
         }
      }

      meshtools::inc_bounding_box(pos, connectB, minB, maxB);
      hmax = vecvec::diff_sqr_two_norm(minB, maxB);
   }
   if (DIM == 3)
   {
      DoubleVec3     minB(DBL_MAX), maxB(-DBL_MAX);
      for (size_t j = 0; j < NEFS; ++j, pK += LD)
      {  
         L2 = vecvec::diff_sqr_two_norm3(pos.data_col(*pK), pos.data_col(*(pK+1)));
         if (L2 > 0.0)
         {
            hmin = std::min(hmin, L2);
            havg += L2;
         }
      }

      meshtools::inc_bounding_box(pos, connectB, minB, maxB);
      hmax = vecvec::diff_sqr_two_norm(minB, maxB);
   }

   hmin = ::sqrt(hmin);
   hmax = ::sqrt(hmax);
   havg = ::sqrt(havg/NEFS);
}


/// Auxiliary routine used to bound metrics by curvatures metrics.
template <class Surface>
int
__chordal_control
   (const Surface& S, 
    const DoubleMat& pos2D, const UIntMat& connectM, const DoubleMat& local_bases, 
    size_t N0, size_t N1,
    double max_chordal_error, unsigned chordal_control_type, double min_h3d, double max_h3d,
    DoubleMat& metrics)
{
   const size_t               NODS        = pos2D.cols();
   const double               MAX_C       = (min_h3d > 0.0) ? 2./min_h3d :  DBL_MAX;
   const double               LAMBDA_MIN  = 1./(max_h3d*max_h3d);
   DoubleMat                  curvatures;
   DoubleVec2                 lambdas;
   DoubleSym2                 M1, M2;
   DoubleSym3                 Mi, Mc;
   DoubleMat2x2               W;
   DoubleVec3                 V0, V1, N;
   DoubleMat3x2               T, Bp;
   DoubleMat3x2::segment_type Bp0(Bp.seg(0)), Bp1(Bp.seg(1));     // Views to columns of Bp.
   DoubleMat2x2::segment_type W0(W.seg(0)), W1(W.seg(1));         // Views to columns of W.
   double                     eps;
   int                        ret(0);

   // Fast return.
   if ((chordal_control_type == 0) || (max_chordal_error == 0.0) || (::fabs(max_chordal_error) == DBL_MAX))
      return 0;
   if (connectM.empty())
      return 0;

   assert (N1 <= NODS);
   assert (connectM.rows() == 3);
   assert ((local_bases.rows() == 6) && (local_bases.cols() == NODS));
   assert (metrics.cols() == NODS);
   assert ((metrics.rows() == 3) || (metrics.rows() == 6));

   // Compute the curvature metric at each node.
   if (chordal_control_type < 3)
      ret = meshtools2d::parametric_curvatures(pos2D, connectM, local_bases, curvatures);      // Approximative curvatures.
   else
   {
      if (!curvatures.resize(3, NODS))
         return -199;
      ret = S.get_local_curvatures(pos2D, curvatures);                                         // Exact curvatures (user-supplied).

      // If S.get_local_curvatures is bogus, use approximative curvatures.
      if (ret < 0)
         ret = meshtools2d::parametric_curvatures(pos2D, connectM, local_bases, curvatures); 

      chordal_control_type -= 2;        // 3 => 1 (iso), 4 => 2 (aniso).
   }
   if (ret < 0) 
      return ret;

   assert ((curvatures.rows() == 3) && (curvatures.cols() == NODS));

   // Output of the curvature metrics (debug only).
#ifdef CM2_DBG_FILES
   std::ofstream       dbg_strm("dbg_curv_info.dat");
   dbg_strm.flags(std::ios::scientific);
   dbg_strm.precision(16);
   dbg_strm << "Curvature metrics.\n";
   matio::transpose_write(dbg_strm, curvatures);
   dbg_strm << std::endl;
#endif

   // Bound the metrics with the curvature metrics.
   for (size_t i = N0; i < N1; ++i)
   {
      // First fundamental form of the surface.
      vecvec::copy_n(local_bases.data_col(i), T.data(), 6);
      M1 = 0.0;
      matmat::transpose_mult(T, T, M1);

      // Second fundamental form of the surface.
      vecvec::copy3(curvatures.data_col(i), M2.data());

      // Compute curvatures (lambdas) and principal directions (W).
      if (!meshtools::eigen_pairs(M1, M2, lambdas, W)) 
         continue;                                    // Infinite curvatures.

      for (unsigned d = 0; d < 2; ++d)
      {
         double&   lambda = lambdas[d];                  
         lambda = ::fabs(lambda);                        // 1/R >= 0

         if (lambda < MAX_C)
         {
            if (max_chordal_error > 0.0)                 // Absolute chordal error.
               eps = max_chordal_error * lambda;
            else                                         // Relative chordal error (relative to curvatures).
               eps = -max_chordal_error;

            if (eps <= 1.0)
               lambda = (lambda*lambda) / ((4.*eps)*(2.-eps));
            else
               lambda = LAMBDA_MIN;
         }
         else
            lambda = LAMBDA_MIN;
      }

      if (chordal_control_type == 1)                  // Isotropic curvature bound.
         lambdas[0] = lambdas[1] = std::max(lambdas[0], lambdas[1]);

      // Compute principal basis Bp (orthonormal).
      Bp = 0.0;
      matmat::mult(T, W, Bp);
      vecscal::normalize(Bp0);
      vecscal::normalize(Bp1);

      if (metrics.rows() == 3)
      {
         M2 = 0.0;
         W = 0.0;
         matmat::transpose_mult(T, Bp, W);

         // Compute the 2-D chordal metric.
         matvec::rank1(lambdas[0], W0, M2);
         matvec::rank1(lambdas[1], W1, M2);

         // Bound the 2-D metric.
         vecvec::copy3(metrics.data_col(i), M1.data());
         meshtools::minimum_metric (M1, M2, M2);
         vecvec::copy3(M2.data(), metrics.data_col(i));
      }
      else
      {
         // Compute the 3-D chordal metric.
         Mc = 0.0;
         matvec::rank1(lambdas[0], Bp0, Mc);
         matvec::rank1(lambdas[1], Bp1, Mc);

         // Bound the 3-D metric.
         vecvec::copy_n(metrics.data_col(i), Mi.data(), 6);
         meshtools::minimum_metric (Mi, Mc, Mc);
         vecvec::copy_n(Mc.data(), metrics.data_col(i), 6);
      }
   }

   return ret;
}

/// Auxiliary routine to adapt the 3D metrics to respect max_gradation.
template <class Surface, class MeshData>
int
__bound_metric3D_gradations 
   (const Surface& S, const MeshData& data2D,
    size_t N0, size_t N1, const UIntVec& hnodes, int sgn, double max_gradation, 
    DoubleMat& pos3D, DoubleMat& metrics3D)
{
   DoubleMat   tmp;
   int         ret(0);

   if ((max_gradation < 0.) || (max_gradation == DBL_MAX))
      return 0;

   if ((pos3D.cols() < N1) && !pos3D.resize(3, N1, 0.0))
      return MeshData::CM2_SYSTEM_MEMORY_ERROR;

   if (S.get_3D_coordinates(data2D.pos.sub_matrix (0, N0, 2, N1-N0), 
                            tmp = pos3D.sub_matrix (0, N0, 3, N1-N0)) < 0)
      return MeshData::CM2_INTERNAL_ERROR;

   ret = meshtools::bound_metric_gradations(pos3D,
                                            data2D.background_mesh, CM2_FACET3, 
                                            hnodes, true, sgn, max_gradation, 
                                            metrics3D);
   return (ret >= 0 ? ret : -200);
}

/// Auxiliary routine for boundary high-order nodes.
template <class MeshData>
int
__treat_midnodes (MeshData& data2D)
{
   const size_t      NB    = data2D.connectB.cols();
   unsigned          n0, n1, n2;
   DoubleSym2        M2;

   assert (data2D.connectB.ld() >= 3);
   data2D.connectB.resize(3, NB);
   for (size_t j = 0; j < NB; ++j)
   {
      n0 = data2D.connectB(0,j);
      n1 = data2D.connectB(1,j);
      n2 = data2D.connectB(2,j);
      if (n2 != CM2_NONE) 
      {
         meshtools::minimum_metric(data2D.metrics.data_col(n0), data2D.metrics.data_col(n1), M2);
         vecvec::copy3(M2.data(), data2D.metrics.data_col(n2));
         data2D.repulsive_points.push_back(n2);       // Save the mid-nodes as repulsive_points.
      }
   }
   data2D.connectB.resize(2, NB);
   return 0;
}

/// Auxiliary routine to compute 3-D surface.
template <class MeshData>
void
__compute_areas2 (MeshData& data3D)
{
   const UIntMat     connectQ4 = data3D.connectM.sub_matrix(0, 0, 4, data3D.nefs_Q4);                 
   const UIntMat     connectT3 = data3D.connectM.sub_matrix(0, data3D.nefs_Q4, 3, data3D.nefs_T3);  

   meshtools2d::area(data3D.pos, connectQ4, data3D.area_Q4);
   meshtools2d::area(data3D.pos, connectT3, data3D.area_T3);
   data3D.area = data3D.area_Q4 + data3D.area_T3;
}

/// Auxiliary routine for surface high-order nodes.
inline void
__generate_high_order_nodes 
   (DoubleMat& pos, UIntMat& connectM, unsigned nefsQ, const UIntMat& connectE, unsigned high_order_type)
{
   const unsigned    Nqi = (high_order_type == 2) ? 1 : 0;     // Number of intern quad nodes.
   UIntMat           connectQ = connectM.sub_matrix(0, 0, 4, nefsQ);
   UIntMat           connectT = connectM.sub_matrix(0, nefsQ, 3, connectM.cols()-nefsQ);

   if (connectE.rows() == 3)
      meshtools2d::convert_mixed(pos, connectQ, connectT, /*Ne=>*/ 1, Nqi, /*Nti=>*/ 0, connectE); 
   else
      meshtools2d::convert_mixed(pos, connectQ, connectT, /*Ne=>*/ 1, Nqi, /*Nti=>*/ 0); 

   connectM = connectQ;
   if (!connectQ.empty()) 
      connectT.resize(connectQ.rows(), connectT.cols(), CM2_NONE);
   matmat::push_back(connectT, connectM);
}

/// Auxiliary routine to adapt mesher's settings to 2D meshing (triamesh version).
template <class Mesher>
void 
__set_tmp_settings2 (Mesher& mesher2D, cm2::int_to_type<CM2_FACET3>) 
{ 
   mesher2D.settings.compute_Qh_flag = false;
   mesher2D.settings.max_gradation = DBL_MAX;
   mesher2D.settings.target_metric = 0.0;
}

/// Auxiliary routine to adapt mesher's settings to 2D meshing (quadmesh version).
template <class Mesher>
void 
__set_tmp_settings2 (Mesher& mesher2D, cm2::int_to_type<CM2_FACE_MIX>)
{
   mesher2D.settings.compute_Qh_flag = false;
   mesher2D.settings.min_Q4_angle_quality = 0.0;  
   mesher2D.settings.max_gradation = DBL_MAX;
   mesher2D.settings.target_metric = 0.0;
}

/// Auxiliary routine used by meshtools2d::mesh_surface_param to split quads (dummy version).
template <class Mesher>
unsigned 
__bad_quads_split2 (const Mesher&, typename Mesher::data_type&, cm2::int_to_type<CM2_FACET3>) 
{ 
   return 0;
}

/// Auxiliary routine used by meshtools2d::mesh_surface_param to split bad quads (quadmesh version).
template <class Mesher>
unsigned
__bad_quads_split2 (const Mesher& mesher2D, typename Mesher::data_type& data3D, cm2::int_to_type<CM2_FACE_MIX>)
{
   UIntVec        good_IDs, bad_IDs, colors;
   UIntMat        connectM;
   int            splits(0);
   
   if (!mesher2D.settings.all_quad_flag && (data3D.warning_code != Mesher::data_type::CM2_INTERRUPTION))
      splits = meshtools2d::split_Q4_into_T3(data3D.pos, data3D.connectM, 
                                             CM2_EPSILON, mesher2D.settings.min_Q4_angle_quality, 
                                             /*split_criterion=>*/ 6,        // Minimizes the curvature between the two triangles.
                                             good_IDs, bad_IDs, connectM);   // Keep Q4 first.

   // Recompute neighbors, colors, nefs, nefs_Q4, nefs_T3.
   if (splits > 0) 
   {
      data3D.connectM = connectM;
      meshtools::get_neighbors(data3D.connectM, CM2_FACE_MIX, 
                               /*accept_multiple_neighbors=>*/ false, data3D.neighbors);
      vecvec::push_back(data3D.colors, good_IDs, colors);
      for (size_t k = 0; k < bad_IDs.size(); ++k)
      {
         const unsigned    j = bad_IDs[k];
         const unsigned    c = data3D.colors[j];
         colors.push_back(c, c);
      }
      data3D.colors = colors;
      data3D.nefs_Q4 -= splits;
      data3D.nefs_T3 += 2*splits;
      data3D.nefs += splits;
   }

   return splits;
}

#endif // DOXYGEN_SKIP

}  // end namespace meshtools2d
}  // end namespace cm2

#endif

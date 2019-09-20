#ifndef __SURF_REMESHER_T3_H__
#define __SURF_REMESHER_T3_H__

/**
   \file       surf_remesher_t3.h
   \brief      Definition of the 3d-surface remesher T3.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3548 $
   $Date: 2019-03-20 11:20:47 +0100 (mer., 20 mars 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/


#include "fe_defs.h"
#include "math1.h"
#include "meshtools.h"
#include "misc.h"


namespace cm2 {
namespace surf_remesh_t3 {

/**
   3-D surface triangle remesher.

   This component improves the quality of a 3-D surface mesh by remeshing the surface.
   
   The algorithm works as follow:
      -  Initial mesh cleaning: it merges nodes that are too close to each other, fixes some topological errors. \n
         This step is useful when the initial mesh has degenerated elements such as in many STL meshes.
      -  Patch finding: patches are group of connected triangles for which the maximum angle between 
         two adjacent elements is lower than a prescribed value (settings_type::patch_angle_tolerance). \n
         The boundaries lines between the patches are called _skeleton lines_. \n
         Each patch can be developed to the Z=0 plane (unfolded) with an induced strain lower than a prescribed value
         (settings_type::strain_tolerance). \n
         The algorithm also makes sure that when unfolded, there is no self-intersection within each patch.  
      -  Metrics field computation: the metrics are computed on each node using the user-specified metric vector (data_type::metrics)
         and the distances between neighbouring skeleton nodes. \n
         Values are then bounded between settings_type::min_h and settings_type::max_h.
      -  Skeleton lines remeshing: generates skeleton edges so that the length of the edges fits the metric field best. \n
         This gives the remeshed skeleton lines.
      -  Patches remeshing: the patches bounded by the remeshed skeleton lines can now be remeshed. \n
         The nodes of the new mesh are projected back onto the initial mesh.
      -  Trans-patch optimization: optimizations are carried out on the new global mesh
         (edge swapping and node merging using the settings_type::optim_tolerance).

   \note       The new mesh is usually not geometrically coincident with the initial mesh. 
   \note       The nodes of the new mesh are located onto the initial mesh but the nodes of the initial mesh
               may be a distance away from the new mesh. 
   \note       An approximate measure of this chordal error between the two meshes is given upon exit by the parameter 
               data_type::max_error_distance.
   \note       The input mesh must be a _all-triangle_ mesh.
   \note       Thread safe.
   \see        surf_remesh_q4::mesher, tetramesh::mesher.

   \image html pulley_i.png  "Initial 3-D surface (STL mesh)"
   \image html pulley_rt.png "Remeshed 3-D surface"
   */
class CM2SURFREMESHT3_API mesher
{
public:

   /**
      This generator is for triangle meshes.
      */
   enum
   {
      generated_FE_type = CM2_FACET3         //!< Generates all-triangle meshes (CM2_FACET3).
   };


   /**
      The settings of surf_remesh_t3::mesher.

      This structure gathers flags and parameters to control the way the %mesher works.
      */
   struct CM2SURFREMESHT3_API settings_type
   {

   /**
      Typedef for the display handler.

      \param[in]  pass_thru   The \ref settings_type::pass_thru pointer (user provided).
      \param[in]  level       The importance of the message: 
                                 -  0 = Basic message.
                                 -  1 = Somewhat detailed message.
                                 -  2 = Very detailed message (DEBUG mode only).
      \param[in]  msg         The message.
      */
   typedef void (*display_handler_type) (void* pass_thru, unsigned level, const char* msg);

   /**
      Typedef for the interrupt handler.

      The interrupt handler, if any, is called internally by the %mesher from time to time 
      to query the abortion of process (when the handler returns true) or to continue 
      the meshing process (when the handler returns false).

      \param[in]  pass_thru   The \ref settings_type::pass_thru pointer (user provided).
      \param[in]  progress    Value (between 0 = just starting and 1 = meshing is complete) 
                              to give an estimate of the progress of the meshing.
      */
   typedef bool (*interrupt_handler_type) (void* pass_thru, double progress);


   /**
      Default constructor.

      The default values are:
         - \ref min_h = 0.0 
         - \ref max_h = `DBL_MAX`
         - \ref target_h = 0.0 
         - \ref max_gradation_1d = 0.5
         - \ref max_gradation_2d = 0.5
         - \ref max_chordal_error = -0.02
         - \ref patch_angle_tolerance = 20.0
         - \ref fix_tolerance = -0.0025
         - \ref optim_tolerance = -0.05
         - \ref strain_tolerance = 0.30
         - \ref initial_cleanup_flag = `true`
         - \ref notch_closing_flag = `true`
         - \ref closed_manifold_flag = `false`
         - \ref remesh_flag = `true`
         - \ref force_even_flag = `false`
         - \ref node_smoothing_flag = `true`
         - \ref node_inserting_flag = `true`
         - \ref node_removing_flag = `true`
         - \ref shell_remeshing_flag = `true`
         - \ref final_optimization_flag = `true`
         - \ref compute_Qh_flag = `false`
         - \ref force_deep_analyzing_flag = `false`        
         - \ref structured_pattern = -1
         - \ref nodes_limit = `UINT_MAX`
         - \ref optim_level = 3
         - \ref shape_quality_weight = 0.6
         - \ref length_upper_bound = 1.414
         - \ref min_NE_for_loops = 6
         - \ref max_NE_for_loops = `UINT_MAX`
         - \ref display_hdl = `NULL`
         - \ref interrupt_hdl = `NULL`
         - \ref pass_thru = `NULL`

      The most useful parameters are:
         - \ref min_h, \ref max_h and \ref target_h to control the size of the elements in the new mesh.
         - \ref patch_angle_tolerance to keep or smooth ridges of the initial mesh (ridges with angle lesser than
           this threshold may disappear in the new mesh).
         - \ref optim_tolerance to merge nodes and swap edges in the final mesh to improve the mesh quality.
      */
   settings_type()
      : min_h(0.0), max_h(DBL_MAX), target_h(0.), max_gradation_1d(0.5), max_gradation_2d(0.5), max_chordal_error(-0.02), 
        patch_angle_tolerance(20.), fix_tolerance(-0.0025), optim_tolerance(-0.05), strain_tolerance(0.30), 
        initial_cleanup_flag(true), notch_closing_flag(true), closed_manifold_flag(false), remesh_flag(true), force_even_flag(false), 
        node_smoothing_flag(true), node_inserting_flag(true), node_removing_flag(true), shell_remeshing_flag(true),
        final_optimization_flag(true), compute_Qh_flag(false), force_deep_analyzing_flag(false),
        structured_pattern(-1), nodes_limit(UINT_MAX), optim_level(3), shape_quality_weight(0.6), 
        length_upper_bound(1.414), min_NE_for_loops(6), max_NE_for_loops(UINT_MAX),
        display_hdl(NULL), interrupt_hdl(NULL), pass_thru(NULL)
      { }

   /**
      Returns true if the sanity check is successful.
      */
   bool
   valid() const        { return this->check() == 0; }

   /**
      Sanity check of the settings.

      \return        Returns a negative value in case of error. \n
                     Returned value = -k => the k-th field of the structure is illegal
                     (\ref min_h is the first field, \ref max_h is the second...)
      */
   int
   check() const;

   /**
      Resets to the \link cm2::surf_remesh_t3::mesher::settings_type::settings_type() default values \endlink.
      */
   void
   reset();

   /**
      Lower bound of the metric field used to generate the new mesh.

      Increase this value to increase the size of the smallest elements. \n
      Reduce this value to reduce the size of the smallest elements.  

      Default = 0. (a small value based on input mesh dimensions will be used instead).

      \pre        min_h >= 0.
      */
   double                  min_h;

   /**
      Upper bound of the metric field used to generate the new mesh.

      The element size in the new mesh will tend to this value in the middle of the skeleton lines
      (according to the \ref max_gradation_1d parameter). \n
      Increase this value to increase the size of the largest elements. \n
      Reduce this value to reduce the size of the largest elements.  

      Default = `DBL_MAX` (a large value based on input mesh dimensions will be used instead).

      \pre        max_h >= \ref min_h.
      */
   double                  max_h;

   /**
      Element size in the middle of the patches.

      The element size in the new mesh will tend to this value in the middle of the patches
      (according to the \ref max_gradation_2d parameter). \n
      This parameter can be used to reduce (or increase) the number of elements in the new mesh. 

      \note       Disabled when \ref max_gradation_2d <= 0. \n
                  In this case, the sizes are linearly interpolated between the sizes along the 
                  skeleton lines.
      \note       target_h >= 0.
      \see        \ref max_gradation_2d.

      Default = 0.
      */
   double                  target_h;

   /**
      Gradation of the element sizes along the skeleton lines.

      This parameter controls the gradation of the element sizes along the skeleton lines
      towards the \ref max_h mesh size. \n
      A value close to 0 leads to a more progressive variation (smoother) of the mesh sizes 
      along the skeleton lines. \n
      When max_gradation_1d = 0, the sizes are linearly interpolated between the sizes
      at the skeleton nodes (which depend on \ref min_h, \ref max_h and \ref max_chordal_error).

      \note       max_gradation_1d >= 0
      \see        \ref max_h.

      Default = 0.5
      */
   double                  max_gradation_1d;

   /**
      Gradation of the element sizes inside the patches.

      This parameter controls the gradation of the element sizes inside the patches
      towards the \ref target_h mesh size. \n
      A value close to 0 leads to a more progressive variation (smoother) of the mesh sizes 
      inside patches. \n
      When max_gradation_2d = 0, \ref target_h is discarded and the sizes are linearly interpolated
      between sizes along the skeleton lines.

      \note       max_gradation_2d >= 0
      \see        \ref target_h.

      Default = 0.5
      */
   double                  max_gradation_2d;

   /**
      Maximum chordal error allowed.

      The mesh size is reduced locally to limit the chordal error between the mesh and the initial surface:
         - If negative, this value is _relative_ to the local radii (for instance -0.01 => max chordal error = 1% of local radii).
         - If positive, this value is _absolute_ (for instance 0.1 => max chordal error = 0.1).

      Set to `DBL_MAX` or a value lesser than -1 to disable.

      \note       Used along ridge lines (angulous skeleton lines). \n
                  Used also to delimit patches. 
      \note       For expert users only. Should not normally be changed.

      Default = -0.02 (max chordal error = 2% of the local radii).
      */
   double                  max_chordal_error;

   /**
      Parameter used to delimit the patches (groups of connected triangles in the initial mesh).

      An angle between two adjacent triangles greater than this value (in degree) will draw a limit 
      between two patches (we call such limit a "skeleton line"). \n
      A small value tends to give numerous small patches and the final mesh will be geometrically close 
      to the initial mesh but may be of poor quality. \n
      A large value tends to give fewer patches but bigger. The remesher can do a better job on big 
      patches but the final mesh may be more distant from the initial mesh (i.e. greater geometric error).        
                        
      A value of 45.0 should be considered as the maximum for this parameter.     

      Default = 20.0

      \pre        patch_angle_tolerance >= 0.
      \pre        patch_angle_tolerance <= 180.
      */
   double                  patch_angle_tolerance;

   /**
      Controls the merging of nodes and the swapping of edges in the initial clean-up step.

      This parameter is the maximum allowable distance (absolute or relative to \ref min_h) that a node merging
      or an edge swapping can induce on the initial surface.

      If fix_tolerance > 0, fix_tolerance is taken as an _absolute_ tolerance. 

      If fix_tolerance < 0, this is a _relative_ tolerance. \n
      The distance of merging is then computed as the product of -fix_tolerance and \ref min_h,
      or if \ref min_h is null, as the product of -fix_tolerance and a mean of the lengths of the edges in the initial mesh. 

      \note       Merging nodes in the initial mesh may lead to elements flips and intersections that 
                  could remain in the final mesh. \n
                  If this happens, reduce the magnitude of fix_tolerance.
      \note       A value of 1% of \ref min_h (-0.01) should be considered as the maximum for this parameter.    
      \note       Set this parameter to 0 to avoid any node merging/edge swapping in the initial mesh
                  (or set \ref initial_cleanup_flag = false).
      \note       For expert users only. Should not normally be changed.
             
      Default = -0.0025 (0.25% of \ref min_h). 
      */
   double                  fix_tolerance;

   /**
      Controls the merging of nodes and the swapping of edges in the final optimization step.

      This parameter is the maximum allowable distance (absolute or relative to \ref min_h) that a node merging
      or an edge swapping can induce on the final surface.

      If optim_tolerance > 0, optim_tolerance is taken as an _absolute_ tolerance. 

      If optim_tolerance < 0, this is a _relative_ tolerance. \n
      The distance of merging is computed as the product of -optim_tolerance and \ref min_h,
      or if \ref min_h is null, as the product of -optim_tolerance and a mean of the lengths of the edges in the initial mesh. 

      \note       A value of 20% of \ref min_h (-0.20) should be considered as the maximum for this parameter.    
      \note       Set this parameter to 0 to avoid any node merging/edge swapping in the final mesh
                  (or set \ref final_optimization_flag = false).
             
      Default = -0.05 (5% of \ref min_h)
      */
   double                  optim_tolerance;

   /**
      The patches will be defined in such a way that when unfolded to the Z=0 plane they are not strained
      more than this parameter.
      
      Set strain_tolerance = 0, if no strain should be allowed. \n
      Only perfectly unfoldable patches will be selected (i.e. flat or simply curved patches). \n
      The strain is computed for each element as the relative difference between the shape quality before and
      after unfolding.

      A value of 0.60 should be considered as the maximum for this parameter.     
                                          
      Default = 0.30

      \pre        strain_tolerance >= 0
      */
   double                  strain_tolerance;

   /**
      Flag to allow a clean-up of the initial mesh (node merging, gaps filling, topological fixing).

      The tolerance used is based on \ref fix_tolerance.

      Default = true.
      */
   bool                    initial_cleanup_flag;

   /**
      Flag to allow the closing of notches during the clean-up phase.

      Triggers a specific algorithm within the initial clean-up step to close open nodes 
      (merged to the closest open nodes).

      \note       Disabled when \ref initial_cleanup_flag = false.

      Default = true.
      */
   bool                    notch_closing_flag;

   /**
      Flag to tell whether the 3-D surface should be considered as simple (manifold) and
      closed (watertight) or not.

      In a closed manifold 3-D surface mesh the following Euler formula holds:

         3*T - 2*E = 0

      with:
         -  T = number of triangles
         -  E = number of edges

      If true, a specific algorithm is used to correct the mesh when
      some elements are under-connected or over-connected (edges not shared exactly by two elements).

      If true, the remesher emits a warning (\ref data_type::CM2_NON_CLOSED_MANIFOLD_WARNING)
      if it cannot enforce all edges to be shared by exactly two elements.

      \warning    This flag should not be set when intern surfaces are embedded in a solid for instance,
                  or when notches must be kept open.
      \note       Disabled when \ref initial_cleanup_flag = false.

      Default = false.
      */
   bool                    closed_manifold_flag;

   /**
      Flag to enable remeshing. 

      If true, remeshes the patches.
      In this case, the flags \ref node_smoothing_flag, \ref node_inserting_flag, \ref node_removing_flag and
      \ref shell_remeshing_flag are discarded (considered as true). \n
      The coordinates of the initial nodes (all the columns of \ref data_type::pos upon entry) are not changed.

      If false, does not remesh the patches but regularizes them
      (depending on \ref node_smoothing_flag, \ref node_inserting_flag, \ref node_removing_flag and
      \ref shell_remeshing_flag). \n
      The coordinates of the initial nodes may be changed.

      \warning    Initial clean-up and final optimization may also modify the mesh. \n
                  Disables these steps with `initial_cleanup_flag = final_optimization_flag = false`
                  if needed.\n
      \note       To get only the skeleton edges and do not change anything in the mesh, set:
                  \code 
                  remesh_flag = initial_cleanup_flag = final_optimization_flag = false; 
                  optim_level = 0;
                  \endcode
      \note       To only smooth nodes on the surface mesh, set:
                  \code 
                  remesh_flag = initial_cleanup_flag = final_optimization_flag = false; 
                  node_inserting_flag = node_removing_flag = shell_remeshing_flag = false;
                  \endcode

      Default = true.
      */
   bool                    remesh_flag;

   /**
      Flag to force the number of edges along each skeleton line to be even.
      
      \note    The skeleton lines are the boundaries between adjacent patches.

      Default = false.
      */
   bool                    force_even_flag;

   /**
      Flag to allow the node-smoothing optimization scheme.
      
      \note    With \ref remesh_flag = false, node smoothing can be used to improve
               the initial surface mesh without changing the mesh connectivity (only coordinates of nodes). \n
               The new position of the nodes are still on the initial surface.
      \note    Disabled when \ref optim_level = 0.

      Default = true.
      */
   bool                    node_smoothing_flag;

   /**
      Flag to allow the node-inserting optimization scheme.
      
      \note    With \ref remesh_flag = false, node-inserting can be used to improve
               the initial surface mesh without changing the initial nodes coordinates. \n
               The position of the new nodes are on the initial surface.
      \note    Node inserting increases the number of nodes, changes the mesh 
               connectivity (around the new nodes), but doesn't change the other nodes coordinates.
      \note    Disabled when \ref optim_level = 0.

      Default = true.
      */
   bool                    node_inserting_flag;

   /**
      Flag to allow the node-removing optimization scheme.

      \note    With \ref remesh_flag = false, node-removing can be used to improve
               the initial surface mesh without changing the initial nodes coordinates. 
      \note    Node removing decreases the number of nodes, changes the mesh 
               connectivity (around the old nodes), but doesn't change the other nodes' coordinates.
      \note    Disabled when \ref optim_level = 0.

      Default = true.
      */
   bool                    node_removing_flag;

   /**
      Flag to allow the shell-remeshing optimization scheme (i.e. edge swapping).

      \note    With \ref remesh_flag = false, shell-remeshing can be used to improve
               the surface mesh with edge-swappings only (no change to the nodes). 
      \note    Shell remeshing changes the mesh connectivity,
               but doesn't change the number of nodes nor their coordinates.
      \note    Disabled when \ref optim_level = 0.

      Default = true.
      */
   bool                    shell_remeshing_flag;

   /**
      Flag to allow the final inter-patch optimization step.

      If true, nodes can be merged together and edges can be swapped to 
      improve the shape quality of the elements
      if the induced geometric error remains lower than \ref optim_tolerance.

      Default = true.
      */
   bool                    final_optimization_flag;

   /**
      Flag to compute the edge quality histogram (quality of edge lengths).

      Default = false.
      */
   bool                    compute_Qh_flag;

   /**
      Flag to force deep analyzing of the model.

      If set to false, the patch-finding algorithm starts in 'fast' mode: it assumes first
      that the flat patches have no self-intersection and check the meshability for curved patches only. \n
      In case of remeshing failure (when a flat patch has self-intersection), it falls back to the 
      deep-analysing mode (where all patches are systematically checked for non self-intersection and meshability).

      If you know that your model has flat self-intersections, you can save some CPU time by setting this
      flag to true from the beginning. \n
      
      \note    In patch-finding-only mode (\ref remesh_flag = false, \ref optim_level = 0) 
               where no remeshing is performed, the algorithm cannot fall back to deep-analyzing mode. \n
               Hence in case of flat self-intersections, correct patch-finding requires 
               \p force_deep_analyzing_flag = true.

      Default = false.
      */
   bool                    force_deep_analyzing_flag;

   /**
      Meshing mode on rectangular-like patches (unstructured or structured meshes).

      Two types of structured meshes:
      \verbatim
         - Simply-oriented pattern:

               *------*------* 
               | \    | \    |
               |   \  |   \  |
               |     \|     \|
               *------*------*
               | \    |\     |
               |   \  |  \   |
               |     \|    \ |
               *------*------*


         - "Union-Jack" pattern:        

               *------*------* 
               | \    |    / |
               |   \  |  /   |
               |     \|/     |
               *------*------*
               |     /|\     |
               |   /  |  \   |
               | /    |    \ |
               *------*------*
      \endverbatim

      Possible values are:
         -  -1   : Always use the frontal algorithm (generates unstructured meshes).
         -   0   : When possible, generate structured simply-oriented meshes.
         -   1   : Same as 0.
         -  >= 2 : When possible, generate structured UJ meshes ("Union-Jack" pattern).

      \note    Structured meshes (i.e. grid) can be generated only on rectangular-like patches 
               and when the discretizations of the four skeleton lines match (same number of 
               edges along opposite skeleton lines).

      Default = -1 (always unstructured mesh).
      */
   int                     structured_pattern;

   /**
      Maximum authorized number of nodes.

      Default = `UINT_MAX`.
      */
   unsigned                nodes_limit;

   /**
      Optimization level between 0 and 10.

      Controls the trade-off between speed (low values) and high element quality (high values). \n
      The higher the level, the more aggressive the algorithms.

      \note    The algorithms involved here aim at optimizing both the shape quality of the elements _and_ their size 
               quality (length of elements' edges). \n
               Hence, increasing the value of optim_level may lead sometimes to a reduction in the  
               shape qualities (min, average or max value) whereas the _global_ quality (which takes into 
               account both types of quality) gets better. 
      \note    The \ref shape_quality_weight parameter controls the weight of the shape quality in the global 
               quality measure.
      \note    Level 0 disables any optimization.

      Default = 3
      */
   unsigned                optim_level;

   /**
      Weight on shape qualities versus size qualities.

      Scalar between 0 and 1 indicating the importance of the shape qualities versus
      the size quality for the regularization step:
         - For values greater than 0.5, the shape quality is privileged.
         - For values lesser than 0.5, the size quality is privileged.

      Default = 0.6 (slight preference for shape quality). Should not normally be changed.

      \pre        shape_quality_weight >= 0.
      \pre        shape_quality_weight <= 1.
      */
   double                  shape_quality_weight;

   /**
      Upper bound value for the edge lengths in the final mesh.

      The mesher tends to generate meshes with no edge longer than this value (normalized length). \n
      But this is _not_ a strict enforcement. Most of the edges will be shorter than this limit, 
      but some may remain somewhat longer.

      The default value (1.414) gives the optimal meshes with respect to the size qualities. \n
      With this default value, the average edge length tends to be 1.

      Sometimes it can be useful to limit the length of the edges to a shorter value (usually between 1 and 1.414),
      and to accept a mean value smaller than 1 (sub-optimal edge qualities). 

      \note       For expert users only. Should not normally be changed.

      Default = 1.414

      \pre        length_upper_bound > 0.
      */
   double                  length_upper_bound;

   /**
      Minimum number of edges to be generated along a loop skeleton line.

      This parameter can be useful when holes or loops need to be remeshed with at least a minimum
      number of edges.

      Default = 6

      \note       Works only for surface domains, not solid domains.
      \note       You can also reduce the \ref min_h parameter to increase the number of edges along a loop.

      \pre        min_NE_for_loops >= 3
      */
   unsigned                min_NE_for_loops;

   /**
      Maximum number of edges to be generated along a loop skeleton line.

      This parameter can be useful when holes or loops need to be remeshed with a limited
      number of edges.

      Default = UINT_MAX

      \note       Works only for surface domains, not solid domains.
      \note       You can also increase the \ref min_h parameter to reduce the number of edges along a loop.

      \pre        max_NE_for_loops >= 3
      */
   unsigned                max_NE_for_loops;

   /**
      The user-provided display function for verbose.

      Level of message: 
         -  0 = Basic message.
         -  1 = Somewhat detailed message.
         -  2 = Very detailed message (debug mode only).

      Default = `NULL`.
      */
   display_handler_type    display_hdl;

   /**
      The user-provided interrupt function.

      When this function returns true, the process is stopped and the remeshing aborts.

      The parameter `progress` (between 0 and 1) gives a hint about the progress
      of the meshing.

      Default = `NULL`.
      */
   interrupt_handler_type  interrupt_hdl;

   /**
      The pointer that will be passed to the \ref display_hdl and \ref interrupt_hdl
      (if any) when called by the mesher.

      Example:
      \code
         void my_display_hdl (void* pass_thru, unsigned level, const char* msg)
         {
            window_type*   my_window = static_cast<window_type*>(pass_thru);
            my_window->show(msg);
         }

         mesher         my_mesher;
         data_type      my_mesh_data(pos, connectB);
         window_type    my_window;                    // A window instance.
         my_mesher.settings.display_hdl = &my_display_hdl;
         my_mesher.settings.pass_thru = static_cast<void*>(&my_window);   
         my_mesher.run(my_mesh_data);                 // Will call my_display_hdl.
      \endcode
      */
   void*                   pass_thru;

   };    // settings_type



   /**
      The data type for surf_remesh_t3::mesher.

      This structure is used to gather all the input and output data of the function 
      surf_remesh_t3::mesher::run.
      */
   struct CM2SURFREMESHT3_API data_type
   {
   public:

   ///
   typedef settings_type::display_handler_type     display_handler_type;

   /**
      Warning enums for the remesher.

      Non-fatal warnings.

      If such a warning is raised (returned in the field \ref warning_code),
      the remesher is not stopped but the user shall check its input and output data for a potential mistake.
      */
   enum warning_type
   { 
      CM2_NO_WARNING                =   0,   //!< OK, no warning.
      CM2_INTERRUPTION              = -10,   //!< The process has been interrupted (by the user through the interrupt handler).
      CM2_NODES_LIMIT_WARNING       = -11,   //!< The limitation on the number of nodes has been reached.
      CM2_SHAPE_QUALITY_WARNING     = -12,   //!< One or several elements have a bad quality (Qs < 0.01).
      CM2_NON_CLOSED_MANIFOLD_WARNING = -13  //!< The 3-D surface mesh could not be made simple and closed. This warning can be raised only if closed_manifold_flag = true.
   };

   /**
      Error enums for the remesher.
       
      Fatal errors.

      If such an error is raised (returned in the field \ref error_code),
      the remesher is stopped and no final mesh is returned.
      */
   enum error_type
   { 
      CM2_NO_ERROR                  =    0,  //!< OK, no error.
      CM2_LICENSE_ERROR             = -100,  //!< Invalid or expired license.
      CM2_MODE_ERROR                = -101,  //!< Error in settings.
      CM2_DATA_ERROR                = -102,  //!< Error in input data (`NaN`, `INF`, error in dimensions of arrays...)
      CM2_NODES_LIMIT_ERROR         = -103,  //!< The limitation on the number of nodes is too tight.
      CM2_DEGENERATED_ELEMENT       = -107,  //!< The process leads to some degenerated elements and therefore the remesher cannot build a valid mesh.
      CM2_REMESHING_ERROR           = -108,  //!< The surface cannot be remeshed. Tip: reduce \ref settings_type::min_h, \ref settings_type::max_h, \ref settings_type::patch_angle_tolerance and/or \ref settings_type::strain_tolerance.
      CM2_SYSTEM_MEMORY_ERROR       = -199,  //!< Insufficient memory available.
      CM2_INTERNAL_ERROR            = -200   //!< Something went wrong but don't know what (please contact support@computing-objects.com).
   };

   /**
      The array type for the metric values.

      This mesh generator is _isotropic_ and the metrics are represented as single scalars (`double` values).
      */
   typedef DoubleVec          metrics_type;



   /// Default constructor.
   data_type()                            { this->clear(); }

   /**
      Copy constructor (hard copy).
      All matrices are hard copied.
      */
   data_type (const data_type& cpy)       { (*this) = cpy; }

   /**
      Constructor.

      Constructs the data with a coordinates matrix and an initial triangle mesh connectivity matrix (shallow copy).

      This is the simplest way to create the data to exchange with the remesher.

      \param   P           The coordinates matrix. New nodes are appended by the remesher.
      \param   connectM_   The connectivity matrix of the initial mesh.

      \note    After the meshing process, get the new coordinate matrix back from the field \ref pos,
               and the new connectivity matrix from \ref connectM, or use the \ref extract function.
      */
   data_type (const DoubleMat& P, const UIntMat& connectM_)
   {
      this->clear();
      pos = P;
      connectM = connectM_;
   }


   /**@name Copy Members */
   //@{
   /// Copy operator (hard copy).
   const data_type&
   operator= (const data_type& data);
   /// Shallow copy.
   void
   shallow_copy (const data_type& data);
   //@}


   /**
      Returns true if the sanity check is successful.
      */
   bool
   valid() const        { return this->check() == 0; }

   /**
      Sanity check of the input fields.

      \return        Returns a negative value in case of error. \n
                     Returned value = -k => the k-th field of the structure is illegal
                     (\ref pos is the first field, \ref connectM is the second...)
      */
   int
   check() const;

   /// Shallow copy.
   void
   extract (DoubleMat& P, UIntMat& connectT3) const
      { P = this->pos; connectT3 = this->connectM; }

   /// Shallow copy. Provided for compatibility with CM2 SurfRemesh Q4.
   void
   extract (DoubleMat& P, UIntMat& connectQ4, UIntMat& connectT3) const
      { P = this->pos; connectQ4.clear(); connectT3 = this->connectM; }

   /// Clears all fields and reinit to default values.
   void
   clear();

   /// Clears only the output fields.
   void
   clear_data_out();

   /**
      Gets the order of elements in the initial mesh according to their color.

      Simply calls meshtools::get_sorted_order on \ref colors_in. \n
      Provided for convenience only.

      Example:
      \code
         surf_remesh_t3::mesher              my_remesher;
         surf_remesh_t3::mesher::data_type   my_data(pos, connect_in);     // Initial triangle mesh in pos and connect_in.
         UIntVec                             color_order, xColor;
         UIntVec                             IDs;
         UIntMat                             connect_7;

         // Do the remeshing.
         my_remesher.run(my_data);               

         // Get element IDs sorted by color in the initial mesh.
         my_data.IDs_sorted_by_color_in(color_order, xColor);             

         // Get the elements of the initial mesh with color 7.
         IDs = color_order.sub_vector(xColor[7], xColor[8]-xColor[7]);     // All the element IDs with color 7 in the initial mesh.
         matmat::push_back(connect_in, IDs, connect_7);                    // connect_7 = connectivity matrix of the elements with color 7 in the initial mesh.
      \endcode
      */
   void
   IDs_sorted_by_color_in (UIntVec& color_order, UIntVec& xColor) const
   {
      meshtools::get_sorted_order(this->colors_in, color_order, xColor);
   }

   /**
      Gets the order of elements in the new mesh according to their color.

      Simply calls meshtools::get_sorted_order on \ref colors. \n
      Provided for convenience only.

      Example:
      \code
         surf_remesh_t3::mesher              my_remesher;
         surf_remesh_t3::mesher::data_type   my_data(pos, connect_in);     // Initial triangle mesh in pos and connect_in.
         UIntVec                             color_order, xColor;
         UIntVec                             IDs;
         UIntMat                             connect_7;

         // Do the remeshing.
         my_remesher.run(my_data);               

         // Get element IDs sorted by color in the new mesh.
         my_data.IDs_sorted_by_color(color_order, xColor);                      

         // Get the elements of the new mesh with color 7.
         IDs = color_order.sub_vector(xColor[7], xColor[8]-xColor[7]);     // All the element IDs with color 7 in the new mesh.
         matmat::push_back(my_data.connectM, IDs, connect_7);              // connect_7 = connectivity matrix of the elements with color 7 in the new mesh.
      \endcode
      */
   void
   IDs_sorted_by_color (UIntVec& color_order, UIntVec& xColor) const
   {
      meshtools::get_sorted_order(this->colors, color_order, xColor);
   }

   /**
      Gets the order of skeleton edges in the initial mesh according to their color.

      Simply calls meshtools::get_sorted_order on \ref skeleton_colors_in. \n
      Provided for convenience only.

      Example:
      \code
         surf_remesh_t3::mesher              my_remesher;
         surf_remesh_t3::mesher::data_type   my_data(pos, connect_in);     // Initial triangle mesh in pos and connect_in.
         UIntVec                             color_order, xColor;
         UIntVec                             IDs;
         UIntMat                             connect_15;

         // Do the remeshing.
         my_remesher.run(my_data);               

         // Get edge IDs sorted by color in the initial mesh.
         my_data.skeleton_IDs_sorted_by_color_in(color_order, xColor);          

         // Get the edges of the initial mesh with color 15.
         IDs = color_order.sub_vector(xColor[15], xColor[16]-xColor[15]);  // All the edges IDs with color 15 in the initial mesh.
         matmat::push_back(my_data.skeleton_edges_in, IDs, connect_15);    // connect_15 = connectivity matrix of the edges with color 15 in the initial mesh.
      \endcode
      */
   void
   skeleton_IDs_sorted_by_color_in (UIntVec& color_order, UIntVec& xColor) const
   {
      meshtools::get_sorted_order(this->skeleton_colors_in, color_order, xColor);
   }

   /**
      Gets the order of elements in the new mesh according to their color.

      Simply calls meshtools::get_sorted_order on \ref skeleton_colors. \n
      Provided for convenience only.

      Example:
      \code
         surf_remesh_t3::mesher              my_remesher;
         surf_remesh_t3::mesher::data_type   my_data(pos, connect_in);     // Initial triangle mesh in pos and connect_in.
         UIntVec                             color_order, xColor;
         UIntVec                             IDs;
         UIntMat                             connect_15;

         // Do the remeshing.
         my_remesher.run(my_data);               

         // Get edge IDs sorted by color in the new mesh.
         my_data.skeleton_IDs_sorted_by_color(color_order, xColor);             

         // Get the edges of the new mesh with color 15.
         IDs = color_order.sub_vector(xColor[15], xColor[16]-xColor[15]);  // All the edges IDs with color 15 in the new mesh.
         matmat::push_back(my_data.skeleton_edges, IDs, connect_15);       // connect_15 = connectivity matrix of the edges with color 15 in the new mesh.
      \endcode
      */
   void
   skeleton_IDs_sorted_by_color (UIntVec& color_order, UIntVec& xColor) const
   {
      meshtools::get_sorted_order(this->skeleton_colors, color_order, xColor);
   }

   /**
      Prints information about the mesh using the display handler.

      \param   hdl   The display handler.
      */
   void
   print_info (display_handler_type hdl) const;

   /**
      Saves the data into a file.

      \param   filename       The file name for the output (overwrite mode).
      \param   precision      The precision of the output for the floating-point values.
      */
   void
   save (const char* filename, unsigned precision = 16) const;

   /**
      Load the data from a file.
      This member is the reciprocal of the save member above.

      \param   filename       The file name for the input.

      \return  Error code (negative value = error).
      */
   int
   load (const char* filename);

   /// Checks if an error has been raised.
   inline bool
   error_raised() const       { return error_code != CM2_NO_ERROR; }

   /// Checks if a warning has been raised.
   inline bool
   warning_raised() const     { return warning_code != CM2_NO_WARNING; }


   /**
      The coordinates matrix (column oriented).

      Node `i` has coordinates in column `pos.col(i)`.

      Upon entry, this matrix must contain the coordinates of all the nodes referenced in 
      \ref connectM. It may also contain other unreferenced points.

      Upon exit, the new nodes are appended at the back of this matrix.

      \note    To keep only the nodes of the final mesh, use meshtools::simplify.

      Mode = IN-OUT.
      */
   DoubleMat               pos;

   /**
      The connectivity of the mesh, input and output (column oriented). 

      The indices refer to columns in the coordinates matrix \ref pos.

      The dimensions of this matrix are 3 x \ref nefs.

      \note    To keep only the nodes of the final mesh, use meshtools::simplify.
      \warning In case of failure of the remeshing (when an error such as \ref CM2_NODES_LIMIT_ERROR, 
               \ref CM2_DEGENERATED_ELEMENT, \ref CM2_REMESHING_ERROR, \ref CM2_SYSTEM_MEMORY_ERROR or 
               \ref CM2_INTERNAL_ERROR is raised), this connectivity matrix is returned empty and the 
               initial matrix is lost.\n
               Hence, a copy of the input connectivity is recommended before the remeshing:
               \code
                  UIntMat  connectMsav;
                  connectMsav.copy(data.connectM);
               \endcode
               Note that the coordinates of the initial nodes in matrix \ref pos are never changed by
               the remesher.

      Mode = IN-OUT.
      */
   UIntMat                 connectM;

   /**
      Mesh sizes near initial nodes.

      `metrics[i]` is the target mesh size near node `i` (which coordinates are at column `i` in matrix \ref pos).

      \note    Set settings_type::max_gradation_1d = 0, if you don't want sizes to tend to 
               settings_type::target_h along skeleton lines but be linearly interpolated 
               between valid metric values.
      \note    Set settings_type::max_gradation_2d = 0, if you don't want sizes to tend to 
               settings_type::target_h inside patches but be linearly interpolated 
               between valid metric values.
      \note    Only the values for the nodes in the initial mesh are considered.
      \note    For nodes with null mesh size (or missing), an automatic mesh size is computed using 
               an average distance to their neighbours.
      \note    Upon exit, this vector contains at least the user-defined values plus computed values 
               on all other skeleton nodes. \n
               When settings_type::compute_Qh_flag = true, it is extended to all final nodes.

      Mode = IN-OUT.
      */
   DoubleVec               metrics;

   /**
      The neighbors matrix. 

      `neighbors(i,j)` is the neighbor of element `j` with edge `i`.

      The dimensions of this matrix are 3 x \ref nefs.

      \note    The neighbor is `CM2_NONE` (i.e. -1) when an element shares an edge with none or several elements. 
      \see     The User's Manual.

      Mode = OUT.
      */ 
   UIntMat                 neighbors;

   /**
      The ancestors vector. 

      `ancestors[i]` is the index of one of the elements connected to node `i`.

      These indices refer to columns in matrices \ref connectM and \ref neighbors, and to position
      in vector \ref colors.

      If a point is not connected to any element, its ancestors' index equals to `CM2_NONE` (i.e. `unsigned(-1)`).

      The size of this vector equals to `pos.cols()` upon exit (i.e. number of final points).

      Mode = OUT.
      */ 
   UIntVec                 ancestors;

   /**
      The shape quality vector of the elements in the final mesh. 

      The size of this vector is \ref nefs.

      \see  The User's Manual.

      Mode = OUT.
      */ 
   DoubleVec               shape_qualities;

   /**
      The shape quality histogram of the elements in the initial mesh. 

      Mode = OUT.
      */ 
   cm2::misc::histogram    histo_Qs_in; 

   /**
      The shape quality histogram of the elements in the final mesh. 

      Mode = OUT.
      */ 
   cm2::misc::histogram    histo_Qs; 

   /**
      The edge quality histogram (edge lengths) in the final mesh.

      Computed only when settings_type::compute_Qh_flag = true.

      \warning    This implies a computation of \ref metrics on all final nodes 
                  (and not only on the skeleton nodes): higher work load and
                  higher memory usage.

      Mode = OUT.
      */ 
   cm2::misc::histogram    histo_Qh;

   /**
      The face IDs in \ref connectM that should not be changed.

      These faces are "frozen". Their edges are considered as hard edges. \n
      Their nodes are considered as skeleton nodes.

      Mode = IN
      */
   UIntVec                 hard_faces;

   /**
      Upon entry, the user can add into this matrix some specific edges 
      to constrain them to be selected as boundaries of the patches (so-called _skeleton edges_).

      Upon exit, this matrix contains all the boundaries of the patches in the initial mesh. \n
      They are the "stitches" found in the initial mesh by the algorithm (or constrained by user).

      \note    Upon entry, only edges of the _initial_ mesh are taken into account.
      \note    Simply-connected skeleton edges make a so-called _skeleton line_.
      \note    The skeleton lines may be remeshed: the initial nodes along these lines (except the two
               summits) are removed and new ones are generated. 
               They are not considered as "hard edges". \n
               To prevent the removal of the old nodes along a skeleton line, 
               one solution is to put their node IDs in the \ref hard_nodes vector. \n
               This does not prevent however new nodes from being added between them. \n
               To go further and treat skeleton edges as "hard edges" use \ref hard_edges instead
               (old nodes are preserved and no new node are added between them).
      \note    User-specified skeleton lines upon entry may be remeshed but
               they are guaranteed however to remain in the final mesh, 
               i.e. they will not be crossed by new element boundaries.
               
      Mode = IN-OUT
      */
   UIntMat                 skeleton_edges_in;

   /**
      Upon exit, this matrix contains the boundaries of the patches in the final mesh
      (i.e. the remeshed \ref skeleton_edges_in). 
             
      Mode = OUT
      */
   UIntMat                 skeleton_edges;

   /**
      This matrix is can be used to exclude some edges from being selected by the algorithm as skeleton edges, if possible.

      The user can use this matrix to make the algorithm select bigger patches than it normally would. 

      \note    Only the edges that violate only the settings_type::patch_angle_tolerance criterion can be excluded this way. \n
               The algorithm will keep any skeleton edge if it violates any other criterion 
               (settings_type::strain_tolerance or if this edge is necessary to achieve a successful remeshing).
      
      Mode = IN
      */
   UIntMat                 exclude_skeleton_edges;

   /**
      The user can specify in this matrix the edges that should be considered as 
      un-remeshable edges.

      \note    A "hard edge" is considered as a skeleton edge: it is a stitch in the initial mesh. \n
               Its vertices are also considered as skeleton nodes (= hard nodes).

      Mode = IN
      */
   UIntMat                 hard_edges;

   /**
      Upon entry, the user can use this vector to specify some nodes to remain in the final mesh. 

      Upon exit, this vector will contain in addition all the "skeleton" nodes, i.e.
      the topologically singular nodes (nodes connected to one, three or more skeleton lines)
      or the angulous nodes of the skeleton lines
      (angle between coincident skeleton lines greater than settings_type::patch_angle_tolerance).

      \note    Upon entry, only nodes of the _initial_ mesh are taken into account.

      Mode = IN-OUT
      */
   UIntVec                 hard_nodes;

   /**
      This vector can be used to exclude some nodes from being selected by the algorithm as skeleton nodes,
      if possible (only the geometrically singular nodes of the skeleton edges, i.e. the "angulous" nodes).

      \note    Only the angulous nodes (that violate only the settings_type::patch_angle_tolerance criterion)
               of the skeleton edges can be excluded. 
      \note    The algorithm will keep any other skeleton nodes, as well as the user's specified hard nodes.
      
      Mode = IN
      */
   UIntVec                 exclude_hard_nodes;

   /**
      This matrix is for the nodal data to be interpolated to the new mesh
      (FEM data, textures...)

      Data for node `i` is at column `interpolate_field.col(i)`.

      \note    As new nodes are appended to the coordinate matrix \ref pos, new interpolated values
               are appended to this matrix.

      \note    To eliminate the old nodes together with their data use the following code snippet after
               the remesher run:
               \code 
                  UIntMat     connectM;
                  connectM.copy(data.connectM);
                  cm2::meshtools::simplify (data.pos, connectM);                    // Eliminate old nodes.
                  cm2::meshtools::simplify (data.interpolate_field, data.connectM); // Do the same on the field.
               \endcode

      Mode = IN-OUT
      */
   DoubleMat               interpolate_field;

   /**
      The patch ID (color) of the initial elements.

      This vector makes possible to see how the initial mesh is divided by the algorithm.

      \note    Element `i` at `connectM.col(i)` upon entry is in the patch with color `colors_in[i]`.
      \note    Patches are numbered (i.e. coloured) consecutively starting from 0.
      \note    Patches with different properties that should be kept over the remeshing (such as material properties,
               boundary conditions...) must be separated from each other by a skeleton line (\ref skeleton_edges_in or \ref hard_edges).
      \note    Elements eliminated during the initial clean up step have no color (`CM2_NONE`). \n
               To avoid this, suppress the initial clean up step (set settings_type::initial_cleanup_flag = false).

      Mode = OUT
      */
   UIntVec                 colors_in;

   /**
      The patch ID (color) of the new elements.

      This vector makes possible to see how the final mesh elements relate to the initial ones.

      \note    Element `i` at `connectM.col(i)` upon exit is in the patch with color `colors[i]`.
      \note    All elements with the same color are remeshed elements from the initial elements with that color. \n
               Old element `i` and new element `j` are in the same patch when `colors_in[i] = colors[j]`.
      \note    Patches are numbered (i.e. coloured) consecutively starting from 0.

      Mode = OUT
      */
   UIntVec                 colors;

   /**
      The skeleton line ID (color) of the skeleton edges in the initial mesh.

      \note    Element `i` at `skeleton_edges_in.col(i)` is in the skeleton line with color `skeleton_colors_in[i]`.
      \note    Lines are numbered consecutively (i.e. coloured) starting from 0.
      \note    Lines with different properties that should be kept over the remeshing (such as material properties,
               boundary conditions...) must be skeleton lines (\ref skeleton_edges_in or \ref hard_edges)
               and, if connected, they must be separated from each other by a skeleton node (\ref hard_nodes).
      \note    A hard edge is a skeleton line with a single non-remeshable edge.

      \post    \p skeleton_colors_in.size() == \p skeleton_edges_in.cols()

      Mode = OUT
      */
   UIntVec                 skeleton_colors_in;

   /**
      The skeleton line ID (color) of the skeleton edges in the final mesh.

      This vector makes possible to see how the final skeleton edges relate to the initial ones.

      \note    Element `i` at `skeleton_edges.col(i)` is in the skeleton line with color `skeleton_colors[i]`.
      \note    All edges with the same color are remeshed edges from the initial edges with that color. \n
               Old edge `i` and new edge `j` are in the same patch when `skeleton_colors_in[i] = skeleton_colors[j]`.
      \note    Lines are numbered consecutively (i.e. coloured) starting from 0.
      \note    A hard edge is a skeleton line with a single non-remeshable edge.

      \post    \p skeleton_colors.size() == \p skeleton_edges.cols()

      Mode = OUT
      */
   UIntVec                 skeleton_colors;

   /** 
      The max distance between the initial mesh and the final mesh (Hausdorff distance).

      This is a measure of the geometric error between the two meshes.

      Mode = OUT
      */
   double                  max_error_distance;

   /**
      The number of elements in the initial mesh.

      Mode = OUT
      */
   unsigned                nefs_in;

   /**
      The number of nodes in the initial mesh.

      Mode = OUT
      */
   unsigned                nods_in;

   /**
      Area of the initial domain (sum of the areas of all triangles).

      Mode = OUT.
      */ 
   double                  area_in;

   /**
      Minimum shape quality of the elements in the initial mesh. 

      Mode = OUT.
      */ 
   double                  Qmin_in;

   /**
      Number of elements in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs;

   /**
      Number of quads in the final mesh (always null). 

      Provided for compatibility with surf_remesh_q4::mesher.

      Mode = OUT.
      */ 
   unsigned                nefs_Q4;

   /**
      Number of triangles in the final mesh. Same as \ref nefs.

      Mode = OUT.
      */ 
   unsigned                nefs_T3;

   /**
      Number of nodes in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nods;

   /**
      Area of the final mesh (sum of the areas of all elements).

      Mode = OUT.
      */ 
   double                  area;

   /**
      Area of the quadrangles. Always null.

      Provided for compatibility with surf_remesh_q4::mesher.

      Mode = OUT.
      */ 
   double                  area_Q4;

   /**
      Area of the triangles. Same as \ref area.

      Provided for compatibility with surf_remesh_q4::mesher.

      Mode = OUT.
      */ 
   double                  area_T3;

   /**
      Minimum shape quality of the elements in the final mesh. 

      Mode = OUT.
      */ 
   double                  Qmin;

   /**
      Total number of points in the coordinates matrix.

      \post Greater or equal to \ref nods.
      \post Equal to `ancestors.cols()`.

      Mode = OUT.
      */ 
   unsigned                total_nods;

   /**
      Time spent for cleaning the initial mesh (merging nodes too close to each other). 

      Mode = OUT.
      */ 
   double                  cleaning_time;

   /**
      Time spent for analysis the surface and finding the patches. 

      Mode = OUT.
      */ 
   double                  analysis_time;
   /**
      Time spent for remeshing the patches. 

      Mode = OUT.
      */ 
   double                  remesh_time;

   /**
      Time spent for optimizing the new mesh.
    
      Mode = OUT.
      */ 
   double                  optim_time;

   /**
      Total time spent for the remeshing. 

      Mode = OUT.
      */ 
   double                  total_time; 

   /**
      Number of elements per second. \n
      Global speed including all steps of the meshing / optimizing process.

      Mode = OUT.
      */ 
   double                  speed;

   /**
      Error code. 

      Mode = OUT.
      */ 
   error_type              error_code;

   /**
      Warning code. 

      Mode = OUT.
      */ 
   warning_type            warning_code;

   /**
      Error/Warning message #1. 

      Mode = OUT.
      */ 
   char                    msg1[256];

   /**
      Error/Warning message #2.
      Debug purpose only.

      Mode = OUT.
      */ 
   char                    msg2[256];

   };    // data_type


/**
   Constructor with default settings.

   See \link cm2::surf_remesh_t3::mesher::settings_type::settings_type() default values \endlink.
   */
mesher();

/**
   Constructor with specific settings.
   
   \param[in]   settings_        The settings. 
   */
mesher (const settings_type& settings_);

/**
   Is the mesh generator authorized to run ?

   \see  surf_remesh_t3::registration.
   */
static bool
authorized();


/**
   Main function of the mesh generator.

   \param[in,out]   data   The data.
   */
void 
run (data_type& data) const;


/**
   The settings of the %mesher.

   See \link cm2::surf_remesh_t3::mesher::settings_type::settings_type() default values \endlink.
   */
settings_type         settings;

};

}  // namespace surf_remesh_t3
}  // namespace cm2


/** 
   \example tmsh3dr01.cpp
   This is an example of how to use cm2::surf_remesh_t3::mesher and cm2::surf_remesh_q4::mesher.

   \image html matdol_i.png  "Initial 3-D surface (STL mesh)"
   \image html matdol_rt.png "Remeshed 3-D surface with surf_remesh_t3::mesher (pure-triangle)"
   \image html matdol_rq.png "Remeshed 3-D surface with surf_remesh_q4::mesher (all-quad)"
   \image html matdol_rqt.png "Remeshed 3-D surface with surf_remesh_q4::mesher (quad-dominant, 94% quads, 6% triangles)"
 */

#endif

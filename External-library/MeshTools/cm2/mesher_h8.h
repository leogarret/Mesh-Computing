#ifndef __CM2_HEXAMESH_MESHER_H__
#define __CM2_HEXAMESH_MESHER_H__

/**
   \file       mesher_h8.h
   \brief      Definition of the unstructured hex-dominant %mesher.
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
namespace hexamesh { 
 
/**
   Unstructured hex-dominant %mesher. 

   \image html engine_qt.png

   Instances of this class mesh a 3-D domain with hexahedrons, wedges, pyramids and tetrahedrons.

   Domains are defined by boundary meshes (all-quadrangle, all-triangle or mixed) 
   and possibly some embedded nodes, edges and faces. 

   It can also be used as an optimizer of an already existing mesh.

   All the data to be passed to the %mesher are gathered in a single 
   structure of type hexamesh::mesher::data_type.

   Local node numbering
   --------------------

   Hexahedrons:
   \verbatim
                          V

                          ^                                 
                          |                                    
                    3*----|------------*2                   F0 = {0 1 2 3}    
                    /|    |           /|                    F1 = {4 7 6 5}         
                   / |    |          / |                    F2 = {0 4 5 1}       
                  /  |    |         /  |                    F3 = {3 2 6 7}       
                 /   |    -        /   |                    F4 = {0 3 7 4}       
                /    |    |       /    |                    F5 = {1 5 6 2}       
               /     |    |      /     |                    
              /      |    |     /      |          
            7*-----------------*6      |                 
             |       |    o ---|----|------> U
             |      0* --/-----|-------*1
             |      /   /      |      /
             |     /   /       |     /
             |    /   -        |    /
             |   /   /         |   /
             |  /   /          |  /
             | /   /           | /
             |/   W            |/
            4*-----------------*5
   \endverbatim

   Wedges (prisms):
   \verbatim
                           V
                                1* 
                           ^    /|\                         FT0 = {0 1 2}   
                           |   / |  \                       FT1 = {3 5 4}   
                           |  /  |    \
                           | /   |      \                   FQ0 = {2 1 4 5}
                           |/    |        \                 FQ1 = {5 3 0 2}
                           /     |          \               FQ2 = {0 3 4 1}
                          /|     |            \
                         / |    2*-------------*0
                        /  |    /             /
                       /   |   /             /
                      /    |  /             /
                    4*     | /             /
                     |\    |/             /
                     |  \  O-------------/----> U
                     |    \             /
                     |   /  \          /
                     |  /     \       /
                     | /        \    /
                     |/           \ /
                    5*-------------*3
                    /
                   /

                 W
   \endverbatim

   Pyramids:
   \verbatim
                                 W       
                        
                                 ^                          FT0 = {0 4 1} 
                                 | 4                        FT1 = {1 4 2}
                                 *                          FT2 = {2 4 3}
                                /|\ \                       FT3 = {3 4 0}
                              /  |     \                    FQ  = {0 1 2 3}
                             / / | \      \
                            /    |           \
                          /  /   |  \           \
                         /       |           V     \
                        /  /     |   \     /          \
                      / 3 *------|-------/---------------* 2
                     /   /       |    \/                /
                   /   /         |   /                /
                  /  /           | /   \            /
                /  /             o ---------------/------>  U
               / /             /         \      /
             / /             /           \    /
            //             /              \ /
           *-------------/----------------*
          0            /                  1
                     /
   \endverbatim
                

   Tetrahedrons:
   \verbatim
                              V

                              ^
                              |
                              * 1               
                             /|\
                             /|  \
                            / |    \                        F0 = {1 2 3}
                            / |      \                      F1 = {2 0 3}
                           /  |        \                    F2 = {1 3 0}
                           /  |3         \ 0                F3 = {2 1 0}
                          /   *-----------*---> U
                          /  /        -
                         /  /      -
                         / /    -
                        / /  -
                        //-
                        * 2
                       /
                      /

                     W
   \endverbatim
          
   \note       Upon entry, the boundary faces in `data.connectB` need to be ordered the same way 
               in the case of several subdomains, or when internal closed surfaces (such as cavities) 
               are present. 
   \note       Thread safe.
   \warning    Boundary quadrangle faces are guaranteed to be recovered and present in the final mesh 
               only if they are _flat_. \n
               Warped quads may be split into two triangles (when `strict_constraints_level < 2`).\n
               Boundary triangles are always recovered and present in the final mesh (should they 
               do not intersect with any other hard entity).
   \see        tetramesh::mesher, surf_remesh_q4::mesher.
   */
class CM2HEXAMESH_API mesher
{
public:

   /**
      This generator is for mixed solid meshes.
      */
   enum
   {
      generated_FE_type = CM2_SOLID_MIX         //!< Generates mixed solid meshes (hexes, wedges, pyramids and tetrahedrons).
   };


   /**
      The settings of hexamesh::mesher.

      This structure gathers flags and parameters to control the way the hexamesh::mesher works.
      */
   struct CM2HEXAMESH_API settings_type
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
      Basic mode for the %mesher.
      */
   enum basic_mode_type
   { 
      MESH_MODE,                //!< Normal mode of the %mesher (generates a new mesh).
      REGULARIZE_MODE,          //!< Takes an input mesh and optimizes it (nodes are added, removed, smoothed, connectivity is changed).
      CONVEX_HULL_MODE          //!< Produces an approximate convex hull of a set of points (_tetrahedrons_ only).
   };


   /**
      Default constructor.

      The default values are:
         - \ref basic_mode = \ref MESH_MODE 
         - \ref strict_constraints_level = 1
         - \ref subdomains_forcing = 0
         - \ref refine_flag = `true`
         - \ref use_default_background_mesh_flag = `false`
         - \ref make_conformal_flag = `true`
         - \ref node_smoothing_flag = `true`
         - \ref node_inserting_flag = `true`
         - \ref node_removing_flag = `true`
         - \ref shell_remeshing_flag = `true`
         - \ref compute_Qh_flag = `false`
         - \ref structured_flag = `true`
         - \ref nodes_limit = `UINT_MAX`
         - \ref optim_level = 3
         - \ref target_metric = 0.0
         - \ref max_gradation = 0.5
         - \ref shape_quality_weight = 0.60
         - \ref enforcement_quality_threshold = 1E-4
         - \ref display_hdl = `NULL`
         - \ref interrupt_hdl = `NULL`
         - \ref pass_thru = `NULL`
         - \ref progress_start = 0.0
         - \ref progress_range = 1.0
      */
   settings_type()
      : basic_mode(MESH_MODE), strict_constraints_level(1), subdomains_forcing(0), 
        refine_flag(true), use_default_background_mesh_flag(false),
        make_conformal_flag(true), 
        node_smoothing_flag(true), node_inserting_flag(true), node_removing_flag(true), 
        shell_remeshing_flag(true), compute_Qh_flag(false), structured_flag(true), 
        nodes_limit(UINT_MAX), optim_level(3),
        target_metric(0.0), max_gradation(0.5),
        shape_quality_weight(0.60), enforcement_quality_threshold(1E-4),
        display_hdl(NULL), interrupt_hdl(NULL), pass_thru(NULL),
        progress_start(0.0), progress_range(1.0)
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
                     (\ref basic_mode is the first field, \ref strict_constraints_level is the second...)
      */
   int
   check() const;

   /**
      Resets to the \link cm2::hexamesh::mesher::settings_type::settings_type() default values \endlink.
      */
   void
   reset();


   /**
      Basic parameters of the %mesher: \ref MESH_MODE, \ref REGULARIZE_MODE or \ref CONVEX_HULL_MODE.

      \warning    In \ref CONVEX_HULL_MODE, the %mesher produces only _tetrahedrons_, and the \ref data_type::connectM matrix has
                  dimensions 4 x nefs.

      Default = \ref MESH_MODE.
      */
   basic_mode_type         basic_mode;

   /**
      Enforcement strictness level for hard nodes, edges and faces.

      If 0, no error is ever raised on hard entity recovery issue (only warnings).

      If 1, an error is raised when the boundary mesh has intersection(s), i.e. in the following cases:
         -  A hard node is intersecting another hard entity (node, edge or face).
         -  A hard edge is intersecting another hard entity (edge or face).
         -  A hard face is intersecting another hard face.

      Note that even with a valid boundary mesh, some wall quads can be missing in the final mesh 
      (too hard to enforce) and replaced with two triangles.\n 
      In these cases, a mere data_type::CM2_FACE_Q_DISCARDED warning is raised instead.

      If 2, an error is raised in the previous case plus when a valid boundary quad could not be
      enforced in the final mesh.

      \note    When a hard node, edge or face is outside the meshed domain
               only a data_type::CM2_NODE_DISCARDED, data_type::CM2_EDGE_DISCARDED or 
               data_type::CM2_FACE_DISCARDED warning is issued, even when \p strict_constraints_level > 0. 

      Default = 1
      */
   unsigned                strict_constraints_level;

   /**
      Forcing mode for intern subdomains.
      
      Possible values are:
         -  -1 : All intern subdomains are considered as holes regardless of the orientation of
                 their enclosing boundary.
         -   0 : Intern subdomains are considered as holes when the orientation of
                 their enclosing boundary is opposite to the orientation of the most enclosing domain.\n
                 Otherwise, they are meshed.
         -  +1 : All intern subdomains are meshed regardless of the orientation of
                 their enclosing boundary.

      \note    Used only in \ref MESH_MODE.

      Default = 0.
      */
   int                     subdomains_forcing;

   /**
      Flag to enable refinement. 

      If false, mesh with hard nodes only and possibly some compulsory "Steiner" nodes
      to enforce wall faces and conformation (\ref make_conformal_flag).

      If true, adds new nodes inside the domain to achieve good shape and size qualities.

      \note    Not used in \ref CONVEX_HULL_MODE.
      \note    In \ref REGULARIZE_MODE, unsetting this flag is equivalent to unset flag 
               \ref node_inserting_flag.

      Default = true.
      */
   bool                    refine_flag;

   /**
      Flag to tell the %mesher to use the default background mesh for the interpolation
      of the metric values. 

      For expert users only.

      The default background mesh is the "front" mesh, i.e. the mesh of the triangulation of the hard nodes
      without any new node.

      If set to false, the background mesh used to interpolate the metrics is the mesh currently processed.

      \warning A user-defined background mesh supersedes the default background mesh, even
               with this flag set.
      \note    Using the current mesh is the fastest mode which indeed gives a similar accuracy,
               so it is recommended to leave this flag to false.
      \note    For expert users only.

      Default = false.
      */
   bool                    use_default_background_mesh_flag;

   /**
      Flag to force the mesh to be conformal.

      In a conformal mesh (`make_conformal_flag = true`), the mesher generates only 
      fully conformal T3-T3 and Q4-Q4 joints. \n
      Hence no hexahedron can have a tetrahedron as neighbour, only another hexahedron, wedge 
      or pyramid by one of its quadrangle faces.

      In the non-conformal mode (`make_conformal_flag = false`), joints between a Q4 and two T3s 
      (semi-conformal joints) are also allowed giving the mesher much more opportunities 
      to reduce the number of tetrahedrons and pyramids. \n

      In any case, no Q4 internal face can ever be jointed partially (by three nodes) to another Q4 face.

      \note Used in the \ref MESH_MODE only.

      Default = true
      */
   bool                    make_conformal_flag;

   /**
      Flag to allow the node-smoothing scheme in the optimization step. 

      It can be useful in \ref REGULARIZE_MODE to set this flag to false
      to freeze the positions of the nodes.

      \note    Node smoothing doesn't change the mesh connectivity, only the coordinates of nodes.
      \note    This option, as part of the optimization step, is taken into account only when \ref optim_level > 0.
      \note    Used in \ref MESH_MODE and \ref REGULARIZE_MODE.

      Default = true.
      */
   bool                    node_smoothing_flag;

   /**
      Flag to allow the node-inserting scheme in the optimization step. 
      
      \note    Node inserting increases the number of nodes, changes the mesh 
               connectivity, but doesn't change the other nodes' coordinates.
      \note    This option, as part of the optimization step, is taken into account only when \ref optim_level > 0.
      \note    Used in \ref MESH_MODE and \ref REGULARIZE_MODE.

      Default = true.
      */
   bool                    node_inserting_flag;

   /**
      Flag to allow the node-removing scheme in the optimization step. 

      \note    Node removing decreases the number of nodes, changes the mesh 
               connectivity, but doesn't change the other nodes' coordinates.
      \note    This option, as part of the optimization step, is taken into account only when \ref optim_level > 0.
      \note    Used in \ref MESH_MODE and \ref REGULARIZE_MODE.

      Default = true.
      */
   bool                    node_removing_flag;

   /**
      Flag to allow the shell-remeshing scheme in the optimization step. 

      \note    Shell remeshing changes the mesh connectivity,
               but doesn't change the number of nodes nor their coordinates.
      \note    This option, as part of the optimization step, is taken into account only when \ref optim_level > 0.
      \note    Used in \ref MESH_MODE and \ref REGULARIZE_MODE.

      Default = true.
      */
   bool                    shell_remeshing_flag;

   /**
      Flag to compute the edge quality histogram (quality in term of edge length).

      This operation can be memory expensive and should be avoided for big cases.

      \note    Not used in \ref CONVEX_HULL_MODE.

      Default = false
      */
   bool                    compute_Qh_flag;

   /**
      Flag to enable structured (grid-like) meshing whenever possible.

      If true, generates a structured mesh (grid-like) whenever possible, i.e. when:
         - The domain is a swiped volume.
         - The meshes of the opposite faces match (similar meshes).
         - There is no isolated hard edge or node (even located outside the domain).
         - There is no repulsive point (even located far outside the domain).
         - There is no valid \ref target_metric specified.

      If false, uses the unstructured Delaunay-based algorithm 
      (can give also a structured mesh in simple cases).

      \note    With this flag set, all-hex meshes can be generated in more cases
               (but hexes can be with very flat and edge length qualities can be very bad too). \n
      \note    Used only when \ref refine_flag = true.
      \note    Used in \ref MESH_MODE only.

      Default = true.
      */
   bool                    structured_flag;

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
               Hence, increasing the value of optim_level may lead sometimes to a
               reduction in the shape qualities (min, average or max value) whereas the _global_ quality (which takes into 
               account both types of quality) gets better. 
      \note    The \ref shape_quality_weight parameter controls the weight of the shape quality in the global 
               quality measure.
      \note    Level 0 disables any optimization.
      \note    Increasing this parameter can also help increase the number of recovered wall-face quads.

      Default = 3
      */
   unsigned                optim_level;

   /**
      Element size inside the domain.

      The elements size tends toward this value as they get away from the hard (boundary) edges. 
      
      \warning Discarded when <= 0.
      \warning Discarded when a background mesh is used.
      \warning Discarded when \ref use_default_background_mesh_flag = true.
      \note    This parameter is often used to reduce the total number of elements
               (when target_metric > default sizes based on boundary edge lengths) 
               and to save FEM computation time without loosing much on element shape quality.
      \note    The target_metric may be smaller or bigger than the default sizes based on boundary edge lengths.
      \note    target_metric >= 0.
      \note    Used in \ref MESH_MODE only.
      \see     max_gradation.

      Default = 0 (disabled)
      */
   double                  target_metric;

   /**
      This parameter controls the gradation of the elements size from the boundary sizes 
      to the size defined by \ref target_metric inside the domain. 

      A value close to 0 leads to a more progressive variation of mesh size (smoother).

      \warning Discarded when \ref target_metric <= 0.
      \warning Discarded when a background mesh is used.
      \warning Discarded when \ref use_default_background_mesh_flag = true.
      \note    max_gradation >= 0.
      \note    Used in \ref MESH_MODE only.
      \see     target_metric.

      Default = 0.5
      */
   double                  max_gradation;

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
      Threshold for the boundary enforcement scheme (wall-quad enforcement).

      The boundary enforcement scheme (to force boundary quads) may generate poor
      elements in the vicinity of some wall quads.

      Increasing the value of this parameter may decrease the number of enforced boundary quads (wall quads),
      but help improve the quality of the elements near these quads. 
      
      Conversely, a lower value forces more quads but can lead to lower wall element qualities.

      Default = 1E-4

      \warning    For expert users only.
      \note       The unenforced quads will be split and replaced with two triangles in the boundary of the resulting mesh.
      \note       Not used when \ref strict_constraints_level = 2 (equivalent to enforcement_quality_threshold = 0).
      \pre        enforcement_quality_threshold >= 0.
      \pre        enforcement_quality_threshold <= 1.
      */
   double                  enforcement_quality_threshold;

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

      When this function returns true, the process is stopped. \n
      This can abort the mesh (if the interruption occurred at an early stage),
      or result in a mesh of poor quality (if the %mesher was in the refinement or optimization 
      stage when the interruption occurs).

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

   /**
      The start value for the progress parameter sent to the interrupt handler.

      Default = 0.0
      */
   double                  progress_start;

   /**
      The range for the progress parameter sent to the interrupt handler.

      The end value equals to progress_start + progress_range.

      Default = 1.0
      */
   double                  progress_range;

   };    // settings_type



   /**
      The data type for hexamesh::mesher.

      This structure is used to gather all the input and output data of the function 
      hexamesh::mesher::run.
      */
   struct CM2HEXAMESH_API data_type
   {

   /// The display handler type.
   typedef settings_type::display_handler_type        display_handler_type;


   /**
      Warning enums for the %mesher.

      Non-fatal warnings.

      If such a warning is raised (returned in the field \ref warning_code),
      the %mesher is not stopped but the user shall check its input and output data for a potential
      mistake.
      */
   enum warning_type
   { 
      CM2_NO_WARNING                =   0,   //!< OK, no warning.
      CM2_INTERRUPTION              = -10,   //!< The process has been interrupted (by the user through the interrupt handler).
      CM2_NODES_LIMIT_WARNING       = -11,   //!< The limitation on the number of nodes has been reached.
      CM2_NODE_DISCARDED            = -12,   //!< An invalid hard node has been rejected (too close to another hard node).
      CM2_EDGE_DISCARDED            = -13,   //!< An invalid hard edge has been rejected (cut by another hard edge/node).
      CM2_FACE_DISCARDED            = -14,   //!< An invalid hard face has been rejected (cut by another hard face/edge/node).
      CM2_FACE_Q_DISCARDED          = -15,   //!< A valid hard quad could not be enforced (split into two triangles instead).
      CM2_BOUNDARY_WARNING          = -16,   //!< The boundary was not watertight and had to be corrected.
      CM2_SHAPE_QUALITY_WARNING     = -17    //!< One or several elements have a bad quality (Qs < 0.01).
   };


   /**
      Error enums for the %mesher.

      Fatal errors.

      If such an error is raised (returned in the field \ref error_code),
      the %mesher is stopped and no final mesh is returned.
      */
   enum error_type
   { 
      CM2_NO_ERROR                  =    0,  //!< OK, no error.
      CM2_LICENSE_ERROR             = -100,  //!< Invalid or expired license.
      CM2_MODE_ERROR                = -101,  //!< Error in settings.
      CM2_DATA_ERROR                = -102,  //!< Error in input data (`NaN`, `INF`, error in dimensions of arrays...)
      CM2_NODES_LIMIT_ERROR         = -103,  //!< The limitation on the number of nodes is too tight.
      CM2_NODE_ERROR                = -104,  //!< Two nodes are too close to each other (same position or almost). 
      CM2_EDGE_ERROR                = -105,  //!< A hard edge is cut by another hard edge or has a hard node inside it. 
      CM2_FACE_ERROR                = -106,  //!< A hard face is cut by another hard face/edge/node.
      CM2_FACE_Q_ERROR              = -107,  //!< A valid hard quad could not be enforced. 
      CM2_BOUNDARY_ERROR            = -108,  //!< Illegal boundary(ies) provided by the user (boundary not closed, boundary Xings...) 
      CM2_DEGENERATED_ELEMENT       = -109,  //!< The process leads to at least one degenerated element and the %mesher cannot build a valid mesh.
      CM2_BACKGROUND_MESH_ERROR     = -110,  //!< The background mesh is not valid (bad orientation, bad dimensions, bad indices, bad associated metrics, degenerated elements...)
      CM2_SYSTEM_MEMORY_ERROR       = -199,  //!< Insufficient memory available.
      CM2_INTERNAL_ERROR            = -200   //!< Something went wrong but don't know what (please contact support@computing-objects.com).
   };


   /**
      The type of metric.

      This mesh generator is _isotropic_ and the metrics are scalars.
     */
   typedef double             metric_type;

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

      Constructs the data with a coordinates matrix and a boundary matrix of faces (shallow copy).
      This is the simplest way to create the data to exchange with the %mesher.

      \param   P           The coordinates matrix. New nodes are appended by the %mesher.
      \param   connectQ4T3 The connectivity matrix of the boundary mesh (mixed or pure).

      \note    After the meshing process, you must get the new coordinate matrix back from the field \ref pos,
               and the connectivity matrix \ref connectM as well, or use the \ref extract function.
      */
   data_type (const DoubleMat& P, const UIntMat& connectQ4T3)
   {
      this->clear();
      this->pos = P;
      this->connectB = connectQ4T3;
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
                     (\ref pos is the first field, \ref connectB is the second...)
      */
   int
   check() const;

   /**
      Shallow copy.

      \param[out]    P           A view to the \ref pos matrix.
      \param[out]    connectHX   A view to the \ref connectM matrix.
                                 This matrix has dimension 8 x nefs:
                                    - The leading part of this matrix (from col 0 to col \ref nefs_H8 - 1)
                                      is the connectivity of the hexahedron elements (8 nodes).
                                    - Columns between \ref nefs_H8 and \ref nefs_H8 + \ref nefs_W6 - 1 are for wedges (6 nodes then 2 `CM2_NONE`). 
                                    - Columns between \ref nefs_H8 + \ref nefs_W6 and \ref  nefs_H8 + \ref nefs_W6 + \ref nefs_P5 - 1 are for pyramids (5 nodes then three `CM2_NONE`). 
                                    - The tailing part of this matrix (from col \ref nefs_H8 + \ref nefs_W6 + \ref nefs_P5 to col \ref nefs - 1)
                                      is the connectivity of the tetrahedron elements (4 nodes then 4 `CM2_NONE`). 
      */
   void
   extract (DoubleMat& P, UIntMat& connectHX) const;

   /**
      Shallow copy.

      \param[out]    P           A view to the \ref pos matrix.
      \param[out]    connectH8   A view to the leading part of \ref connectM, related to hexahedrons. \n
                                 Same as connectM.sub_matrix(0, 0, 8, \ref nefs_H8). \n
                                 This matrix has dimension 8 x \ref nefs_H8.
      \param[out]    connectW6   A view to the part of \ref connectM, related to wedges. \n
                                 Same as connectM.sub_matrix(0, \ref nefs_H8, 6, \ref nefs_W6). \n
                                 This matrix has dimension 6 x \ref nefs_W6.
      \param[out]    connectP5   A view to the part of \ref connectM, related to pyramids. \n
                                 Same as connectM.sub_matrix(0, \ref nefs_H8 + \ref nefs_W6, 5, \ref nefs_P5). \n
                                 This matrix has dimension 5 x \ref nefs_P5.
      \param[out]    connectT4   A view to the part of \ref connectM, related to tetrahedrons. \n
                                 Same as connectM.sub_matrix(0, \ref nefs_H8 + \ref nefs_W6 + \ref nefs_P5, 4, \ref nefs_T4). \n
                                 This matrix has dimension 4 x \ref nefs_T4.
      */
   void
   extract (DoubleMat& P, UIntMat& connectH8, UIntMat& connectW6, UIntMat& connectP5, UIntMat& connectT4) const;

   /// Clears all fields and reinit to default values.
   void
   clear();

   /// Clears only the output fields.
   void
   clear_data_out();

   /**
      Gets the order of elements according to their color.

      Simply calls meshtools::get_sorted_order on \ref colors. \n
      Provided for convenience only.

      Example:
      \code
         hexamesh::mesher              my_mesher;
         hexamesh::mesher::data_type   my_data(pos, connectB);  
         UIntVec                       color_order, xColor;
         UIntVec                       IDs;
         UIntMat                       connect_7;

         // Do the meshing.
         my_mesher.run(my_data);               

         // Get element IDs sorted by color.
         my_data.IDs_sorted_by_color(color_order, xColor);             

         // Get the elements with color 7.
         IDs = color_order.sub_vector(xColor[7], xColor[8]-xColor[7]);     // All the element IDs with color 7.
         matmat::push_back(my_data.connectM, IDs, connect_7);              // connect_7 = connectivity matrix of the elements with color 7.
      \endcode
      */
   void
   IDs_sorted_by_color (UIntVec& color_order, UIntVec& xColor) const
   {
      meshtools::get_sorted_order(this->colors, color_order, xColor);
   }

   /**
      Prints information about the mesh using a display handler.

      \param   hdl   The display handler.

      \note    The first parameter of the handler (param `pass_thru`) is irrelevant (`NULL`).
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
   error_raised() const       { return this->error_code != CM2_NO_ERROR; }

   /// Checks if a warning has been raised.
   inline bool
   warning_raised() const     { return this->warning_code != CM2_NO_WARNING; }


   /**
      The coordinates matrix (column oriented).

      Node `i` has coordinates in column `pos.col(i)`.

      Upon entry, this matrix must contains the coordinates of all the points (i.e. nodes referenced in 
      \ref connectB, \ref connectE, \ref isolated_nodes, \ref background_mesh and \ref repulsive_points). 

      It may also contain other unreferenced points.

      Upon exit, the new nodes are appended at the back of the matrix and the indices in the
      final mesh (\ref connectM) refer to column numbers in this matrix.

      Mode = IN-OUT in settings_type::MESH_MODE and settings_type::REGULARIZE_MODE, \n
             IN in settings_type::CONVEX_HULL_MODE.
      */
   DoubleMat               pos;

   /**
      The hard faces: external (boundary) faces + internal (embedded) hard faces.

      This connectivity matrix must define a valid and closed triangle/quad mesh for the 
      external boundary (skin mesh) of the domain to be meshed.

      The orientation of the boundary faces must be similar (either all normals outward or all inward). \n 
      However, mixed orientations are accepted when:
         - There is only a single external surface.
         - \ref settings_type::subdomains_forcing = +1. 
           In this case, all subdomains are filled with elements (no cavity).

      Internal (embedded) hard faces can be added to the boundary mesh. 

      In order to define holes inside the domain, the boundary mesh of the holes must be closed
      and oriented oppositely to the external boundary mesh 
      (and \ref settings_type::subdomains_forcing = 0 or -1).

      The indices refer to columns in the coordinates matrix \ref pos.

      \note    Mixed quad-triangle, all-quad and all-triangle meshes are allowed.
      \note    In settings_type::REGULARIZE_MODE, 
               external faces and faces between elements with different colors are added. \n
               These new faces, if any, are also considered as hard entities.
      \note    In settings_type::CONVEX_HULL_MODE, only the nodes in \ref connectB 
               and \ref isolated_nodes are taken into account (the edges are not forced to
               be present in the final mesh).
      \see     \ref settings_type::subdomains_forcing, \ref boundary_sgn.
      \see     meshtools::get_mesh_boundaries, meshtools::get_colors_boundaries.

      \pre     connectB is empty or connectB.rows() == 3 (all-triangle mesh) or
               connectB.rows() == 4 (all-quad or mixed mesh).

      Mode = IN in settings_type::MESH_MODE, \n
             IN-OUT in settings_type::REGULARIZE_MODE.
      */
   UIntMat                 connectB;

   /**
      The internal (embedded) hard edges.

      In strict mode (when settings_type::strict_constraints_level > 0), hard edges are not allowed to cut 
      each other nor edges of the hard faces.

      The indices refer to columns in the coordinates matrix \ref pos.

      Mode = IN.
      */
   UIntMat                 connectE;

   /**
      The internal (embedded) hard nodes.

      \note    The indices refer to columns in the coordinates matrix \ref pos.

      Mode = IN.
      */
   UIntVec                 isolated_nodes;

   /**
      The nodes of the mesh (that are not hard nodes) will be placed away from
      these repulsive points.

      \note    The indices refer to columns in the coordinates matrix \ref pos.
      \note    Only repulsive points associated to a valid mesh size (i.e. positive value in
               \ref metrics) are taken into account.
      \note    Not used in settings_type::CONVEX_HULL_MODE.

      Mode = IN.
      */
   UIntVec                 repulsive_points;

   /**
      The connectivity matrix of the background mesh, made of pure tetrahedrons (column oriented). 

      The indices of the nodes refer to columns in the coordinates matrix \ref pos.
      These indices must also refer to valid values in the \ref metrics array.

      The background mesh doesn't need to match exactly the domain to be meshed. 
      It is allowed to extend the domain or to cover only a fraction of it.
      For subdomains outside of the background mesh, the standard interpolation of the metrics
      defined (or computed) at the hard nodes is used.

      \warning    The use of a background mesh may slow down the generator significantly.
                  The finer the background mesh, the slower the meshing.

      \note    Used in settings_type::MESH_MODE only.
      \note    Only pure tetrahedral meshes are accepted as background mesh.

      Mode = IN.
      */
   UIntMat                 background_mesh;

   /**
      Element sizes specified at nodes.

      For nodes with invalid metric (missing, negative or null), an automatic size is 
      computed using the distances to their neighbours and stored in this vector upon exit.

      \note    Upon exit, this array takes into account the values settings_type::target_metric 
               and settings_type::max_gradation.
      \note    Not used in settings_type::CONVEX_HULL_MODE.

      Mode = IN-OUT.
      */
   DoubleVec               metrics;

   /**
      The connectivity of the elements (column oriented). 

      The indices refer to columns in the coordinates matrix \ref pos.

      The dimensions of this matrix are 8 x \ref nefs in settings_type::MESH_MODE and in settings_type::REGULARIZE_MODE,
      but only 4 x \ref nefs in settings_type::CONVEX_HULL_MODE (only tetrahedrons are generated in this case).

      Mode = OUT in settings_type::MESH_MODE and settings_type::CONVEX_HULL_MODE, \n
             IN-OUT in settings_type::REGULARIZE_MODE.
      */
   UIntMat                 connectM;

   /**
      The boundaries that could not be enforced in the mesh.

      This vector gives the column IDs in the \ref connectB matrix that could not be enforced in
      the solid mesh (i.e. boundaries intersected by other hard nodes, edges or faces).

      Regular (not intersected) wall quads can also remain unenforced (when warped and in difficult conditions).

      \note    In strict-constraints mode (settings_type::strict_constraints_flag = true), 
               the generator stops with an error only for intersecting entities, not for
               entities outside the meshed domain.
      \note    To enforce more wall face quads, increase parameter optim_level.
      \note    Used in settings_type::MESH_MODE only.

      Mode = OUT.
      */
   UIntVec                 unenforced_boundary_IDs;        

   /**
      The internal edges that could not be enforced in the solid mesh.

      This vector gives the column IDs in matrix \ref connectE of the internal edges that could not be enforced in
      the solid mesh (i.e. edges intersected by other hard nodes or hard edges, edges outside the domain).

      \note    In strict-constraint mode (settings_type::strict_constraints_flag > 0), 
               the generator stops with an error only for pathological (i.e. intersecting) entities, not for
               entities outside the meshed domain.
      \note    Used in settings_type::MESH_MODE only.

      Mode = OUT.
      */
   UIntVec                 unenforced_edge_IDs;        

   /**
      The embedded (isolated) nodes that could not be enforced in the mesh.

      This vector gives the indices of the nodes that could not be enforced in
      the final mesh (i.e. nodes coincident with another one, nodes inside hard edges/faces, nodes outside the domain).

      \note    In strict-constraints mode (settings_type::strict_constraints_flag > 0), 
               the generator stops with an error only for pathological (i.e. intersecting) entities, not for
               entities outside the meshed domain.
      \note    Used in settings_type::MESH_MODE only.

      Mode = OUT.
      */
   UIntVec                 unenforced_node_IDs;        

   /**
      The boundaries that intersect other boundary(ies).

      This vector gives the column IDs in the \ref connectB matrix of the faces that intersect other hard face(s).

      \note    In strict-constraint mode (settings_type::strict_constraints_level > 0),
               the generator stops with an error when this vector is not empty.
      \note    Used in settings_type::MESH_MODE only.

      Mode = OUT.
      */
   UIntVec                 pathological_boundary_IDs;

   /**
      The "color" of the elements. 

      Each subdomain is assigned with a color ID (consecutive values starting from 0). \n
      Each element is assigned with the color ID of the subdomain it belongs to.

      A subdomain is a subset of connected elements (not separated by any boundary). \n
      Two elements are in the same subdomain when there is a element path between them 
      which is not crossed by any boundary.
      
      Upon exit, the size of this vector equals to the number of elements in \ref connectM.

      The indices refer to columns in the connectivity matrix \ref connectM.

      In settings_type::REGULARIZE_MODE, this vector is used upon entry to search for additional
      hard boundaries (boundaries between elements of different colors). \n
      The user can also provide an empty color vector. In this case, all elements 
      are assumed to be in the same subdomain (color = 0).

      In settings_type::CONVEX_HULL_MODE, all elements are in the same subdomain (color = 0).

      Mode = OUT in settings_type::MESH_MODE and settings_type::CONVEX_HULL_MODE, \n
             IN-OUT in settings_type::REGULARIZE_MODE.
      */
   UIntVec                 colors;

   /**
      The neighbors matrix. 

      `neighbors(i,j)` is the neighbor of element `j` with face `i`.

      \post    The dimensions of this matrix are 8 x \ref nefs in settings_type::MESH_MODE and 
               in settings_type::REGULARIZE_MODE, \n
               but only 4 x \ref nefs in settings_type::CONVEX_HULL_MODE (tetrahedrons are generated in this case).

      \see  The User's Manual.

      Mode = OUT.
      */ 
   UIntMat                 neighbors;

   /**
      The ancestors vector. 

      `ancestors[i]` is the index of one of the elements connected to node `i`.

      These indices refer to columns in matrices \ref connectM and \ref neighbors, and to position
      in vector \ref colors.

      If a point is not connected to any element, its ancestors' index equals to `CM2_NONE` (i.e. `unsigned(-1)`).

      \post    The size of this vector equals to `pos.cols()` (i.e. number of points).

      Mode = OUT.
      */ 
   UIntVec                 ancestors;

   /**
      The shape qualities vector. 

      The size of this vector is \ref nefs.

      \see  The User's Manual.

      Mode = OUT.
      */ 
   DoubleVec               shape_qualities;

   /**
      The histogram of the element shape qualities.

      Mode = OUT.
      */ 
   misc::histogram         histo_Qs; 

   /**
      The histogram of the edges qualities (edge length).

      Computed only when settings_type::compute_Qh_flag = true.

      Mode = OUT.
      */ 
   misc::histogram         histo_Qh;

   /**
      Number of elements in the final mesh. 

      \p nefs = \ref nefs_H8 + \ref nefs_W6 + \ref nefs_P5 + \ref nefs_T4

      Mode = OUT.
      */ 
   unsigned                nefs;

   /**
      Number of hexahedrons in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs_H8;

   /**
      Number of wedges in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs_W6;

   /**
      Number of pyramids in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs_P5;

   /**
      Number of tetrahedrons in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs_T4;

   /**
      Number of nodes in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nods;

   /**
      Total number of points in the coordinates matrix.

      \post Greater or equal to \ref nods.
      \post Equal to `ancestors.cols()`.
      \post Equal to `metrics.size()`.

      Mode = OUT.
      */ 
   unsigned                total_nods;

   /**
      The number of initial hard nodes.

      Mode = OUT.
      */ 
   unsigned                hard_nodes_in;

   /**
      The number of initial hard edges. 

      Mode = OUT.
      */ 
   unsigned                hard_edges_in;

   /**
      The number of initial hard faces. 

      Mode = OUT.
      */ 
   unsigned                hard_faces_in;

   /**
      The number of final hard nodes. 

      \post    \p hard_nodes_out <= \ref hard_nodes_in

      Mode = OUT.
      */ 
   unsigned                hard_nodes_out;

   /**
      The number of final hard edges. 
      
      \post    \p hard_edges_out <= \ref hard_edges_in

      Mode = OUT.
      */ 
   unsigned                hard_edges_out;

   /**
      The number of final hard faces. 
      
      \post    \p hard_faces_out <= \ref hard_faces_in

      Mode = OUT.
      */ 
   unsigned                hard_faces_out;

   /**
      Total volume of the meshed domain.

      \p volume = \ref volume_H8 + \ref volume_W6 + \ref volume_P5 + \ref volume_T4

      Mode = OUT.
      */ 
   double                  volume;

   /**
      Volume of all the hexahedrons.

      Mode = OUT.
      */ 
   double                  volume_H8;

   /**
      Volume of all the wedges.

      Mode = OUT.
      */ 
   double                  volume_W6;

   /**
      Volume of all the pyramids.

      Mode = OUT.
      */ 
   double                  volume_P5;

   /**
      Volume of all the tetrahedrons.

      Mode = OUT.
      */ 
   double                  volume_T4;

   /**
      Minimum shape quality of the elements upon entry. 

      Mode = OUT in settings_type::REGULARIZE_MODE only.
      */ 
   double                  Qmin_in;

   /**
      Minimum shape quality of the elements upon exit. 

      Mode = OUT.
      */ 
   double                  Qmin;

   /**
      The bounding box's min corner. 

      Mode = OUT.
      */ 
   DoubleVec3              min_box;

   /**
      The bounding box's max corner. 

      Mode = OUT.
      */ 
   DoubleVec3              max_box;

   /**
      Number of subdomains in the domain.

      Mode = OUT.
      */ 
   unsigned                subdomains;

   /**
      The orientation of the external boundary.

      The orientation sign is defined by:
         -  +1 when oriented towards the interior of the domain.
         -  -1 when oriented towards the exterior of the domain.
         -  0  when some faces are oriented towards the interior of the domain
               and some towards the exterior 
               (i.e. the external surface orientation is not well defined).

      \note    If the orientation sign is null, the mesher has undefined behaviour with
               respect to any internal/external closed surface (may be considered as hole or not),
               except when forced (by using \ref settings_type::subdomains_forcing).
      \note    This parameter is computed in settings_type::MESH_MODE only.
      \see     \ref settings_type::subdomains_forcing.

      Mode = OUT.
      */ 
   int                     boundary_sgn;

   /**
      Time spent in the initial step. 

      Mode = OUT.
      */ 
   double                  init_time;

   /**
      Time spent in the mesh generation step. 

      Mode = OUT.
      */ 
   double                  mesh_time;

   /**
      Time spent for optimizing the mesh.

      Mode = OUT.
      */ 
   double                  optim_time;

   /**
      Total time spent for the mesh. 

      Mode = OUT.
      */ 
   double                  total_time; 

   /**
      Number of elements per second. 

      Global speed including all steps of the meshing/optimizing process.

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

   See \link cm2::hexamesh::mesher::settings_type::settings_type() default values \endlink.
   */
mesher();

/**
   Constructor with specific settings.

   \param[in]   settings_        The settings. 
   */
mesher (const settings_type& settings_);

/**
   Is the %mesher authorized to run?

   \see  hexamesh::registration.
   */
static bool
authorized();

/**
   Main function of the %mesher.

   Depending on the settings, the %mesher can work
   as a regular %mesher or as a mesh optimizer.

   \param[in,out]   data   The data.
   */
void 
run (data_type& data) const;


/**
   The settings of the %mesher.

   See \link cm2::hexamesh::mesher::settings_type::settings_type() default values \endlink.
   */
settings_type         settings;

};


/** \example tmshhx01.cpp
    This is a simple example (cube) of how to use the hex-dominant unstructured %mesher cm2::hexamesh::mesher.

    \image html tmshhx01.png "Simple cube with structured boundary mesh." 
    */


}  // end namespace hexamesh
}  // end namespace cm2

#endif

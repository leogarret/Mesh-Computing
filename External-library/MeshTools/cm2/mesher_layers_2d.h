/**
   \file       mesher_layers_2d.h
   \brief      Definition of 2-D boundary layer %mesher.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3548 $
   $Date: 2019-03-20 11:20:47 +0100 (mer., 20 mars 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHER_LAYERS_2D_H__
#define __CM2_MESHER_LAYERS_2D_H__


#include "fe_defs.h"
#include "math1.h"
#include "meshtools.h"
#include "misc.h"


namespace cm2 {
namespace layers2d { 

/**
   2-D boundary layer %mesher.

   Instances of this class generate boundary layer meshes in any 2-D domains defined 
   by some boundary edges and possibly some embedded edges.
   
   All the data to be passed to the %mesher are gathered in a single 
   structure of type layers2d::mesher::data_type.

   Example:
   \verbatim

   Boundary layer in a square

   INPUT:
                                                  3                  2
      data.pos          = [0 4 4 0                 *----------------*
                           0 0 4 4]                |                |
                                                   |                |
      data.connectB0    = [0 1 2 3                 |                |
                           1 2 3 0]                |                |
                                                   |                |
      data.depth        = [0.5 0.5 0.5 0.5]        |                |
                                                   |                |
                                                   *----------------*
                                                  0                  1
      
   layers2d::mesher   my_mesher;
   my_mesher.settings.nlayers = 1;
   my_mesher.run(data);


   OUTPUT:
                                                  3                  2
      data.pos = [0  4  4  0  0.5  3.5 3.5  0.5    *----------------*
                  0  0  4  4  0.5  0.5 3.5  3.5]   | \------------/ |
                                                   | |7          6| |
      data.connectM = [0 1 2 3                     | |            | |
                       1 2 3 0                     | |            | |
                       5 6 7 4                     | |            | |
                       4 5 6 7]                    | |4          5| |
                                                   | /------------\ |
         (indices may vary)                        *----------------*
                                                  0                  1
   \endverbatim

   \note       Upon entry, the boundary edges of a given subdomain need to be oriented consistently in \p data.connectB0
               in the case of several subdomains, or when internal closed lines (such as holes) are prescribed.
               However, edges don't need to be consecutive in the matrix.
   \note       Upon exit, the elements are always oriented counter-clock wise, i.e. with the normal&nbsp;=&nbsp;Oz.

   \warning    The third coordinates in matrix \p data.pos is discarded for all points.
               That means that all points are projected onto the Z=0 plane (even for points
               not referenced in \p data.connectB0).
   */
class CM2LAYERS2D_API mesher
{
public:
   /**
      The settings of layers2d::mesher.

      This structure gathers flags and parameters to control the way the layers2d::mesher works.
      */
   struct CM2LAYERS2D_API settings_type
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
      Default constructor.

      The default values are:
         - \ref nlayers = 1 
         - \ref rho = 1.0
         - \ref min_Q4_quality = 0.5
         - \ref max_junction_angle = 45.
         - \ref free_gap_factor = 0.0
         - \ref march_pruning_flag = `false`
         - \ref display_hdl = `NULL`
         - \ref pass_thru = `NULL`
      */
   settings_type()
      : nlayers(1), rho(1.0), min_Q4_quality(0.5), max_junction_angle(45.), free_gap_factor(0.),
        march_pruning_flag(false),
        display_hdl(NULL), pass_thru(NULL)
   { }

   /**
      Returns true if the sanity check is successful.
      */
   bool
   valid() const        { return this->check() == 0; }

   /**
      Sanity check of the settings.

      \return        Returns a negative value in case of error.
                     Returned value = -k => the k-th field of the structure is illegal.
                     (\ref nlayers is the first field, \ref rho is the second...)
      */
   int
   check() const;

   /**
      Resets to the \ref cm2::layers2d::mesher::settings_type::settings_type() default values.
      */
   void
   reset();


   /**
      Number of layers to generate.
      */
   unsigned                   nlayers;

   /**
      Depth growth ratio between 2 consecutive rows.

         \f$ d_{n+1} = \rho d_{n} \f$
      where:
         \f$ d_{n} \f$ is the depth vector computed for the n-th layer (n = 0 .. \ref nlayers - 1)
         and defined at each boundary edge.

      \pre rho > 0.
      \see depth.
      */
   double                     rho;

   /**
      Angular quality above which a quad is considered good enough and kept in the final mesh.

      If the quality of a given quad is lower than this threshold, it will be split into 2 triangles.

      \note    For a quad, the _angular_ quality is given by the minimum angle quality at each corner
               (normalized to 1 for a perfect 90° angle).
      \note    A rectangular element has a quality of 1.0 and a degenerated element has a quality of 0.
      \note    An all-triangle layer mesh will be generated if this parameter has a value strictly greater than 1.

      \pre  min_Q4_quality > 0.

      Default = 0.5
      */
   double                     min_Q4_quality;

   /**
      Maximum node angle above which two adjacent layer’s elements will be disconnected.

      \verbatim                                                                  
                  _____________                           ____ __________   
                      |        |  /\                          |          /\
                      |        | /  \                         |         /  \
                  ____|________|/    \                    ____|_______ /    \
                              Ni\    /\                             Ni \    /\
                                 \  /  \                                \  /  \
                                  \/    \                                \/    \
                                   \                                      \
                                    \                                      \
      \endverbatim

      Default = 45 (degree).
      */
   double                     max_junction_angle;

   /**
      Scalar to control layers collision.

      When dealing with a multiple contour boundary (e.g. multiple airfoils)
      it is handy to prune elements in narrow regions between different contours.
      By setting a value greater than 0 for such contours this feature will be enabled
      and the mesher will detect and prevent locally the layer generation if the layers fronts would collide:
      for each node of the boundary, if a node from a different boundary contour is included in a box
      centred on the top node being constructed, both boundary nodes are invalidated and
      no layer elements will be generated on these nodes.
      The free_gap_factor sets the size of this control box: bi = free_gap_factor * di
      with:
      -   bi is the size of the bounding box at node Ni,
      -   di is the node depth at node Ni.

      Default = 0.
      */
   double                     free_gap_factor;

   /**
      Flag to prune layer elements.

      This flag tells the mesher to prune the layers to avoid
      jumps of more than one element between two adjacent elements.

      When set to true, fewer layer elements are generated.

      Default = false
      */
   bool                       march_pruning_flag;

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
      The pointer that will be passed to the \ref display_hdl
      (if any) when called by the mesher.

      Example:
      \code
         void my_display_hdl (void* pass_thru, unsigned level, const char* msg)
         {
            window_type*   my_window = static_cast<window_type*> (pass_thru);
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
      The data type for layers2d::mesher.

      This structure is used to gather all the input and output data of the member function 
      layers2d::mesher::run.
      */
   struct CM2LAYERS2D_API data_type
   {
   public:

   /// The display handler type.
   typedef settings_type::display_handler_type     display_handler_type;

   /**
      Warning enums for the layers2d %mesher.

      Non-fatal warnings.

      If such a warning is raised (returned in the field \ref warning_code),
      the %mesher is not stopped but the user shall check its input and output data for a potential mistake.
      */
   enum warning_type
   { 
      CM2_NO_WARNING                =   0,   //!< OK, no warning.
      CM2_ANGLE_QUALITY_WARNING     =  -1    //!< One or several elements have a bad quality.
   };

   /**
      Error enums for the layers2d %mesher.

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
      CM2_NODE_ERROR                = -104,  //!< Two nodes are too close to each other (same position or almost). 
      CM2_EDGE_ERROR                = -105,  //!< A hard edge is cut by another hard edge or has a hard node inside it.  
      CM2_BOUNDARY_ERROR            = -106,  //!< Illegal boundaries provided by the user (boundary not closed...) 
      CM2_SYSTEM_MEMORY_ERROR       = -199,  //!< Insufficient memory available.
      CM2_INTERNAL_ERROR            = -200   //!< Something went wrong but don't know what (please contact support@computing-objects.com).
   };

   /// Default constructor.
   data_type()                            { this->clear(); }

   /**
      Copy constructor (hard copy).
      All matrices are hard copied.
      */
   data_type (const data_type& cpy)       { (*this) = cpy; }



   /**
      Constructor.

      Constructs the data with a coordinates matrix, a boundary edges matrix (shallow copy) and a depth defined on each hard edge.

      This is the simplest way to create the data to exchange with the %mesher.

      \param[in]   P              The coordinates matrix. New nodes are appended by the %mesher.
      \param[in]   connectE2      The connectivity matrix for the boundary mesh.
      \param[in]   depth          The user defined depth defined on each hard edge.

      \note       After the meshing process, you must get the new coordinate matrix from the field \ref pos,
                  and the connectivity matrix \ref connectM as well, or use the \ref extract function.
      \warning    Upon exit, the matrix data.pos is resized to two rows. 
                  The third coordinate (i.e. Z) is lost for all points.
      */
   data_type (const DoubleMat& P, const UIntMat& connectE2, const DoubleVec& depth)
   {
      this->clear();
      this->pos = P;
      this->connectB0 = connectE2;
      this->depth = depth;
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
                     (\ref pos is the first field, \ref connectB0 is the second...)
      */
   int
   check() const;

   /**
      Shallow copy.

      \param[out]    P              A view to the \ref pos matrix.
      \param[out]    connectQ4T3    A view to the \ref connectM matrix.
                                    This matrix has dimension 4 x \ref nefs.
                                    The leading part of this matrix (from col 0 to col \ref nefs_Q4 - 1)
                                    is the connectivity of the quadrangle elements.
                                    The tailing part of this matrix (from col \ref nefs_Q4 to col \ref nefs - 1)
                                    is the connectivity of the triangles elements. 
                                    The fourth node ID in this triangle part of the matrix is always `CM2_NONE` (i.e. `unsigned(-1)`).
      */
   void
   extract (DoubleMat& P, UIntMat& connectQ4T3) const;

   /**
      Shallow copy.

      \param[out]    P              A view to the \ref pos matrix.
      \param[out]    connectQ4      A view to the leading part of \ref connectM, related to quadrangles.
                                    Same as connectM.sub_matrix(0, 0, 4, nefs_Q4)
                                    This matrix has dimension 4 x nefs_Q4.
      \param[out]    connectT3      A view to the tailing part of \ref connectM, related to triangles.
                                    Same as connectM.sub_matrix(0, nefs_Q4, 3, nefs_T3)
                                    This matrix has dimension 3 x nefs_T3.
      */
   void
   extract (DoubleMat& P, UIntMat& connectQ4, UIntMat& connectT3) const;

   /// Clears all fields and reinit to default values.
   void
   clear();

   /// Clears only the output fields.
   void
   clear_data_out();

   /**
      Prints information about the mesh using a display handler.

      \param   hdl   The display handler.

      \note    The first parameter of the handler (param `pass_thru`) is irrelevant (`NULL`).
      */
   void
   print_info (display_handler_type hdl) const;

   /**
      Saves the data into a file.

      \param[in]   filename       The file name for the output (overwrite mode).
      \param[in]   precision      The precision of the output for the floating-point values.
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

   // Checks if an error has been raised.
   inline bool
   error_raised() const       { return this->error_code != CM2_NO_ERROR; }

   /// Checks if a warning has been raised.
   inline bool
   warning_raised() const     { return this->warning_code != CM2_NO_WARNING; }


   /**
      The coordinates matrix (column oriented).

      Node `i` has coordinates in column `pos.col(i)`.

      Upon entry, this matrix must contains the coordinates of all the points (i.e. nodes referenced in 
      \ref connectB0). 

      It may also contain other unreferenced points.

      Upon exit, the new nodes are appended at the back of the matrix and the indices in the
      final mesh (\ref connectM) refer to column numbers in this matrix.

      Mode = IN-OUT.

      \warning    On exit, the matrix is resized to two rows. 
                  The third coordinate (Z) is lost for all points.
      */
   DoubleMat               pos;

   /**
      The hard edges (boundary edges + internal hard edges).

      This connectivity matrix must define a valid and closed CM2_EDGE2 mesh for the 
      external boundary of the domain to be meshed.

      The orientation of these boundary edges usually needs to be similar (either all CW or all CCW), 
      except when there is only a single external contour.
      
      Internal hard edges can be added to the boundary mesh. 

      In order to define holes inside the domain, the boundary mesh of the holes must be closed
      and oriented oppositely to the external boundary mesh.

      The indices refer to columns in the coordinates matrix \ref pos.

      Mode = IN.
      */
   UIntMat                 connectB0;

   /**
      The internal hard nodes.

      The indices refer to columns in the coordinates matrix \ref pos.

      Mode = IN.
      */
   UIntVec                 isolated_nodes;

   /**
      User-defined edges depth. 
      
      `depth[k]` is the depth for the k-th edge (k-th column in \ref connectB0).

      Set `depth[k] = 0` to prevent the generation of any layer above boundary element `k`.

      \note    If \p depth.size() > \p connectB0.cols(), extra depths are ignored.
      \note    If \p depth.size() < \p connectB0.cols(), missing depth are set to 0 
               (i.e. no layer will be generated above these edges).

      Mode = IN.
      */
   DoubleVec               depth;

   /**
      The connectivity of the generated elements (column oriented). 
      The indices refer to columns in the coordinates matrix \ref pos.

      The dimension of this matrix is 4 X \ref nefs.

      The leading part of this matrix (from col 0 to col \ref nefs_Q4 - 1)
      is the connectivity of the quadrangle elements.
      The tailing part of this matrix (from col \ref nefs_Q4 to col \ref nefs - 1)
      is the connectivity of the triangles elements. The fourth node ID in this part of the 
      matrix is always `CM2_NONE` (i.e. `unsigned(-1)`).

      \note    The number of rows is always equal to 4, even if the layer contains only triangles.
               In this case, the fourth row contains only `CM2_NONE` values.

      Mode = OUT.
      */
   UIntMat                 connectM;

   /**
      The connectivity of the final layers contour, i.e. intern contour (column oriented). 

      The indices refer to columns in the coordinates matrix \ref pos.

      The dimension of this matrix is 2xNEFS.

      \note Use connectB1 as input of \ref cm2::triamesh::mesher or \ref cm2::quadmesh::mesher
            to generate the rest of the mesh in the domain. 

      Mode = OUT.
      */
   UIntMat                 connectB1;

   /**
      Element sizes specified at nodes upon exit.

      The node indices refer to columns in the node coordinates matrix \ref pos.

      \warning       Not supported at present and may be removed in future versions.

      Mode = OUT.
      */
   DoubleVec               metrics;

   /**
      The angle qualities vector. 

      Mode = OUT.
      */ 
   DoubleVec               angle_qualities;

   /**
      The histogram of the element angle qualities.

      Mode = OUT.
      */ 
   cm2::misc::histogram    histo_Qa; 

   /**
      Number of elements in the final layer mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs;

   /**
      Number of quads in the final layer mesh. 

      nefs_Q4 = nefs in an all-quad mesh.
      nefs_Q4 = 0 in an all-triangle mesh (e.g. if settings_type::min_Q4_quality is strictly greater than 1.0).

      Mode = OUT.
      */ 
   unsigned                nefs_Q4;

   /**
      Number of triangles in the final mesh. 

      nefs_T3 = 0 with a pure quad mesh.
      nefs_T3 = nefs with an all-triangle mesh.

      Mode = OUT.
      */ 
   unsigned                nefs_T3;

   /**
      Number of nodes in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nods;

   /**
      Total number of nodes in the coordinates matrix.

      \post    Greater or equal to \ref nods.

      Mode = OUT.
      */ 
   unsigned                total_nods;

   /**
      Minimum angle quality of the elements in the final mesh. 

      Mode = OUT.
      */ 
   double                  Qmin;

   /**
      Total area of the domain.

      \note    This is not the area of the layers. \n
               To compute it use meshtools2d::area.

      Mode = OUT.
      */ 
   double                  area;
   
   /**
      Total time spent for the mesh. 

      Mode = OUT.
      */ 
   double                  total_time; 

   /**
      Number of elements per second. 

      Global speed including all steps of the meshing process.

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

   See \ref cm2::layers2d::mesher::settings_type::settings_type() default values.
*/
mesher();

/**
   Constructor with specific settings.

   \param[in]   settings_        The settings. 
*/
mesher (const settings_type& settings_);

/**
   Is the %mesher authorized to run?

   \see  cm2::layers2d::registration.
*/
static bool
authorized();

/**
   Main function of the layer %mesher.

   \param[in,out]   data   The data.
*/
void 
run (data_type& data);

/**
   The settings of the %mesher.

   See \ref cm2::layers2d::mesher::settings_type::settings_type() default values.
*/
settings_type         settings;
};

}  // end namespace layers2d
}  // end namespace cm2


/** 
   \example tmsh2db02.cpp
   This is an example of how to use the 2-D layer mesher cm2::layers2d::mesher on a non manifold contour.

   \image html tmsh2db02_q.png "Example tmsh2db02.cpp (pure quadrangle)"
*/

/** 
   \example tmsh2db06.cpp
   This is an example of how to use the 2-D layer mesher cm2::layers2d::mesher on an input boundary edge mesh
   showing layer pruning (to avoid collisions).

   \image html tmsh2db_t.png "Example tmsh2db06.cpp (pure triangle)"
*/

#endif

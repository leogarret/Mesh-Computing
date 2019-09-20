#ifndef __SURF_MESHER_Q4_H__
#define __SURF_MESHER_Q4_H__

/**
   \file       surf_mesher_q4.h
   \brief      Definition of the 3d-surface quad/quad-dominant mesher.
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
#include "misc.h"

// Forward declaration.
class TopoDS_Shape;


namespace cm2 {
namespace surf_mesh_q4 {

/**
   3-D surface all-quad or quad-dominant mesher.

   Parametric surface mesher (direct meshing) based on the OpenCascade kernel. 

   Runs on STEP and IGES files.

   \image html elephant_qt.png "Surface mesh generated by CM2 SurfMesh Q4. CAD model made of 57 surfaces (colorized)."

   \note    Meshing failures can be caused by pathological 
            surfaces (for instance with foldings and self-intersections).
            In these cases, check your model and fix.
   \note    At present, the mesh-size gradation control doesn't work between curves nor surfaces
            (only within each curve, and each surface).
   \note    Bad curve definition can cause accumulation points. 
            A post-meshing correction step may be needed (from simple node merging to local meshings).
   \note    Non-connected surfaces (for instance surfaces with gaps between them) give unconnected meshes
            (this tool doesn't fix these kinds of topological error).
   \warning Still experimental.
   */
class CM2SURFMESHQ4_API mesher
{
public:

   /**
      This generator is for triangle meshes.
      */
   enum
   {
      generated_FE_type = CM2_FACE_MIX          //!< Generates all-quad or quad-dominant meshes.
   };


   /**
      The settings of surf_mesh_q4::mesher.

      This structure gathers flags and parameters to control the way the %mesher works.
      */
   struct CM2SURFMESHQ4_API settings_type
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
         - \ref target_h = 0.0
         - \ref max_chordal_error = -0.02
         - \ref min_h = -0.001
         - \ref chordal_control_type = 4
         - \ref max_gradation = 0.5
         - \ref all_quad_flag = `false`
         - \ref force_even_flag = `false`
         - \ref optim_level = 3
         - \ref quadrangle_weight = 0.70
         - \ref min_Q4_angle_quality = 0.10
         - \ref display_hdl = `NULL`
         - \ref interrupt_hdl = `NULL`
         - \ref pass_thru = `NULL`

      The most useful parameters are:
         - \ref target_h to control the size of the elements in the mesh.
         - \ref max_chordal_error to control the chordal error between the mesh and the surface.
      */
   settings_type()
      : target_h(0.0), max_chordal_error(-0.02), min_h(-0.001),
        chordal_control_type(4), max_gradation(0.5), 
        all_quad_flag(false), force_even_flag(false), optim_level(3), 
        quadrangle_weight(0.70), min_Q4_angle_quality(0.10), 
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
                     (\p target_h is the first field, \p max_chordal_error is the second...)
      */
   int
   check() const;

   /**
      Resets to the \link cm2::surf_mesh_q4::mesher::settings_type::settings_type() default values \endlink.
      */
   void
   reset();

   /**
      The mesh size. 

      Default = 0.

      \pre        target_h >= 0.
      */
   double                  target_h;

   /**
      Maximum chordal error allowed.

      The mesh size will be reduced locally to limit the chordal error between the mesh and the surface:
         - If negative, this value is relative to the local radii (for instance -0.01 => max chordal error = 1% of local radii).
         - If positive, this value is absolute (for instance 0.1 => max chordal error = 0.1).

      With max_chordal_error = -0.05 (5%), a complete circle is meshed with approximatively 10 elements. \n
      With max_chordal_error = -0.03 (3%), a complete circle is meshed with approximatively 13 elements. \n
      With max_chordal_error = -0.02 (2%), a complete circle is meshed with approximatively 16 elements. \n
      With max_chordal_error = -0.01 (1%), a complete circle is meshed with approximatively 22 elements.

      Default = -0.02 (max error = 2% of the local radii).
      */
   double                  max_chordal_error;

   /**
      Minimum mesh size (security). 
                                          
      The mesh size will not be reduced (by the \ref max_chordal_error criterion) beyond this limit:
         - If negative, this value is relative to \ref target_h (min size = - min_h * \ref target_h).
         - If positive, this value is absolute.

      Default = -0.001 (1E-3 * \ref target_h).
      */
   double                  min_h;

   /**
      Chordal control type:
         -  0: No curvature is computed and the chordal error control is disabled.
         -  1: Approximate curvature (least-square method using the local bases). \n
               Isotropic chordal control: element sizes are limited by the same (isotropic) chordal value.
         -  2: Approximate curvature (least-square method using the local bases). \n
               Anisotropic chordal control: element sizes are limited by two different (anisotropic) chordal values
               along the two principal curvature directions.
         -  3: Exact curvature (provided by `Surface::get_local_curvatures`). \n
               Isotropic chordal control: element sizes are limited by the same (isotropic) chordal value.
         -  4: Exact curvature (provided by `Surface::get_local_curvatures`). \n
               Anisotropic chordal control: element sizes are limited by two different (anisotropic) chordal values
               along the two principal curvature directions.

      Default = 4
      */
   unsigned                chordal_control_type;

   /**
      Gradation of the elements size.

      This parameter controls the gradation of the elements size along the skeleton lines
      and inside the patches towards the \ref target_h mesh size.
      
      A value close to 0 leads to a more progressive variation of mesh size (smoother).

      \note    max_gradation >= 0

      Default = 0.5
      */
   double                  max_gradation;

   /**
      Flag to force the generation of a all-quad mesh (no triangle):
         - `false`, generates a mixed triangle-quad mesh.
         - `true`, generates an all-quad mesh.

      \note    A good all-quad mesh can only be generated in simple cases. 
               If possible, rather use the mixed quad-dominant generation mode.
      \note    Setting all_quad_flag to true, disables parameter \ref min_Q4_angle_quality 
               (i.e. same as \ref min_Q4_angle_quality = 0). 
      \note    When this flag is set to false, parameter \ref quadrangle_weight can be used to 
               control the trade-off between meshes with more numerous quads and 
               meshes with more numerous triangles but of better shape quality.

      Default = false.
      */
   bool                    all_quad_flag;

   /**
      Flag to force the number of edges along each skeleton line to be even.
      
      \note    All-quad meshing (\ref all_quad_flag = true) usually requires an even number 
               of edges along each curve.\n
               Hence, even if this flag is set to false, the mesher may consider only
               even-meshing of curves when all-quad meshing is requested.

      Default = false.
      */
   bool                    force_even_flag;

   /**
      Optimization level between 0 and 10.

      Controls the trade-off between speed (low values) and high element quality (high values). \n
      The higher the level, the more aggressive the algorithms.

      \note    Level 0 disables any optimization.

      Default = 3
      */
   unsigned                optim_level;

   /**
      Preference for quadrangles versus triangles.

      Scalar between 0 and 1 indicating the preference for quadrangles over triangles:
         - For quadrangle_weight = 0, quadrangles are never used.
         - For values between 0 and 1, quadrangles are more and more used, even if this lead to a lesser  
           quality of the mesh (a value of 0.5 give no advantage on quads vs triangles).
         - For quadrangle_weight = 1, the number of triangles is minimal (but may be not null).

      Default = 0.70 (serious preference for quads). Should not normally be changed.

      \note       This parameter is taken into account only in mixed mode (\ref all_quad_flag = false). 
      \note       This parameter is not the ratio between quads and triangles. 
                  Furthermore, there is no linearity between this weight and the ratio between quads and triangles. 
      */
   double                  quadrangle_weight;

   /**
      Minimum acceptable angle quality for the quadrangles.

      Scalar between 0 and 1 indicating the minimum acceptable angle quality for the quads.

      Default = 0.20 i.e. acceptable quads have angle between 18 deg (0.20*90) and 162 deg (180 - 0.20*90).
      Others are split into triangles.

      \note       This parameter is taken into account only in mixed mode (\ref all_quad_flag = false). 
      \note       This quality threshold is based on the _angle_ quality of the quads 
                  (not the _geometric_ quality which takes the length ratios also into account).
      \note       The angle quality is computed as the minimum of the four angles at summits.\n
                  The _angle_ quality of a rectangle equals to 1 (perfect) whereas its
                  _geometric_ quality is only equal to 1 when the rectangle is a square.
      \note       Set min_Q4_angle_quality = 1 to allow rectangles only (quads with right angles only).\n
                  In this case, be aware that very few rectangles may be generated (mostly triangles).
      */
   double                  min_Q4_angle_quality;

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

      When this function returns true, the process is stopped and the meshing aborts.

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
      The data type for surf_mesh_q4::mesher.

      This structure is used to gather all the input and output data of the function 
      surf_mesh_q4::mesher::run.
      */
   struct CM2SURFMESHQ4_API data_type
   {
   public:

   ///
   typedef settings_type::display_handler_type     display_handler_type;

   /**
      Warning enums for the mesher.

      Non-fatal warnings.

      If such a warning is raised (returned in the field \ref warning_code),
      the mesher is not stopped but the user shall check its input and output data for a potential mistake.
      */
   enum warning_type
   { 
      CM2_NO_WARNING                =   0,   //!< OK, no warning.
      CM2_INTERRUPTION              = -10,   //!< The process has been interrupted (by the user through the interrupt handler).
      CM2_FAILED_SURFACES_WARNING   = -11,   //!< One or several surfaces could not be meshed.
      CM2_SHAPE_QUALITY_WARNING     = -12    //!< One or several elements have a bad quality (Qs < 0.01).
   };

   /**
      Error enums for the mesher.
       
      Fatal errors.

      If such an error is raised (returned in the field \ref error_code),
      the mesher is stopped and no final mesh is returned.
      */
   enum error_type
   { 
      CM2_NO_ERROR                  =    0,  //!< OK, no error.
      CM2_LICENSE_ERROR             = -100,  //!< Invalid or expired license.
      CM2_MODE_ERROR                = -101,  //!< Error in settings.
      CM2_FILE_ERROR                = -102,  //!< Error in input file. Missing file or OpenCascade could not read it properly.
      CM2_DEGENERATED_ELEMENT       = -103,  //!< The process leads to some degenerated elements and therefore the mesher cannot build a valid mesh.
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
      Shallow copy.

      \param[out]    pos_        A view to the \ref pos matrix.
      \param[out]    connectQ4   A view to the leading part of \ref connectM related to quadrangles.
                                 Same as connectM.sub_matrix(0, 0, 4, nefs_Q4). \n
                                 This matrix has dimensions 4 x \ref nefs_Q4.
      \param[out]    connectT3   A view to the tailing part of \ref connectM related to triangles.
                                 Same as connectM.sub_matrix(0, nefs_Q4, 3, nefs_T3).\n
                                 This matrix has dimensions 3 x \ref nefs_T3.
      */
   void
   extract (DoubleMat& pos_, UIntMat& connectQ4, UIntMat& connectT3) const;

   /**
      Shallow copy.

      \param[out]    pos_        A view to the \ref pos matrix.
      \param[out]    connectM_   A view to the \ref connectM matrix. This matrix has dimensions 4 x \ref nefs. \n
                                 The leading part of this matrix (from col 0 to col \ref nefs_Q4 - 1)
                                 is the connectivity of the quadrangle elements.\n
                                 The tailing part of this matrix (from col \ref nefs_Q4 to col \ref nefs - 1)
                                 is the connectivity of the triangles elements.\n 
                                 The fourth node ID in this part of the matrix is always `CM2_NONE` (i.e. `unsigned(-1)`).\n
      */
   void
   extract (DoubleMat& pos_, UIntMat& connectM_) const;

   /// Clears all fields and reinit to default values.
   void
   clear();

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

   /// Checks if an error has been raised.
   inline bool
   error_raised() const       { return error_code != CM2_NO_ERROR; }

   /// Checks if a warning has been raised.
   inline bool
   warning_raised() const     { return warning_code != CM2_NO_WARNING; }


   /**
      The coordinates matrix (column oriented).

      Node `i` has coordinates in column `pos.col(i)`.

      The dimensions of this matrix are 3 x \ref nods.

      Mode = OUT.
      */
   DoubleMat               pos;

   /**
      The connectivity of the triangle mesh (column oriented). 

      The indices refer to columns in the coordinates matrix \ref pos.

      The dimensions of this matrix are 3 x \ref nefs.

      Mode = OUT.
      */
   UIntMat                 connectM;

   /**
      The coloring of the mesh.

      Each element is assigned a color (from 0 to \ref nbr_surfaces - 1) corresponding
      to the ID of its surface.

      The size of this array equals to the number of columns in \ref connectM.

      Mode = OUT.
      */
   UIntVec                 colors;

   /**
      The IDs of the failed surfaces. 

      Mode = OUT.
      */
   UIntMat                 failed;

   /**
      The histogram of the element shape qualities.

      Mode = OUT.
      */ 
   cm2::misc::histogram    histo_Qs; 

   /**
      Number of curves in the model. 

      Mode = OUT.
      */ 
   unsigned                nbr_curves;

   /**
      Number of surfaces in the model. 

      Mode = OUT.
      */ 
   unsigned                nbr_surfaces;

   /**
      Number of elements in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs;

   /**
      Number of quads in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs_Q4;

   /**
      Number of triangles in the final mesh.

      Mode = OUT.
      */ 
   unsigned                nefs_T3;

   /**
      Number of nodes in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nods;

   /**
      Area of the meshed domain (sum of the areas of all elements).

      Mode = OUT.
      */ 
   double                  area;

   /**
      Area of the quadrangles.

      Mode = OUT.
      */ 
   double                  area_Q4;

   /**
      Area of the triangles.

      Mode = OUT.
      */ 
   double                  area_T3;

   /**
      Minimum shape quality of the elements upon exit. 

      Mode = OUT.
      */ 
   double                  Qmin;

   /**
      Time spent in the OCC kernel for reading the model. 

      Mode = OUT.
      */ 
   double                  read_time;

   /**
      Time spent for meshing all the curves. 
    
      Mode = OUT.
      */ 
   double                  meshC_time;

   /**
      Time spent for meshing all the surfaces. 
    
      Mode = OUT.
      */ 
   double                  meshS_time;

   /**
      Time spent for meshing the CAD model (curves + surfaces). 
    
      Mode = OUT.
      */ 
   double                  mesh_time;

   /**
      Total time spent for the reading the model and meshing the model. 

      Mode = OUT.
      */ 
   double                  total_time; 

   /**
      Meshing speed, excluding the OCC-reading time (number of elements per second). 

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

   See \link cm2::surf_mesh_q4::mesher::settings_type::settings_type() default values \endlink.
   */
mesher();

/**
   Constructor with specific settings.
   
   \param[in]   settings_        The settings. 
   */
mesher (const settings_type& settings_);


/**
   Meshes an OpenCASCADE surface model.

   \param[in]    root      The TopoDS_Shape containing the CAD model.
   \param[out]   data      The data.
   */
void 
run (const TopoDS_Shape& root, data_type& data) const;

/**
   Meshes from a CAD file.

   \param[in]    filename  The IGES/STEP file containing the CAD model.
   \param[out]   data      The data.
   */
void 
run (const char* filename, data_type& data) const;


/**
   The settings of the %mesher.

   See \link cm2::surf_mesh_q4::mesher::settings_type::settings_type() default values \endlink.
   */
settings_type         settings;

};

}  // namespace surf_mesh_q4
}  // namespace cm2

#endif

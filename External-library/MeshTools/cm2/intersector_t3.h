/**
   \file       intersector_t3.h
   \brief      Definition of the surface triangle mesh intersector.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3548 $
   $Date: 2019-03-20 11:20:47 +0100 (mer., 20 mars 2019) $ 
   $Author: Damien LUCAS $   
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_INTERSECTOR_T3_H__
#define __CM2_INTERSECTOR_T3_H__


#include "math1.h"


namespace cm2 {
namespace intersect_t3 { 

/**
   Triangle-mesh intersector.

   This tool finds out all the face-face intersections/overlappings 
   and corrects them by splitting the intersecting faces.

   It may be needed prior to a 3-D meshing (by tetramesh::mesher) in order
   to fix all the intersection problems in a boundary (3-node triangles) mesh.

   Two faces are considered as intersecting each other when 
   their distance is within a tolerance parameter (field of settings_type).

   When two faces are intersecting each other, new nodes are created
   along the lowest face (lowest ID in the \ref data_type::connectM matrix) and these faces are split 
   using these new nodes. 

   \note       The new split faces may need a remeshing (see cm2::surf_remesh_t3::mesher)
               to improve elements quality.
   \note       Thread safe.
   \see        surf_remesh_t3::mesher, surf_remesh_q4::mesher, tetramesh::mesher.
   */
class CM2INTERSECT_T3_API mesher
{
public:
   /**
      The settings of intersect_t3::mesher.

      This structure gathers flags and parameters to control the way the intersect_t3::mesher works.
      */
   struct CM2INTERSECT_T3_API settings_type
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
         - \ref tolerance = 1E-12 
         - \ref fix_flag = `true`
         - \ref display_hdl = `NULL`
         - \ref interrupt_hdl = `NULL`
         - \ref pass_thru = `NULL`
      */
   settings_type()
      : tolerance(1E-12), fix_flag(true), 
        display_hdl(NULL), interrupt_hdl(NULL), pass_thru(NULL)
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
                     (\ref tolerance is the first field, \ref display_hdl is the second...)
      */
   int
   check() const;

   /**
      Resets to the \ref cm2::intersect_t3::mesher::settings_type::settings_type() default values.
      */
   void
   reset();

   /**
      Parameter used to merge nodes that are too close to each other,
      or nodes that are too close to another edge of face.
             
      Default = 1E-12
      */
   double                  tolerance;

   /**
      Fixing mode.

      Fixes pathologies when set to `true` (the default).

      Do not attempt to fix the pathologies when `false`. 
      Just dry-run and report the pathologies in arrays \ref data_type::pathology_IDs_in 
      (or \ref data_type::pathology_IDs_out).
             
      Default = true.
      */
   bool                    fix_flag;

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

      When this function returns true, the meshing is aborted.

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
      The data type for intersect_t3::mesher.

      This structure is used to gather all the input and output data of the member function 
      intersect_t3::mesher::run.
      */
   struct CM2INTERSECT_T3_API data_type
   {
   public:

   ///
   typedef settings_type::display_handler_type     display_handler_type;

   /**
      Warning enums for the intersect_t3 %mesher.

      Non-fatal warnings.

      If such a warning is raised (returned in the field \ref warning_code),
      the %mesher is not stopped but the user shall check its input and output data for a potential mistake.
      */
   enum warning_type
   { 
      CM2_NO_WARNING                =   0,   //!< OK, no warning.
      CM2_INTERRUPTION              = -10,   //!< The process has been interrupted (by the user through the interrupt handler).
      CM2_REMAINING_INTERSECTIONS   = -11    //!< Some intersection(s) remain.
   };

   /**
      Error enums for the intersect_t3 %mesher.

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

      Constructs the data with a coordinates matrix and a triangle mesh.

      This is the simplest way to create the data to exchange with the %mesher.

      \param[in]   P              The coordinates matrix. New nodes are appended by the %mesher.
      \param[in]   connectT3      The connectivity matrix of the surface triangle mesh.

      \note       After the meshing process, you must get the new coordinate matrix from the field \ref pos,
                  and the connectivity matrix \ref connectM as well, or use the \ref extract function.
      \note       Upon exit, the matrix data.pos is resized to three rows if needed
                  (with null Z-coordinate if the matrix had only two rows upon entry).
      */
   data_type (const DoubleMat& P, const UIntMat& connectT3)
   {
      this->clear();
      this->pos.shallow_copy(P);
      this->connectM.shallow_copy(connectT3);
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

      \return        A negative value in case of error. \n
                     Returned value = -k => the k-th field of the structure is illegal
                     (\ref pos is the first field, \ref connectM is the second...)
      */
   int
   check() const;

   /**
      Shallow copy.

      \param[out]    P              A view to the \ref pos matrix.
      \param[out]    connectT3      A view to the \ref connectM matrix.
                                    This matrix has dimension 3 x nefs.
      */
   void
   extract (DoubleMat& P, UIntMat& connectT3) const;

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
      \ref connectM). 

      It may also contain other unreferenced points.

      Upon exit, new nodes (if needed) are appended at the back of the matrix and the indices in the
      final mesh (\ref connectM) refer to column numbers in this matrix.

      Mode = IN-OUT.

      \warning    The matrix can have 2 or 3 rows (2-D or 3-D). 
                  This number of rows in not changed by the intersector.
      */
   DoubleMat               pos;

   /**
      The connectivity of the triangle mesh.

      \note    The mesh needs not to be continuous or in a single piece.
               It can contains several subdomains and can be non manifold.
      \note    The indices refer to columns in the coordinates matrix \ref pos.
      \note    Upon exit, the good faces (i.e. non intersecting) are relocated to the heading columns, 
               from column 0 to column \ref nefs_in_good (past-the-end).
      \note    The new split faces are appended to the back, from column \ref nefs_in_good 
               to column `connectM.cols()` (past-the-end).
      \note    The new split faces may need a remeshing (see cm2::surf_remesh_t3::mesher)
               to improve elements quality.

      Mode = IN-OUT.
      */
   UIntMat                 connectM;

   /**
      The face IDs intersecting other faces in the initial mesh.

      This vector gives the column IDs in the initial \ref connectM matrix of the faces that 
      intersect other faces(s).

      These faces are removed in the final mesh. 
      They are split into several new faces not intersecting any more.

      Mode = OUT.
      */
   UIntVec                 pathology_IDs_in;        

   /**
      The face IDs intersecting other faces in the final mesh.

      This vector gives the column IDs in the final \ref connectM matrix of the faces that 
      intersect other faces(s).

      Should be empty or `CM2_REMAINING_INTERSECTIONS` is raised.

      Mode = OUT.
      */
   UIntVec                 pathology_IDs_out;        

   /**
      The IDs of the parent face in the initial mesh.

      This vector gives the column IDs in the initial \ref connectM matrix of the face  
      from which the new face stems.

      \note    Non-intersecting faces are not modified but can be relocated to a different column of 
               the new \ref connectM matrix.
      \note    The "color" of a non-intersecting face is the column ID of this face in the initial mesh.
      \note    As intersecting faces are split into several sub-faces (children), colors are identical
               for all their children faces.\n
               Each new child is affected with the same "color", which is the column ID of the parent 
               face in the initial \ref connectM matrix.
      \note    This vector is useful to transfer properties from the initial mesh to the new mesh.

      Mode = OUT.
      */
   UIntVec                 colors;        

   /**
      The number of intersecting faces in the initial mesh.

      Same as pathology_IDs_in.size().

      Mode = OUT
      */
   unsigned                pathologies_in;

   /**
      The number of intersecting faces in the final mesh.

      Same as pathology_IDs_out.size().

      Should be zero or `CM2_REMAINING_INTERSECTIONS` is raised.

      Mode = OUT
      */
   unsigned                pathologies_out;

   /**
      The number of non-intersecting faces in the initial mesh.

      The good faces are the heading columns in \ref connectM.

      Mode = OUT
      */
   unsigned                nefs_in_good;

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
      Number of elements in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nefs;

   /**
      Number of nodes in the final mesh. 

      Mode = OUT.
      */ 
   unsigned                nods;

   /**
      Total number of points in the coordinates matrix (i.e. `pos.cols()`).

      \post Greater or equal to \ref nods.

      Mode = OUT.
      */ 
   unsigned                total_nods;

   /**
      Total time spent for the remeshing. 

      Mode = OUT.
      */ 
   double                  total_time; 

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

   See \ref cm2::intersect_t3::mesher::settings_type::settings_type() default values.
*/
mesher();

/**
   Constructor with specific settings.

   \param[in]   settings_        The settings. 
*/
mesher (const settings_type& settings_);

/**
   Is the %mesher authorized to run?

   \see  cm2::intersect_t3::registration.
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

   See \ref cm2::intersect_t3::mesher::settings_type::settings_type() default values.
*/
settings_type         settings;

};

}  // end namespace intersect_t3
}  // end namespace cm2


/** 
   \example tmsh3di01.cpp
    Example of two cylinders intersection each other fixed by cm2::intersect_t3::mesher.
 */

/** 
   \example tmsh3di50.cpp
    This is an example of how to use the intersector remesh cm2::intersect_t3::mesher to fix 
    intersection and overlaps between general meshes (and within each mesh).
 */


#endif

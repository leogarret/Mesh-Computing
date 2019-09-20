/**
   \file       meshtools3d_misc.h
   \brief      Declarations of some miscellaneous meshtools3d routines.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_MESHTOOLS3D_MISC_H__
#define __CM2_MESHTOOLS3D_MISC_H__


namespace cm2 {
namespace meshtools3d {


/****************************************************************************************/
/** @name Volume computation */
//@{

/**
   Computes the volume of a 3-D mesh.

   \param[in]     pos            The coordinates matrix (3-D).
   \param[in]     connectM       The connectivity matrix of the solid mesh
                                 (pure tetra, wedge, pyramid, hex, or mixed).
   \param[out]    v              The total volume of the mesh.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        pos.rows() == 3
   \pre        connectM.rows() == 4 (pure tetrahedral mesh) or connectM.rows() == 5 (pure pyramid mesh)
               connectM.rows() == 6 (pure wedge mesh) or connectM.rows() == 8 (pure hex mesh or mixed mesh)
               or connectM is empty.
   \pre        All nodes IDs referenced in connectM should be lesser than pos.cols().

   \note       The volume of warped elements (pyramids, wedges, hexes) is approximated. 
   \see        File fe_defs.h for local node numbering in elements.
   */
CM2MESHTOOLS3D_API int 
volume (const DoubleMat& pos, const UIntMat& connectM, double& v);                          
//@}


/****************************************************************************************/
/** @name Angle computation */
//@{

/**
   Computes the solid angles (min and max) at nodes of each element in a 3-D mesh.

   Unit for solid angles (if not normalized) is steradians (between 0 and 4*pi):
      - A rectangular parallelepiped have all its eight solid angles equal to pi/2.
      - An equilateral tetrahedron have all its four solid angles equal to 0.55128.
      - A perfect wedge have all its six solid angles equal to pi/3.
      - A perfect pyramid have its four base solid angles equal to 0.6797 and
        the solid angle at summit equal to 1.359.

   \param[in]     pos         The coordinates matrix (3-D).
   \param[in]     connectM    The connectivity matrix of the solid mesh
                                 (pure tetra, wedge, pyramid, hex, or mixed).
   \param[in]     normalized  Flag to normalize the angles.
                              1 becomes the value for a perfect solid angle.
   \param[out]    min_angles  min_angles[i] is the minimum solid angle in element i,
                              expressed in steradians if normalized=false.
   \param[out]    max_angles  max_angles[i] is the maximum solid angle in element i,
                              expressed in steradians if normalized=false.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        pos.rows() == 3
   \pre        connectM.rows() == 4 (pure tetrahedral mesh) or connectM.rows() == 5 (pure pyramid mesh)
               connectM.rows() == 6 (pure wedge mesh) or connectM.rows() == 8 (pure hex mesh or mixed mesh)
               or connectM is empty.
   \pre        All nodes IDs referenced in connectM should be lesser than pos.cols().

   \note       The computed angles for each element should always be strictly positive.
               Negative or null values indicate degenerated elements.
   \see        File fe_defs.h for local node numbering in elements.
   */
CM2MESHTOOLS3D_API int 
solid_angles 
   (const DoubleMat& pos, const UIntMat& connectM, bool normalized,
    DoubleVec& min_angles, DoubleVec& max_angles);

/**
   Computes the dihedral angles (min and max) at edges of each element in a 3-D mesh.

   Unit for dihedral angles (if not normalized) is radian (between 0 and 2*pi):
      - A rectangular parallelepiped have all its twelve dihedral angles equal to pi/2 (90 deg).
      - An equilateral tetrahedron have all its six dihedral angles equal to 1.2309 rad. (70.53 deg).
      - A perfect wedge have three of its dihedral angles equal to pi/3 (60 deg.), the other three
        equal to pi/2 (90 deg).
      - A perfect pyramid have four of its dihedral angles equal to 1.9106 (109.47 deg.), the other four
        equal to 0.9553 (54.74 deg).

   \param[in]     pos         The coordinates matrix (3-D).
   \param[in]     connectM    The connectivity matrix of the solid mesh
                                 (pure tetra, wedge, pyramid, hex, or mixed).
   \param[in]     normalized  Flag to normalize the angles.
                              1 becomes the value for a perfect dihedral angle.
   \param[out]    min_angles  min_angles[i] is the minimum dihedral angle in element i.
   \param[out]    max_angles  max_angles[i] is the maximum dihedral angle in element i.

   \return        Error code. Returns 0 when successful.
                  Returned value = -k => the k-th argument had an illegal value.

   \pre        pos.rows() == 3
   \pre        connectM.rows() == 4 (pure tetrahedral mesh) or connectM.rows() == 5 (pure pyramid mesh)
               connectM.rows() == 6 (pure wedge mesh) or connectM.rows() == 8 (pure hex mesh or mixed mesh)
               or connectM is empty.
   \pre        All nodes IDs referenced in connectM should be lesser than pos.cols().

   \note       The computed angles for each element should always be strictly positive.
               Negative or null values indicate degenerated elements.
   \see        File fe_defs.h for local node numbering in elements.
   */
CM2MESHTOOLS3D_API int 
dihedral_angles 
   (const DoubleMat& pos, const UIntMat& connectM, bool normalized,
    DoubleVec& min_angles, DoubleVec& max_angles);

//@}


}  // end namespace meshtools3d
}  // end namespace cm2

#endif

/**
   \file       node_detector.h
   \brief      Declarations of the cm2::meshtools::node_detector class.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_NODE_DETECTOR_H__
#define __CM2_NODE_DETECTOR_H__


namespace cm2 {
namespace meshtools {

/**
   Node detector.

   A node detector can be used to find the closest node to a point or the nodes inside a box.
   Internally, it uses a balanced 2-D or 3-D bisection tree to make the searches efficient.

   \warning       After construction or reinitialization, the number of rows in the coordinates matrix \p pos
                  must not be changed.
   \warning       Once nodes have been inserted in the detector (through construction, reinitialization or
                  simple insertion), their coordinates in matrix \p pos should not changed (but new coordinates
                  may be back-pushed).
   */
class CM2MESHTOOLS_API node_detector 
{
public:

/// Default constructor.
node_detector();

/**
   Constructor.

   Inserts a batch of points.
   Node IDs range from 0 to pos.cols() - 1

   \param[in]     pos         The coordinates matrix (2 or 3 rows).

   \warning       After construction, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After construction, the coordinates in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
node_detector (const DoubleMat& pos);

/**
   Constructor.

   Inserts a batch of points.
   Inserts only the nodes given by in vector \p indices.

   \param[in]     pos         The coordinates matrix (2 or 3 rows).
   \param[in]     indices     The indices of the points to be inserted in the tree.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= pos.cols())
                  are simply discarded.
   \warning       After construction, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After construction, the coordinates of the points in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
node_detector (const DoubleMat& pos, const UIntVec& indices);

/**
   Constructor.

   Inserts the nodes of a mesh into the tree.
   Inserts only the nodes given by in matrix \p connect.

   \param[in]     pos         The coordinates matrix (2 or 3 rows).
   \param[in]     connect     The connectivity matrix of the mesh.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= pos.cols())
                  are simply discarded.
   \warning       After construction, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After construction, the coordinates of the nodes in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
node_detector (const DoubleMat& pos, const UIntMat& connect);

/**
   Constructor.

   Inserts the nodes in the array of matrices given by \p connect_beg to \p connect_end (excluded).


   \param[in]     pos            The coordinates matrix (2 or 3 rows).
   \param[in]     connect_beg    Pointer to the first connectivity matrix (beginning of the array).
   \param[in]     connect_end    Pointer to the past-the-end connectivity matrix.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= pos.cols())
                  are simply discarded.
   \warning       After construction, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After construction, the coordinates of the nodes in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
node_detector (const DoubleMat& pos, const UIntMat* connect_beg, const UIntMat* connect_end);

/// Copy constructor (hard copy).
node_detector (const node_detector& x);

/// Destructor.
~node_detector();


/// Copy operator (hard copy).
node_detector&
operator= (const node_detector& x);


/// Clears the bisection tree.
void
clear();

/// Returns the number of nodes in the tree.
unsigned
size() const;


/**
   Clears the bisection tree and reinitializes it with new points.

   Clears and inserts all nodes of matrix \p pos.
   Node IDs range from 0 to \p pos.cols() - 1

   \param[in]     pos      The coordinates matrix (2 or 3 rows).

   \return        Negative error code (when pos.rows() != 2 and != 3).
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= \p pos.cols())
                  are simply discarded.
   \warning       After reinitialization, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After reinitialization, the coordinates in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
int        
reinit (const DoubleMat& pos);

/**
   Clears the bisection tree and reinitializes it with new points.

   Clears and inserts all nodes in vector \p indices.


   \param[in]     pos            The coordinates matrix (2 or 3 rows).
   \param[in]     indices        The index of the points to insert into the tree.
   \return        Negative error code (when pos.rows() != 2 and != 3).
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= \p pos.cols())
                  are simply discarded.
   \warning       After reinitialization, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After reinitialization, the coordinates of the points in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
int        
reinit (const DoubleMat& pos, const UIntVec& indices);

/**
   Clears the bisection tree and reinitialize it with new nodes.

   Clears the detector and insert all nodes in matrix \p connect.

   \param[in]     pos            The coordinates matrix (3 rows).
   \param[in]     connect        The nodes of the mesh to insert into the tree.
   \return        Negative error code (when pos.rows() != 2 and != 3).
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= \p pos.cols())
                  are simply discarded.
   \warning       After reinitialization, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After reinitialization, the coordinates of the nodes in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
int        
reinit (const DoubleMat& pos, const UIntMat& connect);

/**
   Clears the bisection tree and reinitializes it with new points.

   Clears and inserts all nodes in matrices between \p connect_beg and \p connect_end.

   \param[in]     pos            The coordinates matrix (3 rows).
   \param[in]     connect_beg    The nodes of the first mesh to insert into the tree.
   \param[in]     connect_end    The nodes of the past-the-end mesh to insert into the tree.
   \return        Negative error code (when pos.rows() != 2 and != 3).
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= \p pos.cols())
                  are simply discarded.
   \warning       After reinitialization, the address of the coordinates matrix \p pos
                  must not be changed.
   \warning       After reinitialization, the coordinates of the nodes in matrix \p pos should not changed 
                  (but new coordinates may be back-pushed).
   */
int        
reinit (const DoubleMat& pos, const UIntMat* connect_beg, const UIntMat* connect_end);

/**
   Inserts new nodes in the bisection tree.

   Inserts the nodes given in vector \p indices.

   \param[in]     indices        The indices of the nodes.
   \return        The number of successfully inserted nodes.
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= \p pos.cols())
                  are simply discarded.
   \warning       After insertion, the coordinates of the nodes in matrix \p pos must not changed.
   */
unsigned       
insert (const UIntVec& indices);

/**
   Inserts new nodes in the bisection tree.
   Inserts the nodes given in the \p connect matrix.

   \param[in]     connect        The connectivity matrix.
   \return        The number of successfully inserted nodes.
                  Returned value = -k => the k-th argument had an illegal value.

   \note          The indices referring to illegal columns in matrix \p pos (i.e. >= pos.cols())
                  are simply discarded.
   \warning       After insertion, the coordinates of the nodes in matrix \p pos must not changed.
   */
unsigned        
insert (const UIntMat& connect);

/**
   Returns the closest node to a given point.

   Returns the ID of the closest recorded node to point P.

   \param[in]     P              The point for which the closest node is to be searched.
   \return        The index of the closest node (CM2_NONE if the detector is empty).
                  Returned value = -k => the k-th argument had an illegal value.

   \note          If the detector has been initialized with a 3-D coordinate matrix, find
                  the closest point to {P[0], P[1], 0}.
   */
unsigned
find_closest (const DoubleVec2& P) const;

/**
   Returns the closest node to a given point.

   Returns the ID of the closest recorded node to point P.

   \param[in]     P              The point for which the closest node is to be searched.
   \return        The index of the closest node (CM2_NONE if the detector is empty).
                  Returned value = -k => the k-th argument had an illegal value.

   \note          If the detector has been initialized with a 2-D coordinate matrix, find
                  the closest point to {P[0], P[1]}, i.e. the third coordinate of P is discarded.
   */
unsigned
find_closest (const DoubleVec3& P) const;

/**
   Returns the closest node to a given node.

   Returns the ID of the closest recorded node to a specific node.

   \param[in]     N              The ID of the node for which the closest node is to be searched.

   \return  The index of the closest node different from N 
            (CM2_NONE if the detector is empty or if N is not recorded in the detector).
   */
unsigned
find_closest (unsigned N) const;


/**
   Returns the nodes located in a given 2-D box.

   Push back into vector \p nodes the IDs of the recorded nodes found strictly inside a specific box.
   The box is oriented as XY and is defined by the \p MinBox and \p MaxBox coordinates.

   \param[in]     MinBox         The min summit of the box.
   \param[in]     MaxBox         The max summit of the box.
   \param[in,out] nodes          The vector in which the found nodes are appended.

   \note          If the detector has been initialized with a 3d-coordinate matrix, 
                  zero is used for the third coordinate of MinBox and MaxBox.
   */
void
find_in_box (const DoubleVec2& MinBox, const DoubleVec2& MaxBox, UIntVec& nodes) const;

/**
   Returns the nodes located in a given 3-D box.

   Push back into vector \p nodes the IDs of the recorded nodes found strictly inside a specific box.
   The box is oriented as XYZ and is defined by the \p MinBox and \p MaxBox coordinates.

   \param[in]     MinBox         The min summit of the box.
   \param[in]     MaxBox         The max summit of the box.
   \param[in,out] nodes          The vector in which the found nodes are appended.

   \note          If the detector has been initialized with a 2d-coordinate matrix, 
                  the third coordinates of MinBox and MaxBox are discarded.
   */
void
find_in_box (const DoubleVec3& MinBox, const DoubleVec3& MaxBox, UIntVec& nodes) const;


/*------------------------ Protected members -----------------------*/
private:
// Private use only.
void*           _t2;
void*           _t3;

};


}  // end namespace meshtools
}  // end namespace cm2

#endif

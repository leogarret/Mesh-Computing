/**
   \file       fe_defs.h
   \brief      Enums for FE types and definition of the local nodes and boundaries.
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_FE_DEFS_H__
#define __CM2_FE_DEFS_H__


/** 
   \page    fe_geo_page Finite Element Geometries

   Nodes are locally numbered as follow:

   \verbatim


             0             1                            0      2      1
             *-------------*                            *------*------*

                CM2_EDGE2                                  CM2_EDGE3
   



   
             1                                          1
             *                                          *
             | \                                        | \
             |   \                                      |   \
             |     \  E2                                |     \
          E0 |       \                                 3*       *5
             |         \                                |         \
             |           \                              |           \
             |             \                            |             \
             *---------------*                          *-------*-------*
             2        E1      0                         2       4        0   

                 CM2_FACET3                                CM2_FACET6


             
             

             3      E2       2                         3       6       2
             *---------------*                         *-------*-------*
             |               |                         |               |
             |               |                         |               |
             |               |                         |               |
          E3 |               | E1                     7*       *8      *5
             |               |                         |               |
             |               |                         |               |
             |               |                         |               |
             *---------------*                         *-------*-------*
             0      E0       1                         0       4       1   

                CM2_FACEQ4                                CM2_FACEQ9

            
             
             
                   
                   |                                           | 
                   |                                           |
                   *1                                          *1
                  /| \                                        /|  \
                  /|   \                                      /|    *6
                   |     \                                     *8     \
                 / |3      \ 0                              4* |        \
                   *---------*---                              *3---*7---*0---  
                / /      _ -                                / /      _ -
                 /   _ -                                     *9  _ *5
               //  -                                       //  -
               *2                                          *2
              /                                           /
             /                                           /
                 CM2_TETRA4                                  CM2_TETRA10

                 F0 = {1 2 3}
                 F1 = {2 0 3}
                 F2 = {1 3 0}
                 F3 = {2 1 0} 



                         W                                                  W                                                   W                              
                                                                                                        
                         ^                                                  ^                                                   ^    
                         |                                                  |                                                   |
                         *4                                                 *4                                                  *4
                       / |\ \                                             / |\ \                                              / |\ \
                      /  |     \                                         /  |     \                                          /  |     \
                     / / | \      \                                     8 / | \      \  7                                   8 / | \      \  7
                   /     |           \                                /  *  |           *                                 /  *  |           *
                  /  /   |  \           \                            /  /   |  \           \                             /  /   |  \           \
                 /       |           V     \                      5 /       |   6       V     \                       5 /       |   6       V     \
               /    /    |   \     /          \                   *    /    |   *  11 /          \                    *    /    |   *  11 /          \
              /   3*-----|-------/--------------*2               /   3*-----|-------*--------------*2                /   3*-----|-------*--------------*2
             /    /      |    \                /                /    /      |    \                /                 /    /      |    \                /
           /    /        |   /               /                /    /        |                   /                 /    /        |   /               /
          /   /          | /   \           /                 /12 /          |     \        10 /                  /12 /        13| /   \        10 /
         /  /            o --------------/------> U         /  *            o --------------*------>  U         /  *            * --------------*------>  U
       /  /            /        \      /                  /  /            /        \      /                   /  /            /        \      /
      / /            /               /                   / /            /               /                    / /            /               /
     //            /             \ /                    //            /             \ /                     //            /             \ /
    *------------/---------------*                     *------------*---------------*                      *------------*---------------* 
    0          /                 1                     0          / 9               1                      0          / 9               1
             /                                                  /                                                   /
                  CM2_PYRAMID5                                       CM2_PYRAMID13                                      CM2_PYRAMID14

                  FT0 = {0 3 4}  
                  FT1 = {1 0 4}  
                  FT2 = {2 1 4}  
                  FT3 = {3 2 4}  

                  FQ  = {0 1 2 3}  





                       1*                                           1*                                           1*                      
                  |    /|\                                     |    /|\                                     |    /|\
                  |   / |  \                                   |   / |  \                                   |   / |  \
                  |  /  |    \                                 |  /  |    \                                 |  /  |    \
                  | /   |      \                               | /  9*      *11                             | /  9*      *11
                  |/    |        \                             |/    |        \                             |/    |        \
                  /     |          \                          7*     |          \                          7*     |          \
                 /|     |            \                        /|\    |            \                        /|\    |            \
                / |    2*-------------*0                     / |  \ 2*------*------*0                     / |  \ 2*------*------*0
               /  |    /             /                      /  |    \      10     /                      /  |    \      10     /
              /   |   /             /                      /   |   /  \          /                      / 15*   /  *17        /
             /    |  /             /                      /    |  /     \       /                      /    |  /     \       /
           4*     | /             /                     4*     | /        \    /                     4*     | /        \    /
            |\    |/             /                       |\    |/           \6/                       |\    |/           \6/
            |  \  o-------------/-----                   |  \ 8*-------------*-----                   |  \ 8*------*------*-----
            |    \             /                         |    \             /                         |    \       16    /
            |   /  \          /                        12*   /  *14        /                        12*   /  *14        /
            |  /     \       /                           |  /     \       /                           |  /     \       /
            | /        \    /                            | /        \    /                            | /        \    /
            |/           \ /                             |/           \ /                             |/           \ /
           5*-------------*3                            5*------*------*3                            5*------*------*3
           /                                            /       13                                   /       13
          /                                            /                                            /
               CM2_WEDGE6                                 CM2_WEDGE15                                  CM2_WEDGE18 

               F0 = {0 1 2}
               F1 = {3 5 4} 
               F2 = {2 1 4 5}
               F3 = {5 3 0 2}
               F4 = {0 3 4 1}






                       |                                           |                            
                       |                                           |  10                                          10             
                 3*----|------------*2                       3*----|---*--------*2                       3*--------*--------*2   
                 /|    |           /|                        /|    |           /|                        /|       /|       /|    
                / |    |          / |                       / |    |          / |                       / |      / |      / | 
               /  |    |         /  |                      /  |    |         /  |                      /  |     /  |     /  |    
              /   |    -        /   |                   19*   |    -        *18 |                   19*------23*--------*18 |  
             /    |    |       /    |                    /  11*    |       /    *9                   /| 11*---/--24*---/|- -*9   
            /     |    |      /     |                   /     |    |      /     |                   / |  /|  /     |  / |  /|    
           /      |    |     /      |                  /      |    |     /      |                  /  | / | /      | /  | / |    
         7*-----------------*6      |                7*--------*14------*6      |                7*--------*14------*6  |/  |    
          |       |    o ---|----|--------            |       |    o ---|----|--------            | 20*   || 26*   ||   *21 |    
          |      0* --/-----|-------*1                |      0* --/---8*|-------*1                |  /|  0*|------8*|--/|---*1   
          |      /   /      |      /                  |      /   /      |      /                  | / |  / |      / | / |  /
          |     /   /       |     /                   |     /   /       |     /                   |/  | /  |     /  |/  | /
          |    /   -        |    /                  15*    /   -        *13  /                  15*------25*--------*13 |/
          |   /   /         |   /                     |   *   /         |   *17                   | 16*----|-22* ---|---*17
          |  /   /          |  /                      |  /   /          |  /                      |  /     |  /     |  /
          | /   /           | /                       | /   /           | /                       | /      | /      | /
          |/   /            |/                        |/   /            |/                        |/       |/       |/
         4*-----------------*5                       4*--------*--------*5                       4*--------*--------*5
                                                               12                                          12
                CM2_HEXA8                                  CM2_HEXA20                                 CM2_HEXA27
   
                F0 = {0 3 7 4}
                F1 = {1 5 6 2}
                F2 = {0 4 5 1}
                F3 = {3 2 6 7}
                F4 = {0 1 2 3}
                F5 = {7 6 5 4}

   \endverbatim

   In the connectivity matrices, the local nodes are stored column-wise.

   \see  cm2::element_type.
*/


/**
   \namespace cm2
   \brief Main namespace for CM2.
   */
namespace cm2 {

/** 
   \defgroup FEtypes CM2 Finite Elements 
   @{
   @}
   */

/**
   \ingroup FEtypes
   Enum to specify the type of element.
   \see  \ref fe_geo_page.
   */ 
enum element_type 
{ 
   CM2_UNKNOWN_ELEMENT_TYPE = 0,         //!< UNKNOWN TYPE (ILLEGAL).

   CM2_NODE      = 1,                    //!< NODE.

   CM2_EDGE_MIX  = 2,                    //!< ANY EDGE ELEMENT. 
   CM2_EDGE2     = 3,                    //!< 2-NODE EDGE (LINEAR GEOMETRY).
   CM2_EDGE3     = 4,                    //!< 3-NODE EDGE (QUADRATIC GEOMETRY).

   CM2_FACE_MIX  = 5,                    //!< ANY LINEAR FACE ELEMENT.
   CM2_FACET3    = 6,                    //!< 3-NODE TRIANGLE (LINEAR GEOMETRY).
   CM2_FACEQ4    = 7,                    //!< 4-NODE QUADRANGLE (LINEAR GEOMETRY).
   CM2_FACET6    = 8,                    //!< 6-NODE TRIANGLE (QUADRATIC GEOMETRY).
   CM2_FACEQ8    = 9,                    //!< 8-NODE QUADRANGLE (SERENDIPITY, NOT USED IN CM2 FEM).
   CM2_FACEQ9    = 10,                   //!< 9-NODE QUADRANGLE (QUADRATIC GEOMETRY).

   CM2_SOLID_MIX = 11,                   //!< ANY LINEAR SOLID ELEMENT.
   CM2_TETRA4    = 12,                   //!< 4-NODE TETRAHEDRON (LINEAR GEOMETRY).
   CM2_TETRA10   = 13,                   //!< 10-NODE TETRAHEDRON (QUADRATIC GEOMETRY).
   CM2_PYRAMID5  = 14,                   //!< 5-NODE PYRAMID (LINEAR GEOMETRY).
   CM2_PYRAMID13 = 15,                   //!< 13-NODE PYRAMID (SERENDIPITY, NOT USED IN CM2 FEM).
   CM2_PYRAMID14 = 16,                   //!< 14-NODE PYRAMID (QUADRATIC GEOMETRY).
   CM2_WEDGE6    = 17,                   //!< 6-NODE WEDGE (LINEAR GEOMETRY).
   CM2_WEDGE15   = 18,                   //!< 15-NODE WEDGE (SERENDIPITY, NOT USED IN CM2 FEM).
   CM2_WEDGE18   = 19,                   //!< 18-NODE WEDGE (QUADRATIC GEOMETRY).
   CM2_HEXA8     = 20,                   //!< 8-NODE HEXAHEDRON (LINEAR GEOMETRY).
   CM2_HEXA20    = 21,                   //!< 20-NODE HEXAHEDRON (SERENDIPITY, NOT USED IN CM2 FEM).
   CM2_HEXA27    = 22                    //!< 27-NODE HEXAHEDRON (QUADRATIC GEOMETRY).
};

}     // namespace cm2

#endif

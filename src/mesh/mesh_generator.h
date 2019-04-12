#pragma once

#include "mesh/mesh.h"


struct MeshGenerator {
    /*!
       Creates a solid cube with a center at the beginning of the coordinates and a long edge equal to 1

                   21-------22
                   /       /
                  /       /
                20-------23
            9    6-------5    14
           /|    |       |    /|
          / |    | Y^  Z^|   / |
        10  | 1--|--|-2/ | 13  |
         |  8 |  7--|-|--4  | 15
         | /  |     |/|     | /
         |/   |     O-|-->X |/
        11    0-------3    12
             16-------19
             /       /
            /       /
          17------18
    */
    static Mesh CreateSolidCube();
    /*!
        Creates a sphere with a center at the beginning of coordinates and a diameter equal to one
        cntVertexCircle - Number of vertices in the circle
    */
    static Mesh CreateSolidSphere(uint16_t cntVertexCircle);
};

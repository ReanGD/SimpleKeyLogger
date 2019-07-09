#pragma once

#include <memory>

#include "engine/mesh/geometry.h"
#include "engine/common/noncopyable.h"


struct GeometryGenerator : Noncopyable {
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
    static std::shared_ptr<Geometry> CreateSolidCube();
    /*!
        Creates a sphere with a center at the beginning of coordinates and a diameter equal to 1
        cntVertexCircle - Number of vertices in the circle
    */
    static std::shared_ptr<Geometry> CreateSolidSphere(uint16_t cntVertexCircle);
    /*!
        Creates a square plane at the beginning of coordinates with an edge side equal to 1
        The plane is located in the X0Z plane, the normatives are directed along the OY

        cntXSides - Number of vertices on 0X axis (cntXSides >= 2)
        cntZSides - Number of vertices on 0Z axis (cntZSides >= 2)
    */
    static std::shared_ptr<Geometry> CreateSolidPlane(uint32_t cntXSides = 2, uint32_t cntZSides = 2, float scaleTextureX = 1.0f, float scaleTextureZ = 1.0f);
};

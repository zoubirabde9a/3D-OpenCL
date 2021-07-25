#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "entity.h"


struct Transform
{
    V3 Position;
    float Scale;
    V3 Rotation;

    V3 Velocity;
    V3 RotationSpeed;
    float ScalDiff;

    std::vector < Vertex3D >* Vertices;
};

#endif // TRANSFORM_H


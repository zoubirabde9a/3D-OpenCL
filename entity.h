#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

struct V3
{
    float x, y, z;

    V3 operator*(float A)
    {
        V3 result = {A * x, A * y, A * z};
        return result;
    }

    V3 operator*(V3 A)
    {
        V3 result = {A.x * x, A.y * y, A.z * z};
        return result;
    }

    V3 operator+(V3 A)
    {
        V3 result = {A.x + x, A.y + y, A.z + z};
        return result;
    }
    V3 operator-(V3 A)
    {
        V3 result = {x - A.x, y - A.y, z - A.z};
        return result;
    }

};

struct V2
{
    float x, y;
};

struct Vertex3D
{
    V3 Position;
    uint32_t Color;
    V2 Uv;
    V3 Normal;
};


#endif // ENTITY_H

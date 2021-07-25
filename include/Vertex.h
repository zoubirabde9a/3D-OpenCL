#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

namespace ZEngine {

    struct Position {
        float x;
        float y;
    };

    struct ColorRGBA8 {
        ColorRGBA8() : r(0), g(0), b(0), a(0) { }
		ColorRGBA8 operator+(ColorRGBA8 color) {
			ColorRGBA8 newColor(r + color.r, g + color.g, b + color.b, a + color.a);
			return newColor;
		}
        ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
            r(R), g(G), b(B), a(A) { }
        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    };
const ColorRGBA8 Black(0, 0, 0, 255);
const ColorRGBA8 White(255, 255, 255, 255);
const ColorRGBA8 Green(0, 255, 0, 255);
const ColorRGBA8 Red(255, 0, 0, 255);
const ColorRGBA8 Blue(0, 0, 255, 255);
const ColorRGBA8 Yellow(255, 255, 0, 255);
const ColorRGBA8 Orange(255, 165, 0, 255);
const ColorRGBA8 Gray(128, 128, 128, 255);

    struct UV {
        float u;
        float v;
    };

    //The vertex definition
    struct Vertex {
        //This is the position struct. When you store a struct or class
        //inside of another struct or class, it is called composition. This is
        //layed out exactly the same in memory as if we had a float position[2],
        //but doing it this way makes more sense.
        Position position;

        //4 bytes for r g b a color.
        ColorRGBA8 color;

        //UV texture coordinates.
        UV uv;
		float tid;
        void setPosition(float x, float y) {
            position.x = x;
            position.y = y;
        }

        void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }

        void setUV(float u, float v) {
            uv.u = u;
            uv.v = v;
        }
    };
}
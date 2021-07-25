#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <GL/glew.h>
#include <string>
#include <glm\glm.hpp>

namespace ZEngine {
    class GLTexture {
    public:
        static const int LINEAR = GL_LINEAR;
        static const int NEAREST = GL_NEAREST;
        static const int LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR;
        static const int LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST;
        static const int NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST;
        static const int NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR;
        static const int CLAMP = GL_CLAMP;
        static const int CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
        static const int REPEAT = GL_REPEAT;
        static const int DEFAULT_FILTER = NEAREST;
        static const int DEFAULT_WRAP = REPEAT;
    public:
        std::string filePath;
        GLuint id;
        int width;
        int height;
    };
    struct Texture {
        const GLTexture* texture;
        glm::vec4 uvs;
    };
}

#endif // GLTEXTURE_H


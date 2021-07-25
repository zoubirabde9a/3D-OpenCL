
#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <string>
#include <GL/glew.h>
#include <qstring.h>

namespace ZEngine {

    //This class handles the compilation, linking, and usage of a GLSL shader program.
    //Reference: http://www.opengl.org/wiki/Shader_Compilation
    class GLSLProgram
    {
    public:
        GLSLProgram();
        ~GLSLProgram();

        void compileShaders(const QString& vertexShaderFilePath, const QString& fragmentShaderFilePath);
		void compileShadersFromSource(const char* vertexSource,const char* fragmentSource);
        void linkShaders();

        void addAttribute(const std::string& attributeName);

        GLint getUniformLocation(const std::string& uniformName);

        void use();
        void unuse();
		void destroy();
    private:

        int _numAttributes;

        void compileShader(const char* source,const std::string &name, GLuint id);

        GLuint _programID;

        GLuint _vertexShaderID;
        GLuint _fragmentShaderID;
    };

}
#endif // GLSLPROGRAM_H

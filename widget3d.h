#ifndef WIDGET3DD_H
#define WIDGET3DD_H
#include <GLSLProgram.h>
#include <gl/glew.h>
#include <QtOpenGL/QGLWidget>
#include <QEvent>
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <entity.h>
#include <transform.h>

struct vec3
{
    float X, Y, Z;
};

class Widget3D : public QGLWidget
{
public:
Widget3D(QWidget* parent = 0);
    inline void setUpdateMethodGpu(boolean b) {updateMethodGpu = b;}

protected:
    void initializeGL()override;
    void paintGL()override;
    bool event(QEvent *event);



private:
    void updateWidget();
private:

    int m_frames = 0;

    boolean updateMethodGpu = true;

    int m_texture;
    std::vector < Vertex3D > treeModel;
    std::vector < Vertex3D > humanModel;
    std::vector < Vertex3D > cubeModel;
    std::vector < Transform > objects;
    std::vector < Transform > particles;

    float offsetX;
    float offsetY;
    float offsetZ;

    float rotateX;
    float rotateY;
    float rotateZ;

    float scale = 0.01;


    glm::vec3 cameraPosition;
    float cameraDistance;
    float angleAroundPosition;
    float pitch;

    glm::vec3 Forward;
    glm::vec3 Up;

    int BoxCount = 200;
    glm::vec3 *positions;

    ZEngine::GLSLProgram m_program;
    GLuint m_vbo;
    GLuint m_vao;

};

#endif // WIDGET3DD_H

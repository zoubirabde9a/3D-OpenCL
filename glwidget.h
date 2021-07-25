#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <gl/glew.h>
#include <QtOpenGL/QGLWidget>
#include <SpriteBatch.h>
#include <Camera2D.h>
#include <GLSLProgram.h>
#include <QEvent>
#include <QKeyEvent>
class GLWidget : public QGLWidget
{
public:
GLWidget(QWidget* parent = 0);
protected:
    void initializeGL()override;
    void paintGL()override;
    bool event(QEvent *event)
    {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            if (ke->key() == Qt::Key_Tab) {
                return true;
            }
        }
        return QWidget::event(event);
    }
private:
    void updateWidget();
private:
    ZEngine::GLSLProgram m_program;
    ZEngine::SpriteBatch m_spriteBatch;
    ZEngine::Camera2D m_camera;
    GLuint m_texture;
    GLuint m_texture2;
};

#endif // GLWIDGET_H

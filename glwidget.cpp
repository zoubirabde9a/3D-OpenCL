#include "glwidget.h"
#include "picoPng.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <qfile.h>
#include <qtextstream.h>
#include <qtimer.h>


using namespace ZEngine;
GLWidget::GLWidget(QWidget* parent) : QGLWidget(parent){
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}
void GLWidget::initializeGL(){
    /*glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_program.compileShaders(":/vertexShader.vert", ":/fragmentShader.frag");
    m_program.addAttribute("vertexPosition");
    m_program.addAttribute("vertexColor");
    m_program.addAttribute("vertexUV");
    m_program.linkShaders();
    m_spriteBatch.init();
    m_texture = 0;
    m_texture2 = 0;
    m_camera.init(size().width(), size().height());
    {
        QFile file(":/texture.png");
        file.open((QIODevice::ReadOnly));
        QByteArray byteArray = file.readAll();
        const unsigned char* imageData = (const unsigned char*)byteArray.data();
        size_t imageSize = byteArray.size();
        std::vector < unsigned char > outImage;
        unsigned long imageWidth = 0, imageHeight = 0;
        int result = decodePNG(outImage, imageWidth, imageHeight, imageData, imageSize, true);
        if (result != 0) {
            std::cout << "failed to decode PNG" << std::endl;
        }
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, outImage.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }*/
}
void GLWidget::updateWidget(){
/*    m_camera.update();*/

}

void GLWidget::paintGL(){
    /*updateWidget();


    glViewport(0, 0, this->size().width(), this->size().height());
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    m_program.use();

    static float time = 0.5f;
    static bool reversed = false;

    GLint timeUniform = m_program.getUniformLocation("time");
    if (timeUniform == GL_INVALID_INDEX) {
        std::cout << "time Error" << std::endl;
    }
    glUniform1f(timeUniform, time);
    if (reversed)time -= 0.01f;
    else time += 0.01;
    if (time >= 1.0f) {
       reversed = true;
       time -= 0.01f;
    }
    else if (time <= 0.5f) {
        reversed = false;
       time += 0.01f;
    }


   GLint textureUniform =  m_program.getUniformLocation("mySampler");
   if (textureUniform == GL_INVALID_INDEX) {
       std::cout << "sampler Error" << std::endl;
   }
   glUniform1i(textureUniform, 0);
   glActiveTexture(GL_TEXTURE0);

   glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
   GLint pUniform = m_program.getUniformLocation("P");
   glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

   m_spriteBatch.begin();
   m_spriteBatch.draw(glm::vec4(50, 50, 200, 200), glm::vec4(0, 0, 1, 1), m_texture, 0, White);
   m_spriteBatch.end();
   m_spriteBatch.renderBatch();

  m_program.unuse();*/

}


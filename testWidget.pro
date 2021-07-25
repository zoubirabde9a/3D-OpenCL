#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T19:36:35
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testWidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    picoPng.cpp \
    widget3d.cpp \
    opencl.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    picoPng.h \
    widget3d.h \
    entity.h \
    transform.h \
    opencl.h

LIBS += -L"$$PWD/lib/" -lZEngine -lglew32 -lOpenCL
INCLUDEPATH += "$$PWD/include/"

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

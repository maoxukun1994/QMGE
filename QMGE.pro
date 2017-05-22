#-------------------------------------------------
# Project created by QtCreator 2017-05-11T14:07:00
# Please Use QT 5.8 or greater for this project
#-------------------------------------------------

CONFIG += c++11
QT += core gui widgets
TARGET = QMGE
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/qmge_glwindow.cpp \
    src/qmge_app.cpp \
    src/qmge_renderer.cpp \
    src/qmge_glbatch.cpp \
    src/qmge_glshaderprogram.cpp \
    src/qmge_gluniformmanager.cpp \
    src/qmge_glcamera.cpp \
    src/qmge_sceneobject.cpp \
    src/qmge_fpscameracontroller.cpp \
    src/qmge_uniformprovider.cpp \
    massiveground/massiveground.cpp \
    massiveground/chunkmanager.cpp \
    massiveground/chunk.cpp

HEADERS += \
    src/qmge_global.h \
    src/qmge_glwindow.h \
    src/qmge_app.h \
    src/qmge_renderer.h \
    src/qmge_glbatch.h \
    src/qmge_glshaderprogram.h \
    src/qmge_gluniformmanager.h \
    src/qmge_glcamera.h \
    src/qmge_sceneobject.h \
    src/qmge_fpscameracontroller.h \
    src/qmge_uniformprovider.h \
    massiveground/massiveground.h \
    massiveground/chunkmanager.h \
    massiveground/chunk.h

DISTFILES += \
    README.md \
    MEMO.md

RESOURCES += \
    resource.qrc

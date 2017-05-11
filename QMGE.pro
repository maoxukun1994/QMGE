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
    src/qmge_renderer.cpp

HEADERS += \
    src/qmge_glwindow.h \
    src/qmge_includes.h \
    src/qmge_app.h \
    src/qmge_renderer.h

DISTFILES += \
    README.md \
    issues.txt

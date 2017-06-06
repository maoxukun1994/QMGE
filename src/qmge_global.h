#ifndef QMGE_GLOBAL_H
#define QMGE_GLOBAL_H

//std c++
#include <typeinfo>

//QtCore Header File
#include <QList>
#include <QFile>
#include <QHash>
#include <QtMath>
#include <QVector>
#include <QThread>
#include <QTextStream>
#include <QMutexLocker>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QCoreApplication>

//QtGui Header File
#include <QWindow>
#include <QMouseEvent>
#include <QApplication>
#include <QGuiApplication>
//QtGui 3d math
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
//QtGui OpenGL headers
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLContext>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>

#endif // QMGE_GLOBAL_H

//#define OPENGL_DESKTOP_4_5
#define OPENGL_DESKTOP_4_4


#ifdef OPENGL_DESKTOP_4_5
#include <QOpenGLFunctions_4_5_Core>
#endif


#ifdef OPENGL_DESKTOP_4_4
#include <QOpenGLFunctions_4_4_Core>
#endif

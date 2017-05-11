#include "qmge_glwindow.h"


namespace QMGE_Core
{

QMGE_GLWindow::QMGE_GLWindow(QSurfaceFormat contextSettings):QWindow()
{
    m_isInitialized = false;
    m_updatePending = false;
    m_isFullScreen = false;

    setSurfaceType(QSurface::OpenGLSurface);
    setFormat(contextSettings);
}

QMGE_GLWindow::~QMGE_GLWindow()
{

}

void QMGE_GLWindow::render()
{

}

void QMGE_GLWindow::init()
{
    //check
    if(m_isInitialized)
    {
        return;
    }

    //create an QOpenGLContext Object
    if(m_context.isNull())
    {
        m_context.reset(new QOpenGLContext());
    }
    //set requested format
    m_context->setFormat(this->requestedFormat());
    //and create & check
    if(m_context->create() && m_context->isValid())
    {
        //initialize functions
        m_context->makeCurrent(this);

        qDebug()<<"Requested :";
        qDebug()<<requestedFormat().renderableType();
        qDebug()<<requestedFormat().version()<<requestedFormat().profile();
        qDebug()<<"Got :";
        qDebug()<<m_context->format().renderableType();
        qDebug()<<m_context->format().version()<<m_context->format().profile();
    }
    else
    {
        qFatal("Failed to create OpenGL(ES) context");
    }

}



bool QMGE_GLWindow::event(QEvent *event)
{

}

void QMGE_GLWindow::exposeEvent(QExposeEvent *event)
{

}

void QMGE_GLWindow::mouseMoveEvent(QMouseEvent * e)
{

}

void QMGE_GLWindow::mousePressEvent(QMouseEvent * e)
{

}

void QMGE_GLWindow::mouseReleaseEvent(QMouseEvent * e)
{

}

void QMGE_GLWindow::keyPressEvent(QKeyEvent * e)
{

}

void QMGE_GLWindow::keyReleaseEvent(QKeyEvent * e)
{

}

}
//namespace QMGE_Core

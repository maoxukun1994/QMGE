#include "qmge_glwindow.h"


namespace QMGE_Core
{

QMGE_GLWindow::QMGE_GLWindow(QSurfaceFormat contextSettings):QWindow()
{
    setSurfaceType(QSurface::OpenGLSurface);
    setFormat(contextSettings);
}

QMGE_GLWindow::~QMGE_GLWindow()
{
}

//slot ,do actual close
void QMGE_GLWindow::safeClose()
{
    this->destroy();
}

bool QMGE_GLWindow::event(QEvent * event)
{
    switch(event->type())
    {
    case QEvent::Close:
        //catch close signal and inform other thread that the window
        //is going to close,and wait for them to be ready.
        //close is done in safeClose()
        emit stopRenderThread();
        event->ignore();
        return true;
    default:
        return QWindow::event(event);
    }
}

void QMGE_GLWindow::exposeEvent(QExposeEvent * event)
{
    Q_UNUSED(event);
    if(isExposed())
    {
        emit startRenderThread();
    }
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

void QMGE_GLWindow::resizeEvent(QResizeEvent * e)
{

}

}
//namespace QMGE_Core

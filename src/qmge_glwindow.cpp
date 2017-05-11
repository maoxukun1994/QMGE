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

bool QMGE_GLWindow::event(QEvent * event)
{
    switch(event->type())
    {
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

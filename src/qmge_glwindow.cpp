#include "qmge_glwindow.h"


namespace QMGE_Core
{

QMGE_GLWindow::QMGE_GLWindow(QSurfaceFormat contextSettings):QWindow()
{
    setSurfaceType(QSurface::OpenGLSurface);
    setFormat(contextSettings);

    m_mousePressed = false;
}

QMGE_GLWindow::~QMGE_GLWindow()
{
}

void QMGE_GLWindow::postRendererInitialized()
{
    emit resized(this->width(),this->height());
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
    if(m_mousePressed)
    {
        emit mouseMoved(e->y()-m_lastMousePos.y(),e->x()-m_lastMousePos.x());
        m_lastMousePos = e->pos();
    }
}

void QMGE_GLWindow::mousePressEvent(QMouseEvent * e)
{
    m_lastMousePos = e->pos();
    m_mousePressed = true;
}

void QMGE_GLWindow::mouseReleaseEvent(QMouseEvent * e)
{
    m_lastMousePos = e->pos();
    m_mousePressed = false;
}

void QMGE_GLWindow::keyPressEvent(QKeyEvent * e)
{
    emit keyChanged(e->key(),true);
}

void QMGE_GLWindow::keyReleaseEvent(QKeyEvent * e)
{
    emit keyChanged(e->key(),false);
}

void QMGE_GLWindow::resizeEvent(QResizeEvent * e)
{
    emit resized(e->size().width(),e->size().height());
}

}
//namespace QMGE_Core

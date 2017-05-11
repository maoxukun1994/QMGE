#include "qmge_renderer.h"
#include "qmge_glwindow.h"

namespace QMGE_Core
{

QMGE_Renderer::QMGE_Renderer(QSurfaceFormat contextSettings,QMGE_GLWindow * parent):QObject(),QOpenGLExtraFunctions()
{
    m_isInitialized = false;
    m_updatePending = false;

    m_contextSettings = contextSettings;
    m_renderWindow = parent;

    m_frames = 0;
}


QMGE_Renderer::~QMGE_Renderer()
{

}

void QMGE_Renderer::init()
{
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
    m_context->setFormat(m_contextSettings);
    //and create & check
    if(m_context->create() && m_context->isValid())
    {
        //initialize functions
        qDebug()<<"Requested :";
        qDebug()<<m_contextSettings.renderableType();
        qDebug()<<m_contextSettings.version()<<m_contextSettings.profile();
        qDebug()<<"Got :";
        qDebug()<<m_context->format().renderableType();
        qDebug()<<m_context->format().version()<<m_context->format().profile();
    }
    else
    {
        qFatal("Failed to create OpenGL(ES) context");
    }

    if(!m_context->makeCurrent(m_renderWindow))
    {
        qFatal("Couldn't make context current against render window.");
    }

    //initialize OpenGL functions for the context
    initializeOpenGLFunctions();

    m_isInitialized = true;

    //prepare the render scene .etc
    //...
}

void QMGE_Renderer::renderOnce()
{
    m_frames++;
    qDebug()<<m_frames;
    if(m_renderWindow->isExposed())
    {
        //first make current
        m_context->makeCurrent(m_renderWindow);

        glClearColor(0.2f,0.5f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        //swap buffer
        m_context->swapBuffers(m_renderWindow);

        //post next request
        renderLater();
    }
    else
    {
        //when window not visible or available,stop the event loop
        m_context->doneCurrent();
        emit stopExec();
    }
}

void QMGE_Renderer::renderLater()
{
    if (!m_updatePending)
    {
        m_updatePending = true;
        QCoreApplication::postEvent(this,new QEvent(QEvent::UpdateRequest),Qt::EventPriority::HighEventPriority);
    }
}

bool QMGE_Renderer::event(QEvent * event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        m_updatePending = false;
        renderOnce();
        return true;
    default:
        return QObject::event(event);
    }
}

void QMGE_Renderer::execRender()
{
    if(!m_isInitialized)
    {
        init();
    }
    renderLater();
}

}
//namespace QMGE_Core

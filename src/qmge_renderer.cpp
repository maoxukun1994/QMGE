#include "qmge_renderer.h"

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

void QMGE_Renderer::postInit()
{
    //initialize resources
    GLfloat verts[] =
    {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    GLfloat tuvs[] =
    {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,0.0f, 1.0f,0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,0.0f, 1.0f,0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f,0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f,0.0f, 1.0f
    };

    batch.reset(new QMGE_GLBatch());
    batch->enableBatchVertexAttrib(VA_POSITION);
    batch->enableBatchVertexAttrib(VA_TUV_0);
    batch->setVertexData(verts,36,VA_POSITION);
    batch->setVertexData(tuvs,36,VA_TUV_0);

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
        qDebug()<<"Requested context:";
        qDebug()<<m_contextSettings.renderableType();
        qDebug()<<"OpenGL"<<m_contextSettings.version()<<m_contextSettings.profile();
        qDebug()<<"Got context:";
        qDebug()<<m_context->format().renderableType();
        qDebug()<<"OpenGL"<<m_context->format().version()<<m_context->format().profile();
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
    postInit();
}

void QMGE_Renderer::renderOnce()
{
    m_frames++;
    if(m_renderWindow->isExposed())
    {
        //first make current
        m_context->makeCurrent(m_renderWindow);

        //do actual rendering
        glClearColor(0.2f,0.5f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        //swap buffer
        m_context->swapBuffers(m_renderWindow);
        //post next request
        renderLater();
    }
    else
    {
        //when window not visible or available,not continue post updateRequest
        m_context->doneCurrent();
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

void QMGE_Renderer::cleanUp()
{
    //do clean up jobs
    //...
    //...

    batch.reset();
    program.reset();
    texture.reset();

    //delete openGL context
    //need to put behind all opengl-related resource cleaning-up
    if(!m_context.isNull())
    {
        m_context->doneCurrent();
        m_context.reset();
    }
    //last emit readyToStop signal
    emit readyToStop();
}

}
//namespace QMGE_Core

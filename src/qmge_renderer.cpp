#include "qmge_renderer.h"

namespace QMGE_Core
{

QMGE_Renderer::QMGE_Renderer(QSurfaceFormat contextSettings,QMGE_GLWindow * parent):QObject(),QOpenGLExtraFunctions()
{
    m_isInitialized = false;
    m_updatePending = false;

    m_contextSettings = contextSettings;

    if(parent == nullptr)
    {
        qFatal("Can not create a render without a valid QMGE_GLWindow.");
    }
    else
    {
        m_renderWindow = parent;
    }

    m_frames = 0;
}


QMGE_Renderer::~QMGE_Renderer()
{
}

void QMGE_Renderer::postInit()
{
    /**
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
    **/

    /*
    GLfloat verts[] =
    {
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        1.0f,1.0f,0.0f
    };

    GLfloat tuvs[] =
    {
        0.0f,0.0f,
        1.0f,0.0f,
        0.0f,1.0f,
        1.0f,1.0f
    };

    GLuint indices[] =
    {
        0,1,2,2,1,3
    };

    testBatch.reset(new QMGE_GLBatch());
    testBatch->enableBatchVertexAttrib(VA_POSITION);
    testBatch->enableBatchVertexAttrib(VA_TUV_0);
    testBatch->setVertexData(verts,4,VA_POSITION);
    testBatch->setVertexData(tuvs,4,VA_TUV_0);
    testBatch->setIndexData(indices,6);
    */

    camera.reset(new QMGE_GLCamera());
    camera->setPosition(32,32,64);
    camera->setPitch(-45.0f);
    camera->setPerspective(75.0f,1.788f,0.1f,1000.0f);
    QMGE_GLUniformManager::getInstance()->registerUniform("vMatrix",QMatrix4x4(),camera->m_vMatrix);
    QMGE_GLUniformManager::getInstance()->registerUniform("pMatrix",QMatrix4x4(),camera->m_pMatrix);
    camcontrol.reset(new QMGE_FPSCameraController(camera.data()));

    m_chunkManager.reset(new ChunkManagerTS());
    m_chunkManager->loadMap(":/textures/hm.png");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f,0.5f,0.8f,1.0f);

#ifdef OPENGL_DESKTOP
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();
    f->glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
#endif

}


//for user implemented render
void QMGE_Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    camcontrol->updateCam(0.016f);
    camera->updateV();
    m_chunkManager->move(camera->getTransform().position);
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

    //inform the window that renderer is ready
    emit initialized();
}

void QMGE_Renderer::renderOnce()
{
    m_frames++;
    if(m_renderWindow->isExposed())
    {
        //first make current
        m_context->makeCurrent(m_renderWindow);

        //do actual rendering
        render();

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

//used for changing glviewport when renderwindow changed size.
void QMGE_Renderer::resizeGL(int w, int h)
{
    if(m_isInitialized)
    {
        glViewport(0,0,w,h);
    }
}

void QMGE_Renderer::windowKeyChanged(int key,bool pressed)
{
    //register key change event
    camcontrol->move(key,pressed);
}

void QMGE_Renderer::windowMouseMoved(int deltax,int deltay)
{
    //register mouse move event
    camcontrol->rotate(-(float)deltax * 0.2,-(float)deltay * 0.2,0);
}

void QMGE_Renderer::cleanUp()
{
    //do clean up jobs
    //...
    //...

    m_chunkManager.reset();

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
